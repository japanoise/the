#
#########################################################################
#
# makefile for The Hessling Editor (THE)
#
#########################################################################
#
# You need the following environment variables set like:
# THE_SRCDIR=c:\the
# PDCURSES_SRCDIR=c:\pdcurses
# PDCURSES_BINDIR=c:\pdc\vc
# If building with Regina...
#   REGINA_BINDIR=c:\regina\vc
#   REGINA_SRCDIR=c:\regina
# If building with Object Rexx...
#   OREXX_BINDIR=c:\objrexx\api
# If building with Open Object Rexx...
#   REXX_HOME is set
# If building with Rexx/Trans...
#   REXXTRANS_BINDIR=c:\rexxtrans
#   REXXTRANS_SRCDIR=c:\rexxtrans
# If building with WinRexx...
#   WINREXX_BINDIR=c:\winrexx
#   WINREXX_SRCDIR=c:\winrexx
# If building with Quercus Rexx...
#   QUERCUS_BINDIR=c:\quercus
#   QUERCUS_SRCDIR=c:\quercus
# If building with uni-Rexx...
#   UNIREXX_BINDIR=c:\unirexx
#   UNIREXX_SRCDIR=c:\unirexx
#
# MSVCDIR=c:\program files\microsoft visual studio\vc98
#
#########################################################################
#

#
# Use STATIC linking to PDCurses ?????
# Using the PDCurses DLL, then "the -1" will refresh the newly edited
# file correctly. The STATIC linking will NOT refresh the newly edited
# file correctly; you will need to hit a key for it to refresh.
#
!if "$(STATIC)" == "N"
PDCURSES_LIB=pdcurses.lib
PDCURSES_DEFINES=-DPDC_DLL_BUILD
CURSESDLLINNSIS=/DCURSESDLL
STATIC=N
!else
PDCURSES_LIB=pdcurses.lib
PDCURSES_DEFINES=
CURSESDLLINNSIS=
STATIC=Y
!endif

!if "$(UTF8)" == "Y"
UTF8_FLAG=-DUSE_UTF8
UTF8=Y
!else
UTF8_FLAG=
UTF8=N
!endif

SRC       = $(THE_SRCDIR)
CURSCON   = $(PDCURSES_CONDIR)
CURSGUI   = $(PDCURSES_GUIDIR)
CURSSDL   = $(PDCURSES_SDLDIR)
REXXTRANSBIN = c:\bin
PDCURSESBIN = c:\bin
INCGUI = -DPDC_WIDE -I$(PDCURSES_SRCDIR_WIN32A) -I$(PDCURSES_SRCDIR_WIN32A)\win32a
INCCONSOLE = -I$(PDCURSES_SRCDIR_WIN32)
CURSINC   = $(PDCURSES_DEFINES)

!ifdef MSVCDIR
# this env variable for VC 6
SETARGV   = "$(MSVCDIR)\lib\setargv.obj"
!else
# The following for Visual C++ 10
!if "$(LARCH)" == "w32"
SETARGV   = "$(VCINSTALLDIR)\lib\setargv.obj"
!else
SETARGV   = "$(VCINSTALLDIR)\lib\amd64\setargv.obj"
!endif
!endif

