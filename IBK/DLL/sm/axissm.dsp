# Microsoft Developer Studio Project File - Name="axissm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=axissm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "axissm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "axissm.mak" CFG="axissm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "axissm - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "axissm - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "axissm - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /Zi /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 Imm32.lib ../axiscm/Release/axiscm.lib /nologo /base:"0x00500000" /subsystem:windows /dll /incremental:yes /map /debug /machine:I386 /def:".\axissm.def" /out:"..\..\..\release\axissm.dll"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "axissm - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Fr /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Imm32.lib ../axiscm/Debug/axiscm.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\exe\axissm.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "axissm - Win32 Release"
# Name "axissm - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pptooltip\CeXDib.cpp
# End Source File
# Begin Source File

SOURCE=.\pptooltip\PPDrawManager.cpp
# End Source File
# Begin Source File

SOURCE=.\pptooltip\PPHtmlDrawer.cpp
# End Source File
# Begin Source File

SOURCE=.\pptooltip\PPTooltip.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\1Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\AcntDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\axGuide.cpp
# End Source File
# Begin Source File

SOURCE=.\axissm.cpp
# End Source File
# Begin Source File

SOURCE=.\axissm.def

!IF  "$(CFG)" == "axissm - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "axissm - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\axissm.rc
# End Source File
# Begin Source File

SOURCE=.\Basebar0.cpp
# End Source File
# Begin Source File

SOURCE=.\CategoryPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Dbar1.cpp
# End Source File
# Begin Source File

SOURCE=.\Dbar2.cpp
# End Source File
# Begin Source File

SOURCE=.\Dbar3.cpp
# End Source File
# Begin Source File

SOURCE=.\EnBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\eticker.cpp
# End Source File
# Begin Source File

SOURCE=.\fxIGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\fxIGrid2.cpp
# End Source File
# Begin Source File

SOURCE=.\hBand.cpp
# End Source File
# Begin Source File

SOURCE=.\IhUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcessors.cpp
# End Source File
# Begin Source File

SOURCE=.\iMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Link.cpp
# End Source File
# Begin Source File

SOURCE=..\..\appl\AxCommon\MDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\MDIClient.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuXP.cpp
# End Source File
# Begin Source File

SOURCE=.\NButton.cpp
# End Source File
# Begin Source File

SOURCE=.\NewListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\NewsHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\NewsListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\NPalette.cpp
# End Source File
# Begin Source File

SOURCE=.\Pane.cpp
# End Source File
# Begin Source File

SOURCE=.\RegCode.cpp
# End Source File
# Begin Source File

SOURCE=.\RegMap.cpp
# End Source File
# Begin Source File

SOURCE=.\RenameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RunX.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenList.cpp
# End Source File
# Begin Source File

SOURCE=.\SDIBand.cpp
# End Source File
# Begin Source File

SOURCE=.\Sdibar.cpp
# End Source File
# Begin Source File

SOURCE=.\SEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTickPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Smain.cpp
# End Source File
# Begin Source File

SOURCE=.\Smcall.cpp
# End Source File
# Begin Source File

SOURCE=.\SmSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SVband.cpp
# End Source File
# Begin Source File

SOURCE=.\Terminator.cpp
# End Source File
# Begin Source File

SOURCE=.\TInfo1.cpp
# End Source File
# Begin Source File

SOURCE=.\TInfo2.cpp
# End Source File
# Begin Source File

SOURCE=.\TInfo3.cpp
# End Source File
# Begin Source File

SOURCE=.\TMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\TransparentMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TrSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\UseStopPane.cpp
# End Source File
# Begin Source File

SOURCE=.\Xcom.cpp
# End Source File
# Begin Source File

SOURCE=.\Xcomex.cpp
# End Source File
# Begin Source File

SOURCE=.\Xedit.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Controls_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pptooltip\CeXDib.h
# End Source File
# Begin Source File

SOURCE=.\pptooltip\PPDrawManager.h
# End Source File
# Begin Source File

SOURCE=.\pptooltip\PPHtmlDrawer.h
# End Source File
# Begin Source File

SOURCE=.\pptooltip\PPTooltip.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AcntDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\axGuide.h
# End Source File
# Begin Source File

SOURCE=.\Basebar0.h
# End Source File
# Begin Source File

SOURCE=.\ColorBtn.h
# End Source File
# Begin Source File

SOURCE=.\CoolDialogBar.h
# End Source File
# Begin Source File

SOURCE=.\Dbar1.h
# End Source File
# Begin Source File

SOURCE=.\Dbar2.h
# End Source File
# Begin Source File

SOURCE=.\Dbar3.h
# End Source File
# Begin Source File

SOURCE=.\EnBitmap.h
# End Source File
# Begin Source File

SOURCE=.\ESetup.h
# End Source File
# Begin Source File

SOURCE=.\eticker.h
# End Source File
# Begin Source File

SOURCE=.\fxIGrid.h
# End Source File
# Begin Source File

SOURCE=.\fxIGrid2.h
# End Source File
# Begin Source File

SOURCE=.\hBand.h
# End Source File
# Begin Source File

