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

	CString	m_code;		// 실시간 코드
	CString	m_date;		// 영업일자
	CString	m_sahg;		// 상한가
	CString	m_hahg;		// 하한가
	CString	m_jjga;		// 전일종가
	CString	m_stock;	// 주식수
	CString	m_start;	// 장시작시간

	CString	m_curr;		// 현재가
	CString	m_udyl;		// 등락
};