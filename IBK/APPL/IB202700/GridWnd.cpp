// GridWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202700.h"
#include "GridWnd.h"
#include "grid/InPlaceEdit.h"
#include "../../h/grid.h"
#include "ioformat.h"

#include "MainWnd.h"
#include "GroupWnd.h"
#include "PopupDlg.h"
#include "CodeDuplicate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	HCOD		"1301"

#define	ARR_UP		"��"
#define	ARR_DN		"��"
#define	TRI_UP		"��"
#define	TRI_DN		"��"

//#define ARRAYSIZE		150

const COLORREF upCOLOR    = PALETTERGB(255,   0,   0);
const COLORREF dnCOLOR    = PALETTERGB(  0,   0, 255);
const COLORREF eqCOLOR    = PALETTERGB(  0,   0,   0);
const COLORREF whCOLOR    = PALETTERGB(255, 255, 255);

/////////////////////////////////////////////////////////////////////////////
// CGridWnd

bool operator<(const CCellID &lsh, const CCellID &rsh)
{
	return memcmp(&lsh, &rsh, sizeof(CCellID))<0;
}

bool operator<(CCellID &lsh, CCellID &rsh)
{
	return memcmp(&lsh, &rsh, sizeof(CCellID))<0;
}

CGridWnd::CGridWnd(CWnd* pMainWnd, int nIndex)
:CGridCtrlExt(), CAxisExt(pMainWnd)
{
	m_pMainWnd = pMainWnd;

	CGridCtrlExt::m_pFont = CAxisExt::m_pFont;

	SetGridIndex(nIndex);

	m_upCOL    = upCOLOR;
	m_dnCOL	   = dnCOLOR;
	m_eqCOL    = eqCOLOR;

	m_mapCode.RemoveAll();
	m_pGridArray.RemoveAll();
	m_bContinue = FALSE;
	m_bSecond = FALSE;
	m_bRTMReady = false;
	m_bSelect = false;
	m_bLBtnDown = false;
	m_kind = 0;
	m_nGroupIndex = -1;
	m_bAddCnd	= FALSE;
	m_bChgCnd  = FALSE;
	m_bPlusMinus	= false;

	m_endsort = false;
	m_posField = false;
	m_ccField  = false;
	m_drag = m_drop = -1;
	m_bongField = -1;
	m_sonikField = -1;
	m_suikField = -1;

	//for(int i=0 ; i< 100 ; i++)
	//{
	//	m_irowCode[i] = 0;

	//	for(int j=0;j<5;j++)
	//	{
	//		m_iYDayVol[i][j] = "";
	//	}
	//}
	
	m_yDayVolField = -1;			//���ϰŷ�������ʵ�
	m_EqualizerField = -1;
	
	m_recomDataField = -1;
	m_recomBasicCostField = -1;
	m_recomSuikrateField = -1;
	m_recomBMjisuField = -1;
	m_recomBMSuikrateField = -1;
	m_recomgoalCostField = -1;
	m_recomJisuGubnField = -1;
	m_upjongCode = -1;
	m_beforeVolField = -1;
	m_newCurrField = -1;

	m_pSearchMap.RemoveAll();
	m_mapCurValue.RemoveAll();

	m_strBeginTime = "081000";
	m_strEndTime = "085959";

	m_dKosdaq = 0;
	m_dKospi = 0; 


	m_bThreadRTM = false;
	m_pThreadRTM = nullptr;
	m_hEventRTM = nullptr;
	m_bRemove = FALSE;	// 2011.12.29 KDK
	m_bEditWork = FALSE;

	initRTM();

}

CGridWnd::~CGridWnd()
{
}


BEGIN_MESSAGE_MAP(CGridWnd, CGridCtrlExt)
	//{{AFX_MSG_MAP(CGridWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


BOOL CGridWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_GVITEM* nmgv = (NM_GVITEM*) lParam;

	if (wParam != IDC_GRID)
		return CWnd::OnNotify(wParam, lParam, pResult);	

	switch (nmgv->hdr.code)
	{
	case GVNM_REFRESH:
		{
			m_bEditWork = FALSE;

			ClearSearchMap();	//2011.12.29 KSJ 
			ReSetSearchMap();	//2011.12.29 KSJ 
		}
		break;
	case GVNM_CHGCOLSIZE:
		{
			const int	ncnt = m_gridHdrX.GetSize();
			CString	sztmp = _T("");
			int	nWidth{};
			m_szWIDTH = _T("");
			struct _gridHdr	grdHdr{};
			for ( int ii = 0 ; ii < ncnt ; ii++ )
			{
				grdHdr = m_gridHdrX.GetAt(ii);
				nWidth = GetColumnWidth(ii);
				CString strSymbol = (CString)grdHdr.symbol;
				
				//��õ�����ʵ�� �������� �ʴ´�
				if(strSymbol.Find("191") > -1 )
					continue;
				
				if(strSymbol.Find("2022") > -1 )
					continue;
				
				if (ii >= colNAME)
				{
					sztmp.Format("%d,", nWidth);
					m_szWIDTH += sztmp;

					if (ii == colCODE && nWidth != 0) m_nCodeWidth = nWidth;
				}
			}
			savecfg();
		}
		break;
	case GVNM_CHANGECODE:
		{		
			m_bEditWork = TRUE;
			SetFocus();
			CString code, oldname, name;
			code = GetItemText(nmgv->row, colCODE);

			if (code.GetLength() == 6 || code.GetLength() == 8 || code.GetLength() == 9)
			{
				name = GetCodeName(code);
			}

			struct	_inters* pinters{};
			const int count = m_inters.size();
			const int xrow  = nmgv->row - 1;

			CString strTemp;

			if(1)
			{
				if (code.GetLength() && IsDuplicateCode(code, nmgv->row))
				{
					CCodeDuplicate dlg;

					if(dlg.DoModal())
					{
						if(dlg.m_bAlert == FALSE)
						{
 							SetItemText(nmgv->row, colCODE, "");

							if (m_bAddCnd) 
							{	
								saveInterest();
							}
							break;
						}
						else
						{
							//���Ͽ� ����
							//((CGroupWnd*)GetParent())->saveOverLap(TRUE);
						}
					}
					else
					{
						auto& pInter = m_inters.at(nmgv->row - 1);
						SetItemText(nmgv->row, colCODE, pInter->code);
						DeleteRow(nmgv->row);	
						insertInterest(MAX_LINE - 1);
						if (m_bAddCnd) 
						{	
							saveInterest();
						}
						break;
					}
				}
			}


			if (count > xrow)
			{
				auto pinters = m_inters.at(xrow);

				// ADD PSH 20070913
				const int nRowCnt = m_arrBaseInters.size();

				if (m_bSorting)
				{
					if (!pinters->code.IsEmpty())
					{
						// 2011.12.19 KSJ �����ϰ��� ������ �ٲٸ� ���⼭ ������.
						// m_arrBaseInters ����� 0��..
						//CopyInter(m_arrBaseInters.GetAt(xrow), pinters);
						m_arrBaseInters.at(xrow) = pinters;
					}
					else
					{
						auto& pBaseInter = m_arrBaseInters.emplace_back(std::make_shared<_intersx>());

						pBaseInter->gubn = '0';
						pBaseInter->code = code;
						pBaseInter->name = name;
					}
				}
				// END ADD

				pinters->gubn = '0';
				pinters->code = code;
				pinters->name = name;
				SetItemText(xrow + 1, colNAME, name);
				//	m_inters.at(xrow) =  pinters;				
			}
			else
			{
				for (int ii = count; ii < xrow; ii++)
				{
					const auto& pinters = std::make_shared<_intersx>();
					pinters->gubn = '0';
					AddInters(pinters);
				}

				const auto& pinters = std::make_shared<_intersx>();

				pinters->gubn = '0';
				pinters->code = code;
				pinters->name = name;
				SetItemText(xrow + 1, colNAME, name);
				AddInters(pinters);
			}

			if (code.GetLength() <= 0)
			{
				for (int ii = 0; ii < m_colC; ii++)
					SetItemText(nmgv->row, ii, "");
				break;
			}


 			if (CAST_TREEID(m_kind)->kind == xINTEREST)
				m_bEditWork = true;

			if (m_bChgCnd) 
			{
				saveInterest();
			}

			sendTransaction(xrow);			
		}

		ClearSearchMap();	//2011.12.29 KSJ 
		ReSetSearchMap();	//2011.12.29 KSJ 
		Refresh();

		break;
	case GVNM_ENDEDIT: case GVNM_ENDRETURN:
		{		
			SetFocus();

			const int nRow = nmgv->row, nCol = nmgv->col;			

			CString	symbol, curr;
			symbol = GetItemSymbol(nRow, nCol);
			curr = GetItemText(nRow, colCURR);
			const auto& pInters = GetData(nRow - 1);
			CString	value = GetItemText(nRow, nCol);
			
			CString zeroValue = "0";

			//2011.12.20 KSJ
			if(symbol.IsEmpty())
				break;
			//KSJ

			if (symbol.CompareNoCase("###1") == 0) // ��������
			{
				value = GetItemText(nRow, nCol);
				//memset(pInters->xnum, 0x00, sizeof(pInters->xnum));
				//memcpy(pInters->xnum, value, min(value.GetLength(), 10));
				pInters->xnum = value;

				if (m_bChgCnd) 
				{
					saveInterest();
				}
			}
			else if (symbol.CompareNoCase("###2") == 0) // �����ܰ�
			{
				value = GetItemText(nRow, nCol);
				//memset(pInters->xprc, 0x00, sizeof(pInters->xprc));
				//memcpy(pInters->xprc, value, min(value.GetLength(), 10));

				pInters->xprc = value;
				if (m_bChgCnd) 
				{
					saveInterest();
				}
			}
			else
			//2011.12.20 KSJ �߰�ȣ �Ǿ� ���� �ʾƼ� �߰���.
			{
				Invalidate(false);
				break;
			}
			// KSJ			

			if (m_sonikField > colCURR)
			{
				value = CalcuPyungaSonik(pInters.get(), curr);
				SetItemText(nRow, m_sonikField, value);
			}
			
			if (m_suikField > colCURR)
			{
				value = CalcuSuik(pInters.get(), curr);
				SetItemText(nRow, m_suikField, value);
			}

 			if (CAST_TREEID(m_kind)->kind == xINTEREST)
				m_bEditWork = true;
		}

		ClearSearchMap();	//2011.12.29 KSJ 
		ReSetSearchMap();	//2011.12.29 KSJ 

		break;

	case GVNM_COLUMNMOVE:
		{
			if (nmgv->row >= m_gridHdrX.GetSize()
					|| nmgv->col >= m_gridHdrX.GetSize())
				break;
			
			_gridHdr gridHdr;
			
			gridHdr = m_gridHdrX.GetAt(nmgv->col);
			m_gridHdrX.InsertAt(nmgv->row, gridHdr);

			if (nmgv->col > nmgv->row)
				nmgv->col += 1;

			m_gridHdrX.RemoveAt(nmgv->col);
			//ReIndexSpecial();

			const int	ncnt = m_gridHdrX.GetSize();
			CString	sztmp, szFIELD = _T("");
			int nWidth{};
			m_szWIDTH = _T("");			
			m_szFIELD = _T("");
			for ( int ii = 0 ; ii < ncnt ; ii++ )
			{
				gridHdr = m_gridHdrX.GetAt(ii);				
				nWidth = GetColumnWidth(ii);
				if (ii >= colNAME)
				{
					sztmp.Format("%d,", nWidth);
					m_szWIDTH += sztmp;
				}
				if (ii > colCURR)
				{
					sztmp.Format("%s,", gridHdr.symbol);
					if (sztmp.Compare("2111,") == 0)
						sztmp.Format("2023,");
					else if (sztmp.Compare("2115,") == 0)
						sztmp.Format("2024,");
					else if (sztmp.Compare("2116,") == 0)
						sztmp.Format("2033,");
					else if (sztmp.Compare("2112,") == 0)
						sztmp.Format("2027,");

					m_szFIELD += sztmp;
				}
			}

			SaveAsInterField();
		}

		break;

	case GVNM_BEGINDRAG:
		{
			m_drag = nmgv->row; m_drop = -1;
			m_strCode = GetItemText(m_drag, colCODE);
			
			//2014.12.10 KSJ �ٸ��׷쿡�� drag&drop�Ѱ� ������ GVNM_OUTDRAG���� �����Ǵ� ���� �߻�
			//m_bOutPos �ʱ�ȭ ���ش�.
			m_bOutPos = false;

			//if (CAST_TREEID(m_kind)->kind == xISSUE)
			//	CintGrid::m_dropdata.SetCode(GetItemText(m_drag, colCODE)); 
		}
		
		break;
	case GVNM_OUTDRAG:
		{	
			if (m_bOutPos)
			{
 				DeleteRow(m_drag);

 				if (CAST_TREEID(m_kind)->kind == xINTEREST)
					m_bEditWork = true;

 				m_bOutPos = false;
			}
		}

		break;
	case GVNM_ENDEND:
		{

		}

		break;
	case GVNM_ENDDRAG:
		{		

		}
		break;
	case GVNM_SELCHANGED:
		{
			CString code, string; 
			if (nmgv->row >= GetFixedRowCount() && nmgv->col >= colNAME)
			{
				code = GetItemText(nmgv->row, colCODE);
				code.TrimLeft(); 
				code.TrimRight();

				if (code.IsEmpty()) break;
				
// 				if (((CGroupWnd*)m_pGroupWnd)->GetSelAction() == 0)
// 					break;

				switch (GetCodeType(code))
				{
				case kospiCODE:
					string.Format("%s\t%s", HCOD, code);
					 break;
				case futureCODE:
					string.Format("%s\t%s", "ed_focod", code);	//2013.01.03 KSJ 2200��������ȭ�鿡�� Ʈ���ź���
					Variant(triggerCC, string);

					string.Format("%s\t%s", FCOD, code);
					break;
				case optionCODE:
					string.Format("%s\t%s", "ed_focod", code);	//2013.01.03 KSJ 2200��������ȭ�鿡�� Ʈ���ź���
					Variant(triggerCC, string);

					string.Format("%s\t%s", OCOD, code);
					break;
				case foptionCODE:
					string.Format("%s\t%s", "ed_focod", code);	//2013.01.03 KSJ 2200��������ȭ�鿡�� Ʈ���ź���
					Variant(triggerCC, string);

					string.Format("%s\t%s", PCOD, code);
					break;
				case indexCODE:
					string.Format("%s\t%s", UCOD, code);
					break;
				case sinjuCODE:
					string.Format("%s\t%s", SINJUCOD, code);
					break;
				case elwCODE:
					string.Format("%s\t%s", HCOD, code);
					Variant(triggerCC, string);
					string.Format("%s\t%s", ELWCOD, code);
					SetFocus();
					break;
				case thirdCODE:
					string.Format("%s\t%s", THIRDCOD, code);					
					break;
				case 0:		//2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
					if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ����  //�Ļ���ǰ �ڵ尳��
					{
						int nGubn = atoi(code.Mid(1,2));
						CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ �ֽļ��� �ű����� ����
						
						if(nGubn > 10 && nGubn < 60)	//�ֽļ���
						{
							string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
							Variant(triggerCC, string);

							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if(nGubn > 60 && nGubn < 70)	//�ݸ�����
						{
							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if(nGubn > 70 && nGubn < 80)	//��ȭ����
						{
							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if(strGubn >= "B0" && strGubn <= "ZZ")	//2014.10.21 KSJ �ֽļ��� �ű����� ����
						{
							string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
							Variant(triggerCC, string);
							
							string.Format("%s\t%s", SFOD, code);
							break;
						}
					}
				}
				
//				TRACE("\n select nmgv row : %d \n", nmgv->row);
				SetFocus();
				
				break;
			}
			break;
			// Map �ʱ�ȭ KSI
			ClearSearchMap();
			ReSetSearchMap();	//2011.12.29 KSJ 
		}

	case GVNM_TRIGGER:
	case GVNM_LMOUSEDOWN:
		{
			CString code = GetItemText(nmgv->row, colCODE);
			CString string; 
			//if (nmgv->row >= GetFixedRowCount() && nmgv->col >= colNAME)
			{
				//code = GetItemText(nmgv->row, colCODE);
				code.TrimLeft(); 
				code.TrimRight();

				if (code.IsEmpty()) 
					break;
				
// 				if (((CGroupWnd*)m_pGroupWnd)->GetSelAction() == 0)
// 					break;

				switch (GetCodeType(code))
				{
				case kospiCODE:
					string.Format("%s\t%s", HCOD, code);
					 break;
				case futureCODE:
					string.Format("%s\t%s", FCOD, code);
					break;
				case optionCODE:
					string.Format("%s\t%s", OCOD, code);
					break;
				case foptionCODE:
					string.Format("%s\t%s", PCOD, code);
					break;
				case indexCODE:
					string.Format("%s\t%s", UCOD, code);
					break;
				case sinjuCODE:
					string.Format("%s\t%s", SINJUCOD, code);
					break;
				case elwCODE:
					string.Format("%s\t%s", HCOD, code);
					Variant(triggerCC, string);
//					SetFocus();
					string.Format("%s\t%s", ELWCOD, code);
					break;
				case thirdCODE:
					string.Format("%s\t%s", THIRDCOD, code);					
					break;
				case 0:		//2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
					if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ���� //�Ļ���ǰ �ڵ尳��
					{
						int nGubn = atoi(code.Mid(1,2));
						CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ �ֽļ��� �ű����� ����

						if(nGubn > 10 && nGubn < 60)	//�ֽļ���
						{
							string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
							Variant(triggerCC, string);

							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if(nGubn > 60 && nGubn < 70)	//�ݸ�����
						{
							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if(nGubn > 70 && nGubn < 80)	//��ȭ����
						{
							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if(strGubn >= "B0" && strGubn <= "ZZ")	//2014.10.21 KSJ �ֽļ��� �ű����� ����
						{
							string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300��������ȭ�鿡�� Ʈ���ź���
							Variant(triggerCC, string);
							
							string.Format("%s\t%s", SFOD, code);
							break;
						}
					}
				}
				Variant(triggerCC, string);
				Variant(codeCC, string);
	
				
//				TRACE("\nClick point is %d, %d, %s\n\n", nmgv->row, m_ClickRow, string);
 				if(nmgv != nullptr)
 				{
 					if(nmgv->row != m_ClickRow)
 					{
 						SetFocus();
 						m_ClickRow = nmgv->row;
 					}
 				}

// 				HCURSOR hCursor;
// 				hCursor = AfxGetApp()->LoadStandardCursor(IDC_IBEAM); 
// 				
// 				if(GetCursor() == hCursor)
// 				{	
// 					TRACE("\nIBEAM\n");
// 				}
// 				else
// 				{
// 					TRACE("\nARROW\n");
// 					SetFocus();
// 				}
				break;
			}
		}
		break;
	case GVNM_DBLCLICKED:
		{
			CString code, string; 
			if (nmgv->row >= GetFixedRowCount() && nmgv->col >= colNAME)
			{
				code = GetItemText(nmgv->row, colCODE);
				code.TrimLeft(); code.TrimRight();
				if (code.IsEmpty()) break;

				switch (dblCURR)//(((CGroupWnd*)m_pGroupWnd)->GetDblAction())
				{
				case dblCURR:
					switch (GetCodeType(code))
					{
					case kospiCODE:
					case thirdCODE:	//2013.08.07 ���������϶��� 2001���� ����ش�.
						string.Format("%s /S/t0/d%s\t%s", kcurrMAP, HCOD, code);
						SetView(string); break;
					case futureCODE:
						string.Format("%s /S/t0/d%s\t%s", fcurrMAP, FCOD, code);
						SetView(string); break;
					case optionCODE:
						string.Format("%s /S/t0/d%s\t%s", ocurrMAP, OCOD, code);
						SetView(string); break;
					case foptionCODE:
						string.Format("%s /S/t0/d%s\t%s", focurrMAP,PCOD, code);
						SetView(string); break;
					case indexCODE:
						string.Format("%s /S/t0/d%s\t%s", icurrMAP, UCOD, code);
						SetView(string); break;
					case elwCODE:
						string.Format("%s /S/t0/d%s\t%s", ecurrMAP, ELWCOD, code);
						SetView(string); break;
					case 0:		//2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
							if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ����//�Ļ���ǰ �ڵ尳��
							{
								int nGubn = atoi(code.Mid(1,2));
								CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ �ֽļ��� �ű����� ����

								if(nGubn > 10 && nGubn < 60)	//�ֽļ���
								{
									string.Format("%s /S/t0/d%s\t%s", sfcurrMAP, SFOD, code);
									SetView(string); break;
								}
								else if(nGubn > 60 && nGubn < 70)	//�ݸ�����
								{
									string.Format("%s /S/t0/d%s\t%s", cccurrMAP, SFOD, code);
									SetView(string); break;
								}
								else if(nGubn > 70 && nGubn < 80)	//��ȭ����
								{
									string.Format("%s /S/t0/d%s\t%s", cccurrMAP, SFOD, code);
									SetView(string); break;
								}
								else if(strGubn >= "B0" && strGubn <= "ZZ")	//2014.10.21 KSJ �ֽļ��� �ű����� ����
								{
									string.Format("%s /S/t0/d%s\t%s", sfcurrMAP, SFOD, code);
									SetView(string); break;
								}
							}
					}
					break;
				case dblCHART:
					string.Format("%s /S/t0/d%s\t%s", chartMAP, CCOD, code);
					SetView(string);
					break;				
				case dblUSER:
					{
						CString	mapname = "";//((CGroupWnd*)m_pGroupWnd)->GetLinkmap();
						if (mapname.IsEmpty())
							break;
							
						switch (GetCodeType(code))
						{
						case kospiCODE:
							string.Format("%s /S/t0/d%s\t%s", mapname, HCOD, code);
							SetView(string); break;
						case futureCODE:
							string.Format("%s /S/t0/d%s\t%s", mapname, FCOD, code);
							SetView(string); break;
						case optionCODE:
							string.Format("%s /S/t0/d%s\t%s", mapname, OCOD, code);
							SetView(string); break;
						case foptionCODE:
							string.Format("%s /S/t0/d%s\t%s", mapname, PCOD, code);
							SetView(string); break;
						case indexCODE:
							string.Format("%s /S/t0/d%s\t%s", mapname, UCOD, code);
							SetView(string); break;
						case 0:		//2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
							if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ���� //�Ļ���ǰ �ڵ尳��
							{
								int nGubn = atoi(code.Mid(1,2));
								const CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ �ֽļ��� �ű����� ����
								
								if(nGubn > 10 && nGubn < 60)	//�ֽļ���
								{
									string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
									SetView(string); break;
								}
								else if(nGubn > 60 && nGubn < 70)	//�ݸ�����
								{
									string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
									SetView(string); break;
								}
								else if(nGubn > 70 && nGubn < 80)	//��ȭ����
								{
									string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
									SetView(string); break;
								}
								else if(strGubn >= "B0" && strGubn <= "ZZ")	//2014.10.21 KSJ �ֽļ��� �ű����� ����
								{
									string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
									SetView(string); break;
								}
							}
						}
						break;
					}
				}
			}
		}
		break;
	case GVNM_ENDSORT:
		{

			m_endsort = true; 
// 			memoCheck();
// 			rebuildInterest();

// 			LONG	ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
// 			hawkeyeAction((ret) ? true : false);
			// DEL PSH 20070913
			//saveInterest();
			// END DEL

			// ADD PSH 20070913
			m_bSorting = TRUE;
			// END ADD		
			
			// Map �ʱ�ȭ KSI
			ClearSearchMap();
			ReSetSearchMap();	//2011.12.29 KSJ 
		}
		break;

	case GVNM_RMOUSEDOWN:
		// MODIFY PSH 20070912
		//RbuttonAction(nmgv->row);
		{
			if (0 < nmgv->row)
			{
				RbuttonAction(nmgv->row);
			}
		}
		// END MODIFY
		break;
	// ADD PSH 20070914
	case GVNM_CHANGEMARKER:
		{		
			SetFocus();
			CString strMarker, strMarkerCode;
			strMarker = GetItemText(nmgv->row, colNAME);

//			struct	_inters* pInter{};
			const int nMarkerRow  = nmgv->row - 1;

			strMarkerCode.Format("m%05d", nMarkerRow);

			if (!m_bSorting)
			{
				auto& pInter = m_inters.at(nMarkerRow);
				//ZeroMemory(pInter->code, sizeof(pInter->code));
				//ZeroMemory(pInter->name, sizeof(pInter->name));
				//CopyMemory(pInter->code, strMarkerCode, strMarkerCode.GetLength());
				//CopyMemory(pInter->name, strMarker, strMarker.GetLength());				

				pInter->code = strMarkerCode;
				pInter->name = strMarker;
				saveInterest();
			}
		}
		break;
	// END ADD
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

BOOL CGridWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CWnd::OnCommand(wParam, lParam);
}

/*
void CGridWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CMemDC	MemDC(&dc);
	
	OnDraw(&MemDC);

	// Do not call CGridCtrlExt::OnPaint() for painting messages
}
*/

void CGridWnd::OnSize(UINT nType, int cx, int cy) 
{
	CGridCtrlExt::OnSize(nType, cx, cy);
	
	OperResize(cx, cy);
	// TODO: Add your message handler code here
}

void CGridWnd::OnDestroy() 
{
	savecfg();
	stopRTM();

	::CloseHandle(m_hEventRTM);
	CGridCtrlExt::OnDestroy();
	// TODO: Add your message handler code here
}

void CGridWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bLBtnDown = true;
	((CGroupWnd*)m_pGroupWnd)->SendMessage(WM_LBUTTONDOWN, nFlags, MAKELPARAM(point.x, point.y));

	CGridCtrlExt::OnLButtonDown(nFlags, point);
}

void CGridWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_bLBtnDown)
	{
		m_bSelect = true;
		((CGroupWnd*)m_pGroupWnd)->SendMessage(WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));
	}

	m_bLBtnDown = false;
	CGridCtrlExt::OnLButtonUp(nFlags, point);
}

void CGridWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	m_bKeyDown = true;
	CGridCtrlExt::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGridWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	const int row = m_idCurrentCell.row;
	if(row == 0) 
		return;

	if(m_bKeyDown)
	{
 		if(nChar == VK_DELETE)	//delete
		{
			m_bEditWork = true;	
			if (gsl::narrow_cast<int>(m_inters.size()) >= row -1)
			{
				if (!(GetItemAttr(row, colNAME) & GVAT_MARKER))
				{
					DeleteRow(row);	
					insertInterest(MAX_LINE - 1);
				}
				// END MODIFY
			}
		}
		else if(nChar == VK_INSERT)	//insert
		{
			m_bEditWork = true;
			
			int rowcount = 0;
			rowcount = GetRowCount();
			
			if(rowcount >= MAX_LINE)
			{
				Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
				return;
			}
			
			insertInterest(row - 1);
			insertRow(row);
			SetFocusCellEdit(row, colNAME, true);
			
			if (m_bAddCnd) 
				saveInterest();
		}

		m_bKeyDown = false;

		Refresh();
	}

	CGridCtrlExt::OnKeyUp(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// CGridWnd message handlers
LRESULT CGridWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:	break;
	case DLL_OUB:	break;
	case DLL_ALERTx:
		{
			if (m_bRTMReady)
			{
				m_syncRTM.Lock();

				parsingAlertx(lParam);

				m_syncRTM.Unlock();
			}
		}
		break;
	case DLL_TRIGGER:	break;
	case DLL_DOMINO:	break;
	case DLL_NOTICE:	break;
	case DLL_DROP:		break;
	case DLL_AXIS:		break;
	case DLL_EXCEL:	break;
	case DLL_INVOKE:	break;
	case DLL_SETPAL:	break;
	case DLL_SETFONT:	break;
	case DLL_SETFCB:	break;
	case DLL_SETFONTx:	break;
	case DLL_SELECT:	break;
	case DLL_ACCOUNT:	break;
	case DLL_GUIDE:	break;
	}
	return TRUE;
}

LONG CGridWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;

	CString strTemp;

	switch (LOWORD(wParam))
	{
	case MK_SISECATCH:
		//OperAlarm(HIWORD(wParam), (char*)lParam);
		break;
	case MK_GETDATAKIND:
		ret = GetKind();
		break;
	case MK_SELGROUP:
		//InvalidateRect(m_rcTitle, FALSE);
		break;
	case MK_CLICKGROUP:
		(CGroupWnd*)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)m_nGridIndex);
		break;
	case MK_SENDTR:
		sendTransaction();
		break;
	case MK_RECVDATA:
		RecvOper(HIWORD(wParam), (CRecvData*)lParam);	
		break;
	case MK_ENDDRAG:
		m_drag = m_drop = -1;
