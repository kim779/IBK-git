// ChartStockOpinionAddInImp.cpp: implementation of the CChartDragAnalAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartPeriodSummaryAddIn.h"
#include "ChartDragAnalAddInImp.h"
#include "resource.h"

#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h
#include "./Include_Chart/MessageDefine.h"			// for UM_CURSOR_EXCHANGE
#include "./Include_Chart/OutsideCommonInterface.h"		// for CMChartInterface

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartDragAnalAddInImp::CChartDragAnalAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// save point for inflate when mouse drag & drop
	// 09.21.2001
	m_ptStartDragDrop = CPoint(-1, -1);
	m_ptCurrDragDrop = CPoint(-1, -1);

	m_ptStart = CPoint(-1, -1);
	m_ptEnd = CPoint(-1, -1);

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = UINT_MAX;

	// ( 2006 / 11 / 27 - Sang-Woo, Cho ) ADDIN�� ���࿩��.
	m_bRun = TRUE;
	m_nStartIndex = -1;
	m_nEndIndex = -1;
}

CChartDragAnalAddInImp::~CChartDragAnalAddInImp()
{
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartDragAnalAddInImp::m_strAddInItemName = "DRAG_ZOOM_ANAL";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartDragAnalAddInImp)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnPacketData)
	ONADDINEVENT( OnResetChart)
	// �������� Real�ݿ� : �������� - ojtaso (20070131)
	ONADDINEVENT( OnUpdatePacketDataMultiItem)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
//////////////////////////////////////////////////////////////////////

// (2006/2/6 - Seung-Won, Bae) Draw Trace Rectangle
CRect CChartDragAnalAddInImp::DrawDragRect(CDC * pdc, CPoint ptFirst, CPoint ptCurr, int nWeight)
{
	CRect rccurr;
	rccurr.SetRectEmpty();
	if( ptCurr == CPoint(-1, -1)) return rccurr;

	// ����� �� �簢�� �����
	CPoint ptStart, ptEnd;
	ptStart.x = ptFirst.x < ptCurr.x ? ptFirst.x : ptCurr.x;
	ptStart.y = ptFirst.y < ptCurr.y ? ptFirst.y : ptCurr.y;
	ptEnd.x = ptFirst.x > ptCurr.x ? ptFirst.x : ptCurr.x;
	ptEnd.y = ptFirst.y > ptCurr.y ? ptFirst.y : ptCurr.y;

	rccurr = CRect(ptStart, ptEnd);
	CRect rc = m_pIChartOCX->GetOcxRegion();
	rc = m_pIChartManager->GetGraphRegionSumInColumn(rc.CenterPoint());
	rccurr.top = rc.top;
	rccurr.bottom = rc.bottom;

	SIZE size;
	size.cx = nWeight; size.cy = nWeight;

	pdc->DrawDragRect(rccurr, size, NULL, size);
	
	return rccurr;
}

// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
RECT CChartDragAnalAddInImp::DrawCursor( HDC p_hDC)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	CDC *pDC = CDC::FromHandle( p_hDC);
	return DrawDragRect( pDC, m_ptStartDragDrop, m_ptCurrDragDrop, 4);
}

BOOL CChartDragAnalAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	// ( 2006 / 11 / 27 - Sang-Woo, Cho ) 
	if( !m_bRun ) return FALSE;

	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->IsLButtonExclusive( m_pIAddInDllBase))
	{
		if( p_dwMouseActionID != m_dwMouseActionID) return FALSE;
		if( !m_pIAddInToolMgr->GetLButtonExclusive( m_pIAddInDllBase)) return FALSE;
		m_ptCurrDragDrop = m_ptStartDragDrop;
	}

	if( m_ptStartDragDrop == point)	return TRUE;

	// Drag��
	m_ptCurrDragDrop = point;
	return TRUE;
}

BOOL CChartDragAnalAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	// ( 2006 / 11 / 27 - Sang-Woo, Cho ) 
	if( !m_bRun )
		return FALSE;

	// (2006/2/13 - Seung-Won, Bae) Check Empty Chart!
	//if( !m_pIChartOCX->IsUseToolScrollBar() || m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty()) return FALSE;
	if( m_pIChartOCX->IsOnSpecialChart() || m_pIChartManager->IsEmpty()) return FALSE;
	CRect rctRegion = m_pIChartManager->GetGraphRegionSumInColumn( point);
	if( rctRegion.IsRectEmpty()) return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	m_dwMouseActionID = p_dwMouseActionID;

	m_ptStartDragDrop = point;
	m_ptCurrDragDrop = CPoint(-1, -1);

	return TRUE;
}

