// DlgAnalysisToolSet.cpp : implementation file
//

#include "stdafx.h"
#include "analysistool.h"
#include "DlgAnalysisToolSet.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_Chart/Conversion.h"		// 2008.10.15 by LYH >> 8,32bit����
#include "../Include/ICanCallYou.h"
	
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysisToolSet dialog

// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// CDlgAnalysisToolSet::CDlgAnalysisToolSet(CWnd* pParent,
// 										 const CString& strUserPath,
// 										 const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
// 										 int nElementType,
// 										 int *pEquiDivType,
// 										 int *pPriceRangeDivType,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
// 										 CTextPosition* pTextPosition,
// 										 CPenEnvironment* pPenEnvironment,
// 										 CElementPointList *pElementPointList,
// 										 CLineExtEnvironment* pLineExtEnvironment,
// 										 CDataTypeEnvironment* pDataEnvironment)
CDlgAnalysisToolSet::CDlgAnalysisToolSet(CWnd* pParent,
										 const CString& strUserPath,
										 const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
										 int nElementType,
										 int *pEquiDivType,
										 int *pPriceRangeDivType,	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
										 CTextPosition* pTextPosition,
										 CPenEnvironment* pPenEnvironment,
										 CElementPointList *pElementPointList,
										 CLineExtEnvironment* pLineExtEnvironment,
										 CDataTypeEnvironment* pDataEnvironment,
										 IChartOCX *p_pIChartOCX,
										 CBlockIndex& blockIndex,
										 CString strFormatX,		// X�� ��¥�ð� ���� (YYYYMMDDHHMMSS)
										 LONG pInterface)
// 2011.02.08 by SYS <<

	: CDialog(CDlgAnalysisToolSet::IDD, pParent),
	m_pParent(pParent),
	m_strUserPath(strUserPath),
	m_nDataPeriod(nDataPeriod),
	m_pPenEnvironment(pPenEnvironment),
	m_basePenEnvironment(*pPenEnvironment),
	m_pElementPointList(pElementPointList),
	m_baseElementPointList(*pElementPointList),
	m_pEquiDivEnvironment(pEquiDivType),
	m_baseEquiDivEnvironment(*pEquiDivType),
	m_pPriceRangeDivEnvironment( pPriceRangeDivType),	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_basePriceRangeDivEnvironment(*pPriceRangeDivType),
	m_pInterface(pInterface)
{
	//{{AFX_DATA_INIT(CDlgAnalysisToolSet)
	m_nWidth = 0;
	m_nStyle = 0;
	//}}AFX_DATA_INIT

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	m_pIChartOCX	= p_pIChartOCX;

 	if(blockIndex.IsAllValueBiggerThanZero())
 	{
 		m_blockIndex.SetRow(blockIndex.GetRow());
 		m_blockIndex.SetColumn(blockIndex.GetColumn());
 	}

	m_strFormatX	= strFormatX;
	// 2011.02.08 by SYS <<


	m_nTextPosType			= G_TEXT_POS_NONE;
	m_nLineExtType			= G_LINE_EXT_NONE;
	m_nEquiDivLine			= G_EQUI_DIV_NONE;
	m_nPriceRangeDivLine	= G_PRICE_RANGE_DIV_NONE;	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_nElementType = nElementType;

	SetInitTextPosition(pTextPosition);
	
	m_pLineExtEnvironment = pLineExtEnvironment;
	m_baseLineExtEnvironment.Set(pLineExtEnvironment);

	m_hOcxWnd = ( pParent ? pParent->GetSafeHwnd() : NULL);

// 2008.10.15 by LYH >> 8,32bit����
	m_pDataEnvironment		= pDataEnvironment;
// 2008.10.15 by LYH <<
}

void CDlgAnalysisToolSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAnalysisToolSet)
	DDX_Control(pDX, IDC_CHECK_PRICE_RANGE, m_chkPriceRangeDiv);
	DDX_Control(pDX, IDC_STYLE, m_ctlStyle);
	DDX_Control(pDX, IDC_WIDTH, m_ctlWidth);
	DDX_Control(pDX, IDC_COLOR, m_ctlColor);
	DDX_Control(pDX, IDC_CHECK_TEXT_LEFT, m_ctrlLeftText);
	DDX_Control(pDX, IDC_CHECK_TEXT_RIGHT, m_ctrlRightText);
	DDX_Control(pDX, IDC_CHECK_TEXT_RATE, m_ctrlRateText);
	DDX_Control(pDX, IDC_CHECK_TEXT_TOP, m_ctrlTopText);
	DDX_Control(pDX, IDC_CHECK_TEXT_BOTTOM, m_ctrlBottomText);
	DDX_Control(pDX, IDC_CHECK_EXT_LEFT, m_ctrlLeftExt);
	DDX_Control(pDX, IDC_CHECK_EXT_RIGHT, m_ctrlRightExt);
	DDX_Control(pDX, IDC_CHECK_EQUIDIV_LINE, m_ctrlEquiDiv);
	DDX_Control(pDX, IDC_CHECK_FILL, m_ctrlCheckFill);
	DDX_Control(pDX, IDC_CHECK_HIGH_LOW, m_ctrlCheckHL);
	DDX_Control(pDX, IDC_EDIT_XDATE1, m_ctrlEditXValue1);
	DDX_Control(pDX, IDC_EDIT_XDATE2, m_ctrlEditXValue2);
	DDX_Control(pDX, IDC_EDIT_XDATE3, m_ctrlEditXValue3);
	DDX_Control(pDX, IDC_EDIT_VALUE1, m_ctrlEditValue1);
	DDX_Control(pDX, IDC_EDIT_VALUE2, m_ctrlEditValue2);
	DDX_Control(pDX, IDC_EDIT_VALUE3, m_ctrlEditValue3);
	DDX_Control(pDX, IDC_DATETIMEPICKER_XDATE1, m_ctrlXDateTime1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_XDATE2, m_ctrlXDateTime2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_XDATE3, m_ctrlXDateTime3);
	DDX_CBIndex(pDX, IDC_WIDTH, m_nWidth);
	DDX_CBIndex(pDX, IDC_STYLE, m_nStyle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAnalysisToolSet, CDialog)
	//{{AFX_MSG_MAP(CDlgAnalysisToolSet)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_BN_CLICKED(IDC_APPLY_BT, OnApplyBt)
	ON_CBN_SELCHANGE(IDC_WIDTH, OnSelchangeWidth)
	ON_CBN_SELCHANGE(IDC_STYLE, OnSelchangeStyle)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_XDATE1, OnDatetimechangeDatetimepickerXdate1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_XDATE2, OnDatetimechangeDatetimepickerXdate2)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_XDATE3, OnDatetimechangeDatetimepickerXdate3)
	ON_BN_CLICKED(IDC_OK_BT, OnOkBt)
	ON_BN_CLICKED(IDC_CANCEL_BT, OnCancelBt)
	ON_BN_CLICKED(IDC_CHECK_TEXT_RIGHT, OnRightText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysisToolSet message handlers
BOOL CDlgAnalysisToolSet::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Dialog�� ��Ŀ���� �ֱ� ���� ���
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	
	InitializeData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////

void CDlgAnalysisToolSet::SaveData()
{
	CString strFileName;
	strFileName.Format("%s%s",m_strUserPath,G_ANALYSIS_PROPERTY_INFO);
	CFileFind finder;
	BOOL bFind = finder.FindFile(strFileName);
	finder.Close();
	
	if(bFind != TRUE)
	{		
		CFile file;
		if(file.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite) == FALSE)
			return;	
		file.Close();
	}

	CString strSection,strKey,strData,strName;	
	strSection.Format("%d",(CToolEnumData::ElementType)m_nElementType);
	
	strKey = _T("TopText");
	strData.Format(_T("%d"),m_stAnalysisTool.nTopText);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("BottomText");
	strData.Format(_T("%d"),m_stAnalysisTool.nBottomText);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LeftText");
	strData.Format(_T("%d"),m_stAnalysisTool.nLeftText);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("RightText");
	strData.Format(_T("%d"),m_stAnalysisTool.nRightText);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LeftExt");
	strData.Format(_T("%d"),m_stAnalysisTool.nLeftExt);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("RightExt");
	strData.Format(_T("%d"),m_stAnalysisTool.nRightExt);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("EquiDiv");
	strData.Format(_T("%d"),m_stAnalysisTool.nEquiDiv);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	strKey = _T("PriceRangeDiv");
	strData.Format(_T("%d"),m_stAnalysisTool.nPriceRangeDiv);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("HLPercent");
	strData.Format(_T("%d"),m_stAnalysisTool.nHLPercent);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("FillRegion");
	strData.Format(_T("%d"),m_stAnalysisTool.nFillRegion);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	
	strKey = _T("LineColor");
	strData.Format(_T("%ld"),m_stAnalysisTool.crLineColor);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LineStyle");
	strData.Format(_T("%d"),m_stAnalysisTool.nLineStyle);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("LineWeight");
	strData.Format(_T("%d"),m_stAnalysisTool.nLineWeight);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);

	strKey = _T("RightRateText");
	strData.Format(_T("%d"),m_stAnalysisTool.nRateText);		
	WritePrivateProfileString(strSection,strKey,strData,strFileName);
}

