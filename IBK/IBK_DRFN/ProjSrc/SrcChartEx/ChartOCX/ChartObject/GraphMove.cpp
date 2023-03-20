// GraphMove.cpp: implementation of the CGraphMove class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphMove.h"

#include "../Include_Chart/Dll_Load/MainBlock.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/Dll_Load/IndicatorInfo.h"	// for CIndicatorInfo
#include "../Include_Chart/IChartCtrl.h"				// for IChartCtrl
#include "../Include_Chart/ScaleDataList.h"
#include "BlockImp.h"
#include "GraphImp.h"
#include "VertScaleGroup.h"
#include "SelectedSubGraphDeque.h"
#include "GraphSearchInfo.h"							// for CGraphSearchInfo_GraphName
#include "DlgGraphMove.h"								// for CDlgGraphMove


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CGraphMove

CGraphMove::CGraphMove(CWnd* pParent):
	m_pParent(pParent),
	m_bGraphMoving(false)
{
	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = NULL;
}


// public =====================================================================
void CGraphMove::InitializeAllData(CWnd* pParent)
{
	m_pParent = pParent;
	m_bGraphMoving = false;

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	if( m_pParent) m_hOcxWnd = pParent->GetSafeHwnd();
}

void CGraphMove::SetGraphMoving(const bool bGraphMoving)
{
	m_bGraphMoving = bGraphMoving;
}

// ----------------------------------------------------------------------------
bool CGraphMove::IsGraphMoving() const
{
	return m_bGraphMoving;
}

// ----------------------------------------------------------------------------
// graph move -> �ϳ��� �̵��ȴ�!!
bool CGraphMove::GraphMoving(CMainBlock* pMainBlock, CGraphImp* pSourceGraph, const CPoint& targetPoint)
{
	// Graph �и�/���� - ojtaso (20070731)
	int nInsertCol = -1;
	int nInsertRow = -1;

	// (2006/11/19 - Seung-Won, Bae) Check Graph Deleting.
	// Graph �и�/���� - ojtaso (20070731)
	CRect targetGraphRegion = pMainBlock->GetNoFrameGraphRegion( targetPoint, nInsertCol, nInsertRow);
	if( targetGraphRegion.IsRectEmpty())
	{
		CString strIndicatorName = pSourceGraph->GetGraphData().GetIndicatorName2();
		
		// ������Ʈ ���� : �������� - ojtso (20070522)
		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		if( strIndicatorName == _MTEXT( C2_PRICE_CHART) && !::strncmp(pSourceGraph->GetGraphRQ(), _T("DEFAULT"), 7))
		{
			ML_SET_LANGUAGE_RES();
			CString strFormat;
			strFormat.LoadString( IDS_CANNOT_DELETE_IT);
			CString strTitle;
			strTitle.LoadString( IDS_NOTATION);
			CString message;
			message.Format( strFormat, _MTOL2( strIndicatorName));
			if( !m_pParent) AfxMessageBox(message, MB_ICONWARNING | MB_OK);
			else MessageBox( m_pParent->m_hWnd, message, strTitle, MB_ICONWARNING | MB_OK);

			//NH:100528 ������Ʈ �������ϹǷ� ����.
			return false;
		}

		IChartCtrl *pChartCtrl = pMainBlock->GetIChartCtrl();
		if( !pChartCtrl) return false;

		// (2006/12/13 - Seung-Won, Bae) Check Special Graph for Change One Chart Mode.
		if( g_iMetaTable.IsSpecialGroupGraph( strIndicatorName)) return pChartCtrl->AddRemoveGraph( strIndicatorName);
		return pMainBlock->DeleteGraph( pSourceGraph->GetGraphData().GetGraphName2());
	}
	
	// Graph �и�/���� - ojtaso (20070731)
	if(IsGraphInsert(pMainBlock, pSourceGraph, targetPoint, targetGraphRegion, nInsertCol, nInsertRow))
		return false;

	if(!IsGraphMoving(pMainBlock, pSourceGraph, targetPoint, targetGraphRegion))
		return false;

	LoadDialog_GraphMoving(pMainBlock, pSourceGraph, targetPoint);
	return true;
}

