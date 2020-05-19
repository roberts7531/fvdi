#!make -f
#
# This software is licensed under the GNU General Public License.
# Please, see LICENSE.TXT for further information.
#

#
# gcc >= 2.95.3 (sparemint) version
#

TARGET = radeon.sys

all: $(TARGET)

top_srcdir = ../..

CPU     = v4e
OPTS    = -mshort -O2 -fomit-frame-pointer
include $(top_srcdir)/CONFIGVARS

BAS_INCLUDE = $(top_srcdir)/../../BaS_gcc/include

CFLAGS  = $(CPUOPTS) $(OPTS) $(WARNINGS) -I$(BAS_INCLUDE) -I$(top_srcdir)/include -I$(top_srcdir)/drivers/include
ASFLAGS = $(CPUOPTS) -I$(top_srcdir)/include -I$(top_srcdir)/drivers/include

SINCSRC = \
	$(top_srcdir)/drivers/bitplane/1_expand.inc

SSRC = \
	../common/c_common.s \
	../common/common.s \
	../common/clip.s \
	../common/lineachk.s

CSRC = \
	init.c \
	colours.c \
	printf.c \
	radeon_bas_interface.c \
	radeon_spec.c \
	radeon.c \
	modeline_vesa.c \
	radeon_pll.c \
	radeon_mouse.c \
	16_bit.c

# handle DevPac -> gas conversions
# defines SSRC_GNU and SINCSRC_GNU variables
include $(top_srcdir)/utility/conv2gas/RULES

SINCSRC_GNU += \
	$(top_srcdir)/include/types.inc \
	$(top_srcdir)/include/macros.gnu \
	$(top_srcdir)/include/vdi.gnu \
	$(top_srcdir)/drivers/include/linea.gnu \
	$(top_srcdir)/drivers/include/pixelmac.gnu

# engine binary objects
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
	$(RM) *.o $(TARGET) *.inf *.ino

install::
	mkdir -p $(DESTDIR)/gemsys
	cp -a $(TARGET) $(DESTDIR)/gemsys/$(TARGET)
	cp -a fvdi.sys "$(DESTDIR)/fvdirad.sys"