void CDlgAnalysisToolSet::ApplyData()
{
	SetPenEnvironment();

	if(m_nTextPosType != G_TEXT_POS_NONE)
		SetTextPosition();

	if(m_nLineExtType != G_LINE_EXT_NONE)
		SetLineExtEnvironment();

	if(m_nEquiDivLine != G_EQUI_DIV_NONE)
	{
		(*m_pEquiDivEnvironment) = m_ctrlEquiDiv.GetCheck();
		m_stAnalysisTool.nEquiDiv = m_ctrlEquiDiv.GetCheck();		
	}

	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	if( m_nPriceRangeDivLine != G_PRICE_RANGE_DIV_NONE)
	{
		( *m_pPriceRangeDivEnvironment) = m_chkPriceRangeDiv.GetCheck();
		m_stAnalysisTool.nPriceRangeDiv = m_chkPriceRangeDiv.GetCheck();		
	}
	
	SetDateValueLists();

	_DoCheckCallInterface();
	
	m_pParent->Invalidate();
}

void CDlgAnalysisToolSet::_DoCheckCallInterface()
{
	if(IsDlgButtonChecked(IDC_TOALLTHICK))
	{
		if(!m_pInterface) return;
		ICanCallYou* pCanCall = (ICanCallYou*)m_pInterface;
		pCanCall->DoCallYou(ICCYT_changeThick,(m_nWidth+1));
	}
}

void CDlgAnalysisToolSet::ApplyOrgData()
{
	m_pPenEnvironment->Set(m_basePenEnvironment);
			
	if(m_nTextPosType != G_TEXT_POS_NONE)
	{
		if(m_nTextPosType == G_TEXT_POS_HV)
			SetTextPosition(m_baseTextHVPosition.GetLeft(),m_baseTextHVPosition.GetRight(),
							m_baseTextHVPosition.GetTop(),m_baseTextHVPosition.GetBottom());		
		else if(m_nTextPosType == G_TEXT_POS_VERT)
			SetTextPosition(m_baseTextVPosition.GetTop(),m_baseTextVPosition.GetBottom());			
		else if(m_nTextPosType == G_TEXT_POS_ONE)
			SetTextPosition(m_baseTextPosition.IsShow());			
		else if(m_nTextPosType == G_TEXT_POS_HORZ)
			SetTextPosition(m_baseTextHPosition.GetLeft(),m_baseTextHPosition.GetRight());		
		
		CTextPosition* pTextPosition = GetTextPosition();
		CTextPosition* pOrgTextPosition = GetOrgTextPosition();
		if(pTextPosition != NULL && pOrgTextPosition != NULL)
			pTextPosition->Set(pOrgTextPosition);
	}

	if(m_nLineExtType != G_LINE_EXT_NONE)
	{
		SetLineExtEnvironment( ( m_baseLineExtEnvironment.GetLeftExt() ? true : false),
								( m_baseLineExtEnvironment.GetRightExt() ? true : false));
		
		CLineExtEnvironment* pLineExtEnvironment = GetLineExtEnvironment();
		CLineExtEnvironment* pOrgLineExtEnvironment = GetOrgLineExtEnvironment();
		if(pLineExtEnvironment != NULL && pOrgLineExtEnvironment != NULL)
			pLineExtEnvironment->Set(pOrgLineExtEnvironment);
	}

	if(m_nEquiDivLine != G_EQUI_DIV_NONE)
		(*m_pEquiDivEnvironment) = m_baseEquiDivEnvironment;		
	
	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	if( m_nPriceRangeDivLine != G_PRICE_RANGE_DIV_NONE)
		( *m_pPriceRangeDivEnvironment) = m_basePriceRangeDivEnvironment;		

	m_pParent->Invalidate();
}

void CDlgAnalysisToolSet::OnColor() 
{
	CColorDialog colorDlg;

	if(colorDlg.DoModal() != IDOK)
		return;

	SetColor(colorDlg.GetColor());
}