REGINA_BIN = $(REGINA_BINDIR)
REGINA_REXXLIBS = $(REGINA_BIN)\regina.lib
REGINA_REXXINC = -I$(REGINA_SRCDIR) -DUSE_REGINA
OREXX_REXXLIBS = $(OREXX_BINDIR)\api\rexx.lib $(OREXX_BINDIR)\api\rexxapi.lib
OREXX_REXXINC = -I$(OREXX_BINDIR)\api -DUSE_OREXX
OOREXX_REXXLIBS = "$(REXX_HOME)\api\rexx.lib" "$(REXX_HOME)\api\rexxapi.lib"
OOREXX_REXXINC = "$(REXX_HOME)\api" -DUSE_OOREXX
OOREXX40_REXXLIBS = $(OOREXX_REXXLIBS)
OOREXX40_REXXINC = -I$(OOREXX_REXXINC) -DOOREXX_40
WINREXX_REXXLIBS = $(WINREXX_BINDIR)\api\rxrexx.lib
WINREXX_REXXINC = -I$(WINREXX_BINDIR)\api -DUSE_WINREXX
QUERCUS_REXXLIBS = $(QUERCUS_BINDIR)\api\wrexx32.lib
QUERCUS_REXXINC = -I$(QUERCUS_BINDIR)\api -DUSE_QUERCUS
UNIREXX_REXXLIBS = $(UNIREXX_BINDIR)\rxx.lib
UNIREXX_REXXINC = -I$(UNIREXX_BINDIR) -DUSE_UNIREXX
REXXTRANS_REXXLIBS = $(REXXTRANS_BINDIR)\rexxtrans.lib
REXXTRANS_REXXINC = -I$(REXXTRANS_SRCDIR) -DUSE_REXXTRANS

.SUFFIXES:
#
# Following included file provides VER, VER_DOT, VER_DATE
#
!include $(SRC)\the.ver

#########################################################################
# MS VC++ compiler on Windows
#########################################################################
PROJ      = the.exe
OBJ       = obj
CC        = cl

!if "$(INT)" == "REGINA"
REXXLIB = $(REGINA_REXXLIBS)
REXXINC =  $(REGINA_REXXINC)
REXXINT = "Regina"
!elseif "$(INT)" == "OREXX"
REXXLIB = $(OREXX_REXXLIBS)
REXXINC =  $(OREXX_REXXINC)
!elseif "$(INT)" == "OOREXX"
REXXLIB = $(OOREXX_REXXLIBS)
REXXINC =  $(OOREXX_REXXINC)
REXXINT = "ooRexx"
!elseif "$(INT)" == "OOREXX40"
REXXLIB = $(OOREXX40_REXXLIBS)
REXXINC =  $(OOREXX40_REXXINC)
REXXINT = "ooRexx"
!elseif "$(INT)" == "WINREXX"
REXXLIB = $(WINREXX_REXXLIBS)
REXXINC =  $(WINREXX_REXXINC)
!elseif "$(INT)" == "QUERCUS"
REXXLIB = $(QUERCUS_REXXLIBS)
REXXINC =  $(QUERCUS_REXXINC)
!elseif "$(INT)" == "UNIREXX"
REXXLIB = $(UNIREXX_REXXLIBS)
REXXINC =  $(UNIREXX_REXXINC)
!elseif "$(INT)" == "REXXTRANS"
REXXLIB = $(REXXTRANS_REXXLIBS)
REXXINC =  $(REXXTRANS_REXXINC)
REXXINT = "RexxTrans"
!else
!message Rexx Interpreter NOT specified via INT macro
!message Valid values are: REGINA OOREXX40 OOREXX OREXX WINREXX QUERCUS UNIREXX REXXTRANS
!error Make aborted!
!endif

#!if "$(PDC)" == "SDL"
#PDC_FLAGS = -DUSE_SDLCURSES
#CURSLIB   = $(CURSSDL)\$(PDCURSES_LIB)
#THEOBJ = "sdl"
#!else
#!if "$(PDC)" == "WINGUI"
#PDC_FLAGS = -DUSE_WINGUICURSES -I$(SRC)\contrib
##DRAGDROPOBJ = DragAndDrop.obj
##EXTRALIB = ole32.lib
#LDEXTRA = "-subsystem:windows"
#CURSLIB   = $(CURSGUI)\$(PDCURSES_LIB)
#THEOBJ = "gui"
#!else
#PDC_FLAGS =
#CURSLIB   = $(CURSCON)\$(PDCURSES_LIB)
#PDC = "CONSOLE"
#THEOBJ = "console"
#!endif
#!endif

