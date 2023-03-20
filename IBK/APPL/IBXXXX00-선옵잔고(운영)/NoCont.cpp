
#include "stdafx.h"
#include "NoCont.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CNoCont::CNoCont()
{
	m_NoContMap.RemoveAll();
}

CNoCont::~CNoCont()
{
	m_NoContMap.RemoveAll();
}

void CNoCont::ParsingNotice(CString sJCode, CString sMemeGubn, CString sStatus, int nAmount)
{
	if (!sMemeGubn.Compare("�ŵ�"))
		sJCode += "1";
	else
		sJCode += "2";

	int nSrcNoCont = 0;
	m_NoContMap.Lookup(sJCode, (void*&)nSrcNoCont);

	if (!sStatus.Compare("ü��"))
		nSrcNoCont -= nAmount;
	else if (!sStatus.Compare("����Ȯ��"))
		nSrcNoCont -= nAmount;
	else if (!sStatus.Compare("���Ȯ��"))
		nSrcNoCont -= nAmount * 2;
	else if (!sStatus.Compare("����"))
		nSrcNoCont += nAmount;
	else if (!sStatus.Compare("�ź�"))
		nSrcNoCont -= nAmount;

	m_NoContMap.SetAt(sJCode, (void*)nSrcNoCont);
}

void CNoCont::SetNoCont(CString sJCode, CString sMemeGubn, int nAmount)
{
	if (!nAmount)
		return;
	if (!sMemeGubn.Compare("�ŵ�"))
		sJCode += "1";
	else
		sJCode += "2";

	int nSrcNoCont = 0;
	m_NoContMap.Lookup(sJCode, (void*&)nSrcNoCont);
	m_NoContMap.SetAt(sJCode, (void*)(nSrcNoCont + nAmount));
}

int CNoCont::GetNoCont(CString sJCode, CString sMemeGubn)
{
	if (!sMemeGubn.Compare("�ŵ�"))
		sJCode += "1";
	else
		sJCode += "2";

	int nSrcNoCont = 0;
	m_NoContMap.Lookup(sJCode, (void*&)nSrcNoCont);
	return nSrcNoCont;
}

void CNoCont::Clear()
{
	m_NoContMap.RemoveAll();
}
