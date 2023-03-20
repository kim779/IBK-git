// SelectSubGraphData.h: interface for the CSelectSubGraphData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELECTSUBGRAPHDATA_H__CB0433E3_FDE0_4978_85E3_7222193FDA25__INCLUDED_)
#define AFX_SELECTSUBGRAPHDATA_H__CB0433E3_FDE0_4978_85E3_7222193FDA25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseData.h"
#include "./Dll_Load/XScaleManager.h"

class CIndicatorInfo;

// subGraph�� ã�°ſ� ���� data
class CSelectSubGraphData
{
public:
	CSelectSubGraphData();

	void SetDrawingOrderType(const CDrawingOption::DRAWINGORDER eDrawingOrderType);
	void SetGraphName(const CString& GraphName);
	void SetIndicatorName( const CString &p_strIndicatorName)	{	m_strIndicatorName = p_strIndicatorName;	}
	void SetVertScaleLogType(const bool bIsVertScaleLogType);
	void SetVertScaleInvertType(const bool bIsVertScaleInvertType);
	void SetGraphRegions(const CGraphRegions& graphRegions);
	void SetSubGraphType(const CGraphBaseData::GRAPHTYPE subGraphType);
	void SetSubGraphStyle(const CGraphStyle& subGraphStyle);
	void SetSubGraphDrawStyle(const CGraphDrawStyle& subGraphDrawStyle);
	void SetDisplayDataHorzMinMax(const CDisplayDataMinMax& displayDataMinMax);
	void SetDisplayDataVertMinMax(const CDisplayDataMinMax& displayDataMinMax);
	void SetDisplayAttributes(const CDisplayAttributes& displayAttributes);
	void SetSubGraphPacketList(CSubGraphPacketList* pSubGraphPacketList);
	void SetIndicatorInfo(CIndicatorInfo* pIndicatorInfo);
	void SetXScaleManager(CXScaleManager* pXScaleManager);
	void SetRQ(CString strRQ);

	CDrawingOption::DRAWINGORDER GetDrawingOrderType() const;
	CString GetGraphName() const;
	CString GetIndicatorName() const	{	return m_strIndicatorName;	}
	bool GetVertScaleLogType() const;
	bool GetVertScaleInvertType() const;
	CGraphRegions GetGraphRegions() const;
	CGraphBaseData::GRAPHTYPE GetSubGraphType() const;
	CGraphStyle GetSubGraphStyle() const;
	CGraphDrawStyle GetSubGraphDrawStyle() const;
	CDisplayDataMinMax GetDisplayDataHorzMinMax() const;
	CDisplayDataMinMax GetDisplayDataVertMinMax() const;
	CDisplayAttributes GetDisplayAttributes() const;
	CSubGraphPacketList* GetSubGraphPacketList() const;
	CIndicatorInfo* GetIndicatorInfo() const;
	CXScaleManager* GetXScaleManager() const;
	CString GetRQ() const;

private:
	CDrawingOption::DRAWINGORDER m_eDrawingOrderType;
	CString m_strGraphName;		// �Լ���
	CString m_strIndicatorName;
	bool m_bIsVertScaleLogType;		// ���� scale log type ����
	bool m_bIsVertScaleInvertType;	// ���� scale ������(�⺻�� = false)
	CGraphRegions m_graphRegions;	// graph �׸��� ������
	CGraphBaseData::GRAPHTYPE m_TYPESubGraphType; // subGraph type
	CGraphStyle m_nSubGraphStyle;	// subGraph style
	CGraphDrawStyle m_nSubGraphDrawStyle;	// subGraph draw style
	CDisplayDataMinMax m_displayDataHorzMinMax;	// ���� min/max
	CDisplayDataMinMax m_displayDataVertMinMax;	// ���� min/max
	CDisplayAttributes m_displayAttributes;		// ȭ�鿡 �׸� ����/��
	CSubGraphPacketList* m_pSubGraphPacketList;
	CIndicatorInfo* m_pIndicatorInfo;
	CXScaleManager* m_pXScaleManager;
	CString m_strRQ;
};


#endif // !defined(AFX_SELECTSUBGRAPHDATA_H__CB0433E3_FDE0_4978_85E3_7222193FDA25__INCLUDED_)
