// PacketRQ.h: interface for the CPacketRQ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETRQ_H__DFF61C77_BAB9_445B_9256_2F8A43F348B1__INCLUDED_)
#define AFX_PACKETRQ_H__DFF61C77_BAB9_445B_9256_2F8A43F348B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "list"
#include "../Include_Chart/BlockBaseEnum.h"

using namespace std;

class CGraphImp;
typedef list<CGraphImp*>		LIST_RQ_GRAPH;

interface IChartCtrl;
class CPacketList;
class CPacketRQ  
{
public:
	CPacketRQ();
	// RQ�� ����� ���� �����ڵ带 ����/����/�ֱ�/ƽ���� ���� : �������� - ojtaso (20070106)
	// ƽ ī��Ʈ �߰� : �������� - ojtaso (20070329)
	// ��Ʈ�� ��, %, $�� ���а��� : �������� - ojtaso (20070411)
	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	CPacketRQ(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, const UINT& nIndex);
	// �������� �ҷ����� : �������� - ojtaso (20070514)
	CPacketRQ(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName);
	virtual ~CPacketRQ();

	LPCTSTR MakeNewRQ(LPCTSTR lpszItemCode);
	BOOL	DeleteRQ(LPCTSTR lpszRQ);

	void	SetRQ(LPCTSTR lpszRQ);

	LPCTSTR GetRQ()			{ return m_lpszRQ; }
	LPCTSTR GetItemCode()	{ return m_lpszItemCode; }
	// Ÿ��Ʋ�� ����� ǥ�� : �������� - ojtaso (20070322)
	LPCTSTR GetItemName()	{ return m_lpszItemName; }
	LPCTSTR GetPacketName()	{ return m_lpszPacketName; }
	// RQ�� ����� ���� �����ڵ带 ����/����/�ֱ�/ƽ���� ���� : �������� - ojtaso (20070106)
	// ƽ ī��Ʈ �߰� : �������� - ojtaso (20070329)
	// ��Ʈ�� ��, %, $�� ���а��� : �������� - ojtaso (20070411)
	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	void	SetItemCode(LPCTSTR lpszItemCode, LPCTSTR lpszItemName);
	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	void	GetItemCode(LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName);

	void	AddRQGraph(CGraphImp* pGraph);
	void	RemoveRQGraph(CGraphImp* pGraph);

	BOOL	IsEmptyRQGraph();
	
	CPacketList*&	GetPacketList() { return m_pPacketList; }
	void	SetPacketList(CPacketList*& pPacketList) {m_pPacketList = pPacketList; }
	

	BOOL	IsSameItemCode(LPCTSTR lpszItemCode);

	void	CreatePacketList(CString strPacketOption, CPacketType::REALTYPE eRealType, int nLength, bool bUseRealData, const int nSize, const CString& szPacketName, double dVersion, IChartCtrl *p_pIChartCtrl);

	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	void	SetItemRQ(LPCSTR lpszRQ);
	void	SetItemCode(LPCTSTR lpszItemCode);
	void	SetItemName(LPCTSTR lpszItemName);
	void	SetPacketName(LPCTSTR lpszPacketName);

	void	Change(CPacketRQ& packetRQ);

	LIST_RQ_GRAPH& GetGraphList() { return m_listGraph; }
	void	SetGraphList(LIST_RQ_GRAPH& listGraph) { m_listGraph = listGraph; }

protected:
	LPTSTR			m_lpszRQ;

	// ���õ� ��Ʈ ������ �ܺ�dll���� ������� ���� : �������� - ojtaso (20070504)
	LPTSTR			m_lpszItemCode;			// �����ڵ�
	LPTSTR			m_lpszItemName;			// �����
	LPTSTR			m_lpszPacketName;		// ��Ŷ��

	CPacketList*	m_pPacketList;

	LIST_RQ_GRAPH	m_listGraph;
};

#endif // !defined(AFX_PACKETRQ_H__DFF61C77_BAB9_445B_9256_2F8A43F348B1__INCLUDED_)
