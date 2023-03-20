#include "stdafx.h"
#include "IBXXXX01.h"
#include "Remain.h"
#include "DefineAll.h"
#include <math.h>
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TESTSAVEFILE	//2014.10.10 KSJ �α׸� ����� ������ �ּ������ϸ� �ȴ�.


CRemain::CRemain()
{
	m_pParent = NULL;

	m_code = m_name = m_remain = m_amt = m_pmaip = m_possible = m_pbep = "";
	m_curr = m_rprice = m_pgsonik = m_psuik = m_maip = "";
	m_date = m_creditS = m_symt = "";
	m_type = "";
	m_biyo = "";

	m_dFee =0;
	m_dMass = 0;//buffet
	m_dSave = 0;//buffet
	m_dCalcType = 0;
	m_iMcgb = 0;
	m_b3RDCode = false;
	m_bKONEXCode = false;
	m_dTax = TAX_RATE;

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

	const _creditGubn creditGubn[] = 
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

CRemain::CRemain(CWnd* pMainWnd)
{
	CRemain();

	m_pParent = (CMapWnd*)pMainWnd;
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
	CString stmp;
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

	if (m_code.Find("005930") >= 0)
		TRACE("123");
#ifdef	CREDIT
	m_date = CString(srec.sydt, sizeof(srec.sydt));
	m_creditS = CString(srec.sycd, sizeof(srec.sycd));
//	if (atoi(m_creditS) >= 10 || atoi(m_creditS) == 0)	// �ſ뱸�� �ƴϸ� 0
//		m_creditS = "  ";
	m_symt = CString(srec.samt, sizeof(srec.samt));
stmp.Format("��ȸ�� ���簡=[%s] ���Աݾ�=[%s] �򰡱ݾ�=[%s]", m_curr, m_maip, m_rprice);
// 	CString strTemp;
// 	strTemp.Format("[KSJ][ParsingRemainData111] ���ڱ�����[%d] �ſ�[%s] ����[%s](%s) dFee(%d) ����(%d) �ܰ�(%f) ���Աݾ�(%.f) �����Աݾ�(%.f) �򰡱ݾ�(%f) ü�Ῡ��(%d) ���ͷ�(%f) �ſ�ݾ�(%f) ���簡(%s)",
// 		m_dCalcType, m_creditS, m_name, m_code, m_dFee, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), m_bchg ? 1:0, atof(m_psuik), atof(m_symt), m_curr);
//putDebugString(strTemp);
// 	testSaveFile(strTemp);

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
	double dSonik{};
	if ( m_dFee == 0 )
	{
		double dd{};

		//pyt 11.09
		if (m_dCalcType==1)	
		{
			if(m_creditS == "05")  //�������
			{
				m_maip.Format("%.f",atof(m_maip));//���Աݾ� + �ſ�ݾ�
				m_rprice.Format("%.f",atof(m_rprice));//�򰡱ݾ� + �ſ�ݾ�
				
				dSonik = atof(m_rprice) - atof(m_maip);
				dSonik *= -1;
			}
			else
			{
				m_maip.Format("%.f",atof(m_maip)+atof(m_symt));//���Աݾ� + �ſ�ݾ�
				m_rprice.Format("%.f",atof(m_rprice)+atof(m_symt));//�򰡱ݾ� + �ſ�ݾ�
				
				dSonik = atof(m_rprice) - atof(m_maip);
			}
		
			m_pgsonik.Format("%.f",dSonik);//�򰡼���
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;
			dd = atof(m_pgsonik)*100/(atof(m_maip));
		}
		else
		{
			dSonik = atof(m_rprice) - atof(m_maip);

			if(m_creditS == "05")  //������� 
				dSonik *= -1;

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

		if(m_creditS == "05")
			symt = 0;

		if(m_b3RDCode)// 2015.04.22 KSJ K-OTC�� ��� �����ᰡ 0.15��. �������� 0.0005��
		{
			m_dTax = TAX_RATE_K_OTC;

			if(m_iMcgb == 0) //2015.04.22 KSJ ��ü������ 0:HTS/MTS/Ȩ������, 1:������/�ݼ���, 2:ARS
			{
				m_dMass = 0.0015;
				m_dSave = 0;
			}
		}
		else if(m_bKONEXCode)
		{
			m_dTax = TAX_RATE_KONEX;
		}
		else
		{
			m_dTax = TAX_RATE;
		}

		//double dMaip = atof(m_maip) + RoundDown((atof(m_maip)+symt) * m_dMass) + m_dSave;//���԰�
		double dMaip{};
		// �ŵ������Ḹ �����ϰ� ����
		if(m_dFee == 1)
		{
			if(m_creditS == "05")  //�������f
				dMaip = atof(m_maip) - RoundDown((atof(m_maip)+symt) * m_dMass) - m_dSave;//���԰�
			else
				dMaip = atof(m_maip) + RoundDown((atof(m_maip)+symt) * m_dMass) + m_dSave;//���԰�
		}
		else 
			dMaip = atof(m_maip);
		
		CString strTemp;
		strTemp.Format("[KSJ] dMaip[%.f] m_dMass[%.f] m_maip[%s] atof(m_maip)(%.f) RoundDown(%.f)", dMaip, m_dMass, m_maip, atof(m_maip), RoundDown((atof(m_maip)+symt) * m_dMass));
		OutputDebugString(strTemp);

		const double dmass = RoundDown((atof(m_rprice)+symt) * m_dMass);
		const double dtax = Round((atof(m_rprice)+symt) * m_dTax);			

		//������� ������ ����
		double dprice = 0;
		//if ( m_code.GetAt(0) == 'J' ) dprice = atof(m_rprice)*(1 - m_dMass ) - m_dSave;
		//else dprice = atof(m_rprice)*(1 - m_dMass - TAX_RATE ) - m_dSave;
	
		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 ) || m_betf || m_code.GetAt(0) == 'Q') //2015.04.22 ETN�߰�
		{
			if(m_creditS == "05")  //�������
				dprice = atof(m_rprice) + ( dmass + m_dSave);
			else
				dprice = atof(m_rprice) - ( dmass + m_dSave);
		}	
		else
		{
			if(m_creditS == "05")  //�������
				dprice = atof(m_rprice) + (dmass + dtax + m_dSave);
			else	
				dprice = atof(m_rprice) - (dmass + dtax + m_dSave);
		}	
		
// 		CString strTemp;
// 		strTemp.Format("[KSJ][ParsingRemainData] �����ڵ�(%s) ������ ���� �� �򰡱ݾ�(%f) ������(%f) ������(%f) �ſ�ݾ�(%f)", m_code, atof(m_rprice), dmass + m_dSave, dtax, symt);
// 		OutputDebugString(strTemp);
// 		testSaveFile(strTemp);

		double dSonik{};

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

		if(m_creditS == "05")  //�������
			dSonik *= -1;
		
// 		strTemp.Format("[KSJ][ParsingRemainData] �����ڵ�(%s) ������ ���� �򰡱ݾ�(%f) �򰡼���(%f) ������(%f) ������(%f) �ſ�ݾ�(%f)", m_code, atof(m_rprice), dSonik, dmass + m_dSave, dtax, symt);
// 		OutputDebugString(strTemp);
// 		testSaveFile(strTemp);

		
		m_pgsonik.Format("%.f",dSonik);//�򰡼���
		if (dSonik > 0)	
			m_pgsonik = "+" + m_pgsonik;
		 
		if ( dMaip == 0 )
			m_psuik.Format("%.2f",0.0);
		else
		{
			double dSuik{};
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

	if(m_b3RDCode)// 2015.04.22 KSJ K-OTC�� ��� �����ᰡ 0.15��. �������� 0.0005��
	{
		m_dTax = TAX_RATE_K_OTC;
		
		if(m_iMcgb == 0) //2015.04.22 KSJ ��ü������ 0:HTS/MTS/Ȩ������, 1:������/�ݼ���, 2:ARS
		{
			m_dMass = 0.0015;
			m_dSave = 0;
		}
	}
	else if(m_bKONEXCode)
	{
		m_dTax = TAX_RATE_KONEX;
	}
	else
	{
		m_dTax = TAX_RATE;
	}

	const double bep_mass = RoundDown2(fabs(atof(m_pmaip)) * m_dMass); 
//	double cur_tax = RoundDown2(fabs(atof(m_curr)) * m_dTax);  //�������
	const double cur_tax = RoundDown2(fabs(atof(m_creditS == "05" ? m_pmaip:m_curr)) * m_dTax);
	const double cur_mass = RoundDown2(fabs(atof(m_curr)) * m_dMass);//TAX_RATE);
	
	if ( m_dFee > 0 )
	{
	//	if ( m_code.GetAt(0) == 'J' ) dbep = fabs(atof(m_pmaip))*(1+m_dMass) + fabs(atof(m_curr))*m_dMass;
	//	else dbep = fabs(atof(m_pmaip))*(1+m_dMass) + fabs(atof(m_curr))*(m_dMass+TAX_RATE);

		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 )|| m_betf || m_code.GetAt(0) == 'Q') //2015.04.22 ETN�߰�
		{	
			if(m_creditS == "05")  //������� // (�ŵ��ݾ� - ������)/(���� * (1 + ��������))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) - RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 + m_dMass));
			else					// (�ż��ݾ� + �ż�������)/(����*(1-��������-��������))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) + RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 - m_dMass));
			/*
			if(m_creditS == "05")  //�������
				dbep = fabs(atof(m_pmaip)) - bep_mass - cur_mass;
			else
				dbep = fabs(atof(m_pmaip)) + bep_mass + cur_mass;
			*/
			m_biyo.Format("%d",(UINT) (RoundDown2(fabs(forby) * m_dTax)+ RoundDown2(fabs(forby) * m_dMass)));
		}	
		else
		{
			if(m_creditS == "05")  //�������  //������� // (�ŵ��ݾ� - ������ - ������)/(���� * (1 + ��������))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) - RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass) - Round(fabs(atof(m_curr))*atoi(m_remain) * m_dTax))/(atoi(m_remain) * (1 + m_dMass ));
			else                  // (�ż��ݾ� + �ż�������)/(����*(1-��������-��������))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) + RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 - m_dMass - m_dTax));
			/*
			if(m_creditS == "05")  //�������
				dbep = fabs(atof(m_pmaip)) - bep_mass - cur_tax - cur_mass;
			else
				dbep = fabs(atof(m_pmaip)) + bep_mass + cur_tax + cur_mass;
			*/
			m_biyo.Format("%d",(int)(RoundDown2(fabs(forby) * m_dTax)+RoundDown2(fabs(forby) * m_dMass) + m_dSave));
		}	
	}		

	m_pbep.Format("%.f",dbep);

	m_symt.TrimLeft(); m_symt.TrimRight();

	CString strTemp;
	strTemp.Format("[KSJ][ParsingRemainData222] ���ڱ�����[%d] %s �ſ�[%s] ����[%s](%s) dFee(%d) ����(%d) �ܰ�(%f) ���Աݾ�(%.f) �����Աݾ�(%.f) �򰡱ݾ�(%f) �򰡼���(%f)  ü�Ῡ��(%d) ���ͷ�(%f) �ſ�ݾ�(%f) ���簡(%s) TAX(%f) ������(%f)",
		m_dCalcType, stmp, m_creditS, m_name, m_code, m_dFee, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), atof(m_pgsonik), m_bchg ? 1:0, atof(m_psuik), atof(m_symt), m_curr, m_dTax, m_dMass);
	OutputDebugString(strTemp);
