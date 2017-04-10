#
#########################################################################
#
# makefile for The Hessling Editor (THE)
#
# You need the following environment variable set like:
# THE_SRCDIR=c:\the
# PDCURSES_BINDIR=c:\pdc
# PDCURSES_SRCDIR=c:\pdcurses
# REGINA_SRCDIR=c:\regina
# REGINA_BINDIR=c:\regina
#
# Then call
# wmake -f wccwin32.mak [DEBUG=Y]
#
#########################################################################
#

TARGET=NT  # includes W95
SYSTEM=nt
THE_TARGET=WIN32

#########################################################################
# Watcom  compiler on Win32
#########################################################################
SRC       = $(%THE_SRCDIR)\
WATCOMSRC = $(%WATCOM)\src\startup\
CURSLIB   = $(%PDCURSES_HOMEDIR)\$(SYSTEM)\pdcurses.lib
CURSINC   = /i=$(%PDCURSES_SRCDIR)
REXXINC   = /i=$(%REGINA_SRCDIR) /DUSE_REGINA
REXXLIB   = $(%REGINA_HOMEDIR)\$(SYSTEM)\regina.lib

PROJ      = the.exe
OBJ       = obj
CC        = wcc386

!include $(SRC)the.ver

!ifeq DEBUG Y
CFLAGS    = /bt=$(TARGET) /d2 /zq /mf /wx /DSTDC_HEADERS /DPROTO /D$(THE_TARGET) /i=$(SRC) $(CURSINC) $(REXXINC) /fo=$@
LDEBUG    = debug all
!else
CFLAGS    = /bt=$(TARGET) /oneatx /zq /mf /wx /DSTDC_HEADERS /DPROTO /D$(THE_TARGET) /i=$(SRC) $(CURSINC) $(REXXINC) /fo=$@
LDEBUG    =
DEBUG     = N
!endif
LD        = wlink
XTRAOBJ   = mygetopt.obj
docdir = doc

#########################################################################
#
#
# Object files
#
OBJ1 = box.$(OBJ) colour.$(OBJ) comm1.$(OBJ) comm2.$(OBJ) comm3.$(OBJ) comm4.$(OBJ) comm5.$(OBJ) &
	commset1.$(OBJ) commset2.$(OBJ) commsos.$(OBJ) cursor.$(OBJ) default.$(OBJ) &
	edit.$(OBJ) error.$(OBJ) execute.$(OBJ) linked.$(OBJ) column.obj mouse.obj memory.obj &
	nonansi.$(OBJ) prefix.$(OBJ) parser.$(OBJ) reserved.$(OBJ) scroll.$(OBJ) show.$(OBJ) sort.$(OBJ) &
	target.$(OBJ) the.$(OBJ) util.$(OBJ) print.$(OBJ) regex.obj alloca.obj single.$(OBJ)
OBJ2 = commutil.$(OBJ) #trace.obj
OBJ3 = getch.$(OBJ)
OBJ4 = query.$(OBJ) query1.$(OBJ) query2.$(OBJ)
OBJ5 = thematch.$(OBJ)
OBJ6 = directry.$(OBJ) file.$(OBJ)
OBJ7 = rexx.$(OBJ)
OBJ8 = wildargv.obj
OBJX = $(XTRAOBJ)
OBJS = $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8) $(OBJX)
#########################################################################

COMM = $(SRC)comm1.c $(SRC)comm2.c $(SRC)comm3.c $(SRC)comm4.c $(SRC)comm5.c &
	$(SRC)commsos.c $(SRC)commset1.c $(SRC)commset2.c $(SRC)query.c

APPENDIX = $(SRC)appendix.1
GLOSSARY = $(SRC)glossary

all: how $(PROJ)

how: .SYMBOLIC
	echo wmake -f $(SRC)wccwin32.mak DEBUG=$(DEBUG) > rebuild.bat
