//=========================================================
// 2005. 08. 02 by sy, nam
// ������Ʈ �� ���� 
// Lib�� ������ �ϴ� Dll�� ���� Include 
// ReplayChartInfoDef.h
//=========================================================

#ifndef __HANHWA_CHART_REPLAY_INFO_H
#define __HANHWA_CHART_REPLAY_INFO_H

#define QUERY_STRREPLAY			"p0608"

	enum {STOP, PLAY, PAUSE};

	typedef struct REPLAY_INFO
	{
		void*			pRecvDBMgr;			//CDBMgr*
		HWND			hRecvMainFrame;		//MainFrame Handle
		HWND			hCallHwnd;			//���⸦ ��û�ϰ� �ִ� Wnd's Handle
		int				nDataKind;			//�ֽ�, ���� ����
		CStringArray	arrStrCode;			//�����ڵ�
		char			cGubun;				//ƽ('0')/��('1') ���� 
		int				nUnit;				//��ȸ ���� 1,2,3...........
		int				nSpeed;				//���	
		int				nDayBefore;			//n����
		CString			strStartDate;		//���� ���۽ð� YYYYMMDDHHMM		
		CString			strEndDate;			//���� ����ð� YYYYMMDDHHMM
	} REPLAY_INFO;

	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 17
	// Comments		: Tick ������
	// Using way	: �������� �������� Tick ������
	//-----------------------------------------------------------------------------
	typedef struct _TICK_DATA
	{
		char chDate[6];				// ��¥ + �ð�( DDHHMMSS )
		char chClosingPrice[10];	// ����
		char chVolume[10];			// ü�ᷮ
	} TICK_DATA;

	//-----------------------------------------------------------------------------
	// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 17
	// Comments		: 
	// Using way	: 
	// See			: �������� �������� ��ü Tick ������
	//-----------------------------------------------------------------------------

	#define MAX_COUNT 50000
	
	typedef struct _TR01002_I
	{
		char chStockCode[16];		// �����ڵ�		// s : �ֽ� , f : ����
		char chDayBefore[2];		// n���� data
		char chTimeData[6];			// HHMMSS
	} TR01002_I;


	typedef struct _TR01002_CHECK
	{
		char chDayBefore[2];
		char chTickCount[5];					// Tick Data Count
		char chSubMsgLength[4];					// ��Ʈ ���� �޼����� ����
		//TICK_DATA chTickData[ MAX_COUNT ];		// ��ü Tick Data
	} TR01002_CHECK;
	
	typedef struct _TR01002_O
	{
		TICK_DATA chTickData[ MAX_COUNT ];		// ��ü Tick Data

	}TR01002_O;
/*
	class CChangeDate
	{
	public:	
		CChangeDate(){};
		virtual ~CChangeDate(){};
	
		CString m_strSubMsg;						// ��Ʈ ���� �޼���		// Date=YYYYMMDDHHMMSS@JG=S(or F)@SC=15( Stock Code )
	};
*/
	
	class CSendRealReplay
	{
	public:	
		CSendRealReplay()
		{
		}
		virtual void OnReplayDrds(CString strCode, TICK_DATA* pReplayData) {};
	};

	const UINT RMSG_REPLAY_REAL			= ::RegisterWindowMessage(_T("RMSG_REPLAY_REAL"));
	const UINT RMSG_REPLAY_FAIL			= ::RegisterWindowMessage(_T("RMSG_REPLAY_FAIL"));
	const UINT RMSG_REPLAY_CHANGE_DATE	= ::RegisterWindowMessage(_T("RMSG_REPLAY_CHANGE_DATE"));
	const UINT RMSG_REPLAY_END			= ::RegisterWindowMessage(_T("RMSG_REPLAY_END"));
	const UINT RMSG_REPLAY_INC_SEC = ::RegisterWindowMessage(_T("RMSG_REPLAY_INC_SEC"));			// 1�ʰ� ���� �ɶ����� ����
	const UINT RMSG_REPLAY_TOTAL_SEC = ::RegisterWindowMessage(_T("RMSG_REPLAY_TOTAL_SEC"));		// ��ü �ð�(�ʴ���)�� ����
#endif