///////////////////////////////////////////////////////////////////////////////
// protected ==================================================================
void CDlgAnalysisToolSet::InitializeData()
{
	m_stAnalysisTool.crLineColor = m_pPenEnvironment->GetColor();
	m_stAnalysisTool.nLineStyle = m_pPenEnvironment->GetStyle();
	m_stAnalysisTool.nLineWeight = m_pPenEnvironment->GetWidth();
	m_stAnalysisTool.dEndXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dEndYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.dStartXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dStartYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.dThirdXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dThirdYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.nEquiDiv = -1;
	m_stAnalysisTool.nPriceRangeDiv = -1;	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_stAnalysisTool.nHLPercent = -1;
	m_stAnalysisTool.nFillRegion = -1;
	m_stAnalysisTool.nLeftExt = -1;
	m_stAnalysisTool.nRightExt = -1;
	m_stAnalysisTool.nTopText = -1;
	m_stAnalysisTool.nBottomText = -1;
	m_stAnalysisTool.nLeftText = -1;
	m_stAnalysisTool.nRightText = -1;
	m_stAnalysisTool.nRateText = -1;
	
	SetColor(m_basePenEnvironment.GetColor());
	
	InitializeData_Width();
	InitializeData_Style();
	UpdateData(FALSE);

	GetDlgItem(IDC_STATIC_XDATE3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_YVALUE3)->ShowWindow(SW_HIDE);
	m_ctrlXDateTime3.ShowWindow(SW_HIDE);
	m_ctrlEditValue3.ShowWindow(SW_HIDE);

	m_ctrlEditXValue1.ShowWindow(SW_HIDE);
	m_ctrlEditXValue2.ShowWindow(SW_HIDE);
	m_ctrlEditXValue3.ShowWindow(SW_HIDE);

	m_ctrlEquiDiv.ShowWindow(SW_HIDE);
	m_ctrlCheckFill.ShowWindow(SW_HIDE);

	m_ctrlLeftExt.ShowWindow(SW_HIDE);
	m_ctrlRightExt.ShowWindow(SW_HIDE);
	m_ctrlTopText.ShowWindow(SW_HIDE);
	m_ctrlBottomText.ShowWindow(SW_HIDE);

	///////////////////////////////////////////
	m_ctrlCheckHL.ShowWindow(SW_HIDE);			//���ݺ�ȭ������ ���
	///////////////////////////////////////////

	m_ctrlRateText.ShowWindow(SW_HIDE);

	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_chkPriceRangeDiv.ShowWindow( SW_HIDE);

	if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Pencil_Line)
	{
		m_ctrlLeftText.ShowWindow(SW_HIDE);
		m_ctrlRightText.ShowWindow(SW_HIDE);

		//���� ��¥�� ����
		m_ctrlXDateTime1.ShowWindow(SW_HIDE);
		m_ctrlEditValue1.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XDATE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE1)->ShowWindow(SW_HIDE);

		m_ctrlXDateTime2.ShowWindow(SW_HIDE);
		m_ctrlEditValue2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_TEXT_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DATE_N_VALUE_STATIC)->ShowWindow(SW_HIDE);

		///////////////////////////////////////////////////////////////////////////////

		CRect rect;
		GetWindowRect(&rect);
		 
		int iExpandSize = 140;
		rect.bottom -= iExpandSize;		
		MoveWindow(rect);	
		
		CRect rectPos;
		CStatic *pStatic = (CStatic *)GetDlgItem(IDC_DATE_N_VALUE_STATIC);
		pStatic->GetWindowRect(&rectPos);
		ScreenToClient(&rectPos);	

		int nTempX = rectPos.left;

		pStatic = (CStatic *)GetDlgItem(IDC_PEN_STATIC);
		pStatic->GetClientRect(&rect);
		pStatic->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

		GetDlgItem(IDC_STATIC_PEN_COLOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PEN_STYLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PEN_WIDTH)->ShowWindow(SW_SHOW);

		IMetaTable::E_LANGUAGEID eLanguageID = g_iMetaTable.GetCurrentLanguageID( m_hOcxWnd); 
		switch(eLanguageID)
		{
		case IMetaTable::ENGLISH:
			{
				rectPos.left += 50;
				rectPos.top += 20;
				CButton *pBtn = (CButton *)GetDlgItem(IDC_COLOR);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

				rectPos.top -= 3;
				rectPos.left = rectPos.left + rect.Width() + 75;
				CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_WIDTH);
				pCombo->GetClientRect(&rect);
				pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

				rectPos.left = rectPos.left + rect.Width() + 40;
				pCombo = (CComboBox *)GetDlgItem(IDC_STYLE);
				pCombo->GetClientRect(&rect);
				pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

				nTempX += 183;
				pBtn = (CButton *)GetDlgItem(IDC_OK_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());

				nTempX = nTempX + rect.Width() + 2;
				pBtn = (CButton *)GetDlgItem(IDC_CANCEL_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());

				nTempX = nTempX + rect.Width() + 2;
				pBtn = (CButton *)GetDlgItem(IDC_APPLY_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());
			}
			break;
		case IMetaTable::KOREAN:
			{
				rectPos.left += 35;
				rectPos.top += 20;
				CButton *pBtn = (CButton *)GetDlgItem(IDC_COLOR);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());
 
				rectPos.top -= 3;
				rectPos.left = rectPos.left + rect.Width() + 35;
				CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_WIDTH);
				pCombo->GetClientRect(&rect);
				pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

				rectPos.left = rectPos.left + rect.Width() + 40;
				pCombo = (CComboBox *)GetDlgItem(IDC_STYLE);
				pCombo->GetClientRect(&rect);
				pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());

				nTempX += 130;
				pBtn = (CButton *)GetDlgItem(IDC_OK_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());

				nTempX = nTempX + rect.Width() + 2;
				pBtn = (CButton *)GetDlgItem(IDC_CANCEL_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());

				nTempX = nTempX + rect.Width() + 2;
				pBtn = (CButton *)GetDlgItem(IDC_APPLY_BT);
				pBtn->GetClientRect(&rect);
				pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());
			}
			break;
		default:
			ASSERT(FALSE);
		};
		
