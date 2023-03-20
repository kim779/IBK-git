// BoundMarkerAddInImp.cpp: implementation of the CBoundMarkerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoundMarkerAddInImp.h"

#include <math.h>													// for HUGE_VAL
#include "../Include_Chart/Dll_Load/IMetaTable.h"					// for _MTEXT()
#include "../Include_Chart/IPropertyMap.h"							// for IPropertyMap
#include "../Include_AddIn_381931/I381931/_IChartOCX.h"				// for IChartOCX
#include "../Include_AddIn_381931/I381931/_IChartManager.h"			// for IChartManager
#include "../Include_AddIn_381931/I381931/_IBlock.h"				// for IBlock
#include "../Include_AddIn_381931/I381931/_IGraphInfoManager.h"		// for IGraphInfoManager
#include "../Include_AddIn_381931/I381931/_IGraphInfo.h"			// for IGraphInfo
#include "../Include_AddIn_381931/I381931/_IPacket.h"				// for IPacket
#include "../Include_AddIn_381931/I381931/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn_381931/I381931/_IDoubleList.h"			// for IDoubleList
#include "../Include_AddIn_381931/I381931/_IString.h"				// for ILPCSTR
#include "Conversion.h"												// for CDataConversion
#include "../../COMMONTR/OAQ69901_io.h"					// for _MTEXT()

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
	m_pmapOrder = NULL;
	m_bDraw = TRUE;
}

CBoundMarkerAddInImp::~CBoundMarkerAddInImp()
{
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CBoundMarkerAddInImp::m_strAddInItemName = "BOUND_MARKER_CKSA";

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
	m_strIndicatorName	= szIndicatorName;
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
	if (m_bDraw == FALSE)
		return;
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	m_rctBoundMark.SetRectEmpty();

	if( m_pIChartOCX->IsOnSpecialChart()) return;

	int nR, nC, nV, nG;
	if (m_pIChartManager->GetIndexOfGraph(_MTEXT(C2_PRICE_CHART),  nR, nC, nV, nG) == FALSE)
		return;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( !pIBlock) return;
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( m_rctGraphRegion, rctNoFrameAndMargin);
	m_rctBoundMark.left = m_rctGraphRegion.right - 1; //-2
	m_rctBoundMark.right = m_rctGraphRegion.right + 2;//3		// FillRect�� Drawing ����� �����Ͽ� 1 Pixcel �߰�

	pIBlock->GetVScaleGroupMinMax( nV, m_dViewMin, m_dViewMax);

	if( m_dViewMin == m_dViewMax && m_dViewMax == -HUGE_VAL)
	{
		pIBlock->Release();
		return;
	}

	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);
	// 2.2.2 ���Ѱ� ��ǥ�� ���Ѵ�.
	m_rctGraphDrawingRegion		= pIBlock->GetGraphDrwingRegion();

	COLORREF clrMedo = RGB(0, 0, 255);
	COLORREF clrMesu = RGB(255, 0, 0);
	CPen MedoPen(PS_SOLID, 1, clrMedo);
	CPen MesuPen(PS_SOLID, 1, clrMesu);

	CString strKey;
	POSITION pos;
	ORDER_INFO * pOrerInfo;
	CString	strCode = m_strItemCode;
	pos = m_pmapOrder->GetStartPosition();
	while (pos)
	{
		m_pmapOrder->GetNextAssoc(pos, strKey, (CObject*&)pOrerInfo);

		if (pOrerInfo->m_strseries == strCode)
		{
			CPen * pOldPen = NULL;
			int nBoundCenter = m_pIChartOCX->ConvertDataToYPosition(pOrerInfo->m_dblPrice,
				m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
		
			if (pOrerInfo->m_strbuysellsect == "0")
			{
				pOldPen = (CPen*) pDC->SelectObject(&MesuPen);
				pDC->SetTextColor(clrMesu);
			}
			else
			{
				pOldPen = (CPen*) pDC->SelectObject(&MedoPen);
				pDC->SetTextColor(clrMedo);
			}
		
			if (m_rctGraphDrawingRegion.top <= nBoundCenter && m_rctGraphDrawingRegion.bottom >= nBoundCenter)
			{
				pDC->MoveTo(m_rctGraphDrawingRegion.left, nBoundCenter);
				pDC->LineTo(m_rctGraphDrawingRegion.right, nBoundCenter);
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOut(m_rctGraphDrawingRegion.left, nBoundCenter+2, pOrerInfo->m_strposprice);
			}
			pDC->SelectObject(pOldPen);
		}

	}

	MedoPen.DeleteObject();
	MesuPen.DeleteObject();

	pIBlock->Release();
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
