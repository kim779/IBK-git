========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : Lib_SignalMng
========================================================================


AppWizard has created this Lib_SignalMng DLL for you.  This DLL not only
demonstrates the basics of using the Microsoft Foundation classes but
is also a starting point for writing your DLL.

This file contains a summary of what you will find in each of the files that
make up your Lib_SignalMng DLL.

Lib_SignalMng.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

Lib_SignalMng.h
	This is the main header file for the DLL.  It declares the
	CLib_SignalMngApp class.

Lib_SignalMng.cpp
	This is the main DLL source file.  It contains the class CLib_SignalMngApp.


Lib_SignalMng.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

Lib_SignalMng.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\Lib_SignalMng.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.

Lib_SignalMng.def
    This file contains information about the DLL that must be
    provided to run with Microsoft Windows.  It defines parameters
    such as the name and description of the DLL.  It also exports
	functions from the DLL.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Lib_SignalMng.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
@유진관련작업.
	메인신호 관련 스펙.
	u2\소리마치\요건정의 및 설계\메인_신호처리기_남궁중.doc
	메인에서 받을 구조체  : SM_SIGNALMSGINFO