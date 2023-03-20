// ToolBarManagerImplementation.cpp: implementation of the CToolBarManagerImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolBarManagerImplementation.h"

#include <fcntl.h>
#include <io.h>

#include "../Include_Chart/Dll_Load/PacketList.h"
#include "../Include_Chart/Dll_Load/MainBlock.h"
#include "../Include_Chart/Dll_Load/IndicatorList.h"
#include "../Include_Chart/Dll_Load/XScaleManager.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl
#include "Definition.h"
#include "ToolBarDlg.h"
#include "ToolOption.h"
#include "ObjTool.h"
#include "ButtonEx.h"
#include "ViewData.h"
#include "Conversion.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolBarManagerImplementation::CToolBarManagerImplementation( IChartCtrl *p_pIChartCtrl)
{
	m_pIChartCtrl = p_pIChartCtrl;
	m_pToolBarDlg = NULL;
	m_pToolOption = NULL;
	m_nMaxRange = 0;
	m_nDataStartIndex = 0;

	// (2006/3/4 - Seung-Won, Bae) Initialize with -1 for Empty Data.
	m_nDataEndIndex = -1;
}

CToolBarManagerImplementation::~CToolBarManagerImplementation()
{
	// (2006/2/20 - Seung-Won, Bae) Manage Module State for Destroy Window
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	if(m_pToolBarDlg != NULL)
	{
		delete m_pToolBarDlg;
		m_pToolBarDlg = NULL;
	}
	
	if(m_pToolOption != NULL)
	{
		delete m_pToolOption;
		m_pToolOption = NULL;
	}
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateToolBar
 - Created at  :  2004-10-28   17:15
 - Author      :  ������
 - Parameters  :  strToolBarOption - ���ٿ� ��ϵ� ��ư�� �������ڿ�
 -                strImageFile     - ��ư�̹������ϸ�
 - Return Value:  bool - true:����, false: ����
 - Description :  ���ٸ� �����Ѵ�. �ֿ켱������ ����Dialog�� ������Ų�� ���� ��ư���� ������Ų��.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::CreateToolBar(const CString& strToolBarOption, 
												  const CString& strImageFile)
{
	// (2006/2/17 - Seung-Won, Bae) Set Module State for Dialog Creation
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	if(m_pToolOption == NULL)
		m_pToolOption = new CToolOption(strToolBarOption);
	else
		m_pToolOption->Reset(strToolBarOption);

	int nScrollBarWidth = 0;	//��ũ�ѹ��� ����� ����Ѵ�.
	bool bRadarChart = false;	// ���̴���Ʈ(��ħ��) ��뿩��
	bool bStatusBar = false;	// ��ġ��ȸ����Ÿ����(StatusBar)��뿩��.
	
	m_rectToolBarDlg = CalculateToolBarRect(nScrollBarWidth, bRadarChart, bStatusBar);

	CreateToolBarDialog();	// ����Dialog ����
	CreateToolBarControl(strImageFile, nScrollBarWidth, bRadarChart, bStatusBar);	// ����Item����
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  CreateToolBarDialog
 - Created at  :  2004-10-21   10:16
 - Author      :  ������
 - Parameters  :  rectToolBarDlg - ���� ����Dialog�� Rect
 - Return Value:  bool - true����, false����
 - Description :  ���� Dialog�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::CreateToolBarDialog()
{
	ML_SET_DEFAULT_RES();

	// (2006/2/8 - Seung-Won, Bae) Do not use GetChartWindowPointer()'s result. That does not exist in CWnd Object Map of this Module.
	if(m_pToolBarDlg == NULL) m_pToolBarDlg = new CToolBarDlg( CWnd::FromHandle( m_pIChartCtrl ? m_pIChartCtrl->GetSafeHwnd() : NULL), this, m_pIChartCtrl);
	else m_pToolBarDlg->DestroyWindow();

	m_pToolBarDlg->SetControlData(m_pPacketList, m_pPacketListManager, m_pxScaleManager);

	// (2006/2/8 - Seung-Won, Bae) Do not use GetChartWindowPointer()'s result. That does not exist in CWnd Object Map of this Module.
	if(!m_pToolBarDlg->Create(IDD_TOOLBAR, CWnd::FromHandle( m_pIChartCtrl->GetChartWindowPointer()->GetSafeHwnd())))
		return false;

	// ���� �����ÿ��� MoveWindow�� ȣ���Ѵ�. --> ��ġ��ȸâ �������� Rect�� ����ϹǷ� ����!
	m_pToolBarDlg->MoveWindow(m_rectToolBarDlg);
	m_pToolBarDlg->ShowWindow(SW_SHOW);
	return true;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateToolBarControl
 - Created at  :  2004-10-25   14:22
 - Author      :  ������
 - Parameters  :  strImageFile - ��ư�̹��� ����
				  nScrollBarWidth - ��ũ�ѹ��� �ʺ�
				  bRadarChart - ���̴���Ʈ ��뿩��
				  bStatusBar - ��ġ��ȸ����Ÿ���� ��뿩��
 - Return Value:  bool - true ����,  false ����.
 - Description :  ����Dialog�� ��ϵ� ����ư�� �����Ͽ� �ø���.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::CreateToolBarControl(const CString& strImageFile, const int nScrollBarWidth, const bool bRadarChart, const bool bStatusBar)
{
	if(m_pToolBarDlg == NULL)
		return false;

	return m_pToolBarDlg->CreateCtrl(m_pToolOption, m_pIChartCtrl->GetRscFont(), strImageFile, nScrollBarWidth, bRadarChart, bStatusBar);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ToolBarPosChanged
 - Created at  :  2004-10-20   15:50
 - Author      :  ������
 - Parameters  :  rectToolBarPos - ���ο� ������ ����
 - Return Value:  None
 - Description :  ������ ��ġ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::ToolBarPosChanged(const CRect rectToolBarPos)
{
	if(m_pToolBarDlg == NULL)
		return;

	// 1. ���� �̵�. (���ٿ� ���ӵ� ��Ʈ�� ��� �̵��ȴ�.)
	m_rectToolBarDlg = rectToolBarPos;
	if(m_pToolBarDlg) m_pToolBarDlg->MoveToolBarDlg();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetToolBarRect
 - Created at  :  2004-10-21   09:25
 - Author      :  ������
 - Description :  ToolBar�� ũ�⸦ �����Ѵ�. ����ũ�⸦ �� ��ƮOCX�� ũ�⸦ �ٽ� ����ϱ� ����.
 -----------------------------------------------------------------------------------------*/
const CRect CToolBarManagerImplementation::GetToolBarRect()
{
	return m_rectToolBarDlg;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CalculateToolBarRect
 - Created at  :  2004-10-25   09:20
 - Author      :  ������
 - Parameters  :  [out]nScrollBarWidth - ��ũ�ѹ��� �ʺ�.
 -				  [out]bRadarChart - ���̴���Ʈ(��ħ��) ��뿩��
 -				  [out]bStatusBar - ��ġ��ȸ����Ÿ����(Status Bar) ��뿩��
 - Return Value:  CRect - ���̾�α��� Rect�� �����Ѵ�.
 - Description :  ���λ������ (OCX�� ����) ������������ ���λ������ ���⼭ ����� ��.
				  ��ũ�ѹ��� ũ�⵵ ���⼭ ����� ��.
				  (NOTE: ��ư���� ������ ���ν������ �������� �����ϴ°�? - �����غ���)
 -----------------------------------------------------------------------------------------*/
const CRect CToolBarManagerImplementation::CalculateToolBarRect(int& nScrollBarWidth, bool& bRadarChart, bool& bStatusBar)
{
	if(m_pToolOption == NULL)
		return CRect(0, 0, 0, 0);

	CTypedPtrList<CObList, CObjTool*>* pToolList = m_pToolOption->GetToolOptionList(CToolOptionInfo::INNER);
	if(pToolList == NULL || pToolList->GetCount() <= 0)
		return CRect(0, 0, 0, 0);
	
	
	// 1. ScrollBar���� ���: �Ϲݹ�ư�� ����(��ư�ʺ�)�� Slider Bar(��ư�ʺ�X3), ������(��ư�ʺ�X3), 
	//					�׷�����Ÿ�ϼ���(��ư�ʺ�X3)�� ������ �� �������� ScrollBar���̷� ����صд�.
	// 2. ���̴���Ʈ�������: ���̴� ��Ʈ�� ù��° �ɼǰ��� '��ħ��'�̸� m_nHeight����ŭ�� �߰��Ѵ�.
	//						(���� �Ʒ��ʿ� ���� ����.)
	// 3. ��ġ��ȸ����Ÿ���⿵�����: (��ư�� ���������� �ʴ´�) ù��° �ɼ��� 'Status Bar'�̸� 
	//						Height���� ���̿� �߰��Ѵ�.(���� ���ʿ� ����)
	

	int nTmpWidth = 0, nTmpHeight = 0;
	int nCount = pToolList->GetCount();
	// NOTE: ����(04/10/25) ������ ���� ���̰� ����. ���� ���캼��.
	if(nCount >= 1)
		nTmpHeight = TB_BUTTON_HEIGHT;

	POSITION pos = pToolList->GetHeadPosition();
	for(int i = 0; i < nCount; i++)
	{
		CObjTool* pObjTool = pToolList->GetNext(pos);
		if(pObjTool == NULL)
			continue;

		CString strOption1, strOption2;	// ����ư�� ù°/�ι�° �ɼ�
		switch(pObjTool->GetType()) 
		{
		case CToolOptionInfo::T_INFLATE:			// Ȯ��/��� (��ư �ΰ�)
			nTmpWidth += (TB_BUTTON_WIDTH * 2);
			break;
		case CToolOptionInfo::T_GRAPH_STYLE:		// �׷�����Ÿ�� (��ư�ʺ�X3)
		case CToolOptionInfo::T_GRAPH_THICKNESS:	// �� ���� (��ư�ʺ�X3)
		case CToolOptionInfo::T_INDICATOR_SERIES:	// ��ǥ������ȸ (��ư�ʺ�X3)
		case CToolOptionInfo::T_AUTOSCROLL:			// �ڵ���ũ�� (��ư ���� == ��ư�ʺ�X3)
			nTmpWidth += (TB_BUTTON_WIDTH * 3);
			break;
		case CToolOptionInfo::T_DISPLAY_SLIDERBAR:	// �����̴��� (�ʺ� == 79 Fixed)
			nTmpWidth += TB_BUTTON_SLIDERBAR;
			break;
		case CToolOptionInfo::T_RADAR:				// ���̴���Ʈ (��ư��:��ư�Ѱ�, ��ħ��:Height�߰�)
			pObjTool->GetOption(strOption1, strOption2);

			if(strOption1 == _MTEXT( C4_BUTTON_TYPE))
			{
				bRadarChart = false;
				nTmpWidth += TB_BUTTON_WIDTH;
			}
			else	// ��ħ��
			{
				bRadarChart = true;
				nTmpHeight += TB_RADAR_HEIGHT;
			}
			break;
		// (2005/9/28 - Seung-Won, Bae) Status Bar is not created in Dialog. It is created in the other Dialog.
		//								So. Do not increase Dialog's Height and Width.
		case CToolOptionInfo::T_INQUIRY_STATUS:
		case CToolOptionInfo::T_SCROLL:				// ��ũ�ѹٴ� �ϴ� ���⼭ ������� �ʴ´�.
			break;
		default:									// �׿��� �͵��� ��ư�ʺ� �߰�.
			nTmpWidth += TB_BUTTON_WIDTH;
			break;
		}//end of switch			
	}//end of for(int...
	
	// ScrollBar�� �ʺ� ����Ѵ�.
	CRect rectToolBar;
	if( m_pIChartCtrl) rectToolBar = m_pIChartCtrl->GetToolBarRegion();
	nScrollBarWidth = rectToolBar.Width() - nTmpWidth;

	return CRect(rectToolBar.left, rectToolBar.bottom - nTmpHeight, rectToolBar.right, rectToolBar.bottom);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetSliderMin
 - Created at  :  2004-10-29   15:26
 - Author      :  ������
 - Description :  Slider�� �ּҰ��� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetSliderMin(const int nSliderMin)
{
	if(m_pToolBarDlg == NULL)
		return;

	m_pToolBarDlg->SetSliderMin(nSliderMin);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphType
 - Created at  :  2004-11-01   16:37
 - Author      :  ������
 - Description :  �׷���Ÿ�Կ� ���� �׷�����Ÿ���� �޺��� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex)
{
	if(m_pToolBarDlg == NULL || m_pToolOption == NULL)
		return;
	
	if(!m_pToolOption->GetTool(CToolOptionInfo::T_GRAPH_STYLE))
		return;
	
	m_pToolBarDlg->SetGraphType(graphType, nIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetLineThickness
 - Created at  :  2004-11-03   11:26
 - Author      :  ������
 - Description :  OCX���� ������ ���εβ��������� ���� �޺��� �ش� ��Ÿ�� ���������� �ٲ۴�.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetLineThickness(int nThickness)
{
	if(m_pToolBarDlg == NULL || m_pToolOption == NULL)
		return;
	
	if(!m_pToolOption->GetTool(CToolOptionInfo::T_GRAPH_THICKNESS))
		return;
	
	m_pToolBarDlg->SetLineThickness(nThickness);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphColor
 - Created at  :  2004-11-29   17:04
 - Author      :  ������
 - Description :  OCX���� ������ �׷����� ������ '������'��ư�� �Ѱ��ش�.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetGraphColor(COLORREF clrGraphColor)
{
	if(m_pToolBarDlg == NULL || m_pToolOption == NULL)
		return;
	
	// ������ ��ư�� ������ Į���� ������ �ʿ� ����.
	if(!m_pToolOption->GetTool(CToolOptionInfo::T_GRAPH_COLOR))
		return;

	m_pToolBarDlg->SetGraphColor(clrGraphColor);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetOneChart
 - Created at  :  2004-11-05   11:08
 - Author      :  ������
 - Description :  ������ǥ��Ʈ������ ���ٿ� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetOneChart( bool bOneChart, bool p_bIsNoZoomChart)
{
	if(m_pToolBarDlg)
		m_pToolBarDlg->SetOneChart(bOneChart, p_bIsNoZoomChart);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetControlData
 - Created at  :  2004-11-01   09:51
 - Author      :  ������
 - Description :  ����ŸŬ������ �����Ѵ�. 
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList)
{
	m_pPacketList = pPacketList;
	m_pPacketListManager = pPacketListManager;
	m_pxScaleManager = pxScaleManager;
	m_pMainBlock = pMainBlock;
	m_pIndicatorList = pIndicatorList;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetStartEndIndex
 - Created at  :  2004-11-01   10:54
 - Author      :  ������
 - Description :  �����ε����� ���ε����� �����´�.
 - UpDate	   :  ��ũ�ѹٰ� ���� ��쿡 ����Ͽ� ��Ŭ������ ����������� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::GetStartEndIndex(int& nStartIndex, int& nEndIndex)
{
	nStartIndex = m_nDataStartIndex;
	nEndIndex = m_nDataEndIndex;
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetMaxRange
 - Created at  :  2004-11-17   14:49
 - Author      :  ������
 - Description :  MaxRange���� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
int CToolBarManagerImplementation::GetMaxRange() const
{
	if(m_pToolBarDlg && m_pToolBarDlg->IsScrollBar())
		return m_pToolBarDlg->GetMaxRange();
	else
		return m_nMaxRange;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  IsToolScrollBar
 - Created at  :  2005-02-28   10:31
 - Author      :  ������
 - Description :  ��ũ�ѹ� ��������� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::IsToolScrollBar()
{
	if(m_pToolBarDlg)
		return m_pToolBarDlg->IsScrollBar();
	return false;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetToolBarHwnd
 - Created at  :  2005-09-10   
 - Author      :  ������
 - Description :  ����Dialog�� ������ �ڵ��� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
HWND CToolBarManagerImplementation::GetToolBarHwnd()
{
	if(m_pToolBarDlg)
		return m_pToolBarDlg->GetSafeHwnd();
	else
		return NULL;
}

/*-------------------------------------------------------------------------------
 - Function    :  SetStartEndIndexToOcx
 - Created at  :  2005-04-19   08:50
 - Author      :  ������
 - Description :  Start�� End Index�� OCX�� �����Ѵ�.
 -------------------------------------------------------------------------------*/
// (2007/5/23 - Seung-Won, Bae) Case Analysis.
//	It is called from ...
/*
	S (Scroll to)
		CToolBarDlg::OnHScroll() ~ ScrollBar

		CToolOptionInfo::T_INFLATE (Zoom In/Out)				~ CToolBarDlg::SetToolFunction()

		CToolBarDlg::OnHScroll() ~ Zoom SlideBar (Zoom In/Out)

		CRadar::OnLButtonUp()

//			CSubGraph::SendMessage_SubGraph()					: from ChartObject DLL, In GlanceBalance Chart's PreSpan SubGraph's Adding or Removing.
//			~ChartCtrl::OnGraphAdded()							: **************************************************** GlanceBlance (After Calculation)
//			~ChartCtrl::OnGraphDeleted()						: **************************************************** GlanceBlance (After Calculation)
//			~ChartCtrl::OnGraphDeleted(RQ)						: **************************************************** GlanceBlance (After Calculation)
//			CGraphImp::~CGraphImp()								: On Removed Some RQ.
*/
void CToolBarManagerImplementation::SetStartEndIndexToOcx(const int nStart, const int nEnd)
{
	m_nDataStartIndex = nStart;
	m_nDataEndIndex = nEnd;

	if(m_pIChartCtrl)
		m_pIChartCtrl->SetStartEndIndex(nStart, nEnd);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetPageCountNextStepToOcx
 - Created at  :  
 - Author      :  yiilyoul@magicn.com
 - Description :
 -----------------------------------------------------------------------------------------*/
int CToolBarManagerImplementation::GetPageCountNextStepToOcx(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, int nMin, int nMax)
{
	if(m_pIChartCtrl)
	{
		return m_pIChartCtrl->GetPageCountNextStep(nSliderPos_Prev, nSliderPos_New, nSBCode, nMin, nMax);
	} else return nSliderPos_New;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetReverse
 - Created at  :  
 - Author      :  yiilyoul@magicn.com
 - Description :
 -----------------------------------------------------------------------------------------*/
BOOL CToolBarManagerImplementation::GetReverseToOcx()
{
	if(m_pxScaleManager)
	{
		return m_pxScaleManager->GetReverse();
	}

	return FALSE;
}

/*-------------------------------------------------------------------------------
 - Function    :  ReleaseOtherButtonUI
 - Created at  :  2005-03-02   14:46
 - Author      :  ������   
 - Description :  ����ư�� ������ �ִٸ�  ������Ų��.
 -------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	if(m_pToolBarDlg)
		m_pToolBarDlg->ReleaseOtherButtonUI(enumToolOption);
}



/*-------------------------------------------------------------------------------
 - Function    :  UseToolOption
 - Created at  :  2005-03-03   09:23
 - Author      :  ������   
 - Description :  ���ٿ� �ش� ���������� ����ϴ��� Ȯ���Ѵ�.(�Լ������� �����߰���)
 -------------------------------------------------------------------------------*/
BOOL CToolBarManagerImplementation::UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	if(!m_pToolOption)
		return FALSE;
	return m_pToolOption->UseTool(CToolOptionInfo::INNER, enumToolOption);
}


/*-------------------------------------------------------------------------------
 - Function    :  GetToolOption
 - Created at  :  2005-03-03   09:46
 - Author      :  ������   
 - Description :  ���ٿ� �ش� ���������� �ִ��� Ȯ���Ѵ�.(�Լ������� �����߰���)
 -------------------------------------------------------------------------------*/
BOOL CToolBarManagerImplementation::GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	if(!m_pToolOption)
		return FALSE;

	CObjTool* pObjTool = m_pToolOption->GetTool(CToolOptionInfo::INNER, enumToolOption);
	if(pObjTool)
		return TRUE;
	else
		return FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/4
// Modifier		: 
// Comments		: Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG
//-----------------------------------------------------------------------------
void CToolBarManagerImplementation::OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption)
{
	// (2006/3/4 - Seung-Won, Bae) Push or Pop the Button of Numerical Inquiry Dialog or Drag Cross Line
	if(m_pToolBarDlg)
	{
		CButtonEx* pButton = m_pToolBarDlg->GetButtonEx( CToolOptionInfo::T_NIDLG_DRAG);
		if( pButton) pButton->SetState( ( p_lNumericalInquiryOption & NUMERICALINQUIRYDLG) ? true : false);

		pButton = m_pToolBarDlg->GetButtonEx( CToolOptionInfo::T_CROSSHAIRS_DRAG);
		if( pButton) pButton->SetState( ( p_lNumericalInquiryOption & CROSSLINEVIEW) ? true : false);
	}
}


/*-------------------------------------------------------------------------------
 - Function    :  GetRealPacketNames
 - Created at  :  2005-03-24   14:08
 - Author      :  ������
 - Parameters  :  strTitle       - "����"�̰�..
 -                strPacketNames - "�ð�,��,����,����"�� ���...
 - Return Value:  CString - ���Ӱ� �ٲ� ��Ŷ��.
 - Description :  1ƽ�ϰ��� �˻��Ͽ� ���ο� ��Ŷ���� �������ش�.
 -------------------------------------------------------------------------------*/
CString CToolBarManagerImplementation::GetRealPacketNames(const CString& strTitle, const CString& strPacketNames) const
{
	CString strRealPacketNames = strPacketNames;
	if(strRealPacketNames.Right(1) != ",")
		strRealPacketNames += ",";

	// title = "����", packetName = "�ð�,��,����,����", "1ƽ"�� ��� 
	// -> packetName : "����"
	if(strTitle == _MTEXT( C3_PRICE) && strPacketNames.Find( _MTEXT( C0_CLOSE)) > 0)
	{
		if(m_pPacketList->GetDateUnitType() == CScaleBaseData::HORZ_TICK && m_pPacketList->GetTimeInterval() == 1)
		{
			strRealPacketNames = _MTEXT( C0_CLOSE);
			strRealPacketNames += ",";
		}
	}

	return strRealPacketNames;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  FilterSimpleInquiryData
 - Created at  :  2005-04-06   18:47
 - Author      :  ������
 - Parameters  :  strPacketNames - ��Ŷ��.
 - Return Value:  bool - true: ��ȸ ����Ÿ�� �߰�, false: ��ȸ ����Ÿ���� ����
 - Description :  Single Line�̳� Simple Window������ ��ġ��ȸâ���� �ʿ���ϴ� ����Ÿ���� �����س���
				  ���� bool������ �������ش�.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::FilterSimpleInquiryData(const CString& strPacketNames)
{
	// Simple������ ��ȸ����Ÿ�� �� �����ϴ� ��Ŷ��...
	if(strPacketNames.Find( _MTEXT( C0_OPEN)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C0_HIGH)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C0_LOW)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C0_CLOSE)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C0_VOLUME)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_1)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_2)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_3)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_4)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_5)) != -1)
		return true;
	if(strPacketNames.Find( _MTEXT( C3_PRICE_MA_6)) != -1)
		return true;

	return false;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  IsSpecialIndicator
 - Created at  :  2005-05-27   16:49
 - Author      :  ������
 - Return Value:  bool - TRUE: Ư����ǥ, FALSE: �Ϲ���ǥ
 - Description :  Ư����ǥ�ϰ�� �׻� ��ü��������̾���Ѵ�. �׷��� 'Ȯ��'��ư�� ������
				  ��ü������°� �����Ǵµ� Ư����ǥ�� ���� ���� ���� ��ü������¸� ����
				  ��Ű�� �ʵ����ϱ� ���� ���� ���� �ִ� ��ǥ�� Ư����ǥ���� ToolBarDlg���� �˷�����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CToolBarManagerImplementation::IsSpecialIndicator()
{
	return m_pIChartCtrl->IsRunningOneChart() ? true : false;
}

// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
bool CToolBarManagerImplementation::IsRunningNoZoomChart()
{
	return m_pIChartCtrl->IsRunningNoZoomChart() ? true : false;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ReDraw
 - Created at  :  2005-08-16   19:01
 - Author      :  ������
 - Description :  ���ٹ� ��ġ��ȸâ�� �׸���. OCX�� OnDraw���� ���������� ȣ��.
 -----------------------------------------------------------------------------------------*/
void CToolBarManagerImplementation::ReDraw()
{
	if(m_pToolBarDlg)
		m_pToolBarDlg->ReDraw();
}

// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
BOOL CToolBarManagerImplementation::OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)
{
	if( !m_pToolBarDlg) return FALSE;

	CButtonEx* pButton = m_pToolBarDlg->GetButtonEx( p_eToolID);
	if( pButton && pButton->GetSafeHwnd()) pButton->SetState( p_bTurnOn ? true : false);
	return TRUE;
}

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
void CToolBarManagerImplementation::UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView)
{
	if( !m_pToolBarDlg) return;
	m_nDataStartIndex = p_nStartPos;
	m_nDataEndIndex = p_nStartPos + p_nViewCount - 1;
	m_pToolBarDlg->UpdateZoomAndScrollInfo( p_nFullRange, p_nStartPos, p_nViewCount, p_bWholeView);
}
