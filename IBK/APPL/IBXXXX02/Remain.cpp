#include "stdafx.h"
#include "IBXXXX01.h"
#include "Remain.h"
#include "DefineAll.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CRemain::CRemain()
{
	m_code = m_name = m_remain = m_amt = m_pmaip = m_possible = m_pbep = "";
	m_curr = m_rprice = m_pgsonik = m_psuik = m_maip = "";
	m_date = m_creditS = m_symt = "";
	m_type = "";
	m_biyo = "";

	m_dFee =0;
	m_dMass = 0;//buffet
	m_dSave = 0;//buffet
	m_dCalcType = 0;
	m_betf = false;
	m_bchg = false;		// ü�᳻���� ���Դ��� üũ.
	
	FillString(m_date, LEN_DATE);
	FillString(m_creditS, LEN_SYGB);
	FillString(m_symt, LEN_SYMT);

	m_mapCredit.RemoveAll();
	m_mapCreditX.RemoveAll();

	// �ſ뱸��
	struct	_creditGubn
	{
		int	gubn;
		char	text[10];
		char	simple[4];
	};

	_creditGubn creditGubn[] = 
	{
		{ 1, "�������� ", "�� " },
		{ 2, "�ܱ����� ", "�� " },
		{ 3, "�ڱ����� ", "�� " },
		{ 5, "������� ", "�� " },
		{ 6, "�����ڱ� ", "�� " },
		{ 7, "�ڱ���� ", "�� " },
		{ 80, "��Ź�㺸 ", "�� " },
		{ 10, "KOSPI    ", "   " },
		{ 20, "KOSDAQ   ", "   " },
		{ 30, "OTC      ", "   " },
		{ 40, "ä��     ", "   " },
		{ 50, "���     ", "�� " },
	};
	CString tmpS;
	for (int ii = 0; ii < sizeof(creditGubn)/sizeof(_creditGubn); ii++)
	{
		tmpS = CString(creditGubn[ii].text, sizeof(creditGubn[ii].text));
		tmpS.TrimRight();
		m_mapCredit.SetAt(creditGubn[ii].gubn, tmpS);

		tmpS = CString(creditGubn[ii].simple, sizeof(creditGubn[ii].simple));
		tmpS.TrimRight();
		m_mapCreditX.SetAt(creditGubn[ii].gubn, tmpS);
	}
}

CRemain::~CRemain()
{
	m_mapCredit.RemoveAll();
	m_mapCreditX.RemoveAll();
}

BEGIN_MESSAGE_MAP(CRemain, CWnd)
	//{{AFX_MSG_MAP(CRemain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CString CRemain::TotalData()
{
	//             0:code        1:name         2:�����ܰ�          3:�ֹ��ܷ�
	//             4:������հ�       5:�ŵ������ܰ�      6:���簡       7:�򰡱ݾ�
	//             8:�򰡼���         9:�򰡼��ͷ�     10:���Աݾ�     
	//  �ſ��߰�   11:������	12:�ſ뱸��	  13:�ſ뱸�г���     14:�ſ뱸�о���	15:�ſ����ݾ�
	//				16 : �ܰ��� 17.BEP�ܰ� 18.������
	//m_biyo = "500";

	CString pmaip, data;
	pmaip.Format("%.f", atof(m_pmaip));

	data = m_code + "\t" + m_name + "\t" + m_remain + "\t" + m_amt + "\t" +
		pmaip + "\t" + m_possible + "\t" + m_curr + "\t" + m_rprice + "\t" + 
		m_pgsonik + "\t" + m_psuik + "\t" + m_maip + "\t" + 
		m_date + "\t" + m_creditS + "\t" + getCredit(m_creditS) + "\t" + getCreditX(m_creditS) + "\t" + m_symt + "\t" +
		m_type + "\t" + m_pbep + "\t" + m_biyo + "\t" + m_srate + "\t";

	return data;
}

