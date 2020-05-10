SUBDIRS = \
	aranym \
	bitplane \
	16_bit \
	uaegfx \
	saga \
	$(empty)

SUBDIRS += firebee
# SUBDIRS += radeon

all clean install::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@ || exit 1; done

