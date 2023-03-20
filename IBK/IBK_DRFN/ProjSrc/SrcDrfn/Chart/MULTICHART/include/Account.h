#if !defined(AFX_ACCOUNT_H__F81234A1_D6B6_11D4_B31B_0048543A0D2E__INCLUDED_)
#define AFX_ACCOUNT_H__F81234A1_D6B6_11D4_B31B_0048543A0D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Account.h : header file
//

#include <afxTempl.h>
#define		SIZEONEACCT		51  // ����(11) + ���¸�(40)
                                // �������� �������� ���� ��Ŷ 

#define		SIZEFULLACCT	64  // ����(11) + ���¸�(40) + ��й�ȣ(13)
                                // ����ó���� SIZEFULLACCT ������

#define		SIZETOTALACCT	66  // ����(11) + ���¸�(40) + ��й�ȣ(13) + ��Ÿ����(1) + ��й�ȣ����(1)

class CAcct
{
public:
	CString m_acctA, m_acctB, m_acctC,
			m_acctName,
			m_acctEncPass,
			m_acctFlag;			// ��, Ÿ ����flag('I', 'Y')
	int		m_nSaveMode;		// �������(0), ��������(1), �ӽ�����(2)
	
	// �����Ͱ� �Ѿ�ö��� ����Ʈ�ڽ��� ��Ÿ���� ���´�� �Ѿ�´�.
	// �ڵ� ü�� [3(������ȣ)-2(��ǰ����)-6(���¹�ȣ)]
	// ����Ű�� ��ǰ�����ڵ�� ����
	CAcct(CString szData)
	{
		int nCount = szData.GetLength();
		if(nCount >= SIZETOTALACCT)
		{
			m_acctA = szData.Left(3);
			m_acctB = szData.Mid(3,2);
			m_acctC = szData.Mid(5, 6);
			m_acctName = szData.Mid(11, 40);
			m_acctEncPass = szData.Mid(51,13);
			m_acctFlag = szData.Mid(64,1);
			m_nSaveMode = atoi(szData.Mid(65,1));
			
			m_acctName.TrimLeft();	m_acctName.TrimRight();
			m_acctEncPass.TrimLeft();	m_acctEncPass.TrimRight();
		}
	}

	// * ���¹�ȣ 64byte�� ���´�.
	void GetString(CString &rString)
	{
		CString strPass = m_nSaveMode > 0 ? m_acctEncPass : _T("");

		rString.Empty();
		rString.Format("%3.3s%2.2s%6.6s%-40.40s%-13.13s", 
					m_acctA, 
					m_acctB,
					m_acctC,
					m_acctName, 
					strPass);
	}

	// * ���¹�ȣ 64byte�� ���忩�� 1byte�� ���´�. 
	void GetString2(CString &rString)
	{
		CString strPass = m_nSaveMode > 0 ? m_acctEncPass : _T("");

		rString.Empty();
		rString.Format("%3.3s%2.2s%6.6s%-40.40s%-13.13s%1.1d", 
					m_acctA, 
					m_acctB,
					m_acctC,
					m_acctName, 
					strPass,
					m_nSaveMode);
	}

	void GetString3(CString &rString)
	{
		CString strPass = m_nSaveMode > 0 ? m_acctEncPass : _T("");

		rString.Empty();
		rString.Format("%3.3s%2.2s%6.6s%-40.40s%-13.13s%-1.1s%1.1d", 
					m_acctA, 
					m_acctB,
					m_acctC,
					m_acctName, 
					strPass,
					m_acctFlag,
					m_nSaveMode);
	}

	// * �������¹�ȣ�� ���´�. 
	void GetPureAcct(CString& rString)
	{
		rString.Empty();
		rString.Format("%3.3s%2.2s%6.6s",
					m_acctA,
					m_acctB,
					m_acctC);
	}
};

class CAcctHelper
{
public:
	~CAcctHelper()
	{
		ClearAll();
	}

	void ClearAll()
	{
		for(POSITION pos=m_List.GetHeadPosition(); pos; )
		{
			CAcct* pData = m_List.GetNext(pos);
			delete pData;
		}
		m_List.RemoveAll();
	}

