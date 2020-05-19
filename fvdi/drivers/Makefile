SUBDIRS =

V4E_SUBDIRS =

V4E_SUBDIRS += firebee
# V4E_SUBDIRS += radeon

DRV_SUBDIRS = \
	aranym \
	bitplane \
	16_bit \
	uaegfx \
	saga \
	$(empty)

ifeq ($(CPU),v4e)

SUBDIRS += $(V4E_SUBDIRS)

else

SUBDIRS += $(DRV_SUBDIRS)

endif

all install::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@ || exit 1; done

clean::
	for i in $(V4E_SUBDIRS) $(DRV_SUBDIRS); do $(MAKE) -C $$i $@ || exit 1; done

