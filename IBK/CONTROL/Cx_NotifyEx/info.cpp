#include "stdafx.h"
#include "Cx_NotifyEx.h"
#include "info.h"
#include "OpSensCal.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CInfo::CInfo(CWnd *pControl)
{
	m_pControl = pControl;
	sCode = sJmm = sGubn = sRemainAmt = sPossibleAmt = sRPrice = sCurr = sRDiff = sPPrice = "";
	sCurr = sRDiff = sPPrice = sPgSonik = sSuik = sMaip = sDiff = sMchg = "";
	sDelta = sGamma = "";
	sJjTrade = sJjCalendar = sBdjs = "";
	m_noCont[0] = m_noCont[1] = 0;
	m_dBase = 0;
}

CInfo::~CInfo()
{

}

void CInfo::RemainParsing(struct f_rec *frec)
{
	sCode	= _GetString(frec->cod2, sizeof(frec->cod2));
	sJmm	= _GetString(frec->hnam, sizeof(frec->hnam));
	sGubn	= _GetString(frec->dsgb, sizeof(frec->dsgb));
	sRemainAmt = _GetString(frec->bqty, sizeof(frec->bqty));
	sPossibleAmt = _GetString(frec->cqty, sizeof(frec->cqty));
	sRPrice = _GetString(frec->pprc, sizeof(frec->pprc));
	sCurr	= _GetString(frec->curr, sizeof(frec->curr));
	sRDiff	= _GetString(frec->diff, sizeof(frec->diff));
	sPPrice	= _GetString(frec->pamt, sizeof(frec->pamt));
	sPgSonik = _GetString(frec->psam, sizeof(frec->psam));
	sSuik	= _GetString(frec->prat, sizeof(frec->prat));
	if (sSuik.GetLength() > 0 && atof(sSuik) > 0.0 && sSuik.GetAt(0) != '+')
		sSuik = "+" + sSuik;
	sMaip	= _GetString(frec->tamt, sizeof(frec->tamt));
}


void CInfo::SetNoContNew(int md, int ms)
{
	m_noCont[0] = md;
	m_noCont[1] = ms;
}

void CInfo::SetNoCont(CString gubnS, CString amtS)
{
	int idx = 0;
	int amt = atoi(amtS);
	if (gubnS.Find("�ŵ�") != -1)
		idx = 0;
	else if (gubnS.Find("�ż�") != -1)
		idx = 1;
	m_noCont[idx] += amt;
}

int CInfo::getNoCont(CString gubnS)
{
	int idx = 0;
	if (gubnS.Find("�ŵ�") != -1)
		idx = 0;
	else if (gubnS.Find("�ż�") != -1)
		idx = 1;
	return m_noCont[idx];
}

void CInfo::setNoCont(CString gubnS, CString stateS, int amt)
{
	int idx = 0;
	if (gubnS.Find("�ŵ�") != -1)
		idx = 0;
	else if (gubnS.Find("�ż�") != -1)
		idx = 1;
	
	int saveAmt = m_noCont[idx];
	
	if (!stateS.Compare("ü��"))
		saveAmt -= amt;
	else if (!stateS.Compare("���Ȯ��"))
		saveAmt -= amt;
	else if (!stateS.Compare("�ź�"))
		saveAmt -= amt;
	if (stateS.Find("����", 0) != -1 && stateS.Find("����", 0) == -1 && stateS.Find("���", 0) == -1)
		saveAmt += amt;

	m_noCont[idx] = saveAmt;
}

