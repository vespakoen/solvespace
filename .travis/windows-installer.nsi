!include MUI2.nsh
!include "x64.nsh"

InstallDirRegKey HKCU "Software\SolveSpace\SolveSpace" ""

; The name of the installer
Name "SolveSpace"

; The Installer to create
OutFile "SolveSpace.exe"

; The default installation directory
InstallDir $PROGRAMFILES64\SolveSpace

RequestExecutionLevel admin

XPStyle on

; Set best compression
SetCompressor lzma

; Do not automatically jump to the finish page, to allow the user to check the install log.
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_UNFINISHPAGE_NOAUTOCLOSE

Var StartMenuFolder

; Pages
!insertmacro MUI_PAGE_LICENSE ..\COPYING.txt
!insertmacro MUI_PAGE_DIRECTORY

;Start Menu Folder Page Configuration
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\SolveSpace\SolveSpace" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "SolveSpace"
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_RESERVEFILE_LANGDLL


Section "Main program" Section_Main
	SectionIn 1

	; Set output path to the installation directory.
	SetOutPath $INSTDIR

	; Store install path in registry
	WriteRegStr HKCU "Software\SolveSpace\SolveSpace" "" $INSTDIR

	; Put files there
	File ..\build\bin\SolveSpace.exe
	; File C:\Windows\System32\libomp.dll
	WriteUninstaller "$INSTDIR\Uninstall.exe"

	WriteRegStr HKCU "Software\SolveSpace\SolveSpace" "ExternalEXEPath" "$INSTDIR"

	; Insert links in start menu
	!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
		CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\SolveSpace.lnk" "$INSTDIR\SolveSpace.exe"
		CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    !insertmacro MUI_STARTMENU_WRITE_END

	; Write uninstall routine and some additional info into registry
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SolveSpace" "DisplayName" "SolveSpace"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SolveSpace" "UninstallString" '"$INSTDIR\Uninstall.exe"'
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SolveSpace" "QuietUninstallString" "$\"$INSTDIR\Uninstall.exe$\" /S"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SolveSpace" "InstallLocation" "$\"$INSTDIR$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SolveSpace" "DisplayIcon" "$\"$INSTDIR\SolveSpace.exe$\""
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SolveSpace" "Publisher" "SolveSpace"
	; WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SolveSpace" "DisplayVersion" "${VER_MAJOR}.${VER_MINOR}.${VER_BUILD}"
	WriteRegDword HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SolveSpace" "NoModify" "1"
	WriteRegDword HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SolveSpace" "NoRepair" "1"
SectionEnd

LangString Message64bit ${LANG_ENGLISH} "SolveSpace is a 64-bit program and can't be installed on a 32 bit version Windows."
LangString Message64bit ${LANG_FRENCH} "SolveSpace est un programme 64-bit et ne peut pas être installée sur une version 32 bit de Windows."
LangString Message64bit ${LANG_GERMAN} "SolveSpace ist ein 64-bit Programm und kann nicht auf einer 32-Bit-Version von Windows installiert werden."

Function .onInit
	${If} ${RunningX64}
		SetRegView 64
	${Else}
		MessageBox MB_OK $(Message64bit)
		Abort
	${EndIf}
	!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "Uninstall" Uninstall
	Delete "$INSTDIR\Uninstall.exe"
	Delete "$INSTDIR\*omp*.dll"
	Delete "$INSTDIR\SolveSpace.exe"
	RMDir "$INSTDIR"

	!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
	Delete "$SMPROGRAMS\$StartMenuFolder\Licenses.lnk"
	Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
	Delete "$SMPROGRAMS\$StartMenuFolder\SolveSpace.lnk"
	RMDir "$SMPROGRAMS\$StartMenuFolder"
	DeleteRegKey /ifempty HKCU "Software\SolveSpace\SolveSpace"
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SolveSpace"
SectionEnd

Function un.onInit
	${If} ${RunningX64}
		SetRegView 64
	${EndIf}
    !insertmacro MUI_UNGETLANGUAGE
FunctionEnd
