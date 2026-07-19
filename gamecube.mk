include $(DEVKITPPC)/gamecube_rules

BUILD		:=	build-gc

LIBS		:=	-lgrrmod-mpt -lopenmpt -lvorbisfile -lvorbis -logg -lmpg123 -lstdc++ -lgrrlib -lpngu `$(PREFIX)pkg-config freetype2 libpng libjpeg --libs` -lfat -laesnd -logc -lm

export OUTPUT	:=	$(abspath $(dir $(firstword $(MAKEFILE_LIST))))/donut-gc
