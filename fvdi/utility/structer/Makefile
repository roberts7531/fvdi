#
# Makefile for C struct to assembly equ converter
#
# Copyright 2002, Johan Klockars
# This software is licensed under the GNU General Public License.
# Please, see LICENSE.TXT for further information.
#

top_srcdir = ../..
srcdir     = .

include $(top_srcdir)/CONFIGVARS

CC              = $(NATIVE_CC)
CFLAGS		= $(NATIVE_CFLAGS) -DYYDEBUG=1
LDFLAGS		= -s
FLEXFLAGS	=
BISONFLAGS	=
LIBS		= 

TARGET		= structer
CSOURCES	= main.c expr.c hash.c list.c memory.c outequ.c \
		  lex.yy.c struct.tab.c
CHEADERS	= expr.h hash.h list.h memory.h misc.h struct.tab.h
OBJECTS		= $(CSOURCES:.c=.o)
FLEXOUT		= lex.yy.c
BISONOUT	= struct.tab.c struct.tab.h

all:		depend $(TARGET)

$(TARGET):		$(OBJECTS)
	$(AM_V_LD)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

depend:		$(CSOURCES) $(CHEADERS)
	$(AM_V_CC)$(CC) -MM $(CSOURCES) > depend

clean::
	$(RM) $(OBJECTS) $(FLEXOUT) $(BISONOUT) $(TARGET) depend

$(BISONOUT):	struct.y
	bison -d $(BISONFLAGS) struct.y

$(FLEXOUT):	struct.lex
	flex $(FLEXFLAGS) struct.lex

-include depend
