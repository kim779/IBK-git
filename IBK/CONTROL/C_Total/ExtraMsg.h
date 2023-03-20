// ExtraMsg.h: interface for the CExtraMsg class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CExtraMsg  
{
public:
	CExtraMsg(class CMainWnd *pMain);
	virtual ~CExtraMsg();

	LRESULT	InputEvent(WPARAM wParam, LPARAM lParam);
	LRESULT	SiseEvent(WPARAM wParam, LPARAM lParam);

private:
	int	GetInputTRInfo(char *pHeader);
	int	GetInputTRInfo2(char *pHeader);
	int	ReceiveData(char *pData, int len);
	int	receiveFORData(char *pData, int len);
	void	realtime(char *pData);
private:
	class CMainWnd	*m_pMainWnd;
	CC_TotalApp	*m_pApp;

	CString	m_code;		// �ǽð� �ڵ�
	CString	m_date;		// ��������
	CString	m_sahg;		// ���Ѱ�
	CString	m_hahg;		// ���Ѱ�
	CString	m_jjga;		// ��������
	CString	m_stock;	// �ֽļ�
	CString	m_start;	// ����۽ð�

	CString	m_curr;		// ���簡
	CString	m_udyl;		// ���
};