// VertScaleGroupList.cpp: implementation of the CVertScaleGroupList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VertScaleGroupList.h"

#include <assert.h>
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/Dll_Load/XScaleManager.h"	// for CXScaleManager
#include "../Include_Chart/Dll_Load/PacketBase.h"		// for CPacket
#include "../Include_Chart/ViewData.h"
#include "../Include_Chart/ScaleDataList.h"
#include "../Include_Chart/Conversion.h"
#include "BlocksColumn.h"
#include "BlockImp.h"
#include "VertScaleGroup.h"
#include "GraphImp.h"
#include "GraphSearchInfo.h"							// for CGraphSearchInfo
#include "PacketRQ.h"									// for CPacketRQ
#include "MainBlockImp.h"								// for CMainBlockImp	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
#include "resource.h"									// for IDS_N_TH

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVertScaleGroupList::CVertScaleGroupList(CMainBlockBaseData* pMainBlockBaseData):
	m_pMainBlockBaseData(pMainBlockBaseData),
	m_pBlock( NULL)
{
}

CVertScaleGroupList::~CVertScaleGroupList()
{
	DeleteAll();
}

//////////////////////////////////////////////////////////////////////
// Base Info
//////////////////////////////////////////////////////////////////////

// ���� class �ּ�
void CVertScaleGroupList::SetBlock( CBlockImp* pBlock)
{
	m_pBlock = pBlock;
}

// ���� class �ּ�
CBlockImp* CVertScaleGroupList::GetBlock() const
{
	return m_pBlock;
}

// ������.
CDrawingRegion CVertScaleGroupList::GetDrawingRegion( void) const
{
	return m_drawingRegion;
}

CGraphRegions CVertScaleGroupList::GetGraphRegions( void) const
{
	return m_drawingRegion.GetGraphRegions();
}

// subGraph title ���� ��������.
CRect CVertScaleGroupList::GetSubGraphTitleRegion(CDC* pDC, const CSubGraph* pSubGraph) const
{
	if(pSubGraph == NULL)
		return CRect(0, 0, 0, 0);
	
	int preTitleEndPosition = VSGL_TITLE_START_POSITION;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();

	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	int nPreLineCount = 1;

	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		CRect region = pVertScaleGroup->GetSubGraphTitleRegion( pDC, pSubGraph, GetGraphRegions().GetTitleRegion(), preTitleEndPosition, nPreLineCount);
		if(!region.IsRectEmpty())
			return region;
	}

	return CRect(0, 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////
// Create
//////////////////////////////////////////////////////////////////////

// ������ vertScaleGroup�� �����ϰ� vertScaleGroup�� �����.
void CVertScaleGroupList::BuildVertScaleGroups( char *p_szVertScaleGroupData, const bool bInitialize)
{
	if( !p_szVertScaleGroupData) return;
	if( !*p_szVertScaleGroupData) return;

	if( bInitialize) DeleteAll();

	char *szVertScaleGroupEnd = NULL;
	char *szVertScaleGroup = strstr( p_szVertScaleGroupData, "VertScaleGroup;\r\n");
	while( szVertScaleGroup)
	{
		szVertScaleGroup += 17;					// "VertScaleGroup;\r\n"

		szVertScaleGroupEnd = strstr( szVertScaleGroup, "VertScaleGroupEnd;\r\n");
		if( szVertScaleGroupEnd) *szVertScaleGroupEnd = '\0';

		// VertScaleGroup ����
		if( *szVertScaleGroup)
		{
			CVertScaleGroup *pVertScaleGroup = CVertScaleGroup::MakeVertScaleGroup( m_pMainBlockBaseData, this, szVertScaleGroup);
			if( pVertScaleGroup) AddTail( pVertScaleGroup);
		}

		if( szVertScaleGroupEnd)
		{
			*szVertScaleGroupEnd = 'V';
			szVertScaleGroup = strstr( szVertScaleGroupEnd, "VertScaleGroup;\r\n");
		}
		else szVertScaleGroup = NULL;
	}
}

// RQ�� �˻� : �������� - ojtaso (20070118)
void CVertScaleGroupList::BuildVertScaleGroups(LPVOID pPacketRQ, char *p_szVertScaleGroupData, const bool bInitialize)
{
	if( !p_szVertScaleGroupData) return;
	if( !*p_szVertScaleGroupData) return;

	if( bInitialize) DeleteAll();

	char *szVertScaleGroupEnd = NULL;
	char *szVertScaleGroup = strstr( p_szVertScaleGroupData, "VertScaleGroup;\r\n");
	while( szVertScaleGroup)
	{
		szVertScaleGroup += 17;					// "VertScaleGroup;\r\n"

		szVertScaleGroupEnd = strstr( szVertScaleGroup, "VertScaleGroupEnd;\r\n");
		if( szVertScaleGroupEnd) *szVertScaleGroupEnd = '\0';

		// VertScaleGroup ����
		if( *szVertScaleGroup)
		{
			CVertScaleGroup *pVertScaleGroup = CVertScaleGroup::MakeVertScaleGroup(pPacketRQ, m_pMainBlockBaseData, this, szVertScaleGroup);
			if( pVertScaleGroup) AddTail( pVertScaleGroup);
		}

		if( szVertScaleGroupEnd)
		{
			*szVertScaleGroupEnd = 'V';
			szVertScaleGroup = strstr( szVertScaleGroupEnd, "VertScaleGroup;\r\n");
		}
		else szVertScaleGroup = NULL;
	}
}

// �̹� �����Ǿ� �ִ� graph�� data�� setting.
// -> empty �� ��쿣 ���ο� vertScaleGroup�� ����� �߰��Ѵ�.
void CVertScaleGroupList::RebuildVertScaleGroups( char *p_szVertScaleGroupData, const bool doesMaintainOldScale)
{
	char *szVertScaleGroup		= strstr( p_szVertScaleGroupData,	"VertScaleGroup;\r\n");
	char *szVertScaleGroupEnd	= strstr( szVertScaleGroup,			"VertScaleGroupEnd;\r\n");

	POSITION posVertScaleGroup = m_realVertScaleGroupList.GetHeadPosition();

	while( szVertScaleGroup)
	{
		szVertScaleGroup += 17;									// "VertScaleGroup;\r\n"
		if( szVertScaleGroupEnd) *szVertScaleGroupEnd = '\0';

		CVertScaleGroup *pVertScaleGroup = NULL;
		if( posVertScaleGroup)
		{
			pVertScaleGroup = m_realVertScaleGroupList.GetNext( posVertScaleGroup);
			if( pVertScaleGroup) pVertScaleGroup->ModifyGraphDataInVertScaleGroup( szVertScaleGroup, doesMaintainOldScale);
			else posVertScaleGroup = NULL;
		}

		if( !pVertScaleGroup && *szVertScaleGroup)
		{
			pVertScaleGroup = CVertScaleGroup::MakeVertScaleGroup( m_pMainBlockBaseData, this, szVertScaleGroup);
			if( pVertScaleGroup)
			{
				if( doesMaintainOldScale)
				{
					CVerticalScale verticalScale;
					CVertScaleGroup *pVertScaleGroup = NULL;
					// �⺻�� �� vertScale�� ����
					// vertScaleGroup�� ������ ��� ���� ù��°�� �⺻���� �Ѵ�.
					if( m_realVertScaleGroupList.GetCount() > 0) pVertScaleGroup = m_realVertScaleGroupList.GetHead();
					else
					{
						// vertScaleGroup�� �������� ���� ��� ���� ù��° block�� ù��° vertScaleGroup�� �⺻���� �Ѵ�.
						CBlockImp *pBlock = GetBlock();
						if( pBlock)
						{
							CBlocksColumn* pBlocksColumn = pBlock->GetBlocksColumn();
							if( pBlocksColumn) if( 0 < pBlocksColumn->GetBlockCount())
								pVertScaleGroup = pBlocksColumn->GetVertScaleGroup( 0, 0);
						}
					}
					if( pVertScaleGroup) verticalScale = pVertScaleGroup->GetVertScale();

					pVertScaleGroup->SetVerticalScale( verticalScale);
				}
				AddTail(pVertScaleGroup);
			}
		}

		if( szVertScaleGroupEnd)
		{
			*szVertScaleGroupEnd = 'V';
			szVertScaleGroup = strstr( szVertScaleGroupEnd, "VertScaleGroup;\r\n");
			if( szVertScaleGroup) szVertScaleGroupEnd = strstr( szVertScaleGroup, "VertScaleGroupEnd;\r\n");
		}
		else szVertScaleGroup = NULL;
	}
}

// ��� VertScaleGroup�� ��� data
void CVertScaleGroupList::GetVertScaleGroupsAllData( CStringList &p_slVertScaleGroupAllData) const
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while( pos)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		pVertScaleGroup->GetVertScaleGroupAllData( p_slVertScaleGroupAllData);
	}
}