void CRemain::ParsingRemainData(char *pData)
{
	struct s_rec srec;
	CopyMemory(&srec, pData, L_srec);
	
	m_type = CString(srec.jggb, sizeof(srec.jggb));
	m_code	= CString(srec.cod2, sizeof(srec.cod2));
	m_name	= CString(srec.hnam, sizeof(srec.hnam));
	m_remain = CString(srec.jqty, sizeof(srec.jqty));
	m_amt = CString(srec.jqty, sizeof(srec.jqty));
	m_pmaip	= CString(srec.pamt, sizeof(srec.pamt));
	m_pmaip.Format("%f",atof(m_pmaip));

	m_maip	= CString(srec.mamt, sizeof(srec.mamt));
	m_omaip.Format("%.f", atof(m_maip));
	m_psuik	= CString(srec.srat, sizeof(srec.srat));
	m_possible = CString(srec.xqty, sizeof(srec.xqty));
	m_curr	= CString(srec.curr, sizeof(srec.curr));
#ifdef SISE_EXTEND
	m_srate = CString(srec.rate, sizeof(srec.rate));
#endif
	m_rprice = CString(srec.camt, sizeof(srec.camt));
	m_pgsonik = CString(srec.tamt, sizeof(srec.tamt));
	m_code.TrimRight();
#ifdef	CREDIT
	m_date = CString(srec.sydt, sizeof(srec.sydt));
	m_creditS = CString(srec.sycd, sizeof(srec.sycd));
//	if (atoi(m_creditS) >= 10 || atoi(m_creditS) == 0)	// �ſ뱸�� �ƴϸ� 0
//		m_creditS = "  ";
	m_symt = CString(srec.samt, sizeof(srec.samt));

//TRACE("�ܰ���ȸ [%s:%s] ����[%s]\n", m_code, m_name, m_psuik);
#else
	FillString(m_date, LEN_DATE);
	FillString(m_creditS, LEN_SYGB);
	FillString(m_symt, LEN_SYMT);

#endif
/*************NEW CALC******************************/
	double forby = 0;
	forby = atof(m_rprice);
	///////////////////////////////////////////
	double dSonik;
	if ( m_dFee == 0 )
	{
		double dd;

		//pyt 11.09
		if (m_dCalcType==1)	
		{
			m_maip.Format("%.f",atof(m_maip)+atof(m_symt));//���Աݾ�
			m_rprice.Format("%.f",atof(m_rprice)+atof(m_symt));//�򰡱ݾ�

			dSonik = atof(m_rprice) - atof(m_maip);
			m_pgsonik.Format("%.f",dSonik);//�򰡼���
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;
			dd = atof(m_pgsonik)*100/(atof(m_maip));
		}
		else
		{
			dSonik = atof(m_rprice) - atof(m_maip);
			m_pgsonik.Format("%.f",dSonik);//�򰡼���
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;

			dd = atof(m_pgsonik)*100/atof(m_maip);
		}
	
		m_psuik.Format("%.2f",dd);

		if (dd > 0)	
			m_psuik = "+" + m_psuik;
	}
	/////////////////////////////////////////////
	if ( m_dFee > 0)
	{
	//	double dMaip = atof(m_maip)*(1+m_dMass) + m_dSave;//���԰�
		double	symt = atof(m_symt);
		double dMaip = atof(m_maip) + RoundDown((atof(m_maip)+symt) * m_dMass) + m_dSave;//���԰�
		double dmass = RoundDown((atof(m_rprice)+symt) * m_dMass);
		double dtax = Round((atof(m_rprice)+symt) * TAX_RATE);
			
		//������� ������ ����
		double dprice = 0;
		//if ( m_code.GetAt(0) == 'J' ) dprice = atof(m_rprice)*(1 - m_dMass ) - m_dSave;
		//else dprice = atof(m_rprice)*(1 - m_dMass - TAX_RATE ) - m_dSave;
	
		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 ) || m_betf)
		{
			dprice = atof(m_rprice) - ( dmass + m_dSave);
		}	
		else
		{
			dprice = atof(m_rprice) - (dmass + dtax + m_dSave);
		}	
		
		double dSonik;

		//pyt 11.09
		if (m_dCalcType==1)
		{	
			dMaip = dMaip + symt;
			m_maip.Format("%.f",dMaip);			//���Աݾ�
			dprice = dprice + symt;				//�򰡱ݾ�	
			m_rprice.Format("%.f",dprice);
			dSonik = (dprice) - dMaip;
		}
		else
		{		
			m_maip.Format("%.f",dMaip);
			m_rprice.Format("%.f",dprice);		//�򰡱ݾ�
			dSonik = dprice - dMaip; 
		}

		
		
		m_pgsonik.Format("%.f",dSonik);//�򰡼���
		if (dSonik > 0)	
			m_pgsonik = "+" + m_pgsonik;
		
		if ( dMaip == 0 )
			m_psuik.Format("%.2f",0);
		else
		{
			double dSuik;
			if (m_dCalcType==1)
				dSuik = dSonik*100/dMaip;//dSuik = dSonik*100/(dMaip+symt);
			else
				dSuik = dSonik*100/dMaip;

			m_psuik.Format("%.2f",dSuik);

			if (dSuik > 0)
				m_psuik = "+" + m_psuik;
		}

