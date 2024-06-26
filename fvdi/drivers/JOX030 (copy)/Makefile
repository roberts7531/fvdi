#!make -f
#
# This software is licensed under the GNU General Public License.
# Please, see LICENSE.TXT for further information.
#

#
# gcc >= 2.95.3 (sparemint) version
#

TARGET = v9958.sys

all: $(TARGET)

top_srcdir = ../..

OPTS    = -mshort -O2 -fomit-frame-pointer
include $(top_srcdir)/CONFIGVARS

CFLAGS  = $(CPUOPTS) $(OPTS) $(WARNINGS) -I$(top_srcdir)/include -I$(top_srcdir)/drivers/include

SINCSRC = \
	$(top_srcdir)/drivers/bitplane/1_expand.inc

SSRC = \
	../common/c_common.s \
	../common/common.s \
	../common/clip.s

CSRC = \
	v99x8.c \
	init.c \
	printf.c \
	v9958_spec.c \
	v9958_scr.c \
	v9958_pal.c \
	v9958_mouse.c \
	v9958_fill.c \
	v9958_blit.c


# handle DevPac -> gas conversions
# defines SSRC_GNU and SINCSRC_GNU variables
include $(top_srcdir)/utility/conv2gas/RULES

SINCSRC_GNU += \
	$(top_srcdir)/include/types.inc \
	$(top_srcdir)/include/macros.gnu \
	$(top_srcdir)/include/vdi.gnu \
	$(top_srcdir)/drivers/include/pixelmac.gnu

# engine binary objects
# Engine binary objects
ifeq ($(CPU),v4e)
OBJECTS = $(SSRC_GNU_CF:.s=.o)
else
OBJECTS = $(SSRC_GNU:.s=.o)
endif
OBJECTS += $(CSRC:.c=.o)

$(OBJECTS): $(SINCSRC_GNU)

$(TARGET): $(OBJECTS) $(LIBFILES)
	$(AM_V_LD)$(LD) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)

include $(top_srcdir)/DEPENDENCIES


strip:
	$(STRIP) --strip-all $(TARGET)

clean:: clean_gnu
	$(RM) $(OBJECTS) $(TARGET)

install::
	mkdir -p $(DESTDIR)/gemsys
	cp -a $(TARGET) $(DESTDIR)/gemsys/$(TARGET)
