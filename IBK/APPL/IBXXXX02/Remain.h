#if !defined(AFX_REMAIN_H__8866F956_10FD_42F2_A9FB_56D282FAF275__INCLUDED_)
#define AFX_REMAIN_H__8866F956_10FD_42F2_A9FB_56D282FAF275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "afxtempl.h"


class CRemain : public CWnd
{
public:
	CRemain();
	virtual ~CRemain();

	//{{AFX_VIRTUAL(CRemain)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CRemain)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
public:
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
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMAIN_H__8866F956_10FD_42F2_A9FB_56D282FAF275__INCLUDED_)