//		TRACE("���� %f �ſ� %f �� %f ���� %F\n", dMaip, symt, dprice, dSonik);
	}
	
	

	double dbep = atof(m_pmaip);

	double bep_mass = RoundDown2(fabs(atof(m_pmaip)) * m_dMass);
	double cur_tax = RoundDown2(fabs(atof(m_curr)) * TAX_RATE);
	double cur_mass = RoundDown2(fabs(atof(m_curr)) * m_dMass);//TAX_RATE);
	
	if ( m_dFee > 0 )
	{
	//	if ( m_code.GetAt(0) == 'J' ) dbep = fabs(atof(m_pmaip))*(1+m_dMass) + fabs(atof(m_curr))*m_dMass;
	//	else dbep = fabs(atof(m_pmaip))*(1+m_dMass) + fabs(atof(m_curr))*(m_dMass+TAX_RATE);

		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 )||m_betf)
		{
			dbep = fabs(atof(m_pmaip)) + bep_mass + cur_mass;
			m_biyo.Format("%d",(int)RoundDown2(fabs(forby) * TAX_RATE)+RoundDown2(fabs(forby) * m_dMass));
		}	
		else
		{
			dbep = fabs(atof(m_pmaip)) + bep_mass + cur_tax + cur_mass;
			m_biyo.Format("%d",(int)(RoundDown2(fabs(forby) * TAX_RATE)+RoundDown2(fabs(forby) * m_dMass) + m_dSave));
		}	
	}		

	m_pbep.Format("%.f",dbep);

	m_symt.TrimLeft(); m_symt.TrimRight();

/**************************************************/
	TRACE("PR[%x][%x] ����(%s) ����(%d) �ܰ�(%f) ���Աݾ�(%.f) �����Աݾ�(%.f) �򰡱ݾ�(%f) ü�Ῡ��(%d) ���ͷ�(%f)\n",
		this, GetCurrentThreadId(),	m_code, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), m_bchg, atof(m_psuik));
}

void CRemain::ParsingAllaccnData(CString sData)
{
	m_code	= parser(sData, "\t");
	m_name	= parser(sData, "\t");
	m_remain = parser(sData, "\t");
	m_amt = parser(sData, "\t");
	m_pmaip	= parser(sData, "\t");
	m_maip	= parser(sData, "\t");
	m_omaip = m_maip;
	m_psuik	= parser(sData, "\t");
	m_possible = parser(sData, "\t");
	m_curr	= parser(sData, "\t");
	m_rprice = parser(sData, "\t");
	m_pgsonik = parser(sData, "\t");
	m_code.TrimRight();	

	m_date = parser(sData, "\t");
	if (m_date.IsEmpty())
		FillString(m_date, LEN_DATE);

	m_creditS = parser(sData, "\t");
//	if (atoi(m_creditS) >= 10 || atoi(m_creditS) == 0)	// �ſ뱸�� �ƴϸ� 0
//		m_creditS = "  ";

	m_symt = parser(sData, "\t");
	if (m_symt.IsEmpty())
		FillString(m_symt, LEN_SYMT);
	

	//FillString(m_date, LEN_DATE);
	//FillString(m_creditS, LEN_SYGB);
	//FillString(m_symt, LEN_SYMT);

}