// vertScaleGroup�� �߰� �Ѵ�. �� �������� ���� ��츸 �߰��Ѵ�.
bool CVertScaleGroupList::AddTailInNoEqual(CVertScaleGroup* pVertScaleGroup)
{
	if(pVertScaleGroup == NULL)
		return false;

	CGraphSearchInfo_VerticalScaleGroup gsi;
	gsi.m_pVertScaleGroup = pVertScaleGroup;
	if( FindGraph( &gsi)) return false;

	AddTail( pVertScaleGroup);
	return true;
}

bool CVertScaleGroupList::MakeVertScaleGroups_Empty(const int nVertScaleGroupCount)
{
	DeleteAll();

	if(nVertScaleGroupCount < 0)
		return false;

	for(int nIndex = 0; nIndex < nVertScaleGroupCount; nIndex++){
		CVertScaleGroup* pVertScaleGroup = CVertScaleGroup::MakeVertScaleGroup(m_pMainBlockBaseData, this);
		if(pVertScaleGroup == NULL)
			return false;
		AddTail(pVertScaleGroup);
	}
	return true;
}

// graph ����
bool CVertScaleGroupList::InsertGraphInVertScaleGroup( const int nVertScaleGroupIndex, const CString& graphData, const bool doesInsertInNewVertScaleGroup)
{
	// nVertScaleGroupIndex ��ġ�� vertScaleGroup�� ���� �����.
	if( doesInsertInNewVertScaleGroup)
	{
		// �ش� index�� vertScaleGroup�� ���� ����� �����Ѵ�.
		if( nVertScaleGroupIndex < 0 || m_realVertScaleGroupList.GetCount() < nVertScaleGroupIndex)
			return false;

		// �ش� index�� vertScaleGroup�� <graph count == 0> �ϰ�쿣 ���� ������ �ʴ´�.
		CVertScaleGroup* pTargetVertScaleGroup = GetVertScaleGroup( nVertScaleGroupIndex);
		if( !pTargetVertScaleGroup || 0 < pTargetVertScaleGroup->GetGraphCount())
		{
			// insert �� vertScaleGroup�� ���� �����.
			CVertScaleGroup* pNewVertScaleGroup = CVertScaleGroup::MakeVertScaleGroup( m_pMainBlockBaseData, this, "");
			assert(pNewVertScaleGroup != NULL);

			// y�� scale���� �������ش�.
			pTargetVertScaleGroup = GetVertScaleGroup( 0);
			if( pTargetVertScaleGroup != NULL) pNewVertScaleGroup->SetVerticalScale( pTargetVertScaleGroup->GetVertScale());

			if( nVertScaleGroupIndex <= 0)
			{
				assert(pNewVertScaleGroup != NULL);
				pNewVertScaleGroup->SetMainBlockBaseData(m_pMainBlockBaseData);
				pNewVertScaleGroup->SetVertScaleGroupList(this);
				m_realVertScaleGroupList.AddHead(pNewVertScaleGroup);
			}
			else
			{
				
				POSITION targetPos = m_realVertScaleGroupList.FindIndex( nVertScaleGroupIndex);
				if(targetPos == NULL)
					m_realVertScaleGroupList.AddTail(pNewVertScaleGroup);
				else
					m_realVertScaleGroupList.InsertBefore(targetPos, pNewVertScaleGroup);
			}
		}
	}

	CVertScaleGroup *pVertScaleGroup = GetVertScaleGroup( nVertScaleGroupIndex);
	if( pVertScaleGroup == NULL) return false;

	CString strGraph;
	char *szGraph = strGraph.GetBufferSetLength( graphData.GetLength());
	strcpy( szGraph, graphData);
	return pVertScaleGroup->MakeGraph( szGraph);
}

