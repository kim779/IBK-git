// ACos.h: interface for the CACos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACOS_H__C196F96C_0619_4B98_BB45_CC2FDBFEEB21__INCLUDED_)
#define AFX_ACOS_H__C196F96C_0619_4B98_BB45_CC2FDBFEEB21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CACos  : public CInnerFunction
{
public:
	CACos();
	virtual ~CACos();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;


};

#endif // !defined(AFX_ACOS_H__C196F96C_0619_4B98_BB45_CC2FDBFEEB21__INCLUDED_)