!if "$(TRACE)" == "Y"
TRACE_FLAGS = -DTHE_TRACE
GUI_TRACE_OBJ = gui\trace.obj
CONSOLE_TRACE_OBJ = console\trace.obj
!else
TRACE_FLAGS =
GUI_TRACE_OBJ =
CONSOLE_TRACE_OBJ =
TRACE=N
!endif

!if "$(DEBUG)" == "Y"
CFLAGS    = -nologo -c -Od -Z7 -FR -DDEBUG -DWIN32 $(UTF8_FLAG) -DSTDC_HEADERS -DHAVE_PROTO $(TRACE_FLAGS) $(PDC_FLAGS) -I$(SRC) -I$(SRC)\contrib $(CURSINC) $(REXXINC) -Fo$@
LDEBUG     = -debug -map:the.map
DIST=
!else
CFLAGS    = -nologo -c -Ox -DWIN32 $(UTF8_FLAG) -DSTDC_HEADERS -DHAVE_PROTO $(TRACE_FLAGS) $(PDC_FLAGS) -I$(SRC) -I$(SRC)\contrib $(CURSINC) $(REXXINC) -Fo$@
LDEBUG     = -release
DEBUG=N
!if "$(INT)" == "REXXTRANS"
DIST=dist
!else
DIST=
!endif
!endif

LD        = link
XTRAOBJ   = mygetopt.obj
MAN       = manext.exe
THERC     = $(SRC)\the$(LARCH).rc
THERES    = the$(LARCH).res
THEWINOBJ = the$(LARCH).obj
docdir = doc
#########################################################################
#
#
# Object files
#
GUIOBJS = gui\box.obj gui\colour.obj gui\comm1.obj gui\comm2.obj gui\comm3.obj gui\comm4.obj gui\comm5.obj \
	gui\commset1.obj gui\commset2.obj gui\commsos.obj gui\cursor.obj gui\default.obj gui\utf8.obj \
	gui\edit.obj gui\error.obj gui\execute.obj gui\linked.obj gui\column.obj gui\mouse.obj gui\memory.obj \
	gui\nonansi.obj gui\prefix.obj gui\reserved.obj gui\scroll.obj gui\show.obj gui\single.obj gui\sort.obj \
	gui\target.obj gui\the.obj gui\util.obj gui\parser.obj gui\regex.obj gui\commutil.obj gui\print.obj \
	gui\getch.obj gui\query.obj gui\query1.obj gui\query2.obj gui\thematch.obj gui\directry.obj gui\file.obj gui\rexx.obj \
	gui\mygetopt.obj $(GUI_TRACE_OBJ) #$(DRAGDROPOBJ)
#-----------------------------------------------------------------------
CONSOLEOBJS = console\box.obj console\colour.obj console\comm1.obj console\comm2.obj console\comm3.obj console\comm4.obj console\comm5.obj \
	console\commset1.obj console\commset2.obj console\commsos.obj console\cursor.obj console\default.obj console\utf8.obj \
	console\edit.obj console\error.obj console\execute.obj console\linked.obj console\column.obj console\mouse.obj console\memory.obj \
	console\nonansi.obj console\prefix.obj console\reserved.obj console\scroll.obj console\show.obj console\single.obj console\sort.obj \
	console\target.obj console\the.obj console\util.obj console\parser.obj console\regex.obj console\commutil.obj console\print.obj \
	console\getch.obj console\query.obj console\query1.obj console\query2.obj console\thematch.obj console\directry.obj console\file.obj console\rexx.obj \
	console\mygetopt.obj $(CONSOLE_TRACE_OBJ) #$(DRAGDROPOBJ)
#-----------------------------------------------------------------------
EXECOBJS = gui\mygetopt.obj .\execthe.obj
#########################################################################

COMM = $(SRC)\comm1.c $(SRC)\comm2.c $(SRC)\comm3.c $(SRC)\comm4.c $(SRC)\comm5.c \
	$(SRC)\commsos.c $(SRC)\commset1.c $(SRC)\commset2.c $(SRC)\query.c

