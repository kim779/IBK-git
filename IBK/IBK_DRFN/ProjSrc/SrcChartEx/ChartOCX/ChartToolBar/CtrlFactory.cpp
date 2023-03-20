// CtrlFactory.cpp: implementation of the CCtrlFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CtrlFactory.h"

#include "../Include_Chart/DLL_Load/IMetaTable.h"		// for ML_DEFAULT_RES
#include "ScrollBarEx.h"
#include "SliderCtrlEx.h"
#include "ButtonEx.h"
#include "ToggleButton.h"
#include "NotToggleButton.h"
#include "GraphStyleCombo.h"
#include "GraphLineThicknessCombo.h"
#include "RadarControl.h"
#include "Definition.h"
#include "ObjTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern bool IsToggleButton(CToolOptionInfo::TOOLOPTION enumToolOption, const CString &p_strParam);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCtrlFactory::CCtrlFactory( IChartCtrl *p_pIChartCtrl, CWnd* pParent, const CRect rectToolBar, const bool bStatusBar, const bool bRadarChart, const int nScrollWidth, const bool bBitmap)
{
	m_pIChartCtrl = p_pIChartCtrl;
	m_pParent = pParent;
	m_rectCtrl = rectToolBar;
	m_bStatusBar = bStatusBar;
	m_bRadarChart = bRadarChart;
	m_nScrollWidth = nScrollWidth;
	m_nToolBarWidth = m_rectCtrl.Width();
	m_nCtrlID = TB_CTRL_INITIAL_ID;
	m_bBitmap = bBitmap;

	m_hOcxWnd = m_pParent->GetParent()->GetSafeHwnd();
}

CCtrlFactory::~CCtrlFactory()
{

}


/*-----------------------------------------------------------------------------------------
 - Function    :  Initialize
 - Created at  :  2004-10-27   11:35
 - Author      :  ������
 - Description :  ���丮 Ŭ���� �ʱ�ȭ. ���ٹ�ư ���� ��ġ�� �ʱ�ȭ ��Ű��, StatusBar�� ���� ���
				  Status Bar�� ���� ��ŭ ���� ������ġ���� �� �ش�.
 -----------------------------------------------------------------------------------------*/
void CCtrlFactory::Initialize()
{
	m_rectCtrl.right = m_rectCtrl.left;

	if(m_bStatusBar)	// ��ġ��ȸ����Ÿ����(StatusBar)�� ��� StatusBar�� ���� ���� ����Ƿ�...
		m_rectCtrl.top += TB_BUTTON_HEIGHT;
	// Radar Chart�� ���� �Ʒ��ʿ� ����Ƿ� �Ű� �Ƚ��൵ ��.
}


/*-----------------------------------------------------------------------------------------
 - Function    :  CreateScrollBar
 - Created at  :  2004-10-27   11:36
 - Author      :  ������
 - Description :  ��ũ�ѹٸ� ������Ų��.
 -----------------------------------------------------------------------------------------*/