SOURCE=.\Hotkey.h
# End Source File
# Begin Source File

SOURCE=.\IhUtils.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessors.h
# End Source File
# Begin Source File

SOURCE=.\iMenu.h
# End Source File
# Begin Source File

SOURCE=.\Link.h
# End Source File
# Begin Source File

SOURCE=.\MDIClient.h
# End Source File
# Begin Source File

SOURCE=.\MenuXP.h
# End Source File
# Begin Source File

SOURCE=.\MsgSetup.h
# End Source File
# Begin Source File

SOURCE=.\NButton.h
# End Source File
# Begin Source File

SOURCE=.\NewListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\NewsHistory.h
# End Source File
# Begin Source File

SOURCE=.\NewsListBox.h
# End Source File
# Begin Source File

SOURCE=.\OrderSetup.h
# End Source File
# Begin Source File

SOURCE=.\Pane.h
# End Source File
# Begin Source File

SOURCE=.\PClock.h
# End Source File
# Begin Source File

SOURCE=.\Rband.h
# End Source File
# Begin Source File

SOURCE=.\RegCode.h
# End Source File
# Begin Source File

SOURCE=.\RegMap.h
# End Source File
# Begin Source File

SOURCE=.\RenameDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RunX.h
# End Source File
# Begin Source File

SOURCE=.\ScreenList.h
# End Source File
# Begin Source File

SOURCE=.\SDIBand.h
# End Source File
# Begin Source File

SOURCE=.\Sdibar.h
# End Source File
# Begin Source File

SOURCE=.\SEdit.h
# End Source File
# Begin Source File

SOURCE=.\SetTickInfo.h
# End Source File
# Begin Source File

SOURCE=.\SetTickPopup.h
# End Source File
# Begin Source File

SOURCE=.\SimpleSplitter.h
# End Source File
# Begin Source File

SOURCE=.\SkinDlg.h
# End Source File
# Begin Source File

SOURCE=.\Smain.h
# End Source File
# Begin Source File

SOURCE=.\Smcall.h
# End Source File
# Begin Source File

SOURCE=.\smdefine.h
# End Source File
# Begin Source File

SOURCE=.\smheader.h
# End Source File
# Begin Source File

SOURCE=.\SmSheet.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SVband.h
# End Source File
# Begin Source File

SOURCE=.\TargetItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\Terminator.h
# End Source File
# Begin Source File

SOURCE=.\TInfo1.h
# End Source File
# Begin Source File

SOURCE=.\TInfo2.h
# End Source File
# Begin Source File

SOURCE=.\TInfo3.h
# End Source File
# Begin Source File

SOURCE=.\TMenu.h
# End Source File
# Begin Source File

SOURCE=.\TransparentMgr.h
# End Source File
# Begin Source File

SOURCE=.\TrSearch.h
# End Source File
# Begin Source File

SOURCE=.\TSetup.h
# End Source File
# Begin Source File

SOURCE=.\UseStopPane.h
# End Source File
# Begin Source File

SOURCE=.\Xcom.h
# End Source File
# Begin Source File

SOURCE=.\Xcomex.h
# End Source File
# Begin Source File

SOURCE=.\Xedit.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\2_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\3_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\4_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\actskin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\alist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\axissm.rc2
# End Source File
# Begin Source File

SOURCE=.\res\bar1back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bar2back.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bar2button.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bk_lb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bk_lb_empty.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bk_lt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bk_rb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bk_rb_empty.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bk_rt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cimg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cimg_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\comarrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Darr_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Darr_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edit.ico
# End Source File
# Begin Source File

SOURCE=.\res\expand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\expand2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\expand_ticklist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HARROW.CUR
# End Source File
# Begin Source File

SOURCE=.\res\HTS_ICON.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HTS_ICON_CUS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\infoback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\infoback_bottom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\itemTitle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\medal.ico
# End Source File
# Begin Source File

SOURCE=.\res\menubk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\new.ico
# End Source File
# Begin Source File

SOURCE=.\res\reflesh_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\reflesh_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\reflesh_o.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sclose.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SDI_CLOSE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SDI_PIN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SDI_RETURN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sdiexpand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sdivscreen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sexpand.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sicon.ico
# End Source File
# Begin Source File

SOURCE=.\res\skin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\spin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sscreensave.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sswitch.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ticklist.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbox.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treeicon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Uarr_down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Uarr_up.bmp
# End Source File
# Begin Source File

SOURCE=.\res\vitualscreenimg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\web.ico
# End Source File
# Begin Source File

SOURCE=".\res\÷������.bmp"
# End Source File
# End Group
# Begin Group "Ctrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ctrl\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\bitmap.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExCombo.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExDialog.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\ExTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\listctrlSort.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\listctrlSort.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\listEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\listEdit.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\listHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\listHeader.h
# End Source File
# Begin Source File

SOURCE=..\ctrl\palette.cpp
# End Source File
# Begin Source File

SOURCE=..\ctrl\palette.h
# End Source File
# Begin Source File

SOURCE=.\SComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\xSkinButton.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
