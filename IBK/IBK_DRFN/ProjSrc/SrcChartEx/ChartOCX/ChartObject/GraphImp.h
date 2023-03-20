// GraphImp.h: interface for the CGraphImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHIMP_H__6C371F97_C2BC_4CBA_8AB9_E44AD1E3B2A0__INCLUDED_)
#define AFX_GRAPHIMP_H__6C371F97_C2BC_4CBA_8AB9_E44AD1E3B2A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/Graph.h"
#include "../Include_Chart/GraphData.h"

#include "SubGraphList.h"
#include "ChartObjectBaseData.h"

class CMainChartFormulate;
class CBlockImp;
class CVertScaleGroupList;
class CVertScaleGroup;
class CChartFormulateData;
class CIndicator;
class CPacketRQ;

class CGraphImp : public CGraph
{
public:
	// graph�� ����& ����
	static CGraphImp* MakeGraph(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData);
	// RQ�� ������ �ִ� Graph ���� : �������� - ojtaso (20061224)
	// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
	static CGraphImp* MakeGraph(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData);
	static void Delete(CGraphImp* pGraph);

protected:
	CGraphImp(CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData);
	// RQ�� ������ �ִ� Graph ���� : �������� - ojtaso (20061224)
	// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
	CGraphImp(LPVOID pPacketRQ, CMainBlockBaseData* pMainBlockBaseData, CVertScaleGroup* pVertScaleGroup, const CGraphData& graphData, char *p_szSubGraphData);
	~CGraphImp();

public:
	void DeleteGraph();
	void DeletePacket(const bool bIsOnlyNoTRData = true);

	// ��ǥ�� ����Ѵ�.
	bool CalculateGraph(const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsCalMinMax = true, const bool bIsClearPacketData = false, const bool bIsRequestTR = false);
	
	// �ʱ�ȭ ���� ����.
	void Initialize_MakeGraph();
//	void Initialize_PacketAndIndicatorInfo(const bool bIsCalculate);
	void Initialize_AllPackets(const bool bIsCalculate);
	void Initialize_IndicatorInfo(const bool bIsNULL);

	// �ش� graph�� indicatorInfo ���� ����.
	// -> gaphData(subGraph����)�� IndicatorInfo�� ����.
	//    (��ǥ���� ���õ� ������ ����� ��� ��ǥ��� ����)
	bool ChangeGraphDataFromIndicatorInfo(const bool bIsCalculate);
	BOOL ChangeGraphDataFromIndicatorInfo( CIndicatorInfo& indicatorInfo, const bool bIsCalculate);
	// -> IndicatorInfo�� gaphData(subGraph����)�� ����.
	bool ChangeIndicatorInfoFromGraphData();
	bool ChangeIndicatorInfoFromGraphData( CIndicatorInfo& indicatorInfo);
	// -> graphData �� indicatorInfo�� ��� ����.
	bool ChangeIndicatorInfoAndGraphData(const CIndicatorInfo* pSourceIndicatorInfo, const bool bIsCalculate);
	// -> �ش� sub graph�� indicatorInfo ���� ����.
	bool ChangeAllSubGraphTitleFromIndicator();
	bool ChangeSubGraphTitleFromIndicator(const int nSubGraphIndex);
	// -> �ش� graph�� Map Default IndicatorInfo�� ����
	bool ChangeMapDefaultIndicatorInfo( CIndicatorList* pIndicatorList);


	void SetVertScaleGroup(CVertScaleGroup* pVertScaleGroup);
	// graph�� data setting.
//	bool SetGraph(const CGraphData& graphData, const CString& strSubGraphData);
	// �Լ���� packetname�� �����Ѵ�.
//	bool SetGraphNameAndPacketName(const CString& strGraphName, const CString& strPacketName);
//	bool SetGraphNameAndSubGraphNames(const CString& strGraphName, const bool bIsSubGraphCheck, const bool bIsCalculate);
//	bool SetGraphName(const CString& strGraphName, const bool bIsCalculate = true);
	bool SetPacketNames(const CString& strPacketName, const bool bIsChangeIndicatorInfo = true, const bool bIsCalculate = true);
	bool SetGraphDataDrawingData(const CGraphDataDrawingData& sourceGraphDrawingData);
	// GraphDataDraw ���� �߰� - ojtaso (20080609)
	bool ShowGraphDataDrawingData(const bool bShow, const int nShowType);

	// ���� class �ּ�
	void SetMainBlockBaseData(CMainBlockBaseData* pMainBlockBaseData);
	CMainBlockImp* GetMainBlock() const;
	CBlockImp* GetBlock() const;
	CVertScaleGroupList* GetVertScaleGroupList() const;
	CVertScaleGroup* GetVertScaleGroup() const;

