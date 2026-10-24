.PHONY: clean gba gamecube wii

VERSION	:= 8.4.1

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
	@echo "i'm awesome clean now. : )"

release:
	@git diff-files --quiet --ignore-submodules || { echo "ERROR: Unstaged files"; exit 1; }
	@git fetch && if git status -sb | grep -q "ahead"; then echo "ERROR: Unpushed commits"; exit 1; else echo "everything checks out : )"; fi
	@cd $(CURDIR)
	@echo Cleaning, building
	$(MAKE) clean
	@echo
	$(MAKE)
	@echo
	$(CURDIR)/genmeta.sh $(VERSION) > meta.xml
	@echo
	@echo Zipping release
	@mkdir -p ./korbo-donut/music
	@cp boot.dol meta.xml icon.png ./korbo-donut/
	@cp boot.elf donut-wii-debug.elf
	@zip -rv donut-wii ./korbo-donut/
	@rm -rf ./korbo-donut
	@mkdir -p ./korbo-donut
	@cp default.dol opening.bnr ./korbo-donut/
	@cp default.elf donut-gamecube-debug.elf
	@zip -rv donut-gamecube ./korbo-donut/
	@echo
	@rm -f temp.md
	nano -L temp.md
	@printf "\n\nThe debug ELFs have no extra features other than debug info embedded into them." >> temp.md
	gh release create v$(VERSION) -t v$(VERSION) donut-wii.zip donut-gamecube.zip donut-wii-debug.elf donut-gamecube-debug.elf -F temp.md
	@rm -rf ./korbo-donut/ donut-wii-debug.elf donut-gamecube-debug.elf donut-wii.zip donut-gamecube.zip temp.md
