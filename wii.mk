include $(DEVKITPRO)/libogc2/wii_rules

BUILD		:=	build-wii

CFLAGS		=	-g -O2 -fexec-charset=CP437 -Wall -DVERSION=\"$(VERSION)\" $(MACHDEP) $(INCLUDE)
CXXFLAGS	=	$(CFLAGS)

LIBS		:=	-lwiiuse -lbte -lgrrmod -lgrrlib -lpngu `$(PREFIX)pkg-config freetype2 libpng libjpeg --libs` -lfat -laesnd -logc -lm

LIBDIRS	:= $(PORTLIBS)

export OUTPUT	:=	$(abspath $(dir $(firstword $(MAKEFILE_LIST))))/boot