BOOL CChartDragAnalAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	// ( 2006 / 11 / 27 - Sang-Woo, Cho ) 
	if( !m_bRun )
		return FALSE;

	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return FALSE;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
	if( !m_pIAddInToolMgr->ReleaseLButtonExclusive( m_pIAddInDllBase)) return FALSE;

	// save point & draw for inflate when mouse drag & drop
	// 09.21.2001	
	// MODIFY: ����(04/04/18) DoubleClick-Drag�� ZoomIn����� ����.
	// (2006/3/4 - Seung-Won, Bae) Make Real Effect Gap 5 Pixcel.
	if( 5 <= abs(m_ptStartDragDrop.x - point.x))
	{
		//nami 8.30
		m_pChartCWnd->SendMessage( UM_CURSOR_EXCHANGE, ( WPARAM)CCursorShape::OBJECT_NONE, NULL);

		m_ptStart.x = m_ptStartDragDrop.x < m_ptCurrDragDrop.x ? m_ptStartDragDrop.x : m_ptCurrDragDrop.x;
		m_ptEnd.x = m_ptStartDragDrop.x > m_ptCurrDragDrop.x ? m_ptStartDragDrop.x : m_ptCurrDragDrop.x;

		// ��Ʈ�� �⺻���� --------------------------------------------------------------------->>
		int nStartIndex = 0, nEndIndex = 0;
		int		nR, nC;									// ������Ʈ�� ���Ե� Block�� Index
		CRect	rctBlockRegion, rctGraphRegion;			// Frame ����, Graph ����
		GetChartBasicData( nStartIndex, nEndIndex, nR, nC, rctBlockRegion, rctGraphRegion);
		m_nStartIndex = GetIndexFromPoint(m_ptStart.x, nStartIndex, nEndIndex, rctGraphRegion);
		m_nEndIndex = GetIndexFromPoint(m_ptEnd.x, nStartIndex, nEndIndex, rctGraphRegion);
		m_pIChartOCX->InvalidateControl();

		CMChartInterface *pMChartInterface = m_pIChartOCX->GetMChartInterface();
		if( pMChartInterface)
		{
			AddData(m_nStartIndex, m_nEndIndex);
			pMChartInterface->SetDataWindowData(&m_listDataWindow);
		}
	}

	m_ptStartDragDrop = CPoint(-1, -1);
	m_ptCurrDragDrop = CPoint(-1, -1);
	return TRUE;
}
void CChartDragAnalAddInImp::OnDrawAfterGraph(HDC p_hDC)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	CDC *pDC = CDC::FromHandle( p_hDC);

	if(m_nStartIndex == m_nEndIndex)
		return;
	if(m_nStartIndex == -1 || m_nEndIndex == -1)
		return;

	// ��Ʈ�� �⺻���� --------------------------------------------------------------------->>
	int nStartIndex = 0, nEndIndex = 0;
	int		nR, nC;									// ������Ʈ�� ���Ե� Block�� Index
	CRect	rctBlockRegion, rctGraphRegion;			// Frame ����, Graph ����
	GetChartBasicData( nStartIndex, nEndIndex, nR, nC, rctBlockRegion, rctGraphRegion);

	int		nDataOnePage = nEndIndex - nStartIndex + 1;								// �� Data View�� Data ����
	double	dCandleWidth = double( rctGraphRegion.Width()) / double( nDataOnePage);	// ���� ���� ���Ѵ�

	m_ptStart.x = GetXPositoin(m_nStartIndex-nStartIndex, dCandleWidth, rctGraphRegion);
	m_ptEnd.x = GetXPositoin(m_nEndIndex-nStartIndex, dCandleWidth, rctGraphRegion);

	if(m_ptStart.x<m_ptEnd.x)
	{
		m_ptEnd.x +=1;
		CRect rc = m_pIChartOCX->GetOcxRegion();
		rc = m_pIChartManager->GetGraphRegionSumInColumn(rc.CenterPoint());
		m_ptStart.y = rc.top;
		m_ptEnd.y = rc.bottom;
		DrawFill(pDC, m_ptStart, m_ptEnd, CRect(m_ptStart, m_ptEnd));
	}
}

