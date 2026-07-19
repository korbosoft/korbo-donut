include $(DEVKITPPC)/wii_rules

BUILD		:=	build-wii

LIBS		:=	-lwiiuse -lbte -lgrrmod-mpt -lopenmpt -lvorbisfile -lvorbis -logg -lmpg123 -lstdc++ -lgrrlib -lpngu `$(PREFIX)pkg-config freetype2 libpng libjpeg --libs` -lfat -laesnd -logc -lm

export OUTPUT	:=	$(abspath $(dir $(firstword $(MAKEFILE_LIST))))/boot
