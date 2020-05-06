top_srcdir = ../..
srcdir     = .

include $(top_srcdir)/CONFIGVARS

CONV2GAS = conv2gas

CFLAGS		= $(NATIVE_CFLAGS)
LDFLAGS		= -s

all: $(CONV2GAS)

$(CONV2GAS): conv2gas.c
	$(AM_V_CC)$(NATIVE_CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean::
	$(RM) $(CONV2GAS)

install::
	@:
