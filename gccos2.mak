#
#########################################################################
#
# makefile for The Hessling Editor (THE)
# Innotek GCC OS/2
#
# You need the following environment variable set like:
# THE_SRCDIR=c:\the
# PDCURSES_SRCDIR=c:\pdcurses
# PDCURSES_BINDIR=c:\pdcurses
# REXXTRANS_SRCDIR=c:\rexxtrans
# REXXTRANS_BINDIR=c:\rexxtrans\gcc
#
#########################################################################
#

ifeq ($(AOUT),Y)
LIBEXT=a
OBJCFLAG=
OBJLDFLAG=
OBJLDFLAGOPT=
OBJTYPE=o
BIND=emxbind the
LXLITE=
else
LIBEXT=lib
OBJCFLAG=-Zomf
OBJLDFLAG=-Zomf
OBJLDFLAGOPT=-s
OBJTYPE=obj
BIND=
LXLITE=lxlite the.exe
endif

SRC       = $(THE_SRCDIR)
CURSBIN   = $(PDCURSES_BINDIR)
CURSLIB   = $(PDCURSES_BINDIR)\curses.$(LIBEXT)
CURSINC   = -I$(PDCURSES_SRCDIR)
REGINA_REXXLIBS = $(REGINA_BINDIR)\regina.$(LIBEXT)
REGINA_REXXINC = -I$(REGINA_SRCDIR) -DUSE_REGINA
OS2REXX_REXXINC = -DUSE_OS2REXX
REXXTRANS_REXXLIBS = $(REXXTRANS_BINDIR)\rexxtrans.$(LIBEXT)
REXXTRANS_REXXINC = -I$(REXXTRANS_SRCDIR) -DUSE_REXXTRANS

include $(SRC)\the.ver

docdir = $(SRC)\doc
HTML_EXT = .html

COMM = \
	$(SRC)\comm1.c \
	$(SRC)\comm2.c \
	$(SRC)\comm3.c \
	$(SRC)\comm4.c \
	$(SRC)\comm5.c

COMMSOS = \
	$(SRC)\commsos.c

COMMSET = \
	$(SRC)\commset1.c \
	$(SRC)\commset2.c

QUERY = $(SRC)\query.c

APPENDIX1 = $(SRC)\appendix.1
APPENDIX2 = $(SRC)\appendix.2
APPENDIX3 = $(SRC)\appendix.3
APPENDIX4 = $(SRC)\appendix.4
APPENDIX7 = $(SRC)\appendix.7
APPENDIX = $(APPENDIX1) $(APPENDIX2) $(APPENDIX3) $(APPENDIX4) $(APPENDIX7)

GLOSSARY = $(SRC)\glossary
OVERVIEW = $(SRC)\overview

#########################################################################
# EMX compiler on OS/2
#########################################################################

ifeq ($(INT),OS2REXX)
REXXLIB = $(OS2REXX_REXXLIBS)
REXXINC =  $(OS2REXX_REXXINC)
endif
ifeq ($(INT),REXXTRANS)
REXXLIB = $(REXXTRANS_REXXLIBS)
REXXINC =  $(REXXTRANS_REXXINC)
endif
ifeq ($(INT),REGINA)
REXXLIB = $(REGINA_REXXLIBS)
REXXINC =  $(REGINA_REXXINC)
endif

ifeq ($(DEBUG),Y)
CFLAGS    = $(OBJCFLAG) -c -g -Wall -DEMX -DTHE_TRACE -DNOVIO -DSTDC_HEADERS -DHAVE_PROTO -I$(SRC) $(CURSINC) $(REXXINC) -o$@
LDEBUG    = -g
TRACE     = trace.$(OBJTYPE)
LXLITE    =
DEBUG     = N
else
CFLAGS    = $(OBJCFLAG) -c -O3 -fomit-frame-pointer -Wall -DEMX -DNOVIO -DSTDC_HEADERS -DHAVE_PROTO -I$(SRC) $(CURSINC) $(REXXINC) -o$@
LDEBUG    = $(OBJLDFLAGOPT)
TRACE     =
endif

LDFLAGS   = $(OBJLDFLAG) $(LDEBUG)

