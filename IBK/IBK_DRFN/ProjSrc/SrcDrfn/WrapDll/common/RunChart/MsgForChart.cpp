#include "StdAfx.h"
#include ".\msgforchart.h"


CMsgForChart::CMsgForChart()
: m_LengthOfSubMsg(0)
, m_strData(_T(""))
, m_strMsg(_T(""))
, m_strPropChart(_T(""))
, m_strSubMsg(_T(""))
, m_pItems(NULL)
, m_strExtMsg(_T(""))
, m_nMultiGID(0)
, m_strGIDs(_T(""))
{
	m_pItems = NULL;
	m_pItems = new CObList;
	
}

CMsgForChart::~CMsgForChart(void)
{
	if(m_pItems)
		delete m_pItems;
}

////////////////////////////////////////////////
// OCX �� �ܴ��� ������ �޽��� ����
void CMsgForChart::GenerateMsg(void)
{	
	// Header ����: ������ ���� + �����޽��� ���� + ���� �޽���
	m_strMsg = m_strCount + m_strLenOfSubMsg + m_strSubMsg;
	// Data body add
	m_strMsg += m_strData;
}

/////////////////////////////////////////////
// ���� �޽��� �����
void CMsgForChart::GenerateSubMsg(void)
{
	//////////////////////////////////////////////////////////////////////////
	// 2006�� 6�� 25��
	// ���� �޽��� ����
	int iToKenStart = 0;
	int iStart = m_strSubMsg.Find(TokenizeEx(m_strExtMsg,"=",iToKenStart), 0);
	int iEnd;
	if( iStart == -1)
		m_strSubMsg += m_strExtMsg;
	else
	{
		iEnd = m_strSubMsg.Find("@",iStart);
		CString strOld = m_strSubMsg.Mid(iStart, iEnd-iStart+1);
		m_strSubMsg.Replace(strOld, m_strExtMsg);
	}
// 	m_strSubMsg = m_strUM + m_strUD + m_strUT + m_strRDATET + m_strPREVPRICE + m_strFUNDSTARTPOINT
// 				+ m_strFUNDENDPOINT + m_strUSEPACKET + m_strBOUND + m_strRESETUSEREAL + m_strRESETPACKET
// 				+ m_strERROR + m_strPRICETYPE + m_strNEWPACKET + m_strDRAWSTARTINDEX + m_strCODEANDNAME
// 				+ m_strDOAPPENDBYREAL + m_strDATE + m_strMARKETTIME + m_strFRDA + m_strUTEC + m_strCB 
// 				+ m_strRDC + m_strBOUNDMARK + m_strExtMsg;


	// ���� �޽��� ���̸� ���ϰ� ��Ʈ������ ��ȯ
	NumToString(m_strSubMsg.GetLength(), LENGTH_MSG_SUBMSG_COUNT, m_strLenOfSubMsg);
}

void CMsgForChart::AppendData(char* szFormat/*IN*/, ...)
{
	va_list vl;
	va_start( vl, szFormat );
//	FormatData();

//	CString tmp;
//	tmp.Format(szFormat,char*);
	//m_strData. += strAddingData;
}

