// 2000.11.24 ���ؿ�(Junok Lee)(mailto:alzio@dooriic.co.kr)
// CBaseDesk2, CDeskParent2   added by Junok Lee. 2001. 07. 12

#if !defined(AFX_0DBASE_DESK__D1DA4A3_84KKEI_EJKJKJD_FHDD0D2E__INCLUDED_)
#define AFX_0DBASE_DESK__D1DA4A3_84KKEI_EJKJKJD_FHDD0D2E__INCLUDED_

// #include "../include/BaseDesk.h"

#define		GDB_OK					0	// ����
#define		GDB_ERROR				-1	// UnKnown Error

class CBaseDesk {
public:
	// * �⺻ ��ġ��θ� ��´�.
	// * �� ���丮�� �������� �ؼ� Bin, Data ���� ���丮�� ���Ѵ�.
	virtual LPCSTR GetBasePath() { ASSERT(FALSE); return NULL; }

	// * ����ũ�� �ڵ��� ���Ѵ�.
	virtual HWND  GetDeskHwnd() { ASSERT(FALSE); return m_hDeskHwnd; }

	// * ����ũ�� IDispatch�� ���Ѵ�.
	virtual LPDISPATCH GetUnknown() { ASSERT(FALSE); return NULL; }

	// * ��Ʈ���� IDispatch�� ���Ѵ�.
	virtual LPUNKNOWN GetCtrlUnknown(LPCTSTR szName) { return NULL; }

	// * ��Ʈ���� �ڵ��� ���Ѵ�.
	virtual HWND GetCtrlHwnd(LPCTSTR szName) { return NULL; }

	//* Global ������ Ŭ���� �����͸� ���Ѵ�.
	virtual LPVOID GetSharedInterface() { return NULL; }

	// * �ڵ��� ������ �̺�Ʈ�� �߻���Ų��.
	// * ����ũ���� �̺�Ʈ�� �Ͼ�� �˸���.
	virtual BOOL InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam) { return TRUE; }

	// * ��Ʈ���� �̸����� �̺�Ʈ�� �߻���Ų��.
	// * ����ũ���� �̺�Ʈ�� �Ͼ�� �˸���.
	// * ��Ʈ���� �̸��� SetAllProperties2�� ���ؼ� �Ѿ��.
	// * ���� ���� �������� �𸣴� Ȯ���ϰ� �۾��ϱ� �ٶ�. 2000.12.04
	// * !VBNAME= ���·� ���� �Ѱ��ش�.
	virtual BOOL InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam) { return TRUE; }

	// * ����ũ���� Ű ���¸� �˸���.
	virtual BOOL SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl) { return FALSE; }

	// * Local �ʴ��� ���� �ڷḦ ���Ѵ�.
	virtual LPVOID GetLSharedData() { return NULL; }

	// * ����ũ���� �ڸǵ������� ����� ���Ѵ�.
	virtual void CallCommand(LPCSTR _szCmd) {}

protected:
	CString	m_szPath;
	HWND	m_hDeskHwnd;
	LPUNKNOWN	m_pDeskUnk;
};

class CBaseDesk2 : public CBaseDesk
{
public:
	// * ���������� GetMainInfo �Լ��� ȣ���� �� Ű���� "USERCOLORTABLE" �ؼ� ���� �����.
	// * nFlag 0 : nIndex���� �̿��� �÷��� ���ϱ�
	// *       1 : szName�� �̿��� �÷��� ���ϱ�
	// * 36=�Ϲ������ӹ�����,190,221,250
	// * nFlag=0�̰� nIndex�� 36�� �� RGB(190,221,250)
	// * nFlag=1�̰� szName�� "�Ϲ������ӹ�����"�� �� RGB(190,221,250)
	virtual OLE_COLOR GetUserColor(short nIndex, LPCTSTR szName, short nFlag) { return RGB(0,0,0); }

	// * ����ũ���� �ʿ��� ������ ��û�Ѵ�.
	// * �ʿ��� ������ ��û�ؼ� �߰��Ѵ�. �ϴ� ��Ʈ�ѿ� �߰��ϸ� ����.
	virtual long GetDeskInfo(LPCSTR szKey) { return NULL; }

	// * ������� ������ ���� ���� ��û�Ѵ�.
	// * �ʿ��� ������ ��û�ؼ� �߰��Ѵ�. �ϴ� ��Ʈ�ѿ� �߰��ϸ� ����.
	// * CBaseDeskParent2�� �ִ� ���� �Լ��� ���Ѵ�.
	virtual long GetUserInfo(LPCSTR szKey) { return NULL; }

	// * ���� �ö󰡴� �������� ������ ���� ���� ��û�Ѵ�.
	// * �ʿ��� ������ ��û�ؼ� �߰��Ѵ�. �ϴ� ��Ʈ�ѿ� �߰��ϸ� ����.
	// * CBaseDeskParent2�� �ִ� ���� �Լ��� ���Ѵ�.
	virtual long GetFrameInfo(LPCSTR szKey) { return NULL; }

	// * ����(CMainFrame, CWinApp)��� �����ϴ� ������ ���� ���� ��û�Ѵ�.
	// * �ʿ��� ������ ��û�ؼ� �߰��Ѵ�. �ϴ� ��Ʈ�ѿ� �߰��ϸ� ����.
	// * CBaseDeskParent2�� �ִ� ���� �Լ��� ���Ѵ�.
	virtual long GetMainInfo(LPCSTR szKey) { return NULL; }
};

