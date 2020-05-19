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

$(TARGET): $(OBJECTS) $(LIBFILES)
	$(AM_V_LD)$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)

include $(top_srcdir)/DEPENDENCIES

strip:
	$(STRIP) --strip-all $(TARGET)

clean::
	$(RM) $(OBJECTS) $(TARGET)

install::
	cp -a $(TARGET) "$(DESTDIR)/fvdi.acc"