int CRemain::CalRemainData(CMapStringToString &symbolMap, CString keyS, bool bNewCode)
{
	CString stateS, gubnS, tmpS;
	CString nContAmt, cpriceS, strMod;
	int nMod = 1;

	CString jggbS = keyS.Left(LEN_JGGB);
	CString codeS = keyS.Mid(LEN_JGGB, LEN_HCODE);
	CString dateS = keyS.Mid(LEN_JGGB + LEN_HCODE, LEN_DATE);

	symbolMap.Lookup(RT_CTDATE, m_date);		// ������
	if (m_date.IsEmpty())
	{
		FillString(m_date, LEN_DATE);
	}

	symbolMap.Lookup(RT_STATE, stateS);
	symbolMap.Lookup(RT_GUBN, gubnS);

	symbolMap.Lookup(RT_CREDIT, m_creditS);		// �ſ뱸��
	int credit = atoi(m_creditS);

	bool bCredit = false;

//	if (credit >= 10 || credit == 0)		// �ſ뱸�� �ƴϸ� 0
//		m_creditS = "  ";
//	else
//	{
		m_creditS.Format("%02d", credit);
		bCredit = true;
//	}

	double	symt = atof(m_symt);
	double	tmpSymt = 0.0;
	CString tmpSymtS = "";
	if (symbolMap.Lookup(RT_CTSYMT, tmpSymtS))
	{
		tmpSymt = atof(tmpSymtS);
		if (tmpSymt > 0.0)
		{
			if (gubnS.Find("�ŵ�") != -1)
			{
				switch (credit)
				{
				case 1:		// ��������
				case 3:		// �ڱ�����
				case 60:	// �������Ǵ㺸����
				case 70:	// �ŵ���ݴ㺸����
				case 80:	// ��Ź�ֽĴ㺸�ſ����
				case 82:	// ��Źä�Ǵ㺸�ſ����
				//case 50:    // ���
					tmpS.Format("%.f", symt - tmpSymt);
					break;
				case 5:		// �������
				case 7:		// �ڱ����
					tmpS.Format("%.f", symt + tmpSymt);
					break;
				}
			}
			else	// �ż�
			{
				switch (credit)
				{
				case 1:		// ��������
				case 3:		// �ڱ�����
				case 60:	// �������Ǵ㺸����
				case 70:	// �ŵ���ݴ㺸����
				case 80:	// ��Ź�ֽĴ㺸�ſ����
				case 82:	// ��Źä�Ǵ㺸�ſ����
				//case 50:    // ���	
					tmpS.Format("%.f", symt/* + tmpSymt*/);
					break;
				case 5:	// �������
				case 7:	// �ڱ����
					tmpS.Format("%.f", symt - tmpSymt);
					break;
				}
			}
		
			m_symt = tmpS;
		}
	}
	if (m_symt.IsEmpty())	FillString(m_symt, LEN_SYMT);

	//symbolMap.Lookup(RT_STATE, stateS);
	//symbolMap.Lookup(RT_GUBN, gubnS);
	if (gubnS.Find("�ŵ�") != -1)
		gubnS = "-" + gubnS;
	else
		gubnS = "+" + gubnS;
	symbolMap.SetAt(RT_GUBN, gubnS);

	if (stateS.Find("�������") != -1 || stateS.Find("��������") != -1 || stateS.Find("����Ȯ��") != -1)
		return CHEG_NOT;
	
	if (bNewCode)	
	{
		if (stateS.Compare("ü��"))
			return CHEG_NOT;

		m_bchg = true;
		//==============================================
		// �ű� ü�� :
		// ��մܰ� = ü�ᰡ 
		// ���԰� = ü�ᰡ * ü�����(�ܰ�) - �ſ�����
		//==============================================
		m_code = codeS;
		m_type = jggbS;
		symbolMap.Lookup(RT_JNAME, m_name);
		m_name = " "+m_name;
		symbolMap.Lookup(RT_CAMT, m_remain);
		symbolMap.Lookup(RT_CPRICE, tmpS);
		symbolMap.Lookup(RT_MOD, strMod);
		nMod = atoi((LPCTSTR)strMod);

		if (nMod == 0)
			nMod = 1;

		cpriceS.Format("%.f", atof(tmpS) / nMod);

		m_pmaip = cpriceS;
		//CString tmp; tmp.Format("ü��: %f=%f*%f-%f\n",atof(cpriceS) * atof(m_remain)- atof(m_symt), atof(cpriceS) , atof(m_remain), atof(m_symt));
		
		m_maip.Format("%.f", atof(cpriceS) * atof(m_remain));
		
		if (bCredit)	m_maip.Format("%.f", atof(m_maip) - atof(m_symt));
		/*
		if ((atof(m_symt) != 0)&&(m_code == "A000660")&&(m_date == "20091125"))
		{
			CString tmp; tmp.Format("%s | %s\n",m_maip, m_symt);
			TRACE(tmp);
		}
		*/
		m_possible = m_remain;
		return CHEG_INSERT;
	}
	else if (!stateS.Compare("ü��"))	// ü��
	{
		m_bchg = true;
		symbolMap.Lookup(RT_CAMT, nContAmt);
		symbolMap.Lookup(RT_CPRICE, tmpS);
		symbolMap.Lookup(RT_MOD, strMod);
		nMod = atoi((LPCTSTR)strMod);

		if (nMod == 0)
			nMod = 1;

		cpriceS.Format("%.f", atof(tmpS) / nMod);

		if (gubnS.Find("�ż�") != -1)
		{
			CString maipS;

			maipS.Format("%.f", atoi(m_remain) * atof(m_pmaip) + atof(cpriceS) * atof(nContAmt));	// ���԰�
			m_remain.Format("%d", atoi(nContAmt) + atoi(m_remain));			// �ܰ����
			m_pmaip.Format("%f", atof(maipS) / atof(m_remain));			// ��հ�
			m_maip = maipS;
			//if (bCredit)
			//	m_maip.Format("%.f", atof(m_maip) - atof(m_symt));

			m_possible.Format("%d", atoi(m_possible) + atoi(nContAmt));		// ���ɼ���
		
		}
		else
		{
			// �ŵ��� ���� ����
			m_amt.Format("%d", atoi(m_amt) - atoi(nContAmt));
			m_remain.Format("%.d", atoi(m_remain) - atoi(nContAmt));
			if (!atoi(m_remain))
			{
				return CHEG_DELETE;
			} 
			else
			{
				m_maip.Format("%.f", atof(m_pmaip) * atof(m_remain));
				//if (bCredit)
				//	m_maip.Format("%.f", atof(m_maip) - atof(m_symt));
			}
		}
		CalPgsonik("", "");
	}
	else if (gubnS.Find("�ŵ�") != -1 && stateS.Find("���Ȯ��") != -1)
	{
		symbolMap.Lookup(RT_CAMT, nContAmt);
		m_possible.Format("%d", atoi(m_possible) + atoi(nContAmt));
		m_amt.Format("%d", atoi(m_amt) - atoi(nContAmt));
	}
	else if (gubnS.Find("�ŵ�") != -1 && stateS.Find("����") != -1 && stateS != "��������")
	{
		symbolMap.Lookup(RT_CAMT, nContAmt);
		m_possible.Format("%d", atoi(m_possible) - atoi(nContAmt));
		m_amt.Format("%d", atoi(m_amt) + atoi(nContAmt));
	}
//	TRACE("CR[%x][%x] ����(%s) ����(%d) �ܰ�(%f) ���Աݾ�(%.f) �����Աݾ�(%.f) �򰡱ݾ�(%f) ü�Ῡ��(%d) ���ͷ�(%f)\n",
//		this, GetCurrentThreadId(), m_code, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), m_bchg, atof(m_psuik));
	return CHEG_UPDATE;
}