// 		FreeDragDrop();
// 		CintGrid::m_dropdata.Reset();
		break;
	case MK_INSERTROW:
		m_bEditWork = true;
		ret = insertRow(HIWORD(wParam));
		break;
	case MK_INSERTCODE:
		m_bEditWork = true;
		InsertRowNCode(HIWORD(wParam));
		break;
	case MK_DELETEROW:
		DeleteRow(HIWORD(wParam));
		// DEL PSH 20070918
		//insertRow(MAX_LINE);
		//saveInterest();
		// END DEL
		insertInterest(MAX_LINE - 1);

		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = true;

		break;
 	case MK_TRIGGEROW:
 		//triggerCode(HIWORD(wParam));
 		break;
	case MK_SAVE:
		// MODIFY PSH 20070918
		saveInterest();
		// END MODIFY
		break;
	case MK_SORTBASE:
		{
			//BaseSorting();
		}
		break;
	// END ADD
	default:
		//ret = ::OnManage(wParam, lParam);
		break;

	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
void CGridWnd::Init()
{

}

void CGridWnd::SetGridHeader(CString strHeader)
{
	if (!GetSafeHwnd()) return;
	
	CGridCellBase *phead = GetDefaultCell(TRUE, FALSE);
	phead->SetBackClr(GetIndexColor(COLOR_GRIDHEAD));
	phead->SetTextClr(GetIndexColor(COLOR_GRIDHEADTXT));
	phead->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

	const int gridHdrC = m_gridHdrX.GetSize();

	SetColumnCount(gridHdrC);

	for(int ii = 0; ii < gridHdrC; ii++)
	{
		CGridCell *pCell = (CGridCell*)(GetCell(0, ii));
		if (pCell)
		{
			GVITEM item;
			item.row = 0;
			item.col = ii;
			item.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_SYMBOL|GVMK_ATTR;
			item.format = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			item.attr = m_gridHdrX[ii].attrx;
			item.text = IH::idTOstring(m_gridHdrX[ii].stid);
			item.symbol = m_gridHdrX[ii].symbol;
			SetItem(&item);
		}

		SetColumnWidth(ii, m_gridHdrX[ii].width);
		//m_gridHdrX.Add(gridHdr[ii]);
	}
	SetColumnResize();
	Refresh();
}

void CGridWnd::GetGridHeader(int nIndex, _gridHdr &gHdr)
{
	if(nIndex > GetColumnCount()) return;

	gHdr = m_gridHdrX.GetAt(nIndex);
}

bool CGridWnd::IsDuplicateCode(CString code, int nrow, bool drag )
{
	CString string;
	const int nMax =GetRowCount();
	
	for (int ii = 1; ii < nMax/*m_rowC*/; ii++)
	{
		if (!drag)
		{
			if (ii == nrow) continue;
		}
		
		string = GetItemText(ii, colCODE);
		if (code == string)
			return true;
	}
	
	return false;
}

void CGridWnd::ReSetSearchMap()
{
	class CIndexMap* idx = nullptr;
	
	const int realtimeCol = 0; 
	CString string;
	
	int count = 0, ii = 0;
	for (count = 0, ii = 1; ii < GetRowCount(); ii++)
	{
		string = GetItemText(ii, realtimeCol);
		m_irowCode[count++] = ii;
		
		string.TrimLeft(); string.TrimRight();
		if (string.IsEmpty()) continue;

		// map�� �ߺ� ������ üũ
		if (!m_pSearchMap.Lookup(string, (CObject*&)idx))
		{
			auto& item = _vIndex.emplace_back(std::make_unique<CIndexMap>());
			idx = item.get();
		}


		if (idx->idxCnt >= maxIDX)
			continue;
		
		idx->index[idx->idxCnt++] = ii;
		m_pSearchMap.SetAt(string, (CObject*)idx);		
	}
}

void CGridWnd::ClearSearchMap()
{
	m_pSearchMap.RemoveAll();
	_vIndex.clear();
	_mapFms.clear();
}

void CGridWnd::CopyInter(_inters* pCpy, _inters* pSrc)
{
	memcpy(pCpy->gubn,		pSrc->gubn,	sizeof(pSrc->gubn));
	memcpy(pCpy->code,		pSrc->code,	sizeof(pSrc->code));
	memcpy(pCpy->name,		pSrc->name,	sizeof(pSrc->name));
	memcpy(pCpy->xprc,		pSrc->xprc,	sizeof(pSrc->xprc));
	memcpy(pCpy->xnum,		pSrc->xnum,	sizeof(pSrc->xnum));
	memcpy(pCpy->xupnum,	pSrc->xupnum, sizeof(pSrc->xupnum));
	memcpy(pCpy->bookmark,	pSrc->bookmark, sizeof(pSrc->bookmark));
	memcpy(pCpy->filler,	pSrc->filler, sizeof(pSrc->filler));
}

void CGridWnd::RemoveAllBaseInters()
{
	m_arrBaseInters.clear();
}
// END ADD

int CGridWnd::AddInters(std::shared_ptr<_intersx> pinters)
{ 
	if (!pinters->code.IsEmpty())
		m_mapCode.SetAt(pinters->code, pinters->name);
	
	m_inters.push_back(pinters);
	return m_inters.size();
}


void CGridWnd::RemoveInters(int nIndex)
{
	if(nIndex == -1) return;	//2013.08.07 KSJ -1�̸� ����
	
	auto& pinters = m_inters.at(nIndex);
	if (!pinters->code.IsEmpty())
	{
		// ADD PSH 20070913	2013.06.03 KSJ m_arrBaseInters�� ���� ������ �ʿ䰡 ����.
		if (!m_bSorting)
		{
			const int nRowCnt = m_arrBaseInters.size();			
			for (int nRow = 0; nRow < nRowCnt; nRow++)
			{
				auto& pInter = m_arrBaseInters.at(nRow);
				
				if ((pInter) && pInter->code.CompareNoCase(pinters->code) == 0)
				{
					m_arrBaseInters.erase(m_arrBaseInters.begin() + nRow);	//2013.06.03 nIndex�� ����°� �ƴ϶� nRow�� ���쵵�� ����
					//delete pInter;	
					break;
				}
			}
		}
		// 2013.06.03 KSJ m_arrBaseInters�� ���� ������ �ʿ䰡 ����. END
		
		m_mapMargin.RemoveKey(pinters->code);
		// END ADD
		
		m_mapCode.RemoveKey(pinters->code);
	}
	
	m_inters.erase(m_inters.begin()+ nIndex);
///	delete pinters;
	
	insertInterest(-1);
}

void CGridWnd::insertInterest(int idx)
{
	if (idx < 0)
		auto& pinters = m_inters.emplace_back(std::make_shared<_intersx>());
	else
		auto& pinters = m_inters.emplace(m_inters.begin() + idx, std::make_shared<_intersx>());
}

