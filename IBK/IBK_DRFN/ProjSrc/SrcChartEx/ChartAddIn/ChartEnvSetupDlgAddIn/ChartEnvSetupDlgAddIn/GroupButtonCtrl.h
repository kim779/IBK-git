// GroupButtonCtrl.h: interface for the CGroupButtonCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUPBUTTONCTRL_H__09E85A91_5C64_4301_AD3E_6C4CBF9D6A6C__INCLUDED_)
#define AFX_GROUPBUTTONCTRL_H__09E85A91_5C64_4301_AD3E_6C4CBF9D6A6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <AFXTEMPL.H>

#include "Include_AddIn.h"			// for IGraphInfoManager

class CDlgMainTabIndicatorSetUp;
class CIndicatorNameListCtrl;
class CIndicatorGroupButton;


enum BTN_TYPE { GENERAL_GROUP, USE_GROUP };

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CGroupButtonCtrl
{
// Construction & Destruction
public:
	CGroupButtonCtrl();
	virtual ~CGroupButtonCtrl();


// Attribute
public:
	CFont m_font;				// Group��ư�� ��Ʈ

	CDlgMainTabIndicatorSetUp *m_pDlgMainTabIndicatorSetUp;				// �θ��������� ������
	CRect m_rcTotalRegion;

	CRect m_rcFirstIndicatorBtn;

	CList< CIndicatorNameListCtrl *, CIndicatorNameListCtrl * > m_lstListCtrl;	// ListCtrl�� List
	CList< CIndicatorGroupButton *, CIndicatorGroupButton * > m_lstGroupButton;	// �ǹ�ư�� List

protected:
	IPacketManager *	m_pIPacketManager;

// Implementaion
public:
	void SetData( CWnd *pWnd);			// �ʿ� ������ ����.
	
	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) - GroupCtrl�� ������ ���� ���������� �ٸ� �Լ��� �����.
	void CreateGeneralCtrl( IStringList * p_pIndicatorNameList, IChartOCX *p_pIChartOCX );
	void CreateUseCtrl( IStringList * p_pIndicatorNameList, IChartOCX *p_pIChartOCX);

	void SetIndicatorSize( const CRect &region );
	CDlgMainTabIndicatorSetUp* GetParent() { return m_pDlgMainTabIndicatorSetUp; };	// �θ������츦 �����Ѵ�.
	CIndicatorGroupButton* GetSelectGroupButton(int nIndex = -1);		// ���� ������ IndicatorGroupButton�� ������.
	CIndicatorNameListCtrl* GetSelectListCtrl(int nIndex = -1);			// ���� ������ IndicatorGroup�� ListCtrl
	void OnGroupButtonDown(int nIndex, const bool bIsDlgCreat = true);	// ��ǥ�׷��ư�� ��������.
	void ReleaseFocusItem();			// ��Ŀ���� Ǭ��.
	void ReleaseAllFocusSpecialTab();	// Ư����ǥ�� üũ�� �������� ��� UnCheck�Ѵ�.
	void SetFocusItem();				// �ش�Index�� ��Ŀ���� ��.
	void ModifyCheckWhenSpecialIndicator(const CString &strIndicatorName);	// Ư����ǥ�� �Ѱ��� üũ�ǵ��� �Ѵ�.
	void SetCheck(IStringList * p_pIndicatorNameList);	// ���� ��Ʈ�� Display�Ǿ��ִ� ��ǥ�� üũ�Ѵ�.
	void SetAllUnCheck();				// ��ǥ�� ListCtrl�� ��� �������� UnCheck�Ѵ�.
	void GetFirstCheckItem( CString &p_strIndicatorName, CString &p_strGraphName);		// ListCtrl���� ù��°�� üũ�Ǿ� �ִ� Item�� ���ڿ��� ������.(������ ������)
	int GetCount(){ return m_lstListCtrl.GetCount();}	// ����Ʈ��Ʈ���� ������ �����´�.

private:
	int m_nSelectButtonIndex;	// ������ ��ư �ε���
	int m_nSelectListCtrlIndex;	// ������ ListCtrl �ε���

	void	SetEachCtrlSize( const int nGroupCount, UINT &nID, CRect &rcListCtrlRegion, CRect &rcButtonRegion );
	void	MoveWindowInAllListCtrl(const int nGroupCount, const CRect& curListCtrlRegion);
	bool	CreateListCtrlAndButton(const UINT& nID, const int nIndex, const char *p_szGroupName, const CRect& listCtrlRegion, const CRect& buttonRegion, 
				IStringList * pGraphNameList, IStringList * pIndicatorNameList, const bool bIsSorting = true);
	bool	CreateListCtrl( const UINT& nID, const int nIndex, const CRect& region, const CString& strGroupName, IStringList * pGraphNameList,
				IStringList * pIndicatorNameList, const bool bIsSorting = true);
	void	CreateButton(const UINT& nID, const int nIndex, const CString& strWndText, const CRect& region);
	CRect	GetButtonRegion(const CRect& baseRegion, const int nIndex);
	void	SelectGroupButton(const bool bIsCurSpecialIndicator);
	bool	IsCheckSpecialIndicator(IStringList * p_pIndicatorNameList);	// ���� Ư����ǥ�� �̿������� Ȯ���Ѵ�.


protected:
	// ( 2007 / 01 / 09 - Sang-Woo, Cho ) 
	int m_nBtnType;			// General : 0,		Use : 1

public:
	void SetGroupButtonCtrlType( int nBtnType )	{	m_nBtnType = nBtnType;	};
	int GetGroupButtonCtrlType( void )			{	return m_nBtnType;	};
	
	BOOL AddGraph( CString strIndicatorName );
	BOOL RemoveGraph( CString strIndicatorName );

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};


#endif // !defined(AFX_GROUPBUTTONCTRL_H__09E85A91_5C64_4301_AD3E_6C4CBF9D6A6C__INCLUDED_)