// RQ�� ������ �ִ� Graph ���� : �������� - ojtaso (20061224)
// CPacketRQ*�� ���� : �������� - ojtaso (20070109)
bool CVertScaleGroupList::InsertGraphInVertScaleGroup(LPVOID pPacketRQ, const int nVertScaleGroupIndex, const CString& graphData, const bool doesInsertInNewVertScaleGroup)
{
	// nVertScaleGroupIndex ��ġ�� vertScaleGroup�� ���� �����.
	if(doesInsertInNewVertScaleGroup)
	{
		// �ش� index�� vertScaleGroup�� ���� ����� �����Ѵ�.
		if( nVertScaleGroupIndex < 0 || m_realVertScaleGroupList.GetCount() < nVertScaleGroupIndex)
			return false;

		// �ش� index�� vertScaleGroup�� <graph count == 0> �ϰ�쿣 ���� ������ �ʴ´�.
		CVertScaleGroup* pTargetVertScaleGroup = GetVertScaleGroup( nVertScaleGroupIndex);
		if( !pTargetVertScaleGroup || 0 < pTargetVertScaleGroup->GetGraphCount())
		{
			// insert �� vertScaleGroup�� ���� �����.
			CVertScaleGroup* pNewVertScaleGroup = CVertScaleGroup::MakeVertScaleGroup( m_pMainBlockBaseData, this, "");
			assert(pNewVertScaleGroup != NULL);

			// y�� scale���� �������ش�.
			pTargetVertScaleGroup = GetVertScaleGroup( 0);
			if( pTargetVertScaleGroup != NULL) pNewVertScaleGroup->SetVerticalScale( pTargetVertScaleGroup->GetVertScale());

			if( nVertScaleGroupIndex <= 0)
			{
				assert(pNewVertScaleGroup != NULL);
				pNewVertScaleGroup->SetMainBlockBaseData(m_pMainBlockBaseData);
				pNewVertScaleGroup->SetVertScaleGroupList(this);
				m_realVertScaleGroupList.AddHead(pNewVertScaleGroup);
			}
			else
			{
				
				POSITION targetPos = m_realVertScaleGroupList.FindIndex( nVertScaleGroupIndex);
				if(targetPos == NULL)
					m_realVertScaleGroupList.AddTail(pNewVertScaleGroup);
				else
					m_realVertScaleGroupList.InsertBefore(targetPos, pNewVertScaleGroup);
			}
		}
	}

	CVertScaleGroup* pVertScaleGroup = GetVertScaleGroup(nVertScaleGroupIndex);
	if(pVertScaleGroup == NULL)
		return false;

	CString strGraph;
	char *szGraph = strGraph.GetBufferSetLength( graphData.GetLength());
	strcpy( szGraph, graphData);
	return pVertScaleGroup->MakeGraph(pPacketRQ, szGraph);
}

// list�� �߰��Ѵ�.
void CVertScaleGroupList::AddTail(CVertScaleGroup* pVertScaleGroup)
{
	assert(pVertScaleGroup != NULL);
	pVertScaleGroup->SetMainBlockBaseData(m_pMainBlockBaseData);
	pVertScaleGroup->SetVertScaleGroupList(this);
	m_realVertScaleGroupList.AddTail(pVertScaleGroup);
}

//////////////////////////////////////////////////////////////////////
// Delete
//////////////////////////////////////////////////////////////////////

// ����Ʈ�� ��� ���� ����
void CVertScaleGroupList::DeleteAll()
{
	// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
	//		In Destructor, can be access the List.
	while( !m_realVertScaleGroupList.IsEmpty())
	{
		CVertScaleGroup::Delete( m_realVertScaleGroupList.GetHead());
		m_realVertScaleGroupList.RemoveHead();
	}
}

void CVertScaleGroupList::RemoveAll(const bool bIsDeleteBlock)
{
	POSITION pos = m_realVertScaleGroupList.GetTailPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetPrev(pos);
		assert(pVertScaleGroup != NULL);
		// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
		//		In Destructor, can be access the List.
		CVertScaleGroup::Delete( pVertScaleGroup);
		RemoveAt(pVertScaleGroup, bIsDeleteBlock);
	}

	// (2006/11/14 - Seung-Won, Bae) Delete Empty Block.
	if(m_pBlock != NULL && bIsDeleteBlock && m_realVertScaleGroupList.GetCount() <= 0)
		m_pBlock->DeleteFromParent();
}

BOOL CVertScaleGroupList::RemoveAt(CVertScaleGroup* pVertScaleGroup, const bool bIsDeleteBlock)
{
	if( !pVertScaleGroup) return FALSE;

	POSITION posVertScaleGroup = m_realVertScaleGroupList.Find( pVertScaleGroup);
	if( !posVertScaleGroup) return FALSE;

	m_realVertScaleGroupList.RemoveAt( posVertScaleGroup);

	if(m_pBlock != NULL && bIsDeleteBlock && m_realVertScaleGroupList.GetCount() <= 0)
		m_pBlock->DeleteFromParent();

	return TRUE;
}

bool CVertScaleGroupList::DeleteVertScaleGroup(CVertScaleGroup* pVertScaleGroup, const bool bIsDeleteBlock)
{
	// (2007/2/16 - Seung-Won, Bae) Remove in list after Deleting the Object.
	//		In Destructor, can be access the List.
	CVertScaleGroup::Delete( pVertScaleGroup);
	if( !RemoveAt( pVertScaleGroup, bIsDeleteBlock)) return false;

	return true;
}

BOOL CVertScaleGroupList::DeleteAllIndicator( const char *p_szIndicatorName)
{
	if( !p_szIndicatorName)		return FALSE;
	if( !*p_szIndicatorName)	return FALSE;

	BOOL bResult = FALSE;
	CVertScaleGroup *pVertScaleGroup = NULL;
	POSITION posVertScaleGroup = m_realVertScaleGroupList.GetHeadPosition();
	while( posVertScaleGroup)
	{
		pVertScaleGroup = m_realVertScaleGroupList.GetNext( posVertScaleGroup);
		if( !pVertScaleGroup) continue;
		pVertScaleGroup->DeleteAllIndicator( p_szIndicatorName);
	}
	return bResult;
}

//////////////////////////////////////////////////////////////////////
// Search Object
//////////////////////////////////////////////////////////////////////

CVertScaleGroup *CVertScaleGroupList::GetVertScaleGroup( const int nIndex) const
{
	POSITION pos = m_realVertScaleGroupList.FindIndex( nIndex);
	if(pos == NULL) return NULL;

	return m_realVertScaleGroupList.GetAt(pos);
}

CGraphImp *CVertScaleGroupList::GetGraph( const int p_nV, const int p_nG) const
{
	CVertScaleGroup *pVertScaleGroup = GetVertScaleGroup( p_nV);
	if( !pVertScaleGroup) return NULL;
	return pVertScaleGroup->GetGraph( p_nG);
}

// (2007/2/22 - Seung-Won, Bae) Support Next Search.
BOOL CVertScaleGroupList::FindGraph( CGraphSearchInfo *p_pGraphSearchInfo) const
{
	if( !p_pGraphSearchInfo)	return FALSE;

	int nVertScaleGroupIndex	= p_pGraphSearchInfo->m_nVertScaleGroupIndex;
	POSITION posVertScaleGroup	= ( p_pGraphSearchInfo->m_posVertScaleGroup ? p_pGraphSearchInfo->m_posVertScaleGroup : m_realVertScaleGroupList.FindIndex( p_pGraphSearchInfo->m_nVertScaleGroupIndex));

	CVertScaleGroup *pVertScaleGroup = NULL;
	POSITION posCurrentVertScaleGroup = NULL;
	while( posVertScaleGroup)
	{
		posCurrentVertScaleGroup = posVertScaleGroup;
		pVertScaleGroup = m_realVertScaleGroupList.GetNext( posVertScaleGroup);
		if( !pVertScaleGroup) continue;
		if( p_pGraphSearchInfo->FindGraph( pVertScaleGroup))
		{
			p_pGraphSearchInfo->m_posVertScaleGroup		= posCurrentVertScaleGroup;
			p_pGraphSearchInfo->m_nVertScaleGroupIndex	= nVertScaleGroupIndex;
			p_pGraphSearchInfo->m_pVertScaleGroup		= pVertScaleGroup;
			return TRUE;
		}
		nVertScaleGroupIndex++;
	}

	// (2007/4/11 - Seung-Won, Bae) Reset for Next Row Block Search
	p_pGraphSearchInfo->m_posVertScaleGroup		= NULL;
	p_pGraphSearchInfo->m_nVertScaleGroupIndex	= 0;
	return FALSE;
}