void CGridWnd::RbuttonAction(int row)
{
	if (row == 0)	return;

	const	int userBASE = 100;
	const	int menuBASE = 200;

	SetSelectRow(row);

	CPoint	point; 
	GetCursorPos(&point);

	int	index = -1, offs{};
	char* ptr{}, wb[1024]{};
	CStringArray items; items.RemoveAll();
	CString string, path, domino, mapN = _T("IB202200");

	CString	code;

	{
		auto popM = std::make_unique<CMenu>(); 
		popM->CreatePopupMenu();

		path.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, "IB20");

		DWORD	rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		if (rc <= 0)
		{
			path.Format("%s/%s/%s", m_root, MTBLDIR, "IB20");
			rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		}

		ptr = wb;
		for ( ; rc > 0; ptr++, rc--)
		{
			string = ptr;
			ptr   += string.GetLength();
			rc    -= string.GetLength();
			if (string.GetLength() <= L_MAPN)
			{
				if (string.Find('-') == 0)
					popM->AppendMenu(MF_SEPARATOR, 0);
				continue;
			}
			mapN   = string.Left(L_MAPN);
			string = string.Mid(L_MAPN);
			offs   = string.Find('=');
			if (offs != -1)
				string = string.Mid(offs+1);

			index = items.Add(mapN);
			popM->AppendMenu(MF_STRING|MF_ENABLED, menuBASE+index, string);
		}

		if (index >= 0)
		{
			index++;
			popM->AppendMenu(MF_SEPARATOR, 0);
		}

		code = GetItemText(row, colCODE); code.TrimRight();

		popM->AppendMenu(MF_STRING, userBASE+0, "���� �߰�<INSERT>");
		popM->AppendMenu(MF_STRING, userBASE+1, "���� ����<DELETE>");
	
		// MODIFY PSH 20070914
		popM->AppendMenu(MF_STRING, userBASE+15, "�����߰�");
		popM->AppendMenu(MF_STRING, userBASE+9, "å���� �߰�");
		popM->AppendMenu(MF_STRING, userBASE+10, "å���� ����");
		// END MODIFY
		index = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, point.x, point.y, m_pParent);
	//	delete popM;
	}

	domino = _T("");
	if (!code.IsEmpty())
	{
		switch (GetCodeType(code))
		{
		case kospiCODE:		domino.Format("%s\t%s", HCOD, code);	break;
		case futureCODE:	domino.Format("%s\t%s", FCOD, code);	break;
		case optionCODE:	domino.Format("%s\t%s", OCOD, code);	break;
		case foptionCODE:	domino.Format("%s\t%s", PCOD, code);	break;
		case indexCODE:		domino.Format("%s\t%s", UCOD, code);	break;
		case elwCODE:		domino.Format("%s\t%s", HCOD, code);	break;
		case 0:		//2012.10.11 KSJ �ֽ�, �ݸ�, ��ȭ���� �߰�
			if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //�ֽ�, �ݸ�, ��ȭ���� //�Ļ���ǰ �ڵ尳��
			{
				int nGubn = atoi(code.Mid(1,2));
				CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ �ֽļ��� �ű����� ����
				
				if(nGubn > 10 && nGubn < 60)	//�ֽļ���
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
				else if(nGubn > 60 && nGubn < 70)	//�ݸ�����
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
				else if(nGubn > 70 && nGubn < 80)	//��ȭ����
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
				else if(strGubn >= "B0" && strGubn <= "ZZ")	//2014.10.21 KSJ �ֽļ��� �ű����� ����
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
			}
		}
	}

	if (index >= menuBASE)
	{
		index -= menuBASE;
		if (index < items.GetSize())
		{
			string = items.GetAt(index);
			string += "/s/t0/d";
			string += domino;
			SetView(string);
		}
		items.RemoveAll();
		return;
	}
	items.RemoveAll();

	switch (index)
	{
	case userBASE+0:	// �߰�
		{	
			m_bEditWork = true;

			int rowcount = 0;
			rowcount = GetRowCount();
			
			if(rowcount >= MAX_LINE)
			{
				Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
				return;
			}

			insertInterest(row - 1);
			insertRow(row);
			SetFocusCellEdit(row, colNAME, true);

			if (m_bAddCnd) 
			{	
				saveInterest();
			}

		}
		break;
	case userBASE+1:	// ����
		{
			m_bEditWork = true;

			if (gsl::narrow_cast<int>(m_inters.size()) >= row -1)
			{
				if (!(GetItemAttr(row, colNAME) & GVAT_MARKER))
				{
					DeleteRow(row);	
					insertInterest(MAX_LINE - 1);
				}
				// END MODIFY
			}
		}
		break;
	case userBASE+15://���� �߰�
		{
			m_bEditWork = true;

			int rowcount = 0;
			rowcount = GetRowCount();
			
			if(rowcount >= MAX_LINE)
			{
				Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
				return;
			}
			
			insertInterest(row - 1);
			insertRow(row);
			
			if (m_bAddCnd) 
			{
				saveInterest();
			}
		}
		break;
	case userBASE+9:// å���� �߰�
		{	
			int rowcount = 0;		
			rowcount = GetRowCount();

			if(rowcount >= MAX_LINE)
			{
				Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
				break;
			}

			insertInterest(row - 1);
			insertRow(row);

			const UINT attr = GetItemAttr(row, colNAME) | GVAT_MARKER;
			SetItemAttr(row, colNAME, attr);

			auto& pInter = m_inters.at(row - 1);

			CString strMarker;
			strMarker.Format("m%05d", row);

			if (pInter)
			{
				pInter->gubn = 'm';
				pInter->code = strMarker; 
			}

			SetItemText(row, colCODE, strMarker);
			
			SetFocusCellEdit(row, colNAME, true);

			if (m_bAddCnd) 
			{
				saveInterest();
			}
		}
		break;
	case userBASE+10:	// å���ǻ���
		{
			if (gsl::narrow_cast<int>(m_inters.size()) >= row -1)
			{
				if (GetItemAttr(row, colNAME) & GVAT_MARKER)
				{
					DeleteRow(row);	
					insertInterest(MAX_LINE - 1);
				}
			}
		}
		break;

	// END ADD
	}
}

BOOL CGridWnd::Create(const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle, DWORD dwExStyle)
{
	m_pGroupWnd = parent;
	
	m_pMainWnd = ((CGroupWnd*)m_pGroupWnd)->GetMainWnd();
	m_fileCFG = ((CMainWnd*)m_pMainWnd)->GetFileCfg();
	m_strTag = ((CMainWnd*)m_pMainWnd)->GetStrTag();

	m_rcRect = rect;

	//m_rcRect.right -= SIZE_GAPS;

	//GRID
	const BOOL bRet = CGridCtrl::Create(m_rcRect, parent, nID, dwStyle, dwExStyle);

	m_root = Variant(homeCC, "");
	m_user = Variant(nameCC, "");

	loadcfg();

	LOGFONT	lf;
	CAxisExt::m_pFont->GetLogFont(&lf);
	struct _fontR fontR;
	fontR.bold = FW_BOLD;
	fontR.italic = FALSE;
	fontR.point = ((CMainWnd*)m_pMainWnd)->GetFontSize();
	fontR.name = lf.lfFaceName;
	m_pFontB = GetAxFont(&fontR);
	lf.lfHeight = fontR.point * 10;

	SetHomeStr(Variant(homeCC, ""));
	SetFontEx(CAxisExt::m_pFont);
	SetHistoryStr(Variant(historyCC, HCOD));

	SetLogFont(lf);

	EnableTitleTips(FALSE);
	SetColumnResize(FALSE);
	EnableSelection(TRUE);
	SetSingleRowSelection(TRUE);
	SetDoubleBuffering(TRUE);
	SetEditable(FALSE);
	SetFrameFocusCell(FALSE);
	SetDefCellWidth(50);
	SetDefCellHeight(GRID_CELL_HEIGHT);
	SetDefCellMargin(1);
	
	SetRowCount(2);
	SetFixedColumnCount(0);
	SetFixedRowCount(1);

	FieldSetup(false);

	SetGridBkColor(GetIndexColor(68));	//�����������
	SetBkSelColor(GetIndexColor(78));	//�����ѻ�

	SetGridFocusLine(FALSE, TRUE);	//2016.06.30 KSJ �׸��� focus = true �̸� ���� �簢�� �ڽ� �׷���
	SetStepColor(1, GetIndexColor(68), GetIndexColor(77));
	SetGubnColor(m_clrKospi, m_clrKosdaq);

	return bRet;
}

void CGridWnd::OperResize(int cx, int cy)
{	
	ResetScrollBars();
	Refresh();
}

void CGridWnd::SetKind(UINT kind)
{
	m_kind = kind;
/*
	m_szTitle.Format(" %s", (char*)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, kind));
	
	if (kind != 0)
		((CMainWnd*)m_pMainWnd)->SetRTSTreeData(this);
	
	InvalidateRect(m_rcTitle, FALSE);
	SetSortColumn(-1);
*/
}

void CGridWnd::ClearInterest()
{
	m_inters.clear();
}

void CGridWnd::RemoveAll()
{
	ClearInterest();
	for (int ii = 0; ii < MAX_LINE; ii++)
	{
		auto& pinters = m_inters.emplace_back(std::make_shared<struct _intersx>());
		pinters->gubn = '0';	
	}
	
	// ADD PSH 20070917
	int nRow = GetRowCount() - 1;
	for (; nRow > 1; nRow--)
	{
		DeleteRow(nRow);
	}
	
	for (int i = 0; i < m_colC; i++)
	{
		SetItemText(1, i, "");
		SetItemData(1, i, 0);
	}

	m_rowC = 2;
	m_nGroupIndex = -1;	//2016.05.03 KSJ �׷��ε����� �ʱ�ȭ��.
	// END ADD
	
	//2012.03.22 KSJ �����׷��� �ٸ� ȭ�鿡�� ��ﶧ ù��° �ٿ��� �ǽð� �����͸� �޾���.
	ClearSearchMap();	//2011.12.29 KSJ 
	ReSetSearchMap();	//2011.12.29 KSJ 
}

int CGridWnd::GetCodeType(CString code)
{
	const int codeL = code.GetLength();
	if (codeL <= 0) return 0;
	
	const int result = m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);
	
	switch (result)
	{
	case kospiType:				// kospi code
	case kosdaqType:			// kosdaq code
	case etfType:				// etf code
	case etnType:				//2014.10.28 KSJ ETN �߰�
		return	kospiCODE;;
	case elwType:				// ELW
		return	elwCODE;
	case futureType:
		return	futureCODE;		// ����
	case sfutureType:
		return sfCODE;		
	case callType:				// call option
	case putType:				// put  option
		return  optionCODE;
	case indexType:				// ����
		return	indexCODE;
	case thirdType:				// ��3����
		return	thirdCODE;
	case sinjuType:				// �����μ���
		return	sinjuCODE;
	case mufundType:			// ������ݵ�
	case reitsType:				// ����
	case hyfundType:			// �����ϵ��ݵ�
		return 200;
	}
	
	return 0;
}

CString CGridWnd::GetcolName(int index)
{ 
	return "";//GetItemText(index + 1, colNAME); 
}


void CGridWnd::GetData(CArray <_inters*, _inters*>& arData)
{
	/*const int ncnt = arData.GetSize();
	const int nTot = m_inters.size();
	int nMax = 0;
	
	struct _intersx*	pInter1 = nullptr;
	struct _intersx*	pInter2 = nullptr;
	
	if ( ncnt >= nTot ) nMax = nTot;
	else nMax = ncnt;
	
	for ( int ii = 0 ; ii < nMax ; ii++ )
	{
		pInter1 = arData.GetAt(ii);
		pInter2 = m_inters.GetAt(ii);
		memcpy(pInter1, pInter2, sizeof(struct _inters));
	}*/
}

void CGridWnd::GetData(CArray <_inters*, _inters*>& arData, int* nRowCount)
{
	//const int ncnt = arData.GetSize();
	//const int nTot = m_inters.GetSize();
	//int nMax = 0;
	//char tCode[12]{};
	//int rCnt = 0;
	//
	//struct _inters*	pInter1 = nullptr;
	//struct _inters*	pInter2 = nullptr;
	//
	//if ( ncnt >= nTot ) nMax = nTot;
	//else nMax = ncnt;
	//
	//for (int ii = 0 ; ii < nMax ; ii++ )
	//{
	//	pInter1 = arData.GetAt(ii);
	//	pInter2 = m_inters.GetAt(ii);
	//	
	//	ZeroMemory(tCode, sizeof(tCode));
	//	CopyMemory(tCode, m_inters.GetAt(ii)->code, sizeof(tCode));
	//	
	//	if(CString(tCode).IsEmpty() == false)
	//	{
	//		rCnt++;
	//	}
	//	
	//	memcpy(pInter1, pInter2, sizeof(struct _inters));
	//}
	//
	//*nRowCount = rCnt;
}

void CGridWnd::AddData(int nIndex, CString sztmp)
{
	BOOL	bInt = FALSE;
	CString	code, amount, price, bookmark;
	
	// ADD PSH 20070913
	CString strName;
	
	if (0 == nIndex)
	{
		const UINT attr = GetItemAttr(nIndex , colNAME & ~GVAT_MARKER);
		SetItemAttr(nIndex, colNAME, attr);
	}
	else
	{
		insertRow(nIndex);	
	}
	// END ADD
	
	code = IH::Parser(sztmp, PTAB);
	
	code.TrimLeft(); code.TrimRight();
	
	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		bInt = TRUE;
		amount = IH::Parser(sztmp, PTAB);
		price = IH::Parser(sztmp, PTAB);
		// ADD PSH 20070914
		strName = IH::Parser(sztmp, PTAB);
		bookmark = IH::Parser(sztmp, PTAB);
		// END ADD
	}
	
	if (!bInt && code.IsEmpty())
		return;
	
	auto& pinters = m_inters.at(nIndex);
	pinters->gubn = '0';	
	pinters->code = code;
	
	if (bInt)
	{
		pinters->xnum = amount;
		pinters->xprc = price;
		pinters->name = strName;
		pinters->bookmark = bookmark[0];
	}
	
	// ADD PSH 20070914
	if (!code.IsEmpty() && code.GetAt(0) == 'm')
	{
		pinters->gubn = 'm';
		pinters->name = strName;
		
		const UINT attr = GetItemAttr(nIndex + 1, colNAME);
		SetItemAttr(nIndex +1, colNAME, attr|GVAT_MARKER);
		SetItemText(nIndex +1, colNAME, strName);
		
		return;
	}
	else
	{
		SetItemAttr(nIndex + 1, colNAME, GetItemAttr(nIndex+1, colNAME)&~GVAT_MARKER);
	}
	// END ADD
}

//2013.01.22 KSJ �Է��ڵ��� m_inters������ ��ġ ã��
int CGridWnd::GetInter(CString code)
{	
	for(int i=0;i< gsl::narrow_cast<int>(m_inters.size());i++)
	{
		auto& pInter = m_inters.at(i);
		
		if (pInter->code.CompareNoCase(code) == 0)	
		{
			return i;
		}
		//2013.08.12 KSJ �����μ��� ���� ���� ���� ��ȸ�� �ǽð����� �����ڵ带 �޾ƿö� J, K, ���� �ڵ尡 �Ⱥپ� ������
		//�����������Ͽ��� �پ��ֱ� ������ ���� �������� ���ڿ��˻��� �ѹ� �� �˻����ش�.
		else if(strstr(pInter->code.GetString(), code.GetString()) != nullptr)
		{
			return i;
		}
	}	
	return -1;
}

void CGridWnd::SetMarketColor(BOOL bApply, COLORREF clrKospi, COLORREF clrKosdaq)
{
	m_bMKClrApply = bApply;
	m_clrKospi    = clrKospi;
	m_clrKosdaq   = clrKosdaq;

	SetGubnColor(clrKospi, clrKosdaq);
}

int CGridWnd::GetGridWidth()
{	
	int childWidth = 0;

	for(int ii=0 ; ii<GetColumnCount() ; ii++)
	{
		childWidth += GetColumnWidth(ii);
	}

	return childWidth;
}

int CGridWnd::GetWidth()
{
	int width;
	CRect rc;
	GetWindowRect(rc);
	
	width = rc.Width();
	
	if(width <= 0)
	{
		width = m_nGridWidth;
	}
	
	return width;
}

CString CGridWnd::GetItemSymbol(int nRow, int nCol)
{
	return GetCell(nRow, nCol)->GetSymbol();
}

void CGridWnd::sendTransactionTR(int update,int nStart,int nEnd)
{
	int	sendL = 0;
	char	tempB[64]{};
	const int bufSize = sizeof(struct _gridHi) + 50 + m_gridHdrX.GetSize()*5 + m_inters.size()*12 + 12;
	std::string sendB;	
	sendB.resize(bufSize, ' ');

	

	m_bAutoExpect = true;

	if(m_bAutoExpect)
	{
		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
	}
	else
	{
		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 0, P_TAB);
	}

	CopyMemory(&sendB[sendL], tempB, strlen(tempB));	
	sendL += strlen(tempB);

	//2011.12.08 KSJ 2013.08.28 KSJ ���� �Ϲݽü��� ���͸��Ǽ� �������� ������ ���� ���� �ʿ� ����.
// 	sprintf(tempB, "%s%c%d%c", gFILTERING, P_DELI, 1, P_TAB);
// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));	
// 	sendL += strlen(tempB);
	//

	sprintf(tempB, "%s%c", gSYMBOL, P_DELI);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));	
	sendL += strlen(tempB);

	struct	_gridHi* gridHi;

	gridHi = (struct _gridHi *) &sendB[sendL];
	sendL += sizeof(struct _gridHi);

	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", m_inters.size());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir  = '1';
	gridHi->sort = '0';
	
	//////////////////////////////////////
	//struct	_gridHdr gridHdr;
	for (int ii = nStart ; ii < nEnd ; ii++)
	{
		//gridHdr = m_gridHdrX.GetAt(ii);
		
		//sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
		strcpy(tempB,(LPTSTR)(LPCTSTR)m_pGridArray.GetAt(ii));
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}
	
	sprintf(tempB, "2321%c", P_NEW);						//���ϰŷ���
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2950%c", P_NEW);						//2012.06.19 KSJ �������
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2951%c", P_NEW);						//2012.06.19 KSJ ��������
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", CCOD, P_DELI);					// �����ڵ�
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);
	
	struct _inters* pinters{};
	int	ncnt = 0;
	const int nInterCnt = m_inters.size();
	CString tempStr;
	const int pos = -1;

	for (int ii = 0; ii < nInterCnt; ii++)
	{
		auto& pinters = m_inters.at(ii);
		
		if (!pinters->code.IsEmpty())
		{
			tempStr= CString(pinters->code).Trim();

			//pos = tempStr.Find(" ");
			//if(pos != -1)
			//{
			//	strcpy(pinters->code, tempStr.Mid(0, pos));
			//}
			
			ncnt++;
		}
		
		sprintf(tempB, "%s%c", strlen(pinters->code) <= 0 ? " " : pinters->code, P_DELI);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sendB[sendL] = P_NEW; 
	sendL += 1;
	sendB[sendL] = P_TAB; 
	sendL += 1;
	sendB[sendL] = 0x00;

	CSendData	sdata;
	char key{};
	_trkey* trkey = (struct _trkey*)&key;

	trkey->group = m_nGridIndex;

	if (m_staticUpdate < 0)
	{
		trkey->kind = TRKEY_GRIDNEW;
	}
	else if(m_staticUpdate >= 0 && m_staticUpdate < 200)
	{
		trkey->kind = TRKEY_GRIDROW;
	}

	m_updateROW = m_staticUpdate;
	

	SendTR("pooppoop", std::byte(US_PASS), sendB.data(), sendL, key);
	m_endsort = false;
	
}

void CGridWnd::sendTransaction(int update)
{
	m_pGridArray.RemoveAll();
	m_staticUpdate = update;
	
	char tempB[64]{};
	
	//////////////////////////////////////
	struct	_gridHdr gridHdr {};
	for (int ii = 0 ; ii < m_gridHdrX.GetSize() ; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);
		
		sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
		m_pGridArray.Add(tempB);
	}
	//////////////////////////////////////
	// 	CWnd* m_pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	// 	m_pWnd->SendMessage(WM_MANAGE, MK_GETTREEITEM);
	
	if(m_pGridArray.GetSize() > GRIDMAXNUM)
	{	
		if(!m_bContinue && !m_bSecond)
		{
			m_bContinue = TRUE;
			
			sendTransactionTR(m_staticUpdate,0,GRIDMAXNUM);

			//m_bContinue = FALSE;
		}
	}
	else
	{
		m_bContinue = FALSE;
		
		sendTransactionTR(m_staticUpdate,0,m_pGridArray.GetSize());
	}
}

void CGridWnd::SaveAsInterField()
{
	if (m_szFIELD.IsEmpty())
		return;
	
	CString tempFile; tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, "interfield.ini");
	
	CFileFind finder;
	if (!finder.FindFile(tempFile))
		return;
	
	const int nIndex = (int)(CGroupWnd*)m_pGroupWnd->SendMessage(WM_MANAGE, MK_GETAFIELD);
	if (nIndex < 0) return;
	
	int	readL = 0;
	char	readB[1024];
	CString	sField, sNewField, sFieldName, sFieldNo;
	
	sFieldNo.Format("%02d", nIndex);
	
	readL  = GetPrivateProfileString("FIELDS", sFieldNo, "", readB, sizeof(readB), tempFile);
	sField = CString(readB, readL);	
	const int index = sField.Find(deliMITER);
	if (index == -1)	return;
	
	sFieldName = sField.Left(index);
	sField.Format("%s", m_szFIELD);
	sField.Replace(",", deliMITER);
	sNewField.Format("%s%s%s", sFieldName, deliMITER, sField);
	
	WritePrivateProfileString("FIELDS", sFieldNo, sNewField, tempFile);
}

