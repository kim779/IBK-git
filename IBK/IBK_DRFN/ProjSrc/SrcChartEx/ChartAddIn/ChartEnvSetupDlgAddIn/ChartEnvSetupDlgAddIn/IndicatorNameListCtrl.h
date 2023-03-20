#if !defined(AFX_INDICATORNAMELISTCTRL_H__F5C39E2F_94DD_421C_85BF_E11BBE8EEFE6__INCLUDED_)
#define AFX_INDICATORNAMELISTCTRL_H__F5C39E2F_94DD_421C_85BF_E11BBE8EEFE6__INCLUDED_

#include "BlockBaseData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndicatorNameListCtrl.h : header file
// 04/03/30 ����: ��ǥ���� ��� ListCtrl�� GroupButton �Ѱ��� ListCtrl �Ѱ��� �����Ѵ�.

#include <AFXTEMPL.H>

#include "Include_AddIn.h"		// for IPacketManager

/////////////////////////////////////////////////////////////////////////////
// CIndicatorNameListCtrl window
class CIndicatorGroupData;
class CGroupButtonCtrl;

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Comments		: 
// Using way	: 
// See			: class CListCtrl
//-----------------------------------------------------------------------------
class CIndicatorNameListCtrl : public CListCtrl
{
// Construction
public:
	CIndicatorNameListCtrl( CGroupButtonCtrl* pParent, LPCTSTR pcszIndicatorGroupName, int nIndex, IPacketManager *p_pIPacketManager);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndicatorNameListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIndicatorNameListCtrl();

	// Generated message map functions
public:
	//{{AFX_MSG(CIndicatorNameListCtrl)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// Function
private:
	void InitColumn();	// Į���� �ʱ�ȭ�Ѵ�.
	bool Initialize( IStringList *pGraphNameList, IStringList *pIndicatorNameList);	// ListCtrl �ʱ�ȭ�Ѵ�.
	bool Initialize_Market( IStringList *pList);
	bool Initialize_PacketTimeType( IStringList * pList);	// ListCtrl �ʱ�ȭ�Ѵ�.

	void GetDataInPacket( IPacketManager::ICOMMODITY_BOND_NAME& eBondName, CScaleBaseData::HORZSCALEDRAWERTYPE& eTimeType);

	bool IsInitialize_Market(const CString& strGroupName) const;
	bool IsInitialize_PacketTimeType(const CString& strGroupName) const;

	bool IsAddIndicatorName_DLL( void);
	bool IsAddIndicatorName( void);
	bool IsAddIndicatorName( const IPacketManager::ICOMMODITY_BOND_NAME eBondName, 
		const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType, const char *p_szGraphName);

	bool IsStock(const IPacketManager::ICOMMODITY_BOND_NAME eBondName) const;
	bool IsFuturesOrOption(const IPacketManager::ICOMMODITY_BOND_NAME eBondName) const;
	bool IsDailyOrWeekleyOrMonthly(const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType) const;
	bool IsDailyOrTimeOrTick(const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType) const;
	bool IsDaily(const CScaleBaseData::HORZSCALEDRAWERTYPE eTimeType) const;

public:
	bool Initialize( const char *p_szGroupName, IStringList *pGraphNameList, IStringList *pIndicatorNameList);	// ListCtrl �ʱ�ȭ�Ѵ�.
	void SetCheckSeletecItem(IStringList *IndicatorNameList);	// íƮ�� �ִ� ��ǥ����� üũ�Ѵ�.
	void SetAllUnCheck();						// ��� ����Ʈ üũ���¸� �ʱ�ȭ(UnCheck)�Ѵ�.
	int GetIndex(){ return m_nIndex;};			// ListCtrl�� �ε����� �����´�.
	void SetFocusItem(const int nIndex);		// �ش��ε����� ��Ŀ���� �ش�.
	void ReleaseFocusItem();					// ��Ŀ���� Ǭ��.
	CString GetGroupName(){ return m_strIndicatorGroupName; };				// ��ǥ�׷���� �����Ѵ�.
	BOOL GetCheckState(const CString &strIndicatorName, int& nIndex);		// �������� üũ���¸� �����Ѵ�.

	// ( 2007 / 01 / 08 - Sang-Woo, Cho ) 
	void ChangeFocusItem( void );
	CGroupButtonCtrl *GetOtherGroupButtonCtrl( void );
	
	BOOL AddGraph( const CString &strIndicatorName, const CString &p_strGraphName);
	BOOL RemoveGraph( int p_nIndex);
	int	 GetSelectItemIndex( void )						{	return m_nSelectItemIndex;	};
	void SetSelectItemIndex( int nSelectItemIndex )		{	m_nSelectItemIndex = nSelectItemIndex;	};
	
	
// Member
private:
	CString m_strIndicatorGroupName;		// ��ǥ�׷��.
	int m_nIndex;							// ListCtrl�� �ε���
	int m_nSelectItemIndex;					// ListCtrl�� ������ Item�� �ε���
	
	CGroupButtonCtrl *m_pParent;			// �θ��� ������.

	BOOL m_bInitCheckIndicator;				// �ʱ⼳������ ���� ��ǥ�� üũǥ���� �� ��ǥ�߰�������ƾ�� ȣ����� �ʰ� ��.
	IPacketManager *	m_pIPacketManager;

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDICATORNAMELISTCTRL_H__F5C39E2F_94DD_421C_85BF_E11BBE8EEFE6__INCLUDED_)
