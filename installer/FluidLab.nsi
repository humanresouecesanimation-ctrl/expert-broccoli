Unicode True
!include "MUI2.nsh"

!define APPNAME "FluidLab"
!define VERSION "0.2.0"
!define COMPANY "XYZ Creative Group"

Name "${APPNAME} ${VERSION}"
OutFile "..\dist\FluidLab-Setup.exe"
InstallDir "$PROGRAMFILES64\${APPNAME}"
InstallDirRegKey HKLM "Software\${COMPANY}\${APPNAME}" "InstallDir"
RequestExecutionLevel admin
SetCompressor /SOLID lzma

!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Section "FluidLab" SecMain
  SetOutPath "$INSTDIR"
  File "..\dist\app\FluidLab.exe"
  File /nonfatal "..\dist\app\*.dll"
  CreateDirectory "$SMPROGRAMS\${APPNAME}"
  CreateShortcut "$SMPROGRAMS\${APPNAME}\FluidLab.lnk" "$INSTDIR\FluidLab.exe"
  CreateShortcut "$DESKTOP\FluidLab.lnk" "$INSTDIR\FluidLab.exe"
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  WriteRegStr HKLM "Software\${COMPANY}\${APPNAME}" "InstallDir" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "Publisher" "${COMPANY}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
SectionEnd

Section "Uninstall"
  Delete "$INSTDIR\FluidLab.exe"
  Delete "$INSTDIR\*.dll"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir "$INSTDIR"
  Delete "$DESKTOP\FluidLab.lnk"
  Delete "$SMPROGRAMS\${APPNAME}\FluidLab.lnk"
  RMDir "$SMPROGRAMS\${APPNAME}"
  DeleteRegKey HKLM "Software\${COMPANY}\${APPNAME}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
SectionEnd

