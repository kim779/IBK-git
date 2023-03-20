// ToolBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarDlg.h"

#include "../Include_Chart/Dll_Load/PacketList.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl
#include "../Include_Chart/IChartCtrl.h"					// for IChartCtrl

#include "Definition.h"
#include "CtrlFactory.h"
#include "ToolBarManagerImplementation.h"
#include "ToolOption.h"
#include "ObjTool.h"
#include "ButtonProperty.h"
// ���� ��Ͽ� �ʿ��� Ctrl �����.
#include "ButtonEx.h"
#include "RadarControl.h"
#include "GraphStyleCombo.h"
#include "GraphLineThicknessCombo.h"
#include "ScrollBarEx.h"
#include "SliderCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBitmap CToolBarDlg::m_bitmapButton;

extern bool IsToggleButton(CToolOptionInfo::TOOLOPTION enumToolOption);
/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg dialog


CToolBarDlg::CToolBarDlg(CWnd* pParent, CToolBarManagerImplementation* pToolBarImpl, IChartCtrl *p_pIChartCtrl)
	: CDialog(CToolBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pScrollBar = NULL;
	m_pSliderCtrl = NULL;
	m_pGraphStyleCombo = NULL;
	m_pGraphLineCombo = NULL;
	m_pRadarControl = NULL;
	m_pButtonProperty = NULL;
	m_clrSelection = RGB(0,0,0);
	m_nSliderMin = 5;
	m_nSliderMax = 5;
	m_nRemainCount = 0;
	m_nGlanceBalanceMargin = 0;
	m_nScrollWidth = 0;
	m_bCheckGlanceBalance = false;
	m_bRadarChart = false;
	m_bStatusBar = false;
	m_bOneChart = false;
	m_bIsNoZoomChart = false;						// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	m_bUseRadarChart = FALSE;
	m_pToolBarManagerImple = pToolBarImpl;
	m_nScrollingDataCount = 1;

	// (2006/11/18 - Seung-Won, Bae) Support Chart OCX Ctrl Interface
	m_pIChartCtrl = p_pIChartCtrl;

	// (2007/2/8 - Seung-Won, Bae) Disable WholeView Btn in Special Chart.
	m_pWholeViewBtn = NULL;
	// (2007/2/8 - Seung-Won, Bae) Disable Zoom Btn in Special Chart.
	m_pZoomInBtn = NULL;
	m_pZoomOutBtn = NULL;
}

void CToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg message handlers

/*-----------------------------------------------------------------------------------------
 - Function    :  OnCancel, OnOK
 - Created at  :  2004-10-18   15:02
 - Author      :  ������
 - Description :  EnterŰ�� EscŰ�� ���� Dlg Close�� ����.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::OnCancel() 
{
//	CDialog::OnCancel();
}
void CToolBarDlg::OnOK() 
{
//	CDialog::OnOK();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ~CToolBarDlg
 - Created at  :  2004-11-25   09:38
 - Author      :  ������
 - Description :  �Ҹ���
 -----------------------------------------------------------------------------------------*/