// RQ�� �˻� : �������� - ojtaso (20070118)
BOOL CVertScaleGroupList::FindGraph(LPVOID pPacketRQ, CGraphSearchInfo *p_pGraphSearchInfo) const
{
	if( !p_pGraphSearchInfo)	return FALSE;

	int nVertScaleGroupIndex = 0;
	CVertScaleGroup *pVertScaleGroup = NULL;
	CPacketRQ* pRQ = (CPacketRQ*)pPacketRQ;
	POSITION posVertScaleGroup = m_realVertScaleGroupList.GetHeadPosition();
	while( posVertScaleGroup)
	{
		pVertScaleGroup = m_realVertScaleGroupList.GetNext( posVertScaleGroup);
		if( !pVertScaleGroup) continue;
		// (2008/9/15 - Seung-Won, Bae) RQ Search
		if( pVertScaleGroup->FindGraph( pPacketRQ, p_pGraphSearchInfo))
		{
			p_pGraphSearchInfo->m_pVertScaleGroup		= pVertScaleGroup;
			p_pGraphSearchInfo->m_nVertScaleGroupIndex	= nVertScaleGroupIndex;
			return TRUE;
		}
		nVertScaleGroupIndex++;
	}
	return FALSE;
}

CSubGraph *CVertScaleGroupList::GetSubGraph( const int p_nV, const int p_nG, const int p_nSG) const
{
	CVertScaleGroup *pVertScaleGroup = GetVertScaleGroup( p_nV);
	if( !pVertScaleGroup) return NULL;
	return pVertScaleGroup->GetSubGraph( p_nG, p_nSG);
}

// subGraph Title�� �̿��Ͽ� subGraphã��.
CSubGraph* CVertScaleGroupList::FindSubGraphTitle(CDC* pDC, const CPoint& point, CRect& titleRegion) const
{
	CRect allTitleRegion = m_drawingRegion.GetGraphRegions().GetTitleRegion();
	
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	int nLineCount = m_drawingRegion.GetGraphRegions().GetTitleLineCount();
	if(nLineCount > 1)
	{
		CRect rcTitleRegion = allTitleRegion;

		int nHeight = rcTitleRegion.Height();
		rcTitleRegion.bottom = rcTitleRegion.top;
		rcTitleRegion.bottom += nHeight * nLineCount;
		
		if(!rcTitleRegion.PtInRect(point))
			return NULL;
	}
	else if(!allTitleRegion.PtInRect(point))
		return NULL;

	int preTitleEndPosition = VSGL_TITLE_START_POSITION;
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	int nPreLineCount = 1;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL){
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		CSubGraph* pSubGraph = pVertScaleGroup->FindSubGraphTitle(pDC, allTitleRegion, point, titleRegion, preTitleEndPosition, nPreLineCount);
		if(pSubGraph != NULL)
			return pSubGraph;
	}

	return NULL;
}

// subGraph ã��.
bool CVertScaleGroupList::FindSubGraph(const CPoint& point, CSelectedSubGraphDeque& selectedSubGraphDeque) const
{
	if(!m_drawingRegion.GetGraphRegions().GetNoFrameRegion().PtInRect(point))
		return false;

	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		if(pVertScaleGroup->FindSubGraph(point, selectedSubGraphDeque))
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// Move
//////////////////////////////////////////////////////////////////////

// vertScaleGroup �̵�
bool CVertScaleGroupList::MoveAllVertScaleGroups(CVertScaleGroupList* pSourceGroupList)
{
	if(pSourceGroupList == NULL || pSourceGroupList->GetCount() <= 0 || pSourceGroupList == this)
		return false;

	while( pSourceGroupList->GetCount() > 0)
	{
		CVertScaleGroup *pSourceGroup = pSourceGroupList->GetVertScaleGroup( 0);
		if( !pSourceGroup) break;
		pSourceGroupList->RemoveAt( pSourceGroup, true);
		AddTail( pSourceGroup);
	}
	return true;
}

// vertScaleGroup�� join
bool CVertScaleGroupList::ReArrangeAllVertScaleGroups( const bool bDoJoin)
{
	if( bDoJoin)
	{
		// vertScaleGroup�� join
		// --> ��ø : vertScaleGroup�� 1���� ���´�.
		//			 (�� vertScaleGroup�� �ִ� ��� graph�� ������ vertScaleGroup�� ����.)
		if(m_realVertScaleGroupList.GetCount() <= 1)
			return false;

		CVertScaleGroup* pTargetGroup = m_realVertScaleGroupList.GetHead();
		if( !pTargetGroup) return false;
		
		while(m_realVertScaleGroupList.GetCount() > 1)
		{
			CVertScaleGroup* pSourceGroup = GetVertScaleGroup(1);
			if( !pSourceGroup) return false;
			if( pSourceGroup == pTargetGroup) return false;

			pTargetGroup->MoveAllGraphs(pSourceGroup);
		}
		return true;
	}
	else
	{
		// --> ���� : vertScaleGroup�� 1���� graph���� ���´�.
		//			  (n�� graph -> n�� vertScaleGroup)
		// vertScaleGroup �� 1���� graph�� �ִ����� ����
		CGraphSearchInfo_NotOneGraphVScaleGroup gsi;
		if( !FindGraph( &gsi)) return false;

		// graph ���� vertScaleGroup�� �����.
		CVertScaleGroupList tempGroupList( m_pMainBlockBaseData);
		while(m_realVertScaleGroupList.GetCount() > 0)
		{
			CVertScaleGroup* pVertScaleGroup = GetVertScaleGroup( 0);
			assert(pVertScaleGroup != NULL);
			if(!pVertScaleGroup->MoveAllGraphs(tempGroupList))
				break;
		}

		if(tempGroupList.GetCount() <= 0)
			return false;

		// ���� ���� vertScaleGroupList�� ������ vertScaleGroupList�� �ִ´�.
		DeleteAll();
		MoveAllVertScaleGroups(&tempGroupList);
		return true;
	}
	return true;
}

// graph �̵�
bool CVertScaleGroupList::MoveAllGraphs(CVertScaleGroup* pSourceGroup, const int nTargetGroupIndex, const bool bIsDeleteBlock)
{
	CVertScaleGroup* pTragetGroup = GetVertScaleGroup(nTargetGroupIndex);
	if(pSourceGroup == NULL || pSourceGroup->GetGraphCount() <= 0) return false;

	if(pTragetGroup == NULL)
	{
		pTragetGroup = CVertScaleGroup::MakeVertScaleGroup(m_pMainBlockBaseData, this, pSourceGroup->GetVertScale());
		if( !pTragetGroup) return false;
		AddTail(pTragetGroup);
	}
	if( pSourceGroup == pTragetGroup) return false;

	return pTragetGroup->MoveAllGraphs( pSourceGroup);
}

bool CVertScaleGroupList::MoveGraph(CGraphImp* pSourceGraph, const int nTargetGroupIndex, const bool bIsDeleteBlock, const bool bIsAddTail)
{
	// nTargetGroupIndex == -1 : new vertScaleGroup
	if( pSourceGraph == NULL || nTargetGroupIndex < -1 || nTargetGroupIndex > GetCount())
		return false;

	CVertScaleGroup* pSourceGroup = pSourceGraph->GetVertScaleGroup();
	CVertScaleGroup* pVertScaleGroup = GetVertScaleGroup( nTargetGroupIndex);
	if( pSourceGroup == NULL || pSourceGroup == pVertScaleGroup) return false;

	const CVerticalScale& vertScale = pSourceGroup->GetVertScale();
	if( pVertScaleGroup) pVertScaleGroup->SetVerticalScale( vertScale);
	else
	{
		pVertScaleGroup = CVertScaleGroup::MakeVertScaleGroup( m_pMainBlockBaseData, this, vertScale);
		m_realVertScaleGroupList.AddTail( pVertScaleGroup);
	}
	pVertScaleGroup->AddGraph( pSourceGraph, bIsAddTail );

	pSourceGroup->RemoveAt( pSourceGraph, bIsDeleteBlock);
	return true;
}

//////////////////////////////////////////////////////////////////////
// Query & Update
//////////////////////////////////////////////////////////////////////

// �ش� RQ�� �۾� : �������� - ojtaso (20070420)
void CVertScaleGroupList::GetRowViewData(LPCTSTR lpszRQ, CRowViewData& rowViewData) const
{
	// ���� set.
	rowViewData.SetRegion(GetGraphRegions().GetNoFrameRegion(), GetGraphRegions().GetDrawingRegion());
	
	// subGraphs set.
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		pVertScaleGroup->GetSubGraphViewDataList(lpszRQ, rowViewData.GetSubGraphViewDataList());
	}
}

