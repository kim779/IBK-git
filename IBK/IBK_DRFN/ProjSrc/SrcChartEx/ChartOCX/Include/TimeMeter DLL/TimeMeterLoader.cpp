// TimeMeterLoader.cpp: implementation of the CTimeMeterLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

// (2003.12.28, ��¿�) TimeMeter DLL Loader�� ���Ǹ� Macro�� ���� ����� �� �ֵ���,
//		TimeMeterLoader.h�� �ƴ� TimeMeterTarget.h�� Include�Ѵ�.
//#include "TimeMeterLoader.h"
#include "TimeMeterTarget.h"

#if _TIMEMETER_USE

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.28
//
//	����	:	TimeMeter DLL�� �⺻���� Loading�� �� �ֵ��� Global�� TimeMeter Loader�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
CTimeMeterLoader	g_dllTimeMeter;

#endif