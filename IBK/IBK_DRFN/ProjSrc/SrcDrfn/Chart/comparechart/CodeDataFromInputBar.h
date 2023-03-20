// CodeDataFromInputBar.h: interface for the CCodeDataFromInputBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEDATAFROMINPUTBAR_H__1943BC6A_7285_42F7_928A_8B99C5A99F71__INCLUDED_)
#define AFX_CODEDATAFROMINPUTBAR_H__1943BC6A_7285_42F7_928A_8B99C5A99F71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Chart_Common/DlgCompData.h"

class CCodeDataFromInputBar  
{
public:
	CCodeDataFromInputBar();
	virtual ~CCodeDataFromInputBar();
	CString	m_strCode;
	long	m_lEndTime;
	char	m_chType;		//1��,2��,3��,4��
	long	m_lTypeNum;		//?��.
	// gm0604 added
	long	m_lDataCnt;
	long	m_lPtrST;
	CString m_strSTName;
	// gm0604 added (end)

	// Display ī��Ʈ
	long	m_lDispCnt;

	// ��ȸ ��û�� ī��Ʈ ��
	long	m_lRequestCnt;

	//�������� ���Һ�(A)/�������� ��ø��(B)/��������� ���� ��ø��(C)/�Ⱓ�� ���ͷ� ��(D)/�ѱⰣ ������� ��(E)
	char	m_cChartShape;
	
	int		m_nUnitType;	//WON_TYPE,PERCENT_TYPE,DOLLAR_TYPE

	// '0' : �⺻,��
	// '1' : ������ �������� 
	// '2' : $
	// '3' : ���Ͻð�����
	// '4' : ���簡����
	// '5' : ������������.
	int		m_cDataGubun;

	// 0��, 1:��
	int		m_nLineType;	

	// ������ȸ����
	BOOL	m_bTodayRequest;

};

// ����Ʈ�� �����͸� �����ϹǷ� �� �������� �����ϱ� ���� ����.
// CShareInputData�� �ϳ��� �����ؼ� �����ؼ� ó���ϵ��� �����ʿ�.
class CShareInputData
{
public:
	CShareInputData();

	CCodeDataFromInputBar	m_DataInputBar;
	int		m_nScreenType;	// 0:����Ʈ, 1:���ϵ������Ʈ
	int		m_nBase;		// ���ϵ������Ʈ�϶� ������� 0:��������, 1:�ð�, 2:���簡
	BOOL	m_bCanDel;

	void	ReInit();
};

class CDlgCompDataHandle : public IDlgCompDataHandle
{
public:
	STDMETHOD_(void, LoadData)(long pDataKey);
	STDMETHOD_(void, SaveData)(long pDataKey);
	
	//��Ÿ ���������� �ʿ��� ����.
	STDMETHOD_(void, SaveSelGroup)(long pDataKey);
	STDMETHOD_(void, SaveShowGroupInfo)(long pDataKey);
	STDMETHOD_(void, SaveSelCodeInfo)(long pDataKey);
	STDMETHOD_(void, SaveTimeUse)(long pDataKey);

	STDMETHOD_(void, ClearAll)(long pDataKey);
	STDMETHOD_(void, ClearGroupList)(long pGroupList);
	STDMETHOD_(int,  CopyGroupList)(long pOrg, long pTarget);

	STDMETHOD_(void, DoCheckFirst)();
};

#endif // !defined(AFX_CODEDATAFROMINPUTBAR_H__1943BC6A_7285_42F7_928A_8B99C5A99F71__INCLUDED_)