CRect CChartDragAnalAddInImp::DrawFill(CDC *pDC, const CPoint &Startpt, const CPoint &Endpt, const CRect& drawingRegion)
{
	CDC dcCompatible;
    CBitmap bm;
	//��Ʈ��, DC ����.
    dcCompatible.CreateCompatibleDC(pDC);
    bm.CreateCompatibleBitmap(pDC, Endpt.x - Startpt.x, Endpt.y - Startpt.y);
    CBitmap *pBitmapOld = dcCompatible.SelectObject(&bm);

    dcCompatible.FillSolidRect(CRect(0, 0, Endpt.x - Startpt.x, Endpt.y - Startpt.y), RGB(230,230,230)); //RGB(0,200,255));//RGB(0x7F, 0x7F, 0x7F)

	dcCompatible.Draw3dRect(0, 0, Endpt.x - Startpt.x, Endpt.y - Startpt.y, RGB(0,0,0),RGB(0,0,0));

    SelectClipRegion(pDC, drawingRegion);
    pDC->BitBlt(Startpt.x, Startpt.y, Endpt.x - Startpt.x, Endpt.y - Startpt.y, &dcCompatible, 0, 0, SRCAND);

	dcCompatible.SelectObject(pBitmapOld);
	bm.DeleteObject();
	dcCompatible.DeleteDC();

	return CRect(Startpt.x, Startpt.y, Endpt.x, Endpt.y);
}

void CChartDragAnalAddInImp::SelectClipRegion(CDC* pDC, const CRect& rect)
{
	CRgn rgn;
	rgn.CreateRectRgn((int)rect.left, (int)rect.top, (int)rect.right, (int)rect.bottom);
	pDC->SelectClipRgn(&rgn);
}

// �׸��� �����⺻������ ������.
void CChartDragAnalAddInImp::SelectClipRegion(CDC* pDC)
{
	pDC->SelectClipRgn(NULL);
}

BOOL CChartDragAnalAddInImp::GetChartBasicData(int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion)
{
	// �� Data View�� Start/End Index�� ���Ѵ�.
	if( !m_pIChartOCX->GetViewStartEndIndex( p_nStartIndex, p_nEndIndex)) return FALSE;
	int nV, nG;
	

	// ������Ʈ�� ���Ե� Block�� Index�� ���Ѵ�.
	if( !m_pIChartManager->GetIndexOfGraph( "������Ʈ", p_nR, p_nC, nV, nG)) return FALSE;
	
	// Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	IBlock *pIBlock = m_pIChartManager->GetBlock(p_nR, p_nC);
	if( pIBlock)
		{
			pIBlock->GetLogAndReverseScaleFlag( m_bLog, m_bReverse);
			pIBlock->GetVScaleGroupMinMax(nV, m_dViewMin, m_dViewMax);
	   		pIBlock->GetGraphRegions(p_rctGraphRegion, p_rctBlockRegion );

			// Scale�� ���� offset ���//////////////////////////////////
			p_rctGraphRegion = pIBlock->GetGraphDrwingRegion();
			//////////////////////////////////////////////////////

			pIBlock->Release();
			pIBlock = NULL;

		}
	return TRUE;
}

int CChartDragAnalAddInImp::GetIndexFromPoint(int x, int nStartIndex, int nEndIndex, CRect rctBlockRegion)
{
	int nBlockWidth = rctBlockRegion.Width();
	int nRet = nStartIndex + ( ( x - rctBlockRegion.left) * ( nEndIndex - nStartIndex + 1)) / nBlockWidth;
	if(nRet > nEndIndex)
		nRet = nEndIndex;
	if(nRet < nStartIndex)
		nRet = nStartIndex;
	return nRet;
}

//////////////////////////////////////////////////////////////////////////
// �����ͷκ��� �γؽ� ���
int CChartDragAnalAddInImp::GetIndexFromValue(CString strTime, int nStartIndex, int nEndIndex)
{
	IPacket *pIPacket = m_pIPacketManager->GetPacket( "�ڷ�����");
	int lx = 0;
	try
	{
		ILPCSTR is = pIPacket->GetType();
		CString s = is;
		if(s == "YYYYMMDD")
		{
			lx = atol(strTime.Left(8));
		}
		else if(s == "YYYYMM")
		{
			lx = atol(strTime.Left(6));
		}
		else if(s == "MMDDHHMM")
		{
			if(strTime.GetLength()>=12)
				lx = atol(strTime.Mid(4,8));
		}
		double dTime;
		for(int i=0; i <= nEndIndex ;i++)
		{
			if( !pIPacket->GetData( i, dTime)) throw this;
			if(s == "YYYYMM" && dTime >= 1000000)
			{
				dTime = dTime/100;
			}
			if(lx == ( long)dTime)	// ������...
			{
				pIPacket->Release();
				return (i);
			}
			else if(lx < ( long)dTime)
			{
				pIPacket->Release();
				return -1;
			}
		}
		
	}
	catch( CChartDragAnalAddInImp *)
	{
		pIPacket->Release();
		return -1;
	}

	pIPacket->Release();
	return -1; 
}