CString CInfo::GetTotalData()
{
	// ��ü����� : �ŵ�+�ż�
	int nMchg = 0;
	nMchg = getNoCont(sGubn=="-�ŵ�"?"+�ż�":"-�ŵ�");
	sMchg.Format("%d", nMchg);

	CString deltaS, gammaS;
	// ��Ÿ, ���� : �ŵ��� (* -1), * ���� 

	double remainAmt = atof(sRemainAmt);
	double delta = atof(sDelta) * remainAmt;
	double gamma = atof(sGamma) * remainAmt;
	
	if (sGubn.Find("�ŵ�", 0) != -1)
	{
		delta *= (-1);
		gamma *= (-1);
	}
	deltaS.Format("%.4f", delta);
	gammaS.Format("%.4f", gamma);
	
	// �ֹ����ɼ���
	CString possibleS = sPossibleAmt;
	double posibble = atof(sPossibleAmt);
	if (posibble < 0.0)	possibleS = "0";

	//              00(�����ڵ�), 01(�����), 02(����),        03(�ܰ����), 0      4(�ŵ�����),      05(��հ�),
	//		06(���簡),   07(��հ����),08(�򰡱ݾ�),     09(�򰡼���),   10(���ͷ�),     11(���Աݾ�)
	//		12(���ϴ��)   13(��ü��)   14(��Ÿ)        15(����)
	CString data = sCode + "\t" + sJmm + "\t" + sGubn + "\t" + sRemainAmt + "\t" + possibleS + "\t" + sRPrice + "\t" + 
			sCurr + "\t" + sRDiff + "\t" + sPPrice + "\t" + sPgSonik + "\t" + sSuik + "\t" + sMaip + "\t" + 
			sDiff + "\t" + sMchg +"\t" + deltaS + "\t" + gammaS + "\t";
	return data;
}

bool CInfo::CalPgsonik(CString newCurrS, CString newDiffS)
{
	if (newCurrS == sCurr && newDiffS == sDiff)
		return false;
	else if (_GetFormatData(newCurrS) == _GetFormatData(sCurr))
	{
		sDiff = newDiffS;
		return true;
	}
	sCurr = newCurrS;
	sDiff = newDiffS;
	CalPgsonik();
	return true;
}

void CInfo::CalPgsonik()
{
	// sCurr : ���簡, sRemainAmt : �ܰ����, sRPrice : ��մܰ�
	if (!m_dBase)	getBase();
	double dSonik = 0;			// ����
	double dGubn = 1;			// *-1���� : �ŵ��� ������ �ݴ�
	double dTemp = 0.0;			// ��հ����
	bool	bMd = false;
	int kind = getJKind(sCode);
	if (sGubn.Find("�ŵ�") != -1)
		bMd = true;
	
	if (bMd)	
		dGubn = -1;
		

	// �򰡱ݾ� : �ܰ���� * ���簡 * ���ذ�
	sPPrice.Format("%.f", atof(sRemainAmt) * atof(_GetFormatData(sCurr)) * m_dBase);
	// �򰡼��� : �򰡱ݾ� - ���԰�
	dSonik = atof(_GetFormatData(sPPrice)) - atof(_GetFormatData(sMaip));
	dSonik *= dGubn;
	sPgSonik.Format("%.0f", dSonik);

	// ���ͷ� : �򰡼��� / (��մܰ� * ���ذ� * �ܰ����) * 100
	dSonik *= 100;
	double div = (atof(sRPrice) * m_dBase * atof(sRemainAmt));
	if (div == 0.0)
		dSonik = 0.0;
	else
		dSonik /= div;

	sSuik.Format("%.2f", dSonik);
	if (dSonik < 0)
		sSuik = "-" + sSuik;		// ���ͷ� --�� ������ ����
	else if (dSonik > 0)
		sSuik = "+" + sSuik;

	// ��հ� ���
	dTemp = atof(_GetFormatData(sCurr)) - atof(_GetFormatData(sRPrice));
	dTemp *= dGubn;
	sRDiff.Format("%.2f", fabs(dTemp));
	sRDiff.Insert(0, CString(' ', 10 - sRDiff.GetLength()));
	if (dTemp > 0.0)
		sRDiff.SetAt(0, '+');
	else if (dTemp < 0.0)
		sRDiff.SetAt(0, '-');
}


