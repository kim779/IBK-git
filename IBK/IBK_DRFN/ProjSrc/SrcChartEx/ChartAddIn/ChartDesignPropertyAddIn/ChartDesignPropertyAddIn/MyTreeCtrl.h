#if !defined(AFX_MYTREECTRL_H__9A417DA4_31C4_11D2_A335_0060088E1AE4__INCLUDED_)
#define AFX_MYTREECTRL_H__9A417DA4_31C4_11D2_A335_0060088E1AE4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MyTreeCtrl.h : header file
//

#include <afxtempl.h>

#include "ChartDrop.h"
#include "ChartLink.h"
#include "resource.h"
#include "DlgGraphSetting.h"
#include "DlgBlockHorizontalScale.h"

#include "LineColor.h"
/////////////////////////////////////////////////////////////////////////////
// CMyTreeCtrl window

///////////////////////////////////////////////////////////////////////////////
//	�۾���	:	��¿�(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	����	:	2003.12.26
//
//	����	:	(2003.12.26) m_pChartRoot�� ���� �����ϹǷ� Pointer�� �ƴ϶� Object�� �����Ѵ�.
///////////////////////////////////////////////////////////////////////////////
class CMyTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CMyTreeCtrl();

// Attributes
public:
	HTREEITEM		m_hTarget;
	CChartDrop		m_dropTarget;
	COleDataSource	m_dragSource;
	BOOL			m_DoDrag;

	// (2003.12.26, ��¿�) m_pChartRoot�� ���� �����ϹǷ� Pointer�� �ƴ϶� Object�� �����Ѵ�.
	CChartLink		m_clChartRoot;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetScaleCompartTypeToAllBlocks(const int nScaleCompartType);
	//set All Item Info of PacketPage.
	void SetAllItemInfoOfPacketPage(const CString& strAllItemInfoOfPacketPage);
	//save received colorvalue from ocx
	void SetColorOfLine(const int nLineIndex, const CString& strItemValue);

	void InitBlock(const CString& strData, const int nRow, const int nColumn); //block ����..
	//ocx���� ���� ������ Tree�� �����Ҷ� ��� ���� �������� ����.
	HTREEITEM AddBlock(HTREEITEM hItem, LPSTR name);
	HTREEITEM AddItem(HTREEITEM hItem, LPSTR name);

	void ChangeBlockData(const int nBlock,
		          const CString& szScaleData,
				  const int nHorizonPos,
				  const int nScaleCompart,
				  const CString& szDate,
				  const CString& time,
				  const int nHorizonStyle,
				  const int nLog,
				  const int nOverTurn);

	CChartLink* AddVerticalScaleItem(	const CString &	p_strScaleData,
										const int		p_nScalePos,
										const int		p_nScaleCompart,
										const int		p_nScaleExpressionType,
										const BOOL		p_bMinMaxLock,			// Lock		// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
										const int		p_nMinMaxType,			// Type
										const double	p_dMin,					// Min
										const double	p_dMax,					// Max
										const int		p_nUpSpace,				// UpSpace		// (2007/7/19 - Seung-Won, Bae) Support Up/Down Space
										const int		p_nDownSpace,			// DownSpace
										const double	p_dHogaNumber,			//2007.01.22 Scale����� ����,ȣ��������
										const double	p_dUserNumber,
										const int		p_nUserHogaType,
										const BOOL		p_bIsInvert				// ��ǥ ������ - ojtaso (20071023)
									 );

	void AddItem(CChartLink* pVertiScale,
						  const CString& strName,
						  const CString &p_strIndicatorName,
						  const CString &p_strGraphName,			// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
						  const CString& strPacket,
						  const bool bLowHighUpDown,
						  const bool bLowNowUpDown,
						  const bool bHighNowUpDown,
						  const bool bPreNowContrast,
						  const int nCurData,
						  //sy 2006.04.17.
						  const bool bSignalUse,
						  const bool bSignalShow,
						  //sy end
						  const CString& strColorValue);

	//������ ��� ���� �ϳ��� String���� �����.
	CString GetBlockandGraphAllData();

	//init TreeCtrl
	void InitTreeView();
	void InitDropTarget();
	virtual ~CMyTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTreeCtrl)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTitlescalecolor();
	afx_msg void OnDelete();
	afx_msg void OnHorizonscale();
	afx_msg void OnVertScale();
	afx_msg void OnVertDelete();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChartinvert();
	//}}AFX_MSG
	LONG OnDragChartData(UINT wParam, LPARAM lParam);
	LONG OnDropChartData(UINT wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	void OnDeleteVertScale(CChartLink* pVertScale);
	//��Į�� ���� Ŭ����.
	CLineColor m_LineColor;
	//Packet�� ��� ������ �Ѿ�´�.
	CString m_strAllItemInfoOfPacketPage;
	//���� Active�� ���� ������.
	CChartLink* m_pChartBlock;
	CImageList m_ImageList;
	//Drag �ϱ� ���� ���õ� Item.
	CChartLink *m_pSelectItem;

	HTREEITEM AddVertScale(HTREEITEM hItem, LPSTR name);
	//Block����
	void DeleteBLOCK_OnlyTreeItem(CChartLink *pLink);
	//Graph����
	void DeleteGRAPH_OnlyTreeItem(CChartLink *pLink);
	void DeleteVERT_SCALE_OnlyTreeItem(CChartLink *pLink);

	int GetimageofIndicatorTypeIndex(LPSTR strGraphType);
	
	//get SubGraph title.
	CString GetTitle(const CString& strTitle);
	//Root���� ������ ���� Root�� ����.
	void SetDlgValueToCChartLinkClass(CDlgBlockHorizontalScale& dlgBlockHorizonScale, CChartLink *pItem);
	//Root���� ������ ���� ��� ���� ����.
	void SetRootValueToAllBlocks(CChartLink* pItem);
	//�������� ������ �ִ� ���������� Dialog�� ����.
	void SetDlgBlockHorizontalScale(CChartLink *pItem, CDlgBlockHorizontalScale& dlgBlockHorizonScale);

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
protected:
	IGraphInfoManager *	m_pIGraphInfoManager;
public:
	void SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager);

// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
protected:
	CScaleBaseData::SCALEGRIDTYPE	GetFirstVScaleGridType( CChartLink *p_pLink);

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);

//@solomon[sashia]	20090902 --- ��� ���� ��� --------->>
public:
	CChartLink*		GetSelectItem(HTREEITEM hItem)	{ return m_clChartRoot.Find(hItem);	};
//@solomon[sashia]	20090902 --- ��� ���� ��� ---------<<
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTREECTRL_H__9A417DA4_31C4_11D2_A335_0060088E1AE4__INCLUDED_)
