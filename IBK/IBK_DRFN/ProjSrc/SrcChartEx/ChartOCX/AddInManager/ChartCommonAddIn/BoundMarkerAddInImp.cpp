// BoundMarkerAddInImp.cpp: implementation of the CBoundMarkerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoundMarkerAddInImp.h"

#include <math.h>											// for HUGE_VAL
#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/Conversion.h"					// for CDataConversion
#include "../../Include_Chart/IPropertyMap.h"				// for IPropertyMap
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock
#include "../Include_AddIn/I000000/_IGraphInfoManager.h"	// for IGraphInfoManager
#include "../Include_AddIn/I000000/_IGraphInfo.h"			// for IGraphInfo
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IDoubleList.h"			// for IDoubleList
#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoundMarkerAddInImp::CBoundMarkerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2004.09.30, ��¿�) BoundMark�� �̿����� ���θ� �����޾� ����/�����Ѵ�.
	m_bEnableMark = TRUE;

	// (2004.10.27, ��¿�) View�� ȯ���� �����Ͽ�, CAskingPriceIndicator���� �̿��� �� �ֵ��� �Ѵ�.
	m_bLog			= FALSE;	// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	m_bReverse		= FALSE;	// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	m_dViewMin		= 0.0;		// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	m_dViewMax		= 0.0;		// View���� ��� Graph�� View�� Maximum Scale ���̴�.

	//sy 2005.11.18.
	m_rctBoundMark = CRect(0, 0, 0, 0);
	m_rctGraphRegion = CRect(0, 0, 0, 0);
	m_rctGraphDrawingRegion = CRect(0, 0, 0, 0);
	m_dUpperBound = 0.0;
	m_dLowerBound = 0.0;
	m_dYesterdayClosePrice = 0.0;
	m_bExistenceBoundMark = false;
	//sy end
}

CBoundMarkerAddInImp::~CBoundMarkerAddInImp()
{
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CBoundMarkerAddInImp::m_strAddInItemName = "BOUND_MARKER";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CBoundMarkerAddInImp)
	ONADDINEVENT( OnDrawBeforeGraph)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()

// (2004.09.14, ��¿�) Bound���� �����ϴ� Interface�� �����Ѵ�.
void CBoundMarkerAddInImp::ResetBound( const char *p_szBound)
{
	// 1. ���� Bound ���� �ʱ�ȭ�Ѵ�. (������ �����ϸ�, Clear�ǵ���)
	m_strIndicatorName.Empty();
	//sy 2005.11.21.
	m_bExistenceBoundMark = false;

	// 2. Message�� Ȯ���Ѵ�.
	if( !p_szBound) return;
	if( !*p_szBound) return;

	// 3. Message�� Buffer�� �ű��.
	CString strBoundMark;
	char *szBoundMark = strBoundMark.GetBufferSetLength( strlen( p_szBound));
	strcpy( szBoundMark, p_szBound);

	// 4. ���� Graph Name�� ���Ѵ�.
	char *	szIndicatorName	= szBoundMark;
	char *	szLowerBound	= NULL;
	char *	szUpperBound	= NULL;
	szBoundMark = strchr( szBoundMark, ':');
	if( !szBoundMark) return;
	*szBoundMark = '\0';

	// 5. ���Ѱ� Message�� ���Ѵ�.
	szBoundMark++;
	szUpperBound = szBoundMark;
	szBoundMark = strchr( szBoundMark, ',');
	if( !szBoundMark) return;
	*szBoundMark = '\0';

	// 6. ���Ѱ� Message�� ���Ѵ�.
	szBoundMark++;
	szLowerBound = szBoundMark;
	
	// (2004.10.25, ��¿�) Ȯ��� ������ �־ ������ ���� �ʵ��� �����Ѵ�. (Ȯ�� ������ ������ �����Ѵ�.)
	szBoundMark = strchr( szBoundMark, ':');
	if( szBoundMark)
	{
		*szBoundMark = '\0';
		szBoundMark++;
		m_strExtMsg = szBoundMark;
		// (2004.10.28, ��¿�) �� Space�� �̸� Clear�� �д�.
		m_strExtMsg.TrimRight();
	}

	if( !*szIndicatorName || !*szUpperBound || !*szLowerBound) return;
	// 7. ������ Graph Name, ��/���Ѱ��� ���Ͽ� �������Ѵ�.
	//m_strIndicatorName	= szIndicatorName;
	m_strIndicatorName	= _STOM2(szIndicatorName);

	m_dUpperBound	= atof( szUpperBound);
	m_dLowerBound	= atof( szLowerBound);

	// 8. ������ ��/���Ѱ��� ������ ��츦 �ٷ���´�.
	if( m_dUpperBound < m_dLowerBound)
	{
		double dTemp	= m_dUpperBound;
		m_dUpperBound	= m_dLowerBound;
		m_dLowerBound	= dTemp;
	}

	//sy 2005.11.21.
	// 9. ��/���ѹٰ� ��� ���ΰ� setting �Ǿ����� �����Ѵ�.
	m_bExistenceBoundMark = true;

	//sy 2005.11.18. ��������
	// 10. ��/���ѹ��� �������� �Ǵ� �������� �����Ѵ�.
	SetYesterdayClosePrice(m_strExtMsg);
}