bool CRemain::CalPgsonik(CString currS, CString rateS, int noticeMode)
{
	if (getFormatData(currS) == getFormatData(m_curr) || m_code.IsEmpty())
		return false;

	// ä��, ELS�� ������
	switch (m_code.GetAt(0))
	{
	case 'K':	case 'B':	case 'E':
		return false;
		break;
	}

	if (!currS.IsEmpty())
		m_curr = currS;
	
	if (!rateS.IsEmpty())
		m_srate = rateS;

	m_code.TrimRight();
	// ����ܰ�� ��� ����
//	if (m_creditS == "50")
//		return false;

//	Comment : jjhur 2008.08.07  ����ܰ��� Ȯ�� �������� �ű�
//	if (!currS.IsEmpty())
//		m_curr = currS;

	if ( m_dFee > 0 )
	{
		//===================================================================
		// �򰡱ݾ� = ���簡 * �ܰ�, ������ �ſ���Ⱚ ����
		// �򰡼��� = �򰡱ݾ� - ���԰�
		// ���ͷ� = �򰡼��� * 100 / ���԰�
		//===================================================================
		// �򰡱ݾ�
		double rprice = 0;
		
		rprice = atof(getFormatData(m_curr)) * atof(m_remain);

		double dmass = RoundDown(rprice * m_dMass);
		double dtax = Round(rprice * TAX_RATE);

		//������� ������ ����
		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 )||m_betf)
		{
			rprice = rprice - (dmass + m_dSave);
			m_biyo.Format("%d", (int)(dmass + m_dSave));
		}	
		else
		{
			rprice = rprice - (dmass + dtax + m_dSave);
			m_biyo.Format("%d", (int)(dmass + dtax + m_dSave));
		}	
		
		m_rprice.Format("%.f", rprice); //�򰡱ݾ�
	
		double dmaip;
		if (m_bchg)	
			dmaip = (atof(m_pmaip)* atof(m_remain)) - atof(m_symt);			
		else
			dmaip = atof(m_omaip);// + atof(m_symt);

		if ( atof(m_maip) == 0 ) 
			dmaip = 0;//==>����� ���԰� 0
		else 
			dmaip = dmaip + RoundDown((dmaip+atof(m_symt)) * m_dMass) + m_dSave;//���Աݿ� ������ ����
		
		m_maip.Format("%.f",dmaip);
	
	
		/////////////////////////////////////////////////
		double dbep = 0;
		double bep_mass = RoundDown2(fabs(atof(m_pmaip)) * m_dMass);
		double cur_mass = RoundDown2(fabs(atof(m_curr)) * m_dMass);
		double cur_tax = RoundDown2(fabs(atof(m_curr)) * TAX_RATE);

		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 )||m_betf) 
			dbep = fabs(atof(m_pmaip))+ bep_mass + cur_mass;
		else 
			dbep = fabs(atof(m_pmaip)) + bep_mass + cur_mass + cur_tax;
		
		m_pbep.Format("%.f",dbep);
		///////////////////////////////////////////////////
		double dSonik; 
		
		

		//pyt 11.09
		if (m_dCalcType==1)
		{
			m_rprice.Format("%.f", rprice); //�򰡱ݾ�
			m_maip.Format("%.f",dmaip+atof(m_symt));
			dSonik = atof(m_rprice) - atof(m_maip);
			m_pgsonik.Format("%.0f", dSonik);

			//�򰡼���
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;

			// ���ͷ�
			if (atof(m_maip) == 0 )
			{
				m_psuik = "0.00";
				return true;
			}

//			dSonik = ((dSonik)*100)/(atof(m_maip)+atof(m_symt));
			dSonik = ((dSonik)*100)/(atof(m_maip));
		}
		else
		{
			m_rprice.Format("%.f", rprice-atof(m_symt));
			m_maip.Format("%.f",dmaip);
			dSonik = atof(m_rprice) - atof(m_maip) ;
			m_pgsonik.Format("%.0f", dSonik);
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;

			// ���ͷ�
			if (atof(m_maip) == 0 )
			{
				m_psuik = "0.00";
				return true;
			}

			dSonik = ((dSonik)*100)/(atof(m_maip));
		}
		
		m_psuik.Format("%.2f", dSonik);
		
