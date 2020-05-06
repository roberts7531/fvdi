SUBDIRS := \
	utility/conv2gas \
	utility/structer \
	modules/ft2 \
	engine \
	drivers \
	$(empty)

all clean install::
	for i in $(SUBDIRS); do $(MAKE) -C $$i $@ || exit 1; done