// vertScaleGroup �� ����
int CVertScaleGroupList::GetCount() const
{
	return m_realVertScaleGroupList.GetCount();
}

void CVertScaleGroupList::SetScaleGridType( const CScaleBaseData::SCALEGRIDTYPE gridType)
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		pVertScaleGroup->GetVertScale().SetGridType(gridType);
	}
}

CString CVertScaleGroupList::GetVertScaleGroupDataList_GetVertScaleTitle(CVertScaleGroup* pVertScaleGroup, const int nIndex) const
{
	if(pVertScaleGroup == NULL)
		return "";

	CGraphImp* pGraph = pVertScaleGroup->GetGraph( 0);
	if(pGraph == NULL)
		return "";

	CString title = pGraph->GetSubGraphList().GetSubGraphTitle();
	_MHWND( m_pMainBlockBaseData->GetParent_Handle());
	if(title.IsEmpty())
	{
		ML_SET_LANGUAGE_RES();
		CString strTH;
		strTH.LoadString( IDS_N_TH);
		title.Format( strTH, nIndex);
	}
	else
	{
		CString strNewTitle = _MTOL1_N( title);
		if( strNewTitle.IsEmpty()) strNewTitle = _MTOL3( title);
		title = "(" + strNewTitle + ")";
	}

	return (title + " Scale");
}

// vertScale���� �����´�.
bool CVertScaleGroupList::GetVertScaleGroupDataList(CVerticalScaleGroupDataList& dataList) const
{
	int nIndex = 1;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		dataList.AddTail(
			CVerticalScaleGroupData(	pVertScaleGroup->GetVertScale(),
										GetVertScaleGroupDataList_GetVertScaleTitle( pVertScaleGroup, nIndex)));
		nIndex++;
	}

	return false;
}

void CVertScaleGroupList::GetVertScalePtrDataList(CVerticalScalePtrDataList& dataList) const
{
	// "������Ʈ" �� ������ -> "Log" ����
	CGraphSearchInfo_IndicatorName gsi;
	gsi.m_strIndicatorName = _MTEXT( C2_PRICE_CHART);
	dataList.SetEnableLogType( FindGraph( &gsi) ? true : false);

	int nIndex = 0;
	CVertScaleGroup* pVertScaleGroup = NULL;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	CString strPacketType;
	while(pos != NULL)
	{
		pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		if( !pVertScaleGroup) continue;

		if( pVertScaleGroup->GetVertScale().GetActiveFlag() & VSGA_GRIDLINE)
			dataList.SetFirstIndex( nIndex);

		// (2008/12/12 - Seung-Won, Bae) for octal number.
		strPacketType = "";
		CGraphImp* pGraph = pVertScaleGroup->FindBaseGraphForDrawingVertScaleData();
		if( pGraph)
		{
			CSubGraphList &subGraphList = pGraph->GetSubGraphList();
			CPacket *pPacket = subGraphList.GetHeadPacket();
			if( pPacket) strPacketType = pPacket->GetType();
		}
		dataList.AddTail(	CVerticalScalePtrData(	&pVertScaleGroup->GetVertScale(),
													GetVertScaleGroupDataList_GetVertScaleTitle(pVertScaleGroup, nIndex + 1),
													strPacketType));
		nIndex++;
	}
}

void CVertScaleGroupList::SetAllVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition)
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL){
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		pVertScaleGroup->GetVertScale().SetVertScalePosition(ePosition);
	}
}

// (2006/11/12 - Seung-Won, Bae) Support VScale Min/Max Type Change for All VSCale
void CVertScaleGroupList::SetAllVScaleMinMaxType( CScaleBaseData::VERTMINMAXTYPE p_eVScaleMinMaxType)
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while( pos)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		if( !pVertScaleGroup) return;
		// (2006/11/13 - Seung-Won, Bae) Deny in Lock.
		CVerticalScale &vertScale = pVertScaleGroup->GetVertScale();
		if( !vertScale.IsVertScaleMinMaxLock()) vertScale.SetVertScaleMinMaxType( p_eVScaleMinMaxType);
	}
}

// (2006/11/14 - Seung-Won, Bae) Support VScale Min/Max Type Release in TR
void CVertScaleGroupList::ReleaseAllVScaleSettingInFixedMinMax( void)
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while( pos)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		if( !pVertScaleGroup) return;
		// (2006/11/13 - Seung-Won, Bae) Deny in Lock.
		CVerticalScale &vertScale = pVertScaleGroup->GetVertScale();
		if( !vertScale.IsVertScaleMinMaxLock() && vertScale.GetVertScaleMinMaxType() == CScaleBaseData::VERT_MINMAX_OF_USER)
			vertScale.SetVertScaleMinMaxType( CScaleBaseData::VERT_MINMAX_OF_VIEW);
	}
}