//		TRACE("���� %f �ſ� %f �� %f ���� %f\n", dmaip, atof(m_symt), rprice, dSonik);

		if (dSonik < 0)
			m_psuik = "-" + m_psuik;
		else if (dSonik > 0)
			m_psuik = "+" + m_psuik;

	} 
	else 
	{	
		//===================================================================
		// �򰡱ݾ� = ���簡 * �ܰ�, ������ �ſ���Ⱚ ����
		// �򰡼��� = �򰡱ݾ� - ���԰�
		// ���ͷ� = �򰡼��� * 100 / ���԰�
		//===================================================================
		double rprice = 0;

		rprice = atof(getFormatData(m_curr)) * atof(m_remain);

		m_rprice.Format("%.f", rprice); //�򰡱ݾ�	


		double dmaip;

		if (m_bchg)	
			dmaip = (atof(m_pmaip)* atof(m_remain)) - atof(m_symt);			
		else
			dmaip = atof(m_omaip);// + atof(m_symt);

		if ( atof(m_maip) == 0 ) 
			dmaip = 0;//==>����� ���԰� 0
		

		/////////////////////////////////////////////////
		m_pbep.Format("%.f",atof(m_pmaip));
		/////////////////////////////////////////////////
		
		//pyt 11.09
		double dSonik;
		if (m_dCalcType==1)
		{
			m_rprice.Format("%.f", rprice); //�򰡱ݾ�	
			m_maip.Format("%.f",dmaip+atof(m_symt));

			dSonik = atof(m_rprice) - atof(m_maip);	
			m_pgsonik.Format("%.0f", dSonik);
			
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;
			
			// ���ͷ�
			if (atof(m_maip) == 0 )
			{
				m_psuik = "0.00";
				return true;
			}

			dSonik = (dSonik*100)/(atof(m_maip));
		}
		else
		{
			m_rprice.Format("%.f", rprice-atof(m_symt)); //�򰡱ݾ�	
			m_maip.Format("%.f",dmaip);

			dSonik = atof(m_rprice) - atof(m_maip);
			m_pgsonik.Format("%.0f", dSonik);
			
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;
			
			// ���ͷ�
			if (atof(m_maip) == 0 )
			{
				m_psuik = "0.00";
				return true;
			}
			dSonik = (dSonik*100)/(atof(m_maip));
		}



		m_psuik.Format("%.2f", dSonik);

		if (dSonik < 0)
			m_psuik = "-" + m_psuik;
		else if (dSonik > 0)
			m_psuik = "+" + m_psuik;

	}
	TRACE("CP[%x][%x] ����(%s) dFee(%d) ����(%d) �ܰ�(%f) ���Աݾ�(%.f) �����Աݾ�(%.f) �򰡱ݾ�(%f) ü�Ῡ��(%d) ���ͷ�(%f)\n",
		this, GetCurrentThreadId(), m_code, m_dFee, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), m_bchg, atof(m_psuik));
	return true;
}

