// ToolBase.h: interface for the CToolBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBASE_H__545B434D_01DD_4446_A40B_C8C95273DE85__INCLUDED_)
#define AFX_TOOLBASE_H__545B434D_01DD_4446_A40B_C8C95273DE85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../gIndc/IndcBase.h"

const	int	szHOLDER = 4;
const	int	szREGION = 8;

#define	_WHITE	RGB(255,255,255)
#define	_YELLOW	RGB(196,196,0)

enum class enumUD { chgNO = 0, chgUP, chgDN };
enum class enumTK
{
	tkNONE = 0,
	tkTENDENCY,		// �߼���
	tkTENDENCYEX,		// �����߼���
	tkHLINE,		// ���μ�
	tkVLINE,		// ���μ�
	tkCROSSLINE,		// ?
	tkUSRINPUT,		// �����Է�
	tkFIBOLINE,		// Fibonacci Time Zone
	tkFIBOFAN,		// Fibonacci Fan
	tkFIBOFAN2,		// Fibonacci Fan
	tkFIBOARC,		// Fibonacci Arc
	tkFIBORET,		// Fibonacci Retracement
	tkGANNLINE,		// Gann Line
	tkGANNFAN,		// Gann Fan (���)
	tkRGANNFAN,		// Gann Fan (�϶�)
	tkACCELFAN,		// Speed Resistance Line
	tkACCELARC,		// Speed Resistance Arc
	tkTRILINE,		// Trizone Line
	tkTRILINELEVEL,		// Trizone Line Level
	tkQUARDLINE,		// Quardrant Line
	tkCROSS,		// ���ڼ�
	tkGRPCONFIG,		// ?
	tkRECT,			// �簢��
	tkTRI,			// �ﰢ��
	tkARC,			// Ÿ��
	tkUARROW,		// ��ȭ��ǥ
	tkDARROW,		// �Ʒ�ȭ��ǥ
	tkLARROW,		// ����ȭ��ǥ
	tkRARROW,		// ������ȭ��ǥ
	tkHORIZON,		// ?
	tkGIGAN,		// �Ⱓ �� ���ǥ�� (�ܼ�)
	tkGIGAN2,		// �Ⱓ �� ���ǥ�� (����)
	tkDAEDEUNG,		// �����
	tkDAEDEUNGCFG,		// ����ϼ���
	tkANDREWPICHFORK,	// Andrews Pichfork
	tkPENCIL
};

class AFX_EXT_CLASS CToolBase  
{
public:
	CToolBase(CWnd* pwndView, CWnd* pwndParent, CRect rectChart, CRect rectGrp, COLORREF rgbBG, CIndcBase* pIndcBase, int iToolKind);
	CToolBase(const CToolBase* rhs);
	virtual ~CToolBase();


public:
	bool		m_bShowHolder;		// show holder
	enumTK		m_eToolKind;		// tool kind name
	int		m_PointCount;		// Andrew PichFork Ponit Count	
	

protected:
	CWnd*		m_pwndView;
	CWnd*		m_pwndParent;
	CIndcBase*	m_pIndcBase;		// graph base

	CRect		m_rectChart;		// ��Ʈ�� ��ü����	m_rectTool;		// total graph area
	CRect		m_rectGrp;		// �ش� �׷��� ����	m_rectDraw;		// tool drawing area
	CRect		m_rectUpHolder;		// Up Holder Rect
	CRect		m_rectDownHolder;	// down Holer Rect

	CRect		m_FisrtHoder;		// first Hoder Tect
	CRect		m_SecondtHoder;		// second Hoder Tect
	CRect		m_ThirdHoder;		// third Hoder Tect

	enumUD		m_eChgUpDn;		// change up dn

	CPoint		m_pointCur;		// current point 
	CPoint		m_pointStart;		// create LButton Down point
	CPoint		m_pointEnd;		// create LButton Up   point 

	double		m_dPosStart;		// start position X
	double		m_dPosEnd;		// End position X
	double		m_dValueStart;		// start Value Y
	double		m_dValueEnd;		// End Value Y
	bool		m_bMove;		// whether moving or changing
	bool		m_bClipRegion;		// whether SetClipRegion of drawRect

	CString		m_strFontName;
	BYTE		m_btFontSize;
	WORD		m_wFontWeight;

	COLORREF	m_rgbLColor;
	BYTE		m_btLStyle;
	BYTE		m_btLWidth;
	DWORD		m_dwToolOption;		// added by schbang 2006.03.29

	COLORREF	m_rgbText;		// line paint RGB
	COLORREF	m_rgbHolder;		// holder paint RGB
	COLORREF	m_rgbBG;		// background RGB
	COLORREF	m_rgbMove;		// moving paint RGB

	CPoint		m_IndexPoint1;		// Create First Point 
	CPoint		m_IndexPoint2;		// Create Second Point
	CPoint		m_IndexPoint3;		// Create Third Point

public:
	CIndcBase*	GetGrpBase() { return m_pIndcBase;}
	virtual void ResizeTool(CRect rectGrp, CRect rectChart);
	virtual void DrawHolder(bool showHolder);
	virtual void DrawHolder(CDC *pDC, bool showHolder);
	virtual void HideHolder();
	virtual void InflateHolder();
	virtual bool PtInHolderRect(CPoint pt);
	virtual bool CheckValidPoint(CPoint pt);
//	virtual	void DrawPosiAngle(CDC* pDC, BOOL bPosi, BOOL bAngle);

	virtual void DrawTool(CDC* pDC) { return; }
	virtual void HideTool() { return; }

	virtual void CreateToolLBDN(CPoint pt, bool &bMouseCapture) { return; }
	virtual void CreateToolLBMV(CPoint pt, bool &bMouseCapture) { return; }
	virtual void CreateToolLBUP(CPoint pt, bool &bMouseCapture) { return; }

	virtual void ChangeToolLBDN(CPoint pt, bool &bMouseCapture) { return; }
	virtual void ChangeToolLBMV(CPoint pt, bool &bMouseCapture) { return; }
	virtual void ChangeToolLBUP(CPoint pt, bool &bMouseCapture) { return; }

	virtual void MoveToolLBDN(CPoint pt, bool &bMouseCapture) { return; }
	virtual void MoveToolLBMV(CPoint pt, bool &bMouseCapture) { return; }
	virtual void MoveToolLBUP(CPoint pt, bool &bMouseCapture) { return; }

	virtual void CopyTool() { return; }

	virtual bool PtInToolRegion(CPoint pt) { return false; }
	virtual void ChangeMemberValue() { return; }

	virtual bool IsVisibleHolder() { return m_bShowHolder; }
	virtual int  GetToolName() { return static_cast<int>(m_eToolKind); }

	virtual void SetDisplayString(CString str) { return; }

	virtual void IndexShift();

	void	ChangeBackGround(int rgb);
	void	SetFontStyle(struct _font *font);
	//void	GetLineStyle(struct _line* pLine);
	void	GetToolInfo(struct _toolInfo* pToolInfo);
	//void	SetLineStyle(struct _line* pLine, bool bColor = true, bool bStyle = true, bool bWidth = true, bool bOption = true);
	void	SetToolInfo(struct _toolInfo* pToolInfo, bool bOption = false);
	void	SetToolColor(COLORREF rgb);
	void	SetToolWidth(BYTE btWidth);
	virtual	void InitValue(int lGap, int rGap) {}

protected:
	CFont*	GetFont(CWnd *pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd *pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd *pView, COLORREF rColor);
};

#endif // !defined(AFX_TOOLBASE_H__545B434D_01DD_4446_A40B_C8C95273DE85__INCLUDED_)
