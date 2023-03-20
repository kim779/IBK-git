// DFontPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DFontPropPage.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _CRTM1()
#include "../Include_Chart/xUtil_Chart.h"				// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"				// for IPropertyMap
#include "PropertiesData.h"								// for g_anPropertyShortKey[]

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CDFontPropPage, CDialog)
	//{{AFX_MSG_MAP(CDFontPropPage)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_FONT_BOLD, OnFontBold)
	ON_BN_CLICKED(IDC_FONT_CANCELLINE, OnFontCancelline)
	ON_BN_CLICKED(IDC_FONT_ITALY, OnFontItaly)
	ON_BN_CLICKED(IDC_FONT_UNDERLINE, OnFontUnderline)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTLIST, OnSelchangeComboFontlist)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTSIZE, OnSelchangeComboFontsize)
	ON_CBN_EDITCHANGE(IDC_COMBO_FONTSIZE, OnEditchangeComboFontsize)
	ON_CBN_EDITCHANGE(IDC_COMBO_FONTLIST, OnEditchangeComboFontlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDFontPropPage::CDFontPropPage - Constructor

// TODO: Define string resource for page caption; replace '0' below with ID.

CDFontPropPage::CDFontPropPage(CWnd* pOcxWnd, CWnd* pParent /*=NULL*/) 
	: CDialog(CDFontPropPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDFontPropPage)
	m_FontProp = _T("");
	//}}AFX_DATA_INIT
	mt_bBold = FALSE;
	mt_bCancelLine = FALSE;
	mt_bItaly = FALSE;
	mt_bUnderLine = FALSE;
	mt_nlfHeight = 12;

	m_pOcxWnd = pOcxWnd;

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = NULL;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = m_pOcxWnd->GetSafeHwnd();
}

/////////////////////////////////////////////////////////////////////////////
// CDFontPropPage::DoDataExchange - Moves data between page and properties

void CDFontPropPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//{{AFX_DATA_MAP(CDFontPropPage)
	DDX_Control(pDX, IDC_FONT_UNDERLINE, m_checkUnderLine);
	DDX_Control(pDX, IDC_FONT_ITALY, m_checkItaly);
	DDX_Control(pDX, IDC_FONT_CANCELLINE, m_checkCancelLine);
	DDX_Control(pDX, IDC_FONT_BOLD, m_checkBold);
	DDX_Control(pDX, IDC_COMBO_FONTLIST, m_cbFontList);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_cbFontSize);
	DDX_Control(pDX, IDC_FONT_PREVIEW, m_FontPreview);
	DDX_Text(pDX, IDC_EDIT_FONT, m_FontProp);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CDFontPropPage message handlers
#include <Wingdi.h>
#include <Windows.h>

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��17��  16��44��	
	���� Ÿ�� : int
	�� �� ��  : EnumFontProc
	�Լ� ���� : �ý��ۿ� �ִ� ��Ʈ������ �޺��� �Է��Ѵ�.
	��     �� : 	
*********************************************************************/
int CALLBACK EnumFontProc(ENUMLOGFONT* lpelf, NEWTEXTMETRIC* lpntm,
						  unsigned long nFontType, LPARAM lParam)
{    
    CComboBox *pCombo = (CComboBox *)lParam;
    if (nFontType & TRUETYPE_FONTTYPE) {
        pCombo->AddString(lpelf->elfLogFont.lfFaceName);
    }
    return(TRUE);
}

BOOL CDFontPropPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO_FONTLIST);
	CDC *pDC = GetDC();
	::EnumFontFamilies(pDC->m_hDC, NULL, (FONTENUMPROC)EnumFontProc, (LPARAM)pCombo);
	ReleaseDC(pDC);	

	FT_vInitFont(m_FontProp);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDFontPropPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	DrawPreview();
	// Do not call COlePropertyPage::OnPaint() for painting messages
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��17��  16��43��	
	���� Ÿ�� : void
	�� �� ��  : OnFontBold
	�Լ� ���� : �� üũ��ư�� üũ������ 
	��     �� : 	
