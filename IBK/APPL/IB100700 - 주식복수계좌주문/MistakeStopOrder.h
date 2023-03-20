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
	
	long double lPrice = strPrice;//atol((LPCTSTR)strPrice);
	long double lQty = strQuantity;//atol((LPCTSTR)strQuantity);
	ULONG lListedQty = atol((LPCTSTR)strListedQuantity) * 1000;
	long double lAmount = 0;
	int smode = ALLOW_ORDER;


	

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
		CJumunCheck jc = new CJumunCheck();
		if ((strMsg2 != "") && (strMsg2 != ""))	strMsg = strMsg + strMsg2;
		else if (strMsg2 != "")	strMsg = strMsg2;
		
		jc.m_msg = strMsg;
		UINT ret = jc.DoModal();
		if (jc)	delete jc;
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
}


BOOL multiCheckupMistakeOrder(CWnd* pWnd, double strPrice, int strQuantity, CString strCode, CString strListedQuantity = _T(""), CString strCodeName = _T(""))
{
	ASSERT(pWnd != NULL);
	//ASSERT(!strPrice.IsEmpty());
	//ASSERT(!strQuantity.IsEmpty());
	ASSERT(!strCode.IsEmpty());

	CString strMsg(_T(""));
	CString strMsg2(_T(""));
	
	long double lPrice = strPrice;//atol((LPCTSTR)strPrice);
	long double lQty = strQuantity;//atol((LPCTSTR)strQuantity);
	ULONG lListedQty = atol((LPCTSTR)strListedQuantity) * 1000;
	long double lAmount = 0;
	int smode = ALLOW_ORDER;


	

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
	
	if (strMsg.IsEmpty() && strMsg2.IsEmpty())
		return TRUE;
	else
	{

		CJumunCheck jc = new CJumunCheck();
		if ((strMsg2 != "") && (strMsg2 != ""))	strMsg = strMsg + strMsg2;
		else if (strMsg2 != "")	strMsg = strMsg2;
		jc.m_msg = strMsg;
		UINT ret = jc.DoModal();
		if (jc)	delete jc;
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
}