//		rectPos.left += 35;
//		rectPos.top += 20;
//		CButton *pBtn = (CButton *)GetDlgItem(IDC_COLOR);
//		pBtn->GetClientRect(&rect);
//		pBtn->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());
// 
//		rectPos.top -= 3;
//		rectPos.left = rectPos.left + rect.Width() + 35;
//		CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_WIDTH);
//		pCombo->GetClientRect(&rect);
//		pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());
//
//		rectPos.left = rectPos.left + rect.Width() + 40;
//		pCombo = (CComboBox *)GetDlgItem(IDC_STYLE);
//		pCombo->GetClientRect(&rect);
//		pCombo->MoveWindow(rectPos.left, rectPos.top, rect.Width(), rect.Height());
//
//		nTempX += 130;
//		pBtn = (CButton *)GetDlgItem(IDC_OK_BT);
//		pBtn->GetClientRect(&rect);
//		pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());
//
//		nTempX = nTempX + rect.Width() + 2;
//		pBtn = (CButton *)GetDlgItem(IDC_CANCEL_BT);
//		pBtn->GetClientRect(&rect);
//		pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());
//
//		nTempX = nTempX + rect.Width() + 2;
//		pBtn = (CButton *)GetDlgItem(IDC_APPLY_BT);
//		pBtn->GetClientRect(&rect);
//		pBtn->MoveWindow(nTempX, rectPos.top + 45, rect.Width(), rect.Height());

		///////////////////////////////////////////////////////////////////////////////
	}
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Cross_Line)		
	{
		m_ctrlTopText.ShowWindow(SW_SHOW);
		m_ctrlBottomText.ShowWindow(SW_SHOW);

		m_nTextPosType = G_TEXT_POS_HV;
		m_ctrlLeftText.SetCheck(m_pTextHVPosition->GetLeft());
		m_ctrlRightText.SetCheck(m_pTextHVPosition->GetRight());
		m_ctrlTopText.SetCheck(m_pTextHVPosition->GetTop());
		m_ctrlBottomText.SetCheck(m_pTextHVPosition->GetBottom());		

		m_stAnalysisTool.nTopText = m_pTextHVPosition->GetTop();
		m_stAnalysisTool.nBottomText = m_pTextHVPosition->GetBottom();
		m_stAnalysisTool.nLeftText = m_pTextHVPosition->GetLeft();
		m_stAnalysisTool.nRightText = m_pTextHVPosition->GetRight();
		
		//���� ��¥�� ����
		m_ctrlXDateTime2.ShowWindow(SW_HIDE);
		m_ctrlEditValue2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);
	}
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Horz_Line)	//����
	{
		m_nTextPosType = G_TEXT_POS_HORZ;
		m_ctrlLeftText.SetCheck(m_pTextHPosition->GetLeft());
		m_ctrlRightText.SetCheck(m_pTextHPosition->GetRight());

		m_stAnalysisTool.nLeftText = m_pTextHPosition->GetLeft();
		m_stAnalysisTool.nRightText = m_pTextHPosition->GetRight();

		//���� ����
		m_ctrlXDateTime2.ShowWindow(SW_HIDE);
		m_ctrlEditValue2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);
	} 	
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Vert_Line ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Time)	//������
	{
		if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Vert_Line)
		{
			m_nTextPosType = G_TEXT_POS_VERT;

			ML_SET_LANGUAGE_RES();

			CString strTitle;
			strTitle.LoadString(IDS_UPPEREND);
			SetDlgItemText(IDC_CHECK_TEXT_LEFT,strTitle);
			strTitle.LoadString(IDS_LOWEREND);
			SetDlgItemText(IDC_CHECK_TEXT_RIGHT,strTitle);
			m_ctrlLeftText.SetCheck(m_pTextVPosition->GetTop());
			m_ctrlRightText.SetCheck(m_pTextVPosition->GetBottom());
			
			m_stAnalysisTool.nTopText = m_pTextVPosition->GetTop();
			m_stAnalysisTool.nBottomText = m_pTextVPosition->GetBottom();
		}
		else
		{
			// 2008.08.20 ������ �Ǻ���ġ Ȯ��=>	�Ǻ���ġ�������� Text �����ֱ� ����<<
			/*
			GetDlgItem(IDC_TEXT_STATIC)->ShowWindow(SW_HIDE);
			m_ctrlLeftText.ShowWindow(SW_HIDE);
			m_ctrlRightText.ShowWindow(SW_HIDE);
			*/
			m_nTextPosType = G_TEXT_POS_ONE;
			
			ML_SET_LANGUAGE_RES();

			CString strTitle;
			strTitle.LoadString(IDS_SHOW);
			SetDlgItemText(IDC_CHECK_TEXT_LEFT,strTitle);
			
			m_ctrlRightText.ShowWindow(SW_HIDE);		
			m_ctrlLeftText.SetCheck(m_pTextPosition->IsShow());
			
			m_stAnalysisTool.nLeftText = m_pTextPosition->IsShow();
			//>>
		}

		//���� ��¥
		m_ctrlEditValue1.EnableWindow(FALSE);
		m_ctrlXDateTime2.ShowWindow(SW_HIDE);
		m_ctrlEditValue2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);
	}
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Line		||	//�߼���
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Period_Summary)	
	{
		BOOL bRight = m_pTextHPosition->GetRight();
		m_nTextPosType = G_TEXT_POS_HORZ;
		m_nLineExtType = G_LINE_EXT_BOTH;
		m_ctrlLeftText.SetCheck(m_pTextHPosition->GetLeft());
		m_ctrlRightText.SetCheck(bRight);
		if ((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Line)
		{
			m_ctrlRateText.ShowWindow(SW_SHOW);
			m_ctrlRateText.EnableWindow(bRight);
			m_ctrlRateText.SetCheck(m_pTextHPosition->GetRightRate());
		}


		m_stAnalysisTool.nLeftText = m_pTextHPosition->GetLeft();
		m_stAnalysisTool.nRightText = bRight;
		m_stAnalysisTool.nRateText = m_pTextHPosition->GetRightRate();
		
		m_ctrlLeftExt.ShowWindow(SW_SHOW);
		m_ctrlRightExt.ShowWindow(SW_SHOW);

		m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());
		m_ctrlRightExt.SetCheck(m_pLineExtEnvironment->GetRightExt());
		
		m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
		m_stAnalysisTool.nRightExt = m_pLineExtEnvironment->GetRightExt();

		if((CToolEnumData::ElementType)m_nElementType != CToolEnumData::Line)
		{
			m_ctrlEquiDiv.ShowWindow(SW_SHOW);
			m_ctrlEquiDiv.SetCheck(m_baseEquiDivEnvironment);
			m_nEquiDivLine = m_baseEquiDivEnvironment;		

			m_stAnalysisTool.nEquiDiv = m_nEquiDivLine;
		}
	}
	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Trisection	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Quadrisect)	
	{
		m_nTextPosType = G_TEXT_POS_HORZ;
		m_nLineExtType = G_LINE_EXT_BOTH;
		m_ctrlLeftText.SetCheck(m_pTextHPosition->GetLeft());
		m_ctrlRightText.SetCheck(m_pTextHPosition->GetRight());

		m_stAnalysisTool.nLeftText = m_pTextHPosition->GetLeft();
		m_stAnalysisTool.nRightText = m_pTextHPosition->GetRight();
		
		m_ctrlLeftExt.ShowWindow(SW_SHOW);
		m_ctrlRightExt.ShowWindow(SW_SHOW);

		m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());
		m_ctrlRightExt.SetCheck(m_pLineExtEnvironment->GetRightExt());
		
		m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
		m_stAnalysisTool.nRightExt = m_pLineExtEnvironment->GetRightExt();

		m_ctrlEquiDiv.ShowWindow(SW_SHOW);
		m_ctrlEquiDiv.SetCheck(m_baseEquiDivEnvironment);
		m_nEquiDivLine = m_baseEquiDivEnvironment;		
		m_stAnalysisTool.nEquiDiv = m_nEquiDivLine;

		// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
		m_chkPriceRangeDiv.ShowWindow( SW_SHOW);
		m_chkPriceRangeDiv.SetCheck( m_basePriceRangeDivEnvironment);
		m_nPriceRangeDivLine = m_basePriceRangeDivEnvironment;		
		m_stAnalysisTool.nPriceRangeDiv = m_nPriceRangeDivLine;
		if( G_PRICE_RANGE_DIV_CHECK == m_nPriceRangeDivLine)
		{
			m_ctrlEditValue1.EnableWindow( FALSE);
			m_ctrlEditValue2.EnableWindow( FALSE);
		}
	}
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Retrace	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Semicircle ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Circle	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Fan_Line	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan1	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan2	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan3	||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan4)
	{
		m_nTextPosType = G_TEXT_POS_ONE;
		
		ML_SET_LANGUAGE_RES();

		CString strTitle;
		strTitle.LoadString(IDS_SHOW);
		SetDlgItemText(IDC_CHECK_TEXT_LEFT,strTitle);
		
		m_ctrlRightText.ShowWindow(SW_HIDE);		
		m_ctrlLeftText.SetCheck(m_pTextPosition->IsShow());
		
		m_stAnalysisTool.nLeftText = m_pTextPosition->IsShow();

		if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Retrace)
		{
			m_nLineExtType = G_LINE_EXT_BOTH;
			m_ctrlLeftExt.ShowWindow(SW_SHOW);
			m_ctrlRightExt.ShowWindow(SW_SHOW);

			m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());
			m_ctrlRightExt.SetCheck(m_pLineExtEnvironment->GetRightExt());

			m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
			m_stAnalysisTool.nRightExt = m_pLineExtEnvironment->GetRightExt();
		}
	} 
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Angle_Line)
	{
		m_nTextPosType = G_TEXT_POS_ONE;
		
		ML_SET_LANGUAGE_RES();

		CString strTitle;
		strTitle.LoadString(IDS_ANGLE);
		SetDlgItemText(IDC_CHECK_TEXT_LEFT,strTitle);
		strTitle.LoadString(IDS_EXTENSIONLINE);
		SetDlgItemText(IDC_CHECK_EXT_LEFT,strTitle);		

		m_ctrlRightText.ShowWindow(SW_HIDE);		
		m_ctrlLeftText.SetCheck(m_pTextPosition->IsShow());
		
		m_stAnalysisTool.nLeftText = m_pTextPosition->IsShow();
		
		m_nLineExtType = G_LINE_EXT_RIGHT;
		m_ctrlLeftExt.ShowWindow(SW_SHOW);
		m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());

		m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
	}
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Candle_Line)	//���߽ɼ�
	{
		m_nTextPosType = G_TEXT_POS_ONE;

		ML_SET_LANGUAGE_RES();

		CString strTitle;
		strTitle.LoadString(IDS_SHOW);
		SetDlgItemText(IDC_CHECK_TEXT_LEFT,strTitle);
		m_ctrlRightText.ShowWindow(SW_HIDE);		
		m_ctrlLeftText.SetCheck(m_pTextPosition->IsShow());

		m_stAnalysisTool.nLeftText = m_pTextPosition->IsShow();

		//���� ����
		m_ctrlXDateTime2.ShowWindow(SW_HIDE);
		m_ctrlEditValue2.ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);
	} 
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Linear_Regression  ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Raff_Regression)//	||
			//(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Standard_Deviation	||
			//(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Standard_Error
	{
		GetDlgItem(IDC_TEXT_STATIC)->ShowWindow(SW_HIDE);
		m_ctrlLeftText.ShowWindow(SW_HIDE);
		m_ctrlRightText.ShowWindow(SW_HIDE);

		m_nLineExtType = G_LINE_EXT_BOTH;
		m_ctrlLeftExt.ShowWindow(SW_SHOW);
		m_ctrlRightExt.ShowWindow(SW_SHOW);

		m_ctrlLeftExt.SetCheck(m_pLineExtEnvironment->GetLeftExt());
		m_ctrlRightExt.SetCheck(m_pLineExtEnvironment->GetRightExt());

		m_stAnalysisTool.nLeftExt = m_pLineExtEnvironment->GetLeftExt();
		m_stAnalysisTool.nRightExt = m_pLineExtEnvironment->GetRightExt();
	} 
	else if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::AndrewsPitchfork)
	{
		ML_SET_LANGUAGE_RES();

		CString strTitle;
		strTitle.LoadString(IDS_MIDDATE);
		SetDlgItemText(IDC_STATIC_XDATE2,strTitle);

		GetDlgItem(IDC_STATIC_XDATE3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_YVALUE3)->ShowWindow(SW_SHOW);
		m_ctrlXDateTime3.ShowWindow(SW_SHOW);
		m_ctrlEditValue3.ShowWindow(SW_SHOW);

		GetDlgItem(IDC_TEXT_STATIC)->ShowWindow(SW_HIDE);
		m_ctrlLeftText.ShowWindow(SW_HIDE);
		m_ctrlRightText.ShowWindow(SW_HIDE);
	}
	else if(//(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Semicircle ||
			//(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Circle	 ||
			//(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Fibo_Fan_Line	 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Line1		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Line2		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan1		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan2		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan3		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan4		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Grid		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Speed_Line		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Speed_Fan		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Speed_Arc		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Cycle_Line		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::ElliotWave_Line)
	{
		if((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan1		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan2		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan3		 ||
			(CToolEnumData::ElementType)m_nElementType == CToolEnumData::Gann_Fan4)
		{
			m_ctrlXDateTime2.ShowWindow(SW_HIDE);
			m_ctrlEditValue2.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_XDATE2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_YVALUE2)->ShowWindow(SW_HIDE);
		}
		
		GetDlgItem(IDC_TEXT_STATIC)->ShowWindow(SW_HIDE);
		m_ctrlLeftText.ShowWindow(SW_HIDE);
		m_ctrlRightText.ShowWindow(SW_HIDE);
	}

	if((CToolEnumData::ElementType)m_nElementType != CToolEnumData::Pencil_Line)
	{
		if(m_pElementPointList != NULL)
		{
			CCoordinate *pCoordinate = NULL;
			for(int i=0;i<m_pElementPointList->GetCount();i++)
			{
				pCoordinate = m_pElementPointList->GetAt(i);

				// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
				//SetXDateTime(pCoordinate->GetX(),i);

				double dX = pCoordinate->GetX();
				if (m_strFormatX == _T("YYYYMMDDHHMMSS"))
				{
					if( m_nDataPeriod == CScaleBaseData::HORZ_WEEKLEY	||
						m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
					{
						// �ְ�, ������ ���� ��Ʈ ��¥�ֱ�� �����Ѵ�.
						IChartManager *pIChartManager = m_pIChartOCX->GetIChartManager();
						if (pIChartManager) 
						{
							// 
							int nRow = m_blockIndex.GetRow();
							int nCol = m_blockIndex.GetColumn();

							int nCenter_x;
							int nOrgXPos		= pIChartManager->GetXFromDouble(nRow, nCol, dX, nCenter_x);

							int nBlockColumn;
							double dModifyData	= pIChartManager->GetDoubleFromX(nRow, nCol, nOrgXPos, &nBlockColumn, nCenter_x);
							dX = dModifyData;

							pIChartManager->Release();
							pIChartManager = NULL;
						}
					}
				}

				SetXDateTime(dX,i);
				// 2011.02.08 by SYS <<

				SetYValue(pCoordinate->GetY(),i);
			}
		}
	}	
}

void CDlgAnalysisToolSet::SetInitTextPosition(CTextPosition* pTextPosition)
{
	if(m_nElementType == CToolEnumData::Cross_Line)
	{
		m_pTextPosition = NULL;
		m_pTextHPosition = NULL;
		m_pTextVPosition = NULL;
		
		m_pTextHVPosition = (CTextHorzVertPosition *)pTextPosition;
		m_baseTextHVPosition.Set(pTextPosition);
	}
	else if(m_nElementType == CToolEnumData::Vert_Line)
	{
		m_pTextPosition = NULL;
		m_pTextHPosition = NULL;
		m_pTextHVPosition = NULL;
		
		m_pTextVPosition = (CTextVertPosition *)pTextPosition;
		m_baseTextVPosition.Set(pTextPosition);
	}
	else if(m_nElementType == CToolEnumData::Fibo_Retrace	||
			m_nElementType == CToolEnumData::Fibo_Semicircle	||
			m_nElementType == CToolEnumData::Fibo_Circle	||
			m_nElementType == CToolEnumData::Fibo_Fan_Line	||
			m_nElementType == CToolEnumData::Fibo_Time	||	// 2008.08.20 ������ �Ǻ���ġ�������� Text �����ֱ� ����
			m_nElementType == CToolEnumData::Angle_Line		||
			m_nElementType == CToolEnumData::Candle_Line	||
			m_nElementType == CToolEnumData::Gann_Fan1	||
			m_nElementType == CToolEnumData::Gann_Fan2	||
			m_nElementType == CToolEnumData::Gann_Fan3	||
			m_nElementType == CToolEnumData::Gann_Fan4)
	{
		m_pTextVPosition = NULL;
		m_pTextHPosition = NULL;
		m_pTextHVPosition = NULL;
		
		m_pTextPosition = (CTextShowPosition *)pTextPosition;
		m_baseTextPosition.Set(pTextPosition);
	}
	else if(m_nElementType == CToolEnumData::Line		||
			m_nElementType == CToolEnumData::Horz_Line	||
			m_nElementType == CToolEnumData::Trisection	||
			m_nElementType == CToolEnumData::Quadrisect	||
			m_nElementType == CToolEnumData::Period_Summary)
	{
		m_pTextPosition = NULL;
		m_pTextVPosition = NULL;
		m_pTextHVPosition = NULL;

		m_pTextHPosition = (CTextHorzPosition *)pTextPosition;
		m_baseTextHPosition.Set(pTextPosition);
	}
	else		//NoText
	{
		m_pTextPosition = NULL;
		m_pTextVPosition = NULL;
		m_pTextHPosition = NULL;
		m_pTextHVPosition = NULL;
	}
}

// private ====================================================================
void CDlgAnalysisToolSet::InitializeData_Width()
{
	ML_SET_DEFAULT_RES();

	m_ctlWidth.LoadBitmap(IDB_LINEWIDTH, 5);
	if(m_basePenEnvironment.GetWidth() <= 0)
		m_nWidth = 0;
	else if(m_basePenEnvironment.GetWidth() >= 6)
		m_nWidth = 4;
	else
		m_nWidth = m_basePenEnvironment.GetWidth() -1;
}

void CDlgAnalysisToolSet::InitializeData_Style()
{
	ML_SET_DEFAULT_RES();

	m_ctlStyle.LoadBitmap(IDB_LINESTYLE, 5);
	m_nStyle = (int) m_basePenEnvironment.GetStyle();
}

void CDlgAnalysisToolSet::SetPenEnvironment()
{
	UpdateData(TRUE);
	m_pPenEnvironment->SetColor(m_ctlColor.GetColor());
	m_pPenEnvironment->SetWidth(m_nWidth+1);
	m_pPenEnvironment->SetStyle((CToolEnumData::PenStyle) m_nStyle);
	
	m_stAnalysisTool.crLineColor = m_ctlColor.GetColor();
	m_stAnalysisTool.nLineStyle = m_nStyle;
	m_stAnalysisTool.nLineWeight = m_nWidth+1;	
}

void CDlgAnalysisToolSet::SetLineExtEnvironment()
{
	UpdateData(TRUE);
	m_pLineExtEnvironment->SetLineExt(m_ctrlLeftExt.GetCheck(),m_ctrlRightExt.GetCheck());

	m_stAnalysisTool.nLeftExt = m_ctrlLeftExt.GetCheck();
	m_stAnalysisTool.nRightExt = m_ctrlRightExt.GetCheck();	
}

void CDlgAnalysisToolSet::SetLineExtEnvironment(const bool bLeft,const bool bRight)
{
	m_pLineExtEnvironment->SetLineExt(bLeft,bRight);
}

void CDlgAnalysisToolSet::SetColor(const COLORREF& color)
{
	m_ctlColor.SetColor(color);
	m_ctlColor.Invalidate();
}

void CDlgAnalysisToolSet::SetTextPosition()
{
	m_stAnalysisTool.dEndXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dEndYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.dStartXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dStartYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.dThirdXPos = G_NOTAVAILABLE;
	m_stAnalysisTool.dThirdYValue = G_NOTAVAILABLE;
	m_stAnalysisTool.nEquiDiv = -1;
	m_stAnalysisTool.nPriceRangeDiv = -1;	// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	m_stAnalysisTool.nHLPercent = -1;
	m_stAnalysisTool.nFillRegion = -1;	

	if(m_nTextPosType == G_TEXT_POS_HV)
	{
		SetTextPosition( ( m_ctrlLeftText.GetCheck() ? true : false),
							( m_ctrlRightText.GetCheck() ? true : false),
							( m_ctrlTopText.GetCheck() ? true : false),
							( m_ctrlBottomText.GetCheck() ? true : false));

		m_stAnalysisTool.nTopText = (int)m_ctrlTopText.GetCheck();
		m_stAnalysisTool.nBottomText = (int)m_ctrlBottomText.GetCheck();
		m_stAnalysisTool.nLeftText = (int)m_ctrlLeftText.GetCheck();
		m_stAnalysisTool.nRightText = (int)m_ctrlRightText.GetCheck();	
		m_stAnalysisTool.nRateText = (int)m_ctrlRateText.GetCheck();
	}
	else if(m_nTextPosType == G_TEXT_POS_ONE)
	{
		SetTextPosition( m_ctrlLeftText.GetCheck() ? true : false);

		m_stAnalysisTool.nLeftText = (int)m_ctrlLeftText.GetCheck();
	}
	else 
	{
		SetTextPosition( ( m_ctrlLeftText.GetCheck() ? true : false),
							( m_ctrlRightText.GetCheck() ? true : false));

		m_stAnalysisTool.nRateText = (int)m_ctrlRateText.GetCheck() ? true : false;
	}	
}

void CDlgAnalysisToolSet::SetTextPosition(const bool bShow)
{
	m_pTextPosition->GetTextEnvironment().SetColor(m_ctlColor.GetColor());
	m_pTextPosition->SetShow(bShow);
}

void CDlgAnalysisToolSet::SetTextPosition(const bool bLeft, const bool bRight)
{
	if(m_nTextPosType == G_TEXT_POS_VERT && m_pTextVPosition != NULL)
	{
		m_pTextVPosition->SetTop(bLeft);
		m_pTextVPosition->SetBottom(bRight);
		m_pTextVPosition->GetTextEnvironment().SetColor(m_ctlColor.GetColor());

		m_stAnalysisTool.nTopText = (int)bLeft;
		m_stAnalysisTool.nBottomText = (int)bRight;
	}
	else if(m_nTextPosType == G_TEXT_POS_HORZ && m_pTextHPosition != NULL)
	{
		m_pTextHPosition->SetLeft(bLeft);
		m_pTextHPosition->SetRight(bRight);
		m_pTextHPosition->SetRightRate(m_ctrlRateText.GetCheck());
		m_pTextHPosition->GetTextEnvironment().SetColor(m_ctlColor.GetColor());

		m_stAnalysisTool.nLeftText = (int)bLeft;
		m_stAnalysisTool.nRightText = (int)bRight;	
		m_stAnalysisTool.nRateText = (int)m_ctrlRateText.GetCheck();
	}	
}

void CDlgAnalysisToolSet::SetTextPosition(const bool bLeft, const bool bRight,const bool bTop, const bool bBottom)
{
	if(m_pTextHVPosition == NULL)
		return;

	m_pTextHVPosition->SetLeft(bLeft);
	m_pTextHVPosition->SetRight(bRight);
	m_pTextHVPosition->SetTop(bTop);
	m_pTextHVPosition->SetBottom(bBottom);
	m_pTextHVPosition->GetTextEnvironment().SetColor(m_ctlColor.GetColor());
}

CTextPosition* CDlgAnalysisToolSet::GetTextPosition()
{
	if(m_nTextPosType == G_TEXT_POS_HV)
		return m_pTextHVPosition;
	else if(m_nTextPosType == G_TEXT_POS_VERT)
		return m_pTextVPosition;
	else if(m_nTextPosType == G_TEXT_POS_HORZ)
		return m_pTextHPosition;
	else if(m_nTextPosType == G_TEXT_POS_ONE)
		return m_pTextPosition;

	return NULL;
}

CTextPosition* CDlgAnalysisToolSet::GetOrgTextPosition()
{
	if(m_nTextPosType == G_TEXT_POS_HV)
		return &m_baseTextHVPosition;
	else if(m_nTextPosType == G_TEXT_POS_VERT)
		return &m_baseTextVPosition;
	else if(m_nTextPosType == G_TEXT_POS_HORZ)
		return &m_baseTextHPosition;
	else if(m_nTextPosType == G_TEXT_POS_ONE)
		return &m_baseTextPosition;

	return NULL;
}

CLineExtEnvironment* CDlgAnalysisToolSet::GetLineExtEnvironment()
{
	return m_pLineExtEnvironment;
}

CLineExtEnvironment* CDlgAnalysisToolSet::GetOrgLineExtEnvironment()
{
	return &m_baseLineExtEnvironment;
}

CPenEnvironment* CDlgAnalysisToolSet::GetCurrentPenEnvironment() const
{
	return m_pPenEnvironment;
}

void CDlgAnalysisToolSet::SetXDateTime(double dXDateTime,int nIndex)
{
	CString strXValue;
	strXValue.Format(_T("%.f"),dXDateTime);	
	if(strXValue.IsEmpty() == FALSE && dXDateTime > 0.0f)
	{	
		if( m_nDataPeriod == CScaleBaseData::HORZ_DAILY		||
			m_nDataPeriod == CScaleBaseData::HORZ_WEEKLEY	||
			m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
		{
			if(nIndex == 0)
			{
				m_ctrlEditXValue1.ShowWindow(SW_HIDE);
				m_ctrlEditXValue2.ShowWindow(SW_HIDE);
				m_ctrlEditXValue3.ShowWindow(SW_HIDE);
			}

			int nYear = atoi(strXValue.Left(4));
			int nMonth = atoi(strXValue.Mid(4,2));
			int nDay = atoi(strXValue.Mid(6,2));

			CTime stime(nYear,nMonth,max(1,nDay),0,0,0);
			if(nIndex == 0)
				m_ctrlXDateTime1.SetTime(&stime);	
			else if(nIndex == 1)
				m_ctrlXDateTime2.SetTime(&stime);	
			else if(nIndex == 2)
				m_ctrlXDateTime3.SetTime(&stime);	
		}
		else
		{	
			// ���̳� ƽ�ΰ�쿡 ó��..
			if(nIndex == 0)
			{
				m_ctrlXDateTime1.ShowWindow(SW_HIDE);
				m_ctrlXDateTime2.ShowWindow(SW_HIDE);
				m_ctrlXDateTime3.ShowWindow(SW_HIDE);
			}

			CString strFormat;		
			

			if(m_nDataPeriod == CScaleBaseData::HORZ_TICK)
			{
				// 2011.01.17 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
				// DDHHMM �� ����
				if (strXValue.GetLength()==14)
					strFormat.Format(_T("%s/%s:%s"),strXValue.Mid(6,2),strXValue.Mid(8,2),strXValue.Mid(10,2));
				else							
					strFormat.Format(_T("%s/%s:%s"),strXValue.Left(2),strXValue.Mid(2,2),strXValue.Mid(4,2));
				// 2011.01.17 by SYS <<
			}
			// 2011.01.20 by SYS <<
			else
			{
				if (strXValue.GetLength() < 8)
					strXValue = "0" + strXValue;

				// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
				// MMDDHHMM �� ����
				if (strXValue.GetLength()==14)
				{
					strFormat.Format(_T("%s/%s-%s:%s"), strXValue.Mid(4,2),strXValue.Mid(6,2), strXValue.Mid(8,2),strXValue.Mid(10,2));
				}
				else 
				{
					strFormat.Format(_T("%s/%s-%s:%s"),strXValue.Left(2),strXValue.Mid(2,2), strXValue.Mid(4,2),strXValue.Mid(6,2));
				}
				// 2011.01.06 by SYS <<
			}
			//
			
			if(nIndex == 0)
			{
				m_ctrlEditXValue1.ShowWindow(SW_SHOW);
				m_ctrlEditXValue1.SetWindowText(strFormat);
				m_ctrlEditXValue1.EnableWindow(FALSE);
			}
			else if(nIndex == 1)
			{
				m_ctrlEditXValue2.ShowWindow(SW_SHOW);
				m_ctrlEditXValue2.SetWindowText(strFormat);
				m_ctrlEditXValue2.EnableWindow(FALSE);
			}
			else if(nIndex == 2)
			{
				m_ctrlEditXValue3.ShowWindow(SW_SHOW);
				m_ctrlEditXValue3.SetWindowText(strFormat);
				m_ctrlEditXValue3.EnableWindow(FALSE);
			}
		}
	}
}

void CDlgAnalysisToolSet::SetYValue(double dYValue,int nIndex)
{
	CString strYValue;
	// 20080911 ������	ȯ�漳�� ȭ�鿡�� �Ҽ��� ���� �ڸ��� �߸��� ���� ����
	//	strYValue.Format(_T("%.f"),dYValue);
	// 2008.10.15 by LYH >> 8,32bit����
	//strYValue.Format(_T("%f"),dYValue);
	strYValue = CDataConversion::DoubleToString(dYValue, m_pDataEnvironment->GetStrVert());
	// 2008.10.15 by LYH <<
	if(strYValue.IsEmpty() == FALSE)
	{	
		if(nIndex == 0)
			m_ctrlEditValue1.SetWindowText(strYValue);	
		else if(nIndex == 1)
			m_ctrlEditValue2.SetWindowText(strYValue);	
		else if(nIndex == 2)
			m_ctrlEditValue3.SetWindowText(strYValue);	
	}
}

void CDlgAnalysisToolSet::OnDatetimechangeDatetimepickerXdate1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strXDate;
	CTime xTime;
	m_ctrlXDateTime1.GetTime(xTime);

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
	//CCoordinate *pCoordinate = m_pElementPointList->GetAt(0);
	//pCoordinate->SetX(atof(strXDate),0);

	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
	__int64 lMakeTime = _atoi64(strXDate);
	CCoordinate *pCoordinate = m_pElementPointList->GetAt(0);
	pCoordinate->SetX((double)lMakeTime,0);
	// 2011.02.08 by SYS <<

	*pResult = 0;
}

void CDlgAnalysisToolSet::OnDatetimechangeDatetimepickerXdate2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strXDate;
	CTime xTime;
	m_ctrlXDateTime2.GetTime(xTime);

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
	//CCoordinate *pCoordinate = m_pElementPointList->GetAt(1);
	//pCoordinate->SetX(atof(strXDate),1);
	
	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
	__int64 lMakeTime = _atoi64(strXDate);
	CCoordinate *pCoordinate = m_pElementPointList->GetAt(1);
	pCoordinate->SetX((double)lMakeTime,1);
	// 2011.02.08 by SYS <<

	*pResult = 0;
}

