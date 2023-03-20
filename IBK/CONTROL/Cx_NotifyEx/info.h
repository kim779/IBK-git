#pragma once

#include "libDefine.h"

struct _mingamBase 
{
	CString		hv[4];			// ������������ (90/60/30/21)
	CString		cdgl;			// CD �ݸ�

	CString		krts;			// �ֱٿ��� ���� code
	CString		kcur;			// �ֱٿ��� ���� ���簡
	CString		time;			// ����ð�
	
	CString		k200rts;		// KOSPI 200 code
	CString		k200cur;		// KOSPI 200 ���簡
};

class CInfo  
{
public:
	CInfo(CWnd *pControl);
	virtual ~CInfo();

private:
	CWnd	*m_pControl;

public:
	void	RemainParsing(struct f_rec *frec);
	void	SetNoCont(CString gubnS, CString amtS);
	void	SetNoContNew(int md, int ms);
	void	SetOpSensCal(class COpSensCal *pOpSensCal) { m_pOpSensCal = pOpSensCal; }
	CString	GetTotalData();
	void	CalMingam(_mingamBase *mgBase, bool bAll = true);
	bool	CalPgsonik(CString newCurrS, CString newDiffS);
	void	CalPgsonik();
	int	NoticeData(CMapStringToString& m_mapNotice, CString codeS, bool bNEw);

private:
	void	setNoCont(CString gubnS, CString stateS, int amt);
	int	getNoCont(CString gubnS);
	int	getJKind(CString sCode);
	double	getOptionStrike(CString opCode);
	void	getBase();
	
public:
	CString	sCode,			// �����ڵ�	00
		sJmm,			// �����	01
		sGubn,			// �Ÿű���	02
		sRemainAmt,		// �ܰ����	03
		sPossibleAmt,		// �ֹ����ɼ���	04
		sRPrice,		// ��մܰ�	05
		sCurr,			// ���簡	06
		sRDiff,			// ��հ����	07
		sPPrice,		// �򰡱ݾ�	08
		sPgSonik,		// �򰡼���	09
		sSuik,			// ���ͷ�	10
		sMaip,			// ���Աݾ�	11
		sDiff,			// ���ϴ��	12
		sMchg,			// ��ü��	13
		sDelta,			// ��Ÿ		14
		sGamma;			// ����		15
		

	CString	sJjTrade;		// �����ϼ� �ŷ��ϼ�
	CString	sJjCalendar;		// �����ϼ� �޷��ϼ�
	CString	sBdjs;			// �������
	int	m_noCont[2];		// ��ü����� : 0�ŵ�,1�ż�

	class COpSensCal	*m_pOpSensCal;	// ������ ���(��Ÿ,����)
	double	m_dBase;
	
};