// Graph �и�/���� - ojtaso (20070731)
bool CGraphMove::IsGraphInsert(CMainBlock* pMainBlock, CGraphImp* pSourceGraph, const CPoint& targetPoint, const CRect &targetGraphRegion, const int& nInsertCol, const int& nInsertRow) const
{
	if(nInsertRow < 0 || nInsertCol < 0)
		return false;

	if(pMainBlock == NULL || pSourceGraph == NULL || !m_bGraphMoving)
		return false;
	
	IChartCtrl *pChartCtrl = pMainBlock->GetIChartCtrl();
	CString strIndicatorName = pSourceGraph->GetGraphData().GetIndicatorName2();

	// (2010/5/4 - Seung-Won, Bae) Fixed Area of Obvious Glance Balance Chart on X Scale Manager
	CString strData;
	CIndicatorInfo *pIndicatorInfo = pSourceGraph->GetIndicatorInfo();
	if( pIndicatorInfo)
	{
		pIndicatorInfo->GetGraphDataForMaking( strData);
		pIndicatorInfo = CIndicatorInfo::Make( pIndicatorInfo);
	}
	else
	{
		CStringList slGraphData;
		pSourceGraph->GetStringGraphData( slGraphData);
		POSITION psnGraphData = slGraphData.GetHeadPosition();
		while( psnGraphData) strData += slGraphData.GetNext( psnGraphData);
	}

	CString strGraphName( pSourceGraph->GetGraphData().GetGraphName2());
	CPacketRQ *pPacketRQ = pSourceGraph->GetPacketRQ();
	if( g_iMetaTable.IsSpecialGroupGraph( strIndicatorName)) 
		pChartCtrl->AddRemoveGraph( strIndicatorName);
	else
		pMainBlock->DeleteGraph( strGraphName);

	bool bresult = pMainBlock->InsertGraphInBlock( pPacketRQ, CBlockIndex( nInsertRow, nInsertCol), 0, strData, TRUE, TRUE, TRUE);

	CIndicatorList *pIndicatorList = pMainBlock->GetIndicatorList();
	if( pIndicatorInfo && pIndicatorList)
	{
		CIndicatorInfo *pNewIndicatorInfo = pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, strIndicatorName, EIIGT_WITHOUT_DEFAULT);
		if( pNewIndicatorInfo)
		{
			pNewIndicatorInfo->CopyDataWithVerificationFrom( *pIndicatorInfo);
			pMainBlock->ChangeGraphDataFromIndicatorInfo( strGraphName, true);
			delete pIndicatorInfo;
		}
	}

	return bresult;

	// �̵��� ������ ���õ� graph�� �����ϰ� Ʋ���� �Ѵ�.
//	if(sourceGraphRegion.PtInRect(targetPoint) || !targetGraphRegion.PtInRect(targetPoint))
//		return false;

//	return true;
}

// private ====================================================================
// graph move ���� ����.
bool CGraphMove::IsGraphMoving(CMainBlock* pMainBlock, CGraphImp* pSourceGraph, const CPoint& targetPoint, const CRect &targetGraphRegion) const
{
	if(pMainBlock == NULL || pSourceGraph == NULL || !m_bGraphMoving)
		return false;

	// �̵��� ������ ���õ� graph�� �����ϰ� Ʋ���� �Ѵ�.
	CRect sourceGraphRegion = pSourceGraph->GetVertScaleGroupList()->GetGraphRegions().GetNoFrameRegion();
	if(sourceGraphRegion.PtInRect(targetPoint) || !targetGraphRegion.PtInRect(targetPoint))
		return false;

// (2007/2/12 - Seung-Won, Bae) Support Any Graphs Moving. even if PriceChart.
/*

	// �̵��� graph�� <������Ʈ>�� �߰� �Ǵ� graph�� �̵� �ȵ�!!
	if(!IsPriceChartScaleGroup(pSourceGraph))
		return true;

	MessageBox_NoGraphMoving(pSourceGraph->GetGraphData().GetGraphName());
	return false;
*/
	return true;
}

bool CGraphMove::IsPriceChartScaleGroup(CGraphImp* pGraph) const
{
	assert(pGraph != NULL);
	if(pGraph->GetSubGraphList().IsViewableGraphType(CGraphBaseData::DataView_Type, 0))
		return true;

	return g_iMetaTable.IsScaleGroup_Price(pGraph->GetGraphData().GetGraphName());
}

