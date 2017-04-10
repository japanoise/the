;
; THE Install Script, based on Modern Example Script Written by Joost Verburg
; Requires: ${NSISDIR}\Contrib\Path\path.nsi
; Run as:
;  makensis /DVERSION=x.x /DNODOTVER=xx the.nsi
; Note:
;  the.nsi MUST be in the current directory!

!define LONGNAME "The Hessling Editor"  ;Long Name (for descriptions)
!define SHORTNAME "THE" ;Short name (no slash) of package
;
; Following !defines required for uninstaller.nsh
!define SFGROUP     "29648"
!define SFHOME      "http://hessling-editor.sourceforge.net"
!define DISPLAYICON "$INSTDIR\${SHORTNAME}.exe,0"
!define UNINSTALLER "${SHORTNAME}un.exe"
!define KEYFILE     "${SHORTNAME}.exe"
;
Name "${LONGNAME} ${VERSION}"

!define MUI_ICON "${SRCDIR}\thewin.ico"
!define MUI_UNICON "${SRCDIR}\thewinun.ico"

!include "MUI.nsh"
!include "Library.nsh"

!define MUI_CUSTOMPAGECOMMANDS

!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_UNFINISHPAGE_NOAUTOCLOSE

Var ALREADY_INSTALLED

;Modern UI System

; VERSION  ;Must be supplied on compile command line
; NODOTVER ;Must be supplied on compile command line

;--------------------------------
;Configuration

  ;General
  OutFile "${SHORTNAME}${NODOTVER}${ARCH}_${INTERPRETER}.exe"
  ShowInstdetails show
  SetOverwrite on


  ;License dialog
  LicenseData "COPYING"


  ;Folder-select dialog
  InstallDir "c:\${SHORTNAME}"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "COPYING"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Language
  !insertmacro MUI_LANGUAGE "English"

;================
;Variables
  Var IsAdminUser


;========================================================================
;Installer Sections

;------------------------------------------------------------------------
; Core

Section "${LONGNAME} Core (required)" SecMain
  SectionIn 1 RO
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  ; add the shared DLL; rexxtrans.dll BEFORE installing the.exe
  ; so that we don't get a false positive that we have already installed the
  ; application
  ; install rexxtrans.dll in $SYSDIR
  IfFileExists "$INSTDIR\the.exe" 0 new_installation
    StrCpy $ALREADY_INSTALLED 1
  new_installation:
  !if ${INTERPRETER} == "RexxTrans"
     !insertmacro InstallLib DLL $ALREADY_INSTALLED REBOOT_NOTPROTECTED rexxtrans.dll $SYSDIR\rexxtrans.dll $SYSDIR
     File rexxtrans.dll
  !endif
  ; Distribution files...
  File the.exe
  File thec.exe
  File theg.exe
  !ifdef CURSESDLL
  File pdcurses.dll
  !endif
  File COPYING
  File HISTORY
  File THE_Help.txt
  CreateDirectory "$SMPROGRAMS\${LONGNAME}"
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\The Hessling Editor.lnk" "$INSTDIR\the.exe" "" "$INSTDIR\the.exe"
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\Set GUI Default.lnk" "$INSTDIR\the.exe" "--gui-default" "$INSTDIR\the.exe"
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\Set Console Default.lnk" "$INSTDIR\the.exe" "--console-default" "$INSTDIR\the.exe"
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\Uninstall.lnk" "$INSTDIR\${UNINSTALLER}" "" "$INSTDIR\${UNINSTALLER}" 0
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\HISTORY.lnk" "$INSTDIR\the.exe" "$INSTDIR\HISTORY" "$INSTDIR\the.exe"
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\LICENSE.lnk" "$INSTDIR\the.exe" "$INSTDIR\LICENSE" "$INSTDIR\the.exe"
  ; Can't use CreateShortcut for URLs
  WriteINIStr "$SMPROGRAMS\${LONGNAME}\THE Home Page.url" "InternetShortcut" "URL" "http://hessling-editor.sourceforge.net"
  ; Write the uninstall keys
!include "${SRCDIR}\common\uninstaller.nsh"
  ; add install directory to PATH
  Push "$INSTDIR"
  Push $IsAdminUser ; "true" or "false"
  Push "PATH"
  Call AddToPath
  ; set env variables
  DetailPrint "Setting THE_HOME_DIR to $INSTDIR"
  Push "THE_HOME_DIR"
  Push "$INSTDIR"
  Push $IsAdminUser ; "true" or "false"
  Call WriteEnvStr

  DetailPrint "Setting THE_MACRO_PATH to $INSTDIR\extras"
  Push "THE_MACRO_PATH"
  Push "$INSTDIR\extras"
  Push $IsAdminUser ; "true" or "false"
  Call WriteEnvStr

  DetailPrint "Setting THE_HELP_FILE to $INSTDIR\THE_Help.txt"
  Push "THE_HELP_FILE"
  Push "$INSTDIR\THE_Help.txt"
  Push $IsAdminUser ; "true" or "false"
  Call WriteEnvStr
SectionEnd

