#pragma once

//#include <afxtempl.h>
#include "ItemInChart.h"


#define LENGTH_MSG_COUNT	5
#define LENGTH_MSG_SUBMSG_COUNT	4

typedef struct _tagPACKMAP
{
	char* szPacket;
	long lFID;
	BOOL bSub;
}PacketMap;

typedef struct _tagPACKSUBMAP
{
	char* szType;
	long lFID;
}PackSubMap;


static const char * pszTypeSubMsg[] =
{
  "UM"				// 0
, "UD"
, "UT"
, "RDATET"
, "PREVPRICE"
, "FUNDSTARTPOINT"
, "FUNDENDPOINT"
, "USEPACKET"
, "BOUND"
, "RESETUSEREAL"	
, "RESETPACKET"		//10
, "ERROR"
, "PRICETYPE"
, "NEWPACKET"
, "DRAWSTARTINDEX"
, "CODEANDNAME"
, "DOAPPENDBYREAL"
, "DATE"
, "MARKETTIME"
, "FRDA"
, "UTEC"			// 20
, "CB"
, "RDC"
, "BOUNDMARK"
};

class CMsgForChart
{
public:
	CMsgForChart();
	~CMsgForChart(void);
	UINT m_LengthOfSubMsg;	// ���� �޽����� ����
	CString m_strLenOfSubMsg;	// ���� �޽����� ����
	CString m_strData;	//������ (�ٵ�)
	CString m_strSubMsg;	// ���� �޽��� ��Ʈ��
	// ���� �޽��� �׸�
// 	CString m_strUM;				// 1
// 	CString m_strUD;
// 	CString m_strUT;
// 	CString m_strRDATET;
// 	CString m_strPREVPRICE;
// 	CString m_strFUNDSTARTPOINT;
// 	CString m_strFUNDENDPOINT;
// 	CString m_strUSEPACKET;
// 	CString m_strBOUND;
// 	CString m_strRESETUSEREAL;		// 10
// 	CString m_strRESETPACKET;
// 	CString m_strERROR;
// 	CString m_strPRICETYPE;
// 	CString m_strNEWPACKET;
// 	CString m_strDRAWSTARTINDEX;
// 	CString m_strCODEANDNAME;
// 	CString m_strDOAPPENDBYREAL;
// 	CString m_strDATE;
// 	CString m_strMARKETTIME;
// 	CString m_strFRDA;				// 20
// 	CString m_strUTEC;
// 	CString m_strCB;
// 	CString m_strRDC;
// 	CString m_strBOUNDMARK;
	CString m_strExtMsg;

public:
	int m_nMultiGID;
	CString m_strGIDs;		// GID list, ������ ;
	CString m_strMsg;		// OCX�� ������ ����: �����Ͱ���+�����޽��� ����+�����޽���+������
	CString m_strCount;		// ������ ����
	CString m_strPropChart;	// ��Ʈ�� ������Ƽ �� packet����
	CObList* m_pItems;		// �ش� DSItem pointer

	//////////////////////////////////////////////////////////////////////////
	//	���ڸ� ��Ʈ������
	//	23 --> 00023 ���·� ����
	void NumToString(long lNum/*IN, ����*/, UINT ulLen/*IN, �ڸ���*/, CString& strNum/*OUT, ��Ʈ��*/)
	{
		CString strFormat;
		strFormat.Format("%%0%dld",ulLen);
		strNum.Format(strFormat,lNum);
	}
	//////////////////////////////////////////////////////////////////////////
	long StringToNum(UINT uLen/*IN*/, CString strNum/*IN*/){return atol(strNum);}	// ��Ʈ���� �ѹ���
	void ClearData(void){m_strData.Empty();	}							// �޽��� ��Ʈ���� �ʱ�ȭ ""
	void SetCount(long lNum){NumToString(lNum, LENGTH_MSG_COUNT, m_strCount);}	// ������ ���� set
	long GetCount(void){return StringToNum(LENGTH_MSG_COUNT, m_strCount);}		// ������ ���� get

	void AppendData(char* szFormat/*IN*/, ...);				// N/A
	void GenerateMsg(void);									// ����(�޽���) ����
	void GenerateSubMsg(void);								// ���� �޽��� ����

	CString GenerateDataFormat();							// OCX�� packet ���� ����ü�� ����
	//CString GenerateDataFormat(CDSArray* pDSItems);
	bool IsValidData(CString strItem/*IN*/);				// packet�� ��ȿ�� �� �Ǵ� (OCX������ checkbox)
	CString GetItemName(CString strItem/*IN*/);				// packet �� ��������
	CString GetFieldName(CString strItem/*IN*/);			// �ʵ� �� ��������
	long GetLengthOfItem(CString strItem/*IN*/);			// packet�� ����(�ڸ���)
	CString GetTypeOfItem(CString strItem/*IN*/);			// packet�� ����
	CString GetRealFlag(CString strItem/*IN*/);				// packet�� �ǽð� ����
	void RemoveAll(void);							 
	CString GenerateStringFormat(WORD wDataType/*IN, from DSItem*/, long lLen/*IN, from packet*/);
	void AddData(CString strData/*IN*/){m_strData += strData;}										//������ �߰� 
//	void mappingDSItem(CString strDSItems/*IN*/, CDSArray* pDSItems/*IN*/);							//packet�� DSItem����
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN,OUT*/);	// ��Ʈ�� tokenizing

	// ���� �޽��� �׸� SET
	void SetSubMsg(CString SubMsgItem, CString value);
// 	void SetUM(CString in);
// 	void SetUD(CString in);
// 	void SetUT(CString in);
// 	void SetRDATET(CString in);
// 	void SetPREVPRICE(CString in);
// 	void SetFUNDSTARTPOINT(CString in);
// 	void SetFUNDENDPOINT(CString in);
// 	void SetUSEPACKET(CString in);
// 	void SetBOUND(CString in);
// 	void SetRESETUSEREAL(CString in);
// 	void SetRESETPACKET(CString in);
// 	void SetERROR(CString in);
// 	void SetPRICETYPE(CString in);
// 	void SetNEWPACKET(CString in);
// 	void SetDRAWSTARTINDEX(CString in);
// 	void SetCODEANDNAME(CString in);
// 	void SetDOAPPENDBYREAL(CString in);
// 	void SetDATE(CString in);
// 	void SetMARKETTIME(CString in);
// 	void SetFRDA(CString in);
// 	void SetUTEC(CString in);
// 	void SetCB(CString in);
// 	void SetRDC(CString in);
// 	void SetBOUNDMARK(CString in);
// 	// ���� �޽��� GET
// 	CString GetUM(void);
// 	CString GetUD(void);
// 	CString GetUT(void);
// 	CString GetRDATET(void);
// 	CString GetPREVPRICE(void);
// 	CString GetFUNDSTARTPOINT(void);
// 	CString GetFUNDENDPOINT(void);
// 	CString GetUSEPACKET(void);
// 	CString GetBOUND(void);
// 	CString GetRESETUSEREAL(void);
// 	CString GetRESETPACKET(void);
// 	CString GetERROR(void);
// 	CString GetPRICETYPE(void);
// 	CString GetNEWPACKET(void);
// 	CString GetDRAWSTARTINDEX(void);
// 	CString GetCODEANDNAME(void);
// 	CString GetDOAPPENDBYREAL(void);
// 	CString SetDATE(void);
// 	CString GetMARKETTIME(void);
// 	CString GetFRDA(void);
// 	CString GetUTEC(void);
// 	CString GetCB(void);
// 	CString GetRDC(void);
// 	CString GetBOUNDMARK(void);
};

