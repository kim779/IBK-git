// SwingHighBar.cpp: implementation of the CSwingHighBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "SwingHighBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSwingHighBar::CSwingHighBar()
{
	m_strName = "SWINGHIGHBAR";
}

CSwingHighBar::~CSwingHighBar()
{

}

CString CSwingHighBar::GetInputs(long lType)
{
	return "Numeric,Numeric,Numeric,Numeric,Numeric";
}


double CSwingHighBar::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lOccur = 	_variant_t(varArg1);
	double dValue = 	_variant_t(varArg2);
	long lLeftStrength = 	_variant_t(varArg3);
	long lRightStrength = 	_variant_t(varArg4);
	long lPeriod = 	_variant_t(varArg5);

	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value",NumericSeries,dValue);		
	}
	pIController->SetVarHolderData(0,0,dValue);
	return yesSwingHighBar(pIController->IsUpdateData()
		,pIController->BarIndex(),lOccur,lPeriod,lLeftStrength,lRightStrength
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetITempMemMng());
}


