// ChartStockOpinionAddInImp.h: interface for the CChartGuideLineAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTSTOCKOPINIONADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTSTOCKOPINIONADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn.h"							// for Ixxxxxx.h

//�߰�
#include "PatternMaker.h"									// for CPatternMaker
#include "DlgSetGuideLine.h"								// ���ؼ� ���� �۵� Dlg
#include "./include_Addin/I003721/_AddInCommand.h"		// for DECLARE_ADDIN_EVENT_MAP()

#define R_ERROR		-1
#define R_OK		0
#define R_OK_SCROLL	1

// RQ�� ���� : �������� - ojtaso (20070305)
using namespace std;

#include <map>
#include <string>

// RQ�� ���� : �������� - ojtaso (20070305)
//typedef map<string, CPatternMaker*>		MAP_PATTERNMAKER;

class CChartGuideLineAddInImp : public CChartAddInBase
{
public:
	DWORD m_dwMouseActionID;
	virtual BOOL InvokeAddIn( int p_nCommandType, long p_lData);
	static CString m_strAddInItemName;
	
	CChartGuideLineAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartGuideLineAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

//	//2005. 03. 02 == �Ｚ �����
//	BOOL	ReadAcctInfoFromFile_ForSamgsungDemo(CString &strAcct, CString &strPass);
	CString	CheckAcctNPassFromMain(HWND hMainFrame, CString strAcct, CString strInputPass, int nAcctCheckType=1);	

	CString m_strUsrPath;
	CString m_strCode;		 //�����ڵ� 	
	void	LoadGlobalCfg(); //ȯ�漳�� ������ �о�´�.
// *******************************************************************************
// Pattern Maker
public:
	//CPatternMaker		m_pmPatternMaker;
	LPCTSTR m_lpszRQ;		// ���� RQ : �������� - ojtaso (20070305)
//@��������	MAP_PATTERNMAKER	m_mapPatternMaker; // RQ�� ���� : �������� - ojtaso (20070305)
	CMapStringToPtr		m_mapPatternMaker; // RQ�� ���� : ��������

	CPatternEnvData		m_PatternEnvData;			// ���� ȯ�漳�� ������	
// ********************************************************************************

// (2005.12.01, sy, nam) Real ���� Event�� �����Ѵ�. =======================================================================================
//protected:
//	virtual BOOL	OnRealData( WORD p_wRealRecordID, WORD p_wRealItemID, const char *p_szRealKeyCode, long p_lPacketAddress, double dData);
//==========================================================================================================================================
	void InvokeRealData(int nMarketGubun, LPSTR szCode, LPSTR oPrice, LPSTR hPrice, LPSTR lPrice, LPSTR cPrice);
	
protected:
	virtual void OnDrawAfterGraph( HDC p_hDC);
	virtual void OnDrawBeforeGraph( HDC p_hDC);
	virtual BOOL OnLButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnLButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonUp(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnRButtonDown(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL OnMouseMove(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	//virtual BOOL OnLButtonDblClk(HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
 	virtual void	OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset);
	virtual void	OnResetChart();
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem( LPCTSTR lpszRQ, BOOL p_bTrUpdate);
	// �������� ������ ���� �߰� : �������� - ojtaso (20070125)
	//virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam);
	//virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);
	// �������� �߰�/���� : �������� - ojtaso (20070302)
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam);

private:
	BOOL		m_bLog;						// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	BOOL		m_bReverse;					// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	CRect		m_rctBoundMark;
	double		m_dViewMin;					// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	double		m_dViewMax;					// View���� ��� Graph�� View�� Maximum Scale ���̴�.
	CRect	rctBlockRegion, rctGraphRegion ;
	int		nStartIndex, nEndIndex;

	void GetPatternMaker(CPatternMaker*& pPatternMaker, LPCTSTR lpszRQ = NULL);
};

#endif // !defined(AFX_CHARTSTOCKOPINIONADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