*********************************************************************/
void CDFontPropPage::OnFontBold() 
{	
	mt_bBold = m_checkBold.GetCheck();
	DrawPreview();		//�̸����� ���������� ReDraw
	FT_vSaveFont();		//��Ʈ������Ʈ���� �ٽ� �����ϱ����� �Լ�
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��17��  16��43��	
	���� Ÿ�� : void
	�� �� ��  : OnFontCancelline
	�Լ� ���� : ��Ҽ� üũ��ư�� üũ������ 
	��     �� : 	
*********************************************************************/
void CDFontPropPage::OnFontCancelline() 
{
	mt_bCancelLine = m_checkCancelLine.GetCheck();
	DrawPreview();		//�̸����� ���������� ReDraw
	FT_vSaveFont();		//��Ʈ������Ʈ���� �ٽ� �����ϱ����� �Լ�
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��17��  16��43��	
	���� Ÿ�� : void
	�� �� ��  : OnFontItaly
	�Լ� ���� : ����Ӳ� üũ��ư�� üũ������ 
	��     �� : 	
*********************************************************************/
void CDFontPropPage::OnFontItaly() 
{	
	mt_bItaly = m_checkItaly.GetCheck();
	DrawPreview();		//�̸����� ���������� ReDraw
	FT_vSaveFont();		//��Ʈ������Ʈ���� �ٽ� �����ϱ����� �Լ�
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��17��  16��43��	
	���� Ÿ�� : void
	�� �� ��  : OnFontUnderline
	�Լ� ���� : ������� üũ��ư�� üũ������ 
	��     �� : 	
*********************************************************************/
void CDFontPropPage::OnFontUnderline() 
{
	mt_bUnderLine = m_checkUnderLine.GetCheck();
	DrawPreview();		//�̸����� ���������� ReDraw
	FT_vSaveFont();		//��Ʈ������Ʈ���� �ٽ� �����ϱ����� �Լ�
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��17��  16��42��	
	���� Ÿ�� : void
	�� �� ��  : OnSelchangeComboFontlist
	�Լ� ���� : ��Ʈ���� �޺����� ����������
	��     �� : 	
*********************************************************************/
void CDFontPropPage::OnSelchangeComboFontlist() 
{	
	int index = m_cbFontList.GetCurSel();
	mt_strlfFaceName = "";
	if(index != -1)
		m_cbFontList.GetLBText(index,mt_strlfFaceName);
	DrawPreview();		//�̸����� ���������� ReDraw
	FT_vSaveFont();		//��Ʈ������Ʈ���� �ٽ� �����ϱ����� �Լ�
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��17��  16��14��	
	���� Ÿ�� : void
	�� �� ��  : OnSelchangeComboFontsize
	�Լ� ���� : ��Ʈ����� �ٲ� ���� �ٽü������ش�.
	��     �� : 	
*********************************************************************/
void CDFontPropPage::OnSelchangeComboFontsize() 
{
	int index = m_cbFontSize.GetCurSel();
	CString tempSize = "";
	if(index != -1)
		m_cbFontSize.GetLBText(index,tempSize);
	mt_nlfHeight = -MulDiv(atoi(tempSize), GetDeviceCaps(GetDC()->GetSafeHdc(), LOGPIXELSY), 72); 
	DrawPreview();		//�̸����� ���������� ReDraw
	FT_vSaveFont();		//��Ʈ������Ʈ���� �ٽ� �����ϱ����� �Լ�
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��17��  16��11��	
	���� Ÿ�� : void
	�� �� ��  : FT_vInitFont
	�Լ� ���� : ��Ʈ������ �Ǿ��ִ� ��Ʈ������ �о ����Ʈ�ѿ� �ʱⰪ�� �ְ�
				���¸� ǥ���Ѵ�.
	��     �� : 	
*********************************************************************/
void CDFontPropPage::FT_vInitFont(const CString& initStr)
{
//	CString initStr="";
//	GetDlgItem(IDC_EDIT_FONT)->GetWindowText(initStr);
	CString strFont = initStr;
	//��Ʈ�̸�
	int namePos = strFont.Find(";");
	CString lfFaceName = strFont.Mid(0,namePos);
	m_cbFontList.SetWindowText(lfFaceName);	//�޺��� ����Ʈ��Ų��.
	mt_strlfFaceName = lfFaceName;			//������ ����
	//��Ʈũ��
	int HeightPos = strFont.Find(";",namePos+1);
	CString lfHeight = strFont.Mid(namePos+1,HeightPos - (namePos+1));
	int nlfHeight = atoi(lfHeight);
	mt_nlfHeight = nlfHeight;				//������ ����
	//��Ʈ���� ��ȭ
	int lfFontSize = (nlfHeight*72)/(GetDeviceCaps(GetDC()->GetSafeHdc(), LOGPIXELSY));	
		
	CString fontSizeTemp="";
	fontSizeTemp.Format("%d",abs(lfFontSize));
	m_cbFontSize.SetWindowText(fontSizeTemp);	//�޺��� ����Ʈ��Ų��.

	//������Ʈ
	int BoldPos = strFont.Find(";",HeightPos+1);
	CString strBold = strFont.Mid(HeightPos+1,BoldPos - (HeightPos+1));
	BOOL	blfWeight = atoi(strBold);
	m_checkBold.SetCheck(blfWeight);		//üũ�ڽ��� üũ�Ѵ�.
	mt_bBold = blfWeight;					//������ ����
	//��Ҽ�
	int CCLinePos = strFont.Find(";",BoldPos+1);
	CString strCCLine = strFont.Mid(BoldPos+1,CCLinePos-(BoldPos+1));
	BOOL	blfStrikeOut = atoi(strCCLine);
	m_checkCancelLine.SetCheck(blfStrikeOut);//üũ�ڽ��� üũ�Ѵ�.
	mt_bCancelLine = blfStrikeOut;			//������ ����
	//����Ӳ�
	int italyPos = strFont.Find(";",CCLinePos+1);
	CString strItaly = strFont.Mid(CCLinePos+1,italyPos - (CCLinePos+1));
	BOOL	blfItalic = atoi(strItaly);
	m_checkItaly.SetCheck(blfItalic);		//üũ�ڽ��� üũ�Ѵ�.
	mt_bItaly = blfItalic;					//������ ����
	//�������
	int UnderLinePos = strFont.Find(";",italyPos+1);
	CString strUnderLine = strFont.Mid(italyPos+1,UnderLinePos-(italyPos+1));
	BOOL	blfUnderline = atoi(strUnderLine);
	m_checkUnderLine.SetCheck(blfUnderline);//üũ�ڽ��� üũ�Ѵ�.
	mt_bUnderLine = blfUnderline;			//������ ����
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��17��  16��10��	
	���� Ÿ�� : void
	�� �� ��  : FT_vSaveFont
	�Լ� ���� : ����Ʈ���� ��Ʈ������ ��Ʈ������ ǥ���ؼ� ����Ʈ��Ʈ�ѿ� �Է��Ѵ�.
				����Ʈ��Ʈ�ѷμ� ������Ƽ�� �����Ѵ�.
	��     �� : 	
*********************************************************************/
void CDFontPropPage::FT_vSaveFont()
{
	CString strFont = "";
	strFont.Format(";%d;%d;%d;%d;%d;",mt_nlfHeight,mt_bBold,mt_bCancelLine,mt_bItaly,mt_bUnderLine);
	m_FontProp= "";
	m_FontProp = mt_strlfFaceName + strFont;
	GetDlgItem(IDC_EDIT_FONT)->SetWindowText(m_FontProp);
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��18��  9��07��	
	���� Ÿ�� : void
	�� �� ��  : DrawPreview
	�Լ� ���� : �۲��� �̸�����κ��� ������Ʈ�ϴ� �Լ�
	��     �� : 	
*********************************************************************/
void CDFontPropPage::DrawPreview()
{
	CDC* pDC = m_FontPreview.GetDC();
	CRect PreViewRect;
	m_FontPreview.GetClientRect(&PreViewRect);
	PreViewRect.DeflateRect(1,1,1,1);
	pDC->FillSolidRect(PreViewRect,RGB(255,255,255));
	
	
	
	CFont* OldFont=pDC->GetCurrentFont();
	CFont m_Font;
	LOGFONT m_logFont;
	
	m_logFont.lfHeight = mt_nlfHeight;
	m_logFont.lfWidth = 0;
	m_logFont.lfEscapement = 0;
	m_logFont.lfOrientation = 0;
	if(mt_bBold)
		m_logFont.lfWeight = FW_BOLD;
	else
		m_logFont.lfWeight = FW_THIN;
	m_logFont.lfItalic = mt_bItaly;
	m_logFont.lfUnderline = mt_bUnderLine;
	m_logFont.lfStrikeOut = mt_bCancelLine;
	m_logFont.lfCharSet = DEFAULT_CHARSET;
	m_logFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
	m_logFont.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	m_logFont.lfQuality = DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
	strcpy(m_logFont.lfFaceName, mt_strlfFaceName);
	
	m_Font.CreateFontIndirect(&m_logFont);	
	
	pDC->SelectObject(&m_Font);
	pDC->SetBkMode(TRANSPARENT);
	
	pDC->DrawText(mt_strlfFaceName,PreViewRect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	
	m_Font.DeleteObject();
	pDC->SelectObject(OldFont);
	
	OldFont=NULL;	delete OldFont;	
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��18��  9��32��	
	���� Ÿ�� : void
	�� �� ��  : OnEditchangeComboFontsize
	�Լ� ���� : ����� �����ϴ� �޺��ڽ����� ����Ʈâ�� ���� ���ڸ� �Է��������
	��     �� : 	
*********************************************************************/
void CDFontPropPage::OnEditchangeComboFontsize() 
{
	CString EditStr = "";
	m_cbFontSize.GetWindowText(EditStr);
	mt_nlfHeight = -MulDiv(atoi(EditStr), GetDeviceCaps(GetDC()->GetSafeHdc(), LOGPIXELSY), 72);
	DrawPreview();
	FT_vSaveFont();		//��Ʈ������Ʈ���� �ٽ� �����ϱ����� �Լ�
}

/********************************************************************
	������    : ������(Sung-Jin Do)	
	Email     : dos74@dooriic.co.kr
	����ð�  : 2001��9��18��  9��33��	
	���� Ÿ�� : void
	�� �� ��  : OnEditchangeComboFontlist
	�Լ� ���� : ��Ʈ�� ���ϴ� �޺����� ���� ��Ʈ�̸��� �Է����� ���
	��     �� : 	
*********************************************************************/
void CDFontPropPage::OnEditchangeComboFontlist() 
{
	CString EditStr = "";
	m_cbFontList.GetWindowText(EditStr);
	if(m_cbFontList.FindStringExact(-1,EditStr) == CB_ERR)
		mt_strlfFaceName = "";
	else
		mt_strlfFaceName = EditStr;

	DrawPreview();
	FT_vSaveFont();		//��Ʈ������Ʈ���� �ٽ� �����ϱ����� �Լ�
}

// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
void CDFontPropPage::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CUSTOMFONT], m_FontProp,	_DPV_CUSTOMFONT);	
	CHART_CONVERT_END()

	int nFNIndex = m_FontProp.Find( ';');
	CString strFontName = m_FontProp.Left( nFNIndex);
	strFontName = _MTOL0( strFontName);
	m_FontProp = strFontName + m_FontProp.Mid( nFNIndex);

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;
}

void CDFontPropPage::OnOK() 
{
	int nFNIndex = m_FontProp.Find( ';');
	CString strFontName = m_FontProp.Left( nFNIndex);
	_SETM( "$FontName$", strFontName);
	m_FontProp = "$FontName$" + m_FontProp.Mid( nFNIndex);

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	CUSTOMFONT], m_FontProp,	_DPV_CUSTOMFONT);
	CHART_ADDTOSTRLIST_END

	CDialog::OnOK();
}
