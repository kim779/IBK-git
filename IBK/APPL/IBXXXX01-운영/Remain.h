#pragma once

#include "afxtempl.h"

class CMapWnd;

class CRemain : public CWnd
{
public:
	CRemain();
	CRemain(CWnd* pMainWnd);

	virtual ~CRemain();

	//{{AFX_VIRTUAL(CRemain)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CRemain)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
public:
	CMapWnd*	m_pParent;

	CString	m_code,			// �����ڵ�
		m_name,			// �����
		m_remain,		// �����ܰ�(�ܰ����)
		m_amt,			// �ܰ����
		m_pmaip,		// �򰡸���, ��մܰ�
		m_pbep,			// BEP�ܰ� 20090527
		m_possible,		// �ֹ����ɼ���
		m_curr,			// ���簡(�ü�)
		m_rprice,		// �򰡱ݾ�
		m_pgsonik,		// �򰡼���
		m_psuik,		// �򰡼��ͷ�
		m_maip,			// ���Աݾ�
		m_date,			// ������
		m_symt,			// �ſ����ݾ�
		m_creditS,		// �ſ뱸��
		m_type,			// �ܰ����ڵ�
		m_biyo,			// ������+����
		m_omaip,		// ���Աݾ�(����)
		m_srate;		// �����(�ü�)

	char	m_system;
	bool m_betf, m_bchg;

	int m_dFee;//���������
	double m_dMass;		//��ü ������
	double m_dSave;		//SAVE ������
	int m_dCalcType;	//���ڱ� ����(0), ����(1)
	int	m_iMcgb;		// 2015.04.22 KSJ ��ü���� 0:HTS/MTS/Ȩ������, 1:������/�ݼ���, 2:ARS
	bool m_b3RDCode;	// 2015.04.22 KSJ K-OTC���� ����
	bool m_bKONEXCode;	//KONEX ����
	double m_dTax;		// 2015.04.22 KSJ ����
		
private:
	CMap <int, int, CString, CString>	m_mapCredit;
	CMap <int, int, CString, CString>	m_mapCreditX;

	double RoundDown(double data); //��ü������(����)
	double Round(double data ); //0.003������(�ݿø�)

public:
	double RoundDown2(double data);
	CString TotalData();
	void	ParsingRemainData(char *pData);
	void	ParsingAllaccnData(CString sData);
	int	CalRemainData(CMapStringToString &symbolMap, CString keyS, bool bNewCode);
	bool	CalPgsonik(CString currS, CString rateS, int noticeMode=FALSE);	//Notice���� ȣ��Ȱ���
	void	FillString(CString& datS, int len);
private:
	CString parser(CString &srcstr, CString substr);
	CString getFormatData(CString src);
	CString	getCredit(CString creditS);
	CString	getCreditX(CString creditS);
public:
	void testSaveFile(CString strText);

};

