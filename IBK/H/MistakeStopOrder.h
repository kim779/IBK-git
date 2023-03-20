#pragma once
// [strPrice]
// ������ �� ���			: �Է¹��� ����
// ���尡 / ������ �� ���	: ���Ѱ�
//
// [strListedQuantity]
// �ֽ� / ELW �� ���		: �����ֽļ� (1334 Symbol Data)
// �����ɼ��� ���			: Default
#define ALLOW_ORDER		0
#define ORD_OVER_20		1
#define ORD_OVER_50		2
#include "JumunCheck.h"
/*
struct	_jinfo {
	char    codx[12];		// �����ڵ�
	char    hnam[32];		// �����
	char    jgub[1];		// �屸�� : '1':�ŷ��� '2':�ڽ���, '3' :��3����, '5':ETF
	char    size[12];		// ����Ը�
	char    k2gb[20];		// �ұ��� ����,����
	char    usun[10];		// ������/�켱��
	char    ksmm[2];		// ����
	char    amga[20];		// �׸鰡 8->20 ��������
	char    jbkm[20];		// �ں���(����:��) 12->20 ��������
	char    siga[12];		// ���Ͻð��Ѿ�(����:��)
	char    sjjs[12];		// �����ֽļ�(����:õ��)
	char	jrat[3];		// ���ű���
	char	nrat[3];		// �ſ����ű��� 20070718 
	char    krgb[12];		// "�ŷ�����", "�ŷ��ߴ�", "��������", "�̻�޵�", "��������", "����"
	char    jggb[12];		// �� ����
	char    lock[12];		// ������
	char    dyga[8];		// ��밡
	char    ujgb[12];		// ��������
	char    ujcd[6];		// RTS �����ڵ�
	char    jisu[8];		// ��������
	char    diff[8];		// �������ϴ��
	char    rate[8];		// �������
	char    gvol[12];		// �����ŷ���(����:õ��)
	char    unio[10];		// KRX 100 (0:�Ϲ� 1:��������)
	
};
#define L_jinfo		sizeof(struct _jinfo)
*/
BOOL CheckupMistakeOrder(CWnd* pWnd, double strPrice, int strQuantity, CString strCode, CString strListedQuantity = _T(""))
{
	ASSERT(pWnd != NULL);
	//ASSERT(!strPrice.IsEmpty());
	//ASSERT(!strQuantity.IsEmpty());
	ASSERT(!strCode.IsEmpty());

	CString strMsg(_T(""));
	CString strMsg2(_T(""));
	
	const long double lPrice = strPrice;//atol((LPCTSTR)strPrice);
	long double lQty = strQuantity;//atol((LPCTSTR)strQuantity);
	const ULONG lListedQty = atol((LPCTSTR)strListedQuantity) * 1000;
	long double lAmount = 0;
	const int smode = ALLOW_ORDER;


	

	UINT nType = MB_YESNO | MB_ICONINFORMATION | MB_DEFBUTTON2;
	//AfxMessageBox(strCode);
	switch (strCode.GetLength())
	{
	case 6:		// �ֽ�, ELW
		{
			lAmount = lPrice * lQty;
			/*
			CString tmp;
			tmp.Format("%f = %f * %f",lAmount, lQty, lPrice);
			AfxMessageBox(tmp);	
			*/	
			if (lAmount > 2000000000 && lAmount <= 5000000000)
				strMsg.Format("%s\n", _T("�ֹ��ݾ��� 20���� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
			else if (lAmount > 5000000000)
			{
				strMsg.Format("%s\n", _T("�ֹ��ݾ��� 50���� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
				nType = MB_OK | MB_ICONINFORMATION;
			}
			//else 
			{
				if (strCode.Left(1) == _T("5") || strCode.Left(1) == _T("6") || 
					strCode.Left(1) == _T("7") || strCode.Left(1) == _T("8"))
					return TRUE;
				if (lListedQty!=0)
				{
					lQty = lQty * 100;
					
					if (lQty > lListedQty && lQty / 3 <= lListedQty)
						strMsg2.Format("%s\n", _T("�ֹ������� �����ֽļ� ��� 1%�� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
					else if (lQty / 3 > lListedQty)
					{
						strMsg2.Format("%s\n", _T("�ֹ������� �����ֽļ� ��� 3%�� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
						nType = MB_OK | MB_ICONINFORMATION;
					}
				}
			}
		} 
		break;

	case 8:		// �����ɼ�
		{
			if (strCode.Left(1) == _T("1") || strCode.Left(1) == _T("4"))
			{
				if (lQty > 300 && lQty <= 500)
					strMsg.Format("%s\n", _T("�ֹ������� 300����� �ʰ��� �̻��ֹ��Դϴ�."));
				else if (lQty > 500)
				{
					strMsg.Format("%s\n", _T("�ֹ������� 500����� �ʰ��� �̻��ֹ��Դϴ�."));
					nType = MB_OK | MB_ICONINFORMATION;
				}
			}
			else if (strCode.Left(1) == _T("2") || strCode.Left(1) == _T("3"))
			{
				lAmount = lQty * lPrice * 100000;


				if (lAmount > 2000000000 && lAmount <= 5000000000)
					strMsg.Format("%s\n", _T("�ֹ��ݾ��� 20����� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
				else if (lAmount > 5000000000)
				{
					strMsg.Format("%s\n", _T("�ֹ��ݾ��� 50����� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
					nType = MB_OK | MB_ICONINFORMATION;
				}
			}
			else
				return TRUE;
		}	
		break;

	default:
		break;
	}
	

	if (strMsg.IsEmpty()&&strMsg2.IsEmpty())
		return TRUE;
	else
	{
		std::unique_ptr<CJumunCheck> jc = std::make_unique<CJumunCheck>();
		if ((strMsg2 != "") && (strMsg2 != ""))	strMsg = strMsg + strMsg2;
		else if (strMsg2 != "")	strMsg = strMsg2;
		
		jc->m_msg = strMsg;
		const UINT ret = jc->DoModal();

//		if (jc)	delete jc;
		//UINT ret = pWnd->MessageBox(strMsg, "�ֹ�Ȯ��", nType);

		if (ret == IDOK)
		{
			return true;
		
		} else if (ret == IDCANCEL)
		{
			return false;
		}	
		//return false;
	}
	return false;
}


BOOL multiCheckupMistakeOrder(CWnd* pWnd, double strPrice, int strQuantity, CString strCode, CString strListedQuantity = _T(""), CString strCodeName = _T(""))
//BOOL multiCheckupMistakeOrder(CWnd* pWnd, double strPrice, int strQuantity, CString strCode, CString strListedQuantity = _T(""), CString strCodeName = _T(""), CString strmmgb = "", int iMarket = 0)
{
	ASSERT(pWnd != NULL);
	//ASSERT(!strPrice.IsEmpty());
	//ASSERT(!strQuantity.IsEmpty());
	ASSERT(!strCode.IsEmpty());

	CString strMsg(_T(""));
	CString strMsg2(_T(""));
	
	const long double lPrice = strPrice;//atol((LPCTSTR)strPrice);
	long double lQty = strQuantity;//atol((LPCTSTR)strQuantity);
	const ULONG lListedQty = atol((LPCTSTR)strListedQuantity) * 1000;
	long double lAmount = 0;
	const int smode = ALLOW_ORDER;


	

	UINT nType = MB_YESNO | MB_ICONINFORMATION | MB_DEFBUTTON2;
	//AfxMessageBox(strCode);
	switch (strCode.GetLength())
	{
	case 6:		// �ֽ�, ELW
		{
			lAmount = lPrice * lQty;
			/*
			CString tmp;
			tmp.Format("%f = %f * %f",lAmount, lQty, lPrice);
			AfxMessageBox(tmp);	
			*/	
			if (lAmount > 2000000000 && lAmount <= 5000000000)
				strMsg.Format("%s[%s]�� %s\n", strCodeName, strCode,  _T("�ֹ��ݾ��� 20����� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
			else if (lAmount > 5000000000)
			{
				strMsg.Format("%s[%s]�� %s\n", strCodeName, strCode,  _T("�ֹ��ݾ��� 50����� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
				nType = MB_OK | MB_ICONINFORMATION;
			}
			
			//else 
			{
				if (strCode.Left(1) == _T("5") || strCode.Left(1) == _T("6") || 
					strCode.Left(1) == _T("7") || strCode.Left(1) == _T("8"))
					return TRUE;
				
				if (lListedQty!=0)
				{
					lQty = lQty * 100;
					
					if (lQty > lListedQty && lQty / 3 <= lListedQty)
						strMsg2.Format("%s[%s]������ %s\n", strCodeName,strCode,  _T("�ֹ������� �����ֽļ� ��� 1%�� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
					else if (lQty / 3 > lListedQty)
					{
						strMsg2.Format("%s[%s]������ %s\n", strCodeName, strCode, _T("�ֹ������� �����ֽļ� ��� 3%�� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
						nType = MB_OK | MB_ICONINFORMATION;
					}
				}
			}
		} 
		break;

	case 8:		// �����ɼ�
		{
			if (strCode.Left(1) == _T("1") || strCode.Left(1) == _T("4"))
			{
				if (lQty > 300 && lQty <= 500)
					strMsg.Format("%s\n", _T("�ֹ������� 300����� �ʰ��� �̻��ֹ��Դϴ�."));
				else if (lQty > 500)
				{
					strMsg.Format("%s\n", _T("�ֹ������� 500����� �ʰ��� �̻��ֹ��Դϴ�."));
					nType = MB_OK | MB_ICONINFORMATION;
				}
			}
			else if (strCode.Left(1) == _T("2") || strCode.Left(1) == _T("3"))
			{
				lAmount = lQty * lPrice * 100000;


				if (lAmount > 2000000000 && lAmount <= 5000000000)
					strMsg.Format("%s\n", _T("�ֹ��ݾ��� 20����� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
				else if (lAmount > 5000000000)
				{
					strMsg.Format("%s\n", _T("�ֹ��ݾ��� 50����� �ʰ��ϴ� �̻��ֹ��Դϴ�."));
					nType = MB_OK | MB_ICONINFORMATION;
				}
			}
			else
				return TRUE;
		}	
		break;

	default:
		break;
	}

	/*
	if(strmmgb == "�ż�"  && (iMarket == 6 || iMarket == 27))
	{
		CJumunCheck jc = new CJumunCheck();
	
		jc.m_msg = "ETF ETN ��������";
		UINT ret = jc.DoModal();
		if (jc)	delete jc;

		if (ret == IDCANCEL)
		{
			return false;
		}	
	}
	*/
	
	if (strMsg.IsEmpty() && strMsg2.IsEmpty())
		return TRUE;
	else
	{

//		CJumunCheck jc = new CJumunCheck();
		std::unique_ptr<CJumunCheck> jc = std::make_unique<CJumunCheck>();
		if ((strMsg2 != "") && (strMsg2 != ""))	strMsg = strMsg + strMsg2;
		else if (strMsg2 != "")	strMsg = strMsg2;
		jc->m_msg = strMsg;
		const UINT ret = jc->DoModal();
//		if (jc)	delete jc;
		//UINT ret = pWnd->MessageBox(strMsg, "�ֹ�Ȯ��", nType);
		if (ret == IDOK)
		{
			return true;
		
		} else if (ret == IDCANCEL)
		{
			return false;
		}		
		//return false;
	}

	return false;
}

//2014.05.22 KSJ �ֹ���������
class MistakeOrder
{
public:
	CString strAccn;			//���¹�ȣ	3007 �׷��ֹ����� ���
	CString strCode;			//�����ڵ�
	int nMmgb{};					//�Ÿű���
	CString strHogb;			//ȣ������
	long double lPrice{};			//�ֹ���
	long double lQuantity{};		//�ֹ�����
	CString strMaxPrc;			//���Ѱ�
	CString strMinPrc;			//���Ѱ�
	CString strListedQuantity;	//�����ֽļ�
	CString strCurPrc;			//���簡
	CString strStdPrc;			//���ذ�
	int nQtyx{};					//�ŷ��¼�	�ֽļ������� ���
	int iMarketType{};			//���屸��  KOSPI = 0, KOSDAQ = 1, ELW = 2, ETF = 3
	CString strKrgb;			//��������
	BOOL	bKospi{};
};

//0:�̻����, 1: ���, -1: ����
BOOL CheckMistakeSOrder(CWnd* pWnd, MistakeOrder misOrd)
{
	ASSERT(pWnd != NULL);
			
	int nResult = 0;
	
	BOOL bReturn = FALSE;
	CString strMsg = "";
	
	long double lTotalAmt = 0;
	const long lListedQuantity = 0;
	const long double lPrice = misOrd.lPrice;
	const long double lQty = misOrd.lQuantity;
	TCHAR cHogb{};
		
	cHogb = misOrd.strHogb.GetAt(1);
	
	if(misOrd.nMmgb == 3 && misOrd.nMmgb == 2)	//�����̸鼭 �����϶��� �˻縦 ���Ѵ�. ������ �𸣴ϱ�..
		return TRUE;

	//�ݾױ��� üũstrCode.operator LPCTSTR()
	// ���ݹ������ֹ��� ��쿡�� �ż�/�����϶��� ���Ѱ�, �ŵ��϶� ���Ѱ� ���
	//��� ���ذ��� ���� 2015.06.05 KSJ
	if((misOrd.nMmgb == 2 || misOrd.nMmgb == 3)&& (cHogb == '3' || cHogb == '5' || cHogb == '6' || cHogb == '7'))	//�ż��϶� ���Ѱ� -> 2015.06.05 ���ذ��� ����
		lTotalAmt = lQty * atol(misOrd.strStdPrc);
	else if(misOrd.nMmgb == 1 && (cHogb == '3' || cHogb == '5' || cHogb == '6' || cHogb == '7'))	//�ŵ��϶� ���Ѱ� -> 2015.06.05 ���ذ��� ����
		lTotalAmt = lQty * atol(misOrd.strStdPrc);
	else if(cHogb == '1')	//�尳����/�ð��� ����
	{
		if(!misOrd.strHogb.Compare("81"))	//�ð��� ����
			lTotalAmt = lQty * atol(misOrd.strCurPrc);
		else if(!misOrd.strHogb.Compare("61"))	//�尳����
			lTotalAmt = lQty * atol(misOrd.strStdPrc);
	}
	else
		lTotalAmt = lQty * lPrice;
		
	int nIndex = 0;
	if(misOrd.strCode.GetLength() == 7)		nIndex = 1;

	//ETF�϶� ����
	if(misOrd.strCode.GetAt(nIndex) >= '5' && misOrd.strCode.GetAt(nIndex) <= '8')
	{
		if(lTotalAmt > 2000000000 && lTotalAmt <= 5000000000)
		{
			strMsg = "[���]�ֹ��ݾ��� 20���� �ʰ��ϴ� �̻��ֹ��Դϴ�.";
			nResult = 1;
		}
		else if(lTotalAmt > 5000000000)
		{
			strMsg = "[����]�ֹ��ݾ��� 50���� �ʰ��ϴ� �̻��ֹ��Դϴ�.";
			nResult = -1;
		}
		//ELW
	}
	else
	{
		//�ֽ� �ݾ� -> 2015.06.05 30�� 60������ ����
		if(lTotalAmt > 1500000000 && lTotalAmt <= 6000000000)
		{
			strMsg = "[���]�ֹ��ݾ��� 15���� �ʰ��ϴ� �̻��ֹ��Դϴ�.";
			nResult = 1;
		}
// 		else if(lTotalAmt > 6000000000)
// 		{
// 			strMsg = "[����]�ֹ��ݾ��� 30���� �ʰ��ϴ� �̻��ֹ��Դϴ�.";
// 			nResult = -1;
// 		}


		//�ֽ� ����
// 		lListedQuantity = atol(misOrd.strListedQuantity);
// 		
// 		if(lQty > (lListedQuantity * 0.01) && lQty <= (lListedQuantity * 0.03))
// 		{
// 			strMsg += "\n[���]�ֹ������� �����ֽļ� ��� 1%�� �ʰ��ϴ� �̻��ֹ��Դϴ�.";
// 			
// 			if(nResult != -1)	nResult = 1;	//������ �����϶��� �״�� ������ �����صд�.
// 		}
// 		else if(lQty > (lListedQuantity * 0.03) && lListedQuantity > 0)
// 		{
// 			strMsg += "\n[����]�ֹ������� �����ֽļ� ��� 3%�� �ʰ��ϴ� �̻��ֹ��Դϴ�.";
// 			nResult = -1;
// 		}
	}

	if (strMsg.IsEmpty())
		bReturn = TRUE;
	else
	{
		std::unique_ptr<CJumunCheck> jc = std::make_unique<CJumunCheck>();
//		CJumunCheck jc = new CJumunCheck();
		
		jc->m_msg = strMsg;
		const UINT ret = jc->DoModal();
//		if (jc)	delete jc;
		//UINT ret = pWnd->MessageBox(strMsg, "�ֹ�Ȯ��", nType);
		if (ret == IDOK)
		{
			bReturn = TRUE;
			
		} else if (ret == IDCANCEL)
		{
			bReturn = FALSE;
		}	
		//return false;
	}

	if(misOrd.nMmgb == 2  && (misOrd.strKrgb.Find("����") >= 0 || misOrd.strKrgb.Find("����") >= 0 || misOrd.strKrgb.Find("�ܱ�") >= 0 || 
		 misOrd.strKrgb.Find("���") >= 0 || misOrd.strKrgb.Find("����") >= 0 || misOrd.strKrgb.Right(1) == "1"))  
	{
//		CJumunCheck jc = new CJumunCheck();
		std::unique_ptr<CJumunCheck> jc = std::make_unique<CJumunCheck>();

		jc->m_msg = "�����������";
//		jc.m_strGubn = misOrd.strKrgb;
		const UINT ret = jc->DoModal();
//		if (jc)	delete jc;
		//UINT ret = pWnd->MessageBox(strMsg, "�ֹ�Ȯ��", nType);
		if (ret == IDOK)
		{
			bReturn = TRUE;
			
		} else if (ret == IDCANCEL)
		{
			bReturn = FALSE;
		}	
	}
	
	return bReturn;
}

//0:�̻����, 1: ���, -1: ����
BOOL CheckMistakeFOrder(CWnd* pWnd, MistakeOrder misOrd)
{
	ASSERT(pWnd != NULL);
	
	int nResult = 0;
	
	BOOL bReturn = FALSE;
	CString strMsg ="";
	
	bool bSpread = false;
	TCHAR cSigb{};	//���屸��	����, �ɼ�
	TCHAR cHogb{};	//ȣ������
	CString sMkty;
	int nTick1 = 0, nTick2 = 0;
	long lTickWarn = 0, lTickLimit = 0;	//���, ����
	long double dCurr = 0.0;	//���簡
	long double lTotalAmt = 0;

	const long double lQty = misOrd.lQuantity;
	long double dPrc = misOrd.lPrice * 100;
	const int nQtyx = (misOrd.nQtyx > 0) ? misOrd.nQtyx:1;	//�ŷ��¼�

	cSigb = misOrd.strCode.GetAt(0);
	sMkty = misOrd.strCode.Mid(1, 2);
	cHogb = misOrd.strHogb.GetAt(1);

	if(cSigb == '1' || cSigb == '4' || cSigb == 'A' || cSigb == 'D')  //�Ļ���ǰ �ڵ尳��
	{
		if(cSigb == '4' || cSigb == 'D') bSpread = true;   //�Ļ���ǰ �ڵ尳��

		if(!sMkty.Compare("01"))	//�ڽ���200����
		{
			if(!bSpread)
			{
				if(lQty > 600 && lQty <= 1000)
				{
					strMsg += "[���]�ֹ������� 600����� �ʰ��� �̻��ֹ��Դϴ�.";
					nResult = 1;
				}
				else if(lQty > 1000)
				{
					strMsg += "[����]�ֹ������� 1000����� �ʰ��� �̻��ֹ��Դϴ�.";
					nResult = -1;
				}
			}
			else
			{
				nTick1 = 50;
				nTick2 = 100;
				lTickWarn = (long)((0.05 * nTick1) * 100);
				lTickLimit = (long)((0.05 * nTick2) * 100);

				dCurr = atof(misOrd.strCurPrc) * 100;
			}
		}
		else if(!sMkty.Compare("05"))	//�̴ϼ��� �߰�
		{
			if(!bSpread)
			{
				if(lQty > 3000 && lQty <= 5000)
				{
					strMsg += "[���]�ֹ������� 3000����� �ʰ��� �̻��ֹ��Դϴ�.";
					nResult = 1;
				}
				else if(lQty > 5000)
				{
					strMsg += "[����]�ֹ������� 5000����� �ʰ��� �̻��ֹ��Դϴ�.";
					nResult = -1;
				}
			}
			else
			{
				nTick1 = 125;
				nTick2 = 250;
				lTickWarn = (long)((0.02 * nTick1) * 100);
				lTickLimit = (long)((0.02 * nTick2) * 100);
				
				dCurr = atof(misOrd.strCurPrc) * 100;
			}
		}
		else if(!sMkty.Compare("65") || !sMkty.Compare("66") || !sMkty.Compare("67"))	//�ݸ�����
		{
			nTick1 = 50;
			nTick2 = 100;
			lTickWarn = (long)((0.01 * nTick1) * 100);
			lTickLimit = (long)((0.01 * nTick2) * 100);

			dCurr = atof(misOrd.strCurPrc) * 100;

			if(!bSpread)
			{
				if(cHogb == '3' || cHogb == '6')
				{
					//���ذ� ����
					dPrc = atof(misOrd.strStdPrc) * 100;
				}
			}
		}
		else if(!sMkty.Compare("75") || !sMkty.Compare("76") || !sMkty.Compare("77") || !sMkty.Compare("78"))	//��ȭ����	2015.11.20 KSJ ����ȭ �߰�
		{
			long double lTmpCurr =0;

			lTmpCurr = atof(misOrd.strCurPrc);

			if(bSpread)
			{
				nTick1 = 50;
				nTick2 = 100;		
			}
			else
			{
				nTick1 = 50;
				nTick2 = 100;

				if(cHogb == '3' || cHogb == '6')
				{
					//���ذ� ����
					lTmpCurr = atof(misOrd.strStdPrc);
				}
			}
		
			if(!sMkty.Compare("78"))	//2015.11.20 KSJ ����ȭ�� 0.01��
			{
				lTickWarn = (long)((0.01 * nTick1) * 100);
				lTickLimit = (long)((0.01 * nTick2) * 100);
			}
			else
			{
				lTickWarn = (long)((0.10 * nTick1) * 100);
				lTickLimit = (long)((0.10 * nTick2) * 100);
			}
			
			dCurr = lTmpCurr * 100;
		}
		else if("10" <= sMkty && sMkty <= "59" || "B0" <= sMkty && sMkty <= "ZZ")	//�ֽļ���	DLL���� �ֽļ��� �ֹ�����. ���߿� ������ �����ؾ���.
		{
			if(!bSpread)
			{
				if(cHogb == '3' || cHogb == '5' || cHogb == '6')
				{
					dPrc = atof(misOrd.strStdPrc);
					//���ذ� ����
				}

				lTotalAmt = lQty * dPrc * nQtyx /100;	//2015.10.21 KSJ �ֽļ����� ���ݿ� ������ *100 �������Ƿ� /100 ���ش�.

// 				CString strTemp;
// 				strTemp.Format("[KSJ] lTotalAmt[%.2f] lQty[%.2f] dPrc[%.2f] nQtyx[%d]", lTotalAmt, lQty, dPrc, nQtyx);
// 				OutputDebugString(strTemp);

				if(lTotalAmt > 500000000 && lTotalAmt <= 1000000000)
				{
					strMsg += "[���]�ֹ��ݾ��� 5���� �ʰ��ϴ� �̻��ֹ��Դϴ�.";
					nResult = 1;
				}
				else if(lTotalAmt > 1000000000)
				{
					strMsg += "[����]�ֹ��ݾ��� 10���� �ʰ��ϴ� �̻��ֹ��Դϴ�.";
					nResult = -1;
				}
			}
			else
			{
				dPrc = dPrc / 100;

				nTick1 = 50;
				nTick2 = 100;
				
				int nHogaPrc = 0;

				if(misOrd.bKospi)
				{
					if(dPrc < 10000)					nHogaPrc = 10;
					else if(dPrc < 50000)				nHogaPrc = 50;
					else if(dPrc < 100000)				nHogaPrc = 100;
					else if(dPrc < 500000)				nHogaPrc = 500;
					else								nHogaPrc = 1000;
				}
				else
				{
					if (dPrc <= 1000)			nHogaPrc = 1;
					else if (dPrc <= 5000)		nHogaPrc = 5;
					else if (dPrc <= 10000)		nHogaPrc = 10;
					else if (dPrc <= 50000)		nHogaPrc = 50;
					else if (dPrc > 50000)		nHogaPrc = 100;
				}

				lTickWarn = 50 * nHogaPrc;
				lTickLimit = 100 * nHogaPrc;

				dCurr = atof(misOrd.strCurPrc);
			}
		}		

		if(cHogb != '3' && cHogb != '6' && lTickWarn > 0 && lTickLimit > 0)
		{
			CString strTmpMsg;
			if((dPrc < (dCurr - lTickLimit - 0.00001)) || (dPrc > (dCurr + lTickLimit + 0.00001)))
			{
				strTmpMsg.Format("[����]�ֹ������� %d ƽ�� ��� �̻��ֹ��Դϴ�.", nTick2);
				strMsg += strTmpMsg;
				nResult = -1;
			}
			else if((dPrc < (dCurr - lTickWarn - 0.00001)) || (dPrc > (dCurr + lTickWarn + 0.00001)))
			{
				strTmpMsg.Format("[���]�ֹ������� %d ƽ�� ��� �̻��ֹ��Դϴ�.", nTick1);
				strMsg += strTmpMsg;

				if(nResult != -1)	nResult = 1;	//������ �����϶��� �״�� ������ �����صд�.
			}
		}
	}
	else if(cSigb == '2' || cSigb == '3' || cSigb == 'B' || cSigb == 'D')	// �ڽ���200 ��/ǲ �ɼ�  //�Ļ���ǰ �ڵ尳��
	{
		if(cHogb == '3' || cHogb == '5' || cHogb == '6')	//���尡, ������, ���Ǻ�
		{
			dPrc = atof(misOrd.strStdPrc) * 100;
			//���ذ� ����
		}

		if(sMkty == "05")
			lTotalAmt = lQty * dPrc * 500;
		else
			lTotalAmt = lQty * dPrc * 2500;

		if(lTotalAmt > 2000000000 && lTotalAmt <= 5000000000)
		{
			strMsg += "[���]�ֹ��ݾ��� 20���� �ʰ��� �̻��ֹ��Դϴ�.";
			nResult = 1;
		}
		else if(lTotalAmt > 5000000000)
		{
			strMsg += "[����]�ֹ��ݾ��� 50���� �ʰ��� �̻��ֹ��Դϴ�.";
			nResult = -1;
		} 
	}
	
	if (strMsg.IsEmpty())
		bReturn = TRUE;
	else
	{
//		CJumunCheck jc = new CJumunCheck();
		std::unique_ptr<CJumunCheck> jc = std::make_unique<CJumunCheck>();

			
		jc->m_msg = !misOrd.strAccn.IsEmpty() ? "���¹�ȣ[" + misOrd.strAccn + "]\n\n" + strMsg:strMsg;
		const UINT ret = jc->DoModal();
//		if (jc)	delete jc;
		//UINT ret = pWnd->MessageBox(strMsg, "�ֹ�Ȯ��", nType);
		if (ret == IDOK)
		{
			bReturn = TRUE;
			
		} else if (ret == IDCANCEL)
		{
			bReturn = FALSE;
		}	
		//return false;
	}
	
	return bReturn;
}

BOOL CheckupMistakeOrder(CWnd* pWnd, MistakeOrder misOrd)
{
	if(misOrd.strCode.GetLength() == 8)
	{
		return	CheckMistakeFOrder(pWnd, misOrd);
	}
	else if (misOrd.strCode.GetLength() < 8 && misOrd.strCode.GetLength() >= 6)
	{
		return	CheckMistakeSOrder(pWnd, misOrd);
	}

	return FALSE;
}
//KSJ END