#if !defined(AFX_INTERFACE_OF_MAININFO_1464_45BB_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_MAININFO_1464_45BB_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/IMainInfoMng.h"
#include "IBaseDefine.h"
//  int nID = 21;
// 	IMainInfoManager* pMainInfoMng = (IMainInfoManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(nID);

#define	MAX_IMainInfoManager		30  // ���� ID������ '0'

#define	USERINFOMNG_USERID			0
#define	USERINFOMNG_USERENCID		1
#define	USERINFOMNG_ROOTDIR			10		// Root Directory
#define	USERINFOMNG_EXECDIR			11		// ���� ���丮
#define	USERINFOMNG_DATADIR			12		// Data ���丮
#define	USERINFOMNG_REALUSERDIR		17		// �������� ����� ���丮. ����Ʈ���� �ٸ�.

// ID ���� �� �ڼ��� ������ ����[����_PC�����͸Ŵ���.xls] �� [����_PC�����͸Ŵ���.doc] ����. 
/* --------------------------------------
ID		Contents
--------------------------------------
0		�α��ξ��̵�
1		��ȣȭ�� ���̵�
2		�α���(�����)��
3		����ھ�ȣ
4		��������й�ȣ
5		����ڱ���
6		����ڵ��
7		�������α��γ���
8		�ֹι�ȣ
9		HTS�������Ͽ���
10		��Ʈ���丮
11		����(bin)���丮
12		������(Data)���丮
13		����(Help)���丮
14		�̹���(Image)���丮
15		�α�(Log)���丮
16		��(Map)���丮
17		����ڵ��丮(User/#ID)
18		����������뿩��
19		�������� �޽��� �ȳ��� ǥ�� ����
20		�ڵ��Է¿���
21		������
22		����IP
23		���ӽð�
24		��������
25		��������
26		�����IP
27		���ӹ��
28		ATM
-------------------------------------- */

#define		IMainInfoManagerLast		IMainInfoManager0002

DECLARE_INTERFACE_(IMainInfoManager, IBaseDataString)
{
	// '����ھ��̵�' ����
	// I-PARAM : -
	// O-PARAM : �������̵�
	STDMETHOD_(LPCSTR, GetUserID)() PURE;

	// '��ȣȭ�� ����ھ��̵�' ����
	// I-PARAM : -
	// O-PARAM : ��ȣȭ�� �������̵�
	STDMETHOD_(LPCSTR, GetEncUserID)() PURE;
	
	// Root���丮 (bin���丮�� ���ܰ� Dir)
	// I-PARAM : -
	// O-PARAM : Root���丮
	STDMETHOD_(LPCSTR, GetRootDir)() PURE;

	// Data���丮
	STDMETHOD_(LPCSTR, GetDataDir)() PURE;

	// UserID�� �Ҵ�� �������丮
	// I-PARAM : -
	// O-PARAM : UserID�� �Ҵ�� �������丮
	STDMETHOD_(LPCSTR, GetEncUserDir)(LPCSTR szKey=NULL) PURE;
	
	// szKey="ISHTS"�� �� ����(���� HTS���� �˷��ش�.
	// I-PARAM : LPCSTR szKey
	// O-PARAM : 
	STDMETHOD_(CString, GetLoginInfo)(LPCSTR szKey) PURE;

	//-->@.ȯ�溯�� ó�� ����
	STDMETHOD_(BOOL, SetEnvironmentVariable)(
				LPCTSTR lpName,  // environment variable name
				LPCTSTR lpValue  // new value for variable
			) PURE;

	STDMETHOD_(DWORD, GetEnvironmentVariable)(
				LPCTSTR lpName,		// environment variable name
				CString &rValue		// CString for variable value
			) PURE;
	STDMETHOD_(BOOL, FreeEnvironmentStrings)(
				LPTSTR lpName  // environment variable name
			) PURE;

	STDMETHOD_(DWORD, GetEnvironmentStrings)(CStringList& szBufferList) PURE;

	// INI���Ͽ��� �о �����Ѵ�.
	// INI������ �Ʒ�����, key�� data ������ �ݷ�(:)���� �Ѵ�.
	// Count= 0���� ����.
	// #=key:data
	STDMETHOD_(int, IniToEnvironmentTable)(LPCSTR szIniPath, LPCSTR szSect) PURE;	
	//<--
};

DECLARE_INTERFACE_(IMainInfoManager0002, IMainInfoManager)
{
	STDMETHOD_(int, GetCountOfAccount)() PURE;
	STDMETHOD_(BOOL, GetUserAccount)(CStringArray& saAccount) PURE;
	STDMETHOD_(CString, GetAccountName)(LPTSTR lpszAccount) PURE;
	STDMETHOD_(CString, GetAccountPass)(LPTSTR lpszAccount) PURE;
	
	STDMETHOD_(int, GetHtsMode)() PURE;
	STDMETHOD_(CString, GetLoginPass)() PURE;
};

#define	USE_IMainInfoManager(xxVal) IMainInfoManager* xxVal=(IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);



#endif //AFX_INTERFACE_OF_MAININFO_1464_45BB_BY_JUNOK_LEE__INCLUDED_
