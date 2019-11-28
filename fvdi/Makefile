M68K_ATARI_MINT_CROSS := yes
CPU=020
DEBUG=yes

SUBDIRS := \
	modules/ft2 \
	engine \
	drivers/bitplane

TARGETS := all clean

.PHONY: all clean $(SUBDIRS)

$(TARGETS): $(SUBDIRS)

$(SUBDIRS):
	M68K_ATARI_MINT_CROSS=$(M68K_ATARI_MINT_CROSS) CPU=$(CPU) DEBUG=$(DEBUG) $(MAKE) -C $@ $(MAKECMDGOALS)

