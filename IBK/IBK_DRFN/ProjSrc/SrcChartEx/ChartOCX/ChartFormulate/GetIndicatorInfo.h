// GetIndicatorInfo.h: interface for the CGetIndicatorInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETINDICATORINFO_H__8AE160FC_10B9_4EEA_A5DB_53F7D66550DE__INCLUDED_)
#define AFX_GETINDICATORINFO_H__8AE160FC_10B9_4EEA_A5DB_53F7D66550DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "CalculateData.h"

class CChartFormulateData;
class CIndicatorInfo;
class CIndicatorList;
class CGetIndicatorInfo  
{
public:
//	static void GetIndicatorInfoFromIndicatorConditionDLL(const CString& strPacketName, 
//														  const CString& strFuncName, 
//														  CIndicatorList* pIndicatorList, 
//														  CList<double, double>& ConditionList, 
//														  CList<CString, CString>& PacketNameList, 
//														  CList<CString, CString>& SubGraphNameList);

	//�Ÿŷ���Ʈ, �ý���Ʈ���̴����� SUbGraphName�� �������� �ϴ� ���� ����.
//	static void GetSubGraphNameList(const CString& strGraphName, CIndicatorList* pIndicatorList, CList<CString, CString>& subGraphNameList);
	static int GetSubGraphIndex(CIndicatorInfo* pIndicatorinfo, const CString& subgraphname);


private:
	//strOriginal�� �Ǹ����� �κп� strCompare�� �ִ��� üũ�ϰ�, ������ strOriginal�� strCompare�� Add.	
	static void AddCompareStringInOriginalString(CString& strOriginal, const CString& strCompare);
	static void GetSubGraphNameList(CIndicatorInfo* pIndicatorInfo, CList<CString, CString>& subGraphNameList);
	
	static void ChangeTypeFromCStringToCList(const CString& strPacketName, CList<CString, CString>& PacketNameList);
	static void GetConditionList(CIndicatorInfo* pIndicatorInfo, CList<double, double>& ConditionList);
	static void GetPacketNameList(CIndicatorInfo* pIndicatorInfo, CList<CString, CString>& PacketNameList);


};

#endif // !defined(AFX_GETINDICATORINFO_H__8AE160FC_10B9_4EEA_A5DB_53F7D66550DE__INCLUDED_)
