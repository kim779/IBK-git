// MainChartFormulate.h: interface for the CMainChartFormulate class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_MAINCHARTFORMULATE_H__40507629_BC7A_441B_9E2D_C576F9CC32B1__INCLUDED_)
#define AFX_MAINCHARTFORMULATE_H__40507629_BC7A_441B_9E2D_C576F9CC32B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CIndicatorList;
class CPacketList;
class CMainChartFormulateImplementation;
class CChartFormulateData;
class CGraphCalculateData;
class CIndicatorInfo;

class CPacket;
class AFX_EXT_CLASS CMainChartFormulate  
{
public:
	CMainChartFormulate();
	virtual ~CMainChartFormulate();

	//�ܺο� ����� �Լ����� ȣ��.df
	//���� Data�� Packet�� �����ϰ�, ������ packetName�� ��´�.
	bool CalculateIndicator(const CChartFormulateData& chartFormulateData,
							CGraphCalculateData* graphCalculateData, 
							CIndicatorInfo*& pIndicatorInfo);
	//���� ����.
	void GetGraphData(const CString& strIndicatorName,
					  CList<double, double>& dConditionList,
					  CList<CString, CString>& dPacketNameList,
					  CPacketList* pPacketList,
					  CIndicatorList* pIndicatorList,
					  CList<CPacket*, CPacket*>* pPacketsList);

	//���� ����.
	void GetIndicatorData(const CString& strFuncName, const CString& strSubIndex, 
					  CList<double, double>& dConditionList, 
					  CList<CString, CString>& PacketNameList, 
					  CPacketList* pPacketList, 
					  CIndicatorList* pIndicatorList, 
					  CList<CPacket*, CPacket*>* pPacketsList);

	//Signal�� ��´�.
	void GetSignalData(CPacketList* pPacketList, const CString& strFunctionName, CList<double, double>*& pSignalData);

	// packet �ּҵ��� �ʱ�ȭ(NULL)
	void Initialize_AllPackets();
	void Initialize_Packet(const CString& strIndicatorName);

	void DeleteDataInPacket(CPacketList* pPacketList, const CString& strIndicatorName);

	void DeleteDataInPacket(const CChartFormulateData& chartFormulateData,
					CGraphCalculateData* graphCalculateData, 
					CIndicatorInfo* pIndicatorInfo);

	void GetAllGraphPackets(const CChartFormulateData& chartFormulateData,
											CGraphCalculateData* graphCalculateData,
											CIndicatorInfo*& pIndicatorInfo);

private:
	CMainChartFormulateImplementation* m_pMainImplementation;
};

#endif // !defined(AFX_MAINCHARTFORMULATE_H__40507629_BC7A_441B_9E2D_C576F9CC32B1__INCLUDED_)