CC        = gcc
LD        = $(CC) $(LDFLAGS) -o the *.$(OBJTYPE) $(CURSLIB) $(REXXLIB)
THERES    = $(SRC)\the.res
RCTHE     = rc $(THERES) the.exe
#########################################################################
#
#
# Object files
#
OBJ1A = box.$(OBJTYPE) colour.$(OBJTYPE) comm1.$(OBJTYPE) comm2.$(OBJTYPE) comm3.$(OBJTYPE) comm4.$(OBJTYPE) comm5.$(OBJTYPE)
OBJ1B = commset1.$(OBJTYPE) commset2.$(OBJTYPE) commsos.$(OBJTYPE) cursor.$(OBJTYPE) default.$(OBJTYPE)
OBJ1C = edit.$(OBJTYPE) error.$(OBJTYPE) execute.$(OBJTYPE) linked.$(OBJTYPE) column.$(OBJTYPE) mouse.$(OBJTYPE)
OBJ1D = nonansi.$(OBJTYPE) prefix.$(OBJTYPE) reserved.$(OBJTYPE) scroll.$(OBJTYPE) show.$(OBJTYPE) sort.$(OBJTYPE)
OBJ1E = memory.$(OBJTYPE) target.$(OBJTYPE) the.$(OBJTYPE) util.$(OBJTYPE) parser.$(OBJTYPE)
OBJ1 = $(OBJ1A) $(OBJ1B) $(OBJ1C) $(OBJ1D) $(OBJ1E)
OBJ2 = commutil.$(OBJTYPE) print.$(OBJTYPE) $(TRACE)
OBJ3 = getch.$(OBJTYPE)
OBJ4 = query.$(OBJTYPE) query1.$(OBJTYPE) query2.$(OBJTYPE)
OBJ5 = thematch.$(OBJTYPE) regex.$(OBJTYPE) mygetopt.$(OBJTYPE)
OBJ6 = directry.$(OBJTYPE) file.$(OBJTYPE)
OBJ7 = rexx.$(OBJTYPE)
OBJ8 = os2eas.$(OBJTYPE)
OBJOTH = $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8)
OBJS = $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8)

COMM = $(SRC)\comm1.c $(SRC)\comm2.c $(SRC)\comm3.c $(SRC)\comm4.c $(SRC)\comm5.c \
	$(SRC)\commsos.c $(SRC)\commset1.c $(SRC)\commset2.c $(SRC)\query.c

all: how the.exe dist

how:
	echo make -f $(SRC)\gccos2.mak DEBUG=$(DEBUG) INT=$(INT) > rebuild.cmd
#
#########################################################################
the.exe:	$(OBJS)
	@echo $(OBJ1A) > the.rsp
	@echo $(OBJ1B) >> the.rsp
	@echo $(OBJ1C) >> the.rsp
	@echo $(OBJ1D) >> the.rsp
	@echo $(OBJ1E) >> the.rsp
	@echo $(OBJOTH) >> the.rsp
	$(LD)
	$(BIND)
	del the.rsp
	$(RCTHE)
	$(LXLITE)
#########################################################################
box.$(OBJTYPE):	$(SRC)\box.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\box.c
colour.$(OBJTYPE):	$(SRC)\colour.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\colour.c
comm1.$(OBJTYPE):	$(SRC)\comm1.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\comm1.c
comm2.$(OBJTYPE):	$(SRC)\comm2.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\comm2.c
comm3.$(OBJTYPE):	$(SRC)\comm3.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\comm3.c
comm4.$(OBJTYPE):	$(SRC)\comm4.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\comm4.c
comm5.$(OBJTYPE):	$(SRC)\comm5.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\comm5.c
commset1.$(OBJTYPE):	$(SRC)\commset1.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\commset1.c
commset2.$(OBJTYPE):	$(SRC)\commset2.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\commset2.c
commsos.$(OBJTYPE):	$(SRC)\commsos.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\commsos.c
cursor.$(OBJTYPE):	$(SRC)\cursor.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\cursor.c
default.$(OBJTYPE):	$(SRC)\default.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\default.c
edit.$(OBJTYPE):	$(SRC)\edit.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\edit.c
error.$(OBJTYPE):	$(SRC)\error.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\error.c
execute.$(OBJTYPE):	$(SRC)\execute.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\execute.c
linked.$(OBJTYPE):	$(SRC)\linked.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\linked.c
column.$(OBJTYPE):	$(SRC)\column.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\column.c
mouse.$(OBJTYPE):	$(SRC)\mouse.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\mouse.c
memory.$(OBJTYPE):	$(SRC)\memory.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\memory.c
nonansi.$(OBJTYPE):	$(SRC)\nonansi.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\nonansi.c
os2eas.$(OBJTYPE):	$(SRC)\os2eas.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\os2eas.c
prefix.$(OBJTYPE):	$(SRC)\prefix.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\prefix.c
print.$(OBJTYPE):	$(SRC)\print.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\print.c
reserved.$(OBJTYPE):	$(SRC)\reserved.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\reserved.c
scroll.$(OBJTYPE):	$(SRC)\scroll.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\scroll.c
show.$(OBJTYPE):	$(SRC)\show.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\show.c
sort.$(OBJTYPE):	$(SRC)\sort.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\sort.c
target.$(OBJTYPE):	$(SRC)\target.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\target.c
the.$(OBJTYPE):	$(SRC)\the.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\the.ver
	$(CC) $(CFLAGS) -DTHE_VERSION=\"$(VER_DOT)\" -DTHE_VERSION_DATE=\"$(VER_DATE)\" $(SRC)\$*.c