// 	testSaveFile(strTemp);

/**************************************************/
	TRACE("PR[%x][%x] ����(%s) ����(%d) �ܰ�(%f) ���Աݾ�(%.f) �����Աݾ�(%.f) �򰡱ݾ�(%f) ���ͷ�(%f)\n",
		this, GetCurrentThreadId(),	m_code, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), atof(m_psuik));
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
	const int credit = atoi(m_creditS);

	bool bCredit = false;

//	if (credit >= 10 || credit == 0)		// �ſ뱸�� �ƴϸ� 0
//		m_creditS = "  ";
//	else
//	{
		m_creditS.Format("%02d", credit);
		bCredit = true;
//	}

	const double	symt = atof(m_symt);
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

CString slog;

		if (gubnS.Find("�ż�") != -1)
		{
			CString maipS;
			if(credit == 5)  //������� �ż���ȯ ü��
			{
				m_amt.Format("%d", atoi(m_amt) - atoi(nContAmt));
				m_remain.Format("%d", atoi(m_remain) - atoi(nContAmt));			// �ܰ����
			}
			else
			{
				maipS.Format("%.f", atoi(m_remain) * atof(m_pmaip) + atof(cpriceS) * atof(nContAmt));	// ���԰�
				m_remain.Format("%d", atoi(nContAmt) + atoi(m_remain));			// �ܰ����
				m_pmaip.Format("%f", atof(maipS) / atof(m_remain));			// ��հ�
				m_possible.Format("%d", atoi(m_possible) + atoi(nContAmt));		// ���ɼ���
				m_maip = maipS;
			}
			
			if(credit == 5)  //�������
			{
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

slog.Format("[baltest]�ż� [m_maip]=[%s] ��մܰ�[m_pmaip]=[%s] �ܰ�[m_remain]=[%s] ���ɼ���[m_possible]=[%s] ü�����[nContAmt]=[%s] cpriceS=[%s]", 
			m_maip, m_pmaip, m_remain, m_possible, nContAmt, cpriceS);
OutputDebugString(slog);
			}
		}
		else
		{
			CString maipS;
			if(credit == 5)  //������� �ŵ�ü�� �ܰ� ����
			{
				maipS.Format("%.f", atoi(m_remain) * atof(m_pmaip) + atof(cpriceS) * atof(nContAmt));	// ���԰�
				m_remain.Format("%.d", atoi(m_remain) + atoi(nContAmt));
				m_pmaip.Format("%f", atof(maipS) / atof(m_remain));			// ��հ�
				m_possible.Format("%d", atoi(m_possible) + atoi(nContAmt));		// ���ɼ���
				m_maip = maipS;

slog.Format("[baltest]�ŵ� [m_maip]=[%s] ��մܰ�[m_pmaip]=[%s] �ܰ�[m_remain]=[%s] ���ɼ���[m_possible]=[%s] ü�����[nContAmt]=[%s] cpriceS=[%s]", 
			m_maip, m_pmaip, m_remain, m_possible, nContAmt, cpriceS);
OutputDebugString(slog);

			}
			else 	// �ŵ��� ���� ����
			{
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
		}
		CalPgsonik("", "");
	}
	else if (gubnS.Find("�ż�") != -1 && stateS.Find("����") != -1 && stateS != "��������" && credit == 5)  //������� �ż���Ȳ ����
	{
		symbolMap.Lookup(RT_CAMT, nContAmt);
		m_possible.Format("%d", atoi(m_possible) - atoi(nContAmt));
		m_amt.Format("%d", atoi(m_amt) + atoi(nContAmt));
	}
	else if (gubnS.Find("�ż�") != -1 && stateS.Find("���Ȯ��") != -1 && credit == 5)  //������� �ż� ���
	{
		symbolMap.Lookup(RT_CAMT, nContAmt);
		m_possible.Format("%d", atoi(m_possible) + atoi(nContAmt));
		m_amt.Format("%d", atoi(m_amt) - atoi(nContAmt));
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

		CString strTemp;
// 		strTemp.Format("[KSJ] CalPgsonik rprice1[%f]", rprice);
// 		OutputDebugString(strTemp);

		if(m_b3RDCode)// 2015.04.22 KSJ K-OTC�� ��� �����ᰡ 0.15��. �������� 0.0005��
		{
			m_dTax = TAX_RATE_K_OTC;
			
			if(m_iMcgb == 0) //2015.04.22 KSJ ��ü������ 0:HTS/MTS/Ȩ������, 1:������/�ݼ���, 2:ARS
			{
				m_dMass = 0.0015;
				m_dSave = 0;
			}
		}
		else if(m_bKONEXCode)
		{
			m_dTax = TAX_RATE_KONEX;
		}
		else
		{
			m_dTax = TAX_RATE;
		}

		const double dmass = RoundDown(rprice * m_dMass);
		const double dtax = Round(rprice * m_dTax);

		//������� ������ ����
		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 )|| m_betf || m_code.GetAt(0) == 'Q') //2015.04.22 ETN�߰�
		{
			if(m_creditS == "05")  //�������
				rprice = rprice + (dmass + m_dSave);
			else
				rprice = rprice - (dmass + m_dSave);

			m_biyo.Format("%d", (int)(dmass + m_dSave));
		}	
		else
		{
			if(m_creditS == "05")  //�������
				rprice = rprice + (dmass + dtax + m_dSave);
			else
				rprice = rprice - (dmass + dtax + m_dSave);

			m_biyo.Format("%d", (int)(dmass + dtax + m_dSave));
		}	
		
		m_rprice.Format("%.f", rprice); //�򰡱ݾ�
	