//2013.04.30 bResize SendGrid���� ȣ��ɶ��� ������¡ ���� �ʴ´�.
void CGridWnd::FieldSetup(bool bDlgSetup, int iEqualizer, bool bResize)
{
	const int nIndex = 0;
	m_bongField = -1;
	m_sonikField = -1;
	m_suikField = -1;
	m_yDayVolField = -1;
	m_recomDataField = -1;
	m_recomBasicCostField = -1;
	m_recomSuikrateField = -1;
	m_recomBMjisuField = -1;	
	m_recomBMSuikrateField = -1;
	m_recomgoalCostField = -1;
	m_recomJisuGubnField = -1;
	m_upjongCode = -1;		
	m_EqualizerField = -1;
	m_beforeVolField = -1;
	m_newCurrField = -1;
	
	m_ccField = false;
	m_posField = false;
	
// 	Clear();
	
	loadField(nIndex, bDlgSetup, iEqualizer);
	
// 	initialGrid(bResize);
// 	SetGridinfo(TRUE);

	MarkerSetup();

	SetGridHeader("");
}


void CGridWnd::MarkerSetup()
{
	const int nCnt = m_inters.size();
	
	for (int nRow = 1; nRow < nCnt; nRow++)
	{
		auto& pInter = m_inters.at(nRow - 1);	
		if (pInter)
		{
			UINT attr{};
			
			if ('m' == pInter->gubn)
			{
				attr = GetItemAttr(nRow, colNAME) | GVAT_MARKER;
			}
			else
			{
				attr = GetItemAttr(nRow, colNAME) & ~GVAT_MARKER;
			}		
			SetItemAttr(nRow, colNAME, attr);
		}
	}
}

BOOL CGridWnd::IsValidCode(CString strCode)
{
	BOOL bFind = FALSE;

	for (int i = 1; i < GetRowCount(); i++)
	{
		CString strGridCode(GetItemText(i, 0));
		
		if (strGridCode.GetLength() == 7)
			strGridCode.Delete(0);
		
		if (strGridCode == strCode)
		{
			bFind = TRUE;
			break;
		}
	}
	
	return bFind;
}

int  CGridWnd::loadcfg_data()
{
	const int result = GetPrivateProfileInt(m_section, "DATA", 0, m_fileCFG);
	
	return result;
}

int CGridWnd::CheckRealTimeCode(CString code)
{
	class CIndexMap* idx = nullptr;

	const int realtimeCol = 0; 
	CString string;

	const int count = 0;
	int ii = 0;
	for (ii = 0; ii < 100 ; ii++)	//2012.09.10 KSJ 200 -> 100���� ����
		m_irowCode[ii] = 0;

	// �ӵ����� Search ��� ����

	if (m_pSearchMap.GetCount() > 0)
	{
		if (!m_pSearchMap.Lookup(code, (CObject*&)idx))
			return count;


		for (ii = 0; ii < idx->idxCnt; ii++)
			m_irowCode[ii] = idx->index[ii];
		return idx->idxCnt;
	}
	return count;

}

//////////////////////////////////////////////////////////////////////////


void CGridWnd::parsingAlertx(LPARAM lParam)
{
	int xrow{};
	CString code, strCode, strGubn, strKey;
	CMap<int, int, CString, CString>* mapLine{};	//�Ѷ��ο��� �����κ� �����Ѵ�.

	struct _alertR* alertR = (struct _alertR*)lParam;
	code = alertR->code;

	if (code.CompareNoCase("S0000") == 0)
		return;	//2012.09.11 KSJ �����϶��� �Ʒ����� Ż�ʿ� ����.

	const int count = CheckRealTimeCode(code);
	if (count == 0)
		return;

	if (code.GetLength() == 7)
	{
		strCode = code.Mid(1);
	}
	else
		strCode = code;

	DWORD* data = (DWORD*)alertR->ptr[0];
	//2012.02.09 KSJ
	//data[0]�� ������ ȣ��, ü������ ������.
	//ȣ���϶��� alertR->size -1�� ���� �ѹ��� �������ָ� �ȴ�.
	
	if (data[0])
	{
		strGubn = (char*)data[0]; 
		strGubn.Trim();

		//2016.05.20 KSJ �ֽ�, ����, ������ �����Ͽ� �ǽð� �޵���.
		//�����ڵ� ���̰� 6�̸� �ֽ�.
		//�ֽĽü�, �ֽ�ü��, �ֽ�ȣ��
		if ((strCode.GetLength() == 6) && (!strGubn.Compare("A") || !strGubn.Compare("B") || (!strGubn.Compare("D") && data[111])))	//ȣ���̸鼭 111�� ��������
		{
			//�ֽ�
		}
		else if ((strCode.GetLength() == 8) && (!strGubn.Compare("K") || !strGubn.Compare("O") || !strGubn.Compare("o") || !strGubn.Compare("f") ||
			!strGubn.Compare("L") || !strGubn.Compare("4") || !strGubn.Compare("P") || !strGubn.Compare("g")))
		{
			//����
		}
		else if ((strCode.GetLength() == 5) && (!strGubn.Compare("X")))
		{
			//����
			//2013.08.26 KSJ	�����϶� ���� ǥ��
			/*
				08:30~09:00 'X' ����ǥ��
				09:01~14:50 'J' ���簡ǥ��
				14:50~15:00 'X' ����ǥ��
				15:00~15:01 'J' ���簡ǥ��
			*/
			code.Delete(0);	//ù ���� 'X'�� �����Ѵ�.
			code.Insert(0, 'K'); //ù ���ڸ� 'K'�� �ٲ��ش�.
			strCode = code;
			//2013.08.26 KSJ END
		}

		strKey = code /*+ strGubn*/;

		auto& mt = _mapFms.emplace(std::make_pair(strKey, std::map<int, CString>()));

		auto fms = mt.first;
		auto& mapLine = fms->second;

		xrow = m_irowCode[0];
		
		CString	oldEXP = GetItemText(xrow, colEXPECT);
		CString	newEXP = _T("");
		
		
		CString entry, symbol, strData;
		BOOL	bTransSymbol = FALSE;			//2012.02.09 KSJ �ɺ�����Ǵ� ��Ȳ �϶� TRUE
		BOOL	bDaebi = FALSE;				//2012.03.20 KSJ ������ 0�϶� üũ��.
		BOOL	bZisu = FALSE;				//2013.08.27 KSJ �����϶��� ������ 'X'�̰� ���󰡰� 23�� �´�. 

		CString strTime, expect, real, excep;
		CString codeExceptA;

		//111�� ���������� 023�� ���������� �Ǵ��ؼ� ���� ����

		//CString en2, saveData;
	
		//if (data[111])	// ����...
		//{
		//	saveData = (char*)data[111];    // ����
		//	en2      = (char*)data[34];     // ü��ð�
		//}
		//else if (data[23])
		//{ 
		//	saveData = (char*)data[23];     // ���簡
		//	en2      = (char*)data[34];	// ü��ð�

		//	if (!en2.IsEmpty())
		//	{
		//		//for (int i = 0; i < MAX_LINE; i++)
		//		//{
		//		//	codeExceptA = strCode;

		//		//	//�����ȣ�� ��Ī�ؼ� �����͸� �����ش�
		//		//	//if (codeExceptA == m_iYDayVol[i][JMCODE])
		//		//	//{
		//		//	//	m_iYDayVol[i][TDAYVALUE] = saveData;
		//		//	//}
		//		//}
		//	}
		//}
		entry.Empty();

		CString str90 = (char*)data[90]; // 10ȣ�� �ŵ�ȣ�� �ܷ�
		str90.Trim();

		BOOL bLast = FALSE;  //2015.01.15 ���͸������� ���� ������ ������ �����ؾ���.
		if ((!strGubn.Compare("L") || !strGubn.Compare("4") || !strGubn.Compare("P") || !strGubn.Compare("g")) && ((!str90.Compare("99")) || (!str90.Compare("40"))))
		{
			//SetItemText(xrow, colEXPECT, "0");
			SetItemData(xrow, colEXPECT, 0); //2013.09.13 KSJ �ɼ����͸������� �������� ���󰡸� �ִµ� �����ؾ��Ѵ�.	
			mapLine.emplace(colEXPECT, "0");
			bLast = TRUE;
		}
		const int nEndOPMarket = GetItemData(xrow, colEXPECT);	//2013.09.17 KSJ �ش������� ������ �Ǿ�����

		if ((!strGubn.Compare("X") || data[111] || nEndOPMarket == 1) && !bLast)	// ���� ����	2013.08.22 �������󰡴�111�ɺ��� ���� ���а���X�� �´�.
		{		
			if (data[111])
				entry = (char*)data[111];
			else if (nEndOPMarket == 1)	//2013.09.17 KSJ �ش������� ������ �Ǿ�����
				entry = " ";			//���󰡰� 0�� �ö��� ���� ��Ȳ��.
			else
			{
				bZisu = TRUE;
				entry = (char*)data[23];	//2013.08.22 �������󰡴�111�ɺ��� ���� ���а���X�� �´�.
			}


			if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
			{
			//	SetItemText(xrow, colEXPECT, "1");
				mapLine.emplace(colEXPECT, "1");
			}
			else
			{
				//2012.05.09 KSJ ���󰡰� 0�� �ö��� ���簡�� �ѷ��ش�.
				m_mapCurValue.Lookup(strCode, strData);

				SetItemText(xrow, colEXPECT, "0");	//���� ���
				mapLine.emplace(colEXPECT, "0");

				//SetItemBkColour(xrow, colNAME, GetItemBkColour(xrow, colCODE));
				int i = col7852;

				_gridHdr gridHdr{};
				CString str7852, str7853, str7854, str7855;

				while (AfxExtractSubString(entry, strData, i, '\t'))
				{
					if (i >= GetColumnCount())	return;

					if (entry == "(null)" || entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0" || entry == " ")
						entry = "";

					//2013.07.01 KSJ ���簡 ����
					gridHdr = m_gridHdrX.GetAt(i);
					symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));

					if (symbol.GetLength() >= 3)
						symbol = symbol.Right(3);

					if (i == col7852)
						str7852 = entry;
					else if (i == col7853)
						str7853 = entry;
					else if (i == col7854)
						str7854 = entry;
					else if (i == col7855)
						str7855 = entry;
					else if (i == colMAR || i == colMAR1 || i == colSIG || i == colCODE || i == colNAME /*|| i == colCURR*/)	//2013.07.25 KSJ ���簡�� ���⼭ Ÿ�� �ȵȴ�.
						entry = GetItemText(xrow, i);
					else if (!symbol.Compare("023") || !symbol.Compare("111"))	//
						entry = str7852;
					else if (!symbol.Compare("027") || !symbol.Compare("112"))	//
						entry = str7853;
					else if (!symbol.Compare("024") || !symbol.Compare("115"))	//
						entry = str7854;
					else if (!symbol.Compare("033") || !symbol.Compare("116"))	//
						entry = str7855;

					if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
					{
						//2013.07.01 KSJ 
						mapLine.emplace(i++, entry);
						//SetItemText(xrow, i++, entry, false);
					}
					else if (!symbol.Compare("##1") || !symbol.Compare("##2") || !symbol.Compare("##3") || !symbol.Compare("##4"))	//2014.02.12 ��������, ���Դܰ�, �򰡼���, ���ͷ��� ����ڰ� �Է��ϴ� �κ��̹Ƿ� �翬�� ���̴�.
					{
						mapLine.emplace(i, GetItemText(xrow, i++));
						//SetItemText(xrow, i, GetItemText(xrow, i++), false);
					}
					else
					{
						mapLine.emplace(i++, " ");
						//SetItemText(xrow, i++, " ", false);
					}
				}

				// strData.Replace("\t", "[]");
				entry = GetItemText(xrow, colCURR);
				// KSJ
				return;
			}
			bTransSymbol = TRUE;
		}
		else if (data[23])
		{
			entry   = (char*)data[23];
			strTime = (char*)data[34];

			if (m_strBeginTime <= strTime && m_strEndTime >= strTime)
			{
				if (entry != "0" && entry != "-0" && entry != "0.00" && entry != "+0" && entry != " 0" && entry != " ")
				{
					SetItemText(xrow, colEXPECT, "1");

					mapLine.emplace(colEXPECT, "1");
					//SetItemBkColour(xrow, colNAME, RGB(255, 255, 128));
				}
				else
				{
					SetItemText(xrow, colEXPECT, "0");		//2012.08.29 KSJ ���� �����. 8:10 ~ 8:30�� ���̿� ü���� ��������

					mapLine.emplace(colEXPECT, "0");
					//SetItemBkColour(xrow, colNAME, GetItemBkColour(xrow, colCODE));
					entry = GetItemText(xrow, colCURR);
					return;
				}
			}
			else
			{
				SetItemText(xrow, colEXPECT, "0");
				mapLine.emplace(colEXPECT, "0");
				//SetItemBkColour(xrow, colNAME, GetItemBkColour(xrow, colCODE));
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//2012.05.10 KSJ ParseOubs���� ������m_mapCurValue�� �����͵��� �ʱⰪ�̹Ƿ� 2012.08.22 ����
			//�ǽð����� ����� ���� ������ �־���Ѵ�. �ŷ���, ü�ᷮ ����..
			//�Ʒ��� ���� ���󰡰� 0�� ���ö��� �������ָ� ��ũ�� ���� �������� �־ ��� ������Ʈ �Ѵ�.
			//���� ȣ�� �ð��϶��� ���� �����Ѵ�.
			//�ڵ��� üũ�Ǿ� �������� �����Ѵ�.

			const int countX = m_gridHdrX.GetSize();
			_gridHdr gridHdr{};

			CString strNewData, strNewTemp, strTemp;
			strNewData.Empty();

			for (int jj = col7852; jj < countX; jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);

				symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));

				if (symbol.GetLength() >= 3)
					symbol = symbol.Right(3);

				//2012.11.19 KSJ �����Ͱ� ������ �Ǹ� �ִ´�.
				//2013.07.01 KSJ ���簡 ����
				if (atoi(symbol) == 0) // 2014.03.18 KSJ �ɺ����� #�� ���� 0�̵Ǿ B,F,G�̷� ���� ����.
					strNewTemp.Format("\t");
				else if (data[atoi(symbol)])
					strNewTemp.Format("\t%s", (char*)data[atoi(symbol)]);
				else if (jj == col7852)
					strNewTemp.Format("\t%s", (char*)data[atoi("023")]);
				else if (jj == col7853)
					strNewTemp.Format("\t%s", (char*)data[atoi("027")]);
				else if (jj == col7854)
					strNewTemp.Format("\t%s", (char*)data[atoi("024")]);
				else if (jj == col7855)
					strNewTemp.Format("\t%s", (char*)data[atoi("033")]);
				else
					strNewTemp.Format("\t");

				strNewData += strNewTemp;
			}

			if (m_mapCurValue.Lookup(strCode, strData))
			{
				int nIndex = 0, nCount = 0;
				while (nIndex != -1)
				{
					nIndex = strData.Find(P_TAB, nIndex + 1);

					nCount++;

					if (nCount == col7852)
					{
						strTemp = strData.Left(nIndex);
						strData = strTemp + strNewData;
						m_mapCurValue.SetAt(strCode, strData);
						break;
					}
				}
			}
			// KSJ 2012.08.22 ���� ��
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		newEXP = GetItemText(xrow, colEXPECT);
		const BOOL bForceDraw = (newEXP == oldEXP) ? FALSE : TRUE;
		BOOL	bExpect = (BOOL)atoi(GetItemText(xrow, colEXPECT));	// ������ ������ 0 �̳� 1 �� return
		const int countX = GetColumnCount();
		_gridHdr gridHdr{};

		//2012.11.08 KSJ �ֽİ� �����ɼ��� �ɺ����� �Ȱ��� ���� �ִ�. ���μ����� ������.
		bool bKospi = true;
		if (strCode.GetLength() == 6)
			bKospi = true;
		else
			bKospi = false;
		//KSJ

		for (int ii = 2; ii < countX; ii++)
		{
			GetGridHeader(ii, gridHdr);
			symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));

			if (symbol.GetLength() >= 3)
				symbol = symbol.Right(3);

			if (!bTransSymbol)	//2013.07.08 ����üũ�Ǿ� ������ �ؿ� Ÿ����
			{
				//2012.03.16 KSJ ����, �ŷ��� �����־ �߰���..
				if (!symbol.Compare("111"))		//����
					entry = (char*)data[23];	//���簡
				else if (!symbol.Compare("112"))	//����ŷ���
					entry = (char*)data[27];	//�ŷ���
				else if (!symbol.Compare("115"))	//�������ϴ��
					entry = (char*)data[24];	//���ϴ��
				else if (!symbol.Compare("116"))	//��������
					entry = (char*)data[33];	//�����
				// 					else if((!symbol.Compare("051") || !symbol.Compare("071")) && !bCheType)	//2012.06.20 KSJ ü���϶��� �ŵ�ȣ�� �ż�ȣ�� �׸�
				// 						entry = "";
				else if (!bKospi && !symbol.Compare("204"))// 2012.11.08 KSJ ���μ�����
					entry = " ";
				else
					entry = (char*)data[atoi(symbol)];
			}
			else if (data[atoi(symbol)] && bZisu)	//2013.08.27 KSJ �����϶��� �ɺ��� �ε����� ��ġ��.
			{
				entry = (char*)data[atoi(symbol)];
			}
			else if (bTransSymbol)
			{
				//2012.03.16 KSJ ����, �ŷ��� �����־ �߰���..
				if (!symbol.Compare("023"))		//����
					entry = (char*)data[111];	//���簡
				else if (!symbol.Compare("027"))	//����ŷ���
					entry = (char*)data[112];	//�ŷ���
				else if (!symbol.Compare("024"))	//�������ϴ��
					entry = (char*)data[115];	//���ϴ��
				else if (!symbol.Compare("033"))	//��������
					entry = (char*)data[116];	//�����
				// 					else if((!symbol.Compare("051") || !symbol.Compare("071")) && !bCheType)	//2012.06.20 KSJ ü���϶��� �ŵ�ȣ�� �ż�ȣ�� �׸�
				// 						entry = "";
				else if (!bKospi && !symbol.Compare("204"))// 2012.11.08 KSJ ���μ�����
					entry = " ";
				else
					entry = (char*)data[atoi(symbol)];
			}
			else
				continue;

			if (!bForceDraw && IH::TOf(entry) == IH::TOf(GetItemText(xrow, ii)))
				continue;

			if (!(gridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
			{
				if ((strcmp("2029", gridHdr.symbol) == 0) ||
					(strcmp("2030", gridHdr.symbol) == 0) ||
					(strcmp("2031", gridHdr.symbol) == 0))
				{
					if ((1 == gridHdr.needs) || (3 == gridHdr.needs))
					{
						CString strVal = entry;
						CString strDiff = strVal;

						if (strVal[0] == '+' || strVal[0] == '-')
						{
							strDiff = strVal.Mid(1);
						}

						double dDiffOpen = 0.0;
						double dPClose{}, dVal{};
						dVal = atof(strDiff.GetBuffer(0));
						dPClose = atof((GetItemText(xrow, colPCURR)).GetBuffer(0));

						if (dVal != 0 && dPClose != 0)
						{
							dDiffOpen = (dVal - dPClose) / dPClose * 100;

							if (1 == gridHdr.needs)
							{
								if (0 > dDiffOpen)
								{
									entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
								}
								else
								{
									entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
								}
							}
							else if (3 == gridHdr.needs)
							{
								if (0 > dDiffOpen)
								{
									entry.Format("%0.2f%c", dDiffOpen, P_PER);
								}
								else if (0 == dDiffOpen)
								{
									entry.Format(" %0.2f%c", dDiffOpen, P_PER);
								}
								else
								{
									entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
								}
							}
						}
					}
				}
			}

			if ((ii == colCURR) && bExpect && !m_bExpect)		// �����ư�� �ȴ������ְ�, ����ȣ����
			{
				entry.TrimLeft();
				entry.TrimRight();

				if ((entry == "0") || (entry == "-0") || (entry == "+0") || (entry == ""))
				{
					entry = " ";

					entry = GetItemText(xrow, colCURR);
					return;
				}
			}

			CString tempStr;

			if (ii == colCURR)
			{
				if (code.Find("K0001") > -1)
				{
					m_dKospi = atof(entry);
					tempStr.Format("%.2f", m_dKospi);
					m_dKospi = atof(tempStr);
				}
				else if (code.Find("KQ001") > -1)
				{
					m_dKosdaq = atof(entry);
					tempStr.Format("%.2f", m_dKosdaq);
					m_dKosdaq = atof(tempStr);
				}
			}

			//2012.03.20 KSJ ���簡 ���յɶ� ���, ����� �������� �ʴ� ���� ����
			if ((strcmp("2115", gridHdr.symbol) == 0) || (strcmp("2024", gridHdr.symbol) == 0)
				|| (strcmp("2116", gridHdr.symbol) == 0) || (strcmp("2033", gridHdr.symbol) == 0))
			{
				entry.TrimLeft(); entry.TrimRight();
				if (entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == "30" || entry == ".00" || entry == "+0.00" || entry == "-0.00")	//2012.07.17 KSJ �ɼ��϶��� ��� �߰�
				{
					bDaebi = TRUE;
				}
			}
			else
			{
				//2012.03.22 KSJ ��������, ���Դܰ�, �򰡼��� �� �ʵ尪�� #�� ���� ���� atoi���� 0�� ���´�.
				//�׷��� entry�� data[0]���� �̾ƿ��Ƿ� ���� ���а� d,f,b, �̷������� �� �׸��忡 �ѷ�����.
				entry.TrimLeft(); entry.TrimRight();
				if (entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == ".00" || entry == "+0.00" || entry == "-0.00" || atof(entry) == 0 || atof(symbol) == 0)	//KSJ 2012.07.17 atoi --> atof�� ����
				{
					if (entry.GetLength() > 0 && atof(entry.Mid(1)) == 0)	entry = " ";	//2013.09.24 KSJ �������������� ++ +- �� ��ȣ�� ���ڸ��� �����µ� �̶��� atof�� ���� ���ϸ� 0���� ���´�. �׷��� ���ڸ� �ڸ��� ���غ�
				}
			}

			CString tmp;

			// 				entry.TrimLeft();
			// 				entry.TrimRight();

			if (ii == colCURR)
			{
				if (entry == "")
				{
					entry = GetItemText(xrow, colCURR);
				}
			}

			CString strPreValue;

			strPreValue = GetItemText(xrow, ii);
			strPreValue.TrimLeft();
			strPreValue.TrimRight();

			if (ii != colCURR && strPreValue != entry)
			{
				entry.TrimLeft();
				entry.TrimRight();

				if (!entry.IsEmpty())	//2012.03.20 KSJ ���簡 ���յɶ� ���, ����� �������� �ʴ� ���� ����
				{
					if (bDaebi)
						entry = "";

					bDaebi = FALSE;

					//2012.02.27 KSJ �׷����� �����Ϳ� �̻��������� ���⸦ üũ�ؾ���					
					//SetItemText(xrow, ii, entry);

					//mapLine->SetAt(ii, entry);	//2016.05.30 KSJ map �߰�
					mapLine.emplace(ii, entry);
				}
			}
			else if (ii == colCURR)
			{
				//2013.08.23 KSJ �������������̸� ����+, �϶�+, ���- �� ���� ��ȣ�� ǥ��������Ѵ�.
				if (strCode.GetLength() == 8 && (strCode.GetAt(0) == '4' || strCode.GetAt(0) == 'D'))  //�Ļ���ǰ �ڵ尳��
				{
					if (entry.GetAt(0) == ' ')	//ó���� ��ĭ�̸� ���տ� -, + ǥ�ð� �ִ� ���̴�.
						entry.Replace(" ", "��");	//�׶��� '��' �� ���� 1�� �� �־��ش�. �����̽� �ƴ�..
					else if (entry.GetAt(0) == '0')
					{
						entry.Delete(0);
						entry.Insert(0, "��");
					}

				}
				//2013.08.23 KSJ END


//				SetItemText(xrow, ii, entry, bForceDraw); //2016.05.17 KSJ ����-> ���簡 ���簡 -> ���󰡷� �ٲ�� �׷��ش�.

				mapLine.emplace(ii, entry);
			}

			//���ϰŷ������ �ǽð� ���
			CString str2027, str2403;
			double int2027 = 0;
			double int2403 = 0;
			const double int2321 = 0;

			if (data[27])
			{
				str2027 = (char*)data[27];
				int2027 = atoi(str2027);
				//for (int i = 0; i < 100; i++)
				//{
				//	//�����ȣ�� ��Ī�ؼ� �����͸� �����ش�
				//	if (strCode == m_iYDayVol[i][JMCODE])
				//	{
				//		int2321 = atoi(m_iYDayVol[i][YDAYVOLUMN]);
				//	}
				//}

				//if (int2321 != 0)
				//{
				//	int2403 = (int2027 / int2321) * 100;
				//}
				//else
				//{
				//	int2403 = 0;
				//}

				if (!bExpect && m_yDayVolField >= 0)
				{
					entry.Format("%.2f", int2403);

					//SetItemText(xrow, m_yDayVolField, entry);
					mapLine.emplace(m_yDayVolField, entry);
				}
			}

			if ((ii == colCURR))
			{
				if (!bExpect && m_bongField >= 0)
				{
					CString	bongdata = _T(""), open = _T(""), high = _T(""), low = _T("");

					if (data[29])
					{
						open = (char*)data[29];
						mapLine.emplace(colOPEN, open);
					}

					if (data[30])
					{
						high = (char*)data[30];
						high.Remove('+'), high.Remove('-');

						if (data[31])
						{
							low = (char*)data[31];
							low.Remove('+'), low.Remove('-');
							bongdata.Format("%s%c%s", high, P_TAB, low);
							mapLine.emplace(m_bongField, bongdata);
						}
					}
				}

			//	entry = data[2];
				entry.TrimLeft();
				entry.TrimRight();

				if ((entry == "0") || (entry == "-0") || (entry == "+0") || (entry == ""))
				{
					entry = " ";
					entry = GetItemText(xrow, colCURR);
					entry.Replace(",", "");		//,�� ��������� ���ش�.
				}	
			}

			//2012.03.20 KSJ ���簡 ���յɶ� ���, ����� �������� �ʴ� ���� ����
			if ((strcmp("2115", gridHdr.symbol) == 0) || (strcmp("2024", gridHdr.symbol) == 0)
				|| (strcmp("2116", gridHdr.symbol) == 0) || (strcmp("2033", gridHdr.symbol) == 0))
			{
				entry.TrimLeft(); entry.TrimRight();
				if (entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == "30" || entry == ".00" || entry == "+0.00" || entry == "-0.00")	//2012.07.17 KSJ �ɼ��϶��� ��� �߰�
				{
					bDaebi = TRUE;
				}
			}
			else
			{
				//2012.03.22 KSJ ��������, ���Դܰ�, �򰡼��� �� �ʵ尪�� #�� ���� ���� atoi���� 0�� ���´�.
				//�׷��� entry�� data[0]���� �̾ƿ��Ƿ� ���� ���а� d,f,b, �̷������� �� �׸��忡 �ѷ�����.
				entry.TrimLeft(); entry.TrimRight();
				if (entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == ".00" || entry == "+0.00" || entry == "-0.00" || atof(entry) == 0)	//KSJ 2012.07.17 atoi --> atof�� ����
				{
					if (entry.GetLength() > 0 && atof(entry.Mid(1)) == 0)	entry = " ";	//2013.09.24 KSJ �������������� ++ +- �� ��ȣ�� ���ڸ��� �����µ� �̶��� atof�� ���� ���ϸ� 0���� ���´�. �׷��� ���ڸ� �ڸ��� ���غ�
				}
			}

			if (ii == colCURR)
			{
				if (entry == "")
				{
					entry = GetItemText(xrow, colCURR);
					entry.Replace(",", "");		//,�� ��������� ���ش�.
				}
			}

			strPreValue = GetItemText(xrow, ii);
			strPreValue.Replace(",", "");		//,�� ��������� ���ش�.

			strPreValue.TrimLeft();
			strPreValue.TrimRight();

			if (ii != colCURR && strPreValue != entry)
			{
				entry.TrimLeft();
				entry.TrimRight();

				if (!entry.IsEmpty())	//2012.03.20 KSJ ���簡 ���յɶ� ���, ����� �������� �ʴ� ���� ����
				{
					if (bDaebi)
						entry = "";

					bDaebi = FALSE;

					//2012.02.27 KSJ �׷����� �����Ϳ� �̻��������� ���⸦ üũ�ؾ���
					//�����ϴ�� �ٽ� �׸��� �ȵȴ�.
// 						if(	GetItemText(xrow, colEXPECT) == "1" && ii == colNAME)
// 							DisplayAttribute(GetItemAttr(0, ii), entry, xrow, ii);
// 						else
// 							SetItemFgColour(xrow, ii, DisplayAttribute(GetItemAttr(0, ii), entry, xrow, ii));

					//SetItemText(xrow, ii, entry);
					mapLine.emplace(ii, entry);
				}
			}
			else if (ii == colCURR)
			{
				//2013.08.23 KSJ �������������̸� ����+, �϶�+, ���- �� ���� ��ȣ�� ǥ��������Ѵ�.
				if (strCode.GetLength() == 8 && (strCode.GetAt(0) == '4' || strCode.GetAt(0) == 'D')) //�Ļ���ǰ �ڵ尳��
				{
					if (entry.GetAt(0) == ' ')	//ó���� ��ĭ�̸� ���տ� -, + ǥ�ð� �ִ� ���̴�.
						entry.Replace(" ", "��");	//�׶��� '��' �� ���� 1�� �� �־��ش�. �����̽� �ƴ�..
				}
				//2013.08.23 KSJ END
				//SetItemText(xrow, ii, entry);
				mapLine.emplace(ii, entry);
			}

			if (data[23])
			{
				CString sCurr = CString((char*)data[23]);
				const auto& mt = mapLine.emplace(colCURR, sCurr);
				if (!mt.second)
					mapLine.at(colCURR) = sCurr;
			}
		}
	}
}

