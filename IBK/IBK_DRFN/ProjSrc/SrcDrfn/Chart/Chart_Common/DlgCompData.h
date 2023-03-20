#if !defined(AFX_DLGCOMPDATA_H__6433A5DB_243F_46FD_BDF5_88DD98F6C29A__INCLUDED_)
#define AFX_DLGCOMPDATA_H__6433A5DB_243F_46FD_BDF5_88DD98F6C29A__INCLUDED_

//#include "../Chart_Common/DlgCompData.h"

#include <AFXTEMPL.H>

DECLARE_INTERFACE(IDlgCompDataHandle)
{
	CString m_szDataFile;

	STDMETHOD_(void, LoadData)(long pDataKey) PURE;
	STDMETHOD_(void, SaveData)(long pDataKey) PURE;

	//��Ÿ ���������� �ʿ��� ����.
	STDMETHOD_(void, SaveSelGroup)(long pDataKey) PURE;
	STDMETHOD_(void, SaveShowGroupInfo)(long pDataKey) PURE;
	STDMETHOD_(void, SaveSelCodeInfo)(long pDataKey) PURE;
	STDMETHOD_(void, SaveTimeUse)(long pDataKey) PURE;

	// ����õ�� ������ Ŭ������ �� Call
	STDMETHOD_(void, ClearAll)(long pDataKey) PURE;

	// �׷������� Ŭ�����Ѵ�. 
	//List_CDlgCompGroupData* pGroupList
	STDMETHOD_(void, ClearGroupList)(long pGroupList) PURE;

	// �׷������� �����Ѵ�.
	//List_CDlgCompGroupData* pOrg, List_CDlgCompGroupData* pTarget
	STDMETHOD_(int,  CopyGroupList)(long pOrg, long pTarget) PURE;
	
	// ó�������϶��� ����Ʈó����
	STDMETHOD_(void, DoCheckFirst)() PURE;
};

class CDlgCompSubItem
{
public:
	CString		m_szCode;
	CString		m_szCodeName;
	COLORREF	m_clr;
	int			m_nDrawStyle;
				//	Chart OCX���� ����ϴ� �ڵ�.
				//		enum LINEDRAWSTYLE		// LINESTYLE
				//		{
				//			Solid_Draw = 0,			// �Ǽ�
				//			Dot_Draw,				// ��ǥ����
				//			DottedLine_Draw,		// ���� ����
				//			DashDot_Draw,			// ������
				//			DashDotDot_Draw			// ��������
				//		};
	int			m_nThickness;
};
#define INIT_CDlgCompSubItem(xItem) {xItem.m_clr=RGB(255,0,0); xItem.m_nDrawStyle=0; xItem.m_nThickness=0;}
#define INIT_CDlgCompSubItemP(pItem) {pItem->m_clr=RGB(255,0,0); pItem->m_nDrawStyle=0; pItem->m_nThickness=0;}

typedef CList<CDlgCompSubItem*, CDlgCompSubItem*> List_CDlgCompSubItem;
class CDlgCompGroupData
{
public:
	CString			m_szGroupName;
	List_CDlgCompSubItem m_ItemList;
};

typedef CList<CDlgCompGroupData*, CDlgCompGroupData*> List_CDlgCompGroupData;
typedef struct //CDlgCompData
{
public:
	List_CDlgCompGroupData	m_GroupList;
	int		m_nGroupCnt;	// �׷�Count
	int		m_nBase;		// �� ���ذ�. 0:��������,1:���Ͻð�,1:���簡
	int		m_nTimer;		// ��������(��������)
	BOOL	m_bCurDisp;		// ���簡(����)ǥ�� ����
	int		m_nTypeUnit;	// ��к�����.
	
	CString m_szSelectGroup;	// ����ڰ� ������ �׷�
	CString	m_szSelCode, m_szSelCodeName;	// 1�������񺸱� �ϴ� ���õ� ��������.
	BOOL	m_bShowAll;			// 1:�⺻�׷����� ����, 0:�������񺸱�
	BOOL	m_bTimerUse;		// �������� ��뿩��.

	IDlgCompDataHandle*	m_pFn;
} STDlgCompData;

#endif // !defined(AFX_DLGCOMPDATA_H__6433A5DB_243F_46FD_BDF5_88DD98F6C29A__INCLUDED_)
