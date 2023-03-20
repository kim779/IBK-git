#pragma once
#include <afxwin.h>

class CHDwnd : public CWnd
{
public:
    CHDwnd();
    virtual ~CHDwnd();
    HWND	 m_hMain{};  //axis mainframe�� �ڵ�

    struct st_RequestHwnd
    {
        HWND hwnd;
        int key;
        char* pMid;
        BOOL bComplete;
    };
    CArray < st_RequestHwnd*, st_RequestHwnd*> m_hwndArr;

    CString m_slog;

    CWnd* m_pParent{};
    
    BOOL CreateMap(CWnd* pParent);
    CString Parser(CString& srcstr, CString substr);
    int        KeyTran(int ikey);                                               //��ȸ����� ��ûȭ�鿡 �����ٶ� �������ؼ� �����ֱ� ������ ��ӵ� Ű�� ��ȯ �ʿ�
    void     CheckMainMsg();
    void     sendTR(CString trCode, char* datB, int datL, int key);

    void	    Request_GroupList();											//���ɱ׷� ��ȸ
    void     Request_GroupCode(char* pdata);           //���ɱ׷쳻�� �����ڵ� ��ȸ
    void     Request_uploadEachGroup(int gno);


    void    DB_uploadBackup();                                              //DB ��� TR  (���ɼ��� ���½� ȣ��)
    void    DB_NewGroupAppend(char* pdata);                     //���׷���
    void    DB_GroupCodeSave(char* pdata);                         //�׷����� ���
public:
    LRESULT OnMsgFromMain(WPARAM wParam, LPARAM lParam);
    LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};