// vert scale Min/Max �� ����Ѵ�.
void CVertScaleGroupList::CalculateDisplayDataMinMaxAllVertScaleGroup()
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		pVertScaleGroup->CalculateDisplayDataMinMax();
	}
}

void CVertScaleGroupList::SetActivate( CVertScaleGroup *p_pVertScaleGroup)
{
	if( !p_pVertScaleGroup) return;
	
	int nActiveFlag = 0;
	CVerticalScale &verticalScale = p_pVertScaleGroup->GetVertScale();
	switch( verticalScale.GetVertScalePosition())
	{
		case CScaleBaseData::VERT_LEFT:		nActiveFlag |= VSGA_LEFTSCALE;
											break;
		case CScaleBaseData::VERT_RIGHT:	nActiveFlag |= VSGA_RIGHTSCALE;
											break;
		case CScaleBaseData::VERT_BOTH:		nActiveFlag |= VSGA_LEFTSCALE;
											nActiveFlag |= VSGA_RIGHTSCALE;
											break;
		case CScaleBaseData::VERT_NONE:
		default:							break;
	}
	if( verticalScale.GetGridType() != CScaleBaseData::SLNONE) nActiveFlag |= VSGA_GRIDLINE;

	SetActivate( p_pVertScaleGroup, nActiveFlag);
}

void CVertScaleGroupList::SetActivate( CVertScaleGroup *p_pVertScaleGroup, int p_nActiveFlag)
{
	if( !p_pVertScaleGroup) return;

	// (2007/7/13 - Seung-Won, Bae) Set Active Flag with Possible Checking.
	CVerticalScale &verticalScale = p_pVertScaleGroup->GetVertScale();
	CScaleBaseData::VERTSCALEPOSITION eVertScalePosition = verticalScale.GetVertScalePosition();
	int nActiveFlag = 0;
	switch( eVertScalePosition)
	{
		case CScaleBaseData::VERT_LEFT:		nActiveFlag = VSGA_LEFTSCALE;						break;
		case CScaleBaseData::VERT_RIGHT:	nActiveFlag = VSGA_RIGHTSCALE;						break;
		case CScaleBaseData::VERT_BOTH:		nActiveFlag = VSGA_LEFTSCALE | VSGA_RIGHTSCALE;	break;
	}
	if( nActiveFlag) nActiveFlag |= VSGA_GRIDLINE;
	nActiveFlag &= p_nActiveFlag;

	CVertScaleGroup *pVertScaleGroup = NULL;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		if( !pVertScaleGroup) continue;
		CVerticalScale &verticalScale = pVertScaleGroup->GetVertScale();
		if( pVertScaleGroup == p_pVertScaleGroup)	verticalScale.AddActiveFlag(	p_nActiveFlag);
		else										verticalScale.RemoveActiveFlag(	p_nActiveFlag);
	}
}

void CVertScaleGroupList::SetAllVertScaleInvertType( BOOL p_bReverse)
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while( pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		pVertScaleGroup->SetVertScaleInvertType( p_bReverse);
	}
}

int CVertScaleGroupList::GetActiveVScaleIndex( CScaleBaseData::VERTSCALEPOSITION p_eVScalePosition)
{
	int nIndex = -1;
	CVertScaleGroup *pVertScaleGroup = NULL;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		nIndex++;
		pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		if( !pVertScaleGroup) continue;
		CVerticalScale &verticalScale = pVertScaleGroup->GetVertScale();
		if( p_eVScalePosition == CScaleBaseData::VERT_LEFT)
		{
			if( verticalScale.GetActiveFlag() & VSGA_LEFTSCALE) return nIndex;
		}
		else
		{
			if( verticalScale.GetActiveFlag() & VSGA_RIGHTSCALE) return nIndex;
		}
	}
	return -1;
}

bool CVertScaleGroupList::GetAllIndicatorName(CList<CString, CString>& strIndicatorNameList) const
{
	bool bResult = false;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		if(pVertScaleGroup->GetAllIndicatorName(strIndicatorNameList))
			bResult = true;
	}
	return bResult;
}

bool CVertScaleGroupList::GetAllGraphName(CList<CString, CString>& strGraphNameList) const
{
	bool bResult = false;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		if(pVertScaleGroup->GetAllGraphName(strGraphNameList))
			bResult = true;
	}
	return bResult;
}

// subGraph�� ���̱� setting.
void CVertScaleGroupList::SetAllGraphsHiding(const bool bIsHiding)
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL){
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		pVertScaleGroup->SetAllGraphsHiding(bIsHiding);
	}
}

// SubGraph
bool CVertScaleGroupList::SetSubGraphHiding(const CString& subGraphTitle, const bool bIsHiding)
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL){
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		if(pVertScaleGroup->SetSubGraphHiding(subGraphTitle, bIsHiding))
			return true;
	}

	return false;
}

// (2007/2/20 - Seung-Won, Bae) Support Indicator Hiding
//		It can not be replaced with AddChartBlock(), because of Condition Reset.
void CVertScaleGroupList::ShowIndicator( const char *p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate)
{
	if( !p_szIndicatorName) return;
	if( !*p_szIndicatorName) return;

	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		pVertScaleGroup->ShowIndicator( p_szIndicatorName, p_bShow, p_bRecalculate);
	}
}

// strPacketName �� ���Ե� ��� subGraph�� packet���� �ʱ�ȭ
bool CVertScaleGroupList::Initialize_Packet(const CString& strPacketName, const bool bIsCalculate)
{
	bool bResult = false;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL){
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		if(pVertScaleGroup->Initialize_Packet(strPacketName, bIsCalculate))
			bResult = true;
	}
	return bResult;
}

// Change Input Packet Names and Calculate again.
BOOL CVertScaleGroupList::ChangeIndicatorPacketNames( const char *p_szIndicatorName, const int p_nSubGraphIndex, const char *p_szPacketNames)
{
	if( !p_szIndicatorName || !p_szPacketNames) return FALSE;
	if( !*p_szIndicatorName || !*p_szPacketNames) return FALSE;

	BOOL bResult = FALSE, bResult2 = FALSE;
	CVertScaleGroup *pVertScaleGroup = NULL;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while( pos)
	{
		pVertScaleGroup = m_realVertScaleGroupList.GetNext( pos);
		if( !pVertScaleGroup) continue;
		bResult2 = pVertScaleGroup->ChangeIndicatorPacketNames( p_szIndicatorName, p_nSubGraphIndex, p_szPacketNames);
		bResult = bResult || bResult2;
	}
	return bResult;
}

