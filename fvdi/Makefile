M68K_ATARI_MINT_CROSS := yes

# set CPU and DEBUG if not passed in already
ifeq ($(CPU),)
CPU=020
endif

ifeq ($DEBUG,)
DEBUG=yes
endif

SUBDIRS := \
	modules/ft2 \
	engine \
	drivers/bitplane \
	drivers/firebee

# SUBDIRS += drivers/radeon

TARGETS := all clean

.PHONY: all clean $(SUBDIRS)

$(TARGETS): $(SUBDIRS)

$(SUBDIRS):
	M68K_ATARI_MINT_CROSS=$(M68K_ATARI_MINT_CROSS) CPU=$(CPU) DEBUG=$(DEBUG) $(MAKE) -C $@ $(MAKECMDGOALS)

