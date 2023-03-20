// Account.h: interface for the CAccount class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CAccount  
{
public:
	CString m_strPriAcc;		//��ǥ���±���
	CString m_strSortNum;		//�����Ϸù�ȣ
	CString m_strAccntNum;		//���¹�ȣ
	CString m_strAccntName;		//�����̸�
	CString	m_strPassword;		//���º��
	CString m_strAllocRate;		//��������
	CString	m_strAllocMulti;	//�¼�
	CString m_strAccntNick;		//���º�Ī
	BOOL	m_bDelegation;		//���Ӱ��¿���
	BOOL	m_bWrap;			//Wrap���¿���
	BOOL	m_bAgent5;			//2013.03.12 KSJ ������ǰ�ŸŴ븮 ��������
	BOOL	m_bAgent6;			//2013.03.12 KSJ ���հ��°��ڱݴ�ü�븮�� ��������
public:
	CAccount()
	{
		m_strPriAcc = _T("");
		m_strSortNum = _T("");
		m_strAccntNum = _T("");
		m_strAccntName = _T("");
		m_strPassword = _T("");
		m_strAllocRate = _T("");
		m_strAllocMulti = _T("");
		m_strAccntNick = _T("");
		m_bDelegation = FALSE;
		m_bWrap = FALSE;
		m_bAgent5 = FALSE;
		m_bAgent6 = FALSE;
	};
	
	CAccount(CAccount const &other)
	{
		Copy(other);
	};
	
	~CAccount() {}
	
	inline void Copy(CAccount const &other)
	{
		m_strPriAcc = other.m_strPriAcc;
		m_strSortNum = other.m_strSortNum;
		m_strAccntNum = other.m_strAccntNum;
		m_strAccntName = other.m_strAccntName;
		m_strPassword = other.m_strPassword;
		m_strAllocRate = other.m_strAllocRate;
		m_strAllocMulti = other.m_strAllocMulti;
		m_strAccntNick = other.m_strAccntNick;
		m_bDelegation = other.m_bDelegation;
		m_bWrap = other.m_bWrap;
		m_bAgent5 = other.m_bAgent5;
		m_bAgent6 = other.m_bAgent6;
	};
	
	inline void TrimData()
	{
		m_strPriAcc.TrimRight();
		m_strSortNum.TrimRight();
		m_strAccntNum.TrimRight();
		m_strAccntName.TrimRight();
		m_strPassword.TrimRight();
		m_strAllocRate.TrimRight();
		m_strAllocMulti.TrimRight();
		m_strAccntNick.TrimRight();
	}
};