// TEMA.cpp: implementation of the CTEMA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "indi_func.h"
#include "TEMA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTEMA::CTEMA()
{
	m_strName = "TEMA";
}

CTEMA::~CTEMA()
{

}

CString CTEMA::GetInputs(long lType)
{
//	return "Numeric, Numeric=close";
	return "Numeric, Numeric";
}

/*-----------------------------------------------------------------------------
 Author		: Hui-Eun, Lee	Date :2005.7.11
 Modifier		: 
 Comments		: {{{ TEMA = EMA�� EMA�� EMA }}}
-----------------------------------------------------------------------------*/
double CTEMA::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	long lPeriod = 	_variant_t(varArg1);
	double dValue = _variant_t(varArg2);
	
	if(pIController->IsFirstInThisMem())	// init
	{
		pIController->SetVariables(0,"Value1", NumericSeries,dValue);		
		pIController->SetVariables(1,"Value2", NumericSeries,0);		
		pIController->SetVariables(2,"Value3", NumericSeries,0);		
		pIController->SetVariables(3,"Opt1", NumericSeries,0);		
		pIController->SetVariables(4,"Opt2", NumericSeries,0);		
		pIController->SetVariables(5,"Opt3", NumericSeries,0);		
	}	
	pIController->SetVarHolderData(0,0,dValue);
	return yesTEMA(pIController->IsUpdateData()
		,pIController->BarIndex()
		,lPeriod
		,pIController->GetVarHolderDataItem(0)
		,pIController->GetVarHolderDataItem(1)
		,pIController->GetVarHolderDataItem(2)
		,pIController->GetVarHolderDataItem(3)
		,pIController->GetVarHolderDataItem(4)
		,pIController->GetVarHolderDataItem(5)
		,pIController->GetITempMemMng());
}
