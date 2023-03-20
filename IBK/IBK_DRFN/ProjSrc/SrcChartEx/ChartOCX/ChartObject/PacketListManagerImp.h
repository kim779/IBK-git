// PacketListManagerImp.h: interface for the CPacketListManagerImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETLISTMANAGERIMP_H__FCEBB523_2C87_46AA_B201_5EE1455FB2B4__INCLUDED_)
#define AFX_PACKETLISTMANAGERIMP_H__FCEBB523_2C87_46AA_B201_5EE1455FB2B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <string>
#include <afxtempl.h>			// for CList

#include "../Include_Chart/Dll_Load/PacketListManager.h"		// for CPacketListManager
#include "../Include_Chart/BlockBaseEnum.h"

using namespace std;

class CPacketRQ;
class CPacketList;

typedef map<string, CPacketRQ*>		MAP_RQ_PACKETLIST;
typedef pair<string, CPacketRQ*>	PAIR_RQ_PACKETLIST;
typedef map<string, UINT>			MAP_RQ;				// RQ map  : �������� - ojtaso (20070108)

class CGraphImp;			// ���� RQ���� : �������� - ojtaso (20070308)
interface IChartCtrl;
class CPacketListManagerImp : public CPacketListManager
{
public:
	CPacketListManagerImp(CString strPacketOption, CPacketType::REALTYPE eRealType, int nLength, bool bUseRealData, const int nSize, const CString& szPacketName, double dVersion);
	virtual ~CPacketListManagerImp();

	LPCTSTR CreateDefaultChartItem( HWND p_hOcxWnd);

	// RQ�� ����� ���� �����ڵ带 ����/����/�ֱ�/ƽ���� ���� : �������� - ojtaso (20070106)
	// ƽ ī��Ʈ �߰� : �������� - ojtaso (20070329)
	// ��Ʈ�� ��, %, $�� ���а��� : �������� - ojtaso (20070411)
	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	LPCTSTR AddChartItem(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, HWND p_hOcxWnd);		// �������� �׷��� �߰�
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	LPCTSTR AddPairItem( LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName, HWND p_hOcxWnd);
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	BOOL	AddChartItemRQ(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName, HWND p_hOcxWnd);
	BOOL	DeleteChartItem(LPCTSTR lpszRQ);
	
//	LPCTSTR GetCurrentRQ();
//	void	SetCurrentRQ(LPCTSTR lpszRQ);
	// RQ�� ����� ���� �����ڵ带 ����/����/�ֱ�/ƽ���� ���� : �������� - ojtaso (20070106)
	// ƽ ī��Ʈ �߰� : �������� - ojtaso (20070329)
	// ��Ʈ�� ��, %, $�� ���а��� : �������� - ojtaso (20070411)
	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	void	SetItemCode(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName);

	CPacketList*	GetPacketList(LPCTSTR lpszRQ);
//	CPacketList*	GetCurrentPacketList();
	CPacketRQ*		GetPacketRQ(LPCTSTR lpszRQ);
	CPacket *		GetBasePacket( LPCTSTR lpszRQ);

	LPVOID	GetMapPacketList();
	void	SetIChartCtrl(IChartCtrl* p_pIChartCtrl);
	IChartCtrl *	GetIChartCtrl( void)	{	return m_pIChartCtrl;	}

	LPCTSTR GetPacketListRQ(CPacketList* pPacketList);

	int		GetPacketListCount();

	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	void	GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName);
	void	GetRQByItemCode( LPCTSTR lpszItemCode, CList< CString, CString> &p_slRQ);
	// ���� RQ���� : �������� - ojtaso (20070308)
	BOOL	IsSameRQ(LPCTSTR lpszRQ, CGraphImp* pGraph);

	// DEFALUT�� ������ ��� RQ : �������� - ojtaso (20070427)
	void	GetAllRQ( CList <CString, CString> &p_slRQs);

	// Graph�� �����ǵ� PacketRQ�� �������� �ʵ��� �ϴ� Flag : �������� - ojtaso (20070531)
	void	SetRemovablePacketRQ(BOOL bRemove);
	BOOL	GetRemovablePacketRQ();
	
	// ���õ� RQ ���� - ojtaso (20070620)
	void	ChangePacketRQ(LPCTSTR lpszRQ1, LPCTSTR lpszRQ2);

	// OneChart���� : �������� - ojtaso (20070627)
	void	SetOneChart( BOOL bOneChart, const char *p_szRQ);
	BOOL	GetOneChart();

	// ���簡 ����� - ojtaso (20080602)
	void	OffsetPacketData(LPCTSTR lpszRQ, double dData, CString strPacketName = _T(""));

	// 2008.10.31 JS.Kim	����ڼ��� �ð��� �ʱ�ȭ
	void	ResetTimeDiff( void);

protected:
	MAP_RQ_PACKETLIST	m_mapPacketList;
	MAP_RQ_PACKETLIST	m_mapOnePacketList;
	MAP_RQ				m_mapRQ; // RQ map  : �������� - ojtaso (20070108)

//	LPTSTR		m_lpszCurrentRQ;

	CString		m_strPacketOption;
	CPacketType::REALTYPE m_eRealType;
	int			m_nLength;
	bool		m_bUseRealData;
	//const int	m_nHoldingPacketSize;
	int	m_nHoldingPacketSize;
	CString		m_strBasePacketName;
	double		m_dVersion;

	// Graph�� �����ǵ� PacketRQ�� �������� �ʵ��� �ϴ� Flag : �������� - ojtaso (20070531)
	BOOL		m_bRemovablePacketRQ;
	// OneChart���� : �������� - ojtaso (20070627)
	BOOL		m_bOneChart;

	IChartCtrl* m_pIChartCtrl;	// ocx interface

// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
public:
	virtual BOOL	IsBasePacketString( void);

// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
protected:
					// for CKTBChartCtrl::SetRealDataCode(). ~ Scripted Real Data.
	virtual void	SetRealData( LPCTSTR lpszItemCode, LPCTSTR lpszPacketName, double dData, short nOption);
					// for IRealReceiver AddIn Interface.
	virtual BOOL	SetRealData( const char *p_szRQ, const char *p_szPacketName, const double& dData, int &p_nRemovePacketDataCount);

// (2008/11/4 - Seung-Won, Bae) for Nomalized Minute Time Clock.
public:
	// (2008/11/16 - Seung-Won, Bae) Notify to PacketList.
	virtual void	OnNMTRClock( const __int64 &p_nClock, IRealReceiver *p_pIRealReceiver);

// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
public:
	virtual void	ChangeChartMode( int p_nChartMode);

	// (2010/02/25 - alzioyes) ���չ��� ������ �ִ� �����.
	virtual BOOL	FindRQ(LPCTSTR lpszRQ);

	// (2010/06/01 - alzioyes) ������Ŷ ���ѻ����� ����.
	virtual void	SetHoldingPacketSize(int nHoldingPacketSize);
};	

#endif // !defined(AFX_PACKETLISTMANAGERIMP_H__FCEBB523_2C87_46AA_B201_5EE1455FB2B4__INCLUDED_)