// ���� �Ǵ� NeoDesk�� �θ𿡼� ����������� �⺻ Ŭ����
class CBaseDeskParent 
{
public:
	// * �θ𿡰� �⺻���(��ġ���)�� ���´�. 
	virtual LPCSTR GetBasePath() { ASSERT(FALSE); return NULL; }

	// * �θ��� �ڵ��� ���Ѵ�.
	virtual HWND  GetParentHwnd() { ASSERT(FALSE); return NULL; }

	// * �θ��� IDispatch �����͸� ���Ѵ�.
	virtual LPDISPATCH GetParentDisp() { ASSERT(FALSE); return NULL; }

	// * { �����ڵ� ó��
	// * CBaseCodeTable�� �����͸� �Ѱ��ش�.
	// * ���� ������� �ʴ´�.
	virtual long GetBaseCodeTable() {ASSERT(FALSE);return NULL;}
	// }

	// { ȭ�鿭��
	// _nFlag : (0) ��������� ȭ�鿩�� ���� ����
	//          (1) ���� ��ȭ�鿡 ����.
	//          (2) ���� ����ȭ�鿡 ����.
	//          (3) ���� �˾����� ����.
	//          (4) ���� ���̾�α� ���·� ����.
	// _szParam : ȭ���� ���鼭 �� ȭ�鿡�� �Ѱ��ִ� ������
	// * �Ʒ� OpenMap�� ������� ������. �޽����������� ȭ���� �������ϴ� ��ƾ���� �ٲ������.
	virtual BOOL OpenMap(LPCSTR _szMapID, int _nFlag, LPCSTR _szParam) {ASSERT(FALSE); return FALSE;}
	// }

	// { ��Ÿ �������� ���� ����� ���� �߰�(1)
	// ���ϴ� _szFunc����� ������ TRUE,
	// _szFunc, _szParam�� �̿��� ���ϴ� ó���� �Ѵ�.
	virtual BOOL InvokeEx1(LPCSTR _szFunc, LPCSTR _szParam, CString& szResult) {ASSERT(FALSE); return FALSE;}

	// { ��Ÿ �������� ���� ����� ���� �߰�(2)
	// ���ϴ� _szFunc����� ������ TRUE,
	// _szFunc, _dwParam �̿��� ���ϴ� ó���� �Ѵ�.
	virtual BOOL InvokeEx2(LPCSTR _szFunc, long _dwParam, long& dwResult) {ASSERT(FALSE); return FALSE;}
	// }

	// * ����ũ�� �θ𿡰� �ڸǵ������� ����� ���Ѵ�.
	virtual void CallCommand(LPCSTR _szCmd) {}

	// * ����ũ�� �θ𿡰� �ڸǵ������� ����� ���Ѵ�.
	virtual void CallCommand(int _nType, long _dwData, long& dwResult) {}

	// * ����ũ�� �θ𿡰� �ڸǵ������� ����� ���Ѵ�.
	// * �ڵ��� ���� �Ѱܴ��� ��ɸ� �� �߰��� ����
	virtual void CallCommand(HWND _hDesk, LPCSTR _szCmd) {}

	// * Ű���� �ְ� �ű⿡ �ش��ϴ� ������ ���ϱ� ���� ����Ѵ�.
	// * �� �κ��� ����Ʈ���� �پ��ϰ� ����ϱ� ���� ������� Ȱ���� �� �ִ�.
	// * ���ΰ� � Ű���϶��� � ������ ���������� ��Ⱑ �Ǿ�� �Ѵ�.
	virtual long GetKeyData(LPCSTR _szKey1, LPCSTR _szKey2){return NULL;}
	virtual long GetKeyData(LPCSTR _szKey, int _nKey){return NULL;}
};

class CBaseDeskParent2 : public CBaseDeskParent
{
public:
	// * ������� ������ ���� ���� ��û�Ѵ�.
	// * �ʿ��� ������ ��û�ؼ� �߰��Ѵ�. �ϴ� ��Ʈ�ѿ� �߰��ϸ� ����.
	// * CBaseDeskParent2�� �ִ� ���� �Լ��� ���Ѵ�.
	virtual long GetUserInfo(LPCSTR szKey) { return NULL; }

	// * ���� �ö󰡴� �������� ������ ���� ���� ��û�Ѵ�.
	// * �ʿ��� ������ ��û�ؼ� �߰��Ѵ�. �ϴ� ��Ʈ�ѿ� �߰��ϸ� ����.
	// * CBaseDeskParent2�� �ִ� ���� �Լ��� ���Ѵ�.
	virtual long GetFrameInfo(LPCSTR szKey) { return NULL; }

	// * ����(CMainFrame, CWinApp)��� �����ϴ� ������ ���� ���� ��û�Ѵ�.
	// * �ʿ��� ������ ��û�ؼ� �߰��Ѵ�. �ϴ� ��Ʈ�ѿ� �߰��ϸ� ����.
	// * CBaseDeskParent2�� �ִ� ���� �Լ��� ���Ѵ�.
	virtual long GetMainInfo(LPCSTR szKey) { return NULL; }
};

#endif // AFX_0DBASE_DESK__D1DA4A3_84KKEI_EJKJKJD_FHDD0D2E__INCLUDED_