void CGraphMove::MessageBox_NoGraphMoving(const CString& GraphName) const
{
	ML_SET_LANGUAGE_RES();
	CString strFormat;
	strFormat.LoadString( IDS_CANNOT_MOVE_IT);
	CString strTitle;
	strTitle.LoadString( IDS_NOTATION);

	CString message;
	message.Format( strFormat, GraphName);
	if(m_pParent == NULL)
		AfxMessageBox(message, MB_ICONWARNING | MB_OK);
	else
		MessageBox(m_pParent->m_hWnd, message, strTitle, MB_ICONWARNING | MB_OK);
}

// ----------------------------------------------------------------------------
// block�� vertScale group list.
/*
CVertScaleGroupList* CGraphMove::GetVertScaleGroupList(CMainBlock* pMainBlock, const CPoint& point) const
{
	if(pMainBlock == NULL)
		return NULL;

	CBlockIndex bi = pMainBlock->FindBlockIndex( point);
	CBlockImp* pBlock = pMainBlock->GetBlock( bi.GetRow(), bi.GetColumn());
	if(pBlock == NULL)
		return NULL;

	return &pBlock->GetVertScaleGroupList();
}
*/

// block�� �ִ� vertScale�� ��������.
CVerticalScaleGroupDataList CGraphMove::GetVertScaleGroupDataList(CGraphImp* pSourceGraph, const CVertScaleGroupList& targetVertScaleGroupList) const
{
	if(pSourceGraph == NULL)
		return CVerticalScaleGroupDataList();

	CVerticalScaleGroupDataList vertScaleGroupDataList;
	targetVertScaleGroupList.GetVertScaleGroupDataList( vertScaleGroupDataList);
	CVerticalScale movingGraphVertScale = pSourceGraph->GetVertScaleGroup()->GetVertScale();
	vertScaleGroupDataList.AddTail(CVerticalScaleGroupData(movingGraphVertScale, "New Scale"));

	return vertScaleGroupDataList;
}

// �̵��� graph�� ��������.
CMovingGraphDeque CGraphMove::GetMovingGraphDeque(CGraphImp* pGraph)
{
	assert(pGraph != NULL);

	CMovingGraphDeque movingGraphDeque;
	movingGraphDeque.AddTail(pGraph);

	// <������Ʈ>, <�ŷ�����Ʈ> �� ��쿣 <�̵����>�� ���� �̵��Ѵ�.
//	if(pGraph->GetGraphData().GetGraphName() == _MTEXT( C2_PRICE_CHART))
//		AddMovingGraph(pGraph->GetVertScaleGroupList(), _MTEXT( C2_PRICE_MA), movingGraphDeque);
//	else if(pGraph->GetGraphData().GetGraphName() == _MTEXT( C2_PRICE_MA))
//		AddMovingGraph(pGraph->GetVertScaleGroupList(), _MTEXT( C2_PRICE_CHART), movingGraphDeque);
//	else if(pGraph->GetGraphData().GetGraphName() == _MTEXT( C2_VOLUME_CHART))
//		AddMovingGraph(pGraph->GetVertScaleGroupList(), _MTEXT( C2_VOLUME_MA), movingGraphDeque);
//	else if(pGraph->GetGraphData().GetGraphName() == _MTEXT( C2_VOLUME_MA))
//		AddMovingGraph(pGraph->GetVertScaleGroupList(), _MTEXT( C2_VOLUME_CHART), movingGraphDeque);

	return movingGraphDeque;
}

void CGraphMove::AddMovingGraph(CVertScaleGroupList* pVertScaleGroupList, const CString& addGraphName, CMovingGraphDeque& movingGraphDeque)
{
	if(pVertScaleGroupList == NULL)
		return;

	// (2008/9/13 - Seung-Won, Bae) Search with Indicator Name.
	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = addGraphName;
	if( pVertScaleGroupList->FindGraph( &gsi)) movingGraphDeque.AddTail( gsi.m_pGraphImp);
}