// graph�� indicatorinfo�� �ʱ�ȭ(NULL)
void CVertScaleGroupList::Initialize_IndicatorInfo(const bool bIsNULL)
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		pVertScaleGroup->Initialize_IndicatorInfo(bIsNULL);
	}
}

// -> IndicatorInfo�� gaphData(subGraph����)�� ����.
bool CVertScaleGroupList::ChangeAllIndicatorInfoFromGraphData( void)
{
	bool bResult = false;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		if(pVertScaleGroup->ChangeAllIndicatorInfoFromGraphData())
			bResult = true;
	}
	return bResult;
}

BOOL CVertScaleGroupList::ChangeAllGraphDataFromIndicatorInfo( BOOL p_bWithCalculate)
{
	BOOL bResult = FALSE;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		if(pVertScaleGroup->ChangeAllGraphDataFromIndicatorInfo( p_bWithCalculate))
			bResult = TRUE;
	}
	return bResult;
}

// -> �ش� graph�� Map Default IndicatorInfo�� ����
bool CVertScaleGroupList::ChangeAllMapDefaultIndicatorInfo(CIndicatorList* pIndicatorList)
{
	bool bResult = false;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		if(pVertScaleGroup->ChangeAllMapDefaultIndicatorInfo(pIndicatorList))
			bResult = true;
	}
	return bResult;
}

//////////////////////////////////////////////////////////////////////
// Calculate Indicator
//////////////////////////////////////////////////////////////////////

// ��ǥ�� ����Ѵ�.
void CVertScaleGroupList::CalculateAllGraphs(const CCalculateBaseData::CALCULATETYPE eCalType)
{
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		pVertScaleGroup->CalculateAllGraphs(eCalType);
	}
}

bool CVertScaleGroupList::CalculateAllGraphs(const CList<CString, CString>& packetNameList, const CCalculateBaseData::CALCULATETYPE eCalType, const bool bIsHeadGraph)
{
	bool bResult = false;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);
		if(!pVertScaleGroup->CalculateAllGraphs(packetNameList, eCalType, bIsHeadGraph))
			continue;

		bResult = true;
		if(bIsHeadGraph)
			return true;
	}
	return bResult;
}

//////////////////////////////////////////////////////////////////////
// Draw
//////////////////////////////////////////////////////////////////////

void CVertScaleGroupList::GetVertScaleRegionWidth( CDC *p_pDC, int &p_nLeftWidth, int &p_nRightWidth)
{
	CScaleBaseData::VERTSCALEPOSITION eVertScalePosition = CScaleBaseData::VERT_NONE;
	CVertScaleGroup *pCanLeftActiveVertScaleGroup	= NULL;
	CVertScaleGroup *pCanRightActiveVertScaleGroup	= NULL;
	CVertScaleGroup *pCanTopActiveVertScaleGroup	= NULL;
	CVertScaleGroup *pLeftActiveVertScaleGroup		= NULL;
	CVertScaleGroup *pRightActiveVertScaleGroup		= NULL;
	CVertScaleGroup *pGridActiveVertScaleGroup		= NULL;
	CVertScaleGroup *pTopActiveVertScaleGroup		= NULL;
	CVertScaleGroup *pVertScaleGroup				= NULL;
	int nActiveFlag = 0;
	POSITION posVertScaleGroup = m_realVertScaleGroupList.GetTailPosition();
	while( posVertScaleGroup)
	{
		pVertScaleGroup = m_realVertScaleGroupList.GetPrev( posVertScaleGroup);
		if( !pVertScaleGroup) continue;
		if( pVertScaleGroup->DoesAreHidingAllGraphs()) continue;

		CVerticalScale &verticalScale = pVertScaleGroup->GetVertScale();
		nActiveFlag = verticalScale.GetActiveFlag();
		if( nActiveFlag & VSGA_LEFTSCALE)
		{
			if( pLeftActiveVertScaleGroup) pLeftActiveVertScaleGroup->GetVertScale().RemoveActiveFlag( VSGA_LEFTSCALE);
			pLeftActiveVertScaleGroup = pVertScaleGroup;
			pTopActiveVertScaleGroup = pVertScaleGroup;
		}
		if( nActiveFlag & VSGA_RIGHTSCALE)
		{
			if( pRightActiveVertScaleGroup) pRightActiveVertScaleGroup->GetVertScale().RemoveActiveFlag( VSGA_RIGHTSCALE);
			pRightActiveVertScaleGroup = pVertScaleGroup;
			pTopActiveVertScaleGroup = pVertScaleGroup;
		}
		if( nActiveFlag & VSGA_GRIDLINE)
		{
			if( pGridActiveVertScaleGroup) pGridActiveVertScaleGroup->GetVertScale().RemoveActiveFlag( VSGA_GRIDLINE);
			pGridActiveVertScaleGroup = pVertScaleGroup;
		}

		if( pLeftActiveVertScaleGroup && pRightActiveVertScaleGroup) continue;

		switch( verticalScale.GetVertScalePosition())
		{
			case CScaleBaseData::VERT_LEFT:		pCanLeftActiveVertScaleGroup	= pVertScaleGroup;
												pCanTopActiveVertScaleGroup		= pVertScaleGroup;
												break;
			case CScaleBaseData::VERT_RIGHT:	pCanRightActiveVertScaleGroup	= pVertScaleGroup;
												pCanTopActiveVertScaleGroup		= pVertScaleGroup;
												break;
			case CScaleBaseData::VERT_BOTH:		pCanLeftActiveVertScaleGroup	= pVertScaleGroup;
												pCanRightActiveVertScaleGroup	= pVertScaleGroup;
												pCanTopActiveVertScaleGroup		= pVertScaleGroup;
												break;
		}
	}

	if( pGridActiveVertScaleGroup
		&& pLeftActiveVertScaleGroup != pGridActiveVertScaleGroup
		&& pRightActiveVertScaleGroup != pGridActiveVertScaleGroup)
	{
		pGridActiveVertScaleGroup->GetVertScale().RemoveActiveFlag( VSGA_GRIDLINE);
		pGridActiveVertScaleGroup = NULL;
	}

	if( !pLeftActiveVertScaleGroup)
	{
		pLeftActiveVertScaleGroup = pCanLeftActiveVertScaleGroup;
		if( pLeftActiveVertScaleGroup) pLeftActiveVertScaleGroup->GetVertScale().AddActiveFlag( VSGA_LEFTSCALE);
	}

	if( !pRightActiveVertScaleGroup)
	{
		pRightActiveVertScaleGroup = pCanRightActiveVertScaleGroup;
		if( pRightActiveVertScaleGroup) pRightActiveVertScaleGroup->GetVertScale().AddActiveFlag( VSGA_RIGHTSCALE);
	}

	if( !pGridActiveVertScaleGroup)
	{
		if( pTopActiveVertScaleGroup)	pGridActiveVertScaleGroup = pTopActiveVertScaleGroup;
		else							pGridActiveVertScaleGroup = pCanTopActiveVertScaleGroup;
		if( pGridActiveVertScaleGroup) pGridActiveVertScaleGroup->GetVertScale().AddActiveFlag( VSGA_GRIDLINE);
	}

	// (2008/6/13 - Seung-Won, Bae) for Support BuySell Overhanging Supply.
	//		Call with Dual Check.
	if( pLeftActiveVertScaleGroup)	pLeftActiveVertScaleGroup->GetLeftVertScaleRegionWidth(		p_pDC, p_nLeftWidth,	pLeftActiveVertScaleGroup == pRightActiveVertScaleGroup);
	if( pRightActiveVertScaleGroup)	pRightActiveVertScaleGroup->GetRightVertScaleRegionWidth(	p_pDC, p_nRightWidth,	pLeftActiveVertScaleGroup == pRightActiveVertScaleGroup);
}

