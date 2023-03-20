// ConfigHelper.h: interface for the CConfigHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGHELPER_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_)
#define AFX_CONFIGHELPER_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "Control/BitmapItemComboBox.h"
#include "../Common_ST/STConfigDef.h"

// graph region ��
class CConfigHelper
{
public:
	// ��ȣ�������
	// ��ȣ���ǥ��
	// nType  (0) : �ż�Ÿ��, (1) : �ŵ�Ÿ��
	static int cmSignal(CComboBox* pCb, int nType)
	{
		char *aStr[] = STRLIST_SIGNAL;

		int nSize = sizeof(aStr) / sizeof(char*);
		for(int i=0; i<nSize; i+=2) {
			if(nType==0)	pCb->AddString(aStr[i]);
			else pCb->AddString(aStr[i+1]);
		}

		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");

		return pCb->GetCount();
	}

	// ����ǥ��
	static void cmColor(CComboBox* pCb)
	{
	}

	// % , Point
	static int cmPercentPoint(CComboBox* pCb)
	{
		pCb->AddString("%");
		pCb->AddString("Point");

		return pCb->GetCount();
	}

	// �����޺�, ���¸�
	static int cmAcct(CComboBox* pCb, CStatic* pTxt)
	{
	}

	// ���԰���
	// nCodeType  �ֽ�(0), ����(1)
	static int cmBuy(CComboBox* pCb, int nCodeType)
	{
		pCb->AddString("���簡");
		pCb->AddString("�ż�1ȣ��");
		pCb->AddString("�ż�2ȣ��");
		pCb->AddString("�ż�3ȣ��");
		pCb->AddString("�ŵ�1ȣ��");
		pCb->AddString("�ŵ�2ȣ��");
		pCb->AddString("�ŵ�3ȣ��");

		if(nCodeType==0)
		{
			pCb->AddString("���Ѱ�");
			pCb->AddString("���Ѱ�");
		}

		return pCb->GetCount();
	}

	// û�갡��
	// nCodeType  �ֽ�(0), ����(1)
	static int cmSell(CComboBox* pCb, int nCodeType)
	{
		pCb->AddString("���簡");
		pCb->AddString("�ż�1ȣ��");
		pCb->AddString("�ż�2ȣ��");
		pCb->AddString("�ż�3ȣ��");
		pCb->AddString("�ŵ�1ȣ��");
		pCb->AddString("�ŵ�2ȣ��");
		pCb->AddString("�ŵ�3ȣ��");

		if(nCodeType==0)
		{
			pCb->AddString("���Ѱ�");
			pCb->AddString("���Ѱ�");
		}

		return pCb->GetCount();
	}

	// �ֹ����
	static int cmJumunMethod(CComboBox* pCb)
	{
		pCb->AddString("�ֹ�����(��������)");
		pCb->AddString("���ڵ��ֹ�(Ȯ�� �� �ֹ�����)");
//		pCb->AddString("�ڵ��ֹ�(Ȯ�ξ��� �ֹ�����)");

		return pCb->GetCount();
	}
	// �ֹ�����
	static int cmJumunTime(CComboBox* pCb)
	{
		pCb->AddString("���� ������");
		pCb->AddString("�� �ϼ���");

		return pCb->GetCount();
	}

	// �ֹ����۽�ȣ
	static int cmJumunStartSignal(CComboBox* pCb)
	{
		pCb->AddString("���Խ�ȣ");
		pCb->AddString("����ȣ");

		return pCb->GetCount();
	}

	// ��ȣ���߻� ���Խ�
	static void cmBuySound(CComboBox* pCb)
	{
	}

	// ��ȣ���߻� û���
	static int cmSellSound(CComboBox* pCb)
	{
	}

	// �ð�
	static int cmTimeA(CComboBox* pCb)
	{
		pCb->AddString("����");
		pCb->AddString("����");

		return pCb->GetCount();
	}

	// ���������
	static int cmAllow(CComboBox* pCb)
	{
		pCb->AddString("�ż�/�ż�û��");
		pCb->AddString("�ŵ�/�ŵ�û��");
		pCb->AddString("��� ���");

		return pCb->GetCount();
	}

	// �ߺ�����
	static int cmRepeatIn(CComboBox* pCb)
	{
		pCb->AddString("��� ����");
		pCb->AddString("�ٸ� ���Խ�ȣ�� ���");
		pCb->AddString("��� ���");

		return pCb->GetCount();
	}

	// ��������� ǥ������
	// *
	static int cmStar(CComboBox* pCb)
	{
		char *aStr[] = STRLIST_CLEAR;

		int nSize = sizeof(aStr) / sizeof(char*);
		for(int i=0; i<nSize; i++)
			pCb->AddString(aStr[i]);

		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");
		//pCb->AddString("��");

		return pCb->GetCount();
	}

};

#endif // !defined(AFX_CONFIGHELPER_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_)