inline CString CRemain::parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

CString CRemain::getFormatData(CString src)
{
	CString sRtn = src;
	if (!sRtn.IsEmpty() && sRtn.FindOneOf("+-") == 0)
		sRtn = sRtn.Mid(1);
	sRtn.TrimLeft();
	return sRtn;
}

CString CRemain::getCredit(CString creditS)
{
	if (creditS.IsEmpty())
		return " ";
	int credit = atoi(creditS);
	CString tmpS = " ";
	if (!m_mapCredit.Lookup(credit, tmpS))
		tmpS = " ";
	return tmpS;
}

CString CRemain::getCreditX(CString creditS)
{
	if (creditS.IsEmpty())
		return " ";
	int credit = atoi(creditS);
	CString tmpS = " ";
	if (!m_mapCreditX.Lookup(credit, tmpS))
		tmpS = " ";
	return tmpS;
}


void CRemain::FillString(CString& datS, int len)
{
	CString tmpS;
	tmpS.Format("%*s", len, datS);
	datS = tmpS;
}

double CRemain::RoundDown(double data) //��ü������(����)
{
    return ( (int)((data * 0.1)) / 0.1 );
}

double CRemain::Round(double data ) //0.003������(�ݿø�)
{
    return (int)(data + 0.5);
}

double CRemain::RoundDown2(double data)
{
	return ( (int) Round(data) );
}