	// graph data
	CGraphData& GetGraphData();
	const CGraphData& GetGraphData() const;
	// graph�� ������ �ִ� subGraph���� �ִ� list.
	CSubGraphList& GetSubGraphList();
	const CSubGraphList& GetSubGraphList() const;
	CIndicatorInfo* GetIndicatorInfo();
	const CIndicatorInfo* GetIndicatorInfo() const	{	return m_pIndiInfo;	}

	// graph data ��������
	void GetStringGraphData( CStringList &p_slGraphData) const;

	// graph title region
	CRect GetGraphTitleRegion(CDC* pDC, const CRect& graphRegion, int& preGraphTitleEndPosition) const;
	// view data setting
	void GetSubGraphViewDataList(CSubGraphViewDataList& subGraphViewDataList) const;

	// ���� scale�� ���� ���Ѵ�.
	int GetLeftVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const;
	int GetRightVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const;
	// ���� ���̴� ȭ�鿡���� min, max�� ���Ѵ�. ==> y���� ��
	bool GetDisplayDataMinMax(const CString& strScaleData, CDisplayDataMinMax& displayDataMinMax, BOOL p_bWithFullRange) const;

	bool IsExistencePacketName(const CList<CString, CString>& packetNameList) const;
	bool IsExistencePacketName(const CString& strPacketName) const;

	// (2009/1/7 - Seung-Won, Bae) Support graph title drawing routine.
	void DrawGraphTitle( CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, int& nGraphTitleEndLine);
	void DrawGraphAndSignal( CDC* pDC, CDrawingGraphData* pDrawingGraphData);

	LPCTSTR GetGraphRQ() const;						// Graph�� RQ : �������� - ojtaso (20061224)
	void	SetGraphRQ(LPCTSTR lpszRQ);				// RQ ���� : �������� - ojtaso (200705015)
	BOOL	ComparePacketRQ(LPVOID pPacketRQ);		// CPacketRQ*�� Graph üũ : �������� - ojtaso (20070109)
	 // CPacketRQ�� �˷��� : �������� - ojtaso (20070531)
	CPacketRQ*	GetPacketRQ();

	// Ÿ��Ʋ�� ����� ǥ�� : �������� - ojtaso (20070322)
	void ChangeSubGraphTitle(LPCTSTR lpszPreName, LPCTSTR lpszNewName);

private:
	// Info�� data�� SubGraph�� �����.
//	bool MakeAllSubGraphFromIndicatorInfo(const bool bIsCalculate);
	// IndicatorInfo �����ϱ�
	bool DeleteIndicatorInfo();
	// Packet ���� �ϱ�
	void DeletePacket_SubGraph(const bool bIsOnlyNoTRData = true);
	void DeletePacket_Signal(const bool bIsOnlyNoTRData = true);
	// Packet Clear �ϱ�
	void ClearPacket(const bool bIsOnlyNoTRData = true);
	void ClearPacket_Signal(const bool bIsOnlyNoTRData = true);
	// Packet �ʱ�ȭ
	void Initialize_AllPackets();
	// Indicator ����ϱ�
	bool CalculateIndicator(const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsClearPacketData = false, const bool bIsRequestTR = false);
	bool CalculateIndicator_TR(const CCalculateBaseData::CALCULATETYPE eCalType);
	bool CalculateIndicator_Indicator(const CCalculateBaseData::CALCULATETYPE eCalType);
	void CalculateIndicator_OCX(const CCalculateBaseData::CALCULATETYPE eCalType);
	// TR ��û
	bool SendMessage_TRData();

	bool IsDisplayDataMinMax() const;
	bool IsEqualPacketNames(const CString& strPacketNames) const;
	bool IsDeleteIndicatorInfo() const;
	bool IsSendMessage_TRData() const;
	bool IsSendMessage_NewTRData(const CPacket* pPacket) const;
	bool IsSendMessage_InsertTRData(const CPacket* pPacket) const;
	bool IsSendMessage_InsertTRData(const CPacketList* pPacketList, const int nDataCount) const;

