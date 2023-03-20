// NewsObj.h: interface for the CNewsObj class.
//
//////////////////////////////////////////////////////////////////////
//#include "CISBitmap.h"

#if !defined(AFX_NEWSOBJ_H__52F70937_49A7_4319_804F_4A5DC080C42A__INCLUDED_)
#define AFX_NEWSOBJ_H__52F70937_49A7_4319_804F_4A5DC080C42A__INCLUDED_

//#include "Linear.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afxwin.h>

#define HEIGHT	16
#define WIDTH	16

class CNewsObj : public CObject  
{
	DECLARE_SERIAL(CNewsObj);
public:
	void SetDate(long lDate/*IN*/);
	CNewsObj();
	virtual ~CNewsObj();

	CDC* m_pdcMem;
	CDC m_dcTransMem;
	CBitmap m_bitmap;	
	CBitmap* m_pOldBmp;
	int m_nIndex;		// index
	long m_lxData;		// date
	double m_dyData;	
	long m_lCntNews;	// news count
	CPoint m_ptAt;		//NewsObj�� ��ġ
	CSize m_size;		//NewsObj �׸��� ũ��
	BOOL m_bSelected;	// ���� ����
	CString m_strDate;
	void Draw(CDC* pDC);
	void Erase(CDC* pDC);
	BOOL IsInRegion(CPoint point);
	void ButtonDownAt(const CPoint& pt);
	void ButtonUp() { m_bDrag = FALSE; }
	void SetPos(const CPoint& ptNew);
	CPoint GetPos() { return m_ptAt; }
	CRect GetRect() { return CRect(m_ptAt, m_size); }
	BOOL Initialize();
	static void Release();

protected:
	BOOL m_bDrag;		//�巡�� ����
	CPoint m_ptOffset;	//NewsObj ��ġ�� Ŭ���� ��ġ�� ���� 
	COLORREF	m_crBlack;
	COLORREF	m_crWhite;
	CRect m_rc;			// �׸��� ����
};

//////////////////////////////////////////////////////////////////////////
// Object list class
class CNewsObjList : public CObList
{
public:
	CNewsObj* GetNextServer(POSITION pos);	//NewsObj �߿��� ���� ���� ���
	CNewsObj* FindNewsObj(DWORD type);		// ���ϴ� �߷��� NewsObjã��
	int GetNewsObjCount(DWORD type);		//���ϴ� ������ NewsObj�� ��
	int GetTotalNewsObj();					// ��ü NewsObj ��
	void DeleteAll();
	void AddHeadNewsObj(CNewsObj* pNewsObj){ AddHead(pNewsObj); }
	void SetAtNewsObj(POSITION pos, CNewsObj* pNewsObj){ SetAt(pos, pNewsObj);}
	CNewsObj* GetHeadNewsObj(){ return (CNewsObj*)GetHead(); }
	CNewsObj* GetNextNewsObj(POSITION& pos){ return (CNewsObj*)GetNext(pos); }
	CNewsObj* GetPrevNewsObj(POSITION& pos){ return (CNewsObj*)GetPrev(pos); }

};

#endif // !defined(AFX_NEWSOBJ_H__52F70937_49A7_4319_804F_4A5DC080C42A__INCLUDED_)