void CDlgAnalysisToolSet::OnDatetimechangeDatetimepickerXdate3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString strXDate;
	CTime xTime;
	m_ctrlXDateTime3.GetTime(xTime);

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	//strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
	//CCoordinate *pCoordinate = m_pElementPointList->GetAt(2);
	//pCoordinate->SetX(atof(strXDate),2);
	
	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
	__int64 lMakeTime = _atoi64(strXDate);
	CCoordinate *pCoordinate = m_pElementPointList->GetAt(2);
	pCoordinate->SetX((double)lMakeTime,2);
	// 2011.02.08 by SYS <<

	*pResult = 0;
}

void CDlgAnalysisToolSet::SetDateValueLists()
{
	CString strXDate;
	CTime xTime;
	CCoordinate *pCoordinate = NULL;
	if(m_ctrlXDateTime1.IsWindowVisible() == TRUE)
	{
		m_ctrlXDateTime1.GetTime(xTime);
		
		// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 		if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
// 			strXDate.Format(_T("%04d%02d"),xTime.GetYear(),xTime.GetMonth());
// 		else
// 			strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
// 	
// 		pCoordinate = m_pElementPointList->GetAt(0);
// 		pCoordinate->SetX(atof(strXDate),0);
// 		m_stAnalysisTool.dStartXPos = atof(strXDate);

		//if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(), 1, _T("000000"));
		//else
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		__int64 lMakeTime = _atoi64(strXDate);

		pCoordinate = m_pElementPointList->GetAt(0);
		pCoordinate->SetX((double)lMakeTime,0);
		m_stAnalysisTool.dStartXPos = (double)lMakeTime;

		// 2011.02.08 by SYS <<
	}
	
	if(m_ctrlXDateTime2.IsWindowVisible() == TRUE)
	{
		m_ctrlXDateTime2.GetTime(xTime);

		// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 		if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
// 			strXDate.Format(_T("%04d%02d"),xTime.GetYear(),xTime.GetMonth());
// 		else
// 			strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
// 		pCoordinate = m_pElementPointList->GetAt(1);
// 		pCoordinate->SetX(atof(strXDate),1);
// 		m_stAnalysisTool.dEndXPos = atof(strXDate);

		//if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(), 1, _T("000000"));
		//else
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		__int64 lMakeTime = _atoi64(strXDate);

		pCoordinate = m_pElementPointList->GetAt(1);
		pCoordinate->SetX((double)lMakeTime,1);
		m_stAnalysisTool.dEndXPos = (double)lMakeTime;
		// 2011.02.08 by SYS <<
	}

	if(m_ctrlXDateTime3.IsWindowVisible() == TRUE)
	{
		m_ctrlXDateTime3.GetTime(xTime);

		// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
// 		if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
// 			strXDate.Format(_T("%04d%02d"),xTime.GetYear(),xTime.GetMonth());
// 		else
// 			strXDate.Format(_T("%04d%02d%02d"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay());
// 		pCoordinate = m_pElementPointList->GetAt(2);
// 		pCoordinate->SetX(atof(strXDate),2);
// 		m_stAnalysisTool.dThirdXPos = atof(strXDate);

		//if( m_nDataPeriod == CScaleBaseData::HORZ_MONTHLY)
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(), 1, _T("000000"));
		//else
		//	strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		strXDate.Format(_T("%04d%02d%02d%s"),xTime.GetYear(),xTime.GetMonth(),xTime.GetDay(), _T("000000"));
		__int64 lMakeTime = _atoi64(strXDate);

		pCoordinate = m_pElementPointList->GetAt(2);
		pCoordinate->SetX((double)lMakeTime,2);
		m_stAnalysisTool.dThirdXPos = (double)lMakeTime;
		// 2011.02.08 by SYS <<
	}

