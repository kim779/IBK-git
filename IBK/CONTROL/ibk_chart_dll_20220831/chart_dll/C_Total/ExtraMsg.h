// ExtraMsg.h: interface for the CExtraMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTRAMSG_H__F3C648F2_E944_4374_964F_5B6E058FF5DE__INCLUDED_)
#define AFX_EXTRAMSG_H__F3C648F2_E944_4374_964F_5B6E058FF5DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif // !defined(AFX_EXTRAMSG_H__F3C648F2_E944_4374_964F_5B6E058FF5DE__INCLUDED_)
