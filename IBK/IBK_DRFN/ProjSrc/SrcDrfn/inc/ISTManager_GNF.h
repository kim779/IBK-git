#if !defined(AFX_INTERFACE_OF_STCONTENTS_BY_JUNOKLEE_060721A__INCLUDED_)
#define AFX_INTERFACE_OF_STCONTENTS_BY_JUNOKLEE_060721A__INCLUDED_

// #include "../../inc/ISTManager.h" // �ý���Ʈ���̵� ���õ� ������ �������̽�

#define		UUSTID_ISTAnalysisInput		0

//{{ 20061012_ST�����߰�
#include <afxtempl.h>

interface ISTInterface
{
	STDMETHOD_(void*, GetInterface)(int nID) PURE;
	STDMETHOD_(LONG, GetOutBarColor) (COLORREF& clrOLBkgrnd, COLORREF& clrOLOutLine) PURE;
};

typedef struct {
	CString szSTRData;		// ST���丮 ������ ���� �����̸�
	CString szSTName;		// ������
	CString szSTPath;		// �ش������� ���丮����
							// ��ü���ϰ�� = szSTPath + "\\" + szSTName + ".st"
	int		nSTType;		// ������ ����, ��������(0), ����� ����(1)
							// src\Common_ST\LoadSTDir.h �� ���ǵǾ� ����.
	int		nFolderIndx;	// �׷��ε���, OutlookBar-CListCtrl ��Ÿ���϶�.
	int		nItemIndx;		// �������ε���, OutlookBar-CListCtrl ��Ÿ���϶�.
} STMNG_ITEMINFO;

typedef CList<STMNG_ITEMINFO*, STMNG_ITEMINFO*> LIST_STMNG_ITEMINFO;

typedef struct : public STMNG_ITEMINFO{
	int		  nSTLevel;		// 0:�ʱ�, 1:���
							// src\Common_ST\LoadSTDir.h �� ���ǵǾ� ����.
	HTREEITEM hTreeItem;	// OutlookBar-CTreeCtrl ��Ÿ���϶�.
	HTREEITEM hParentItem;	// ���� Ʈ���� �ڵ�, ���簡 Item���϶��� ���õ�.
	BOOL	  bIsItem;		// TRUE(Item), FALSE(Folder��)
} STMNG_ITEMINFO_EX1;

// LIST_STMNG_ITEMINFO* a;
// DELETE_PLIST_STMNG_ITEMINFO(a);
#define	DELETE_PLIST_STMNG_ITEMINFO(xxpList) { for(POSITION xpos=xxpList->GetHeadPosition(), xpos; ) delete xxpList->GetNext(xpos); \
				xxpList->RemoveAll(); }

// LIST_STMNG_ITEMINFO a;
// DELETE_LIST_STMNG_ITEMINFO(a);
#define	DELETE_LIST_STMNG_ITEMINFO(xxList) { for(POSITION xpos=xxList.GetHeadPosition(), xpos; ) delete xxList.GetNext(xpos); \
	xxList.RemoveAll(); }


interface ISTUserData
{
	// �����Ͱ��� ����.
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwCLose,LONG& dwVolume, int& nBong) PURE;
};
//}} 20061012_ST�����߰�

interface ISTAnalysisInput
{
//{{ 20061012_ST�����߰�
	//BOOL	m_bUseUserData;					// �����ϴ� Ŭ�������� ���������� ����
	//ISTUserData* m_pInterfaceUserData;	// �����ϴ� Ŭ�������� ���������� ����
	STDMETHOD_(void, SetUserData)(BOOL bUse, ISTUserData* pInterfaceUserData) PURE;	// TRUE �ϰ�쿡�� ISTUserData* ���� ��������� �Ѵ�.
	STDMETHOD_(BOOL, IsUseUserData)() PURE;					// ����ڰ��� ������ ��뿩��
	STDMETHOD_(ISTUserData*, GetUserDataInterface)() PURE;	// �ð�����ȭ�� ���� ISTAnalysisInput*���� �����ϴ� GetChartData ��ſ�

	// ID�� ������
	STDMETHOD_(COleVariant, GetData)(int nID)  PURE;

	// 0: �����ڵ� , GetCode(0)
	STDMETHOD_(CString, GetCode)() PURE;

	// �����ڵ��, GetCode(1)
	STDMETHOD_(CString, GetCodeName)() PURE;

	// �����ڵ� ���� : 0:�ֽ�, 1:����, GetCode(2)
	// �ڵ������� ��Ƽ��Ʈ�� InputBarDlg.h�� ���ǵ� �� ���� : enum	{STOCK_CHART, UPJONG_CHART, FUTOPT_CHART, FOREIGN_CHART};
	STDMETHOD_(int, GetCodeType)() PURE;

	// ������, GetCode(3)
	//STDMETHOD_(CString, GetSTName)() PURE;
	// ����������� ��������, TRUE/FALSE
	STDMETHOD_(BOOL, GetSTInfo)(STMNG_ITEMINFO* pInfo) PURE;

	// ����, GetCode(4)
	STDMETHOD_(int, GetCount)() PURE;

	// ������, GetCode(5)
	STDMETHOD_(CString, GetEndDay)() PURE;

	// ���Ҽ�, GetCode(6)
	STDMETHOD_(int, GetDivideCnt)() PURE;

	// ���� ����, GetCode(7)
	// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)
	STDMETHOD_(int, GetDivideState)() PURE;

	// GetCode(8)
	// ���� ������ ���� ���� ���� 1, 5, 10, 15 ��ư���� ǥ�õ� �κ�.
	// ���ϰ� 1(0),5(1),10(2),15(3)
	STDMETHOD_(int, GetUnitPos)() PURE;

	// ��ø��, GetCode(9)
	STDMETHOD_(double, GetDeivideRate)() PURE;

	// �����Ͱ��� ����.
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwCLose,LONG& dwVolume, LONG& dwDataExt) PURE;

	// ���ҿ���
	STDMETHOD_(BOOL, IsDivide)() PURE;

	// ��Ʈ���� ���´�.
	// nID  0 : ISysTabExChartItem*�� �Ѱܹ޴´�. GetChartItem()�� ���ؼ� ChartOcx �����ͷ� ��ȯ�ؼ� ����� �� �ִ�.
	//          ������ ��� CKoscomChart* ������ ��ȯ�ؼ� ����� �� �ִ�.
	//		1 : CListCtrl
//{{ 20061012_ST�����߰�
	STDMETHOD_(LONG, GetControl)(int nID) PURE;

	// 20081016 �ڽ·� - 8���� ��ȯó��
	STDMETHOD_(CString, GetPacketType)() PURE;

	//>> 20081203 �ڽ·� - 30�� TimeFormat ó��
	STDMETHOD_(BOOL, Is30SecUnit)() PURE;
	//<< 20081203 �ڽ·� - 30�� TimeFormat ó��
};


#endif // AFX_INTERFACE_OF_STCONTENTS_BY_JUNOKLEE_060721A__INCLUDED_
