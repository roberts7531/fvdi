SUBDIRS = \
	aranym \
	bitplane \
	16_bit \
	uaegfx \
	saga \
	firebee \
	$(empty)

# SUBDIRS += radeon

all clean install::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@; done

