.PHONY: clean gba gamecube wii

VERSION	:=	v5.5.1

CURDIR = $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

all: gamecube .WAIT wii

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
	@rm -rf build-* *.elf *.dol
	@echo i\'m awesome clean now :\)

release:
	@mkdir -p korbodonut
	@cp boot.dol meta.xml icon.png ./korbodonut/
	@cp donut-gc.elf donut-gc-debug.elf
	@cp boot.elf donut-wii-debug.elf
	@zip -rv donut ./korbodonut/
	@gh release create $(VERSION) donut.zip donut-gc.dol donut-wii-debug.elf donut-gc-debug.elf
	@rm -rf ./korbodonut/ donut-wii-debug.elf donut-gc-debug.elf donut.zip

