!
! THE on OpenVMS
! MMS/MMK-makefile
!
.IGNORE         ! ignore errors, continue processing, like "make -k"
!
.FIRST
        @ write sys$output f$fao("!/==!AS !%D==", -
                "Processing DESCRIP.MMS (THE) begins at", 0)
.LAST
        @ write sys$output f$fao("!/==!AS !%D==", -
                "Processing DESCRIP.MMS (THE) concludes at", 0)
!
.IFDEF DEBUGGING
CC=CC/DECC
!
! with regina:
!       /INCLUDE_DIRECTORY=([],[-.REGINA])/UNSIGNED_CHAR/NAMES=SHORTENED -
!
CFLAGS=/NOOPTIMIZE/STANDARD=RELAXED_ANSI89/WARNINGS=DISABLE=(IMPLICITFUNC) -
        /INCLUDE_DIRECTORY=[]/UNSIGNED_CHAR/NAMES=SHORTENED -
        /DEFINE=VMS/DEBUG/NOLIST/OBJECT=$(MMS$TARGET_NAME).OBJ
LINK=LINK/DEBUG
.ELSE
CC=CC/DECC
CFLAGS=/OPTIMIZE/STANDARD=RELAXED_ANSI89/WARNINGS=DISABLE=(IMPLICITFUNC) -
        /INCLUDE_DIRECTORY=[]/UNSIGNED_CHAR/NAMES=SHORTENED -
        /DEFINE=VMS/NOLIST/OBJECT=$(MMS$TARGET_NAME).OBJ
.ENDIF
!
! platform dependant, ...
!
.IFDEF ALPHA
.ENDIF
!
.IFDEF VAX
.ENDIF
!

OBJ1=box.obj,colour.obj,column.obj,comm1.obj,comm2.obj,comm3.obj,comm4.obj,comm5.obj
OBJ2=commset1.obj,commset2.obj,commsos.obj,commutil.obj,cursor.obj,default.obj
OBJ3=directry.obj,edit.obj,error.obj,execute.obj,file.obj,thematch.obj,getch.obj
OBJ4=linked.obj,mouse.obj,memory.obj,nonansi.obj,parser.obj,prefix.obj,print.obj
OBJ5=query.obj,query1.obj,query2.obj,regex.obj,reserved.obj,rexx.obj,scroll.obj,show.obj,single.obj
OBJ6=sort.obj,target.obj,the.obj,util.obj,mygetopt.obj

H0=regex.h
H1=the.h,vars.h,defines.h,proto.h
H2=the.h,vars.h,command.h,defines.h,proto.h,getch.h,key.h
H3=getch.h,vars.h,defines.h,proto.h
H4=the.h,vars.h,query.h,defines.h,proto.h
H5=the.h,vars.h,thematch.h
H6=the.h,vars.h,directry.h,defines.h,proto.h
H7=the.h,vars.h,therexx.h,proto.h,defines.h,query.h
H8=the.h,vars.h,therexx.h,defines.h

!
all : the.exe
!
the.exe : $(OBJ1),$(OBJ2),$(OBJ3),$(OBJ4),$(OBJ5),$(OBJ6)
        @ write sys$output "Linking $(MMS$TARGET) "
        $(LINK) $(LINKFLAGS) $(MMS$SOURCE_LIST)
        @ write sys$output "Done (linking)."
!
clean : rexx.exe, regina.exe
        @ delete/nolog  rexx.exe;*,regina.exe;*
        @ delete/nolog *.obj;*
        @ write sys$output "Done (cleaning)."

box.obj : box.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

colour.obj : colour.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

column.obj : column.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

comm1.obj : comm1.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

comm2.obj : comm2.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

comm3.obj : comm3.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

comm4.obj : comm4.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

comm5.obj : comm5.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

commset1.obj : commset1.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

commset2.obj : commset2.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

commsos.obj : commsos.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

commutil.obj : commutil.c,$(H2)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

cursor.obj : cursor.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

default.obj : default.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

directry.obj : directry.c,$(H6)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

edit.obj : edit.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

error.obj : error.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

execute.obj : execute.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

file.obj : file.c,$(H6)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

mygetopt.obj : mygetopt.c,$(H6)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

thematch.obj : thematch.c,$(H5)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

getch.obj : getch.c,$(H3)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

linked.obj : linked.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

mouse.obj : mouse.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

memory.obj : memory.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

nonansi.obj : nonansi.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

parser.obj : parser.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

os2eas.obj : os2eas.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

prefix.obj : prefix.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

print.obj : print.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

query.obj : query.c,$(H4)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

query1.obj : query1.c,$(H4)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

query2.obj : query2.c,$(H4)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

regex.obj : regex.c,$(H0)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

reserved.obj : reserved.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

rexx.obj : rexx.c,$(H7)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

scroll.obj : scroll.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

show.obj : show.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

single.obj : single.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

sort.obj : sort.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

target.obj : target.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

the.obj : the.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

trace.obj : trace.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

util.obj : util.c,$(H1)
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

alloca.obj : alloca.c
        @ write sys$output ""
        @ write sys$output "Compiling $(MMS$SOURCE) "
        $(CC) $(CFLAGS) $(MMS$SOURCE)
        @ write sys$output "Done (compiling)."

! -- end of descrip.mms