// (2004.09.30, ��¿�) BoundMark�� �̿����� ���θ� �����޾� ����/�����Ѵ�.
void CBoundMarkerAddInImp::EnableBoundMark( BOOL p_bEnable)
{
	m_bEnableMark = p_bEnable;
}

//sy 2005.11.18.
void CBoundMarkerAddInImp::SetYesterdayClosePrice(const CString& strData)
{
	CString strTemp = strData;
	CDataConversion::GetStringData(strTemp, ":");//�����ڵ�
	CDataConversion::GetStringData(strTemp, ":");//??
	CString strPrice = CDataConversion::GetStringData(strTemp, ":");//��������

	m_dYesterdayClosePrice = atof(strPrice);
}

double CBoundMarkerAddInImp::GetYesterdayClosePrice() const
{
	return m_dYesterdayClosePrice;
}

double CBoundMarkerAddInImp::GetSteadyPrice() const
{
	if(m_dYesterdayClosePrice != 0)
		return m_dYesterdayClosePrice;

	return ((m_dUpperBound + m_dLowerBound) / 2.0);
}
//sy end


//////////////////////////////////////////////////////////////////////
// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
//////////////////////////////////////////////////////////////////////

// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
// (2004.09.14, ��¿�) ���Ѱ�/���Ѱ� Mark�� ����ϴ� Routine�� �����Ѵ�.
// (2004.10.25, ��¿�) BoundMark�� ��� ������ �޾� �����Ѵ�. (�̸� ��¹޴� Asking Price Indicator���� �̿��Ѵ�.)
void CBoundMarkerAddInImp::OnDrawBeforeGraph( HDC p_hDC)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	// (2004.10.25, ��¿�) Drawing Skip�� ��츦 ����Ͽ� �̸� Empty Rectangle�� �ʱ�ȭ�Ѵ�.
	m_rctBoundMark.SetRectEmpty();

	// (2004.09.30, ��¿�) BoundMark�� �̿����� ���θ� �����޾� ����/�����Ѵ�.
	if( !m_bEnableMark) return;

	// (2004.09.15, ��¿�) Ư����Ʈ�� ��쿡�� ������ �ʵ��� �Ѵ�.
	if( m_pIChartOCX->IsOnSpecialChart()) return;

	// 0. Bound�� ������ ��츸 �׸���.
	if( m_strIndicatorName.IsEmpty()) return;

	// 1. ������Ʈ�� ���Ե� Block�� Index�� ���Ѵ�.
	// (2004.10.27, ��¿�) Ư�� Graph�� ��ϵ� Block�� Index�� ���ϴ� Interface�� OCX���� �����Ѵ�.
	int nR, nC, nV, nG;
	// (2007/2/1 - Seung-Won, Bae) Receive ReturnValue with ILPCSTR for Memroy Free.
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( m_strIndicatorName, nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return;
	// (2004.09.22, ��¿�) Bound Mark�� Block������ �������� �������� ǥ���Ѵ�.
	// 2. Bound Mark�� �⺻ ��ġ�� Ȯ���Ѵ�.
	// 2.1 (2004.09.23, ��¿�) Bound Mark�� Block�� ���� Border�� �߽����� �¿� 2 Pixel�� ��Ÿ������ �Ѵ�.
	//		�̶� ������ Border Line���� ǥ�õǵ��� Color ������ ó���Ѵ�.
	// (2006/6/16 - Seung-Won, Bae) Use Block Interface
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( !pIBlock) return;
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( m_rctGraphRegion, rctNoFrameAndMargin);
	m_rctBoundMark.left = m_rctGraphRegion.right - 1; //-2
	m_rctBoundMark.right = m_rctGraphRegion.right + 2;//3		// FillRect�� Drawing ����� �����Ͽ� 1 Pixel �߰�
	// 2.2 Bound Mark�� ũ�⸦ ���Ѵ�.
	// 2.2.0 View�� Min/Max ���� ���Ѵ�.
	pIBlock->GetVScaleGroupMinMax( nV, m_dViewMin, m_dViewMax);
	// (2007/1/15 - Seung-Won, Bae) Check No Data in Block
	if( m_dViewMin == m_dViewMax && m_dViewMax == -HUGE_VAL) return;
	// (2004.10.27, ��¿�) Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	// 2.2.1 ������Ʈ�� Block�� Log/������ �������� Ȯ���Ѵ�.
	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);
	// 2.2.2 ���Ѱ� ��ǥ�� ���Ѵ�.
	m_rctGraphDrawingRegion		= pIBlock->GetGraphDrwingRegion();
	m_rctBoundMark.top		= m_pIChartOCX->ConvertDataToYPosition( m_dUpperBound, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
	// 2.2.3 ���Ѱ� ��ǥ�� ���Ѵ�.
	m_rctBoundMark.bottom	= m_pIChartOCX->ConvertDataToYPosition( m_dLowerBound, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
	pIBlock->Release();
	pIBlock = NULL;

	// (2004.10.27, ��¿�) Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	//		'������' �����̸� Bound Mark�� ������ ���´�. (�Ʒ��� ���� �ٲ� ����ϸ� �ٷ� ���δ�.)
	if( m_bReverse)
	{
		int nTemp				= m_rctBoundMark.top;
		m_rctBoundMark.top		= m_rctBoundMark.bottom;
		m_rctBoundMark.bottom	= nTemp;
	}

	// 3. Bound Mark�� ����Ѵ�.
	// 3.0 ���� Box�� �׷��� ��Ȳ���� Ȯ���Ѵ�.
	// (2004.10.01, ��¿�) Drawing ������ GraphRegion�� �ƴ϶� BlockRegion���� Ȯ���Ѵ�.
	//		(GraphRegion�� BlockReion�� ���Ͽ� �ణ�� ������ �ִµ�, �� ������ �ΰ� �׸��� �ʴ� ��Ȳ�� �߻����� �ʵ����Ѵ�.)
	if( m_rctGraphRegion.bottom <= m_rctBoundMark.top || m_rctBoundMark.bottom <= m_rctGraphDrawingRegion.top)
	{
		m_rctBoundMark.SetRectEmpty();
		return;
	}
	// 3.1 Bound Mark�� ����/������ �����Ѵ�. (Block�� ��ġ�� �ʵ���)
	//	Ư��, ������ �� �������� Graph Region�� �ƴ� Block Region���� �Ͽ�, �߸��� Ȯ���� �� �ֵ��� �Ѵ�.
	if( m_rctBoundMark.top < m_rctGraphRegion.top)			m_rctBoundMark.top = m_rctGraphRegion.top;
	if( m_rctGraphRegion.bottom < m_rctBoundMark.bottom)	m_rctBoundMark.bottom = m_rctGraphRegion.bottom;

	// 3.2 Bound Mark�� �߽��� ���Ͽ� ��°� �϶� Mark�� �������� ��´�.
	// (2004.10.27, ��¿�) Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	int nBoundCenter = m_pIChartOCX->ConvertDataToYPosition(GetSteadyPrice(), //sy 2005.11.18.
		m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
	// (2004.09.23, ��¿�) ���/�϶�/������ ������ ���Ͽ� Bound Mark�� ǥ���Ѵ�.
	// 3.2.5.1 �⺻ ������ �����Ѵ�.
	COLORREF clrUp		= RGB( 255,	0, 0);
	COLORREF clrDown	= RGB( 0,	0, 255);
	COLORREF clrSteady	= RGB( 0,	0, 0);
	// 3.2.5.2 ������Ʈ �������� ��ȸ�Ѵ�.
	IGraphInfoManager *pIGraphInfoManager = m_pIChartOCX->GetIGraphInfoManager();
	if( pIGraphInfoManager)
	{
		// (2006/9/20 - Seung-Won, Bae) Use IGraphInfo
		IGraphInfo *pIGraphInfo = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, m_strIndicatorName);
		if( pIGraphInfo)
		{
			clrUp		= pIGraphInfo->GetSubGraphColorWithZBI( 0, 0);
			clrDown		= pIGraphInfo->GetSubGraphColorWithZBI( 0, 2);
			clrSteady	= pIGraphInfo->GetSubGraphColorWithZBI( 0, 4);
			
			pIGraphInfo->Release();
		}
		pIGraphInfoManager->Release();
	}

	// (2004.10.27, ��¿�) Block�� Log/������ ������ Ȯ���Ͽ� ��Ȯ�ϰ� Data�� ��ȯ�Ѵ�. (����<->Y Pos)
	//		��±����� �϶������� ������ ���, ���� ������ �����Ѵ�. (������ ������ �̹� ������ ���Ҵ�.)
	if( m_bReverse)
	{
		COLORREF clrTemp	= clrDown;
		clrDown				= clrUp;
		clrUp				= clrTemp;
	}

	// 3.3 �϶� Mark�� ����Ѵ�.
	// (2004.10.01, ��¿�) Drawing ������ GraphRegion�� �ƴ϶� BlockRegion���� Ȯ���Ѵ�.
	//		(GraphRegion�� BlockReion�� ���Ͽ� �ణ�� ������ �ִµ�, �� ������ �ΰ� �׸��� �ʴ� ��Ȳ�� �߻����� �ʵ����Ѵ�.)
	if( nBoundCenter < m_rctGraphRegion.bottom)
	{
		// 3.3.1 �϶� Mark�� ũ�⸦ ���Ѵ�.
		CRect rctDownMark( m_rctBoundMark);
		if( rctDownMark.top < nBoundCenter) rctDownMark.top = nBoundCenter;
		// 3.3.2 �϶� Mark�� ����Ѵ�.
		//	(FillRect�� Right, Bottom�� Pixel�� Drawing���� �ʴ� ���� �����Ͽ� Bottom�� 1 Pixel ������Ų��.)
		CBrush brDownMark( clrDown);
		rctDownMark.bottom++;
		pDC->FillRect( &rctDownMark, &brDownMark);
	}

	// 3.4 ��� Mark�� ����Ѵ�.
	// (2004.10.01, ��¿�) Drawing ������ GraphRegion�� �ƴ϶� BlockRegion���� Ȯ���Ѵ�.
	//		(GraphRegion�� BlockReion�� ���Ͽ� �ణ�� ������ �ִµ�, �� ������ �ΰ� �׸��� �ʴ� ��Ȳ�� �߻����� �ʵ����Ѵ�.)
	if( m_rctGraphRegion.top < nBoundCenter)
	{
		// 3.4.1 ��� Mark�� ũ�⸦ ���Ѵ�.
		CRect rctUpMark( m_rctBoundMark);
		if( nBoundCenter < rctUpMark.bottom) rctUpMark.bottom = nBoundCenter;
		// 3.3.2 ��� Mark�� ����Ѵ�.
		//	(FillRect�� Right, Bottom�� Pixel�� Drawing���� �ʴ� ���� �����Ͽ� Bottom�� 1 Pixel ������Ų��.)
		CBrush brUpMark( clrUp);
		rctUpMark.bottom++;
		pDC->FillRect( &rctUpMark, &brUpMark);
	}

	// 3.5 ���� Line�� ����Ѵ�.
	// (2004.10.01, ��¿�) Drawing ������ GraphRegion�� �ƴ϶� BlockRegion���� Ȯ���Ѵ�.
	//		(GraphRegion�� BlockReion�� ���Ͽ� �ణ�� ������ �ִµ�, �� ������ �ΰ� �׸��� �ʴ� ��Ȳ�� �߻����� �ʵ����Ѵ�.)
	if( nBoundCenter < m_rctGraphRegion.bottom && m_rctGraphRegion.top < nBoundCenter)
	{
		CPen pnSteady( PS_SOLID, 1, clrSteady);
		CPen *ppnOld = pDC->SelectObject( &pnSteady);
		pDC->MoveTo( m_rctBoundMark.left,	nBoundCenter);
		pDC->LineTo( m_rctBoundMark.right,	nBoundCenter);
		pDC->SelectObject( ppnOld);
	}

	// 4. ���簡�� ���Ͽ� Mark�Ѵ�.
	// 4.1 ���� Data List�� ���Ѵ�.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE));
	if( pIPacket)
	{
		IDoubleList *pIDoubleList = pIPacket->GetDataList();
		pIPacket->Release();
		if( pIDoubleList)
		{
			// (2004.12.17, ��¿�) GetTail()�ÿ� ������ ���� �ʵ��� ������ Ȯ���Ѵ�.
			if( pIDoubleList && !pIDoubleList->IsEmpty())
			{
				// 4.2 ������ Marking Position�� ���Ѵ�.
				double dClose;
				pIDoubleList->GetTail( dClose);
				int nCloseX = m_rctBoundMark.right;
				int nCloseY = m_pIChartOCX->ConvertDataToYPosition( dClose, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
				// 4.3 ���� ǥ�ø� �� �� �ִ� ��Ȳ���� Ȯ���Ѵ�.
				if( m_rctBoundMark.top <= nCloseY && nCloseY <= m_rctBoundMark.bottom)
				{
					CPoint ptClose[ 3];
					ptClose[ 0].x = nCloseX;
					ptClose[ 0].y = nCloseY;
					ptClose[ 1].x = nCloseX	+ 5;
					ptClose[ 1].y = nCloseY	+ 5;
					ptClose[ 2].x = nCloseX	+ 5;
					ptClose[ 2].y = nCloseY	- 5;

					// (2004.12.14, ��¿�) ���簡 ǥ�� ������ ǥ�� ��ġ (���/�϶�)�� ���� �޸��Ѵ�.(�⺻:����)
					COLORREF clrEndMark = clrSteady;
					//sy 2005.11.18.
					//double dSteady = ( m_dUpperBound + m_dLowerBound) / 2;
					double dSteady = GetSteadyPrice();
					//sy end
					if( dSteady < dClose)		clrEndMark = clrUp;
					else if( dClose < dSteady)	clrEndMark = clrDown;

					CPen	pnEndMark( PS_SOLID, 1, clrEndMark);
					CBrush	brEndMark( clrEndMark);

					CPen *ppnOld	= pDC->SelectObject( &pnEndMark);
					CBrush *pbrOld	= pDC->SelectObject( &brEndMark);

					pDC->Polygon( ptClose, 3);

					pDC->SelectObject( ppnOld);
					pDC->SelectObject( pbrOld);
				}
			}
			pIDoubleList->Release();
		}
	}
}

// RQ�� Packet : �������� - ojtaso (20070111)
void CBoundMarkerAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// 0. NULL Pointer�� Ȯ���Ѵ�.
	if( !m_pIDefaultPacketManager || strcmp( lpszRQ, "DEFAULT")) return;

	// (2004.10.13, ��¿�) �켱 �����Ѱ� ���� Message�� ������ �������� Ȯ���Ѵ�.
	// (2004.10.22, ��¿�) ����/���Ѱ� ǥ�� ���� ���� ���Ϻ� �ʱ�ȭ �����
	//			��� ���� ��Ʈ�� �ִ� ������ �ʱ�ȭ TR�� ���Ž����� �ش��Ѵ�.
	//		�� ��ɵ��� ���� ������Ʈ�� ����/������ ����/������ ����Ʈ�� �����ϸ鼭,
	//			��/���Ѱ��� ǥ���ϰų� ���� ���� ǥ�ø� Reset ��Ű�� ����� ó���ϴ� ���̴�.
	//		Ư�� �� ��ɵ��� ��Ʈ�� 1���� Graph���� ������ �� �ִ� �������
	//			(����� Data ������ AddIn �� 1���� ���ѵ� ������� �����̴�.)
	//			���� Packet�� �̿��ϴ� ������Ʈ���� �����Ǵ� ������� �����Ѵ�.
	//		*. ���� �ڼ��� ������ �� Class Header ����
	if( !( BOOL)strstr( p_szPacketNames, CString( _MTEXT( C0_CLOSE)) + ";") || !p_bWithReset) return;

	// (2004.09.14, ��¿�) �����Ѱ� ǥ�ø� ���� ���� Ȯ���Ѵ�.
	// (2004.10.04, ��¿�) �����Ѱ� ǥ�ô� ������Ʈ�� ���� ����Ǵ� ��쿡�� �ݿ��Ѵ�.
	// (2004.10.14, ��¿�) �����Ѱ��� ������ ���߾ �����Ѵ�. (Clear ȿ��)
	ILPCSTR szHelpMsg = NULL;
	m_pIDefaultPacketManager->GetHelpMessageItem( "BOUNDMARK", szHelpMsg);
	// (2004.10.22, ��¿�) ��/���Ѱ� ǥ�ñ�ɿ� ȣ������ ����� �����Ѵ�.
	ResetBound( szHelpMsg);
}

// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
void CBoundMarkerAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	const char *szEnvValue = NULL;
	if( !p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue)) return;
	m_bEnableMark = ( atoi( szEnvValue) == 1);
}
void CBoundMarkerAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	const char *szEnvValue = NULL;
	if( p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue)) return;

	CString strEnvValue;
	strEnvValue.Format( "%d", m_bEnableMark ? 1 : 0);
	p_pIAddInEnvMap->SetAt( m_strAddInItemName, strEnvValue);
}
