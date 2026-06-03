include $(DEVKITPRO)/libogc2/gamecube_rules

BUILD		:=	build-gc

CFLAGS		=	-g -O2 -fexec-charset=CP437 -Wall -DGC -DVERSION=\"$(VERSION)\" $(MACHDEP) $(INCLUDE)
CXXFLAGS	=	$(CFLAGS)

LIBS		:=	-lfat -lgrrmod -laesnd -lgrrlib -lpngu `$(PREFIX)pkg-config freetype2 libpng libjpeg --libs` -lfat -laesnd -logc -lm

LIBDIRS	:= $(PORTLIBS)

export OUTPUT	:=	$(abspath $(dir $(firstword $(MAKEFILE_LIST))))/donut-gc
