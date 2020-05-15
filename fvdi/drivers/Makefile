SUBDIRS =

ifeq ($(CPU),v4e)

SUBDIRS += firebee
# SUBDIRS += radeon

else

SUBDIRS = \
	aranym \
	bitplane \
	16_bit \
	uaegfx \
	saga \
	$(empty)

endif

all clean install::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@ || exit 1; done