CToolBarDlg::~CToolBarDlg()
{
	DeleteCtrl();
	
	if(m_pButtonProperty)
	{
		delete m_pButtonProperty;
		m_pButtonProperty = NULL;
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  DeleteCtrl
 - Created at  :  2004-11-30   15:35
 - Author      :  ������
 - Description :  ����Ʈ�� ����� �ִ� ��Ʈ�ѵ��� �����Ѵ�. �ܼ� ����(Remove)�� �ƴ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::DeleteCtrl()
{
	POSITION pos = m_listToolBarCtrl.GetHeadPosition();
	
	CWnd* pWnd = NULL;
	while(pos)
	{
		pWnd = m_listToolBarCtrl.GetNext(pos);
		if(pWnd)
		{
			if( pWnd->GetSafeHwnd()) pWnd->DestroyWindow();
			delete pWnd;
		}
		pWnd = NULL;
	}
	m_listToolBarCtrl.RemoveAll();
	m_listToolButton.RemoveAll();	// ��ư ������Ʈ���� m_listToolBarCtrl���� �̹� delete�ȴ�.

	if(m_pRadarControl)
	{
		delete m_pRadarControl;
		m_pRadarControl = NULL;
	}

	// (2008/2/5 - Seung-Won, Bae) Reset Pointers
	m_pSliderCtrl = NULL;
	m_pScrollBar = NULL;
	m_pGraphStyleCombo = NULL;
	m_pGraphLineCombo = NULL;
	m_pWholeViewBtn = NULL;
	m_pZoomInBtn = NULL;
	m_pZoomOutBtn = NULL;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateCtrl
 - Created at  :  2004-10-26   13:21
 - Author      :  ������
 - Parameters  :  pToolOption  - Tool������ ��������Ʈ (WILL: CTypedPtrList<CObList, CObjTool*>*�� ���ڷ� ������)
 -				  pFont - ���ҽ� ��Ʈ
 -				  strImageFile - ��ư �̹��� ���ϸ�.
 -                nScrollWidth - ��ũ�ѹ��� ����.
 -                bRadarChart  - RadarChart(��ħ��) ��뿩��
 -                bStatusBar   - ��ġ��ȸ����Ÿ����(Status Bar) ��뿩��
 - Return Value:  bool - true:����, false:����.
 - Description :  ����Dialog�� ����ư�� �����Ѵ�. RadarChart�� Status Bar�� ���� ������.(�������ٿ�
				  ��ϵǾ� ���� ���.)
 - Update	   :  ����(05/04/14) �������ӿ��� ¯���� ���Ծ���. ��~ �۷���� ���λ�. �ڵ���ũ�ѹ�ư�� �߰��Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::CreateCtrl(CToolOption* pToolOption, const CFont* pFont, const CString& strImageFile, const int nScrollWidth, const bool bRadarChart, const bool bStatusBar)
{
	DeleteCtrl();	// ����DLG�� �������� ������ϴ� ���μ����� ����.

	m_bStatusBar = bStatusBar;
	m_bRadarChart = bRadarChart;
	m_nScrollWidth = nScrollWidth;

	bool bBitmap = false;
	if(!m_bitmapButton.GetSafeHandle())
		bBitmap = MakeBitMap(strImageFile);	// ��Ʈ�ʸ����
	else
		bBitmap = true;					//��Ʈ���̹����� �̹� �ִ�.

	CRect rectToolBarDlg;
	GetClientRect(rectToolBarDlg);

	// ��Ʈ�ѻ��� ���丮
	CCtrlFactory factory( m_pIChartCtrl, this, rectToolBarDlg, bStatusBar, bRadarChart, nScrollWidth, bBitmap);
	factory.Initialize();
	
	CTypedPtrList<CObList, CObjTool*>* pToolList = pToolOption->GetToolOptionList(CToolOptionInfo::INNER);
	POSITION pos = pToolList->GetHeadPosition();
	while(pos)
	{
		CObjTool* pObjTool = pToolList->GetNext(pos);
		switch(pObjTool->GetType())
		{
		case CToolOptionInfo::T_DISPLAY_SLIDERBAR:		// ------------�����̴���
			{
				m_pSliderCtrl = factory.CreateSliderCtrl();
				m_listToolBarCtrl.AddTail(m_pSliderCtrl);
				break;
			}
		case CToolOptionInfo::T_INQUIRY_STATUS:			// ------------��ġ��ȸ����Ÿ����
			{
				CString strOpt1, strOpt2;
				pObjTool->GetOption(strOpt1, strOpt2);
				if(strOpt1 != _T("Status Bar"))	// Status Bar �ɼ��� �ƴҰ�쿡�� ��ư ����.
				{
					CButtonEx* pButton = factory.CreateButton(pObjTool);
					m_listToolBarCtrl.AddTail(pButton);
					m_listToolButton.AddTail(pButton);
				}

				// (2006/6/27 - Seung-Won, Bae) Not Supported Type 2
				// else m_pToolBarManagerImple->CreateNumericalInquiryDlg(2);

				break;
			}
		case CToolOptionInfo::T_SCROLL:					// ------------��ũ��
			{
				m_pScrollBar = factory.CreateScrollBar();
				m_listToolBarCtrl.AddTail(m_pScrollBar);
				break;
			}
		case CToolOptionInfo::T_GRAPH_STYLE:			// ------------�׷��� ����
			{
				m_pGraphStyleCombo = factory.CreateGraphStyleCombo(pObjTool->GetName());
				m_listToolBarCtrl.AddTail(m_pGraphStyleCombo);
				break;
			}
		case CToolOptionInfo::T_GRAPH_THICKNESS:		// ------------������
			{
				m_pGraphLineCombo = factory.CreatreGraphLineThicknessCombo(pObjTool->GetName());
				m_listToolBarCtrl.AddTail(m_pGraphLineCombo);
				break;
			}
		case CToolOptionInfo::T_RADAR:					// ------------���̴���Ʈ
			{
				if(!m_bRadarChart)
				{	// ��ư�� ����
					CButtonEx* pButton = factory.CreateButton(pObjTool);
					m_listToolBarCtrl.AddTail(pButton);
					m_listToolButton.AddTail(pButton);				
				}
				else
				{	// ���̴���Ʈ�� ����
					// �̳��� Position Changed�Ҷ� ���� ����ϹǷ� List�߰����� �ʴ´�.
					m_pRadarControl = factory.CreateRadarControl();				
					m_pRadarControl->SetIChartCtrl( m_pIChartCtrl);
				}
				break;
			}
		case CToolOptionInfo::T_INFLATE:				// ------------Ȯ��/���
			{
				// (2007/1/5 - Seung-Won, Bae) Change Position for samsung.
				// ��� ��ư ����
				CButtonEx* pButtonZoomOut = factory.CreateButton(pObjTool, _MTEXT( C4_ZOOM_OUT));
				m_listToolBarCtrl.AddTail(pButtonZoomOut);
				m_listToolButton.AddTail(pButtonZoomOut);		
				// Ȯ�� ��ư ����
				CButtonEx* pButtonZoomIn = factory.CreateButton(pObjTool, _MTEXT( C4_ZOOM_IN));
				m_listToolBarCtrl.AddTail(pButtonZoomIn);
				m_listToolButton.AddTail(pButtonZoomIn);
				// (2007/2/8 - Seung-Won, Bae) Disable Zoom Btn in Special Chart.
				m_pZoomInBtn = pButtonZoomIn;
				m_pZoomOutBtn = pButtonZoomOut;
				break;
			}
		case CToolOptionInfo::T_AUTOSCROLL:				// ------------�ڵ���ũ��.
			{
				// ���ʽ�ũ�� ��ư ����
				CButtonEx* pButtonLeftScroll = factory.CreateButton(pObjTool, _MTEXT( C4_LEFT_AUTO_SCROLL));
				m_listToolBarCtrl.AddTail(pButtonLeftScroll);
				m_listToolButton.AddTail(pButtonLeftScroll);
				// �ڵ���ũ�� ���� ��ư ����
				CButtonEx* pButtonStop = factory.CreateButton(pObjTool, _MTEXT( C4_STOP_AUTO_SCROLL));
				m_listToolBarCtrl.AddTail(pButtonStop);
				m_listToolButton.AddTail(pButtonStop);
				//�����ʽ�ũ�� ��ư ����
				CButtonEx* pButtonRightScroll = factory.CreateButton(pObjTool, _MTEXT( C4_RIGHT_AUTO_SCROLL));
				m_listToolBarCtrl.AddTail(pButtonRightScroll);
				m_listToolButton.AddTail(pButtonRightScroll);
				break;
			}
		default:										// ------------���� ����� case�̿��� �͵��� ��� ��ư�̴�. ��ư�ƴ� �ѵ��� �����־�! ^^;
			{
				CButtonEx* pButton = factory.CreateButton(pObjTool);
				m_listToolBarCtrl.AddTail(pButton);
				m_listToolButton.AddTail(pButton);
				// (2007/2/8 - Seung-Won, Bae) Disable WholeView Btn in Special Chart.
				if( CToolOptionInfo::T_WHOLEVIEW == pObjTool->GetType()) m_pWholeViewBtn = pButton;
				break;
			}
		}
	}

	// ��ϵ� ��ư�� ������ ButtonRelationŬ����������Ʈ ����.
	if(m_listToolButton.GetCount() > 0)
	{
		if(m_pButtonProperty)
		{
			delete m_pButtonProperty;
			m_pButtonProperty = NULL;
		}
		m_pButtonProperty = new CButtonProperty(this, &m_listToolButton);
	}

	return true;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  MakeBitMap
 - Created at  :  2004-10-21   15:22
 - Author      :  ������
 - Parameters  :  strFileName - ��ư�̹��� ����.
 - Return Value:  bool - true����, false����
 - Description :  ��ư�̹��� ���Ϸ� ���� Bitmap�� �����.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::MakeBitMap(const CString& strFileName)
{
	if(strFileName.IsEmpty())
		return false;

	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(NULL, strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);

	return (m_bitmapButton.Attach(hBitmap) ? true : false);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  MoveToolBarDlg
 - Created at  :  2004-10-29   13:25
 - Author      :  ������
 - Parameters  :  rectNew - ���ο� ���ٿ���
 - Return Value:  None
 - Description :  ��Ʈâ�� Resizing�̳� �̵��� ȣ��.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::MoveToolBarDlg( void)
{
	// ������ ���α����� ���̸� ���Ѵ�.
	CRect rectOld;
	GetClientRect(rectOld);
	CRect rectNew = m_pToolBarManagerImple->GetToolBarRect();
	int nSubWidth = rectNew.Width() - rectOld.Width();
	// ���ο� Dialog�� ���̸� �����Ѵ�.
	rectNew.top = rectNew.bottom - rectOld.Height();

	// ����ư�� �������� ToolBar Dlg������ ����!
	if(m_listToolBarCtrl.GetCount() == 0)
		rectNew.top = rectNew.bottom;

	// 1. ToolBar Dialog�� �̵���Ų��.
	MoveWindow( rectNew);
	m_pToolBarManagerImple->SetToolBarRect( rectNew);

	// 2. ToolBar Dialog�� ��ϵ� Ctrl���� �̵���Ų��.
	CRect rectCtrl(0, 0, 0, TB_BUTTON_HEIGHT);

	// ��ġ��ȸ����Ÿ����(StatusBar)�� ������ ���� �߰�
	if(m_bStatusBar) 
	{
		rectCtrl.top += TB_BUTTON_HEIGHT;
		rectCtrl.bottom += TB_BUTTON_HEIGHT;
	}
	
	POSITION pos = m_listToolBarCtrl.GetHeadPosition();
	while(pos)
	{
		CWnd* pWnd = m_listToolBarCtrl.GetNext(pos);

		if(pWnd == NULL)
			continue;
		switch(pWnd->GetDlgCtrlID())
		{
		case IDC_SCROLL:
			rectCtrl.right = rectCtrl.left + m_nScrollWidth + nSubWidth;
			m_nScrollWidth = rectCtrl.Width();	// ��ũ�ѹ� ���� �� ����.
			((CScrollBarEx*)pWnd)->SetScrollBarWidth(m_nScrollWidth);
			pWnd->MoveWindow(rectCtrl);
			break;
		case IDC_SLIDER:
			rectCtrl.right = rectCtrl.left + TB_BUTTON_SLIDERBAR;
			pWnd->MoveWindow(rectCtrl);
			break;
		case IDC_GRAPHSTYLE_COMBO:		// �׷�����Ÿ��
		case IDC_LINETHICKNESS_COMBO:	// ������
			rectCtrl.bottom += 100;
			rectCtrl.right = rectCtrl.left + (TB_BUTTON_WIDTH * 3);
			pWnd->MoveWindow(rectCtrl);
			rectCtrl.bottom -= 100;
			break;
		case IDC_RADAR:
			if(!m_bRadarChart)	// ��ư��.
				rectCtrl.right = rectCtrl.left + TB_BUTTON_WIDTH;
			pWnd->MoveWindow(rectCtrl);
			break;
		default:
			rectCtrl.right = rectCtrl.left + TB_BUTTON_WIDTH;
			pWnd->MoveWindow(rectCtrl);
			break;
		}//end of switch

		rectCtrl.left = rectCtrl.right;
	}//end of while

	if(m_pRadarControl)
		m_pRadarControl->OnToolBarPositionChanged(rectNew);
	
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetScrollData_TR
 - Created at  :  2004-10-27   13:52
 - Author      :  ������
 - Parameters  :  nTotalDataCount      - �ѵ���Ÿ ����
 -                nPageDataCount       - ���������� ������ ����Ÿ ����
 -                bFullPage            - ��ü�����ΰ�?
 -                dataType             - 
 -                nGlanceBalanceMargin - �ϸ����ǥ������ �߰��Ǵ� ����Ÿ����(SliderCtrl�� Max Range��)
 - Return Value:  None
 - Description :  �ʱ�TR�� �޾����� ScrollBar ����. SliderCtrl�� Min/Max���� Position ����.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetScrollData_TR(const int nTotalDataCount, 
								   const int nPageDataCount, 
								   const bool bFullPage, 
								   CPacketDataInfo::DATAINSERTTYPE dataType, 
								   const int nGlanceBalanceMargin)
{
	// 0. Process Scale with None-Data TR as with nPageDataCount Data TR
	if(nTotalDataCount == 0)
	{
		//sy 2003.7.15.
		//��ü data���Ⱑ �ƴ� ��� ��ȭ�鿡 ���̴� data ������ �����Ѵ�.
		//-> TR�� ���� ó�� �ǽð��� ������ ��쿡 ��ȭ�鿡 ������ �Ǵ� ���� ����
		if(!bFullPage && nPageDataCount > 0)
		{
			SetScrollData_TR(nPageDataCount, nPageDataCount, bFullPage, dataType, nGlanceBalanceMargin);
			return;
		}
	}

	// 1. Check Minimum Limitation of One-Page-Data-Count.
	// (2003.11.12, ��¿�) Map���� ������ '�� ȭ�� �ּ� Data ��'�� SlideBar�� �����Ѵ�.
	int nPageDataCountTmp = nPageDataCount;
	if( nPageDataCountTmp < m_nSliderMin) 
		nPageDataCountTmp = m_nSliderMin;

	// 2. with GlanceBalance
	// NOTE: ����(04/11/17) MaxRange ���ϴ� �κ�. �̺κ� �̷��� �������� �ʿ䰡 �������ϱ⵵ �ϴ�.
	//		���߿� ������ ����!
	if(nGlanceBalanceMargin != 0)
	{// �ϸ����ǥ üũ��
		if(nPageDataCountTmp > nTotalDataCount)
			m_nSliderMax = nPageDataCountTmp;
		else
			m_nSliderMax = nTotalDataCount;

		m_nGlanceBalanceMargin = nGlanceBalanceMargin;
	}
	else if(m_nGlanceBalanceMargin != 0 && nGlanceBalanceMargin == 0)
	{// �ϸ����ǥ ��üũ��
		if(m_nRemainCount <= 0)
		{
			m_nSliderMax = nTotalDataCount;

			if(nPageDataCountTmp > nTotalDataCount)
				nPageDataCountTmp = nTotalDataCount;
		}
		else if(m_nRemainCount > m_nGlanceBalanceMargin)
			m_nSliderMax = nPageDataCountTmp;
		else if( m_nRemainCount > 0 && m_nRemainCount < m_nGlanceBalanceMargin)
		{
			m_nSliderMax = nTotalDataCount + m_nRemainCount;
			nPageDataCountTmp = m_nSliderMax;
		}

		if(m_bCheckGlanceBalance)
		{
			m_nGlanceBalanceMargin = 0;
			m_bCheckGlanceBalance = FALSE;
		}
		else
			m_bCheckGlanceBalance = TRUE;			
	}
	else if(nGlanceBalanceMargin == 0)
	{
		// MODIFY: ����(04/07/28) '��ü����'���ý� SliderMaxRange����
		if(bFullPage)
			m_nSliderMax = nTotalDataCount;
		else if(nPageDataCountTmp > nTotalDataCount)
			m_nSliderMax = nPageDataCountTmp;
		else
			m_nSliderMax = nTotalDataCount;
		// MODIFY: 04/07/28

		// MODIFY: ����(04/07/21) ������ ������ ����. ������ �ϸ����ǥ�����̾�!!! T_T
		m_nRemainCount = m_nSliderMax - nTotalDataCount;
	}//else if(nGla...

	// (2007/2/8 - Seung-Won, Bae) Disable WholeView Btn in Special Chart.
	if( m_pWholeViewBtn) m_pWholeViewBtn->EnableWindow( !m_bIsNoZoomChart);
	// (2007/2/8 - Seung-Won, Bae) Disable Zoom Btn in Special Chart.
	if( m_pZoomInBtn) m_pZoomInBtn->EnableWindow( !m_bIsNoZoomChart);
	if( m_pZoomOutBtn) m_pZoomOutBtn->EnableWindow( !m_bIsNoZoomChart);

	// 3. with SlideBar
	if(m_pSliderCtrl)
	{
		m_pSliderCtrl->SetRange(m_nSliderMin, m_nSliderMax, true);

		m_pSliderCtrl->EnableWindow( !m_bIsNoZoomChart);

		if( m_bOneChart) m_pSliderCtrl->SetPos( nTotalDataCount);
		else
		{
			if( bFullPage)	m_pSliderCtrl->SetPos( nTotalDataCount);
			else			m_pSliderCtrl->SetPos( nPageDataCountTmp);
		}

	}//if(m_pSliderCtrl...

	// 4. with ScrollBar
	if(!m_pScrollBar) return;
	m_pScrollBar->SetScrollData_TR(nTotalDataCount, nPageDataCountTmp, bFullPage, dataType, m_nGlanceBalanceMargin);
	
	// (2007/2/8 - Seung-Won, Bae) Disable ScrollBar in Special Chart.
	m_pScrollBar->EnableWindow( !m_bIsNoZoomChart);

	// 5. with Radar Control
	if(m_pRadarControl)
	{
		// ����(04/09/01) �ʱ� ��Ʈ������ ������ �Ʒ� ���̴���Ʈ�� startIndex�� ����� �ֱ� ���ؼ�...
		int nStartIndex = 0;
		int nEndIndex = 0;
		m_pScrollBar->GetDataStartEnd(nStartIndex, nEndIndex);
		// ����(04/11/29) �ϸ����ǥ�� ���� �߰�Data���� �������־�� �Ѵ�.
		m_pRadarControl->SetDataViewInfo(nTotalDataCount - nGlanceBalanceMargin, nStartIndex, nEndIndex - nStartIndex + 1, m_bOneChart);
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStartEndIndexToOcx
 - Created at  :  2004-10-27   18:01
 - Author      :  ������
 - Description :  ��ũ�ѹٿ��� ����� Start/End Index�� OCX�� ������.
 - Update	   :  (11/05) Radar��Ʈ���� ��ũ�ѹٿ��� ���� Index�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetStartEndIndexToOcx(const int nStartIndex, const int nEndIndex)
{
	if(m_pToolBarManagerImple)
		m_pToolBarManagerImple->SetStartEndIndexToOcx(nStartIndex, nEndIndex);
	
	// Radar��Ʈ�� �������. Radar Chart���� �ش� �ε����� �����Ѵ�. ��������~~~
	if(m_pRadarControl)
		m_pRadarControl->SetDataViewInfo(m_pPacketList->GetMaxCount(), nStartIndex, nEndIndex - nStartIndex + 1, m_bOneChart);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetPageCountNextStepToOcx
 - Created at  :  
 - Author      :  yiilyoul@magicn.com
 - Description :  
 - Update	   :  
 -----------------------------------------------------------------------------------------*/
int CToolBarDlg::GetPageCountNextStepToOcx(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, const int nMin, const int nMax)
{
	if(m_pToolBarManagerImple)
	{
		return m_pToolBarManagerImple->GetPageCountNextStepToOcx(nSliderPos_Prev, nSliderPos_New, nSBCode, nMin, nMax);
	} else return nSliderPos_New;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetReverseToOcx
 - Created at  :  
 - Author      :  yiilyoul@magicn.com
 - Description :  
 - Update	   :  
 -----------------------------------------------------------------------------------------*/
BOOL CToolBarDlg::GetReverseToOcx()
{
	if(m_pToolBarManagerImple)
	{
		return m_pToolBarManagerImple->GetReverseToOcx();
	}
	
	return FALSE;
}

/*-------------------------------------------------------------------------------
 - Function    :  ExecuteZoomInZoomOut
 - Created at  :  2005-04-15   14:04
 - Author      :  ������
 - Description :  Zoom In/Zoom Out ����
 -------------------------------------------------------------------------------*/
void CToolBarDlg::ExecuteZoomInZoomOut(CButtonEx* pButton)
{
	if(pButton == NULL)
		return;

	int nZoomCount = 0;
	bool bCount = true;

	CString strOption2 = pButton->GetOption2();
	bCount = ( pButton->GetOption1() == _MTEXT( C4_COUNT));
	CString strPrefix = strOption2.SpanExcluding( "0123456789");
	nZoomCount = atoi( strOption2.Mid( strPrefix.GetLength()).SpanIncluding( "0123456789"));

	if( !bCount) nZoomCount = ( m_nSliderMax * nZoomCount) / 100;
	if(pButton->GetParam() == _MTEXT( C4_ZOOM_IN))	// Ȯ��
			m_pIChartCtrl->ZoomTo( -nZoomCount, TRUE);
	else	m_pIChartCtrl->ZoomTo( nZoomCount, TRUE);
}


/*-------------------------------------------------------------------------------
 - Function    :  ExecuteRadar
 - Created at  :  2005-04-15   14:07
 - Author      :  ������
 - Description :  Radar Chart�� �����Ѵ�.
 -------------------------------------------------------------------------------*/
void CToolBarDlg::ExecuteRadar()
{
	// �̹� ���̴���Ʈ�� ������ �ƹ��͵� ���� ������ �ȴ�.
	if(m_pRadarControl)
		return;

	// ���ٿ� Radar Chart �߰��� Rect���� �ٽ� ����Ѵ�.
	m_bUseRadarChart = TRUE;
	
	CRect rectToolBarDlg( m_pToolBarManagerImple->GetToolBarRect());
	rectToolBarDlg.top -= TB_RADAR_HEIGHT;
	MoveWindow( rectToolBarDlg);
	m_pToolBarManagerImple->SetToolBarRect( rectToolBarDlg);

	m_pRadarControl = new CRadarControl();
	m_pRadarControl->Create(this, IDC_RADAR);
	m_pRadarControl->SetIChartCtrl( m_pIChartCtrl);

	int nStartIndex = 0;
	int nEndIndex = 0;
	if(m_pScrollBar)
		m_pScrollBar->GetDataStartEnd(nStartIndex, nEndIndex);
	else
	{
		nStartIndex = 0;
		nEndIndex = m_pPacketList->GetMaxCount() - 1;
	}

	m_pRadarControl->SetDataViewInfo(m_pPacketList->GetMaxCount(), nStartIndex, nEndIndex - nStartIndex + 1, m_bOneChart);
	if( m_pIChartCtrl) m_pIChartCtrl->InvalidateOleControl(true);
}


/*-------------------------------------------------------------------------------
 - Function    :  ExecuteGraphColorSetting
 - Created at  :  2005-04-15   14:11
 - Author      :  ������
 - Description :  �׷���������â�� �����Ѵ�.
 -------------------------------------------------------------------------------*/
void CToolBarDlg::ExecuteGraphColorSetting()
{
	CColorDialog dlgColor(m_clrSelection);
	if(dlgColor.DoModal() != IDOK)
		return;
	m_clrSelection = dlgColor.GetColor();
	
	if( m_pIChartCtrl) m_pIChartCtrl->RunToolFuncGraphColor(m_clrSelection);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetToolFunction
 - Created at  :  2004-11-09   15:34
 - Author      :  ������
 - Description :  ��ưCtrl�� ������� �����Ѵ�. (OCX�� ��ɱ����� �Լ����� ȣ���Ѵ�.)
				  ����� ����� ���࿡ �ʿ��� �ּ� ����Ÿ�� nParam�̳� pButtonŬ������ü���� �ִ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetToolFunction(CToolOptionInfo::TOOLOPTION enumToolOption, CButtonEx* pButton /* = NULL*/, long nParam /* = 0*/)
{
	switch(enumToolOption) 
	{
		case CToolOptionInfo::T_INFLATE:	// Ȯ��, ���
			ExecuteZoomInZoomOut(pButton);
			break;
		case CToolOptionInfo::T_AUTOSCROLL:	// �ڵ� ��ũ��
			// (2006/11/25 - Seung-Won, Bae) Support New Scroll ID with Each Button.
			if( !pButton || !m_pIChartCtrl) break;
			if(			pButton->GetParam() == _MTEXT( C4_LEFT_AUTO_SCROLL))	m_pIChartCtrl->RunToolFunction( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT,	1);
			else if(	pButton->GetParam() == _MTEXT( C4_STOP_AUTO_SCROLL))			m_pIChartCtrl->RunToolFunction( CToolOptionInfo::T_ADDIN_AUTOSCROLL_STOP,		1);
			else if(	pButton->GetParam() == _MTEXT( C4_RIGHT_AUTO_SCROLL))	m_pIChartCtrl->RunToolFunction( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT,	1);
			break;
		case CToolOptionInfo::T_GRAPH_STYLE:				// �׷�����Ÿ��
			if( m_pIChartCtrl) m_pIChartCtrl->RunToolFuncGraphStyle(nParam);
			break;
		case CToolOptionInfo::T_GRAPH_THICKNESS:			// ������
			if( m_pIChartCtrl) m_pIChartCtrl->RunToolFuncLineThickness(nParam);
			break;
		case CToolOptionInfo::T_RADAR:						// ���̴���Ʈ ����
			ExecuteRadar();
			break;
		case CToolOptionInfo::T_GRAPH_COLOR:				// ������
			ExecuteGraphColorSetting();
			break;

		default:
			if( m_pIChartCtrl) m_pIChartCtrl->RunToolFunction(enumToolOption, 1);
			break;
	}//end of switch
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseToolFunction
 - Created at  :  2004-11-09   15:34
 - Author      :  ������
 - Description :  ��ưCtrl�� ����� ������ �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::ReleaseToolFunction(CToolOptionInfo::TOOLOPTION enumToolOption, CButtonEx* pButton /* = NULL*/, long nParam /* = 0*/)
{
	switch(enumToolOption)
	{
		// ���̴���Ʈ
		case CToolOptionInfo::T_RADAR:	if(m_pRadarControl)
										{
											// ���ٿ��� Radar Chart ���� Rect�� �ٽ� ����Ѵ�.
											m_bUseRadarChart = FALSE;

											CRect rectToolBarDlg( m_pToolBarManagerImple->GetToolBarRect());
											rectToolBarDlg.top += TB_RADAR_HEIGHT;
											MoveWindow( rectToolBarDlg);
											m_pToolBarManagerImple->SetToolBarRect( rectToolBarDlg);

											delete m_pRadarControl;
											m_pRadarControl = NULL;

											if( m_pIChartCtrl) m_pIChartCtrl->InvalidateOleControl(true);
										}
										break;

		case CToolOptionInfo::T_AUTOSCROLL:	// �ڵ� ��ũ��
			// (2006/11/25 - Seung-Won, Bae) Support New Scroll ID with Each Button.
			if( !pButton || !m_pIChartCtrl) break;
			if(			pButton->GetParam() == _MTEXT( C4_LEFT_AUTO_SCROLL))	m_pIChartCtrl->RunToolFunction( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT,	0);
			else if(	pButton->GetParam() == _MTEXT( C4_RIGHT_AUTO_SCROLL))	m_pIChartCtrl->RunToolFunction( CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT,	0);
			break;

		default:	if( m_pIChartCtrl) m_pIChartCtrl->RunToolFunction( enumToolOption, 0);
					break;
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetOneChart
 - Created at  :  2004-11-05   11:03
 - Author      :  ������
 - Description :  ������ǥ��Ʈ�ϰ�츦 �����Ѵ�. true:������ǥ��Ʈ.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetOneChart( bool bOneChart, bool p_bIsNoZoomChart)
{
	m_bOneChart = bOneChart;
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	m_bIsNoZoomChart = p_bIsNoZoomChart;

	// (2007/2/8 - Seung-Won, Bae) Disable WholeView Btn in Special Chart.
	if( m_pWholeViewBtn) m_pWholeViewBtn->EnableWindow( !m_bIsNoZoomChart);
	// (2007/2/8 - Seung-Won, Bae) Disable Zoom Btn in Special Chart.
	if( m_pZoomInBtn) m_pZoomInBtn->EnableWindow( !m_bIsNoZoomChart);
	if( m_pZoomOutBtn) m_pZoomOutBtn->EnableWindow( !m_bIsNoZoomChart);

	// 3. with SlideBar
	if(m_pSliderCtrl) m_pSliderCtrl->EnableWindow( !m_bIsNoZoomChart);

	// 4. with ScrollBar
	if( m_pScrollBar) m_pScrollBar->EnableWindow( !m_bIsNoZoomChart);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetOneChart
 - Created at  :  2004-11-05   11:05
 - Author      :  ������
 - Description :  ���� ������ǥ��Ʈ�������� �����Ѵ�. true:������ǥ��Ʈ.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::GetOneChart()
{
	return m_bOneChart;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetCountMinPerPage
 - Created at  :  2004-11-04   10:26
 - Author      :  ������
 - Description :  ��ȭ���� �ּ� ����Ÿ ���� �����´�. (For Radar Chart)
 -----------------------------------------------------------------------------------------*/
int CToolBarDlg::GetCountMinPerPage()
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetMinCountPerPage();
	return -1;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetDataCount
 - Created at  :  2004-10-28   14:20
 - Author      :  ������
 - Description :  ��������Ÿ����(��������)�� ���ؿ´�.
 -----------------------------------------------------------------------------------------*/
int CToolBarDlg::GetDataCount()
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetDataCountForToolBar();
	return -1;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetControlData
 - Created at  :  2004-11-04   11:30
 - Author      :  ������
 - Parameters  :  pPacketList - ��Ŷ����Ʈ
 - Return Value:  None
 - Description :  OCX�κ��� ������ ��Ʈ�ѵ���Ÿ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager)
{
	m_pPacketList = pPacketList;
	m_pPacketListManager = pPacketListManager;
	m_pxScaleManager = pxScaleManager;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetScrollData_Real
 - Created at  :  2004-10-28   15:04
 - Author      :  ������
 - Parameters  :  nTotalDataCount - �ѵ���Ÿ����
 -                nPageDataCount  - ���������� ���� ����Ÿ ����
 -                bFullPage       - ��ü���� ����
 - Return Value:  None
 - Description :  RealData�� �������� SliderCtrl�� ScrollBar�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetScrollData_Real(const int nTotalDataCount, const int nPageDataCount, const bool bFullPage)
{
	// 1. Process Scroll with ScrollBar
	if(m_pScrollBar)	// ScrollBar�� ������ �� �Լ� ��ü�� ��Ÿ���� �׷��� ������ġ.
		m_pScrollBar->SetScrollData_Real(nTotalDataCount);

	// 2. Backup m_nSliderMax
	// MODIFY: ����(04/07/28) '��ü����'������ TotalDataCount�� SliderMaxRange�̴�.
	if(bFullPage)
		m_nSliderMax = nTotalDataCount;
	else if( m_nSliderMax < nPageDataCount)
		m_nSliderMax = nPageDataCount;
	else if(m_nSliderMax < nTotalDataCount)
		m_nSliderMax = nTotalDataCount;

	// 3. Update Slider Control
	if( m_pSliderCtrl)
	{
		m_pSliderCtrl->SetRange(m_nSliderMin, m_nSliderMax, true);
		if(bFullPage) 
			m_pSliderCtrl->SetPos(nTotalDataCount);
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetScrollData_RemovePacket
 - Created at  :  2004-10-28   15:11
 - Author      :  ������
 - Parameters  :  nTotalDataCount  - �ѵ���Ÿ����
 -                bFullPage        - ��ü���� ����
 -                nRemoveDataCount - ������ ����Ÿ ����
 - Return Value:  None
 - Description :  PacketData�� ���Ƽ� ó���ϱⰡ ���������� ���� ��Ŷdata�� ����� �Ǵµ� �̶�
				  ȣ��Ǿ� ScrollBar�� SliderCtrl�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetScrollData_RemovePacket(const int nTotalDataCount, const bool bFullPage, const int nRemoveDataCount)
{
	// 0. Update SlideBar
	m_nSliderMax -= nRemoveDataCount;
	if(m_pSliderCtrl)
	{
		m_pSliderCtrl->SetRange(m_nSliderMin, m_nSliderMax, true);
		if(bFullPage)
			m_pSliderCtrl->SetPos(nTotalDataCount);
	}

	// 1. Scroll with ScrollBar
	if(m_pScrollBar) m_pScrollBar->SetScrollData_RemovePacket(nTotalDataCount, nRemoveDataCount);	
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetSliderPos
 - Created at  :  2004-10-28   15:49
 - Author      :  ������
 - Description :  SliderCtrl�� PageDataCount�� �����ϰ� OCX�� �˷��ش�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetSliderPos(int nPageDataCount)
{
	//sy working -> "��ü����" �� ��� : ���
	ToggleBT(CToolOptionInfo::T_WHOLEVIEW, IsWholeView());

	if(!m_pSliderCtrl || !m_pSliderCtrl->GetSafeHwnd())
		return;

	m_pSliderCtrl->SetPos(nPageDataCount);
	m_pSliderCtrl->Invalidate();
}

//sy 2005.12.09.
void CToolBarDlg::ToggleBT(const CToolOptionInfo::TOOLOPTION eToolOption, const bool bState)
{
	CButtonEx* pButtonEx = GetButtonEx(eToolOption);
	if(pButtonEx == NULL)
		return;

	if(pButtonEx->GetState() == bState)
		return;

	pButtonEx->SetState(bState);
	pButtonEx->Invalidate();
}
//sy end

/*-----------------------------------------------------------------------------------------
 - Function    :  SetSliderMin
 - Created at  :  2004-10-29   15:25
 - Author      :  ������
 - Description :  Slider�� �ּҰ��� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetSliderMin(const int nSliderMin)
{
	m_nSliderMin = nSliderMin;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnHScroll
 - Created at  :  2004-11-01   14:34
 - Author      :  ������
 - Description :  Scroll�� SliderBar�� HScroll�۾��� ó���Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if( !pScrollBar) return;

	if( pScrollBar->GetDlgCtrlID() == IDC_SCROLL)
	{
		m_pScrollBar->OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}

    if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER && nSBCode != SB_ENDSCROLL)
    {
    	CSliderCtrl *pSlider = ( CSliderCtrl *)pScrollBar;
    	int nCount = m_pScrollBar->GetPageDataCount();
    	int nSliderPos = m_pSliderCtrl->GetPos();
    	
   		if( m_pToolBarManagerImple && (nSBCode == SB_PAGELEFT || nSBCode == SB_PAGERIGHT || nSBCode == SB_THUMBTRACK))
  		{
  			int nSliderPos_Prev;
  			int nSliderPageSize = m_pSliderCtrl->GetPageSize();
  			int nSliderMin, nSliderMax;
  			m_pSliderCtrl->GetRange(nSliderMin, nSliderMax);

  			switch(nSBCode)
  			{
  			case SB_PAGELEFT:	
  				{
  					nSliderPos_Prev = nSliderPos + nSliderPageSize;
  					if(nSliderPos_Prev > nSliderMax) nSliderPos_Prev = nSliderMax;
  				}
  				break;

  			case SB_PAGERIGHT:
  				{
  					nSliderPos_Prev = nSliderPos - nSliderPageSize;
  					if(nSliderPos_Prev < nSliderMin) nSliderPos_Prev = nSliderMin;
  				}
  				break;
  			}

  			nSliderPos = GetPageCountNextStepToOcx(nSliderPos_Prev, nSliderPos, nSBCode);
  		}

		m_pIChartCtrl->ZoomTo( nSliderPos - nCount, TRUE);
    }
}



/*-----------------------------------------------------------------------------------------
 - Function    :  GetStartEndIndex
 - Created at  :  2004-11-01   14:35
 - Author      :  ������
 - Parameters  :  nStartIndex - ����Ÿ�� �����ε���
 -                nEndIndex   - ����Ÿ�� ���ε���
 - Return Value:  bool : true����, false����.
 - Description :  ScrollBar�� ������ �ִ� Start/End�ε����� �����´�.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::GetStartEndIndex(int& nStartIndex, int& nEndIndex)
{
	if(m_pScrollBar)
	{
		m_pScrollBar->GetDataStartEnd(nStartIndex, nEndIndex);
		return true;
	}
	else
		return false;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphType
 - Created at  :  2004-11-01   16:32
 - Author      :  ������
 - Parameters  :  graphType - ���� ���õ� �׷���Ÿ��.
 -                nIndex    - �ε���
 - Return Value:  None
 - Description :  OCX���� ������ �׷�����Ÿ�Ͽ� ���� �޺��� �ش罺Ÿ�������� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex)
{
	if(m_pGraphStyleCombo)
		m_pGraphStyleCombo->SetGraphType(graphType, nIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetLineThickness
 - Created at  :  2004-11-03   11:25
 - Author      :  ������
 - Parameters  :  nThickness - ���β� ����.
 - Description :  OCX���� ������ ���εβ��������� ���� �޺��� �ش� ��Ÿ�� ���������� �ٲ۴�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetLineThickness(int nThickness)
{
	if(m_pGraphLineCombo)
		m_pGraphLineCombo->SetLineThickness(nThickness);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphColor
 - Created at  :  2004-11-29   17:07
 - Author      :  ������
 - Description :  OCX���� ������ �׷����� ������ ��������� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetGraphColor(COLORREF clrGraphColor)
{
	m_clrSelection = clrGraphColor;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetPacketList
 - Created at  :  2004-11-04   11:32
 - Author      :  ������
 - Description :  ��Ŷ����Ʈ�� �����Ѵ�.(Radar Chart���� ���)
 -----------------------------------------------------------------------------------------*/
CPacketList* CToolBarDlg::GetPacketList()
{
	return m_pPacketList;
}

CPacketListManager* CToolBarDlg::GetPacketListManager()
{
	return m_pPacketListManager;
}

CXScaleManager* CToolBarDlg::GetXScaleManager()
{
	return m_pxScaleManager;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  GetResourceFont
 - Created at  :  2004-11-04   11:39
 - Author      :  ������
 - Description :  ���ҽ� ��Ʈ�� �����´�.
 -----------------------------------------------------------------------------------------*/
CFont* CToolBarDlg::GetResourceFont()
{
	if( m_pIChartCtrl) return m_pIChartCtrl->GetRscFont();
	return NULL;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsWholeView
 - Created at  :  2004-11-04   13:48
 - Author      :  ������
 - Description :  OCX�� ���� ��ü���������� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::IsWholeView()
{
	if( m_pIChartCtrl) return m_pIChartCtrl->IsWholeView();
	return false;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetStateWholeView
 - Created at  :  2004-11-22   15:58
 - Author      :  ������
 - Description :  ��ü���� ���¸� OCX�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::SetStateWholeView(const int nState)
{
	if( m_pIChartCtrl) m_pIChartCtrl->SetStateWholeView(nState);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseOtherButtonUI
 - Created at  :  2004-11-11   11:08
 - Author      :  ������
 - Parameters  :  enumToolOption - ������ ��ų Tool��ư�� Enum��
 -                strParam - �ϳ��� ���ɼ��� �����ϴ� ��ư�� �����ϱ� ���� ��. (ex: Ȯ��/���)
 - Description :  �ش��ư�� ������ ��Ų��.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam /*= _T("")*/)
{
	if(m_pButtonProperty)
		m_pButtonProperty->ReleaseOtherButtonUI(enumToolOption, strParam);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  IsScrollBar
 - Created at  :  2004-11-17   13:29
 - Author      :  ������
 - Description :  ��ũ�ѹٸ� ����ϴ����� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
bool CToolBarDlg::IsScrollBar()
{
	if(m_pScrollBar == NULL)
		return false;
	return m_pScrollBar->GetSafeHwnd() ? true : false;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetMaxRange
 - Created at  :  2004-11-17   14:59
 - Author      :  ������
 - Description :  MaxRange�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
int CToolBarDlg::GetMaxRange()
{
	return m_nSliderMax;
}



/*-----------------------------------------------------------------------------------------
 - Function    :  GetButtonEx
 - Created at  :  2004-12-02   15:03
 - Author      :  ������
 - Description :  �ش� ���ɼ��� �����Ǿ� �ִ� ��ư �����͸� ���´�.
 -----------------------------------------------------------------------------------------*/
CButtonEx* CToolBarDlg::GetButtonEx(CToolOptionInfo::TOOLOPTION enumToolOption)
{
	// (2006/11/25 - Seung-Won, Bae) Support Auto Scroll Tool Button
	CToolOptionInfo::TOOLOPTION eToolOption2 = enumToolOption;
	if( enumToolOption == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT || enumToolOption == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT)
		enumToolOption = CToolOptionInfo::T_AUTOSCROLL;

	POSITION pos = m_listToolButton.GetHeadPosition();
	CButtonEx* pButton = NULL;
	while(pos)
	{
		pButton = m_listToolButton.GetNext(pos);
		if( enumToolOption == pButton->GetToolOptionInfo())
		{
			// (2006/11/25 - Seung-Won, Bae) Support Auto Scroll Tool Button
			if( enumToolOption != CToolOptionInfo::T_AUTOSCROLL) return pButton;
			if( eToolOption2 == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_LEFT && pButton->GetParam() == _MTEXT( C4_LEFT_AUTO_SCROLL)) return pButton;
			if( eToolOption2 == CToolOptionInfo::T_ADDIN_AUTOSCROLL_PLAY_RIGHT && pButton->GetParam() == _MTEXT( C4_RIGHT_AUTO_SCROLL)) return pButton;
		}
	}
	return NULL;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ReDraw
 - Created at  :  2005-08-16   19:02
 - Author      :  ������
 - Description :  ���� Invalidate.
 -----------------------------------------------------------------------------------------*/
void CToolBarDlg::ReDraw()
{
	POSITION pos = m_listToolBarCtrl.GetHeadPosition();
	while(pos)
	{
		CWnd* pWnd = m_listToolBarCtrl.GetNext(pos);

		if(pWnd == NULL)
			continue;
		pWnd->Invalidate();
	}
}

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
void CToolBarDlg::UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView)
{
	CButtonEx* pButton = m_pWholeViewBtn;
	if( pButton && pButton->GetSafeHwnd()) pButton->SetState( p_bWholeView ? true : false);

	if( m_pScrollBar) if( m_pScrollBar->GetSafeHwnd())
		m_pScrollBar->UpdateZoomAndScrollInfo( p_nFullRange, p_nStartPos, p_nViewCount);
	if( m_pSliderCtrl) if( m_pSliderCtrl->GetSafeHwnd())
	{
		m_nSliderMax = p_nFullRange;
		m_pSliderCtrl->SetRange( m_nSliderMin, m_nSliderMax, true);
		m_pSliderCtrl->SetPos( p_nViewCount);
	}
}
