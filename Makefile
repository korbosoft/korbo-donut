.PHONY: clean gba gamecube wii

VERSION	:=	v5.4.0

CURDIR = $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

all: gba .WAIT gamecube .WAIT wii

gba:
	@echo Building GBA Donut
	@$(MAKE) --no-print-directory -C $(CURDIR)/gba

gamecube:
	@echo
	@echo Building GameCube version
	@$(MAKE) --no-print-directory -f $(CURDIR)/common.mk GC=1 VERSION=$(VERSION)

wii:
	@echo
	@echo Building Wii version
	@$(MAKE) --no-print-directory -f $(CURDIR)/common.mk VERSION=$(VERSION)

clean:
	@rm -rf build-* *.elf *.dol
	@rm -rf build-* *.elf *.dol data/demo_mb.gba
	@echo i\'m awesome clean now :\)

release:
	@mkdir -p korbodonut
	@cp boot.dol meta.xml icon.png ./korbodonut/
	@cp donut-gc.elf donut-gc-debug.elf
	@cp boot.elf donut-wii-debug.elf
	@zip -rv donut ./korbodonut/
	@gh release create $(VERSION) donut.zip donut-gc.dol donut-wii-debug.elf donut-gc-debug.elf
	@rm -rf ./korbodonut/ donut-wii-debug.elf donut-gc-debug.elf donut.zip

