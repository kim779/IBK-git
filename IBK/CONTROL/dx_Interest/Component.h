#pragma once
// Component.h : header file
//


struct _hjcode
{
	char code[13]{};
	char name[17]{};
	char etc[45]{};
};


/////////////////////////////////////////////////////////////////////////////
// CComponent command target

class CComponent : public CCmdTarget
{
	DECLARE_DYNCREATE(CComponent)

	CComponent();           // protected constructor used by dynamic creation
	virtual ~CComponent();

// Attributes
public:
	CString		m_sRoot, m_sUser;
// Operations
public:
	CString Parser(CString &srcstr, CString substr);
	CString loadCdNNmList(CString sGroup);
	CString getMidString(char *pStr, int nFirst, int nLen);
	CString	loadCodeList(CString sGroup);
	CString	loadGroupNames();
	int	getGroupCnt();
	CString	getPrvGroupCode(CString GroupCode);
	CString	getNextGroupCode(CString GroupCode);
	CString	loadGroupList(CString GroupCode, int nCount);
	CString getGroupOrder();
	int	getGroupCodeIdx(CString GroupCode);
	CString	getIdxGroupCode(CString sIndex) ;
	boolean addItemCd(CString GroupCode, CString ItemCd);
	CString getCodeNm(CString ItemCd);
	BOOL	IsCodeInGroup(CString sGroup, CString Code);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponent)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
//protected:

	// Generated message map functions
	//{{AFX_MSG(CComponent)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CComponent)
	afx_msg BSTR LoadGroupNames();					// Group�� ��������
	afx_msg void SetUserName(LPCTSTR sUser);			// ���ɱ׷� ������ ������ User�� ����
	afx_msg BSTR LoadCodeList(LPCTSTR sGroup);			// ���ɱ׷��� �����ڵ� ��������
	afx_msg BSTR LoadCdNNmList(LPCTSTR sGroup);			// ���ɱ׷��� �����ڵ�� ����� ��������
	afx_msg BSTR LoadGroupList(LPCTSTR GroupCode, long nCount);	// Ư�����ɱ׷�(GroupCode) ������ ������ŭ�� ���ɱ׷�� ��������
	afx_msg long GetGroupCnt();					// ���ɱ׷� ���� ��ȯ
	afx_msg BSTR GetNextGroupCode(LPCTSTR nGroupCode);		// ���� ���ɱ׷��ڵ��� ���� ���ɱ׷��ڵ� ��������
	afx_msg BSTR GetPrvGroupCode(LPCTSTR GroupCode);		// ���� ���ɱ׷��ڵ��� ���� ���ɱ׷��ڵ� ��������. 
										// - ""���� �Է��ϸ� ù��° ���ɱ׷��ڵ带 ��ȯ�Ѵ�
	afx_msg long GetGroupCodeIdx(LPCTSTR GroupCode);		// �׷��ڵ��� ������ȣ ��������
	afx_msg BSTR GetIdxGroupCode(LPCTSTR sIndex);			// ������ȣ�� �ִ� �׷��ڵ� ��������
	afx_msg BOOL AddItemCd(LPCTSTR GroupCode, LPCTSTR ItemCd);	// ���ɱ׷쿡 �����ڵ� �߰��ϱ�
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
