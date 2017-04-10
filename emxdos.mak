#
#########################################################################
#
# makefile for The Hessling Editor (THE)
# emx dos
#
# You need the following environment variable set like:
# THE_SRCDIR=c:/the
# THE_SRCDIRN=c:\the
# PDCURSES_VCPDIR=c:/pdc/vcp
# REGINA_VCPDIR=c:/regina/vcp
# REGINA_VCPDIRN=c:\regina\vcp
#
#########################################################################
#

SRC       = $(THE_SRCDIR)
ASRC      = $(THE_SRCDIRN)
CURSBIN   = $(PDCURSES_VCPDIR)
CURSLIB   = $(CURSBIN)/pdcurses.a
CURSINC   = -I$(PDCURSES_SRCDIR)
REGINA_BIN = $(REGINA_VCPDIR)
AREGINA_BIN = $(REGINA_VCPDIRN)
REGINA_REXXLIBS = $(REGINA_BIN)/regina.a
REGINA_REXXINC = -I$(REGINA_SRCDIR) -DUSE_REGINA

include $(SRC)/the.ver

#########################################################################
# EMX compiler on OS/2
#########################################################################

REXXLIB = $(REGINA_REXXLIBS)
REXXINC =  $(REGINA_REXXINC)

ifeq ($(DEBUG),Y)
CFLAGS    = -c -g -Wall -DEMXVIDEO -DMSDOS -DSTDC_HEADERS -DHAVE_PROTO -I$(SRC) $(CURSINC) $(REXXINC) -o$*.o
LDEBUG    = -g
TRACE     = trace.o
else
CFLAGS    = -c -O3 -fomit-frame-pointer -Wall -DEMXVIDEO -DMSDOS -DSTDC_HEADERS -DHAVE_PROTO -I$(SRC) $(CURSINC) $(REXXINC) -o$*.o
LDEBUG    =
TRACE     =
DEBUG     = N
endif

LDFLAGS   = $(LDEBUG)

CC        = gcc
LD        = $(CC) $(LDEBUG) -o the *.o $(CURSLIB) $(REXXLIB) -lvideo
MAN       = manext.exe
MANLD     = link386 manext+mygetopt+g:\ibmc\lib\setargv,,,,ibm-man.def /NOE /NOI /EXEPACK:2 /PACKCODE /PACKDATA /ALIGN:4;
#########################################################################
#
#
# Object files
#
OBJ1A = box.o colour.o comm1.o comm2.o comm3.o comm4.o comm5.o
OBJ1B = commset1.o commset2.o commsos.o cursor.o default.o
OBJ1C = edit.o error.o execute.o linked.o column.o mouse.o
OBJ1D = nonansi.o prefix.o reserved.o scroll.o show.o sort.o
OBJ1E = memory.o target.o the.o util.o parser.o
OBJ1 = $(OBJ1A) $(OBJ1B) $(OBJ1C) $(OBJ1D) $(OBJ1E)
OBJ2 = commutil.o print.o $(TRACE)
OBJ3 = getch.o
OBJ4 = query.o query1.o query2.o
OBJ5 = thematch.o regex.o mygetopt.o
OBJ6 = directry.o file.o
OBJ7 = rexx.o
OBJ8 =
OBJOTH = $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8)
OBJS = $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(OBJ8)

COMM = $(SRC)/comm1.c $(SRC)/comm2.c $(SRC)/comm3.c $(SRC)/comm4.c $(SRC)/comm5.c \
	$(SRC)/commsos.c $(SRC)/commset1.c $(SRC)/commset2.c $(SRC)/query.c

APPENDIX = $(SRC)/appendix.1 $(SRC)/appendix.2 $(SRC)/appendix.3
GLOSSARY = $(SRC)/glossary

all: how the.exe dist

how:
	echo make -f $(SRC)\emxdos.mak DEBUG=$(DEBUG) > rebuild.cmd
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
	emxbind the -acm
	del the.rsp
#########################################################################
box.o:	$(SRC)/box.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
colour.o:	$(SRC)/colour.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
comm1.o:	$(SRC)/comm1.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
comm2.o:	$(SRC)/comm2.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
comm3.o:	$(SRC)/comm3.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
comm4.o:	$(SRC)/comm4.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
comm5.o:	$(SRC)/comm5.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
commset1.o:	$(SRC)/commset1.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
commset2.o:	$(SRC)/commset2.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
commsos.o:	$(SRC)/commsos.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
cursor.o:	$(SRC)/cursor.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
default.o:	$(SRC)/default.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
edit.o:	$(SRC)/edit.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
error.o:	$(SRC)/error.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
execute.o:	$(SRC)/execute.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
linked.o:	$(SRC)/linked.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
column.o:	$(SRC)/column.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
mouse.o:	$(SRC)/mouse.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
memory.o:	$(SRC)/memory.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
nonansi.o:	$(SRC)/nonansi.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
os2eas.o:	$(SRC)/os2eas.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
prefix.o:	$(SRC)/prefix.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
print.o:	$(SRC)/print.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
reserved.o:	$(SRC)/reserved.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
scroll.o:	$(SRC)/scroll.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
show.o:	$(SRC)/show.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
sort.o:	$(SRC)/sort.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
target.o:	$(SRC)/target.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
the.o:	$(SRC)/the.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h $(SRC)/the.ver
	$(CC) $(CFLAGS) -DTHE_VERSION=$(VER_DOT) -DTHE_VERSION_DATE=$(VER_DATE) $(SRC)/$*.c
util.o:	$(SRC)/util.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
commutil.o:	$(SRC)/commutil.c $(SRC)/the.h $(SRC)/command.h $(SRC)/thedefs.h $(SRC)/proto.h $(SRC)/getch.h $(SRC)/key.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
trace.o:	$(SRC)/trace.c $(SRC)/the.h $(SRC)/command.h $(SRC)/thedefs.h $(SRC)/proto.h $(SRC)/getch.h $(SRC)/key.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
getch.o:	$(SRC)/getch.c $(SRC)/getch.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
query.o:	$(SRC)/query.c $(SRC)/query.h $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
query1.o:	$(SRC)/query1.c $(SRC)/query.h $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
query2.o:	$(SRC)/query2.c $(SRC)/query.h $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
thematch.o:	$(SRC)/thematch.c $(SRC)/the.h $(SRC)/thematch.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
parser.o:	$(SRC)/parser.c $(SRC)/the.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
directry.o:	$(SRC)/directry.c $(SRC)/the.h $(SRC)/directry.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
file.o:	$(SRC)/file.c $(SRC)/the.h $(SRC)/directry.h $(SRC)/thedefs.h $(SRC)/proto.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
rexx.o:	$(SRC)/rexx.c $(SRC)/the.h $(SRC)/therexx.h $(SRC)/proto.h $(SRC)/thedefs.h $(SRC)/query.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
regex.o:	$(SRC)/regex.c $(SRC)/regex.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
mygetopt.o:	$(SRC)/mygetopt.c $(SRC)/mygetopt.h
	$(CC) $(CFLAGS) $(SRC)/$*.c
########################################################################
manual:	$(MAN) $(SRC)/overview $(COMM) $(APPENDIX) $(GLOSSARY)
	manext $(SRC)/overview $(COMM) $(APPENDIX) $(GLOSSARY) > the.man
#
$(MAN):	$(XTRAOBJ) manext.o
	$(MANLD)
	$(CHMODMAN)

#########################################################################
dist: the.exe
	echo run $(ASRC)\makedist.cmd
#########################################################################