APPENDIX = $(SRC)\appendix.1
GLOSSARY = $(SRC)\glossary

all: how theg.exe thec.exe the.exe $(DIST)

how:
	echo nmake -f $(SRC)\vc$(WARCH).mak INT=$(INT) DEBUG=$(DEBUG) TRACE=$(TRACE) STATIC=$(STATIC) UTF8=$(UTF8) ^%1 ^%2 > rebuild.bat
#
#########################################################################
# Rules for building the THE launcher
the.exe:	$(EXECOBJS) $(THERES) $(THEWINOBJ)
	$(LD) /subsystem:windows /NOLOGO /VERSION:$(VER_DOT) $(LDEBUG) $(EXECOBJS) $(THEWINOBJ) $(SETARGV) -out:the.exe $(LDEXTRA) kernel32.lib userenv.lib advapi32.lib
#------------------------------------------------------------------------
execthe.obj:	$(SRC)\execthe.c
	$(CC) $(CFLAGS) -DTHE_VERSION=\"$(VER_DOT)\" -DTHE_VERSION_DATE=\"$(VER_DATE)\" $(SRC)\$(*B).c
#########################################################################
# Rules for building the GUI version of THE
theg.exe: gui $(GUIOBJS) $(THERES) $(THEWINOBJ)
	$(LD) /subsystem:windows /NOLOGO /VERSION:$(VER_DOT) $(LDEBUG) $(GUIOBJS) $(THEWINOBJ) $(SETARGV) -out:$*.exe $(LDEXTRA) $(CURSGUI)\$(PDCURSES_LIB) $(REXXLIB) user32.lib gdi32.lib comdlg32.lib winspool.lib wsock32.lib shell32.lib advapi32.lib kernel32.lib $(EXTRALIB)
#------------------------------------------------------------------------
gui\box.obj:	$(SRC)\box.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\colour.obj:	$(SRC)\colour.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\comm1.obj:	$(SRC)\comm1.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\comm2.obj:	$(SRC)\comm2.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\comm3.obj:	$(SRC)\comm3.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\comm4.obj:	$(SRC)\comm4.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\comm5.obj:	$(SRC)\comm5.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\commset1.obj:	$(SRC)\commset1.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\commset2.obj:	$(SRC)\commset2.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\commsos.obj:	$(SRC)\commsos.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\cursor.obj:	$(SRC)\cursor.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\default.obj:	$(SRC)\default.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\edit.obj:	$(SRC)\edit.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\error.obj:	$(SRC)\error.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\execute.obj:	$(SRC)\execute.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\linked.obj:	$(SRC)\linked.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\column.obj:	$(SRC)\column.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\mouse.obj:	$(SRC)\mouse.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\memory.obj:	$(SRC)\memory.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\nonansi.obj:	$(SRC)\nonansi.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\directry.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\prefix.obj:	$(SRC)\prefix.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\regex.obj:	$(SRC)\regex.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\alloca.obj:	$(SRC)\alloca.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\parser.obj:	$(SRC)\parser.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\print.obj:	$(SRC)\print.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\reserved.obj:	$(SRC)\reserved.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\scroll.obj:	$(SRC)\scroll.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\show.obj:	$(SRC)\show.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\single.obj:	$(SRC)\single.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\sort.obj:	$(SRC)\sort.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\target.obj:	$(SRC)\target.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\the.obj:	$(SRC)\the.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\the.ver
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) -DTHE_VERSION=\"$(VER_DOT)\" -DTHE_VERSION_DATE=\"$(VER_DATE)\" $(SRC)\$(*B).c
gui\util.obj:	$(SRC)\util.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\commutil.obj:	$(SRC)\commutil.c $(SRC)\the.h $(SRC)\command.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\getch.h $(SRC)\key.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\trace.obj:	$(SRC)\trace.c $(SRC)\the.h $(SRC)\command.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\getch.h $(SRC)\key.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\getch.obj:	$(SRC)\getch.c $(SRC)\getch.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\query.obj:	$(SRC)\query.c $(SRC)\query.h $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\query1.obj:	$(SRC)\query1.c $(SRC)\query.h $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\query2.obj:	$(SRC)\query2.c $(SRC)\query.h $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\thematch.obj:	$(SRC)\thematch.c $(SRC)\the.h $(SRC)\thematch.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\directry.obj:	$(SRC)\directry.c $(SRC)\the.h $(SRC)\directry.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\file.obj:	$(SRC)\file.c $(SRC)\the.h $(SRC)\directry.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\rexx.obj:	$(SRC)\rexx.c $(SRC)\the.h $(SRC)\therexx.h $(SRC)\proto.h $(SRC)\thedefs.h $(SRC)\query.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\mygetopt.obj:	$(SRC)\mygetopt.c
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\$(*B).c
gui\DragAndDrop.obj:	$(SRC)\contrib\DragAndDrop.c
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\contrib\$(*B).c
gui\utf8.obj:	$(SRC)\contrib\utf8.c $(SRC)\contrib\utf8.h
	$(CC) -DUSE_WINGUICURSES $(CFLAGS) $(INCGUI) $(SRC)\contrib\$(*B).c