util.$(OBJTYPE):	$(SRC)\util.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\util.c
commutil.$(OBJTYPE):	$(SRC)\commutil.c $(SRC)\the.h $(SRC)\command.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\getch.h $(SRC)\key.h
	$(CC) $(CFLAGS) $(SRC)\commutil.c
trace.$(OBJTYPE):	$(SRC)\trace.c $(SRC)\the.h $(SRC)\command.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\getch.h $(SRC)\key.h
	$(CC) $(CFLAGS) $(SRC)\trace.c
getch.$(OBJTYPE):	$(SRC)\getch.c $(SRC)\getch.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\getch.c
query.$(OBJTYPE):	$(SRC)\query.c $(SRC)\query.h $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\query.c
query1.$(OBJTYPE):	$(SRC)\query1.c $(SRC)\query.h $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\query1.c
query2.$(OBJTYPE):	$(SRC)\query2.c $(SRC)\query.h $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\query2.c
thematch.$(OBJTYPE):	$(SRC)\thematch.c $(SRC)\the.h $(SRC)\thematch.h
	$(CC) $(CFLAGS) $(SRC)\thematch.c
parser.$(OBJTYPE):	$(SRC)\parser.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\parser.c
directry.$(OBJTYPE):	$(SRC)\directry.c $(SRC)\the.h $(SRC)\directry.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\directry.c
file.$(OBJTYPE):	$(SRC)\file.c $(SRC)\the.h $(SRC)\directry.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\file.c
rexx.$(OBJTYPE):	$(SRC)\rexx.c $(SRC)\the.h $(SRC)\therexx.h $(SRC)\proto.h $(SRC)\thedefs.h $(SRC)\query.h
	$(CC) $(CFLAGS) $(SRC)\rexx.c
regex.$(OBJTYPE):	$(SRC)\regex.c $(SRC)\regex.h
	$(CC) $(CFLAGS) $(SRC)\regex.c
mygetopt.$(OBJTYPE):	$(SRC)\mygetopt.c $(SRC)\mygetopt.h
	$(CC) $(CFLAGS) $(SRC)\mygetopt.c
########################################################################
dist: the.exe
	echo run $(SRC)\makedist.cmd
#########################################################################

manext.exe: manext.o
	$(CC) manext.o -o manext
	emxbind manext

manext.o: $(SRC)\manext.c $(SRC)\the.h
	$(CC) -c -I$(SRC) -o manext.o $(SRC)\manext.c

html:	manext.exe
	-del *$(HTML_EXT)
	copy $(SRC)\man2html.rex .\man2html.cmd
	man2html $(HTML_EXT) $(VER_DOT) TOCSTART > index$(HTML_EXT)
	.\manext $(OVERVIEW) > overview.man
	man2html $(HTML_EXT) $(VER_DOT) OVERVIEW overview.man index$(HTML_EXT) > overview$(HTML_EXT)
	.\manext $(COMM) > comm.man
	man2html $(HTML_EXT) $(VER_DOT) COMM comm.man index$(HTML_EXT) > comm$(HTML_EXT)
	.\manext $(COMMSET) > commset.man
	man2html $(HTML_EXT) $(VER_DOT) COMMSET commset.man index$(HTML_EXT) > commset$(HTML_EXT)
	.\manext $(COMMSOS) > commsos.man
	man2html $(HTML_EXT) $(VER_DOT) COMMSOS commsos.man index$(HTML_EXT) > commsos$(HTML_EXT)
	.\manext $(QUERY) > query.man
	man2html $(HTML_EXT) $(VER_DOT) QUERY    query.man index$(HTML_EXT)    > query$(HTML_EXT)
	.\manext $(GLOSSARY) > glossary.man
	man2html $(HTML_EXT) $(VER_DOT) GLOSSARY glossary.man index$(HTML_EXT) > glossary$(HTML_EXT)
	.\manext $(APPENDIX1) > app1.man
	man2html $(HTML_EXT) $(VER_DOT) APPENDIX1 app1.man index$(HTML_EXT) > app1$(HTML_EXT)
	.\manext $(APPENDIX2) > app2.man
	man2html $(HTML_EXT) $(VER_DOT) APPENDIX2 app2.man index$(HTML_EXT) > app2$(HTML_EXT)
	.\manext $(APPENDIX3) > app3.man
	man2html $(HTML_EXT) $(VER_DOT) APPENDIX3 app3.man index$(HTML_EXT) > app3$(HTML_EXT)
	.\manext $(APPENDIX4) > app4.man
	man2html $(HTML_EXT) $(VER_DOT) APPENDIX4 app4.man index$(HTML_EXT) > app4$(HTML_EXT)
	man2html $(HTML_EXT) $(VER_DOT) APPENDIX5 $(docdir)\app5.htm index$(HTML_EXT) > app5$(HTML_EXT)
	man2html $(HTML_EXT) $(VER_DOT) APPENDIX6 $(docdir)\app6.htm index$(HTML_EXT) > app6$(HTML_EXT)
	.\manext $(APPENDIX7) > app7.man
	man2html $(HTML_EXT) $(VER_DOT) APPENDIX7 app7.man index$(HTML_EXT) > app7$(HTML_EXT)
	copy $(SRC)\HISTORY history.man
	man2html $(HTML_EXT) $(VER_DOT) HISTORY history.man index$(HTML_EXT) > history$(HTML_EXT)
	copy $(SRC)\THE_Help.txt quickref.man
	man2html $(HTML_EXT) $(VER_DOT) QUICKREF quickref.man index$(HTML_EXT) > quickref$(HTML_EXT)
	man2html $(HTML_EXT) $(VER_DOT) TOCEND >> index$(HTML_EXT)