//////////////////////////////////////////////////////////////////////////////
// map���κ��� ��ȿ�� ������ ���� get & ������ ���Ͽ� CList�� m_pItems�� ���
// 
CString CMsgForChart::GenerateDataFormat()
{

	CString strFormat, ss;
	int cur=0, prev=0;
	CItemInChartEu* pItem;
	while(1)
	{
		//////////////////////////////////////////////////////////////////////////
		// �� �� �� �и�, �Ӽ� �׸񺰷� �и� �۾�
		cur = m_strPropChart.Find(0x0d, prev);
		if(cur == -1)
			break;
		ss = m_strPropChart.Mid(prev, cur-prev);
		prev = cur+2;
		//////////////////////////////////////////////////////////////////////////

		if(!IsValidData(ss)) continue;	// ��ȿ�� ���� �ƴϸ� skip

		// OCX packet���� ���� �и�
		pItem = new CItemInChartEu;
		pItem->m_bValid = true;
		pItem->m_strName = GetItemName(ss);
		pItem->m_strField = GetFieldName(ss);
		pItem->m_strType = GetTypeOfItem(ss);
		pItem->m_lLength = GetLengthOfItem(ss);
		pItem->m_lLengthBackup = pItem->m_lLength;
		pItem->m_bReal = (GetRealFlag(ss)== "��")?TRUE:FALSE;

		// �Ｚ : TR�� ��� �����۸�� FID�� �и��Ѵ�. ���) �����۸�, �����۸�(GID:FID), ���ڵ��:�����۸�(GID:FID)
		//@�������: ��ȣ'()'�� �и��Ǵ� �κ��� �����Ѵ�. ��ȸ�� FID�� ����ó�������� �и��Ѵ�.
		int iStart = 0;
		{
			// ������ �Ｚ�� GID �� ���������� ������ Ư�� TR���� �����ϹǷ� GID�� ���ٰ� ���� �Ұ����� ������.
			// () �� ���� ��쿡�� ������Ŷ���� �и��Ѵ�.
			CString strField = TokenizeEx(pItem->m_strField, "(", iStart);
			m_nMultiGID = 1;
			if(strField == "")	// �������� ���� ���
			{
				pItem->m_nFID = atoi(pItem->m_strField);
			}
			else
			{
				pItem->m_nFID = atoi(strField);
				strField= pItem->m_strField.Mid(iStart);
				strField.TrimRight(")");
				//pItem->m_nRealID = atoi(strField);

				//@Champ-union090827-alzioyes.
				if(strField[0]>='0' && strField[0]<='9')
					pItem->m_nRealID = atoi(strField);
				else
					pItem->m_szRealID = strField;
			}
		}
		
		m_pItems->AddTail(pItem);
	}

	return _T("");
}

// ������ ��ƾ GenerateDataFormat
/*
CString CMsgForChart::GenerateDataFormat()
{

	CString strFormat, ss;
	int cur=0, prev=0;
	CItemInChartEu* pItem;
	while(1)
	{
		//////////////////////////////////////////////////////////////////////////
		// �� �� �� �и�, �Ӽ� �׸񺰷� �и� �۾�
		cur = m_strPropChart.Find(0x0d, prev);
		if(cur == -1)
			break;
		ss = m_strPropChart.Mid(prev, cur-prev);
		prev = cur+2;
		//////////////////////////////////////////////////////////////////////////

		if(!IsValidData(ss)) continue;	// ��ȿ�� ���� �ƴϸ� skip

		// OCX packet���� ���� �и�
		pItem = new CItemInChartEu;
		pItem->m_bValid = true;
		pItem->m_strName = GetItemName(ss);
		pItem->m_strField = GetFieldName(ss);
		pItem->m_strType = GetTypeOfItem(ss);
		pItem->m_lLength = GetLengthOfItem(ss);
		pItem->m_lLengthBackup = pItem->m_lLength;
		pItem->m_bReal = (GetRealFlag(ss)== "��")?TRUE:FALSE;


		// �Ｚ : TR�� ��� �����۸�� FID�� �и��Ѵ�. ���) �����۸�, �����۸�(GID:FID), ���ڵ��:�����۸�(GID:FID)
		//@�������: ��ȣ'()'�� �и��Ǵ� �κ��� �����Ѵ�. ��ȸ�� FID�� ����ó�������� �и��Ѵ�.
		int iStart = 0;
		if(pItem->m_strField.Find("(",0) != -1 || atol(pItem->m_strField) == 0)	// TR ���
		{
			CString strField = pItem->m_strField;
			pItem->m_strField = TokenizeEx(strField, "(",iStart);
			if(pItem->m_strField == "")
			{
				pItem->m_strField = strField;
			}
			else
			{
				// �����۸��� ���ڵ�� �и�
				int iStart2 = 0;
				CString strRecord = TokenizeEx(pItem->m_strField,":",iStart2);
				if(strRecord != "") // record name�� �ִٸ�...
				{
					pItem->m_strRecord = strRecord;
					pItem->m_strField = pItem->m_strField.Right(pItem->m_strField.GetLength() - iStart2);
				}
				// GIF, FID �и�
				CString strGID = TokenizeEx(strField, ":", iStart);
				if(strGID == "")	// GID �� ���ٸ�...
					pItem->m_nFID = atoi(TokenizeEx(strField, ")", iStart));
				else
				{
					pItem->m_nGID = atoi(strGID);
					pItem->m_nFID = atoi(TokenizeEx(strField, ")", iStart));
				}
			}
		}
		else	//FID ���, ���) FID, GID:FID, GID:FID:INDEX
		{
			CString strGID = TokenizeEx(pItem->m_strField, ":", iStart);
			if(strGID == "")	// GID�� ���� ���
			{
				m_nMultiGID = 1;
				pItem->m_nFID = atoi(pItem->m_strField);
			}
			else				// GID�� �ִ� ���
			{

				if(m_strGIDs.Find(strGID) == -1)	// �������� ������(new�� ���)....�߰�
				{
					m_strGIDs += strGID + ";";
					m_nMultiGID++;
				}
				pItem->m_nGID = atoi(strGID);
				CString strFID = TokenizeEx(pItem->m_strField, ":", iStart);
				if(strFID == "")	// INDEX�� ���� ���
				{
					pItem->m_nFID = atoi(pItem->m_strField.Right(pItem->m_strField.GetLength()-iStart));
				}
				else				// INDEX�� �ִ� ���
				{
					pItem->m_nFID = atoi(strFID);
					pItem->m_nIndex = atoi(pItem->m_strField.Right(pItem->m_strField.GetLength()-iStart));
				}
			}
		}
		
		m_pItems->AddTail(pItem);
	}

	return _T("");
}
*/