#
#########################################################################
$(PROJ):	$(OBJS)
	@%create dummy.lnk
	@%append dummy.lnk  option quiet
	@%append dummy.lnk  option map
	@%append dummy.lnk  option stack=256k
	@%append dummy.lnk  system $(SYSTEM)
	@%append dummy.lnk  name the.exe
	@%append dummy.lnk  file *.obj
	@%append dummy.lnk  library $(CURSLIB)
	@%append dummy.lnk  library $(REXXLIB)
	@%append dummy.lnk  library wsock32
	$(LD) $(LDEBUG) $(LDFLAGS) @dummy.lnk
	%erase dummy.lnk
#########################################################################
box.obj:	$(SRC)box.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
colour.obj:	$(SRC)colour.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
comm1.obj:	$(SRC)comm1.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
comm2.obj:	$(SRC)comm2.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
comm3.obj:	$(SRC)comm3.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
comm4.obj:	$(SRC)comm4.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
comm5.obj:	$(SRC)comm5.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
commset1.obj:	$(SRC)commset1.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
commset2.obj:	$(SRC)commset2.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
commsos.obj:	$(SRC)commsos.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
cursor.obj:	$(SRC)cursor.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
default.obj:	$(SRC)default.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
edit.obj:	$(SRC)edit.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
error.obj:	$(SRC)error.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
execute.obj:	$(SRC)execute.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
linked.obj:	$(SRC)linked.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
column.obj:	$(SRC)column.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
mouse.obj:	$(SRC)mouse.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
memory.obj:	$(SRC)memory.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
nonansi.obj:	$(SRC)nonansi.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
parser.obj:	$(SRC)parser.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
prefix.obj:	$(SRC)prefix.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
print.obj:	$(SRC)print.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
regex.obj:	$(SRC)regex.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\$*.c
alloca.obj:	$(SRC)alloca.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(SRC)\$*.c
reserved.obj:	$(SRC)reserved.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
scroll.obj:	$(SRC)scroll.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
show.obj:	$(SRC)show.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
sort.obj:	$(SRC)sort.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
target.obj:	$(SRC)target.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
the.obj:	$(SRC)the.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h $(SRC)the.ver
	$(CC) $(CFLAGS) -DTHE_VERSION=$(VER_DOT) -DTHE_VERSION_DATE=$(VER_DATE) $(SRC)$*.c
util.obj:	$(SRC)util.c $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
commutil.obj:	$(SRC)commutil.c $(SRC)the.h $(SRC)command.h $(SRC)thedefs.h $(SRC)proto.h $(SRC)getch.h $(SRC)key.h
	$(CC) $(CFLAGS) $(SRC)$*.c
trace.obj:	$(SRC)trace.c $(SRC)the.h $(SRC)command.h $(SRC)thedefs.h $(SRC)proto.h $(SRC)getch.h $(SRC)key.h
	$(CC) $(CFLAGS) $(SRC)$*.c
getch.obj:	$(SRC)getch.c $(SRC)getch.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
query.obj:	$(SRC)query.c $(SRC)query.h $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
query1.obj:	$(SRC)query1.c $(SRC)query.h $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
query2.obj:	$(SRC)query2.c $(SRC)query.h $(SRC)the.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
thematch.obj:	$(SRC)thematch.c $(SRC)the.h $(SRC)thematch.h
	$(CC) $(CFLAGS) $(SRC)$*.c
directry.obj:	$(SRC)directry.c $(SRC)the.h $(SRC)directry.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
file.obj:	$(SRC)file.c $(SRC)the.h $(SRC)directry.h $(SRC)thedefs.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c
rexx.obj:	$(SRC)rexx.c $(SRC)the.h $(SRC)therexx.h $(SRC)proto.h $(SRC)thedefs.h $(SRC)query.h
	$(CC) $(CFLAGS) $(SRC)$*.c
mygetopt.obj:	$(SRC)mygetopt.c
	$(CC) $(CFLAGS) $(SRC)$*.c
single.obj:     $(SRC)single.c $(SRC)the.h $(SRC)proto.h
	$(CC) $(CFLAGS) $(SRC)$*.c

wildargv.obj:	$(WATCOMSRC)wildargv.c
	$(CC) $(CFLAGS) $(WATCOMSRC)$*.c
#
#########################################################################
