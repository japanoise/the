#
# Makefile for The Hessling Editor (THE)
#
# Using MS Visual C++ under Win32
# Usage:
#    nmake -f [DEBUG=N|Y] [TRACE=N|Y] [STATIC=N] [INT=REGINA|REXXTRANS|OOREXX|OOREXX40|OREXX|WINREXX]
#
#
SRCDIR=$(THE_SRCDIR)
LARCH=w32
BARCH=W32
WARCH=win32
MACH=IX86
!include $(SRCDIR)\vcwin.mak