// 2008.10.15 by LYH >> 8,32bit����
	CString strValue;
// 2008.10.15 by LYH <<
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CString strYValue;
	if(m_ctrlEditValue1.IsWindowVisible() == TRUE)
	{
		m_ctrlEditValue1.GetWindowText(strYValue);
		pCoordinate = m_pElementPointList->GetAt(0);

// 2008.10.15 by LYH >> 8,32bit����		
		if(CDataConversion::GetDataToDec(strYValue, m_pDataEnvironment->GetStrVert(), strValue))
		{
			pCoordinate->SetY(atof(strValue));
			m_stAnalysisTool.dStartYValue = atof(strValue);
		}
		else
		{
			pCoordinate->SetY(atof(strYValue));
			m_stAnalysisTool.dStartYValue = atof(strYValue);
		}
// 2008.10.15 by LYH <<
	}
	
	if(m_ctrlEditValue2.IsWindowVisible() == TRUE)
	{
		m_ctrlEditValue2.GetWindowText(strYValue);
		pCoordinate = m_pElementPointList->GetAt(1);

// 2008.10.15 by LYH >> 8,32bit����
		if(CDataConversion::GetDataToDec(strYValue, m_pDataEnvironment->GetStrVert(), strValue))
		{
			pCoordinate->SetY(atof(strValue));
			m_stAnalysisTool.dEndYValue = atof(strValue);
		}
		else
		{
			pCoordinate->SetY(atof(strYValue));
			m_stAnalysisTool.dEndYValue = atof(strYValue);
		}
// 2008.10.15 by LYH <<
	}

	if(m_ctrlEditValue3.IsWindowVisible() == TRUE)
	{
		m_ctrlEditValue3.GetWindowText(strYValue);
		pCoordinate = m_pElementPointList->GetAt(2);

// 2008.10.15 by LYH >> 8,32bit����
		if(CDataConversion::GetDataToDec(strYValue, m_pDataEnvironment->GetStrVert(), strValue))
		{
			pCoordinate->SetY(atof(strValue));
			m_stAnalysisTool.dEndYValue = atof(strValue);
		}
		else
		{
			pCoordinate->SetY(atof(strYValue));
			m_stAnalysisTool.dThirdYValue = atof(strYValue);
		}
// 2008.10.15 by LYH <<
	}
}

