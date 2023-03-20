#pragma once

class CMapWnd;
class CRemainFuture : public CWnd
{
public:
	CRemainFuture();
	CRemainFuture(CWnd* pMainWnd);
	virtual ~CRemainFuture();

	//{{AFX_VIRTUAL(CRemainFuture)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CRemainFuture)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CMapWnd*	m_pParent;

	CString	m_code,			// �����ڵ�
		m_name,			// �����
		m_gubn,			// �Ÿű���
		m_possible,		// �ֹ����ɼ���
		m_rprice,		// ��մܰ�
		m_curr,			// ���簡
		m_rdiff,		// ��հ����
		m_pprice,		// �򰡱ݾ�
		m_pgsonik,		// �򰡼���
		m_suik,			// ���ͷ�
		m_remian,		// �ܰ����
		m_maip,			// ���Աݾ�
		m_diff,			// ���ϴ��
		m_srate;		// �����(�ü�)
	double	m_dBase;
	CMapStringToPtr	m_NoContMap;
	char	m_system;

public:
	CString TotalData();
	void	ParsingRemainData(char *pData);
	void	ParsingAllaccnData(CString sData);
	int	CalRemainData(CMapStringToString &symbolMap, CString keyS, bool bNewCode);
	void	CalPgsonik();
	bool	CalPgsonik(CString sNCurr, CString sNDiff, CString sNRate);
	void	SetNoCont(CString sMemeGubn, int nAmount);
	double	GetBaseValue(CString code);			//�ֽļ����� ���, �¼��� codefile���� �о��
private:			
	int	getNoCont(CString sMemeGubn);
	void	parsingNcont(CString sMemeGubn, CString sStatus, int nAmount);
	void	getBase();
	CString	getFormatData(CString src);
	int	getJKind(CString m_code);
	CString parser(CString &srcstr, CString substr);
	CString	getString(char *pChar, int nMinLen);
public:
	void	testSaveFile(CString strText, bool bServer = false);
};
