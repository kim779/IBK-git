#if !defined(AFX_INTERFACE_OF_STRUNMANAGER_BY_JUNOKLEE_060721A__INCLUDED_)
#define AFX_INTERFACE_OF_STRUNMANAGER_BY_JUNOKLEE_060721A__INCLUDED_

// #include "../../inc/ISTRunManager.h" // ����������� �������̽� ����.

interface ISTRunManager
{
	// Return Value : 0(Sucess), 0�̿��ǰ�(Error Code)
	// pInData : STMNG_ITEMINFO*
	STDMETHOD_(int, RunST)(BOOL bImsiRun, CString &szRunFile, LONG pInData)  PURE;

	// ���������� �����´�.
	// ���ϰ��� STMNG_ITEMINFO* ����.
	STDMETHOD_(LONG, GetSelectedSTInfo) () PURE;

	// View Handle�� ������
	STDMETHOD_(LONG, GetViewHandle) () PURE;

	// OutBar Color
	STDMETHOD_(LONG, GetOutBarColor) (COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine) PURE;
};

#endif // AFX_INTERFACE_OF_STRUNMANAGER_BY_JUNOKLEE_060721A__INCLUDED_