;------------------------------------------------------------------------
; Language Definition Files

Section "${LONGNAME} Language Definition Files" SecDemo
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\extras
  File *.tld
SectionEnd

;------------------------------------------------------------------------
; Sample Macros

Section "${LONGNAME} Sample Macros" SecDev
  CreateDirectory "$SMPROGRAMS\${LONGNAME}\Sample Profiles"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\extras
  ; Distribution files...
  File append.the
  File build.the
  File codecomp.the
  File complete.the
  File comm.the
  File compile.the
  File config.the
  File cua.the
  File demo.the
  File diff.the
  File l.the
  File match.the
  File mhprf.the
  File nl.the
  File rm.the
  File spell.the
  File syntax.the
  File tags.the
  File total.the
  File uncomm.the
  File words.the
  File xeditprf.the
  File demo.txt
  File rexx.syntax
  File rexxdw.syntax
  File rexxeec.syntax
  File rexxutil.syntax
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\Sample Profiles\XEDIT Profile.lnk" "$INSTDIR\the.exe" '-p "$INSTDIR\extras\xeditprf.the" "$INSTDIR\extras\xeditprf.the"' "$INSTDIR\the.exe"
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\Sample Profiles\CUA Profile.lnk" "$INSTDIR\the.exe" '-p "$INSTDIR\extras\cua.the" "$INSTDIR\extras\cua.the"' "$INSTDIR\the.exe"
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\Sample Profiles\Authors Profile.lnk" "$INSTDIR\the.exe" '-p "$INSTDIR\extras\mhprf.the" "$INSTDIR\extras\mhprf.the"' "$INSTDIR\the.exe"
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\Sample Profiles\THE Demo.lnk" "$INSTDIR\the.exe" '-p "$INSTDIR\extras\demo.the" "$INSTDIR\extras\demo.txt"' "$INSTDIR\the.exe"
SectionEnd

;------------------------------------------------------------------------
; Doco

Section "${LONGNAME} Documentation" SecDoc
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR\doc
  File THE.pdf
;  File ..\doc\*.html
;  File ..\doc\logo1.jpg
  CreateShortCut "$SMPROGRAMS\${LONGNAME}\${SHORTNAME} PDF Documentation.lnk" "$INSTDIR\doc\THE.pdf" "" "$INSTDIR\doc\THE.pdf" 0
SectionEnd

;------------------------------------------------------------------------
; Regina

!if ${INTERPRETER} == "Regina"
Section "Regina Rexx Interpreter" SecRegina
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  File regina.dll
SectionEnd
  !endif


Section ""

  ;Invisible section to display the Finish header
; !insertmacro MUI_FINISHHEADER

SectionEnd

;========================================================================
;Installer Functions

Function .onInit
!include "${SRCDIR}\common\oninit.nsh"
FunctionEnd

Function .onMouseOverSection

  !insertmacro MUI_DESCRIPTION_BEGIN

    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} "Installs the core components of ${LONGNAME} to the application folder."
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDemo} "Installs extra language definition files."
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDev} "Install sample ${LONGNAME} macros."
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDoc} "Install ${LONGNAME} documentation."
    !if ${INTERPRETER} == "Regina"
       !insertmacro MUI_DESCRIPTION_TEXT ${SecRegina} "Install Regina Rexx Interpreter."
    !endif

 !insertmacro MUI_DESCRIPTION_END

FunctionEnd

;========================================================================
;Uninstaller Section

Section "Uninstall"
  ; remove directory from PATH
  Push $INSTDIR
  Push $IsAdminUser ; pushes "true" or "false"
  Push "PATH"
  Call un.RemoveFromPath
  DeleteRegKey HKCR "${SHORTNAME}"
  !if ${INTERPRETER} == "RexxTrans"
     !insertmacro UnInstallLib DLL SHARED REBOOT_NOTPROTECTED $SYSDIR\rexxtrans.dll
  !endif
  ; remove environment variables
  Push "THE_MACRO_PATH"
  Push $IsAdminUser ; "true" or "false"
  Call un.DeleteEnvStr

  Push "THE_HELP_FILE"
  Push $IsAdminUser ; "true" or "false"
  Call un.DeleteEnvStr

  Push "THE_HOME_DIR"
  Push $IsAdminUser ; "true" or "false"
  Call un.DeleteEnvStr

  RMDir /r "$INSTDIR"

  ; Remove the installation stuff
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SHORTNAME}"
  DeleteRegKey HKLM "SOFTWARE\${LONGNAME}"

  ; remove shortcuts directory and everything in it
  RMDir /r "$SMPROGRAMS\${LONGNAME}"

; !insertmacro MUI_UNFINISHHEADER

SectionEnd

;========================================================================
;Uninstaller Functions

Function un.onInit
!include "${SRCDIR}\common\unoninit.nsh"
FunctionEnd

!include "${SRCDIR}\common\admin.nsh"
!include "${SRCDIR}\common\isnt.nsh"
!include "${SRCDIR}\common\path.nsh"
!include "${SRCDIR}\common\WriteEnv.nsh"

;eof
