#if !defined(AFX__REALUPDATE__INCLUDED_)
#define AFX__REALUPDATE__INCLUDED_
// ��Ʈ ���̺� ���� 
#include <vector>
#include <algorithm>
// FIDLIST�� ���� ���̺��� �̿��Ѵ�.
#if !defined(_WIN32_WCE)
	typedef std::vector<char*> CHARVECTOR;
	typedef std::vector<std::vector<char*> > CHARVECTOR2;
	typedef CHARVECTOR::iterator CHARVECTORIT;
	typedef CHARVECTOR2::iterator CHARVECTOR2IT;
#else
	typedef std::vector<TCHAR*> CHARVECTOR;
	typedef std::vector<std::vector<TCHAR*> > CHARVECTOR2;
	typedef CHARVECTOR::iterator CHARVECTORIT;
	typedef CHARVECTOR2::iterator CHARVECTOR2IT;
#endif

// ȭ�鿡�� �ǽð�������(PB)ó���� �ϵ����ϴ� �޼���. 
//#define WM_GET_BROD				WM_USER + 605		//@���� 601 -> 605 : WM_USER + 601

#define	STOCK_SISE_PID			21	//50		// �ֽ� �ǽð� �ü� ������
//#define	STOCK_HOGA_PID			3	//51		// �ֽ� �ǽð� ����ȣ�� ������
#define	UPJONG_SISE_PID			101				// ���� �ǽð� �ü� ������
#define	FUTURE_SISE_PID			65	//65		// ���� �ǽð� �ü� ������
#define	OPTION_SISE_PID			66	//67		// �ɼ� �ǽð� �ü� ������
#define	STAR_SISE_PID			67				// ��Ÿ �ǽð� �ü� ������
#define FOREIGN_SISE_PID		109
#define	ELW_SISE_PID			138				// ELW �ǽð� �ü� ������
#define FX_SISE_PID				140				// FX �ǽð� �ü� ������

#define DI_CODE				16013	//�ڵ�
#define DI_NAME				16002
#define DI_TIME				15019	//ü��ð�
#define DI_PRICE			15001	//���簡
#define DI_CHANGE			15472	//���
#define DI_SIGN				15006	//�����ȣ
#define DI_CHGRATE			15004	//�����
#define DI_HIGH				15010	//��
#define DI_LOW				15011	//����
#define DI_TRDVOL			15020	//ü�ᷮ
#define DI_VOLUME			15015	//�����ŷ���
#define DI_AMOUNT			15023	//�����ŷ����
#define DI_OPENPRICE		15009	//�ð�
#define DI_OFFER_HO1		14501	//�ŵ�ȣ��1
#define DI_OFFER_REM1		14511	//�ŵ�ȣ���ܷ�1
#define DI_BID_HO1			14531	//�ż�ȣ��1
#define DI_BID_REM1			14541	//�ż�ȣ���ܷ�1
#define DI_OFFER_TOT5		14621	//�ŵ�5�ܰ�ȣ���ܷ���
#define DI_BID_TOT5			14623	//�ż�5�ܰ�ȣ���ܷ���
#define DI_OFFER_TOTREM		14561	//�ŵ�ȣ���ܷ���
#define DI_BID_TOTREM		14563	//�ż�ȣ���ܷ���
#define DI_OFFER_OUT_TOT	14575	//�ð��ܸŵ�ȣ���ܷ�
#define DI_BID_OUT_TOT		14577	//�ð��ܸż�ȣ��


class CRealUpdateData
{
public:
	CRealUpdateData()
	{
		m_pFidList = NULL;
		m_pFidPtrValue = NULL;
		m_pFidTypeList = NULL;
	}
	~CRealUpdateData()
	{
	}

	BOOL GetDataString(WORD wID, CString &strData)
	{
		if(!m_pFidList)
			return FALSE;

		void* pData;
		WORD nType;
		for(int i=0; i<m_pFidList->GetSize(); i++)
		{
			if(wID == m_pFidList->GetAt(i))
			{
				pData = m_pFidPtrValue->GetAt(i);
				nType = m_pFidTypeList->GetAt(i);
				strData = ConvertToString(nType, (char *)pData);
				return TRUE;
			}
		}
		return FALSE;
	}

	CString ConvertToString(WORD wDataType, char *pBuf, int nLen=0)
	{
		CString strData="";

		int iIndex = 0;
		double fData;
		int nTemp;
		switch(wDataType)
		{
		case 1:
			if(nLen > 0)
				strData.Format("%*.*s", nLen, nLen, pBuf);
			else
				strData.Format("%s", pBuf);
			break;
		case 2:
			strData.Format("%d", pBuf[0]);
			break;
		case 3:
		case 4:
			if(nLen > 0)
				strData.Format("%*d", nLen, *(short *)pBuf);
			else
				strData.Format("%d", *(short *)pBuf);
			break;
		case 5:
			if(nLen > 0)
				strData.Format("%*d", nLen, *(long *)pBuf);
			else
				strData.Format("%d", *(long *)pBuf);
			break;
		case 6:
			if(nLen > 0)
				strData.Format("%*u", nLen, *(ULONG *)pBuf);
			else
				strData.Format("%u", *(ULONG *)pBuf);
			break;
		case 7:
			if(nLen > 0)
				strData.Format("%*u", nLen, *(ULONG *)pBuf);
			else
				strData.Format("%u", *(ULONG *)pBuf);
			nTemp = atoi(strData);
			fData = nTemp/100.;
			strData.Format("%*.2f", nLen, fData);
			break;
			//daetoo
		case 8:
		case 9:
			if(nLen > 0)
				strData.Format("%*ld", nLen, *(long *)pBuf);
			else
				strData.Format("%ld", *(long *)pBuf);
			break;
		default:
			break;
		}
		return strData;
	}

	CWordArray*		m_pFidList;				///> FID List
	CWordArray*		m_pFidTypeList;				///> FID Type List
	CPtrArray*		m_pFidPtrValue;				///> Value�� Pointer��ġ
	WORD			m_nMainKeyFid;
	CString			m_strMainKeyVal;
	void			SetMainKey(WORD nFid, CString strVal){m_nMainKeyFid = nFid; m_strMainKeyVal = strVal; }
};

// ȭ�鿡�� �ǽð�������(PB)ó���� �ϵ����ϴ� �޼���. 
// #define WM_GET_BROD				WM_USER + 601

#endif // AFX__REALUPDATE__INCLUDED_
