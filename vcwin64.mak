#
# Makefile for The Hessling Editor (THE)
#
# Using MS Visual C++ under Win64
# Usage:
#    nmake -f [DEBUG=N|Y] [TRACE=N|Y] [STATIC=N] [INT=REGINA|REXXTRANS|OOREXX|OOREXX40|OREXX|WINREXX]
#
#
SRCDIR=$(THE_SRCDIR)
LARCH=w64
BARCH=W64
WARCH=win64
MACH=X64
!include $(SRCDIR)\vcwin.mak