helpviewer:	manext.exe
	-del \Y *$(HTML_EXT)
	-del \Y *.man
	-del \Y comm\\*.*
	-del \Y commsos\\*.*
	-del \Y commset\\*.*
	-del \Y misc\\*.*
	-mkdir comm
	-mkdir commsos
	-mkdir commset
	-mkdir misc
	copy $(SRC)\man2hv.rex .\man2hv.cmd
	man2hv $(HTML_EXT) $(VER_DOT) TOCSTART 'junk' index$(HTML_EXT)
	.\manext $(OVERVIEW) > overview.man
	man2hv $(HTML_EXT) $(VER_DOT) OVERVIEW overview.man index$(HTML_EXT)
	.\manext $(COMM) > comm.man
	man2hv $(HTML_EXT) $(VER_DOT) COMM comm.man index$(HTML_EXT)
	.\manext $(COMMSET) > commset.man
	man2hv $(HTML_EXT) $(VER_DOT) COMMSET commset.man index$(HTML_EXT)
	.\manext $(COMMSOS) > commsos.man
	man2hv $(HTML_EXT) $(VER_DOT) COMMSOS commsos.man index$(HTML_EXT)
	.\manext $(QUERY) > query.man
	man2hv $(HTML_EXT) $(VER_DOT) QUERY    query.man index$(HTML_EXT)
	.\manext $(GLOSSARY) > glossary.man
	man2hv $(HTML_EXT) $(VER_DOT) GLOSSARY glossary.man index$(HTML_EXT)
	.\manext $(APPENDIX1) > app1.man
	man2hv $(HTML_EXT) $(VER_DOT) APPENDIX1 app1.man index$(HTML_EXT)
	.\manext $(APPENDIX2) > app2.man
	man2hv $(HTML_EXT) $(VER_DOT) APPENDIX2 app2.man index$(HTML_EXT)
	.\manext $(APPENDIX3) > app3.man
	man2hv $(HTML_EXT) $(VER_DOT) APPENDIX3 app3.man index$(HTML_EXT)
	.\manext $(APPENDIX4) > app4.man
	man2hv $(HTML_EXT) $(VER_DOT) APPENDIX4 app4.man index$(HTML_EXT)
	man2hv $(HTML_EXT) $(VER_DOT) APPENDIX5 $(docdir)\app5.htm index$(HTML_EXT)
	man2hv $(HTML_EXT) $(VER_DOT) APPENDIX6 $(docdir)\app6.htm index$(HTML_EXT)
	.\manext $(APPENDIX7) > app7.man
	man2hv $(HTML_EXT) $(VER_DOT) APPENDIX7 app7.man index$(HTML_EXT)
	copy $(SRC)\HISTORY history.man
	man2hv $(HTML_EXT) $(VER_DOT) HISTORY history.man index$(HTML_EXT)
	copy $(SRC)\THE_Help.txt quickref.man
	man2hv $(HTML_EXT) $(VER_DOT) QUICKREF quickref.man index$(HTML_EXT)
	man2hv $(HTML_EXT) $(VER_DOT) TOCEND 'junk' index$(HTML_EXT)