UINT ParsingRTM(LPVOID lpvoid)
{
	CGridWnd* pMain = (CGridWnd*)lpvoid;
	if (pMain == nullptr)
		return FALSE;

	POSITION pos{}, posLine{};

	int nRow{};
	CString strEntry, strTemp;

	CMap<int, int, CString, CString>* mapLine{};	//�Ѷ��ο��� �����κ� �����Ѵ�.

	CString code = "";

	while (pMain->m_bThreadRTM && WaitForSingleObject(pMain->m_hEventRTM, 800))
	{
		if (pMain->m_bRemove == TRUE || pMain->_mapFms.size() <= 0) // 2011.12.29 KDK
			continue;
		pMain->m_syncRTM.Lock();

		for (auto& item : pMain->_mapFms)
		{
			int count = 0;
			count = pMain->CheckRealTimeCode(item.first);		
			if (count == 0)
				continue;
	
			auto& map = item.second;
			for (auto mapitem : map)
			{
				for (int rowPosition = 0; rowPosition < count; rowPosition++)
				{
					nRow = pMain->m_irowCode[rowPosition];
					pMain->SetItemText(nRow, mapitem.first, mapitem.second);
				}
			}
		}
		pMain->_mapFms.clear();
		pMain->m_syncRTM.Unlock();
	}
	return 0;
}

bool CGridWnd::WaitRTM()
{
	if (!m_pThreadRTM)
		return false;

	//2012.01.09 KDK 3�ʸ� 1�ʷ� 2012.01.13 1�ʸ� 0.5�ʷ�
	//DWORD res = WaitForSingleObject(m_hEventRTM, 3000);
	const DWORD res = WaitForSingleObject(m_hEventRTM, 300);
	// KDK

	switch (res)
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_FAILED:
		return false;
	case WAIT_ABANDONED:
		return false;
	case WAIT_TIMEOUT:
		return false;
	}
	return true;
}

void CGridWnd::initRTM()
{
	if (m_bThreadRTM) 
		return;

	m_bThreadRTM = true;
	m_pThreadRTM = AfxBeginThread((AFX_THREADPROC)ParsingRTM, (LPVOID)this, THREAD_PRIORITY_NORMAL);	//2012.02.10 KSJ
	m_hEventRTM = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CGridWnd::stopRTM()
{
	if (!m_bThreadRTM || !m_pThreadRTM)
		return;

	m_bThreadRTM = false;
	SetEvent(m_hEventRTM);
	m_pThreadRTM->PostThreadMessage(WM_QUIT, 0, 0);
	switch (WaitForSingleObject(m_pThreadRTM->m_hThread, 1000))
	{
	case WAIT_OBJECT_0:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
		break;
	case WAIT_TIMEOUT:
		TerminateThread(m_pThreadRTM->m_hThread, 0);
		break;
	}
	removeRTM();
}

void CGridWnd::removeRTM()
{
	m_bRemove = TRUE;		// 2011.12.29 KDK
	m_syncRTM.Lock();
	_mapFms.clear();
	m_syncRTM.Unlock();
	m_bRemove = FALSE;		// 2011.12.29 KDK
}


int CGridWnd::insertRow(int row, BOOL bInsertInter /* = TRUE */)
{
	//2011.11.09 KSJ
	//row�� 0�̸� (ƼĿ)��..
	if(row <= 0)
		return 0;

	if (row > GetRowCount())
		row = GetRowCount();

	// ADD PSH 20070914
	m_rowC++;
	// END ADD

	LOGFONT lf{}, lfB{};
	const int fontsize = ((CMainWnd*)m_pMainWnd)->GetFontSize();
	CString	fontname = ((CMainWnd*)m_pMainWnd)->GetFontName();
	// MODIFY PSH 20070917
	const BOOL	bCurrBold = ((CMainWnd*)m_pMainWnd)->GetCurrBold();
	const BOOL	bNameBold = ((CMainWnd*)m_pMainWnd)->GetNameBold();
	const BOOL	bAllBold = ((CMainWnd*)m_pMainWnd)->GetAllBold();

	CAxisExt::m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, false ,true);
	
	CAxisExt::m_pFont->GetLogFont(&lf); lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	m_pFontB->GetLogFont(&lfB); lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	const int	ret = InsertRow("", row);
	
	if(row >= gsl::narrow_cast<int>(m_inters.size()))
	{
		return 0;
	}

	auto& pInter = m_inters.at(row);	

	GVITEM	gvitem; 
	_gridHdr gridHdr{};
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row   = row;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= gridHdr.format;
		gvitem.symbol= gridHdr.symbol; 
		gvitem.attr  = gridHdr.attrx & ~GVAT_MARKER;
		gvitem.fgcol = GetIndexColor(69);
		
		//�ϸ�ũ�Ǿ��ִ��� Ȯ��
		if(pInter->bookmark == '1')
		{
			//gvitem.bkcol = ((CMainWnd*)m_pMainWnd)->GetBookMarkColor();	
		}
		else
		{
			gvitem.bkcol = GetIndexColor(68);	
		}
		/*
		if(pInter->code[0] == 'm')
		{
			if (ii == colCODE ||ii == colRCOD)
			{
				gvitem.text = pInter->code;
			}
		}
		*/
		
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		if ((ii == colCODE) || (ii == colCURR) || (ii == colNAME) || bAllBold)
		{	// 20070706 kwon
			if(gvitem.symbol.Compare("2024"))	//2012.11.29 KSJ ����϶��� GVAT_CONDITIONx�ϸ� �������� �ȵ�
				gvitem.attr |= GVAT_CONDITIONx;
			
			// MODIFY PSH 20070917
			//if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
			// END MODIFY
			{
				//gvitem.attr &= ~GVAT_CONDITIONx;
				CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
			}
			//else
			//	gvitem.attr |= GVAT_CONDITIONx;
		}
		if (ii == colNAME)
		{
			//gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}

		if (ii == colCODE)
		{
			gvitem.mask |= GVMK_PARAM;
			CString string = GetItemText(row,colCODE);
			int result = ((CGroupWnd*)m_pGroupWnd)->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)string);
			
			if(result == 1)
			{
				result = 1;
			}
			else if(result == 2)
			{
				result = 0;
			}
			else
			{
				result = 2;
			}
			
			gvitem.param = (LPARAM)result;
		}

		SetItem(&gvitem);
	}

	ClearSearchMap();	//2011.12.29 KSJ 
	ReSetSearchMap();	//2011.12.29 KSJ 

	return ret;
}

void CGridWnd::InsertRowNCode(int row)
{
	//2011.11.09 KSJ
	//row�� 0�̸� (ƼĿ)��..
	if(row <= 0)
		return;

	int rowcount = 0;
	
	rowcount = GetRowCount();
	
	if(rowcount >= MAX_LINE)
	{
		Variant(guideCC, IH::idTOstring(IDS_GUIDE4));

		return;
	}

	insertInterest(row - 1);
	insertRow(row);
	SetFocusCellEdit(row, colNAME, true);

	if (m_bAddCnd)
		saveInterest();
}

void CGridWnd::DeleteRow(int row)
{
	//2011.11.09 KSJ
	//GetRowCount�� 2�̸� ���� Row�� �ϳ� ���� ���̹Ƿ� ����� �ȵ�.
	//����� Insert�� ���� ����.
	//row�� 0�̸� (ƼĿ)��..
	
	CString strCode;
	if(GetRowCount() <= 2 || row <= 0)
		return;
	
	//2012.12.18 KSJ ������ �ѻ��¿��� �����ϸ� �ش��ϴ� row�� m_inters�� ���� ���� �ʴ�.
	//m_inters�� ���� baseSorting �������� �Ǿ� �ֱ� ������.
	//�׷��� �ڵ带 ������ ��ġ�� ã�ƾ� �Ѵ�.
	strCode = GetItemText(row, colCODE);
	
	CGridCtrlExt::DeleteRow(row);
	m_rowC--;	
	
	//2013.01.28 KSJ �ڵ带 ã�Ƽ� ����� �����϶��� �������� �ʴ´�.
	//�׷��� �ڵ尡 ���϶��� row -1�� �����.
	strCode.TrimLeft(); strCode.TrimRight();
	if(!strCode.IsEmpty())
		RemoveInters(GetInter(strCode)); //RemoveInters(row -1);
	else
		RemoveInters(row -1);
	//2013.01.28 KSJ End
	//2012.12.18 KSJ End
	
	//ReIndexing();
	
	ClearSearchMap();	//2011.12.29 KSJ 
	ReSetSearchMap();	//2011.12.29 KSJ 
	
}

