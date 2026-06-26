.PHONY: clean gba gamecube wii

VERSION	:= 5.8.0

CURDIR = $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

all: gamecube .WAIT wii

gamecube:
	@echo Building GameCube version
	@$(MAKE) --no-print-directory -f $(CURDIR)/common.mk GC=1 VERSION=$(VERSION)

wii:
	@echo
	@echo Building Wii version
	@$(MAKE) --no-print-directory -f $(CURDIR)/common.mk VERSION=$(VERSION)

clean:
	@cd $(CURDIR)
	@rm -rf build-* *.elf *.dol
	@echo i\'m awesome clean now :\)

release:
	@git diff-files --quiet --ignore-submodules || { echo "ERROR: Unstaged files"; exit 1; }
	@git fetch && if git status -sb | grep -q "ahead"; then echo "ERROR: Unpushed commits"; exit 1; else echo "everything checks out :)"; fi
	@cd $(CURDIR)
	@echo Cleaning, building
	$(MAKE) clean
	@echo
	$(MAKE)
	@echo
	$(CURDIR)/genmeta.sh $(VERSION) > meta.xml
	@echo
	@echo Zipping release
	@mkdir -p korbodonut
	@cp boot.dol meta.xml icon.png ./korbodonut/
	@cp donut-gc.elf donut-gc-debug.elf
	@cp boot.elf donut-wii-debug.elf
	@zip -rv donut ./korbodonut/
	@echo
	nano -L temp.md
	@printf "\n\nThe debug ELFs have no extra features other than debug info embedded into them." >> temp.md
	gh release create v$(VERSION) -t v$(VERSION) donut.zip donut-gc.dol donut-wii-debug.elf donut-gc-debug.elf -F temp.md
	@rm -rf ./korbodonut/ donut-wii-debug.elf donut-gc-debug.elf donut.zip temp.md