//////////////////////////////////////////////////
// �������� ��/�� get
bool CMsgForChart::IsValidData(CString strItem)
{
	CString res;
	int cur = 0;
	res = TokenizeEx(strItem, ";", cur);	
	return (res == "1") ? true : false;
}

///////////////////////////////////////////////////
// ��Ʈ���� ǥ���ϰ��� �ϴ� �������� �̸� get
CString CMsgForChart::GetItemName(CString strItem)
{
	CString res;
	int cur = 0;
	TokenizeEx(strItem, ";", cur);
	res = TokenizeEx(strItem, ";", cur);

	return res;
}

//////////////////////////////////////////////////
// ��Ʈ���� ǥ���ϰ��� �ϴ� �������� �ʵ�� get
CString CMsgForChart::GetFieldName(CString strItem)
{
	CString res;
	int cur = 0;
	TokenizeEx(strItem, ";", cur);
	TokenizeEx(strItem, ";", cur);
	res = TokenizeEx(strItem, ";", cur);

	return res;
}

///////////////////////////////////////////
// ��Ʈ���� ǥ���ϰ��� �ϴ� �������� ���� get
long CMsgForChart::GetLengthOfItem(CString strItem)
{
	CString res;
	int cur = 0;
	TokenizeEx(strItem, ";", cur);	
	TokenizeEx(strItem, ";", cur);
	TokenizeEx(strItem, ";", cur);
	res = TokenizeEx(strItem, ";", cur);	

	return atol((LPSTR)(LPCTSTR)res);
}

//////////////////////////////////////////////////
// ��Ʈ���� ǥ���ϰ��� �ϴ� �������� ���� get
CString CMsgForChart::GetTypeOfItem(CString strItem)
{
	CString res;
	int cur = 0;
	TokenizeEx(strItem, ";", cur);	
	TokenizeEx(strItem, ";", cur);	
	TokenizeEx(strItem, ";", cur);
	TokenizeEx(strItem, ";", cur);
	res = TokenizeEx(strItem, ";", cur);
	return res;
}

CString CMsgForChart::GetRealFlag(CString strItem)
{
	CString res;
	int cur = 0;
	TokenizeEx(strItem, ";", cur);	
	TokenizeEx(strItem, ";", cur);	
	TokenizeEx(strItem, ";", cur);
	TokenizeEx(strItem, ";", cur);
	TokenizeEx(strItem, ";", cur);
	res = TokenizeEx(strItem, ";", cur);
	return res;
}