CScrollBarEx* CCtrlFactory::CreateScrollBar()
{
	CScrollBarEx* pScrollBar = new CScrollBarEx( m_pIChartCtrl, m_nScrollWidth);
	
//	m_nCtrlID++;
	m_rectCtrl.right = m_rectCtrl.left + m_nScrollWidth;
	
	if(!pScrollBar->Create(SBS_HORZ|WS_VISIBLE|WS_CHILD, m_rectCtrl, m_pParent, IDC_SCROLL))
	{
		delete pScrollBar;
		return NULL;
	}

	pScrollBar->SetParentWnd((CToolBarDlg*)m_pParent);

	m_rectCtrl.left = m_rectCtrl.right;

	return pScrollBar;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  CreateSliderCtrl
 - Created at  :  2004-10-27   11:36
 - Author      :  ������
 - Description :  �����̴��ٸ� ������Ų��.
 -----------------------------------------------------------------------------------------*/
CSliderCtrlEx* CCtrlFactory::CreateSliderCtrl()
{
	CSliderCtrlEx* pSliderCtrl = new CSliderCtrlEx();

	m_rectCtrl.right = m_rectCtrl.left + TB_BUTTON_SLIDERBAR;
	
	if(!pSliderCtrl->Create(TBS_HORZ|TBS_AUTOTICKS|TBS_BOTTOM|TBS_TOOLTIPS|WS_VISIBLE, m_rectCtrl, m_pParent, IDC_SLIDER))
	{
		delete pSliderCtrl;
		return NULL;
	}

	pSliderCtrl->SetTipSide(TBTS_TOP);

	m_rectCtrl.left = m_rectCtrl.right;

	return pSliderCtrl;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  CreateGraphStyleCombo
 - Created at  :  2004-11-01   14:13
 - Author      :  ������
 - Description :  �׷���Ÿ�� ���� �޺��ڽ� ����
 -----------------------------------------------------------------------------------------*/
CGraphStyleCombo* CCtrlFactory::CreateGraphStyleCombo(const CString& strName)
{
	CGraphStyleCombo* pGraphStyleCombo = new CGraphStyleCombo();

	m_rectCtrl.right = m_rectCtrl.left + TB_BUTTON_WIDTH * 3;
	m_rectCtrl.bottom += 100;

	if(!pGraphStyleCombo->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_OWNERDRAWVARIABLE, m_rectCtrl, m_pParent, IDC_GRAPHSTYLE_COMBO))
	{
		delete pGraphStyleCombo;
		return NULL;
	}
	m_rectCtrl.left = m_rectCtrl.right;
	m_rectCtrl.bottom -= 100;
	
	pGraphStyleCombo->SetParentWnd((CToolBarDlg*)m_pParent);

	ML_SET_DEFAULT_RES();
	pGraphStyleCombo->GetBitmap().DeleteObject();
	if(!pGraphStyleCombo->GetBitmap().LoadBitmap(IDB_LINE_STYLE))
	{
		delete pGraphStyleCombo;
		return NULL;
	}	

	pGraphStyleCombo->SetGraphType(CGraphBaseData::Line_Type, 0);
	pGraphStyleCombo->SetToolTipText( _MTOL4( strName));

	return pGraphStyleCombo;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreatreGraphLineThicknessCombo
 - Created at  :  2004-11-03   11:09
 - Author      :  ������
 - Description :  ������ ���� �޺��ڽ� ����.
 -----------------------------------------------------------------------------------------*/
CGraphLineThicknessCombo* CCtrlFactory::CreatreGraphLineThicknessCombo(const CString& strName)
{
	CGraphLineThicknessCombo* pGraphLineCombo = new CGraphLineThicknessCombo();

	m_rectCtrl.right = m_rectCtrl.left + TB_BUTTON_WIDTH * 3;
	m_rectCtrl.bottom += 200;

	if(!pGraphLineCombo->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_OWNERDRAWVARIABLE, m_rectCtrl, m_pParent, IDC_LINETHICKNESS_COMBO))
	{
		delete pGraphLineCombo;
		return NULL;
	}
	
	pGraphLineCombo->SetParentWnd((CToolBarDlg*)m_pParent);
	
	ML_SET_DEFAULT_RES();
	pGraphLineCombo->GetBitmap().DeleteObject();
	if(!pGraphLineCombo->GetBitmap().LoadBitmap(IDB_LINE_THICKNESS))
	{
		delete pGraphLineCombo;
		return NULL;
	}
	
	pGraphLineCombo->AddLineType(5);
	pGraphLineCombo->SetCurSel(0);
	pGraphLineCombo->SetToolTipText( _MTOL4( strName));

	m_rectCtrl.left = m_rectCtrl.right;
	m_rectCtrl.bottom -= 200;
	
	return pGraphLineCombo;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  CreateButton
 - Created at  :  2004-11-08   11:25
 - Author      :  ������
 - Parameters  :  pObjTool	 - Tool������Ʈ�� ������, �� ������Ʈ�� ���� �ɼ�1,�ɼ�2, �̸�, Ÿ�Ե��� �����´�.
 -                strParam   - toolOption�ϳ��� �ΰ��̻��� ��ư���� �����ؾ��� ��� ������ ��ư���� �������ִ� �Ķ����.
 - Return Value:  CButtonEx* - ������ ��ưŬ������ �����Ѵ�.
 - Description :  ������ ��ư���� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
CButtonEx* CCtrlFactory::CreateButton(CObjTool* pObjTool, const CString& strParam)
{
	CButtonEx* pButton = NULL;
	
	// ��ۿ��ο� ���� ��ü����.
	if(IsToggleButton(pObjTool->GetType(), strParam))
		pButton = new CToggleButton();
	else	
		pButton = new CNotToggleButton();

	pButton->SetParentWnd((CToolBarDlg*)m_pParent);
	
	pButton->SetToolObject(pObjTool, strParam);

	m_rectCtrl.right = m_rectCtrl.left + TB_BUTTON_WIDTH;

	if(!pButton->Create(_T("B"), WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_AUTORADIOBUTTON | WS_GROUP/*BS_PUSHBUTTON*/, m_rectCtrl, m_pParent, m_nCtrlID))
	{
		delete pButton;
		return NULL;
	}
	
	if(m_bBitmap)	// ��Ʈ���� ������ OWNER_DRAW�� ��ư�̹����� �׸���.
		pButton->SetButtonStyle(BS_OWNERDRAW);

	m_nCtrlID++;
	m_rectCtrl.left = m_rectCtrl.right;

	return pButton;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  CreateRadarControl
 - Created at  :  2004-11-04   14:04
 - Author      :  ������
 - Description :  ���̴���Ʈ�� �����Ѵ�.
 -----------------------------------------------------------------------------------------*/
CRadarControl* CCtrlFactory::CreateRadarControl()
{
	CRadarControl* pRadarControl = new CRadarControl();

	// Rect����� �����Ѵ�. �ֳĸ� ���̴���Ʈ�� �� �Ʒ��ٿ� �Ҿƾ� ����� Ư���ѳ��̹Ƿ�
	// Create()�Լ��ο��� ����Ѵ�.
	if(!pRadarControl->Create(m_pParent, IDC_RADAR))
	{
		delete pRadarControl;
		return NULL;
	}
	
	return pRadarControl;
}
