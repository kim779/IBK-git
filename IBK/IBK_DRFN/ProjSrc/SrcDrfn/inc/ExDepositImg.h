#if !defined(AFX_EXDEPOSITIMG_H__921F6F27_7901_458F_B660_F8589472B6CD__INCLUDED_)
#define AFX_EXDEPOSITIMG_H__921F6F27_7901_458F_B660_F8589472B6CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExDepositImg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExDepositImg window

class CLoaderDepositImg;
class CExDepositImg : public CStatic
{
// Construction
public:
	CExDepositImg();

// Attributes
public:

// Operations
public:

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2006/2/15
	// Return		: void
	// Paramter
	//		UINT nIndex : ���ű� �̹��� �ε��� ��ȣ
	// Comments		: DEPOSIT_20, DEPOSIT_30, DEPOSIT_40, DEPOSIT_100�߿� �Ѱ��� ������
	//				  ���ű� �̹��� ����
	//-----------------------------------------------------------------------------	
	void	SetDepositIndex(UINT nIndex);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2006/2/15
	// Return		: void
	// Paramter
	//		void
	// Comments		: ��Ʈ���� ũ�⸦ �̹����� ���� ũ��� ����
	//-----------------------------------------------------------------------------
	void	SizeToImage();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExDepositImg)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	CLoaderDepositImg*		m_pLoader;

// Implementation
public:
	virtual ~CExDepositImg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExDepositImg)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXDEPOSITIMG_H__921F6F27_7901_458F_B660_F8589472B6CD__INCLUDED_)
