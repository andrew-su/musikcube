SetCompressor /SOLID lzma

!include "MUI.nsh"
!include "FileFunc.nsh"
!insertmacro DirState


!define TRUE 1
!define FALSE 0

!define MUI_BGCOLOR "FFFFFF"

!define PROJECT_NAME "musikCube 2"
!define SUB_NAME "developers milestone 2"
!define INSTALLER_NAME "mC2dm2"
!define INSTALL_DIR "musikCube 2"

;----------------------------------------------------------------
!define MC2_DB_DIR "$APPDATA\mC2"

OutFile ".\${INSTALLER_NAME}.exe"

Name "${PROJECT_NAME} ${SUB_NAME}"

ShowInstDetails show	;show/hide

Var RemoveOldDatabases

!include "LanguageStrings.nsh"
!insertmacro MUI_LANGUAGE "English"

InstallDir "$PROGRAMFILES\${INSTALL_DIR}"
InstallDirRegKey HKCU "Software\${INSTALL_DIR}" ""

; Installation pages order
!insertmacro MUI_PAGE_LICENSE "..\LICENSE.txt"
!insertmacro MUI_PAGE_DIRECTORY
Page custom RemoveOldFilesPage RemoveOldFilesLeave  ;Custom page
;!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES


Section "mC2installation" main

	SectionIn RO

	SetShellVarContext current

	SetOverwrite on

	SetOutPath "$INSTDIR"
	File /r "..\bin\release\mC2.exe"
	File /r "..\bin\release\musikServer.exe"
	File /r "..\LICENSE.txt"
	File /r "..\bin\release\resources"

	SetOutPath "$INSTDIR\plugins"
	Delete "*.dll"
	File /r "..\bin\release\plugins\*.dll"

	SetAutoClose false


	IntCmpU $RemoveOldDatabases 0 DoNotRemoveDBFiles
	; Remove the app data
	RMDir /r "${MC2_DB_DIR}"
	DoNotRemoveDBFiles:


	CreateDirectory "$SMPROGRAMS\${PROJECT_NAME} ${SUB_NAME}"
	CreateShortCut "$SMPROGRAMS\${PROJECT_NAME} ${SUB_NAME}\${PROJECT_NAME}.lnk" "$INSTDIR\mC2.exe"
	CreateShortCut "$SMPROGRAMS\${PROJECT_NAME} ${SUB_NAME}\musikServer.lnk" "$INSTDIR\musikServer.exe"

SectionEnd

Section -Post
	WriteRegStr HKCU "Software\${INSTALL_DIR}" "" $INSTDIR
	WriteUninstaller "$INSTDIR\uninst.exe"
	CreateDirectory "$SMPROGRAMS\${PROJECT_NAME} ${SUB_NAME}"
  	CreateShortCut "$SMPROGRAMS\${PROJECT_NAME} ${SUB_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section Uninstall
	SetShellVarContext current
	RMDir /r "$INSTDIR"
	RMDir /r "$SMPROGRAMS\${PROJECT_NAME} ${SUB_NAME}"
SectionEnd

Function .onInit
	!insertmacro MUI_INSTALLOPTIONS_EXTRACT "remove_old_db.ini"
FunctionEnd

Function RemoveOldFilesPage
        ${DirState} "${MC2_DB_DIR}" $0
        IntCmp $0 -1 0 +2 +2
           Abort
	!insertmacro MUI_HEADER_TEXT "mC2 installation" "Removing old database files"
	!insertmacro MUI_INSTALLOPTIONS_WRITE "remove_old_db.ini" "Field 2" "Text" "Remove old mC2 databases in ${MC2_DB_DIR}"
	!insertmacro MUI_INSTALLOPTIONS_DISPLAY "remove_old_db.ini"
FunctionEnd

Function RemoveOldFilesLeave
		!insertmacro MUI_INSTALLOPTIONS_READ $RemoveOldDatabases "remove_old_db.ini" "Field 2" "State"
FunctionEnd