//////////////////////////////////////////////////////////////////////////
// �־��� index �� ��ȿ�� ������ ���� �Ǵ�
BOOL CChartDragAnalAddInImp::AddData(int nStart, int nEnd)
{
	IPacket *pIPacketForCur = m_pIPacketManager->GetPacket( "����");
	try
	{
		m_listDataWindow.RemoveAll();
		double dCur=0;
		CString strClose;
		for(int i=nStart; i<=nEnd; i++)
		{
			pIPacketForCur->GetData( i, dCur);
			strClose.Format("%.0f", dCur);
			m_listDataWindow.AddTail(strClose);
		}
	}
	catch( CChartDragAnalAddInImp *)
	{
		pIPacketForCur->Release();
		return FALSE;
	}
	pIPacketForCur->Release();
	return TRUE;
}

int CChartDragAnalAddInImp::GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion)
{
	int	nLeftBorder, nRightBorder, nDataOffset, nXPos, nAdd;
	double dDecimalFrac;

	nDataOffset = int(dDataOffset);
	dDecimalFrac = dDataOffset - nDataOffset;

	if((int)dCandleWidth > 2)
	{
		// ���� �°�踦 ���Ѵ�. (�� ���� �������� 1 Pixcel(����) �����̴�.)
		nLeftBorder = rctGraphRegion.left + int( dCandleWidth * double( nDataOffset) + 0.5) + 1;
		
		// ���� ���踦 ���Ѵ�. (���� �� ���� �°�躸�� 1 Pixcel(����) �����̴�.)
		nRightBorder = rctGraphRegion.left + int( dCandleWidth * double( nDataOffset + 1) + 0.5) - 1;
		// �Ҽ����� ���� �߰��� Pixel
		nAdd = int(dDecimalFrac * dCandleWidth);

		// ���� �߽��� ��´�. (�������� ����Ѵ�.)
		nXPos = ( nLeftBorder + nRightBorder) / 2 + nAdd;
	}
	else
	{
		nXPos = rctGraphRegion.left + int( dCandleWidth * double( nDataOffset + 1) + 0.5);
	}

	if( nXPos > rctGraphRegion.right)
		nXPos = rctGraphRegion.right;
	if( nXPos < rctGraphRegion.left)
		nXPos = rctGraphRegion.left;
	return nXPos;
}

void CChartDragAnalAddInImp::OnResetChart()
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	//////////////////////////////////////////////////////////////////////////
	m_nStartIndex = -1;
	m_nEndIndex = -1;
	//////////////////////////////////////////////////////////////////////////
}
void CChartDragAnalAddInImp::OnPacketData(const char *p_szPacketNames, BOOL p_bWithReset)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	//////////////////////////////////////////////////////////////////////////
	m_nStartIndex = -1;
	m_nEndIndex = -1;
	//////////////////////////////////////////////////////////////////////////
}

BOOL CChartDragAnalAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());
	
	int nCommandType = atoi(p_szCommandName);
	
	switch(nCommandType)
	{
	case 0:	// ��¥ ����
		{
			CString strData = p_szData;
			strData += ":";
			int iStart = 0;
			CString strStartDate = TokenizeEx(strData, ":",iStart);		// ���۳�¥
			CString strEndDate = TokenizeEx(strData, ":",iStart);	// ����¥
			int nStartIndex, nEndIndex;
			if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex))
				return 0;
			m_nStartIndex = GetIndexFromValue(strStartDate, nStartIndex, nEndIndex);
			m_nEndIndex = GetIndexFromValue(strEndDate, nStartIndex, nEndIndex);
			m_bRun = FALSE;
		}
		break;
	case 1:	// draw
		{
		}
		break;
	}

	return TRUE;
}

CString CChartDragAnalAddInImp::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
{
	CString strRes = _T("");
	int iCur = iStart;
	iStart = strSrc.Find(pszTokens, iCur);
	if(iStart != -1)
	{
		strRes = strSrc.Mid(iCur, iStart-iCur);
		iStart++;
		return strRes;
	}
	return strRes;
}

void CChartDragAnalAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
}

void CChartDragAnalAddInImp::OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
{
	if(p_bTrUpdate) return;
}