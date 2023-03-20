#if !defined(AFX_LOCAL_H__44F0C9D2_6436_434C_8614_0C5E7BEB72CB__INCLUDED_)
#define AFX_LOCAL_H__44F0C9D2_6436_434C_8614_0C5E7BEB72CB__INCLUDED_

// #include "./LocalHeader.h"
#include "../../inc/IGateMng.h"
#include "../../inc/IAUGwanMng.h"
#include "../../inc/IWinixPlatform.h"
#include "../../Inc/IAUTrCommMng.h"

#include "../../Chart/COMMONTR/TRDefine.h"

//#include "../../../SrcSite/include/FxInterestManager.h"
//#include "../../../SrcSite/include/InterestInfo.h"
//������ȸ���� tr
struct _uinfo {
	char gubn[2]; // ��������
	   // "MY" : ��������
	   // "UL" : ����� ������ ��� 
	   // "UD" : ����� ������
	char dirt[1]; // ���۹��� 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : ����ȸ
	char cont[1]; // ���ӱ��� 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel, 
		 // 'S':�׷�����, 'E':�׷�����ó��, 'b':DB���, 'R':DB����, 'V':������������, 'r':�������º���
		 // 'D':�׷����, 'j':�ش�׷���������, 'g':�׷츮��Ʈ��ȸ, 'G':�׷���������, 'C':�������
	char name[80]; // file name
	char nblc[5]; // ������, ����ũ��(max:64k)
	char retc[1]; // return value 'O':���� 'E':���� 'U'�ű� ����
	char emsg[40]; // error message
};

struct _ginfo {
	char gnox[2]; // group number
	char gnam[20]; // group name
	char jrec[4]; // _jinfo count
};

struct _jinfo {
	char gubn[1]; // ���񱸺� 0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char code[12]; // �����ڵ�
	char xprc[10]; // �����ܰ�
	char xnum[10]; // ��������
};

struct _updn {
	_uinfo uinfo;
	_ginfo ginfo;
};

const int sz_uinfo = sizeof(struct _uinfo);
const int sz_ginfo = sizeof(struct _ginfo);
const int sz_jinfo = sizeof(struct _jinfo);

const UINT RMSG_SENDMESSAGE = ::RegisterWindowMessage(_T("RMSG_SENDMESSAGE"));

class CWinixGwanItem
{
public:
	CWinixGwanItem(HWND hWinix, HWND hDrfnWrap, HWND hGwanReciever);
	~CWinixGwanItem();
	
	void DoInitialize(BOOL bCreate);
	void RequestGwansimGroup();
public:
	// 2012.2.7 �ڼ���: ���� �ַθ�
	//CFxInterestManager* m_pIM;
	HWND  m_hWinix, m_hDrfnWrap, m_hGwanReciever;
	
	class CTrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData);
		STDMETHOD(StatusChange) (int nState);
		STDMETHOD(ReceiveMessage) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage);
	} m_xTrCommSite;
	friend class CWinixGwanItem;
	
	// 	enum{
	// 		wgi_ready=0,
	// 		wgi_querygroup=1,
	// 		wgi_queryitem,
	// 	};
	// 	int m_nQueryState;
	ITrCommSite* m_pCallBackSite;
};
typedef CList<CWinixGwanItem*, CWinixGwanItem*> List_CWinixGwanItem;

class CWinixGwanGroup
{
public:
	CString m_szKey;
	CString m_szData;
};
typedef CList<CWinixGwanGroup*, CWinixGwanGroup*> List_CWinixGwanGroup;

class CWinixGwanGroupList
{
public:
	CWinixGwanGroupList();
	~CWinixGwanGroupList();
	void _DoClearList();

public:
	int m_nVersion;
	List_CWinixGwanGroup m_List;
};

#endif //AFX_LOCAL_H__44F0C9D2_6436_434C_8614_0C5E7BEB72CB__INCLUDED_