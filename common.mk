#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

ifeq ($(GC),1)
include $(abspath $(dir $(firstword $(MAKEFILE_LIST))))/gamecube.mk
else
include $(abspath $(dir $(firstword $(MAKEFILE_LIST))))/wii.mk
endif

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
SOURCES		:=	main
DATA		:=	data
INCLUDES	:=

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS		=	-std=gnu23 -g -O3 -fexec-charset=CP437 -Wall -DVERSION=\"$(VERSION)\" $(MACHDEP) $(INCLUDE) --embed-dir=$(CURDIR)/../$(DATA)
CXXFLAGS	=	$(CFLAGS)

LDFLAGS		=	-g $(MACHDEP) -Wl,-Map,$(notdir $@).map

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:= $(PORTLIBS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))

RAW_PNGS	:=	$(foreach dir,$(DATA),$(wildcard $(dir)/*.png))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(sFILES:.s=.o) $(SFILES:.S=.o)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES), -iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) \
					-I$(LIBOGC_INC)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:= -L$(LIBOGC_LIB) $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean optimize

#---------------------------------------------------------------------------------
$(BUILD): optimize
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/common.mk

optimize: $(RAW_PNGS)

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).dol: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

-include $(DEPENDS)

define png-optimize
	@errcount=0; \
	rm -f "$(1)-tmp"; \
	echo "file: '$(1)'"; \
	echo ""; \
	echo "step 1: oxipng"; \
	oxipng "$(1)" -aqomax --strip all --scale16 || errcount=$$(($$errcount + 1)); \
	echo ""; \
	if [ "$$errcount" -eq "0" ]; then \
		echo "step 2: pngquant"; \
		pngquant "$(1)" --speed 1 -f --ext .png-tmp || errcount=$$(($$errcount + 1)); \
		echo ""; \
		if [ "$$errcount" -eq "0" ]; then \
			echo "step 3: oxipng (again)"; \
			oxipng "$(1)-tmp" -aqomax --strip all --scale16 || errcount=$$(($$errcount + 1)); \
			echo ""; \
		fi; \
	fi; \
	if [ "$$errcount" -eq "0" ] && [ -f "$(1)-tmp" ]; then \
		size1=$$(wc -c < "$(1)-tmp" | tr -d ' '); \
		size2=$$(wc -c < "$(1)" | tr -d ' '); \
		diff=$$(($$size1 - $$size2)); \
		awk "BEGIN {printf \"\n%+.2f%% ($$diff bytes) difference\n\n\", ($$diff / $$size2) * 100}"; \
		if [ "$$size1" -ge "$$size2" ]; then \
			echo "$(1)-tmp >= original file size, discarding..."; \
			rm -f "$(1)-tmp"; \
		else \
			echo "$(1)-tmp is smaller!! replacing original :D"; \
			mv -f "$(1)-tmp" "$(1)"; \
		fi; \
		echo ""; \
	fi; \
	if [ "$$errcount" -ne "0" ]; then \
		echo "optimization failed for $(1)"; \
	fi
endef

%.png: FORCE
	@$(call png-optimize,$@)

FORCE:

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
