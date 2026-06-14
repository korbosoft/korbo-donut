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

CFLAGS		=	-g -O3 -fexec-charset=CP437 -Wall -DVERSION=\"$(VERSION)\" $(MACHDEP) $(INCLUDE)
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
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(sFILES:.s=.o) $(SFILES:.S=.o)
export OFILES := $(OFILES_BIN) $(OFILES_SOURCES)

export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES)))

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

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/common.mk

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).dol: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

$(OFILES_SOURCES) : $(HFILES)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .mod extension
#---------------------------------------------------------------------------------
%.mod.o	%_mod.h :	%.mod
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	$(bin2o)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .mod extension
#---------------------------------------------------------------------------------
%.gba.o	%_gba.h :	%.gba
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	$(bin2o)

-include $(DEPENDS)

define png-optimize
	@errcount=0; \
	rm -f *.png-tmp; \
	echo "file: '$(1)'"; \
	echo ""; \
	echo "step 1: oxipng"; \
	oxipng "$(1)" -aqomax --strip all --scale16 || errcount=$$(($$errcount + 1)); \
	if [ "$$errcount" -eq "0" ]; then \
		size1=$$(wc -c <"$(1)-tmp"); \
		size2=$$(wc -c <"$(1)"); \
		diff=$$(($$size1 - $$size2)); \
		awk "BEGIN {printf \"\n%+.2f%% ($$diff bytes) difference\n\n\", ($$diff / $$size2) * 100}"; \
		if [ $$size1 -ge $$size2 ]; then \
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
	else \
		echo "success :)"; \
	fi
endef

PNG_OFILES := $(filter %.png.o,$(OFILES_BIN))
define png-sequence-rule
$(word $(1),$(PNG_OFILES)): | $(word $(shell expr $(1) - 1),$(PNG_OFILES))
endef
$(foreach i,$(shell seq 2 $(words $(PNG_OFILES))),$(eval $(call png-sequence-rule,$(i))))

#---------------------------------------------------------------------------------
%.png.o	%_png.h :	%.png
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
# 	@$(call png-optimize,$<)
	@$(shell oxipng $< -aqomax --strip all --scale16)
	@$(bin2o)

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPSDIR)/*.d

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
