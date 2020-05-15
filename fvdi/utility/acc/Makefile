#!make -f
#
# This software is licensed under the GNU General Public License.
# Please, see LICENSE.TXT for further information.
#

#
# gcc >= 2.95.3 (sparemint) version
#

TARGET = fvdiagnu.acc

all: $(TARGET)

top_srcdir = ../..

include $(top_srcdir)/CONFIGVARS

CFLAGS  = $(CPUOPTS) $(OPTS) $(WARNINGS) -I$(top_srcdir)/include -D__GEMLIB_OLDNAMES -DNEW_GEMLIB

LIBS += -lgem

CSRC = \
	fvdiacc.c \
	dloghndl.c \
	dlogtble.c \
	dodialog.c \
	objcedit.c \
	popup.c \
	wind.c

# engine binary objects
ifeq ($(CPU),v4e)
OBJECTS = $(SSRC_GNU_CF:.s=.o)
else
OBJECTS = $(SSRC_GNU:.s=.o)
endif
OBJECTS += $(CSRC:.c=.o)

$(TARGET): depend $(OBJECTS) $(LIBFILES)
	$(AM_V_LD)$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)

depend: $(CSRC)
#	"$(CC) -MM" does not seem able to keep track of the directory of the
#	object file, so dependencies of files outside this directory are
#	never taken into account later. makedepend does this correctly.
#	Unfortunately, makedepend includes system headers in its
#	dependency lists, but you can't get everything...
#	$(CC) -MM $(CFLAGS) $(CSRC) > depend
	$(AM_V_at)makedepend -f- -- $(CFLAGS) $(CSRC) > depend 2> /dev/null
	$(AM_V_at)sed "s#\/usr[^ ]*\.h##g" < depend  > depend2
	$(AM_V_at)sed "s#^.*: *\$$##"      < depend2 > depend
	$(AM_V_at)$(RM) depend2

strip:
	$(STRIP) --strip-all $(TARGET)

clean::
	$(RM) $(OBJECTS) depend $(TARGET)

install::
	cp -a $(TARGET) "$(DESTDIR)/fvdi.acc"

-include depend
