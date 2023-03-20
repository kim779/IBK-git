# Microsoft Developer Studio Project File - Name="Wizard" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Wizard - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Wizard.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Wizard.mak" CFG="Wizard - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Wizard - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Wizard - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Wizard - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FAcs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ws2_32.lib imm32.lib version.lib winmm.lib ../dll/lib/release/axislib.lib ../dll/form/release/axisform.lib ../dll/vbs/release/axisvbs.lib /nologo /subsystem:windows /dll /map /debug /machine:I386 /out:"../../release/axWizard.ocx"
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Release
TargetPath=\Program Files\IBKS\IBK hot Trading\src\release\axWizard.ocx
InputPath=\Program Files\IBKS\IBK hot Trading\src\release\axWizard.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "Wizard - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib imm32.lib version.lib winmm.lib ../dll/lib/debug/axislib.lib ../dll/form/debug/axisform.lib ../dll/vbs/debug/axisvbs.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../../exe/axWizard.ocx" /pdbtype:sept
# SUBTRACT LINK32 /map
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=\Program Files\IBKS\IBK hot Trading\exe\axWizard.ocx
InputPath=\Program Files\IBKS\IBK hot Trading\exe\axWizard.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "Wizard - Win32 Release"
# Name "Wizard - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Dde.cpp
# End Source File
# Begin Source File

SOURCE=.\Dll.cpp
# End Source File
# Begin Source File

SOURCE=.\Event.cpp
# End Source File
# Begin Source File

SOURCE=.\excel9.cpp
# End Source File
# Begin Source File

SOURCE=.\Guard.cpp
# End Source File
# Begin Source File

SOURCE=.\History.cpp
# End Source File
# Begin Source File

SOURCE=.\Key.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc.cpp
# End Source File
# Begin Source File

SOURCE=.\Mouse.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDropEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Printer.cpp
# End Source File
# Begin Source File

SOURCE=.\Screen.cpp
# End Source File
# Begin Source File

SOURCE=.\Script.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Tips.cpp
# End Source File
# Begin Source File

SOURCE=.\Wizard.cpp
# End Source File
# Begin Source File

SOURCE=.\Wizard.def
# End Source File
# Begin Source File

SOURCE=.\Wizard.odl
# End Source File
# Begin Source File

SOURCE=.\Wizard.rc
# End Source File
# Begin Source File

SOURCE=.\WizardCtl.cpp
# End Source File
# Begin Source File

SOURCE=.\WizardPpg.cpp
# End Source File
# Begin Source File

SOURCE=.\Works.cpp
# End Source File
# Begin Source File

SOURCE=.\xscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\xsystem.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\Dde.h
# End Source File
# Begin Source File

SOURCE=.\Dll.h
# End Source File
# Begin Source File

SOURCE=.\excel9.h
# End Source File
# Begin Source File

SOURCE=.\Guard.h
# End Source File
# Begin Source File

SOURCE=.\History.h
# End Source File
# Begin Source File

SOURCE=.\Key.h
# End Source File
# Begin Source File

SOURCE=.\Misc.h
# End Source File
# Begin Source File

SOURCE=.\Mouse.h
# End Source File
# Begin Source File

SOURCE=.\Objects.h
# End Source File
# Begin Source File

SOURCE=.\OleDrop.h
# End Source File
# Begin Source File

SOURCE=.\OleDropEx.h
# End Source File
# Begin Source File

SOURCE=.\Printer.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Screen.h
# End Source File
# Begin Source File

SOURCE=.\Script.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Stream.h
# End Source File
# Begin Source File

SOURCE=.\Tips.h
# End Source File
# Begin Source File

SOURCE=.\Wizard.h
# End Source File
# Begin Source File

SOURCE=.\WizardCtl.h
# End Source File
# Begin Source File

SOURCE=.\WizardPpg.h
# End Source File
# Begin Source File

SOURCE=.\Works.h
# End Source File
# Begin Source File

SOURCE=.\xscreen.h
# End Source File
# Begin Source File

SOURCE=.\xsystem.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\WizardCtl.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
