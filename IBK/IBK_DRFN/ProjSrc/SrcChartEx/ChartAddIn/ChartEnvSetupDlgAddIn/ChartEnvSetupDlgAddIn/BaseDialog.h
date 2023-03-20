// BaseDialog.h: interface for the CBaseDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEDIALOG_H__3781D3C9_FDE5_4E79_99D0_EE69E14C1CBC__INCLUDED_)
#define AFX_BASEDIALOG_H__3781D3C9_FDE5_4E79_99D0_EE69E14C1CBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include_AddIn.h"		// for IGraphInfo

// Control ����
#include "NumSpinCtrl.h"				
#include "NumEdit.h"					
#include "BitmapItemComboBox.h"			
#include "ListBoxColorPickerST.h"		
#include "BaseListBox.h"

// Colorlistbox����
#include "SetItemOfColorListBox.h"		
#include "Data_ConditionTitle.h"

// ��ǥ�� Type ����
#include "FindType.h"					

#include "DoubleList.h"				// for CDoubleList

// LineType�� ������ Type��
static TCHAR *szType[] = 
{
	_T("0"),	_T("1"), 	_T("2"),	_T("3"),	_T("4"),	_T("5"),
	_T("6"),	_T("7"),	_T("8"),	_T("9"),	_T("10"),	_T("11"),
	_T("12"),	_T("13"),	_T("14"),	_T("15"),	_T("16"),	_T("17"),
	_T("18"),	_T("19"),	_T("20"),	_T("21"),	_T("22"),	_T("23"),
	_T("24")
};


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 12
// Comments		: ��ǥ�� ���� �� Sub Dialog���� �θ� class 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CBaseDialog : public CDialog
{
// Construction & Destruction
public:
	CBaseDialog( UINT nIDTemplate, CWnd *pParentWnd, IChartOCX *p_pIChartOCX, CWnd *pTopWnd = NULL);
	virtual ~CBaseDialog();

	BOOL m_bSaveDefault;			// �⺻������ ����
	BOOL m_bOrgSaveDefault;			// ������ ���� �⺻������ ����

// (2004.10.11, ��¿�) OCX�κ��� �ֿ� Manager�� Interface�� ���Ͽ� �����Ѵ�.
protected:
	IChartOCX *			m_pIChartOCX;
	IPacketManager *	m_pIPacketManager;
	IChartManager *		m_pIChartManager;
	IGraphInfoManager *	m_pIGraphInfoManager;
	IAddInToolMgr *		m_pIAddInToolMgr;

// Attributes
protected:
	IGraphInfo* m_pIGraphInfo;

	bool m_bUseMultiplePriceData;
	bool m_bDoCalculate;			// ����ؾߵǴ� ��ǥ���� ����

	int m_nSubGraphIdx;				// SubGraph�� Index
	CWnd *m_pTopWnd;				// �ֻ��� Window

	CSetItemOfColorListBox m_MgrColorList;			// ����Ʈ �ڽ��� Į�� ����
	CData_ConditionTitle m_DataConditionTitle;		// Condition�� title��´�.
	
// Pure Virtual Functions
public:
	virtual void Initial( IGraphInfo *p_pIGraphInfo) = 0;				// ���� �ʱ�ȭ
	// 20081014 �ڵ��� ���ϰ��� BOOL������ �Ѵ�. ����ó�� �˻� ������... >>
	//virtual void OnApply( IGraphInfo *p_pIGraphInfo) = 0;				// ���� ���� (�̸� ���� ����)
	virtual BOOL OnApply( IGraphInfo *p_pIGraphInfo) = 0;				// ���� ���� (�̸� ���� ����)
	// 20081014 �ڵ��� <<
	virtual void SetTitle( const int nIndex, const CString &strTitle );		// �̵������ ��� condition���� �ٲ�� title�� �����Ѵ�.
	virtual bool DoCalculate();						// ���� Dialog�� ������ �����Ҷ� ��� ���
	
// Implementation
public:
	//sy test
	//void SetMainBlock( CMainBlock *pMainBlock );
	IGraphInfo *GetGraphInfo()		{ if( m_pIGraphInfo) m_pIGraphInfo->AddRef();	return m_pIGraphInfo; };	// SubDialog�� ��ǥ ������ return
	CString GetIndicatorName() const;

	// 20081014 �ڵ��� >>
	CString	GetPacketType( void);
	// 20081014 �ڵ��� <<
	
protected:
	// Init Control
	void Initial_Spin( CNumSpinCtrl &pSpin, CNumEdit &pEdit, const double Value );
	void Initial_Spin( CNumSpinCtrl &pSpin, CNumEdit &pEdit, const double Value, const float StartRange, const float EndRange, const float delta );
	void Initial_SubGraphName( int p_nSubGraphIndex, CButton &SubName_Bt );
	void Initial_FillUp( int p_nSubGraphIndex, CButton &FillUp_Bt );
	void Initial_FillDown( int p_nSubGraphIndex, CButton &FillDown_Bt );
	void Initial_Title( int p_nSubGraphIndex, CEdit &Title_Edit );
	void Initial_DrawStyleComboBox( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo );
	void Initial_ThicknessComboBox( int p_nSubGraphIndex, int idStatic, CBitmapItemComboBox &Thickness_Combo );
	void Initial_ColorListBox( IGraphInfo *p_pIGraphInfo, int p_nSubGraphIndex, CListBoxColorPickerST *pColor_List );
	void Initial_ListBox( IDoubleList *p_pIDblBase, CBaseListBox &pList);
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	void Initial_BaseLineThicknessComboBox( int p_nSubGraphIndex, int idStatic, CBitmapItemComboBox &Thickness_Combo );
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	void Initial_DrawBaseLineStyleComboBox( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo );

	// Apply Setting data
	void OnApply_FillUp( int p_nSubGraphIndex, CButton &FillUp_Bt );
	void OnApply_FillDown( int p_nSubGraphIndex, CButton &FillDown_Bt );
	void OnApply_Title( int p_nSubGraphIndex, CEdit &Title_Edit );
	void OnApply_DrawStyle( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo );
	void OnApply_Thickness( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo );
	void OnApply_ColorListBox( CListBoxColorPickerST *pColor_List, IGraphInfo *p_pIGraphInfo, int p_nSubGraphIndex );
	CString OnApply_Condition( CEdit &Condi_Edit );
	void OnApply_PacketData( CComboBox &packetcombo, IGraphInfo* p_pIGraphInfo );
	void OnApply_SubPacketData( CComboBox &packetcombo, IGraphInfo* p_pIGraphInfo, int nSubGraphIdx );
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	void OnApply_BaseLineThickness( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo );
	// ���ؼ� �β�, ��� ���� - ojtaso (20071213)
	void OnApply_BaseLineDrawStyle( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo );

//	double GetCondition( const CString &szCondition, const int nIndex );
	CString GetConditionTitle( const char *p_szIndicatorName, const int nIndex );
	
	BOOL SetStatePacketCombo( IGraphInfo *p_pIGraphInfo, CComboBox &packetcombo, CStatic &packetstatic );
	
	void DoModal_ColorDialog( CListBoxColorPickerST &Color_List );
	void MoveWindow( CStatic &condi_static, CNumEdit &Condi_Edit, CNumSpinCtrl &Condi_Spin );

	CString TrimStringWhenDecimalDataIsZero( const CString &strInputData );		// �Ҽ��� �ڸ��� ����
	
	bool IsSpecialIndicator( const CString &strIndicatorName );					// ���õ� ��ǥ�� Ư����ǥ���� Ȯ��
	bool IsExistence_CurSubGraph( const CString &strSubGraphName );
	bool itob( const int nValue );
	
	void DeleteString_ColorListBox( CListBoxColorPickerST &Color_List );		// Colorlistbox�� �ִ� ��� Item �����.
	
	
private:
	void AddLineType( CComboBox *pCombo, int Type );
	void LoadBitmapToDrawStyleCombo( int p_nSubGraphIndex, CBitmapItemComboBox &Drawsytle_Combo );
	void RealLoadBitmap( const int nCount, const int nIDBitmap, CBitmapItemComboBox &Drawsytle_Combo );

// (2006/8/11 - Seung-Won, Bae) Support Condition String Handling Routines
protected:
	void SetCommonCalConditionData( IGraphInfo *p_pIGraphInfo, const CString& strData, const CString& strCompart = ',');
	BOOL DoesEqualCommonPakcetNames( IGraphInfo *p_pIGraphInfo, const CString& strData) const;
	BOOL DoesEqualPacketNames_SubGraphPart( IGraphInfo *p_pIGraphInfo, const int nSubGraphIndex, const CString& strData) const;
	void SetDoubleList( const CString& strData, const CString& strCompart, IDoubleList *p_ItargetList) const;
	BOOL DoesEqualDoubleList( IDoubleList *p_pIDList1, IDoubleList *p_pIDList2) const;
	BOOL DoesEqualStringData( const CString& strTargetData, const CString& strSourceData) const;

// 2008.01.25 by LYH >> for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	CString m_strMsgLineThick;
	CString m_strMsgLineShape;
	CString m_strMsgExistMin;
	CString m_strUp;
	CString m_strDown;
// 2008.01.25 by LYH <<

// (2009/9/29 - Seung-Won, Bae) for WorldOn
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
};

#endif // !defined(AFX_BASEDIALOG_H__3781D3C9_FDE5_4E79_99D0_EE69E14C1CBC__INCLUDED_)