	CPacketList* GetPacketList() const;
	CIndicatorList* GetIndicatorList() const;
	CMainChartFormulate* GetMainChartFormulate() const;
	bool GetChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType, CChartFormulateData& chartFormulateData) const;
	CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition(const CString& strVertScalePosition) const;
	// ���� scale width ���ϱ�.
	int GetVertScaleRegionWidth(CDC* pDC, CVertScaleMaxWidthCalculatorData* pCalData) const;
	// <������Ʈ>�� color ���ϱ�
	CGraphColor* GetPriceChartColor() const;
	// �糡�� " "�� ","�� ���� string.
	CString GetNoSpaceCommaString(const CString& strData) const;
	// TR ���� count �� type
	bool GetRequestTRCountAndType(int& nRequestTRCount, bool& bIsNew) const;
	bool GetRequestTRCountAndType_New(int& nRequestTRCount, bool& bIsNew) const;
	bool GetRequestTRCountAndType_Insert(const CPacket* pPacket, int& nRequestTRCount, bool& bIsNew) const;

//	void ChangeGraphData(const CGraphData& sourceGraphData, const bool bIsAutoCalCheck);
//	bool ChangeAllSubGraphData(const bool bIsCheckSubGraphCount, const bool bIsAllChangeSubGraphData);
	bool ChangeAllSubGraphDataFromIndicatorInfo(const CIndicatorInfo* pIndicatorInfo, const bool bIsAllChangeSubGraphData = true);
	bool ChangeIndicatorInfoFromAllSubGraphData(CIndicatorInfo& indicatorInfo);
	void ChangeTitleDraw(CDrawingGraphData* pDrawingGraphData, bool& bOldConditionTitleDraw, bool& bOldDataTitleDraw);
	bool ChangePacketNameInIndicatorInfo(const CString& strPacketName);
	bool ChangeIndicatorInfo(const CIndicatorInfo* pSourceIndicatorInfo);

	// graph�� �׸���.
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
//	void DrawGraph(CDC* pDC, CDrawingGraphData* pDrawingGraphData, int& graphTitlePosition, int& graphTitleLine, int& nGraphTitleEndLine);
	// signal�� �׸���.
//	void DrawSignal(CDC* pDC, const CDrawingGraphData& drawingGraphData);
	void CheckWeightedVolume();
	// Ư���� ��ġ�� �ڽ����¸� �׸� (���� 2512 ȭ�� ��û) - ojtaso (20080709)
	void DrawVerticalBox(CDC* pDC, CDrawingGraphData* pDrawingGraphData);

private:
	CMainBlockBaseData* m_pMainBlockBaseData;
	CVertScaleGroup* m_pVertScaleGroup;
	CIndicatorInfo* m_pIndiInfo;
	CPacket* m_pSignalPacket;
	CGraphData m_graphData;
	CSubGraphList m_subGraphList;	// subGraph���� list���·� �ִ� ��ü

	//	CString		m_strRQ;		// RQ�� ������ �ִ� Graph : �������� - ojtaso (20061224)
	CPacketRQ*	m_pPacketRQ;		// ������� CPacketRQ* : �������� - ojtaso (20070109)
 
// (2006/9/8 - Seung-Won, Bae) the Temp Variables for Calculating 
protected:
	CIndicator *m_pIndicator;

// (2007/2/28 - Seung-Won, Bae) Support Graph Hiding
protected:
	BOOL	m_bHide;
public:
	virtual void	SetGraphHide( BOOL p_bHide)		{	m_bHide = p_bHide;	}
	virtual BOOL	IsGraphHide( void)				{	return m_bHide;		}

	// �ѹ��� �׷��� �Ӽ� ���� ������ �޽�� �߰� - ojtaso (20080602)
	void ChangeGraphPropertyAtOnce(short nSubGraphIndex, short nGraphType, short nGraphStyle, short nDrawStyle, short nThickness1, short nThickness2, short nThickness3);

// 20081007 JS.Kim	����ڽð��� ����
public:
	LPCTSTR			GetPacketTimeDiffName();
	int				GetPacketOrgBaseTimeDiff();
	void			SetPacketTimeDiff(CString strTimeName, int nTimeDiff);

	// KTB ü���ܰ�ǥ�� - ojtaso (20091019)
	long	AddHorizontalLine(LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);
	void	DeleteHorizontalLine(long nIndex);
	void	DeleteAllHorizontalLine();
	void	UpdateHorizontalLine(long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);

// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
public:
	CDC *				m_pDC;
	CDrawingGraphData *	m_pDrawingGraphData;

	// �ѹ��� �׷��� �Ӽ� ���� �� �޽�� �߰� - (@Solomon-20100120)
	bool GetGraphPropertyAtOnce( short nSubGraphIndex, short &nGraphType, short &nGraphStyle, short &nDrawStyle, short &nThickness1, short &nThickness2, short &nThickness3);
};

#endif // !defined(AFX_GRAPHIMP_H__6C371F97_C2BC_4CBA_8AB9_E44AD1E3B2A0__INCLUDED_)
