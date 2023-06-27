#pragma once


// CWrapper

class CWrapper : public CWnd
{
	//DECLARE_DYNAMIC(CWrapper)

public:
	CWrapper();
	virtual ~CWrapper();
	CString m_slog;

	CWnd* m_pWizard{};
	HANDLE m_Targethandle{};
	HWND m_hTarget{};
	CWnd* m_pSubWnd{};
	HANDLE	m_hSharedM;
	CRITICAL_SECTION csMapHandle{};
	LPVOID SetData_SharedM(BYTE* pdata, int len, int igubn);

	int InitSharedMemory();

	bool GetProcessModule(DWORD dwPID, char* sProcessName);
	HANDLE ProcessFind(char* strProcessName);



	void TestCode();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