#########################################################################
# Rules for building the CONSOLE version of THE
thec.exe: console $(CONSOLEOBJS) $(THERES) $(THEWINOBJ)
	$(LD) /NOLOGO /VERSION:$(VER_DOT) $(LDEBUG) $(CONSOLEOBJS) $(THEWINOBJ) $(SETARGV) -out:$*.exe $(LDEXTRA) $(CURSCON)\$(PDCURSES_LIB) $(REXXLIB) user32.lib gdi32.lib comdlg32.lib winspool.lib wsock32.lib shell32.lib advapi32.lib kernel32.lib $(EXTRALIB)
#------------------------------------------------------------------------
console\box.obj:	$(SRC)\box.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\colour.obj:	$(SRC)\colour.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\comm1.obj:	$(SRC)\comm1.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\comm2.obj:	$(SRC)\comm2.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\comm3.obj:	$(SRC)\comm3.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\comm4.obj:	$(SRC)\comm4.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\comm5.obj:	$(SRC)\comm5.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\commset1.obj:	$(SRC)\commset1.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\commset2.obj:	$(SRC)\commset2.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\commsos.obj:	$(SRC)\commsos.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\cursor.obj:	$(SRC)\cursor.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\default.obj:	$(SRC)\default.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\edit.obj:	$(SRC)\edit.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\error.obj:	$(SRC)\error.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\execute.obj:	$(SRC)\execute.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\linked.obj:	$(SRC)\linked.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\column.obj:	$(SRC)\column.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\mouse.obj:	$(SRC)\mouse.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\memory.obj:	$(SRC)\memory.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\nonansi.obj:	$(SRC)\nonansi.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\directry.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\prefix.obj:	$(SRC)\prefix.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\regex.obj:	$(SRC)\regex.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\alloca.obj:	$(SRC)\alloca.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\parser.obj:	$(SRC)\parser.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\print.obj:	$(SRC)\print.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\reserved.obj:	$(SRC)\reserved.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\scroll.obj:	$(SRC)\scroll.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\show.obj:	$(SRC)\show.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\single.obj:	$(SRC)\single.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\sort.obj:	$(SRC)\sort.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\target.obj:	$(SRC)\target.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\the.obj:	$(SRC)\the.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\the.ver
	$(CC) $(CFLAGS) $(INCCONSOLE) -DTHE_VERSION=\"$(VER_DOT)\" -DTHE_VERSION_DATE=\"$(VER_DATE)\" $(SRC)\$(*B).c
