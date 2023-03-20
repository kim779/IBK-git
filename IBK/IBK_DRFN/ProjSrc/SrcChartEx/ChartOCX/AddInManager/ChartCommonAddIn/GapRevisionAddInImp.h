// GapRevisionAddInImp.h: interface for the CGapRevisionAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAPREVISIONADDINIMP_H__0628571B_96CC_4f57_B3B1_774DB5B77085__INCLUDED_)
#define AFX_GAPREVISIONADDINIMP_H__0628571B_96CC_4f57_B3B1_774DB5B77085__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Addin/I000000/_ChartAddInBase.h"	// for CChartAddInBase

struct ST_GAP_INFO
{
	ST_GAP_INFO();
	~ST_GAP_INFO();

// member functions
	void Init();
	void SetGapInfo(double dGapValue, BOOL bReal = TRUE);
	double GetGapInfo(int nIndx);
	void SetRealDate(int nCurRealDate);
	int GetRealDate();
	void SetPacketCount(int nPacketCount);
	int GetPacketCount();
	int GetAllGapInfo(double& dAllGapValue);

// member variables
private:
	CList<double, double> m_arGapInfo;
	int	m_nCurRealDate;
	int m_nPacketCount;
};

class CGapRevisionAddInImp : public CChartAddInBase
{
public:
	CGapRevisionAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CGapRevisionAddInImp();

// Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

protected:
	CTypedPtrMap< CMapStringToPtr, CString, ST_GAP_INFO *> m_mapRQGapInfo;

	BOOL	m_bUseGapRevision;			// ������ ��뿩��

	BOOL	m_bUseMonthGapRevision;		// ������ ������ ��뿩��
	BOOL	m_bShowMonthGapRevision;	// ������ ������ ���� ǥ�ÿ���.

// (2009/9/19 - Seung-Won, Bae) Manage OnPacketDataMultiItem Flag of On-GapRevision.
protected:
	BOOL	m_bOnPacketDataMultiItem;

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);

protected:
	virtual void SetUseGapRevision( const BOOL bUseGapRevision, const BOOL bWithNewData);
	virtual void CalcGapRevision( const char *p_szRQ, const BOOL bIsReset = TRUE);
	virtual void CalcGapRevisionReal( const char *p_szRQ);
	virtual void RestoreGapRevision( const char *p_szRQ);

// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

protected:
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1);
	virtual BOOL	OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID);
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);
};

#endif // !defined(AFX_GAPREVISIONADDINIMP_H__0628571B_96CC_4f57_B3B1_774DB5B77085__INCLUDED_)
