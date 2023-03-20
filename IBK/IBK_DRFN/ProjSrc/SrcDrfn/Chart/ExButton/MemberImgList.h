// MemberImgList.h: interface for the CMemberImgList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERIMGLIST_H__BE4E53E7_5060_4BBF_A9F2_FEEECF423DB1__INCLUDED_)
#define AFX_MEMBERIMGLIST_H__BE4E53E7_5060_4BBF_A9F2_FEEECF423DB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"

class CMemberImgList : public CMember  
{
public:
	CMemberImgList(LPVOID lpControl);
	virtual ~CMemberImgList();

	void	SetMemeberImgList(CImageList* pImageList, int nNormal, int nDown, int nDisable, int nOver, LPCTSTR lpszText, COLORREF clrText, COLORREF clrTextDis);
	void	SetText(LPCTSTR lpszText);
	LPCTSTR		GetText();

	CImageList*				m_pImageList;				// ��ư�� �׸��� �̹����� ��� �ִ� ����Ʈ
	int						m_nNormal;					// Normal ������ �̹��� �ε���
	int						m_nDown;					// Down ������ �̹��� �ε���
	int						m_nDisable;					// Disable ������ �̹��� �ε���
	int						m_nOver;					// Over ������ �̹��� �ε���
	BOOL					m_bButtonDown;
	BOOL					m_bCheck;					// ChekcState
	BOOL					m_bLbuttonDown;				// ���콺��ư �ٿ� üƮ

	LPSTR					m_lpszText;

	COLORREF				m_clrText;
	COLORREF				m_clrTextDis;

protected:
    class CButton_Friendly : public CButton
    {
		friend class CExButtonApp; 
    };

public:
	CButton_Friendly*		m_pButton;
};

#endif // !defined(AFX_MEMBERIMGLIST_H__BE4E53E7_5060_4BBF_A9F2_FEEECF423DB1__INCLUDED_)