console\util.obj:	$(SRC)\util.c $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\commutil.obj:	$(SRC)\commutil.c $(SRC)\the.h $(SRC)\command.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\getch.h $(SRC)\key.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\trace.obj:	$(SRC)\trace.c $(SRC)\the.h $(SRC)\command.h $(SRC)\thedefs.h $(SRC)\proto.h $(SRC)\getch.h $(SRC)\key.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\getch.obj:	$(SRC)\getch.c $(SRC)\getch.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\query.obj:	$(SRC)\query.c $(SRC)\query.h $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\query1.obj:	$(SRC)\query1.c $(SRC)\query.h $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\query2.obj:	$(SRC)\query2.c $(SRC)\query.h $(SRC)\the.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\thematch.obj:	$(SRC)\thematch.c $(SRC)\the.h $(SRC)\thematch.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\directry.obj:	$(SRC)\directry.c $(SRC)\the.h $(SRC)\directry.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\file.obj:	$(SRC)\file.c $(SRC)\the.h $(SRC)\directry.h $(SRC)\thedefs.h $(SRC)\proto.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\rexx.obj:	$(SRC)\rexx.c $(SRC)\the.h $(SRC)\therexx.h $(SRC)\proto.h $(SRC)\thedefs.h $(SRC)\query.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\mygetopt.obj:	$(SRC)\mygetopt.c
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\$(*B).c
console\DragAndDrop.obj:	$(SRC)\contrib\DragAndDrop.c
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\contrib\$(*B).c
console\utf8.obj:	$(SRC)\contrib\utf8.c $(SRC)\contrib\utf8.h
	$(CC) $(CFLAGS) $(INCCONSOLE) $(SRC)\contrib\$(*B).c

$(THERES) $(THEWINOBJ): $(THERC)
	copy $(SRC)\thewin.ico
	rc /r /fo$(THERES) /i$(SRC) $(THERC)
	cvtres /MACHINE:$(MACH) /NOLOGO /OUT:$(THEWINOBJ) $(THERES)
#
#########################################################################
manual:	$(MAN) $(SRC)\overview $(COMM) $(APPENDIX) $(GLOSSARY)
	manext $(SRC)\overview $(COMM) $(APPENDIX) $(GLOSSARY) > the.man
#
$(MAN):	$(XTRAOBJ) manext.$(OBJ)
	$(MANLD)
	$(CHMODMAN)

dist: the.exe theg.exe thec.exe
	-mkdir tmpdir
	cd tmpdir
	-del /Q *.*
	copy ..\the.exe .
	copy ..\thec.exe .
	copy ..\theg.exe .
	copy $(SRC)\*.the .
	copy $(SRC)\syntax\*.syntax .
	copy $(SRC)\*.tld .
	copy $(SRC)\README .
	copy $(SRC)\COPYING .
	copy $(SRC)\HISTORY .
	copy $(SRC)\INSTALL.Win .\INSTALL.txt
	copy $(SRC)\TODO .
!if "$(INT)" == "REXXTRANS"
	copy $(REXXTRANSBIN)\rexxtrans.dll .
!endif
!if "$(INT)" == "REGINA"
	copy $(REGINA_BINDIR)\regina.dll .
!endif
	-copy $(PDCURSESBIN)\pdcurses.dll .
	copy $(SRC)\THE_Help.txt .
	copy $(SRC)\demo.txt .
#	copy $(SRC)\win32.diz file_id.diz
	copy $(SRC)\doc\THE-$(VER_DOT).pdf THE.pdf
#	the -b -p $(SRC)\fix.diz -a "$(VER) $(VER_DOT) T any available Rexx interpreter" file_id.diz
	zip THE$(VER)$(LARCH)_$(REXXINT) *
	copy $(SRC)\the.nsi .
	makensis $(CURSESDLLINNSIS) /DVERSION=$(VER_DOT) /DNODOTVER=$(VER) /DSRCDIR=$(SRC) /DARCH=$(LARCH) /DINTERPRETER=$(REXXINT) the.nsi
	cd ..

clean:
	-del *.obj
	-del gui/*.obj
	-del console/*.obj
	-del theg.exe
	-del thec.exe
	-del the.exe

gui:
	-mkdir gui

console:
	-mkdir console

sdl:
	-mkdir sdl