void CGridWnd::ReIndexing()
{
	m_arExist.RemoveAll();
	CString	sztmp, szindex;
	for ( int ii = 0 ; ii < MAX_LINE ; ii++ )
	{
		sztmp = GetItemText(ii+1, 0);
		if (!sztmp.IsEmpty())
		{	
			szindex.Format("%d", ii);
			m_arExist.SetData(sztmp, szindex);
		}
	}
}

void CGridWnd::Reset(bool bAll)
{
/*
	KillTimer(1000);
	RemoveAll();
	int	nActive = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ACTIVE));
	if (nActive == m_nIndex)
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET));
	
	Clear();
	
	if (bAll)
	{
		m_szTitle = _T("");
		SetKind(0);		
	}
*/
}

// ������������ ��ư ���ý�
void CGridWnd::saveInterestX()
{
	CString	code, bookmark, string, stringx;
	
	
	//2012.02.09 KSJ ���⼭ �� m_inters�� ���ϴ��� �𸣰ڴ�..
	//�������δ� BaseSort���� ������ �Ұ� ������..
	//rebuildInterest�� �Ⱦ��� ������ �ּ�ó���ϴϱ� ����ε�..

	int scnt = 0;
	for (scnt = m_inters.size() - 1 ; scnt >= 0 ; scnt--)
	{
		auto &pinters = m_inters.at(scnt);
		if (strlen(pinters->code) > 0)
			break;
	}
	
	if (scnt < 0)
	{
		//if(!m_bPlusMinus)	//2011.12.05 KSJ  �׷� �߰� �����ÿ��� �޼��� �ȶߵ���
		Variant(guideCC, "���õ� �׷쿡 ������ �����ϴ�.");
		
		return;
	}
	
		
	if(m_bEditWork)
	{
		saveInterest(true);	
		m_bEditWork = FALSE;
	}
	else
	{
		Variant(guideCC, "�ش� ���ɱ׷��� ����� ������ �����ϴ�.");
	}
}

void CGridWnd::saveInterest(bool btmp, int gno, bool bBookMark)	//2013.05.14 KSJ �ϸ�ũ�����Ҷ��� ���Ҹ�� �����ϵ���.
{
	if (gno < 0)
	{
		if(m_nGroupIndex < 0)	return;
		gno = m_nGroupIndex;
	}
	
	CSendData sData;
	CString strPath(_T("")), strTemp(_T("")), strBook(_T(""));
	CString strSendData(_T(""));
	
	strPath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, gno);
	strBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_user, gno);

	char szTemp[10]{};
	int nScnt = 0;
	
	if(btmp)
		strPath += ".tmp";
	
	struct _inters* pBInters{};
	
	int i = 0;
	for (i = m_inters.size() - 1; i >= 0; i--)
	{
		auto& pInters = m_inters.at(i);	
		if (!pInters->code.IsEmpty())
			break;
	}
	
	if (i < 0)
		nScnt = 0;
	else
		nScnt = i;
	
	//2013.05.21 KSJ �ϸ�ũ�����Ҷ��� �ϸ�ũ ���ϸ� ���� ���̽��� �������� �����ϵ���
	if(bBookMark)
	{
		//�ϸ�ũ�� ���� ���Ϸ� ���� <�ӽ�>
		::DeleteFile(strBook);
		CFile	file2(strBook, CFile::modeWrite|CFile::modeCreate);

		//2013.05.21 KSJ pInters�� �ϸ�ũ�� �� row�� pBInters���� ã�Ƽ� �־��ش�.
		auto& pInters = m_inters.at(m_nBookMarkRow);
		for(int jj = 0; jj < gsl::narrow_cast<int>(m_arrBaseInters.size()); jj++)
		{
			auto& pBInters = m_arrBaseInters.at(jj);
			if(pInters->code.CompareNoCase(pBInters->code) == 0)
			{
				pBInters->bookmark = pInters->bookmark == '1' ? '1':'0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
				m_arrBaseInters.at(jj) =  pBInters;
			}
		}
		//2013.05.21 KSJ End

		for (i = 0; i <= nScnt; i++)
		{
			auto& pBInters = m_arrBaseInters.at(i);

			//�ϸ�ũ�� ���� ���Ϸ� ����
			struct _bookmarkinfo binfo;
			ZeroMemory(&binfo, sizeof(_bookmarkinfo));
			FillMemory(&binfo, sizeof(_bookmarkinfo), ' ');
			
			if(strlen(pBInters->code) == 0)
			{
				CopyMemory(binfo.code, "            ", sizeof(binfo.code));
				CopyMemory(binfo.name, "            ", sizeof(binfo.name));
				binfo.bookmark[0] = '0';
			}
			else
			{
				CopyMemory(binfo.code, pBInters->code, sizeof(binfo.code));
				CopyMemory(binfo.name, pBInters->name, sizeof(binfo.name));
				binfo.bookmark[0] = pBInters->bookmark == '1' ? '1':'0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.
			}
			
			file2.Write(&binfo, sizeof(_bookmarkinfo));
			
		}
		file2.Close();

		(CGroupWnd*)m_pGroupWnd->SendMessage(WM_MANAGE, MK_PROCDLL);

		CString string = "OnPortfolio\tok";
		((CGroupWnd*)m_pGroupWnd)->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR) string);

		return;
	}
	//2013.05.21 KSJ End


	//::DeleteFile(strPath);
	//CFile	file(strPath, CFile::modeWrite|CFile::modeCreate);

	//�ϸ�ũ�� ���� ���Ϸ� ���� <�ӽ�>
	::DeleteFile(strBook);
	CFile	file2(strBook, CFile::modeWrite|CFile::modeCreate);

	struct _updn updn;

	ZeroMemory(&updn, sizeof(_updn));
	FillMemory(&updn, sizeof(_updn), ' ');

	CopyMemory(&updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'G';
	CopyMemory(updn.uinfo.nblc, _T("00001"), sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'O';
	
	sprintf(szTemp, "%02d", gno);
	CString strGrouptName = ((CMainWnd*)m_pMainWnd)->GetGroupName(gno);
	
	CopyMemory(updn.ginfo.gnox, szTemp, sizeof(updn.ginfo.gnox));
	CopyMemory(updn.ginfo.gnam, (LPCTSTR)strGrouptName, min(sizeof(updn.ginfo.gnam), strGrouptName.GetLength()));
	sprintf(szTemp, "%04d", i + 1); 
	CopyMemory(updn.ginfo.jrec, szTemp, sizeof(updn.ginfo.jrec));
	
	strSendData = CString((char*)&updn, sizeof(_updn));

	bool bSetBookMark = false;	//2014.06.05 KSJ ���࿡ �ϸ�ũ�� �ϳ��� �����Ǿ� ���� ������ ������ �ʿ䰡 ����.

	for (i = 0; i <= nScnt; i++)
	{
		auto& pInters = m_inters.at(i);
		
		if (!pInters->code.IsEmpty() && pInters->name.IsEmpty())
		{
			CString strName = GetItemText(i + 1, colNAME);
			pInters->name = strName;
		}
		
		struct _jinfo jinfo;
		FillMemory(&jinfo, sizeof(_jinfo), ' ');
		
		jinfo.gubn[0] = pInters->gubn > 0 ? pInters->gubn:'0';


		if (!pInters->code.IsEmpty())
		{
			CopyMemory(jinfo.code, pInters->code, strlen(pInters->code));
			CopyMemory(jinfo.xprc, strlen(pInters->xprc) > 0 ? pInters->xprc:"          ", sizeof(jinfo.xprc));	//2015.04.08 KSJ Cstring�� �ֱ⶧���� �ΰ��� ���� �ȵ�.
			CopyMemory(jinfo.xnum, strlen(pInters->xnum) > 0 ? pInters->xnum:"          ", sizeof(jinfo.xnum)); //2015.04.08 KSJ Cstring�� �ֱ⶧���� �ΰ��� ���� �ȵ�.
		}

		strSendData += CString((char*)&jinfo, sizeof(_jinfo));

		//�ϸ�ũ�� �����ϰ� �����ϱ� ����!
		//file.Write(pInters, sz_inters);	

		//�ϸ�ũ�� ���� ���Ϸ� ����
		struct _bookmarkinfo binfo;
		ZeroMemory(&binfo, sizeof(_bookmarkinfo));
		FillMemory(&binfo, sizeof(_bookmarkinfo), ' ');

		if(strlen(pInters->code) == 0)
		{
			binfo.bookmark[0] = '0';
		}
		else
		{
			CopyMemory(binfo.code, pInters->code, sizeof(binfo.code));
			CopyMemory(binfo.name, pInters->name, sizeof(binfo.name));
			binfo.bookmark[0] = pInters->bookmark == '1' ? '1':'0';//2015.04.03 KSJ 1�̾ƴϸ� 0���� ���ش�.

			if(pInters->bookmark == '1' || pInters->code[0] == 'm')	//2015.05.31 KSJ  å���ǵ� bookmark.i �� ����ȴ�.
				bSetBookMark = true;
		}

		file2.Write(&binfo, sizeof(_bookmarkinfo));

	}
	file2.Close();
	//file.Close();

	//2014.06.05 KSJ �ϸ�ũ�� �����Ǿ� ���� ������ ������ �ʿ䰡 ��� �����Ѵ�.
	if(!bSetBookMark) ::DeleteFile(strBook);

	char	key{};
	_trkey* trkey = (struct _trkey*)&key;
	trkey->kind = TRKEY_GRIDSAVE;
	trkey->group = m_nGridIndex;
	
	sData.SetData(trUPDOWN, key, (LPSTR)(LPCTSTR)strSendData, strSendData.GetLength(), "");

	SendTR(trUPDOWN, std::byte(US_PASS), (LPSTR)(LPCTSTR)strSendData, strSendData.GetLength(), key);



	(CGroupWnd*)m_pGroupWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
	(CGroupWnd*)m_pGroupWnd->SendMessage(WM_MANAGE, MK_PROCDLL);
	
	if (CAST_TREEID(m_kind)->kind == xINTEREST)
		m_bEditWork = true;


	CString string = "OnPortfolio\tok";
	((CGroupWnd*)m_pGroupWnd)->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR) string);
}

void CGridWnd::queryCodes(CString strGroup, CString strGroupName)
{
	m_bRTMReady = true;

	struct	_updn	updn {};
	
	CSendData	sdata;
	char	key{};
	_trkey* trkey = (struct _trkey*)&key;
		
	FillMemory((char*) &updn, sizeof(_updn), ' ');
	
	CopyMemory(updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'j';
	CopyMemory(updn.uinfo.nblc, "00001", sizeof(updn.uinfo.nblc));
	
	updn.uinfo.retc[0] = '0';

	if(strGroup == "-1")
	{
		strGroup.Format("%d", m_nGroupIndex);
		//�޺��ڽ��� 0���� ����
		strGroupName = ((CMainWnd*)m_pMainWnd)->GetGroupName(m_nGroupIndex);
	}

	m_nGroupIndex = atoi(strGroup);	//�׷��ε���

	trkey->group = m_nGridIndex;
	trkey->kind = SEND_CODES;

	strGroupName = "";
	CopyMemory(updn.ginfo.gnox, strGroup, sizeof(updn.ginfo.gnox));
	CopyMemory(updn.ginfo.gnam, strGroupName, sizeof(updn.ginfo.gnam));
	CopyMemory(updn.ginfo.jrec, "0000", sizeof(updn.ginfo.jrec));

	SendTR(trUPDOWN, std::byte(US_PASS), (char*)&updn, sz_updn, key);

	ResetScrollBars();
	Refresh();
}

//strCodes�� �����ڵ���̿� P_DELI�� �־���� �Ѵ�.
void CGridWnd::queryGrid(CString strCodes)
{
	int	sendL = 0;
	char	tempB[64]{};
	const int	bufSize = 100000;
	
	std::string sendB;
	sendB.resize(bufSize);
	int nStart = 0, nEnd = 0;
	
	FillMemory(sendB.data(), bufSize, ' ');
	
	sprintf(tempB, "%s%c%d%c", "90991", P_DELI, 1, P_TAB);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", "$90303", P_DELI);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct	_gridHi* gridHi;
	
	gridHi = (struct _gridHi *) &sendB[sendL];
	sendL += sizeof(struct _gridHi);
	
	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", GetRowCount());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));
	
	gridHi->type = '0';
	gridHi->dir  = '1';
	gridHi->sort = '0';

	if(!m_bContinue)	//ó���̰ų�, ������ �ƴҶ� : �÷������� ���� 
	{
		if(GetColumnCount() > GRIDMAXNUM)	//�÷������� 31���� ������ : ������ȸ�� �ְ�, 0���� 31����
		{
			m_bContinue = TRUE;
			nStart = 0;
			nEnd = GRIDMAXNUM;
		}
		else											// ������ȸ�� ����, 0���� ������
		{
			m_bContinue = FALSE;
			nStart = 0;
			nEnd = GetColumnCount();
		}
	}
	else				// ������ȸ�϶� 31���� ������. �÷��߰��� ������ �ֱ⶧���� ������.
	{
		m_bContinue = FALSE;	//������ ��ȸ�̱⶧���� �ʱ�ȭ���ش�.
		nStart = GRIDMAXNUM;
		nEnd = GetColumnCount();
	}


	for(int ii = nStart; ii < nEnd; ii++)
	{
		sprintf(tempB, "%s%c", GetCell(0, ii)->GetSymbol(), P_NEW);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "%s%c", "1301", P_DELI);				// �����ڵ�
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	if(strCodes.IsEmpty())
	{		
		for(auto item : m_inters)
		{
			CString sCode = item->code;
			sCode.Trim();

			sprintf(tempB, "%s%c", sCode.IsEmpty() ? " " : sCode, P_DELI);
			CopyMemory(&sendB[sendL], tempB, strlen(tempB));
			sendL += strlen(tempB);
		}
	}
	else
	{
		sprintf(tempB, "%s", strCodes.GetString());
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sendB[sendL] = P_NEW; 
	sendL += 1;
	sendB[sendL] = P_TAB; 
	sendL += 1;
	sendB[sendL] = 0x00;

	char	key{};
	_trkey* trkey = (struct _trkey*)&key;
	
	trkey->group = m_nGridIndex;
	trkey->kind = TRKEY_GRIDNEW;

	SendTR("pooppoop", std::byte(US_PASS), sendB.data(), sendL, key);
}

void CGridWnd::parsingGrid(char *datB, int datL)
{
	int nStart = 0, nEnd = 0;

	if(!m_bContinue)	//������ȸ�� ������ �ٽ� ��û�Ѵ�.
	{
		if(m_bSecond)
		{
			nStart = GRIDMAXNUM;
			nEnd = GetColumnCount();
		}
		else
		{
			nStart = 0;
			nEnd = GetColumnCount();
		}
	}
	else
	{
		if(m_bSecond)
		{
			nStart = GRIDMAXNUM;
			nEnd = GetColumnCount();
		}
		else
		{
			nStart = 0;
			nEnd = GRIDMAXNUM;
		}
	}

	CString strTime = ((CMainWnd*)m_pMainWnd)->GetMarketTime();
	CString strTemp;
	
	if(!strTime.IsEmpty())
	{		
		strTime.TrimLeft(); strTime.TrimRight();
		
		strTime.Replace("+", "");
		strTime.Replace("-", "");
		strTime.Replace(" ", "");
		
		const char ch = 0x7e;
				
		m_strBeginTime = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
		m_strEndTime = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);
		// ���ð� ����
	}


	CString strData, strLine, strField, bongdata, entry, strCode, strTodayVolumn;
	CString smapData;
	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL) return;

	strData = CString(&datB[gridHoL], datL - gridHoL);	//_gridHo size ����� ������� �ʱ⶧���� ������ �������


	LOGFONT lf{}, lfB{};
	const int fontsize = ((CMainWnd*)m_pMainWnd)->GetFontSize();
	CString	fontname = ((CMainWnd*)m_pMainWnd)->GetFontName();

	const BOOL	bCurrBold = ((CMainWnd*)m_pMainWnd)->GetCurrBold();
	const BOOL	bNameBold = ((CMainWnd*)m_pMainWnd)->GetNameBold();
	const BOOL	bAllBold = ((CMainWnd*)m_pMainWnd)->GetAllBold();

	CAxisExt::m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, false, true);
	
	CAxisExt::m_pFont->GetLogFont(&lf); lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	m_pFontB->GetLogFont(&lfB); lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	m_mapCurValue.RemoveAll();

	_mapAlert.clear();
	if(!strData.IsEmpty())
	{
		for(int ii = 0; ii < gsl::narrow_cast<int>(m_inters.size()); ii++)	//å���Ƕ�����
		{
			strTodayVolumn = "";
			strLine = IH::Parser(strData, PNEW);
			smapData = strLine;
			//2013.07.04 KSJ �ʵ尡 GRIDMAXNUM(27)���� ������ 2�� ������ ��ȸ�Ѵ�.
			//�׷��⶧���� ù���� �����Ϳ��� 27��°������ �̾Ƽ� �޵����Ͷ� ���̸�
			//�����ѵ����Ͱ� �ȴ�.
			strCode = GetItemText(ii + 1, colCODE);
			_mapAlert.emplace(std::make_pair(strCode, 0));
			if(m_bContinue && m_bSecond)
			{
				CString strValue;
				strCode = GetItemText(ii + 1, colCODE);
				
				if(m_mapCurValue.Lookup(strCode, strValue))
				{
					int nIndex = 0, nCount = 0;
					
					while(nIndex != -1)
					{
						nIndex = strValue.Find("\t", nIndex+1);
						
						if(++nCount == GRIDMAXNUM)
						{
							strValue = strValue.Left(nIndex+1);
							break;
						}
					}
					
					strValue += strData;
					m_mapCurValue.SetAt(strCode, strValue);
				}
			}

			const BOOL bExpect = FALSE;

			if(!strLine.IsEmpty())
			{
				CString tempOnebyte = strLine.Left(1);
				tempOnebyte.TrimLeft();
				
				for(int jj = nStart; jj < nEnd; jj++)
				{
					entry = IH::Parser(strLine, PTAB);
					//entry.Empty();

					auto& pinters = m_inters.at(ii);
					const _gridHdr gridHdr = m_gridHdrX.GetAt(jj);

					SetItemAttr(ii + 1, jj, GetItemAttr(0, jj));
					SetItemSymbol(ii + 1, jj, GetItemSymbol(0, jj));
					SetItemFont(ii + 1, jj, &lf);

					strField = entry;
					
					strField.TrimRight();
					strField.TrimLeft();
					
					if(strField == "0" || strField == "-0"|| strField == "0.00" || strField == "+0" || strField == "+0.00" || strField == "-0.00" || strField == ".00")	//2012.07.17 �ɼ��϶��� ��� �߰�
					{
						entry = " ";
					}
					
					if (jj == colCODE && !entry.IsEmpty())
					{
						CString	sztmp1, sztmp2;
						sztmp1 = GetItemText(ii + 1, 0);
						
						sztmp2.Format("%d", ii);
						m_arExist.SetData(sztmp1, sztmp2);
												
						//2012.04.03 KSJ ó�� ���簡�� �����͵��� ����.
						m_mapCurValue.SetAt(strCode, smapData);
					}

					if(jj == colEXPECT)
					{
						SetItemText(ii + 1, jj, strField);
					}
					else if ((jj == colCODE) || (jj == colNAME) || (jj == colCURR) || bAllBold)
						// END MODIFY
					{	// 20070706 kwon					
						// MODIFY PSH 20070917
						//if (bBold)
						if ((jj == colCODE) || (jj == colCURR && bCurrBold) || (jj == colNAME && bNameBold) || bAllBold)
							// END MODIFY
						{
							//gvitem.attr &= ~GVAT_CONDITIONx;
							SetItemFont(ii + 1, jj, &lfB);
						}
						//			else
						//				gvitem.attr |= GVAT_CONDITIONx;
					}
					else if(jj != colNAME)
					{				
						if ((strField == "0") || (strField == " 0") || (strField == "-0") || (strField == "+0") || (strField == "") || (strField == "0.00") || (strField == "+0.00"))
							strField = " ";

						SetItemText(ii + 1, jj, strField);
					}
					
					if (jj == colCODE)
					{
						switch (m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)entry))
						{
						case kospiType:
							SetItemData(ii+1, jj, 1);
							break;
						case kosdaqType:
							SetItemData(ii+1, jj, 0);
							break;
						default :
							SetItemData(ii+1, jj, 2);
						}
					}

					if (!(gridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
					{
						if ((strcmp("2029", gridHdr.symbol) == 0) ||
							(strcmp("2030", gridHdr.symbol) == 0) ||
							(strcmp("2031", gridHdr.symbol) == 0) )
						{
							if ((1 == gridHdr.needs) || (3 == gridHdr.needs))
							{
								CString strVal  = entry;
								CString strDiff = strVal;
								
								if (strVal[0] == '+' || strVal[0] == '-')
								{
									strDiff = strVal.Mid(1);
								}
								
								double dDiffOpen = 0.0;
								double dPClose{}, dVal{};
								dVal	= atof(strDiff.GetBuffer(0));
								dPClose = atof( (GetItemText(ii + 1, colPCURR)).GetBuffer(0) );
								
								if (dVal != 0 && dPClose != 0)
								{
									dDiffOpen = (dVal - dPClose) / dPClose * 100;
									
									if (1 == gridHdr.needs)
									{
										if (0 > dDiffOpen)
										{
											entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
										}
										else
										{
											entry.Format("%s( %0.2f%c)", strVal, dDiffOpen, P_PER);
										}
									}
									else if (3 == gridHdr.needs)
									{
										if (0 > dDiffOpen)
										{
											entry.Format("%0.2f%c", dDiffOpen, P_PER);
										}
										else if (0 == dDiffOpen)       
										{
											entry.Format(" %0.2f%c", dDiffOpen, P_PER);
										}
										else
										{
											entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
										}
									}
								}
							}
						}				
					}

					if(jj == colCURR)
					{
						if(strCode.GetLength() == 8 && (strCode.GetAt(0) == '4' || strCode.GetAt(0) == 'D')) //�Ļ���ǰ �ڵ尳��
						{
							if(entry.GetAt(0) == ' ')	//ó���� ��ĭ�̸� ���տ� -, + ǥ�ð� �ִ� ���̴�.
								entry.Replace(" ", "��");	//�׶��� '��' �� ���� 1�� �� �־��ش�. �����̽� �ƴ�..
							else if(entry.GetAt(0) == '0')
							{
								entry.Delete(0);
								entry.Insert(0,"��");
							}
						}
					}
					//2013.08.23 KSJ END

					if(jj == colNAME)
					{
						UINT attr{};
						
						if (pinters->gubn == 'm')
						{						
							//å���� ���·� ����
							attr = GetItemAttr(ii + 1, colNAME) | GVAT_MARKER;
							SetItemAttr(ii + 1, colNAME, attr);
							SetItemText(ii + 1, colCODE, pinters->code);
							SetItemText(ii + 1, colNAME, pinters->name);
						}
						else
						{
							attr = GetItemAttr(ii + 1, colNAME) & ~GVAT_MARKER;
							SetItemAttr(ii + 1, colNAME, attr);
							//�ؽ�Ʈ ����
							SetItemText(ii + 1, jj, entry);
						}
					}
					else
						//�ؽ�Ʈ ����
						SetItemText(ii + 1, jj, entry);

					//���������� ����(2320�ɺ�)
					CString tempYvol;
					
					if(jj == colPCURR)
					{
						tempYvol = entry;
						tempYvol.Remove('+');
						tempYvol.Remove('-');
//						m_iYDayVol[ii][YDAYLASTVALUE] = tempYvol;						
					}
					
					
					if(jj == colVOL)
					{
						tempYvol = entry;
						tempYvol.Remove('+');
						tempYvol.Remove('-');
						strTodayVolumn = tempYvol;
					}
					
					if(jj == colCURR)
					{
						const int iHeadersymbol = atoi(gridHdr.symbol);
						
						CString tempStr;
						
						if(strCode == "001")
						{	
							m_dKospi = atof(entry);
							tempStr.Format("%.2f", m_dKospi);
							m_dKospi = atof(tempStr);
						}
						else if(strCode == "201")
						{
							m_dKosdaq = atof(entry);
							tempStr.Format("%.2f", m_dKosdaq);
							m_dKosdaq = atof(tempStr);
						}
					}

					if(m_yDayVolField >= 0)
					{		
						if(m_yDayVolField == jj)
						{
							entry.Remove('+'), entry.Remove('-');
							SetItemText(ii + 1, jj, entry);
						}
					}

					if (m_bongField >= 0)
					{
						switch (jj)
						{
						case colHIGH:
							bongdata.Format("%s%c", strField, P_TAB);
							break;
						case colLOW:
							bongdata += strField;
							break;
						}
						
						if (m_bongField == jj)
						{
							bongdata.Remove('+')/*, bongdata.Remove('-')*/;
							SetItemText(ii + 1, jj, bongdata);
						}
					}

					if (m_newCurrField >= 0)
					{
						if (m_newCurrField == jj)
						{
							entry.TrimRight();
							entry.TrimLeft();
							entry.Remove('+');
							entry.Remove('-');
							//���ϰŷ����� ����(2321�ɺ�)
//							m_iYDayVol[ii][TDAYVALUE] = entry;
						}
					}
				}
			}
		}
	}
	
	if(!m_bContinue)	//������ȸ�� ������ �ٽ� ��û�Ѵ�.
	{
		if(m_bSecond)
		{		
			m_bSecond = FALSE;
			//Refresh();
		}

		m_nType = -1;
	}
	else
	{		
		m_bSecond = TRUE;
		queryGrid();
	}
}