void CDlgAnalysisToolSet::OnSelchangeWidth() 
{
	int nWidth = m_ctlWidth.GetCurSel();
	int nStyle = m_ctlStyle.GetCurSel();
	if(nStyle != 0 && nWidth > 0)
	{
		ML_SET_LANGUAGE_RES();

		CString strNotice;
		strNotice.LoadString(IDS_CHANGE_THICKNESS);
		AfxMessageBox(strNotice);
		m_ctlStyle.SetCurSel(0);
	}
}

void CDlgAnalysisToolSet::OnSelchangeStyle() 
{
	int nWidth = m_ctlWidth.GetCurSel();
	int nStyle = m_ctlStyle.GetCurSel();
	if(nStyle > 0 && nWidth != 0)
	{
		ML_SET_LANGUAGE_RES();

		CString strNotice;
		strNotice.LoadString(IDS_CHANGE_THICKNESS);
		AfxMessageBox(strNotice);
		m_ctlWidth.SetCurSel(0);
	}
}

void CDlgAnalysisToolSet::OnOkBt() 
{
	ApplyData();
	SaveData();
	CDialog::OnOK();
}

void CDlgAnalysisToolSet::OnCancelBt() 
{
	ApplyOrgData();
	CDialog::OnCancel();
}

void CDlgAnalysisToolSet::OnApplyBt() 
{
	ApplyData();
}


void CDlgAnalysisToolSet::OnRightText() 
{
	if ((CToolEnumData::ElementType)m_nElementType == CToolEnumData::Line)
		m_ctrlRateText.EnableWindow(m_ctrlRightText.GetCheck());
}