//////////////////////////////////////////////////////////////////////////
// CString�� tokenize�� ��ü
// CString�� tokenize�� ������� ���� ����
// �ʵ���� �Է����� �ʾ��� ��� ""�� �ν����� ���ϱ� ����
// �ڷ�����;D1��ȸ��;8;YYYYMMDD;��; <-- OK
// �ڷ�����;;8;YYYYMMDD;��;			<-- not OK, �̷��� ��츦 ����ϱ� ���ؼ�
CString CMsgForChart::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
{
	CString strRes = _T("");
	int iCur = iStart;
	iCur = strSrc.Find(pszTokens, iStart);
	if(iCur != -1)	// tokenize �� ���ڰ� ���� ��쿡�� "" ����
	{
		strRes = strSrc.Mid(iStart, iCur-iStart);
		iCur++;
		iStart = iCur;
	}
	return strRes;
}
//////////////////////////////////////////////////////////////////////////
// OCX property���� ������ packet�� DSItem�� �����ϱ� ����
// ��Ʈ ��Ʈ���� property �� "DSItem����" �׸�� ���õ�
// void CMsgForChart::mappingDSItem(CString strDSItems, CDSArray* pDSItems)
// {
// 	CString strRes;
// 	int cur = 0;
// 	POSITION pos = m_pItems->GetHeadPosition();
// 	while(pos)
// 	{
// 		CItemInChartEu* pItem = (CItemInChartEu*)m_pItems->GetNext(pos);
// 		cur = strDSItems.Find(pItem->m_strName);
// 		if(cur == -1) continue;
// 		strDSItems.Tokenize(":", cur);
// 		strRes = strDSItems.Tokenize(";", cur);
// 		//pItem->m_strField = strRes;
// 		pItem->m_pDSItem = pDSItems->GetDSItem(strRes);
// 		if(!pItem->m_pDSItem) continue;
// 		pItem->m_strFormat = GenerateStringFormat(pItem->m_pDSItem->m_wDataType, pItem->m_lLength); // �Է� ������ ����
// 		pItem->m_wType = pItem->m_pDSItem->m_wDataType;		
// 	}
// }
////////////////////////////////////////////////
// ocx map ���� ������ ��ȿ�� ������ ����
void CMsgForChart::RemoveAll(void)
{
	POSITION pos = m_pItems->GetHeadPosition();
	while(pos)
	{
		CItemInChartEu * pItem = (CItemInChartEu*)m_pItems->GetNext(pos);
		delete pItem;
		pItem = NULL;
	}
	m_pItems->RemoveAll();
}

/////////////////////////////////////////////////////////////////////
// OCX�� ������ ������ ������ ���߱� ���� 
// data �� ������ DSItem�� �� ��������
// data�� ���̴� ocx������ ������ map���� �����´�.
// ��ȯ��: ���� ��Ʈ�� ��) %10ld
//
CString CMsgForChart::GenerateStringFormat(WORD wDataType, long lLen)
{
	CString ret;

	static const char * pszTypeString[] =
	{"", "s", "c", "hd", "hu", "ld", "lu", "f", "lf", "I64d"	};

	ret.Format("%%%ld%s",lLen, pszTypeString[wDataType]);

	return ret;
}

// ���� �޽��� �׸� SET - �Է� value
void CMsgForChart::SetSubMsg(CString SubMsgItem, CString value)
{
	if(SubMsgItem == _T("RESETPACKET") || SubMsgItem == _T("NEWPACKET"))
	{
		CItemInChartEu* pItem = NULL;
		int iStart = 0;
		//value += "|";
		CString strPacketName = TokenizeEx(value,":", iStart);	// get packet name 
		POSITION pos = m_pItems->GetHeadPosition();
		while (pos)
		{
			pItem = (CItemInChartEu*)m_pItems->GetNext(pos);
			//if(pItem->m_strName == strPacketName)
			{
				//pItem->m_lLengthBackup = pItem->m_lLength;
				pItem->m_lLength = atol(TokenizeEx(value,":", iStart));	
				pItem->m_strType = TokenizeEx(value,"|", iStart);

			//	break;
			}
			strPacketName = TokenizeEx(value,":", iStart);	// get packet name 

		}
		//value.Delete(value.GetLength()-1, 1);
	}
	else if(SubMsgItem == _T("RDATET"))
	{
		CItemInChartEu* pItem;
		POSITION pos = m_pItems->GetHeadPosition();
		while (pos)
		{
			pItem = (CItemInChartEu*)m_pItems->GetNext(pos);
			if(pItem->m_strName == "�ڷ�����")
				break;
		}

		pItem->m_strType = value;
		pItem->m_lLength = value.GetLength();
	}
	
	m_strExtMsg = SubMsgItem + "=" + value + "@";	GenerateSubMsg();
}
