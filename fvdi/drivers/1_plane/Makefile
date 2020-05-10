#!make -f
#
# This software is licensed under the GNU General Public License.
# Please, see LICENSE.TXT for further information.
#

#
# gcc >= 2.95.3 (sparemint) version
#

TARGET = 1_plane.sys

all: $(TARGET)

top_srcdir = ../..

OPTS = -O2 -fomit-frame-pointer -mshort
include $(top_srcdir)/CONFIGVARS

CFLAGS  = $(CPUOPTS) $(OPTS) $(WARNINGS) -I$(top_srcdir)/include -I$(top_srcdir)/drivers/include
ASFLAGS = $(CPUOPTS) -I$(top_srcdir)/include -I$(top_srcdir)/drivers/include

SINCSRC = \
	$(top_srcdir)/include/macros.inc \
	$(top_srcdir)/include/types.inc \
	$(top_srcdir)/include/vdi.inc \
	$(top_srcdir)/drivers/include/linea.inc \
	$(top_srcdir)/drivers/include/pixelmac.inc \
	1_expand.inc \
	1_fill.inc \
	1_blit.inc

SSRC = \
	../common/c_common.s \
	../common/common.s \
	../common/clip.s \
	../common/lineachk.s \
	1_blit.s \
	1_expand.s \
	1_fill.s \
	1_line.s \
	1_mouse.s \
	1_pixel.s \
	1_text.s

CSRC = \
	../common/init.c \
	1_spec.c

# handle DevPac -> gas conversions
# defines SSRC_GNU and SINCSRC_GNU variables
include $(top_srcdir)/utility/conv2gas/RULES

# need to convert the .inc file as well
$(SSRC_GNU): $(SINCSRC_GNU)

# engine binary objects
OBJECTS = $(SSRC_GNU:.gnu=.gnu.o) $(CSRC:.c=.o)

$(TARGET): depend $(OBJECTS) $(LIBFILES)
	$(AM_V_LD)$(LD) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBS)

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

clean:: clean_gnu
	$(RM) $(OBJECTS) depend $(TARGET)

-include depend
