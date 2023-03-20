// BackgroundDrawer.h: interface for the CBackgroundDrawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUNDDRAWER_H__0117DC90_6CBE_459F_8082_F52D66D8D8EF__INCLUDED_)
#define AFX_BACKGROUNDDRAWER_H__0117DC90_6CBE_459F_8082_F52D66D8D8EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CBackgroundDrawer  
{
public:
	CBackgroundDrawer();
	virtual ~CBackgroundDrawer();
	
	void DrawBackground(CDC* pDC,
				CRect& backgroundRegion,	// �׸� background ���� 
				const int frameThickness,		// frame ����
				COLORREF backgroundColor,		// background ��
				COLORREF backFrameColor,		// block ��
				const int frameType);			// frame Ÿ��

};

#endif // !defined(AFX_BACKGROUNDDRAWER_H__0117DC90_6CBE_459F_8082_F52D66D8D8EF__INCLUDED_)