void CInfo::CalMingam(_mingamBase *mgBase, bool bAll /*= true*/)
{
	if (!m_pOpSensCal)	return;
	
	int kind = getJKind(sCode);
	// ����, ��������
	if (kind == -1 || kind == JK_FUTURE || kind == JK_SPREAD || kind == JK_STAR)
	{
		if (!bAll)			return;
		sDelta = "5";	sGamma = "0";	return;
	}
	
	if (sJjTrade == "" && sJjCalendar == "")	// ������, �޷����� ���� ���
	{
		if (!bAll)			return;
		sDelta = "0";	sGamma = "0";	return;
	}
	// �ɼ�
	CString currS, strikeS;
	m_pOpSensCal->SetJCode(sCode);				// �����ڵ�
	double strike = getOptionStrike(sCode);			// ��簡
	strikeS.Format("%.2f", strike);
	m_pOpSensCal->SetStrike(strikeS);

	m_pOpSensCal->SetBzRemainDays(sJjTrade);		// ������
	m_pOpSensCal->SetCalRemainDays(sJjCalendar);		// �޷���
	m_pOpSensCal->SetBedangJisu(sBdjs);			// �������
	currS.Format("%.2f", fabs(atof(sCurr)));		// ���簡
	m_pOpSensCal->CalcEx(currS, mgBase->k200cur, mgBase->kcur, mgBase->time);

	sDelta = m_pOpSensCal->GetDelta();
	sGamma = m_pOpSensCal->GetGamma();

//if (sCode == "201C2237")
//	TRACE("strike[%s] ������[%s] �޷���[%s] �������[%s] ���簡[%s] k200[%s] ����[%s] time[%s] delta[%s]\n", strikeS, sJjTrade, sJjCalendar, sBdjs, sCurr, mgBase->k200cur, mgBase->kcur, mgBase->time, sDelta);
}

int CInfo::NoticeData(CMapStringToString& mapNotice, CString codeS, bool bNew)
{

	// return 0: cannot find, 1: update, 2: delete, 3: OOP Send
	sCode = codeS;								// �����ڵ�
	CString stateS, gubnS, amtS, chegS, chgaS, tmpS;
	mapNotice.Lookup(RT_STATE, stateS);					// �ֹ�����
	
	mapNotice.Lookup(RT_GUBN, gubnS);					// �ֹ�����
	if (gubnS.Find("�ŵ�") != -1)
		gubnS = "-�ŵ�";
	else
		gubnS = "+�ż�";
	mapNotice.SetAt(RT_GUBN, gubnS);

	mapNotice.Lookup(RT_CAMT, amtS);					// �ֹ�����
	setNoCont(gubnS, stateS, atoi(amtS));					// ��ü�����
	if (!m_dBase)	getBase();
	
	if (bNew)	// �ű�����
	{
		if (stateS.Compare("ü��"))
			return SM_NOT;

		mapNotice.Lookup(RT_JNAME, sJmm);				// �����
		mapNotice.Lookup(RT_GUBN, sGubn);				// �ֹ�����
		//mapNotice.Lookup(RT_JPRICE, tmpS);				// ��հ�
		sRPrice.Format("%.2f", atof(tmpS) / 100);
		mapNotice.Lookup(RT_CAMT, chegS);				// ü�����
		mapNotice.Lookup(RT_CPRICE, tmpS);	
		sRPrice.Format("%.2f", atof(tmpS) / 100);			// ��հ�
		chgaS.Format("%.2f", atof(tmpS) / 100);				// ü�ᰡ
		sMaip.Format("%.f", atof(chgaS) * atof(chegS) * m_dBase);	// ���Աݾ�
		sRemainAmt = chegS;						// ü�����
		sPossibleAmt.Format("%d", atoi(chegS) - getNoCont(sGubn=="-�ŵ�"?"+�ż�":"-�ŵ�"));
										// �ֹ����ɼ���
		return SM_INSERT;
	}
	else if (!stateS.Compare("ü��"))	// ü��
	{
		CString sNGubn, sCPrice;
		mapNotice.Lookup(RT_CAMT, chegS);				// ü�����
		mapNotice.Lookup(RT_GUBN, sNGubn);				// �ֹ�����
		mapNotice.Lookup(RT_CPRICE, tmpS);				// ü�ᰡ
		chgaS.Format("%.2f", atof(tmpS) / 100);
		sCPrice = chgaS;
		
		//=================================================================================
		// * ������ ���� ��� 
		//	- �ܰ���� = �����ܰ���� + ü�����
		//	- �ֹ����ɼ��� = ���ܰ���� - ��ü�����
		//	- ���԰� = �������԰� + ü�ᰡ * ü�ᷮ * ���ذ�
		//
		// * ������ �ݴ� ���
		//	- �ܰ���� = �����ܰ���� - ü����� (�ܰ� ���� delete)
		//	- ����ܰ������ 0���� �������(�ݴ� ������)
		//		- �ܰ���� * -1
		//		- �ֹ� ������ ���ο� ����������
		//		- �ֹ����ɼ��� = �ܰ���� - ��ü�����
		//		- ���԰� : ü�ᰡ * �ܰ���� * ���ذ�
		//	- ����ܰ������ 0���� ū���(�� �����ǿ� �߰�)
		//		- �ֹ����ɼ��� = �ܰ���� - ��ü�����
		//		- ���԰� : ���԰� * �ܰ���� / (�ܰ���� + ü�����)
		//=================================================================================
		if (sGubn == sNGubn)					
		{
			sRemainAmt.Format("%d", atoi(chegS) + atoi(sRemainAmt));
			sPossibleAmt.Format("%d", atoi(sRemainAmt) - getNoCont(sNGubn=="-�ŵ�"?"+�ż�":"-�ŵ�"));
			sMaip.Format("%.f", atof(sMaip) + atof(sCPrice) * atof(chegS) * m_dBase);
		}
		else								// ������ �ٸ� ���
		{
			sRemainAmt.Format("%.f", atof(sRemainAmt) - atof(chegS));
			if (!atoi(sRemainAmt))
				return SM_DELETE;
			else if (atoi(sRemainAmt) < 0)
			{
				sRemainAmt.Replace("-", "");
				mapNotice.Lookup(RT_GUBN, sGubn);
				sPossibleAmt.Format("%d", atoi(sRemainAmt) - getNoCont(sNGubn== "-�ŵ�" ? "+�ż�" : "-�ŵ�"));
				sMaip.Format("%.f", atof(sCPrice) * atof(sRemainAmt) * m_dBase);
			}
			else
			{
				sPossibleAmt.Format("%d", atoi(sRemainAmt) - getNoCont(sNGubn));
				double div = atof(sRemainAmt) + atof(chegS);
				if (div == 0.0)
					sMaip = "0";
				else
					sMaip.Format("%.f", atof(sMaip) * atof(sRemainAmt) / (atof(sRemainAmt) + atof(chegS)));
			}
		}
		if (atof(sRemainAmt) == 0.0)
			sRPrice = "0.00";
		else
			sRPrice.Format("%.2f", atof(sMaip) / atof(sRemainAmt) / m_dBase);		// ��հ� = ���԰�/��������/���ذ�
		CalPgsonik();
	}
	else
	{
		sPossibleAmt.Format("%d", atoi(sRemainAmt) - getNoCont(sGubn=="-�ŵ�"?"+�ż�":"-�ŵ�"));
												// �ֹ����ɼ��� = �ܰ���� - ��ü�����
	}
	return SM_UPDATE;
}