// drawing region �� ����Ѵ�.
void CVertScaleGroupList::CalculateDrawingRegion( CDC* pDC, const CDrawingRegion& drawingRegion)
{
	m_drawingRegion = drawingRegion;
}

// ���� scale �׸���
void CVertScaleGroupList::DrawVertScale(CDC* pDC, const CBlockData& blockData)
{
	if( m_pMainBlockBaseData == NULL) return;

	// (2009/2/20 - Seung-Won, Bae) check no graph region
	if( m_pMainBlockBaseData->GetXScaleManager()->GetScreenWidth( m_pBlock->GetBlocksColumn()->GetColumnIndex()) <= 0) return;

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	CDrawingVertScaleData drawingVertScaleData(	m_drawingRegion.GetGraphRegions().GetNoFrameRegion(),
												m_drawingRegion.GetGraphRegions().GetDrawingRegion(),
												m_pMainBlockBaseData->GetGraphRegionColor(),
												m_pMainBlockBaseData->GetScaleAttributes().GetScaleColor(),
												FALSE,	// (2008/2/26 - Seung-Won, Bae) It must be set with each VScaleGroup.
												blockData.GetVertScaleLogType(),
												m_pMainBlockBaseData->GetDisplayAttributes(),
												m_pMainBlockBaseData->GetPacketList());		// ȣ������, ���ذ� ��� - ojtaso (20070917)

	CVertScaleGroup *pVertScaleGroup = NULL;
	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	while( pos)
	{
		pVertScaleGroup = m_realVertScaleGroupList.GetNext( pos);
		if( !pVertScaleGroup)  continue;
		// ��ǥ ������ - ojtaso (20071023)
		drawingVertScaleData.SetVertScaleInvertType(pVertScaleGroup->GetVertScaleInvertType());
		pVertScaleGroup->DrawVertScale( pDC, &drawingVertScaleData, m_drawingRegion);
	}
}

// graph �׸���
void CVertScaleGroupList::DrawGraphs(CDC* pDC, const CBlockData& blockData)
{
	if(m_pMainBlockBaseData == NULL)
		return;

	int preGraphTitleEndPosition = VSGL_TITLE_START_POSITION;
	// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
	int	nGraphTitleEndLine = 1;

	// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
	if( THREAD_CALCULATION == m_pMainBlockBaseData->GetXScaleManager()->GetThreadFlag())
	{
		// (2004.04.01, ��¿�) Default Indicator List�� Static���� �����Ͽ�, �ݺ� Loading�� �����Ѵ�.
		//		Static���� CIndicatorList�� Pointer�� �������� �ʴ´�. (CIndicatorList�� Static�� �ƴϴ�.)
		//		CIndicatorInfo�� �������� �ʰ� CIndicatorList�� ������ �� �ֵ��� CDrawingGraphData�� ���� �����Ѵ�.)
		m_pDrawingGraphData =  new CDrawingGraphData(m_pMainBlockBaseData->GetIndicatorList(), 
			m_pMainBlockBaseData->GetViewAttributes().GetDrawingOrderType(), 
			m_drawingRegion.GetGraphRegions(), 
			m_drawingRegion.GetVertScaleRegion().GetLeftScaleTextRegion(),		// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
			m_drawingRegion.GetVertScaleRegion().GetRightScaleTextRegion(),
			blockData.GetDisplayDataHorzMinMax(), blockData.GetHorzScale().GetScaleData(), 
			m_pMainBlockBaseData->GetGraphAttributes().GetEtcGraphColor(), 
			m_pMainBlockBaseData->GetDisplayAttributes(), 
			blockData.GetVertScaleLogType(),
			FALSE,	// (2008/2/26 - Seung-Won, Bae) It must be set with each VScaleGroup.
			m_pMainBlockBaseData->GetGraphAttributes().DoesConditionTitleDraw(),
			m_pMainBlockBaseData->GetGraphAttributes().DoesDataTitleDraw(),
			m_pMainBlockBaseData->GetPacketList(), m_pMainBlockBaseData->GetWithBaseText(),
			m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl()->GetDrawBaseLine(),
			m_pMainBlockBaseData->GetParent_Handle(), m_pBlock->GetBlocksColumn()->GetColumnIndex(),	// ���ؼ� ���� ȯ�漳���� Scale �ڵ����� ��� - ojtaso (20071101)
			m_pMainBlockBaseData->GetMainAttributes().GetChartMode(),			// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
			m_pMainBlockBaseData->GetMainAttributes().GetMainBlockImpl());
	}

	POSITION pos = m_realVertScaleGroupList.GetHeadPosition();
	int nEndLimit = m_drawingRegion.GetVertScaleRegion().GetRightScaleRegion().left;
	while(pos != NULL)
	{
		CVertScaleGroup* pVertScaleGroup = m_realVertScaleGroupList.GetNext(pos);
		assert(pVertScaleGroup != NULL);

		// Y Scale�� ���� ǥ�� ���� - ojtaso (20070807)
		m_pDrawingGraphData->SetVertScaleDisplayAttributes(pVertScaleGroup->GetVertScale().GetVertScalePosition(),
														pVertScaleGroup->IsScaleTextDrawing());
		// ��ǥ ������ - ojtaso (20071023)
		m_pDrawingGraphData->SetVertScaleInvertType(pVertScaleGroup->GetVertScaleInvertType());
		// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
		pVertScaleGroup->DrawGraphs(pDC, m_pDrawingGraphData, preGraphTitleEndPosition, nEndLimit, nGraphTitleEndLine);
	}

	// (2009/10/9 - Seung-Won, Bae) for Thread Draw.
	if( THREAD_DRAWING == m_pMainBlockBaseData->GetXScaleManager()->GetThreadFlag())
	{
		delete m_pDrawingGraphData;
		m_pDrawingGraphData = NULL;
	}
}

// Ÿ��Ʋ �����ٷ� ǥ�� : �������� - ojtaso (20070227)
void CVertScaleGroupList::RecalculateTitleRegion(BOOL bIncreaseLine)
{
	m_drawingRegion.GetGraphRegions().OffsetTitleLine(1);
}