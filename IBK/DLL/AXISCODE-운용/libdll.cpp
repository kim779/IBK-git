#include "stdafx.h"
#include "AxisCode.h"
#include "libdll.h"

#include "USCodeDlg.h"
#include "jongmukdlg.h"
#include "futuresdlg.h"
#include "stocknewdlg.h"
#include "bonddlg.h"
#include "UpCodeDlg.h"
#include "ELWDlg.h"
#include "EtfDlg.h"
#include "CustomFuturesDlg.h"
#include "RetailBondDlg.h"
#include "CommodityDlg.h"
#include "SFCodeDlg.h"
#include "../../h/axisfire.h"

using namespace AxStd;


__declspec(dllexport) void WINAPI axCreateEx(void* root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString szRoot((char*)root);

	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	pApp->m_root = szRoot;
	pApp->LoadCode();
	pApp->LoadETFCode();
	pApp->loadCJCode();
}

__declspec(dllexport) bool WINAPI axGetCode(int kind, char *user, char* code, int type, CPoint pt)
{		
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString szCode , szName , szTemp;
	CString	szUser(user); 

	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	pApp->m_folder = user;
#ifdef _DEBUG       
//	kind = comspCODE;
#endif	
	switch (kind) 
	{
	case customCODE:
		{
			szCode.Format("%s", code);
			CCustomFuturesDlg dlg(kind, pt, type, szCode);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;

			}
			return FALSE;	
		}
	case comspCODE:
		{		
			szCode.Format("%s", code);			
			CCommodityDlg dlg( nullptr, pt );
			dlg.SetSpreadMode( TRUE );
			if( dlg.DoModal() == IDOK )
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();

				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case goodFOCODE:
		{
			szCode.Format("%s", code);			
			CCommodityDlg dlg( nullptr, pt );
			if( dlg.DoModal() == IDOK )
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();

				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	case kkCODE:
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;
				
				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case ETFCODE:
		{
			CEtfDlg dlg(pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;
				
				if (szCode.IsEmpty())
					return FALSE;
				
				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case reitsCODE:
		{
			CJongmukDlg dlg(kind, type, pt);

			if (type != reitsType && type != shipType && type != infraType && type != reitsallType)
				return FALSE;

			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;
				
				strcpy(code, szTemp);
				return TRUE;
			}

			return FALSE;
		}
		break;

	case isCODEtype: // ECN, ETF
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.IsCodeType(code, type))
				return TRUE;
		}
		break;

	case spreadCODE:
		break;

	case all2CODE:
		{
			CFuturesDlg dlg(kind, pt, type);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;

			}
			return FALSE;
		}
		break;

	case usCODE:
		{
			CUSCodeDlg dlg(pt);
			if (dlg.DoModal() == IDOK)
			{
				CString rtnData = dlg.GetRtnData();
				if (!rtnData.IsEmpty())
				{
					strcpy(code, dlg.GetRtnData());
					return TRUE;
				}
			}
			return FALSE;
		}
		break;

	case faoptionCODE:
		{	
			CFuturesDlg dlg(kind, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;

			}
			return FALSE;
		}
		break;

	case ecnCODE:
		{	
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	
	case 88:	// ���ڸ� �ڵ� ���� �ڵ�
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	case allCODE:
	case kkelwCODE:
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	
	case kospiCODE:	//Kospi && Kosdaq...
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;
				
				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	
	case thirdCODE:
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;
			
				if (szCode.IsEmpty())
					return FALSE;
					
				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}

		break;
	
	case indexCODE:	//�� ��... type�� ���� ���� ����..
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;
			
				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	case 222:	
	case futureCODE: // Futures
		{
			CFuturesDlg dlg(kind, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;				

				if (szCode.IsEmpty())
					return FALSE;				

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case optionCODE: // option
		{
			CFuturesDlg dlg(kind, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case sfCODE: // option
		{
// 			CFuturesDlg dlg(kind, pt);
// 			if (dlg.DoModal() == IDOK)
// 			{
// 				szCode = dlg.GetCode();
// 				szName = dlg.GetName();
// 				szTemp = szCode + "\t" + szName;
// 				
// 				if (szCode.IsEmpty())
// 					return FALSE;
// 				
// 				strcpy(code, szTemp);
// 				return TRUE;
// 			}
			CSFCodeDlg dlg(kind,pt);
			if(dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case foptionCODE: // Futures && option	
		{
			CFuturesDlg dlg(kind, pt, type);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;	

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case koptionCODE: // �ֽ� �ɼ�
		{
			CFuturesDlg dlg(kind, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;
				
				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case getCODEfuture: // No Dialog
		{
			CFuturesDlg dlg(kind);
			dlg.loadFjCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case getCODEcall: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			dlg.loadOjCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;
		
	case getCODEput: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			dlg.loadOjCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case sinjuCODE:	// ���� �μ���
		{
			CStockNewDlg dlg(pt, type);
			if (dlg.DoModal() == IDOK)
			{
				szName = dlg.GetName();
				szCode = dlg.GetCode();
				szTemp = szCode + "\t" + szName;
		
				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
		
	case kospi2CODE: //kospi 200
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szName = dlg.GetName();
				szCode = dlg.GetCode();
				szTemp = szCode + "\t" + szName;
			
				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case bondCODE:	//ä �� 
		{
			CBondDlg dlg(pt, type);
			if (dlg.DoModal() == IDOK)
			{
				szName = dlg.GetName();
				szCode = dlg.GetCode();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	case elwCODE:
		{
			CElwDlg dlg(pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
		}
		break;
	case smbondCODE:	//�Ҹ�ä�� 
		{
			CRetailBondDlg dlg(pt);
			if (dlg.DoModal() == IDOK)
			{
				szName = dlg.GetName();
				szCode = dlg.GetCode();
				szTemp = szCode + "\t" + szName;
				
				if (szCode.IsEmpty())
					return FALSE;
				
				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

		//2015.07.10 KSJ mini �߰�
	case getCODEminifuture: // No Dialog
		{
			CFuturesDlg dlg(kind);
			dlg.loadMfCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;
		
	case getCODEminicall: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			dlg.loadMoCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;
		
	case getCODEminiput: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			dlg.loadMoCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;

	//2016.03.11 KSJ �ֽļ��� �ڵ� �������� �κ� �߰�
	case getCODEsfuture: // No Dialog
		{
// 			CFuturesDlg dlg(kind);
// 			dlg.loadSfCode();
// 			if (dlg.FindCode(kind, szCode))
// 			{
// 				szCode = dlg.GetCode();
// 				if (szCode.IsEmpty())
// 					return FALSE;
// 				strcpy(code, szCode);
// 				return TRUE;
// 			}
			CSFCodeDlg dlg(kind);
			dlg.LoadMaster();
			if (dlg.FindCode(kind,szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;
	}

	return FALSE;
}

// íƮ�� �̴�����
__declspec(dllexport) void WINAPI axGetMiniJisu(char* link)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CUpCodeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		strcpy(link, dlg.GetRtnData());
	}
}

#include <sys/timeb.h>



__declspec(dllexport) bool WINAPI axGetName(int kind, char* code, char* name, int* type)
{
	// No Dialog...
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString szName;
	CString szCode(code); 
	szCode.Trim();
	const CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	
	if (kind == typeforTAX)  //�ŷ��� ����� ���� �߰�
	{
		CJongmukDlg dlg(116, 0);
		if (dlg.FindCode(typeforTAX, code, -1))
		{	// �ֽ�
			szName = dlg.GetName();
			CString stype;
			stype.Format("%d", dlg.GetJongmuk());
			strcpy(name, stype);
			
			*type = dlg.GetJongmuk();
			dlg.DeleteAllData();				
			return TRUE;
		}
		else if (szCode.GetLength() == 6 && (szCode[0] == '5' || szCode[0] == '6' || szCode[0] == '7'))
		{
			CString sElwCode;
			sElwCode.Format("J%s", code);
			if (dlg.FindCode(kind, sElwCode, -1))
			{
				CString stype;
				stype.Format("%d", dlg.GetJongmuk());
				strcpy(name, stype);
				
				*type = dlg.GetJongmuk();
				dlg.DeleteAllData();
				return TRUE;
			}
		} 
		*type = -1;
		return FALSE; 
	}
	

	if (kind == whichNAME)
	{
		if (szCode.GetLength() <= 0)
			return FALSE;


		if( (szCode[0] == '1' || szCode[0] == '4' || szCode[0] == 'A' || szCode[0] == 'D') && (szCode[1] == '6' ||  szCode[1] == '7' || szCode[1] == '8') )  //�Ļ���ǰ �ڵ尳��   //��ǰ���� 
		{
			kind = commodityNAME;
		}
		else if( (szCode[0] == '2' || szCode[0] == '3' || szCode[0] == 'B' || szCode[0] == 'C') && szCode[1] == '7' ) // �޷��ɼ�  //�Ļ���ǰ �ڵ尳��  //szCode[1] == '3' �� ����
		{
			kind = comoptionNAME;
		}
		else if (szCode.GetLength() == 6)
		{
			*type = 1;
			kind = codeNAME;
		}
		else if (szCode[0] == '#')	//2016.03.18 KSJ �ֽļ��� ���� �߰�
		{
			szCode.Delete(0);
			kind = getMarketsfuture;
		}
		else if (szCode.GetLength() == 9 || szCode.GetLength() == 12)
		{
			kind = bondNAME;
		}
		/*
		else if (szCode.GetLength() == 12)
		{
			kind = smbondNAME;
		}
		*/
		else if (szCode.GetLength() <= 3)
		{
			kind = indexNAME;
		}
		//2015.01.15 3010 �ֽļ��� ���� �߰�
		else if((szCode.Mid(1,2) >= "10" && szCode.Mid(1,2) <= "59") || (szCode.Mid(1,2) >= "B0" && szCode.Mid(1,2) <= "ZZ"))
		{
			*type = 6;
			kind = sfName;
		}
		else if (szCode[0] == '1' || szCode[0] == '4' || szCode[0] == 'A' || szCode[0] == 'D')
		{
			if (szCode[1] == '0')
			{
				*type = 2;
				kind = futureNAME;
			}
			else
			{
				*type = 6;
				kind = sfutureNAME;
			}
		}		
		else if (szCode[0] == '2' || szCode[0] == '3' || szCode[0] == 'B' || szCode[0] == 'C')  //�Ļ���ǰ �ڵ尳��
		{
			if (szCode[1] == '0' || (szCode[1] == 'A' && szCode[2] == 'F'))
			{
				*type = 3;
				kind = optionNAME;
			}
			else
			{
				*type = 5;	
				kind = koptionNAME;
			}
		}
		else if (!szCode.Compare("99999999"))
		{
			strcpy(name, "���ἱ��");
			return TRUE;
		}		
	}

	switch (kind)
	{
	case ETFNAME:
		{
			//CEtfDlg dlg;

		}
	case whichORDER:	// �Ÿ�����
		{
			CFuturesDlg dlg(kind);
			if (dlg.GetCodeOrderable(code, type))
				return TRUE;
		}
		break;

	case whichTYPE: // ����..
		{
			auto& map = pApp->_mapCODEx;
			if (const auto ft = map.find(szCode); ft != map.end())
			{
				const struct hjcodex* hjc = ft->second;
				szName = CString(hjc->hnam, HNameLen).Trim();
				
				switch (hjc->kosd)
				{
				case jmKOSPI:
					*type = kospiType;		break;
				case jmKOSDAQ:
					*type = kosdaqType;		break;
				case jm3RD:
					*type = thirdType;		break;
				case jmMUFND:
					//	case jmMUFND2:
					*type = mufundType;		break;
				case jmREITS:
					*type = reitsType;		break;
				}

				if (hjc->ssgb == jmETF)
					*type = etfType;
				if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS)
					*type = sinjuType;
				if (hjc->ssgb == jmHYFND)
					*type = hyfundType;
				if (hjc->ssgb == jmELW)
					*type = elwType;
				if (hjc->ssgb == jmFOREIGN)
					*type = foreignType;
				if (hjc->ssgb == jmETN)//KSJ 2014.10.28 ETN �߰�
					*type = etnType;

				//20191114 ��ȭ�鿡�� getcodetype�� ����ϴ� ���� �ʹ� ���Ƽ� kind�� typeforTAX�� �ִ� ��쿡�� Ÿ���� ����ȭ ����
				//SCREEN.GetName(199, �����ڵ�, 0)
				if (hjc->ssgb == jmKONEX && kind == typeforTAX)
					*type = KONEXType;
				strcpy(name, szName);
				return TRUE;			
			}
			else
			{
				if (szCode.GetLength() > 5)
				{
					CStringArray arrCode;
					arrCode.RemoveAll();
					arrCode.Add("165");  //3�ⱹä
					arrCode.Add("465");  //3�ⱹä ��������
					arrCode.Add("166");  //5�ⱹä
					arrCode.Add("466");  //5�ⱹä ��������
					arrCode.Add("167");  //10�ⱹä
					arrCode.Add("467");  //10�ⱹä ��������
					arrCode.Add("468");  //3~10�ⱹä

					arrCode.Add("175");  //�̱��޷�
					arrCode.Add("475");  //�̱��޷� ��������
					arrCode.Add("176");  //��
					arrCode.Add("476");  //�� ��������
					arrCode.Add("177");  //����
					arrCode.Add("477");  //���� ��������
					arrCode.Add("178");  //����
					arrCode.Add("478");  //���� ��������

					const TCHAR	ch1 = szCode.GetAt(0);
					const TCHAR	ch2 = szCode.GetAt(1);
				
					CString stmp;
					bool bfind = false;  //��ǰ���� �ڵ����� Ȯ���Ѵ�
					for (int ii = 0; ii < arrCode.GetSize(); ii++)
					{
						stmp = arrCode.GetAt(ii);
						if (stmp.Find(szCode.Left(3)) >= 0)
						{
							bfind = true;
							break;
						}
					}

					//if ((ch1 == '1' || ch1 == '4') && ch2 != '0')   //test 20230214
					if (bfind) //��ǰ�����ڵ���
					{
						CCommodityDlg dlg(0);
						dlg.LoadCCodeFromFile();

						if (dlg.FindCode(szCode))
						{
							szName = dlg.GetName();
							strcpy(name, szName);

							*type = productFutureType;
							return TRUE;
						}
					}
					else if((ch1 == '1' || ch1 == '4' || ch1 == 'A' || ch1 == 'D') && ch2 != '0')  //�Ļ���ǰ �ڵ尳��
					{
						CCustomFuturesDlg cfdlg(0);
						cfdlg.loadSfCode();
						
						if (cfdlg.FindCode(kind, code))
						{
							szName = cfdlg.GetName();
							strcpy(name, szName);

							*type = cfdlg.GetJongmuk();
							return TRUE;
						}
					}
					else
					{
						CFuturesDlg fdlg(kind);
						fdlg.loadFjCode();
						fdlg.loadOjCode();
						fdlg.loadPjCode();
						if (fdlg.FindCode(kind, code))
						{
							// ����
							szName = fdlg.GetName();
							strcpy(name, szName);

							*type = fdlg.GetJongmuk();
							return TRUE;
						}
					}
				} 
				else if (szCode.GetLength() <= 3)
				{
					CJongmukDlg dlg(kind, 0);

					// ����
					dlg.LoadUPCode();
					if (dlg.FindCode(indexNAME, code, *type))
					{
						szName = dlg.GetName();
						strcpy(name, szName);

						*type = indexType;
						dlg.DeleteAllData();
						return TRUE;
					} 
				} 

				*type = -1;
AxStd::_Msg("GetCodeTypexxxxx  [%s][%s][%d]", code, name, type);
			}
		}
		break; 

	case codeNAME:	// ����� ��ȯ...
		{
			auto& map = pApp->_mapCODEx;
			if (const auto ft = map.find(szCode); ft != map.end())
			{
				const struct hjcodex* hjc = ft->second;
				szName = CString(hjc->hnam, HNameLen).Trim();
				
				switch (hjc->kosd)
				{
				case jmKOSPI:
					*type = kospiType;		break;
				case jmKOSDAQ:
					*type = kosdaqType;		break;
				case jm3RD:
					*type = thirdType;		break;
				case jmMUFND:
					//	case jmMUFND2:
					*type = mufundType;		break;
				case jmREITS:
					*type = reitsType;		break;
				}

				if (hjc->ssgb == jmETF)
					*type = etfType;
				if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS)
					*type = sinjuType;
				if (hjc->ssgb == jmHYFND)
					*type = hyfundType;
				if (hjc->ssgb == jmELW)
					*type = elwType;
				if (hjc->ssgb == jmFOREIGN)
					*type = foreignType;
				if (hjc->ssgb == jmETN)//KSJ 2014.10.28 ETN �߰�
					*type = etnType;

				//20191114 ��ȭ�鿡�� getcodetype�� ����ϴ� ���� �ʹ� ���Ƽ� kind�� typeforTAX�� �ִ� ��쿡�� Ÿ���� ����ȭ ����
				//SCREEN.GetName(199, �����ڵ�, 0)
				if (hjc->ssgb == jmKONEX && kind == typeforTAX)
					*type = KONEXType;

				strcpy(name, szName);
				return TRUE;			
			}
		}
		break;

	case futureNAME: // ������ ��ȯ...
		{
			CFuturesDlg dlg(kind);
			dlg.loadFjCode();

			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE;
			}
		} 
		break;
	
	case sfName:
		{
// 			CFuturesDlg dlg(kind);
// 			dlg.loadSfCode();
// 
// 			if(dlg.FindCode(kind, code))
// 			{
// 				szName = dlg.GetName();
// 				strcpy(name,szName);
// 				return TRUE;
// 			}
			CSFCodeDlg dlg(kind);
			dlg.LoadMaster();
			
			if(dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE;
			}
		}
		break;

	case comspName:
		{
			CCommodityDlg dlg;
			dlg.LoadCCodeFromFile();

			if( dlg.FindOCode( code ) )
			{

				szName = dlg.GetName();
				szName = szName.Left(20);
				strcpy(name, szName);

				return TRUE;
			}
		}
		break;
	case comoptionNAME:
		{
			CCommodityDlg dlg;
			dlg.LoadCOCodeFromFile();

			if( dlg.FindOCode( code ) )
			{

				szName = dlg.GetName();
				szName = szName.Left(20);
				strcpy(name, szName);

			
				return TRUE;
			}
		}
		break;

	case commodityNAME:
		{
			CCommodityDlg dlg;
			dlg.LoadCCodeFromFile();

			if( dlg.FindCode(code) )
			{
				szName = dlg.GetName();
				szName = szName.Left(40);
				strcpy(name, szName);
	
				return TRUE;
			}
			else
			{	
			}
		}
		break;

	case optionNAME: // �ɼǸ� ��ȯ...
		{
			CFuturesDlg dlg(kind);
			dlg.loadOjCode();
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				szName = szName.Left(20);
				strcpy(name, szName);
				return TRUE;
			}
		}
		break;

	case sfutureNAME: // �ֽļ���
		{
			CCustomFuturesDlg dlg(kind);
			dlg.loadSfCode();
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				szName = szName.Left(20);
				strcpy(name, szName);
				return TRUE;
			}
		
		}
		break;

	case basicCODE:
		{
			CCustomFuturesDlg	dlg(kind);
			
			if (szCode.GetLength() == 8)				
			{
				if (dlg.FindCode(kind, szCode))
				{
					szName = dlg.GetBCode();
					strcpy(name, szName);
					return TRUE;
				}
			}			
		}

	case indexNAME:	// ������ ��ȯ...
		{
			CJongmukDlg dlg(kind, *type);
			dlg.LoadUPCode();
			if (dlg.FindCode(kind, code, *type))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				*type = 3;
				dlg.DeleteAllData();
				return TRUE;
			}
			dlg.DeleteAllData();
		}
		break;
	
	case koptionNAME: //�ֽ� �ɼǸ� ��ȯ...
		{
			CFuturesDlg dlg(kind);
			dlg.loadPjCode();
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE; 
			}
		}
		break;

	case sinjuNAME: // �����μ���...
		{
			CStockNewDlg dlg;
			dlg.loadJCode();
			CString szName = dlg.GetName(code);
			if (!szName.IsEmpty())
			{
				strcpy(name, szName);
				return TRUE;
			}
		}
		break;

	case bondNAME:	// ä��..
		{
		   CBondDlg bondDlg(kind);
//		   bondDlg.LoadCJCode();

		   BOOL bFind = bondDlg.FindCode(code);

		   if (bFind)
		   {
			szName = bondDlg.GetName();
			strcpy(name, szName);

			*type = bondDlg.GetCodeType();
		   }
		   else
		   {
			CRetailBondDlg rtBondDlg(kind);
			rtBondDlg.LoadSJCode();

			bFind = rtBondDlg.FindCode(code);

			if (bFind)
			{
			 szName = rtBondDlg.GetName();
			 strcpy(name, szName);
			}
			else
			{
			 CJongmukDlg jmDlg(kind, 0);

			 //bFind = jmDlg.FindCode(kind, code, -1);
			 bFind = jmDlg.FindCode(whichTYPE, code, -1);

			 if (bFind)
			 {
			  szName = jmDlg.GetName();
			  strcpy(name, szName);

			  /**type = jmDlg.GetJongmuk();*/

			  *type = 1;

			  /*CString str;
			  str.Format("[%d]", *type );
			   AfxMessageBox(str);*/
			 }
 
			 jmDlg.DeleteAllData();
			}
			rtBondDlg.DeleteAllData();
		   }

		   return bFind;
		  }
		  break; 	
	case smbondNAME: // �Ҹ�ä��..
		{
			CRetailBondDlg dlg(kind);
			dlg.LoadSJCode();
			if (dlg.FindCode(code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				dlg.DeleteAllData();
				return TRUE;
			}
			dlg.DeleteAllData();
		}	
		break;

		//2016.03.18 KSJ �ֽļ��� ���ϱ��� �߰�
	case getMarketsfuture: // No Dialog
		{					
// 			CString strMgubn;
// 			CFuturesDlg dlg(kind);
// 			dlg.loadSfCode();
// 			if (dlg.FindCode(kind, szCode))
// 			{
// 				strMgubn = dlg.GetMgubn();
// 
// 				if (strMgubn.IsEmpty())
// 					return FALSE;
// 				strcpy(name, strMgubn);
// 
// 				return TRUE;
// 			}
			CString strMgubn;
			CSFCodeDlg dlg(kind);
			dlg.LoadMaster();
			if (dlg.FindCode(kind, szCode))
			{
				strMgubn = dlg.GetMgubn();
				
				if (strMgubn.IsEmpty())
					return FALSE;
				strcpy(name, strMgubn);
				
				return TRUE;
			}
			return FALSE;
		}
		break;

	case kofexNAME:   
	case thirdNAME: // ��3����...
	case ecnNAME:	// ECN... 
	default:
		break;
	}
	
	return FALSE;
}


__declspec(dllexport) char* WINAPI getArray(int type = 0)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	switch (type)
	{
	case 1:
		return (char*)&pApp->_mapCODEx;
		break;
	case 2:
		return (char*)&pApp->_mapNAME;
		break;
	case 0:
	default:
		return (char*)&pApp->_mapACODE;
		break;
	}
	return nullptr;
}

__declspec(dllexport) const char* WINAPI getName(const char* sCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	auto& map = pApp->_mapCODEx;
	const auto ft = map.find(CString(sCode));
	if (ft == map.end())
		return "";

	return CString(ft->second->hnam, HNameLen).Mid(1).Trim();
}

__declspec(dllexport) const char* WINAPI getCode(const char* sName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	auto& map = pApp->_mapNAME;
	const auto ft = map.find(CString(sName));
	if (ft == map.end())
		return "";

	return ft->second.GetString();
}