void CGridWnd::parsingGridOne(char *datB, int datL, int update)
{
	if (m_staticUpdate < 0)
		return;

	int nStart = 0, nEnd = m_gridHdrX.GetSize();

	if(m_bContinue)
	{
		if(m_bSecond)
		{
			m_bSecond = FALSE;
			m_bContinue = FALSE;
			
			nStart = GRIDMAXNUM;
			nEnd = m_gridHdrX.GetSize();
		}
		else
		{
			nStart = 0;
			nEnd = GRIDMAXNUM;
		}
	}

	CString strData, strLine, strField, bongdata;
	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL) 
		return;

	strData = CString(&datB[gridHoL], datL - gridHoL);	//_gridHo size ����� ������� �ʱ⶧���� ������ �������

	CString strTemp = strData;
	

	if(!strData.IsEmpty())
	{
		for(int ii = 0; ii < GetRowCount(); ii++)	//å���Ƕ�����
		{
			strLine = IH::Parser(strData, PNEW);

			if (ii < m_staticUpdate) continue;

			const BOOL bExpect = FALSE;

			auto& pinters = m_inters.at(ii);

			if(!strLine.IsEmpty())
			{
				CString tempOnebyte = strLine.Left(1);
				tempOnebyte.TrimLeft();
				
				for(int jj = nStart; jj < nEnd; jj++)
				{
					const _gridHdr gridHdr = m_gridHdrX.GetAt(jj);

					strField = IH::Parser(strLine, PTAB);

					SetItemAttr(ii + 1, jj, GetItemAttr(0, jj));
					SetItemSymbol(ii + 1, jj, GetItemSymbol(0, jj));

					if(jj == colNAME)
					{
						UINT attr{};
						
						if (pinters->gubn == 'm')
						{						
							//å���� ���·� ����
							attr = GetItemAttr(ii + 1, colNAME) | GVAT_MARKER;
							SetItemAttr(ii + 1, colNAME, attr);
							SetItemText(ii + 1, colCODE, pinters->code);
							SetItemText(ii + 1, colNAME, pinters->name);
						}
						else
						{
							attr = GetItemAttr(ii + 1, colNAME) & ~GVAT_MARKER;
							SetItemAttr(ii + 1, colNAME, attr);
							//�ؽ�Ʈ ����
							SetItemText(ii + 1, jj, strField);
						}
					}
					else if (jj == colCODE)
					{
						strField.TrimRight();
						strField.TrimLeft();

						switch (m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)strField))
						{
						case kospiType:
							SetItemData(ii+1, jj, 1);
							break;
						case kosdaqType:
							SetItemData(ii+1, jj, 0);
							break;
						default :
							SetItemData(ii+1, jj, 2);
						}
					}
					else
					{
						strField.TrimRight();
						strField.TrimLeft();

						if(strField == "0" || strField == "-0"|| strField == "0.00" || strField == "+0" || strField == "+0.00" || strField == "-0.00" || strField == ".00")	//2012.07.17 �ɼ��϶��� ��� �߰�
						{
							strField = " ";
						}

						//�ؽ�Ʈ ����
						SetItemText(ii + 1, jj, strField);
					}

					if (m_bongField >= 0)
					{
						switch (jj)
						{
						case colHIGH:
							bongdata.Format("%s%c", strField, P_TAB);
							break;
						case colLOW:
							bongdata += strField;
							break;
						}
						
						if (m_bongField == jj)
						{
							bongdata.Remove('+');// , bongdata.Remove('-');
							SetItemText(ii + 1, jj, bongdata);					
						}
					}


					//���������ʵ尡 �ִ� Ȯ��
					if (!m_posField) continue;

					// DEL PSH 20070912
					//_gridHdr gridHdr = m_gridHdrX.GetAt(jj);
					// END DEL

					if (gridHdr.needs != 9)
						continue;

					// DEL PSH 20070912
					//struct _inters* pinters = m_inters.GetAt(ii);
					// END DEL
					CString mCode, entry;
					int sizeCode;
					mCode = pinters->code;
					sizeCode = strlen(mCode);

					switch (gridHdr.symbol[3])
					{
					case '1':		// ��������
						entry = pinters->xnum;
						break;
					case '2':		// ���Դܰ�
						if (sizeCode != 0)	//2212.03.22 KSJ �ڵ尡�������� �ƹ��͵� �����ʴ´�.
							entry = pinters->xprc;
						else
							entry = "";
						break;
					case '3':		// �򰡼���
						entry = CalcuPyungaSonik(pinters.get(), GetItemText(ii + 1, colCURR));
						break;
					case '4':		// ������
						entry = CalcuSuik(pinters.get(), GetItemText(ii + 1, colCURR));
						break;
					default:
						continue;
					}

					SetItemText(ii + 1, jj, entry);
				}
				break;
			}
		}
	}
	
	if (m_ccField) calcInClient();

	if(m_bContinue)
	{
		m_bSecond = TRUE;
		sendTransactionTR(-1,GRIDMAXNUM,m_pGridArray.GetSize());
	}
	else
	{
		m_updateROW = -1;
		ReIndexing();
		Invalidate(false);
	}
}

void CGridWnd::parsingCodes(char *datB, int datL)
{
	m_inters.clear();
	m_arrBaseInters.clear();

	int	parseL = 0;
	struct fold* pFold = (struct fold *) &datB[0];
	parseL += sz_fold;
	_inters* pinters{};

	const int uInfo = sizeof(struct _uinfo);
	if (datL == uInfo) return;

	CString	gnox, gnam, jcnt;
	
	gnox = CString(pFold->kjid, sizeof(pFold->kjid)); 
	gnox.TrimRight();
	gnam = CString(pFold->name, sizeof(pFold->name)); 
	gnam.TrimRight();
	jcnt = CString(pFold->nrec, sizeof(pFold->nrec)); 
	jcnt.TrimRight();
	
	m_szTitle = gnam;

	const int gridHdrC = sizeof(gridHdr) / sizeof(_gridHdr);
	const int nLen = min(atoi(jcnt), 100);

	SetRowCount(nLen + 1);	//HEAD�� 0��°����..

	if (atoi(jcnt) == 0)	SetRowEmpty(1, true);

	CString fileBook;
	fileBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_user, m_nGroupIndex);
	
	CFile	fileB;
	int ii = 0;
	int count = 0;
	CString sBuffer;
	if (fileB.Open(fileBook, CFile::modeRead))
	{
		const int length = gsl::narrow_cast<int>(fileB.GetLength());
		const int readlen = fileB.Read(sBuffer.GetBuffer(length + 1), length + 1);
		sBuffer.ReleaseBuffer();

		if (length == readlen)
			count = length / sizeof(struct  _bookmarkinfo);
		fileB.Close();
	}
	 
	const gsl::span<struct  _bookmarkinfo> spanbook((struct  _bookmarkinfo*)sBuffer.GetBuffer(), count);	
	for (ii = 0; ii < nLen; ii++)	//å���Ƕ�����
	{
		CString code, name = _T("");
		SetRowEmpty(ii + 1);

		code = CString(pFold->jlist[ii].code, sizeof(pFold->jlist[ii].code)).Trim();
		name = GetCodeName(code);

		auto& pinters = std::make_shared<_intersx>();

		if (code.GetAt(0) == 'm')
		{
			pinters->gubn = 'm';
			SetItemAttr(ii + 1, colNAME, GetItemAttr(0, colNAME) | GVAT_MARKER);
			name = _T("");
			if (ii < count)
				name = CString(spanbook[ii].name, 32);
		}
		else
		{
			pinters->gubn = '0';
			SetItemAttr(ii + 1, colNAME, GetItemAttr(0, colNAME) & ~GVAT_MARKER);
		}

		pinters->code = code;
		pinters->name = name;

		AddInters(pinters);
		m_arrBaseInters.emplace_back(pinters);

		SetItemText(ii + 1, colCODE, code);				//HEAD�� 0��°����..
		SetItemText(ii + 1, colNAME, name);				//HEAD�� 0��°����..
		SetItemFormat(ii + 1, colNAME, gridHdr[colNAME].format);	//�̸��� ��������
	}

	for ( ; ii < MAX_LINE ; ii++ )
	{
		const auto& pinters = std::make_shared<_intersx>();
		pinters->gubn = '0';
		AddInters(pinters);
	}
	
	queryGrid();
}

void CGridWnd::RecvOper(int kind, CRecvData* rdata)
{
	struct _trkey* trkey = (struct _trkey*)&kind;
	if (trkey->group != (UINT)m_nGridIndex)
		return;
		
	switch (trkey->kind)
	{
	case TRKEY_GRIDUPJONG:
	case TRKEY_GRIDNEW:
	case SEND_GRID:
		ClearSearchMap();
		parsingGrid(rdata->data, rdata->size);
		// Map �ʱ�ȭ KSI
		ReSetSearchMap();	//2011.12.29 KSJ 
		break;
	case TRKEY_GRIDROW:
		ClearSearchMap();
		parsingGridOne(rdata->data, rdata->size, m_updateROW);
		// Map �ʱ�ȭ KSI
		ReSetSearchMap();	//2011.12.29 KSJ 
		break;
	case TRKEY_GRIDSAVE:
		Variant(guideCC, "���������� ����Ǿ����ϴ�.");
		break;
	case TRKEY_RECOMMAND:
		//parseReCommandData((char*)rdata->m_lParam, HIWORD(rdata->m_wParam));
		break;
	case SEND_CODES:
		parsingCodes(rdata->data, rdata->size);

		// Map �ʱ�ȭ KSI
		ClearSearchMap();
		ReSetSearchMap();	//2011.12.29 KSJ 
		break;
	}	
}

void CGridWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CGridCtrlExt::OnRButtonDown(nFlags, point);
}

