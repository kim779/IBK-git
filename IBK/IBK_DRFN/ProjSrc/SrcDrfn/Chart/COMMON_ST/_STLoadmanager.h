#if !defined(AFX_STLOADMANAGER_IMPLE_H__6029D970_3E68_4DF0_84BE_B12B9DD33C6F__INCLUDED_)
#define AFX_STLOADMANAGER_IMPLE_H__6029D970_3E68_4DF0_84BE_B12B9DD33C6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// _STLoadManager.h

#include "../../inc/ISTLoadManager.h"
#include "STGfxOutBarCtrl.h"
#include "STListCtrl.h"
//{{ 20061012_ST�����߰�
//#include "Path.h"
//}} 20061012_ST�����߰�
#include "afxtempl.h"
#include "../../inc/ISTManager.h"

typedef struct {
	CString szSTName;
	CString szRawName;
}STFILEITEM;
typedef CList<STFILEITEM*, STFILEITEM*> List_STFILEITEM;
#define	Delete_List_STFILEITEMP(xpSTList) { for(POSITION pos=xpSTList->GetHeadPosition(); pos; ) { delete xpSTList->GetNext(pos); } xpSTList->RemoveAll(); }
#define	Delete_List_STFILEITEM(xSTList) { for(POSITION pos=xSTList.GetHeadPosition(); pos; ) { delete xSTList.GetNext(pos); } xSTList.RemoveAll(); }

class CSTLoadManager : public ISTLoadManager2
{
// Construction
public:
	STDMETHOD_(void, Init)(BOOL bSubFolder, CString szExtName, CImageList* pILSTIcon);
	STDMETHOD_(void, LoadST)(CWnd* pOutBar, CStringArray* parSTPath, DWORD dwSTTypeToLoad, int nDefaultSelFolder);
	STDMETHOD_(BOOL, UnloadST)();
	STDMETHOD_(int,  SearchST)(CString szSTName, CStringArray& arFindIndx);
	STDMETHOD_(BOOL, UpdateST)(int nState, CString szSTPath);
	STDMETHOD_(int,	 GetFolderST)(DWORD dwInfo);
	STDMETHOD_(int,	 GetItemST)(DWORD dwInfo);
	STDMETHOD_(void, SetSelectedItem)(int nFolderIndx = -1, int nItemIndx = -1);
	STDMETHOD_(void, SetUseBoldFont)(BOOL bUseBoldFont = TRUE) {m_bUseBoldFont = bUseBoldFont;}

	// p_LoadOption :	0 �̸� LoadST(..) �״�� ȣ��
	//					1 �̸� ���հ����� ������ �ε�(��ǥ,����/�༼,���ǵ� ����)
	//					2 �̸� ����� ���� �ε� ����.
	//					And(&) ���հ�����.
	STDMETHOD_(void, LoadST2)(CWnd* pOutBar, CStringArray* pArSTPath, DWORD dwSTTypeToLoad, int nDefaultSelFolder, int p_LoadOption) ;
	STDMETHOD_(void, SetSelectedItem)(STMNG_ITEMINFO* pInData);

// ����
public:
	CSTLoadManager();
	CSTLoadManager(CImageList* pILSTIcon, CWnd* pParent = NULL);

// �������	
protected:
	enum
	{
		Basic_STChange_Delete=0x10,		//�ʱ������� ����
		Basic_STChange_Add=0x11,		//�ʱ������� �߰�
		Ext_STChange_SDDelete=0x20,		//��������� ����ڴ�����������
		Ext_STChange_SDAdd=0x21,		//��������� ����ڴ��������߰�
		Ext_STChange_Delete=0x40,		//��������� �����������������
		Ext_STChange_Add=0x41,			//��������� ��������������߰�
	};

	CSTGfxOutBarCtrl*					m_pOutBarCtrl;
	CStringArray*						m_parSTPath;			// Data / User ���� ��� StringArray
	CString								m_szCurrentSTPath;		// �˻����� ���� �������
	CImageList*							m_pILSTIcon;

	CString								m_szExtName;			// �������δ��� ���� �˻��� ���� Ȯ����
	BOOL								m_bSubFolders;			// ���� ���� ������� �˻����� ����

	UINT								m_uSTIndx;				// ������ �ǹ��ϴ� ListCtrl ID Indexing
	CString								m_szSTPath;
	BOOL								m_bSearching;			// �������δ��� ���� �˻� ��
	BOOL								m_bSTUserLoading;		// �������δ��� ���� ������������� �˻� ��
	UINT								m_uSTUserCount;			// ListCtrl ID ����� ���� ����ڴ���/�������� ���� Counting
	DWORD								m_dwSTTypeToLoad;		// Reserved
	BOOL								m_bUseBoldFont;
	//int									m_nLoadOption;			// �ƿ���� �ε�ɼ�


// ����Լ�
public:
	BOOL GetSelectedItem(STMNG_ITEMINFO& InData);
	void DeleteSTFile(int nUserIndx, CString szSTFile);
	BOOL GetRunSTItemInfo(CString szSTName, STMNG_ITEMINFO& InData);

protected:
	void LoadSTList(BOOL bOnlyUser = FALSE);

	// p_LoadOption :	0 �̸� LoadST(..) �״�� ȣ��
	//					1 �̸� ���հ����� ������ �ε�(��ǥ,����/�༼,���ǵ� ����)
	//					2 �̸� ����� ���� �ε� ����.
	//					And(&) ���հ�����.
	void LoadSTListFromSTIndex(int p_LoadOption, BOOL bOnlyUser = FALSE);

	BOOL IsSTTypeToLoad(CString& szFileTitle, CString& szSTType);
	void AddFolder(LPCTSTR szSTFolderPath, LPCSTR szLastFolerName);
	void AddSTFile(LPCTSTR szSTFilePath, LPCSTR p_szShowName=NULL);
	void GetFolderName(CString szPath, CString& szSTFolder);
	void SetExtName(CString szExtName)	{ m_szExtName = szExtName; }
	int	 GetFolderIndexFromFolderName(CString szFolderName);

	// ������丮�� ã������ ��ƾ
	int	DirectorySearch(LPCSTR pszStartDir, LPCSTR pszExt, BOOL bSub=TRUE);

	// �������� �ش�Ÿ���� ������ �о �߰��Ѵ�.
	void LoadSTFolderList(LPCSTR p_szGroupName, LPCSTR p_szTargetDir, int p_nSTType);

	/*	p_nSTType�� ST_ReadFrom_Dir �϶��� �����Ѵ�.
		1	 : @1	-> ��ǥ
		237 : @2,@3,@7 -> ��ǥ, �����༼ ����
		5	 : @5	-> �����༼
		999 : ���ηε� */
	static int GetSTFileList(LPCSTR p_szDir, int p_nSTType, List_STFILEITEM *p_arrSTList);
	static int GetSTFileList(LPCSTR p_aTarDir, CStringArray &p_arr);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLOADMANAGER_IMPLE_H__6029D970_3E68_4DF0_84BE_B12B9DD33C6F__INCLUDED_)