int CInfo::getJKind(CString sCode)
{
	if (sCode.GetLength() != 8)
		return -1;
	switch (sCode.GetAt(0))
	{
	case '1':	// future
	case 'A':	// �Ļ���ǰ �ڵ尳��
		{
			if (sCode.Mid(1,2) == "03")
				return JK_STAR;
			return JK_FUTURE;
		}
	case '4':	// future spread
	case 'D':
		return JK_SPREAD;
	case '2':	// call option
	case 'B':
		if (sCode.GetAt(1) == '0')
			return JK_CALLOPT;
		else
			return JK_JCALLOPT;
		break;
	case '3':	// put option
	case 'C':
		if (sCode.GetAt(1) == '0')
			return JK_PUTOPT;
		else
			return JK_JPUTOPT;
		break;
	}
	return -1;
}

double CInfo::getOptionStrike(CString opCode)
{
	double strike;
	strike = atof(opCode.Mid(5, 3));
	if (opCode[7] == '2' || opCode[7] == '7')
		strike = strike + 0.5;
	return strike;
} 

void CInfo::getBase()
{
	if (sCode.IsEmpty())	return;
	m_dBase = 100000;
	
	switch (getJKind(sCode))
	{
	case JK_JCALLOPT:
	case JK_JPUTOPT:
		m_dBase = 1;
		break;
	case JK_FUTURE:
	case JK_SPREAD:
		m_dBase = 500000;
		break;
	case JK_STAR:
		m_dBase = 10000;
		break;
	}
}