	void Add(CString szData)
	{
		if(szData.GetLength()<SIZEONEACCT)
			return;

		CAcct* pAcct = new CAcct(szData);
		m_List.AddTail(pAcct);
	}

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ������(Yoo-Kang, Choi)
	// ���� ��¥          : 2003/10/30
	// ���� Ÿ��          : int
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		CString _szKey     : ��ǰ�ڵ� ���ڸ�
	//		CString& rString   : [out] ���� ����Ʈ
	// �Լ� ����          : Ư�� ��ǰ�ڵ��� ���������� ��� �Լ�
	//						����Ʈ�� �ִ� ������� ��Ʈ���� �����Ѵ�. 
	///////////////////////////////////////////////////////////////////////////////
	int GetKeyString(CString _szKey, CString& rString)
	{
		int nCount = 0;
		for(POSITION pos = m_List.GetHeadPosition(); pos; )
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				if(pAcct->m_acctB.Compare(_szKey)==0)
				{
					nCount++;
					CString szTemp;
					pAcct->GetString(szTemp);
					rString += szTemp;
				}
			}
		}
		return nCount;
	}

	///////////////////////////////////////////////////////////////////////////////
	// ������             : ������(Yoo-Kang, Choi)
	// ���� ��¥          : 2003/10/30
	// ���� Ÿ��          : int
	// ���� Ÿ�� ����     : 
	// �Ķ����
	//		CString _szKey     : ��ǰ�ڵ� ���ڸ�
	//		CString rOrigin	   : ���� ���� ���õ� ���� ����Ʈ
	//		CString& rString   : [out] ���� ����Ʈ
	// �Լ� ����          : Ư�� ��ǰ�ڵ��� ���������� ��� �Լ�
	//						���� ���� ���õǾ� �ִ� ������� ��Ʈ���� �����Ѵ�.
	//						�� ���º�� ������ �ٽ� ���� �����ϱ� ���� �߰��� �Լ�
	///////////////////////////////////////////////////////////////////////////////
	int GetKeyString2(CString _szKey, CString rOrigin, CString& rString)
	{
		int nSize = SIZEFULLACCT;	// �� ������ ����.
		int nCount = 0;
		while(rOrigin.GetLength() >= nSize)
		{
			CString szPureAcct = rOrigin.Mid(5, 6);		// ���� ���¹�ȣ�� ����
			rOrigin = rOrigin.Mid(nSize);	// �ѹ� ���� ����(�Ǿհ���)�� ����

			for(POSITION pos = m_List.GetHeadPosition(); pos; )
			{
				CAcct* pAcct = m_List.GetNext(pos);
				if(pAcct)
				{
					if(pAcct->m_acctB.Compare(_szKey)==0 &&
						pAcct->m_acctC.Compare(szPureAcct)==0 )
					{
						nCount++;
						CString szTemp;
						pAcct->GetString(szTemp);
						rString += szTemp;
					}
				}
			}
		}
		
		return nCount;
	}

	int GetAllString(CString& rString)
	{
		int nCount = 0;
		for(POSITION pos = m_List.GetHeadPosition(); pos; nCount++)
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				CString szTemp;
				pAcct->GetString(szTemp);
				rString += szTemp;
			}
		}
		return nCount;
	}

	// GetString2() �Լ��� �Ἥ ��� ���������� ����
	int GetAllString2(CString& rString)
	{
		int nCount = 0;
		for(POSITION pos = m_List.GetHeadPosition(); pos; nCount++)
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				CString szTemp;
				pAcct->GetString2(szTemp);
				rString += szTemp;
			}
		}
		return nCount;	
	}

	CString GetKey(CString szAcct)
	{
		char* aPre = "!NP_Acct";
		CString szKey = szAcct.Mid(3,2);
		szKey = aPre + szKey;
		return szKey;
	}

	// * ���ڷ� �Ѿ�� ����(szAcct)�� _szKey�� �ش��ϴ� �������� �� ó�� �׸����� ������. 
	// * ��Ƽ ��ŸƮ ������ ���� �߰��� ����..
	// * MainGlobalSharedData.cpp ���� ���.. 
	// * _szKey : (!NP_Acctxx - 'xx'�� ��ǰ�ڵ�) ��������..
	// * szAcct : 64 Byte (���¹�ȣ+���¸�+��й�ȣ)
	void MoveToHead(CString _szKey, CString szAcct, CString& rString, BOOL bAddValue)
	{
		CString szExist;			// ���ڷ� �Ѿ�� ���°� ���� ����Ʈ�� �����ϴ� ��� �ӽ� ����
		CString szOriginAcct;		// Trim() ���� ���� ���� ���� ����Ÿ (�ڿ��� ����ϱ� ����)
		int		bExist = FALSE;
		
		szOriginAcct = szAcct;
		szAcct.TrimRight();

		CString szKey = _szKey.Right(2);
		for(POSITION pos = m_List.GetHeadPosition(); pos; )
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				if(pAcct->m_acctB.Compare(szKey)==0)
				{
					CString szTemp;
					pAcct->GetString(szTemp);
					//if(szAcct.Compare(szTemp)==0)	// �� ������ �ֱ� ���� �켱 �����´�.
					if(szTemp.Find(szAcct)>=0)		// ���¸� �Ѿ�� ��츦 ���� Compare��� Find���
					{
						bExist = TRUE;
						szExist = szTemp;
					}
					else				
						rString += szTemp;
				}
			}
		}
		
		// * �Ѱܹ��� ���°� ����Ʈ�� ��� �ִ� ���, 
		// * Ȥ�� bAddValue�� True�� ���(��ũ��Ʈ�� main.NotifyGShareData() ȣ���� ���)���� 
		// * �� ���¸� �籸���� ����Ʈ�� �� �տ� �ٿ��ְ�
		// * �׷��� ���� ��� �籸���� ����Ʈ�� �����Ѵ�. 
		if(bExist || bAddValue)
			rString = szExist + rString;
		// * �Ѱܹ��� ���°� ����Ʈ�� �������� ���� ���, 
		// * ���¿���(64����Ʈ)�� �籸���� ����Ʈ�� �� �տ� �ٿ��ش�. 
		else if(!bExist)
			rString = szOriginAcct + rString;
	}

	// * ���¹�ȣ�� �ش��ϴ� ��й�ȣ�� �Ѱ��ش�. 
	// * szAcct : 11 Byte(���¹�ȣ)
	CString GetAcctPasswd(CString szAcct)
	{
		CString szKey = szAcct.Mid(3,2);
		for(POSITION pos = m_List.GetHeadPosition(); pos; )
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				if(pAcct->m_acctB.Compare(szKey)==0)
				{
					CString szTemp;
					if(pAcct->m_acctC.Compare(szAcct.Right(6))==0)
						return pAcct->m_acctEncPass;
				}
			}
		}
		return "";
	}

	// * ���¹�ȣ�� �ش��ϴ� ��й�ȣ�� �����Ѵ�. 
	// * szAcct : 11 Byte(���¹�ȣ)
	// * szPasswd : szAcct�� �ش��ϴ� ��й�ȣ 
	void SetAcctPasswd(CString szAcct, CString szPasswd)
	{
		for(POSITION pos = m_List.GetHeadPosition(); pos; )
		{
			CAcct* pAcct = m_List.GetNext(pos);
			if(pAcct)
			{
				CString szTemp;
				pAcct->GetPureAcct(szTemp);
				if(!szTemp.Compare(szAcct))
				{
					pAcct->m_acctEncPass = szPasswd;
					return;
				}
			}
		}		
	}

	void Copy(CAcctHelper* pOrig)
	{
		// ����Ʈ Clear
		for(POSITION pos = m_List.GetHeadPosition(); pos; )
		{
			delete m_List.GetNext(pos);			
		}
		m_List.RemoveAll();

		// Copy.. 
		for(pos = pOrig->m_List.GetHeadPosition(); pos; )
		{
			CAcct* pOld = pOrig->m_List.GetNext(pos);
			if(pOld)
			{
				CString szAcct;
				szAcct.Format("%3.3s%2.2s%6.6s%-40.40s%-13.13s%-1.1s%1.1d", pOld->m_acctA, 
																	pOld->m_acctB, 
																	pOld->m_acctC, 
																	pOld->m_acctName,
																	pOld->m_acctEncPass,
																	pOld->m_acctFlag,
																	pOld->m_nSaveMode);
				CAcct* pAcct = new CAcct(szAcct);
				m_List.AddTail(pAcct);
			}
		}
	}

	CList<CAcct*, CAcct*> m_List;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNT_H__F81234A1_D6B6_11D4_B31B_0048543A0D2E__INCLUDED_)
