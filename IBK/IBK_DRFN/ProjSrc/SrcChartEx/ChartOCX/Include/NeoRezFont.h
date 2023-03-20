#ifndef __NEOCOMPONENT__RESIZE__CONTROL__FONT__HEADER__
#define __NEOCOMPONENT__RESIZE__CONTROL__FONT__HEADER__

// #include "../include/NeoRezFont.h"

// ###############################################################################
// * START : 2001. 01. 30 ������.. ���ؿ� (��)�θ�������� ���������
// * ����ũ���� ����ϴ� ��Ʈ�� ���� ����ü 9�� �������� �Ѵ�.
// * ��Ʈ�� ������ �۰�, ���۰�, ����, ũ��, ��ũ�� �� 5�ܰ踦 ���´�.
// *                             9     11     13
// * ������ ������ �ʿ信���� Data/NeoDesk.cfg �� �����Ѵ�.
// * 
// * ó������
// *    1. ��Ʈ�ѻ����� "RMSG_FORMINITIALCOMPLETED" �Ķ����(0, 0) �޽����� ������
// *       �����¸� �⺻���� �Ͽ� ������ �����Ѵ�.
// *    2. ���ο��� ��Ʈ�� �ٲٰų� ����ũ���� ��Ʈ�� ������ �ʿ䰡 ���� �� 
// *       ����ũ���� "RMSG_FORMSTATECHANGE" �޽����� ������.
// *       WPARAM :
// *	   LPARAM : STNEOFONTINFO ����ü ������
// *
// * �߰��Ǵ� ���� �� ������ �Ʒ��� �߰��ϱ� �ٶ��ϴ�.
// ###############################################################################

typedef struct {
	char sFontName[20+1];
	int	 nFontStep;			// Font Step
} STNEOFONTINFO;

typedef struct {
//	int	 nWidth;			// Font Width
//	int	 nHeight;			// Font Height
	char sFontName[20+1];
} STRESIZEFONT;


class CNeoReszCtl
{
public:
	// *
	CNeoReszCtl() : m_pCtrl(NULL)
	{
	}

	CNeoReszCtl(CWnd* _pWnd)
	{
		m_pCtrl = _pWnd;
	}

	~CNeoReszCtl()
	{
	}

	// Variable
	int		m_nBaseStep,	// �⺻���� (9)
			m_nCurStep;		// ���罺��

	CRect	m_BaseRect,		// �⺻���ܿ����� �⺻ ��ġ����
			m_CurRect;		// ������ ��ġ����

	CFont*	m_BaseFont,		// �⺻���ܿ����� �⺻ ��Ʈ����
			m_CurFont;		// ������ ��Ʈ����


	CWnd*	m_pCtrl;		// ������ �ڵ�

	// Operation
	void	Resize(STNEOFONTINFO *_pNewInfo)
	{
	}

};

#endif // __NEOCOMPONENT__RESIZE__CONTROL__FONT__HEADER__