// ----------------------------------------------------------------------------
// graph �̵��� ���� dlg load.
bool CGraphMove::LoadDialog_GraphMoving(CMainBlock* pMainBlock, CGraphImp* pSourceGraph, const CPoint& targetPoint)
{
	assert(pMainBlock != NULL && pSourceGraph != NULL);

	CBlockImp* pTargetBlock = ( CBlockImp *)pMainBlock->GetBlock( pMainBlock->FindBlockIndex( targetPoint));
	if(pTargetBlock == NULL)
		return false;

	CVerticalScaleGroupDataList dataList = GetVertScaleGroupDataList(pSourceGraph, pTargetBlock->GetVertScaleGroupList());
	CMovingGraphVertScaleData movingVertScaleData;
	if(!LoadDialog_GraphMoving( pMainBlock, pSourceGraph, dataList, pMainBlock->GetOCXPath(), movingVertScaleData))
		return false;

	return GraphMoving(pSourceGraph, &pTargetBlock->GetVertScaleGroupList(), movingVertScaleData);
}

bool CGraphMove::LoadDialog_GraphMoving( CMainBlock* pMainBlock, CGraphImp* pSourceGraph, const CVerticalScaleGroupDataList& vertScaleGroupDataList, const CString& strOCXPath, CMovingGraphVertScaleData& movingVertScaleData)
{
	if(vertScaleGroupDataList.GetCount() <= 0)
		return false;

	// (2007/6/27 - Seung-Won, Bae) Show UI Directly.
	//	CChartObjectWrapperHelper wrapperHelper(strOCXPath);
	//	return wrapperHelper.MoveGraph(m_pParent, vertScaleGroupDataList, movingVertScaleData);
	ML_SET_LANGUAGE_RES();
	CDlgGraphMove dlg( m_pParent, pSourceGraph, vertScaleGroupDataList, pMainBlock->GetIChartCtrl());
	if( IDOK != dlg.DoModal()) return false;

	movingVertScaleData = dlg.GetMovingGraphVertScaleData();
	return true;
}

// ----------------------------------------------------------------------------
// graph move.
bool CGraphMove::GraphMoving(CGraphImp* pSourceGraph, CVertScaleGroupList* pTargetScaleGroupList, const CMovingGraphVertScaleData& movingVertScaleData)
{
	if(pSourceGraph == NULL || pTargetScaleGroupList == NULL)
		return false;

	CVertScaleGroupList* pSourceScaleGroupList = pSourceGraph->GetVertScaleGroupList();
	// �̵��� graph���� �����ͼ� graph�� �̵��ϱ�.
	if(!GraphMoving(pTargetScaleGroupList, movingVertScaleData, GetMovingGraphDeque(pSourceGraph)))
		return false;

	SetVertScaleGridType(pSourceScaleGroupList);

	// (2006/11/14 - Seung-Won, Bae) Delete Empty Block.
	CGraphSearchInfo_AnyGraph gsi;
	if( !pSourceScaleGroupList->FindGraph( &gsi)) pSourceScaleGroupList->RemoveAll( true);

	return true;
}

bool CGraphMove::GraphMoving(CVertScaleGroupList* pTargetScaleGroupList, const CMovingGraphVertScaleData& movingVertScaleData, const CMovingGraphDeque& movingGraphDeque)
{
	if(movingGraphDeque.GetCount() <= 0)
		return false;

	bool bResult = false;
	for(int nIndex = 0; nIndex < movingGraphDeque.GetCount(); nIndex++){
		CGraphImp* pMovingGraph = movingGraphDeque.GetAt(nIndex);
		assert(pMovingGraph != NULL);
		if(pTargetScaleGroupList->MoveGraph(pMovingGraph, movingVertScaleData.GetVertScaleGroupIndex(), false)){
			bResult = true;
			pMovingGraph->GetVertScaleGroup()->SetVerticalScale(movingVertScaleData.GetVerticalScale());
		}
	}

	return bResult;
}

void CGraphMove::SetVertScaleGridType(CVertScaleGroupList* pVertScaleGroupList)
{
	if(pVertScaleGroupList == NULL)
		return;

	CGraphSearchInfo_HasVertScaleAndGridLine gsi;
	gsi.m_eVertScalePosition = ( CScaleBaseData::VERTSCALEPOSITION)-1;
	if( !pVertScaleGroupList->FindGraph( &gsi)) return;
	if( !gsi.m_pVertScaleGroup) return;

	pVertScaleGroupList->SetActivate( gsi.m_pVertScaleGroup, VSGA_GRIDLINE);
}