// 		strTemp.Format("[KSJ] CalPgsonik rprice2[%f]", rprice);
// 		OutputDebugString(strTemp);

		double dmaip{};
		if (m_bchg)	
		{
			if(m_creditS == "05")  //�������
				dmaip = (atof(m_pmaip)* atof(m_remain));			
			else
				dmaip = (atof(m_pmaip)* atof(m_remain)) - atof(m_symt);				
		}
		else
			dmaip = atof(m_omaip);// + atof(m_symt);

		if(!m_code.Compare("A035720"))
		{
			strTemp.Format("[KSJ] CalPgsonik m_bchg[%s] dmaip1[%.4f] m_omaip[%s] m_pmaip[%s] m_remain[%s] m_symt[%s]", m_bchg ? "true":"false", dmaip, m_omaip, m_pmaip, m_remain, m_symt);
			OutputDebugString(strTemp);
		}

		if ( atof(m_maip) == 0 ) 
			dmaip = 0;//==>����� ���԰� 0
		else
		{
			if(!m_code.Compare("A035720"))
			{
				strTemp.Format("[KSJ] dMaip[%.4f] m_dMass[%0.4f] m_maip[%s] m_symt[%s] m_dSave[%0.4f] atof(m_maip)(%.4f) RoundDown(%.4f)", dmaip, m_dMass, m_maip, m_symt, m_dSave, atof(m_maip), RoundDown((atof(m_maip)+atof(m_symt)) * m_dMass));
				OutputDebugString(strTemp);
			}

			//dmaip = dmaip + RoundDown((dmaip+atof(m_symt)) * m_dMass) + m_dSave;//���Աݿ� ������ ����
			// �ŵ������Ḹ �����ϰ� ����
			if(m_dFee == 1)
			{
				if(m_creditS == "05")  //�������
					dmaip = dmaip - RoundDown(dmaip * m_dMass) - m_dSave;//���Աݿ� ������ ���� 
				else
					dmaip = dmaip + RoundDown((dmaip+atof(m_symt)) * m_dMass) + m_dSave;//���Աݿ� ������ ����  //test
			}
			else
				dmaip = dmaip;
		}
		
		m_maip.Format("%.f",dmaip);
	
		/////////////////////////////////////////////////
		double dbep = 0;

		if(m_b3RDCode)// 2015.04.22 KSJ K-OTC�� ��� �����ᰡ 0.15��. �������� 0.0005��
		{
			m_dTax = TAX_RATE_K_OTC;
			
			if(m_iMcgb == 0) //2015.04.22 KSJ ��ü������ 0:HTS/MTS/Ȩ������, 1:������/�ݼ���, 2:ARS
			{
				m_dMass = 0.0015;
				m_dSave = 0;
			}
		}
		else if(m_bKONEXCode)
		{
			m_dTax = TAX_RATE_KONEX;
		}
		else
		{
			m_dTax = TAX_RATE;
		}

		const double bep_mass = RoundDown2(fabs(atof(m_pmaip)) * m_dMass);
		const double cur_mass = RoundDown2(fabs(atof(m_curr)) * m_dMass);  //�������
//		double cur_tax = RoundDown2(fabs(atof(m_curr)) * m_dTax);
		const double cur_tax = RoundDown2(fabs(atof(m_creditS == "05"?m_pmaip:m_curr)) * m_dTax);

		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 )|| m_betf || m_code.GetAt(0) == 'Q') //2015.04.22 ETN�߰�
		{
			if(m_creditS == "05")  //������� // (�ŵ��ݾ� - ������)/(���� * (1 + ��������))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) - RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 + m_dMass ));
			else					// (�ż��ݾ� + �ż�������)/(����*(1-��������-��������))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) + RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 - m_dMass));
			/*
			if(m_creditS == "05")  //�������
				dbep = fabs(atof(m_pmaip))- bep_mass - cur_mass;
			else
				dbep = fabs(atof(m_pmaip))+ bep_mass + cur_mass;
			*/
		}
		else 
		{
			if(m_creditS == "05")  //�������  //������� // (�ŵ��ݾ� - ������ - ������)/(���� * (1 + ��������))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) - RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass) - Round(fabs(atof(m_curr))*atoi(m_remain) * m_dTax))/(atoi(m_remain) * (1 + m_dMass ));
			else                  // (�ż��ݾ� + �ż�������)/(����*(1-��������-��������))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) + RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 - m_dMass - m_dTax));
			/*
			if(m_creditS == "05")  //�������
				dbep = fabs(atof(m_pmaip)) - bep_mass - cur_mass - cur_tax;
			else
				dbep = fabs(atof(m_pmaip)) + bep_mass + cur_mass + cur_tax;
			*/
		}
		
		m_pbep.Format("%.f",dbep);
		///////////////////////////////////////////////////
		double dSonik{};
		
		

		//pyt 11.09
		if (m_dCalcType==1)
		{
			m_rprice.Format("%.f", rprice); //�򰡱ݾ�
			if(m_creditS == "05")  //�������
				m_maip.Format("%.f",dmaip);
			else
				m_maip.Format("%.f",dmaip+atof(m_symt));

			dSonik = atof(m_rprice) - atof(m_maip);

			if(m_creditS == "05")  //�������
				dSonik *= -1;

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
			if(m_creditS == "05")  //�������
				m_rprice.Format("%.f", rprice);
			else
				m_rprice.Format("%.f", rprice-atof(m_symt));

			m_maip.Format("%.f",dmaip);
			dSonik = atof(m_rprice) - atof(m_maip) ;

			if(m_creditS == "05")  //�������
				dSonik *= -1;

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

// 		CString strTemp;
// 		strTemp.Format("[KSJ] CalPgsonik rprice2[%f]", rprice);
// 		OutputDebugString(strTemp);

		double dmaip{};

		if (m_code.Find("005930"))
			TRACE("123");

		if (m_bchg)	
		{
			if(m_creditS == "05")  //�������
				dmaip = (atof(m_pmaip)* atof(m_remain));			
			else
				dmaip = (atof(m_pmaip)* atof(m_remain)) - atof(m_symt);			  //test 2023
		}
		else
			dmaip = atof(m_omaip);// + atof(m_symt);

		if ( atof(m_maip) == 0 ) 
			dmaip = 0;//==>����� ���԰� 0
		

		/////////////////////////////////////////////////
		m_pbep.Format("%.f",atof(m_pmaip));
		/////////////////////////////////////////////////
		
		//pyt 11.09
		double dSonik{};
		if (m_dCalcType==1)
		{
			m_rprice.Format("%.f", rprice); //�򰡱ݾ�	

			if(m_creditS == "05")  //�������   
				m_maip.Format("%.f",dmaip);
			else
				m_maip.Format("%.f",dmaip+atof(m_symt));

			dSonik = atof(m_rprice) - atof(m_maip);	
			if(m_creditS == "05") //�������   
				dSonik *= -1;
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
			if(m_creditS == "05")  //�������   
				m_rprice.Format("%.f", atof(m_rprice));
			else
				m_rprice.Format("%.f", rprice-atof(m_symt)); //�򰡱ݾ�	

		//	m_maip.Format("%.f",dmaip);  //test 20230215

			dSonik = atof(m_rprice) - atof(m_maip);

			if(m_creditS == "05") //�������   
				dSonik *= -1;

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

// 	CString strTemp;
// 	strTemp.Format("[KSJ][CalPgsonik] ���ڱ�����[%d] ����(%s) dFee(%d) ����(%d) �ܰ�(%f) ���Աݾ�(%.f) �����Աݾ�(%.f) �򰡱ݾ�(%f) ü�Ῡ��(%d) ���ͷ�(%f) �ſ�ݾ�(%f) TAX(%f)",
// 		m_dCalcType, m_code, m_dFee, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), m_bchg ? 1:0, atof(m_psuik), atof(m_symt), m_dTax);
// 	OutputDebugString(strTemp);
// 	testSaveFile(strTemp);

// 	TRACE("CP[%x][%x] ����(%s) dFee(%d) ����(%d) �ܰ�(%f) ���Աݾ�(%.f) �����Աݾ�(%.f) �򰡱ݾ�(%f) ü�Ῡ��(%d) ���ͷ�(%f)\n",
// 		this, GetCurrentThreadId(), m_code, m_dFee, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), m_bchg, atof(m_psuik));
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
	const int credit = atoi(creditS);
	CString tmpS = " ";
	if (!m_mapCredit.Lookup(credit, tmpS))
		tmpS = " ";
	return tmpS;
}

CString CRemain::getCreditX(CString creditS)
{
	if (creditS.IsEmpty())
		return " ";
	const int credit = atoi(creditS);
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


void CRemain::testSaveFile(CString strText)
{	
#ifndef TESTSAVEFILE
	return;
#endif
		
	CString strPath(_T("")), titletime;
	strPath.Format("%s\\%s", m_pParent->GetRoot(), SAVEFILENAME);
	
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	
	CString strCurTime;
	strCurTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	
	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 	
	
	titletime.Format("[%s]", strCurTime);
	file.Write(titletime, titletime.GetLength());
	file.SeekToEnd(); 	
	
	file.Write(strText, strText.GetLength());
	file.WriteString("\n");
	
	file.Close();	
}
