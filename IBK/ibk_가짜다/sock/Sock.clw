; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSockCtrl
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Sock.h"
CDK=Y

ClassCount=5
Class1=CSockCtrl
Class2=CSockPropPage

ResourceCount=2
LastPage=0
Class3=CStream
Class4=CWsh
Class5=CDgram
Resource2=IDD_PROPPAGE_SOCK

[CLS:CSockCtrl]
Type=0
HeaderFile=SockCtl.h
ImplementationFile=SockCtl.cpp
Filter=W
LastObject=CSockCtrl
BaseClass=COleControl
VirtualFilter=wWC

[CLS:CSockPropPage]
Type=0
HeaderFile=SockPpg.h
ImplementationFile=SockPpg.cpp
Filter=D
LastObject=CSockPropPage

[DLG:IDD_PROPPAGE_SOCK]
Type=1
Class=CSockPropPage
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[CLS:CStream]
Type=0
HeaderFile=Stream.h
ImplementationFile=Stream.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=q
LastObject=CStream

[CLS:CWsh]
Type=0
HeaderFile=Wsh.h
ImplementationFile=Wsh.cpp
BaseClass=CWinThread
Filter=N
VirtualFilter=TC
LastObject=CWsh

[CLS:CDgram]
Type=0
HeaderFile=Dgram.h
ImplementationFile=Dgram.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=q
LastObject=CDgram

