SUBDIRS := \
	modules/ft2 \
	engine \
	drivers/bitplane \
	drivers/firebee

# SUBDIRS += drivers/radeon

all clean install::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@ || exit 1; done
