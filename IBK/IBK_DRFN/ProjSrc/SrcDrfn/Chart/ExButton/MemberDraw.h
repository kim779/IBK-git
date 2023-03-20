// MemberDraw.h: interface for the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERDRAW_H__B0F05FF4_6E99_44CA_8487_01349F69CE7C__INCLUDED_)
#define AFX_MEMBERDRAW_H__B0F05FF4_6E99_44CA_8487_01349F69CE7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"

class CMemberDraw : public CMember  
{
public:
	CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin);
	virtual ~CMemberDraw();

	void		SetDefaultColor();
	void		SetAccentColor();
	void		SetTabColor();
	void		SetBuyColor();
	void		SetSellColor();
	void		SetModifyColor();
	void		SetCancelColor();
	
	void		SetSkinColor();

	void		UseToggleMode(BOOL bUse) {m_bUseToggleMode = bUse;}
	BOOL		GetToggleState() {return m_bPushed;}
	void		SetToggleState(BOOL bPushed) {m_bPushed = bPushed;}
	
/*
	void		SetMemberIndex(UINT nOutLine, UINT nOutLineTopLeft, UINT nInLineTopLeft, UINT nInLineBottomRight, UINT nOutLineBottomRight, UINT nBackground, UINT nText
							, UINT nOutLineDis, UINT nOutLineTopLeftDis, UINT nInLineTopLeftDis, UINT nInLineBottomRightDis, UINT nOutLineBottomRightDis, UINT nBackgroundDis, UINT nTextDis);
	void		SetMemberDraw(LPCTSTR lpszText, COLORREF clrOutLine, COLORREF clrOutLineTopLeft, COLORREF clrInLineTopLeft, COLORREF clrInLineBottomRight, COLORREF clrOutLineBottomRight, COLORREF clrBackground, COLORREF clrText
							, COLORREF clrOutLineDis, COLORREF clrOutLineTopLeftDis, COLORREF clrInLineTopLeftDis, COLORREF clrInLineBottomRightDis, COLORREF clrOutLineBottomRightDis, COLORREF clrBackgroundDis, COLORREF clrTextDis);
*/

	void		SetText(LPCTSTR lpszText);
	LPCTSTR		GetText();

	FpChangeSkinNotify	GetNotifySkinTableFunction();
	
	LPSTR		m_lpszText;

	COLORREF	m_clrOutLine;				// �ٱ� �׵θ�
	COLORREF	m_clrOutLineTopLeft;		// ���� ��� �ٱ� ����
	COLORREF	m_clrInLineTopLeft;			// ���� ��� ���� ����
	COLORREF	m_clrInLineBottomRight;		// ���� ���� ����
	COLORREF	m_clrOutLineBottomRight;	// ���� �ٱ� ����
	COLORREF	m_clrBackground;			// ����
	COLORREF	m_clrText;					// �⺻�۲û�

	COLORREF	m_clrOutLineOver;			// �ٱ� �׵θ�
	COLORREF	m_clrOutLineTopLeftOver;	// ���� ��� �ٱ� ����
	COLORREF	m_clrInLineTopLeftOver;		// ���� ��� ���� ����
	COLORREF	m_clrInLineBottomRightOver;	// ���� ���� ����
	COLORREF	m_clrOutLineBottomRightOver;// ���� �ٱ� ����
	COLORREF	m_clrBackgroundOver;		// ����
	COLORREF	m_clrTextOver;				// �⺻�۲û�

	COLORREF	m_clrOutLineDown;			// �ٱ� �׵θ�
	COLORREF	m_clrOutLineTopLeftDown;	// ���� ��� �ٱ� ����
	COLORREF	m_clrInLineTopLeftDown;		// ���� ��� ���� ����
	COLORREF	m_clrInLineBottomRightDown;	// ���� ���� ����
	COLORREF	m_clrOutLineBottomRightDown;// ���� �ٱ� ����
	COLORREF	m_clrBackgroundDown;		// ����
	COLORREF	m_clrTextDown;				// �⺻�۲û�

	COLORREF	m_clrOutLineDis;			// �ٱ� �׵θ�
	COLORREF	m_clrOutLineTopLeftDis;		// ���� ��� �ٱ� ����
	COLORREF	m_clrInLineTopLeftDis;		// ���� ��� ���� ����
	COLORREF	m_clrInLineBottomRightDis;	// ���� ���� ����
	COLORREF	m_clrOutLineBottomRightDis;	// ���� �ٱ� ����
	COLORREF	m_clrBackgroundDis;			// ����
	COLORREF	m_clrTextDis;				// �⺻�۲û�

	UINT		m_nOutLine;
	UINT		m_nOutLineTopLeft;
	UINT		m_nInLineTopLeft;
	UINT		m_nInLineBottomRight;
	UINT		m_nOutLineBottomRight;
	UINT		m_nBackground;
	UINT		m_nText;

	UINT		m_nOutLineOver;
	UINT		m_nOutLineTopLeftOver;
	UINT		m_nInLineTopLeftOver;
	UINT		m_nInLineBottomRightOver;
	UINT		m_nOutLineBottomRightOver;
	UINT		m_nBackgroundOver;
	UINT		m_nTextOver;

	UINT		m_nOutLineDown;
	UINT		m_nOutLineTopLeftDown;
	UINT		m_nInLineTopLeftDown;
	UINT		m_nInLineBottomRightDown;
	UINT		m_nOutLineBottomRightDown;
	UINT		m_nBackgroundDown;
	UINT		m_nTextDown;

	UINT		m_nOutLineDis;
	UINT		m_nOutLineTopLeftDis;
	UINT		m_nInLineTopLeftDis;
	UINT		m_nInLineBottomRightDis;
	UINT		m_nOutLineBottomRightDis;
	UINT		m_nBackgroundDis;
	UINT		m_nTextDis;

    HICON    m_hIcon;
    HICON    m_hIconPushed;
    CSize    m_sizeImage;

	UINT	m_nIconAlign;
	
	BOOL	m_bUseToggleMode;
	BOOL	m_bPushed;

	CList<LPVOID, LPVOID>	m_listToggleGroup;

protected:
    class CButton_Friendly : public CButton
    {
		friend class CExButtonApp; 
    };

public:
	CButton_Friendly*		m_pButton;
};

#endif // !defined(AFX_MEMBERDRAW_H__B0F05FF4_6E99_44CA_8487_01349F69CE7C__INCLUDED_)