void CGridWnd::parsingField(CString symbol, bool bDlgSetup/* = false*/)
{
	CString	string;
	const int gridHdrC = sizeof(gridHdr) / sizeof(_gridHdr);
	m_gridHdrX.RemoveAll();
	

	for (int ii = 0; ii < gridHdrC; ii++)
	{
		if (gridHdr[ii].needs == 2)
		{
			m_gridHdrX.Add(gridHdr[ii]);
		}
	}

	if (symbol.IsEmpty())
	{
		m_szFIELD = _T(""), m_szWIDTH = _T("");
		for (int ii = 0; ii < gridHdrC; ii++)
		{
			if (gridHdr[ii].needs == 1)	// default
				m_gridHdrX.Add(gridHdr[ii]);
		}
		return;
	}

	if (!symbol.IsEmpty()) IH::Parser(symbol, deliMITER);		// field name ����
	
	m_posField = false; 
	m_ccField = false;
	int	nIndex = 0;
	CString	sztmp = _T(""), sztmp2;
	CMapStringToString	mapSymbol;
	CWordArray		arWidth;
	CStringArray		arSymbol;	
	
	int	ncnt = m_gridHdrX.GetSize();
	int	wcnt = 0;
	struct _gridHdr	gHdr {};
	bool	bField = true;

	sztmp.Format("%s", symbol);
	m_szFIELD = sztmp;

	while (!sztmp.IsEmpty())
	{
		string = IH::Parser(sztmp, deliMITER);
		arSymbol.Add(string);
	}

	sztmp.Format("%s", m_szWIDTH);


	while (sztmp.GetLength())
	{
		string = IH::Parser(sztmp, ",");

		if (string.IsEmpty())
			continue;
		
		arWidth.Add(atoi(string));
	}

	wcnt = arWidth.GetSize();
	sztmp.Format("%s", m_szFIELD);
	while (sztmp.GetLength())
	{
		string = IH::Parser(sztmp, ",");
		if (string.IsEmpty())
			continue;

		mapSymbol.SetAt(string, "");
	}		

	wcnt = mapSymbol.GetCount();

	if (!bDlgSetup && wcnt == arSymbol.GetSize())
	{
		bField = true;

		CString strSym;
		for ( int ii = 0 ; ii < wcnt ; ii++ )
		{
			strSym = arSymbol.GetAt(ii);
			if (!mapSymbol.Lookup(strSym, sztmp2))
			{
				if ((strSym == "#1##") || (strSym == "#4##") ||
					(strSym == "#2##") || (strSym == "#5##") ||
					(strSym == "#3##") || (strSym == "#6##"))
				{
					continue;
				}

				bField = false;
				break;
			}			
		}	
		
		if (bField)
		{
			symbol.Format("%s", m_szFIELD);
			symbol.Replace(",", ";");
		}

	}
	
	while (!symbol.IsEmpty())
	{
		string = IH::Parser(symbol, deliMITER);
		if (string.IsEmpty()) continue;
		
		//TRACE("%s\n", string);
		for (int ii = 0; ii < gridHdrC; ii++)
		{
			
			if (gridHdr[ii].needs == 2)
				continue;

			if (string.Compare(gridHdr[ii].symbol))
				continue;

			nIndex = m_gridHdrX.Add(gridHdr[ii]);
			sztmp.Format("%d", gridHdr[ii].stid);			

			if ((gridHdr[ii].needs == 1) || (gridHdr[ii].needs == 3))
			{
				if ((string.Compare("#1##") == 0) || (string.Compare("#4##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2029");
				}
				else if ((string.Compare("#2##") == 0) || (string.Compare("#5##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2030");
				}
				else if ((string.Compare("#3##") == 0) || (string.Compare("#6##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2031");
				}
			}

			if (gridHdr[ii].needs == 9)
			{
				m_posField = true;
				
				if (string.Compare("###3") == 0)
					m_sonikField = nIndex;
				if (string.Compare("###4") == 0)
					m_suikField = nIndex;	
			}

			if (gridHdr[ii].needs == 8)
				m_ccField = true;						
			
			if (gridHdr[ii].needs == 7)
				m_bongField = nIndex;

			if (gridHdr[ii].needs == 10)
				m_yDayVolField = nIndex;

			if (gridHdr[ii].needs == 11)
				m_EqualizerField = nIndex;
			
			if (gridHdr[ii].needs == 12)
				m_recomDataField = nIndex;

			if (gridHdr[ii].needs == 13)
				m_recomBasicCostField = nIndex;

			if (gridHdr[ii].needs == 14)
				m_recomSuikrateField = nIndex;

			if (gridHdr[ii].needs == 18)
				m_recomJisuGubnField = nIndex;

			if (gridHdr[ii].needs == 19)
				m_upjongCode = nIndex;

			if (gridHdr[ii].needs == 21)
				m_newCurrField = nIndex;

			if (gridHdr[ii].needs == 22)
				m_beforeVolField = nIndex;



			break;
		}
	}

	CString strSymbol;
	if (bField)
	{		
		ncnt = m_gridHdrX.GetSize();
		wcnt = arWidth.GetSize();

		if ((ncnt - colNAME - 3) == wcnt)
		{
			for (int ii = 0 ; ii < wcnt ; ii++ )
			{
				gHdr = m_gridHdrX.GetAt(ii + colNAME);

				gHdr.width = arWidth.GetAt(ii);

				m_gridHdrX.SetAt(ii + colNAME, gHdr);
			}
		}
	}
}


bool CGridWnd::loadField(int fno /* = -1*/, bool bDlgSetup/* = false*/, int iEqualizer)
{
	CString tempFile; 
	tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);

	CFileFind finder;
	if (!finder.FindFile(tempFile))
	{
		parsingField("", bDlgSetup); 
		m_colC = m_gridHdrX.GetSize();
		return true;
	}

	if (fno < 0)
		return false;

	int	readL = 0;
	char	readB[1024];
	CString	string, fieldno;

	fieldno.Format("%02d", fno);
	
	readL  = GetPrivateProfileString("FIELDS", fieldno, "", readB, sizeof(readB), tempFile);
	
	string = CString(readB, readL);	

	 if(iEqualizer == 0)	//�Ϲ�
	{
		string += "2022;1915;";
	}
	
	string += "1918;";
	
	memset(readB, 0x00, sizeof(readB));	
	GetPrivateProfileString(m_section, "WIDTH", "", readB, sizeof(readB), m_fileCFG); 
	m_szWIDTH.Format("%s", readB);

	parsingField(string, bDlgSetup); 
	m_colC = m_gridHdrX.GetSize();
	
	WritePrivateProfileString("FIELDS", "ACTIVE", fieldno, tempFile);
	return true;
}

void CGridWnd::loadcfg()
{
	char	buf[1024];
	
	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(m_section, KEY_FIELD, "", buf, sizeof(buf), m_fileCFG); 
	m_szFIELD.Format("%s", buf);

	memset(buf, 0x00, sizeof(buf));	
	GetPrivateProfileString(m_section, KEY_WIDTH, "", buf, sizeof(buf), m_fileCFG); 
	m_szWIDTH.Format("%s", buf);

	DWORD dwRes;

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "BKGCLR", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_clrMarkerBKG = RGB(240, 240, 240);;
	}
	else
	{
		m_clrMarkerBKG = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TXTCLR", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_clrMarkerTXT = RGB(142, 142, 142);
	}
	else
	{
		m_clrMarkerTXT = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "ADDCND", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_bAddCnd = FALSE;
	}
	else
	{
		m_bAddCnd = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "CHGCND", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_bChgCnd = FALSE;
	}
	else
	{
		m_bChgCnd = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "CODEWIDTH", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_nCodeWidth = 80;
	}
	else
	{
		m_nCodeWidth = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "GRIDWIDTH", "", buf, sizeof(buf), m_fileCFG); 

	if(0 == dwRes)
	{
		m_nGridWidth = 0;
	}
	else
	{
		m_nGridWidth = atoi(buf);
		dwRes = 0;
	}

	m_clrKospi = GetPrivateProfileInt(SEC_MAIN, "KOSPI", 0, m_fileCFG);
	
	m_clrKosdaq = GetPrivateProfileInt(SEC_MAIN, "KOSDAQ", 0, m_fileCFG);

}

void CGridWnd::savecfg()
{
	CString	str;

	const UINT kind = MAKE_TREEID(xINTEREST, 0, 2, m_nGroupIndex, TD_ITEM);
	m_kind = kind;

	str.Format("%d", m_kind);
	WritePrivateProfileString(m_section, KEY_DATA, str.operator LPCTSTR(), m_fileCFG);	

	int	nWidth = 0;
	const int ncnt = m_gridHdrX.GetSize();
	struct _gridHdr	grdHdr {};
	CString	sztmp, szWIDTH = _T(""), szFIELD = _T("");

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		grdHdr = m_gridHdrX.GetAt(ii);
		nWidth = GetColumnWidth(ii);
		CString strSymbol = (CString)grdHdr.symbol;
	
		//��õ�����ʵ�� �������� �ʴ´�
		if(strSymbol.Find("191") > -1 )
			continue;

		if(strSymbol.Find("2022") > -1 )
			continue;

		if (ii >= colNAME)
		{
			sztmp.Format("%d,", nWidth);
			szWIDTH += sztmp;
		}
		else if (ii == colCODE && 0 < nWidth)
		{
			m_nCodeWidth = nWidth;
		}

		if (ii > colCURR)
		{
			sztmp.Format("%s,", grdHdr.symbol);
			if (sztmp.Compare("2111,") == 0)
				sztmp.Format("2023,");
			else if (sztmp.Compare("2115,") == 0)
				sztmp.Format("2024,");
			else if (sztmp.Compare("2116,") == 0)
				sztmp.Format("2033,");
			else if (sztmp.Compare("2112,") == 0)
				sztmp.Format("2027,");

			if (grdHdr.needs == 1)
			{
				if (sztmp.Compare("2029,") == 0)
				{
					sztmp = "#1##,";
				}
				else if (sztmp.Compare("2030,") == 0)
				{
					sztmp = "#2##,";
				}
				else if (sztmp.Compare("2031,") == 0)
				{
					sztmp = "#3##,";
				}
			}
			else if (grdHdr.needs == 3)
			{
				if (sztmp.Compare("2029,") == 0)
				{
					sztmp = "#4##,";
				}
				else if (sztmp.Compare("2030,") == 0)
				{
					sztmp = "#5##,";
				}
				else if (sztmp.Compare("2031,") == 0)
				{
					sztmp = "#6##,";
				}
			}

			szFIELD += sztmp;
		}
	}


	WritePrivateProfileString(m_section, KEY_FIELD, szFIELD.operator LPCTSTR(), m_fileCFG);
	WritePrivateProfileString(m_section, KEY_WIDTH, szWIDTH.operator LPCTSTR(), m_fileCFG);
	

	sztmp.Format("%d", m_clrMarkerBKG);
	WritePrivateProfileString(m_section, "BKGCLR", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_clrMarkerTXT);
	WritePrivateProfileString(m_section, "TXTCLR", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bAddCnd);
	WritePrivateProfileString(m_section, "ADDCND", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bChgCnd);
	WritePrivateProfileString(m_section, "CHGCND", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_nCodeWidth);
	WritePrivateProfileString(m_section, "CODEWIDTH", sztmp.operator LPCTSTR(), m_fileCFG);
	// END ADD

	//����ũ�� ����
	CRect rc;
	GetWindowRect(rc);
	sztmp.Format("%d", rc.Width());
	WritePrivateProfileString(m_section, "GRIDWIDTH", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("1");			// �϶�
	WritePrivateProfileString(m_section, "ISFIRSTGRIDSETINFO", sztmp.operator LPCTSTR(), m_fileCFG);
}

CString CGridWnd::CalMaketTime(CString strTime, bool bEnd)
{
	CString strData;
	
	strTime.Replace("��", "");
	strTime.Replace("��", "");
	
	CString strTemp;
	
	if(bEnd)
	{
		if(strTime.IsEmpty())
			strTime = "90";
		
		int nTime = atoi(strTime) -1;
		
		if(nTime%10 == 9)
			nTime -= 4;
		
		strData.Format("%03d959", nTime);	//085959, 092959, 095959
	}
	else
	{
		if(strTime.IsEmpty())
			strTime = "80";
		
		strData.Format("%03d000", atoi(strTime) + 1);	//081000, 084000, 091000
	}
	
	return strData;
}



void CGridWnd::calcInClient()
{
	CString	code;
	for (int ii = 1; ii < m_rowC; ii++)
	{
		code = GetItemText(ii, colCODE);
		code.TrimLeft(); code.TrimRight();
		if (code.IsEmpty()) continue;

		calcInClient(ii);
	}
}

void CGridWnd::calcInClient(int row)
{
	CString	string, stringx;
	double	curr{}, open{}, high{}, low{};

	curr = IH::TOfabs(GetItemText(row, colCURR));
	open = IH::TOfabs(GetItemText(row, colOPEN));
	high = IH::TOfabs(GetItemText(row, colHIGH));
	low = IH::TOfabs(GetItemText(row, colLOW));

	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		const _gridHdr gridHdr = m_gridHdrX.GetAt(ii);

		if (gridHdr.needs != 8)
			continue;

		string = _T("");

		switch (gridHdr.symbol[2])
		{
		case '1':	// �ð����(��)
			if (curr <= 0. || open <= 0.)
				break;
			string.Format("%+.0f", curr - open);
			break;
		case '2':	// �ð����(%)
			if (curr <= 0. || open <= 0.)
				break;
			string.Format("%+.2f%%", (curr - open) / curr * 100);
			break;
		case '3':	// �����(��)
			if (curr <= 0. || high <= 0.)
				break;
			string.Format("%+.0f", curr - high);
			break;
		case '4':	// �����(%)
			if (curr <= 0. || high <= 0.)
				break;
			string.Format("%+.2f%%", (curr - high) / curr * 100);
			break;
		case '5':	// �������(��)
			if (curr <= 0. || low <= 0.)
				break;
			string.Format("%+.0f", curr - low);
			break;
		case '6':	// �������(%)
			if (curr <= 0. || low <= 0.)
				break;
			string.Format("%+.2f%%", (curr - low) / curr * 100);
			break;
		}
		stringx = GetItemText(row, ii);
		if (atof(stringx) != atof(string))
		{
			SetItemText(row, ii, string);
		}
	}
}

CString CGridWnd::CalcuPyungaSonik(struct _intersx* pinters, CString curr)
{
	CString	ret = _T("");
	const double	dval1 = atof(pinters->xnum.Trim());
	const double	dval2 = atof(pinters->xprc.Trim());
	const double	dval3 = IH::TOfabs(curr);
	const double	maeipPrc = pinters->maeipPrc;
	const double    creditPrc = pinters->creditPrc;
	double	result = 0;
	double rPrice = 0;

	if (dval1 <= 0 || dval2 <= 0)
		return ret;


	//	�򰡼��� = �򰡱ݾ�(�ſ�ݾ� ����) - ���Աݾ�
	rPrice = (dval3 * dval1) - creditPrc;

	if (maeipPrc != 0)
	{
		result = rPrice - maeipPrc;
	}
	else
	{
		result = rPrice - (dval2 * dval1);
	}

	if (result > 0)
	{
		ret.Format("%+.0f", result);
	}
	else if (result < 0)
	{
		ret.Format("-%.0f", result);
	}
	return ret;
}

CString CGridWnd::CalcuSuik(struct _intersx* pinters, CString curr)
{
	CString	ret = _T("");
	const double	dval1 = atof(pinters->xnum.Trim());
	const double	dval2 = atof(pinters->xprc.Trim());
	const double	dval3 = IH::TOfabs(curr);
	const double	creditPrc = pinters->creditPrc;
	const double	maeipPrc = pinters->maeipPrc;
	double	result = 0;
//	double resultVal = 0;
	double rPrice = 0;

	if (dval1 <= 0 || dval2 <= 0)
		return ret;

	//	�򰡼��� = �򰡱ݾ�(�ſ�ݾ� ����) - ���Աݾ�
	rPrice = (dval3 * dval1) - creditPrc;

	if (maeipPrc != 0)
	{
		result = rPrice - maeipPrc;
	}
	else
	{
		result = rPrice - (dval2 * dval1);
	}

	//	ret.Format("%+.0f", result);

	//	resultVal = atof(ret);

		//  ���ͷ� = �򰡼��� * 100 / ���԰�
	if (maeipPrc != 0)
	{
		ret.Format("%+.2f", ((result * 100.0) / maeipPrc));
	}
	else
	{
		ret.Format("%+.2f", ((result * 100.0) / (dval2 * dval1)));
	}


	return ret;
}

CString CGridWnd::CalFutureEvalPrice(struct _intersx* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian)
{
	//	if (!m_dBase)	
	getBase(m_code);

	const double maeipPrc = pinters->maeipPrc;	//���忡�� ���� ���Աݾ�

	double dSonik = 0, dGubn = 1;
	bool	bMd = false;
	CString m_pprice, m_pgsonik;
	double m_pmaeip = 0;

	if (m_curr == 0)
	{
		return "";
	}

	const int kind = getJKind(m_code);
	if (m_gubn == "1")
		bMd = true;
	if (bMd)	dGubn = -1;

	m_pprice.Format("%.f", m_remian * m_curr * m_dBase);

	if (maeipPrc != 0)
	{
		m_pmaeip = maeipPrc;
	}
	else
	{
		m_pmaeip = m_remian * m_maip * m_dBase;
	}

	dSonik = atof(getFormatData(m_pprice)) - m_pmaeip;
	dSonik *= dGubn;

	if (dSonik > 0)
		m_pgsonik.Format("+%.0f", dSonik);
	else if (dSonik < 0)
		m_pgsonik.Format("-%.0f", dSonik);

	return m_pgsonik;
}

CString CGridWnd::CalFutureEvalRate(struct _intersx* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian)
{
	getBase(m_code);

	const double maeipPrc = pinters->maeipPrc;	//���忡�� ���� ���Աݾ�

	double dSonik = 0, dGubn = 1;
	bool	bMd = false;
	CString m_pprice, m_pgsonik, m_suik;
	double m_pmaeip = 0;

	if (m_curr == 0)
	{
		return "";
	}

	const int kind = getJKind(m_code);
	if (m_gubn == "1")
		bMd = true;
	if (bMd)	dGubn = -1;

	m_pprice.Format("%.f", m_remian * m_curr * m_dBase);

	if (maeipPrc != 0)
	{
		m_pmaeip = maeipPrc;
	}
	else
	{
		m_pmaeip = m_remian * m_maip * m_dBase;
	}

	dSonik = atof(getFormatData(m_pprice)) - m_pmaeip;
	dSonik *= dGubn;

	// ���ͷ�
	dSonik *= 100;
	dSonik /= m_pmaeip;//(atof(m_rprice) * m_dBase * atof(m_remian));
	m_suik.Format("%.2f", dSonik);
	if (dSonik < 0)
		m_suik = "" + m_suik;
	else if (dSonik > 0)
		m_suik = "+" + m_suik;

	return m_suik;
}

void CGridWnd::getBase(CString m_code)
{
	m_code.TrimLeft(); m_code.TrimRight();

	if (m_code.IsEmpty())
		return;
	const int nkind = getJKind(m_code);

	m_dBase = 100000;

	switch (nkind)
	{
	case JK_JCALLOPT:
	case JK_JPUTOPT:
		m_dBase = 1;
		break;
	case JK_SFUTURE:
	case JK_SFSPREAD:
		m_dBase = 10;
		break;
	case JK_FUTURE:
	case JK_SPREAD:
		m_dBase = 500000;
		break;
	case JK_STAR:
		m_dBase = 10000;
		break;
	case JK_KTBFUTURE:
		m_dBase = 1000000;
		break;
	case JK_USDFUTURE:
		m_dBase = 10000;
		break;
	case JK_GOLDFUTURE:
		m_dBase = 1000;
		break;
	}
}

CString CGridWnd::getFormatData(CString src)
{
	CString sRtn = src;
	if (!sRtn.IsEmpty() && sRtn.FindOneOf("+-") == 0)
		sRtn = sRtn.Mid(1);
	sRtn.TrimLeft();
	return sRtn;
}

int CGridWnd::getJKind(CString m_code)
{
	if (m_code.GetLength() != 8)
		return -1;
	const  TCHAR	ch1 = m_code.GetAt(0);
	const TCHAR	ch2 = m_code.GetAt(1);
	const TCHAR	ch3 = m_code.GetAt(2);

	switch (ch1)
	{
	case '1':	// future
	case 'A':  //�Ļ���ǰ �ڵ尳��
	{
		if (ch2 == '0')
		{
			if (ch3 == '1')
				return JK_FUTURE;
			else
				return JK_STAR;
		}
		else if (ch2 == '6')
			return JK_KTBFUTURE;
		else if (ch2 == '7')
			return JK_USDFUTURE;
		else if (ch2 == '8')
			return JK_GOLDFUTURE;
		else
			return JK_SFUTURE;
	}
	case '4':	// future spread
	case 'D':   //�Ļ���ǰ �ڵ尳��
		if (ch2 == '0')
			return JK_SPREAD;
		else
			return JK_SFSPREAD;
	case '2':	// call option
	case 'B':   //�Ļ���ǰ �ڵ尳��
		if (m_code.GetAt(1) == '0')
			return JK_CALLOPT;
		else
			return JK_JCALLOPT;
		break;
	case '3':	// put option
	case 'C':   //�Ļ���ǰ �ڵ尳��
		if (m_code.GetAt(1) == '0')
			return JK_PUTOPT;
		else
			return JK_JPUTOPT;
		break;
	}
	return -1;
}

