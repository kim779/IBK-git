// Draw3DRectangle.h: interface for the CDraw3DRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW3DRECTANGLE_H__B4148023_98CF_440F_A884_2B8B9F18607B__INCLUDED_)
#define AFX_DRAW3DRECTANGLE_H__B4148023_98CF_440F_A884_2B8B9F18607B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDraw3DRectangle  
{
public:
	CDraw3DRectangle();
	virtual ~CDraw3DRectangle();

//Attributes
public:
	CRect			m_rcWnd;			// Output ����
	CFont*		m_pFont;			// Font
	CString		m_strText;		// Text

	BOOL			m_bOuterLine;	// �ܰ��� �׸� ��/��
	BOOL			m_bTransParent;// TransParent
	BOOL			m_b3D;			// 3D ǥ��
	BOOL			m_bGradient;	// �׶��̼�
	BOOL			m_bMouseUp;		// Mouse Up�� Event
	BOOL			m_bFrame;		// Frame ǥ��
	
	short			m_nScroll;		// ���� Scroll
	short			m_nAlign;		// ���� ���� ����
	short			m_nScrollX;		// Scroll�� X ��ǥ
	short			m_nScrollY;		// Scroll�� Y ��ǥ
	COLORREF		m_clrBack;		// ����
	COLORREF		m_clrText;		// �����


//Member Functions
public:
	void	DrawGradient(CDC *pDC, COLORREF clrStart);
	void  DrawTitleFrame(CDC* pDC);
	void	DrawOutput(CDC *pDC);
	void	DrawRectangle(CDC *pDC, CRect rc);

	void  SetFontStyle();
	void	SetScroll(short nScroll);	
	void	SetText(char *szText);

	char	*GetText() { return (LPSTR)(LPCTSTR)m_strText; }
	void	SetBackColor(COLORREF clrBack) { m_clrBack = clrBack; }
	void	SetFlatOutput(BOOL bFlatOutput) { m_b3D = bFlatOutput; }
	void	SetFrameType() { m_bFrame = TRUE; }
	void	SetGradation(BOOL bGradient = TRUE) { m_bGradient = bGradient; }
	void	SetOuterLine(BOOL bDraw = TRUE) { m_bOuterLine = bDraw; }
	void	SetTextColor(COLORREF clrText) { m_clrText = clrText; }
	void	SetTransParent(BOOL bTrans = TRUE) { m_bTransParent = bTrans; }

// Not Use
	void	SetAlign(short nType);

};

#endif // !defined(AFX_DRAW3DRECTANGLE_H__B4148023_98CF_440F_A884_2B8B9F18607B__INCLUDED_)
