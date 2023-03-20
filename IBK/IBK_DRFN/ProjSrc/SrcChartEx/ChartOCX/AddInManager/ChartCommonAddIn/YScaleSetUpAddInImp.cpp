// YScaleSetUpAddInImp.cpp: implementation of the CYScaleSetUpAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "YScaleSetUpAddInImp.h"

#include <MATH.H>

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/Conversion.h"					// for CDataConversion
#include "../../Include_Chart/IPropertyMap.h"				// for IPropertyMap
#include "../Include_AddIn/AddInCommand.h"					// for EAI_CUSTOM_ENABLE_BOUNDMARK
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IDoubleList.h"			// for IDoubleList
#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock


#include "../Include_AddIn/_resource.h"						// for ID_SCI_SAVE_SCREEN
#include "DlgYScaleSetUp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYScaleSetUpAddInImp::CYScaleSetUpAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_eYScaleSetupApplyState = YSCALE_APPLY_STATE::APPLY_CANCEL;

	m_strStockCode = "000000";
	m_strPrevStockCode = "";
	
	m_strFormat = "%.f";

	// ������ / ���� ( 0 : ������, 1 : ���� )
	m_nYScaleMaxMinSetupType = 0;

	// ���� - �ִ� / �ּҰ�
	m_dYScaleMaxValue = 0;
	m_dYScaleMinValue = 0;

	// ���� - �ִ� / �ּҰ�
	m_dYScaleMaxPercentValue = 30;
	m_dYScaleMinPercentValue = -30;
}


CYScaleSetUpAddInImp::~CYScaleSetUpAddInImp()
{
}

// ( 2006 / 11 / 16 - Sang-Woo, Cho ) - Multiple Item in DLL
CString CYScaleSetUpAddInImp::m_strAddInItemName = "PRICE_YSCALE_SETUP";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CYScaleSetUpAddInImp)
	ONADDINEVENT( OnToolCommand)	
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_char)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()

// RQ�� Packet : �������� - ojtaso (20070111)
void CYScaleSetUpAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	if( !m_pIDefaultPacketManager || strcmp( lpszRQ, "DEFAULT")) return;

	// 1. ������Ʈ�� ���Ե� Block�� Index�� ���Ѵ�.
	// Ư�� Graph�� ��ϵ� Block�� Index�� ���ϴ� Interface�� OCX���� �����Ѵ�.
	ILPCSTR szHelpMsg = NULL;
	// 20081016 ����ǥ >>
	BOOL bSuccess = m_pIDefaultPacketManager->GetHelpMessageItem( "BOUNDMARK", szHelpMsg );
	if(bSuccess)
	{
		// 2. �����޼����� AddIn�� ���� ������ �����Ѵ�.
		ResetYScaleSetUp( szHelpMsg );
	}
	else
	{		
		bSuccess = m_pIDefaultPacketManager->GetHelpMessageItem( "PREVPRICE", szHelpMsg );
		if(bSuccess)
		{ 
			CString strYScale;
			CString strData, strPacket, strPacketName, strValue;
			strData.Format("%s", szHelpMsg);
			int nPos = 0, nIdx = 0;
			while(nPos != -1)
			{
				nPos = strData.Find("|");
				if( nPos == -1)
				{
					strPacket = strData;
					strData.Empty();
				}
				else
				{
					strPacket = strData.Mid(0, nPos);
					strData = strData.Mid(nPos + 1);
				}
				nPos = strPacket.Find(":");
				strPacketName = strPacket.Mid(0, nPos);
				strValue = strPacket.Mid(nPos + 1);
				strValue.TrimLeft();strValue.TrimRight();
				if(strPacketName.Compare(_MTOL0(_MTEXT(C0_CLOSE))) == 0)
				{
					m_strGraphName = strPacketName;
					m_dYesterdayClosePrice = atof(strValue);
					break;
				}
			}
		}
	}
	// 20081016 ����ǥ <<
	
	// 2. �����޼����� AddIn�� ���� ������ �����Ѵ�.
//	ResetYScaleSetUp( szHelpMsg );

	// YScale�� ������ ���� ���ų� ����� �����̸� �ƹ��� �۾��� ���� �ʴ´�.
	if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_CANCEL )
	{
		m_strPrevStockCode = m_strStockCode;
		return;
	}

	int nR, nC, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, m_nVertScaleGroupIndex, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return;

	// Use Block Interface
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock) return;

	// 3. ���� ��ȸ ������ ������ ���� ���� ������ �Է��Ѵ�.
	if( m_strPrevStockCode == "" )		m_strPrevStockCode = m_strStockCode;

	if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_PERCENT )
	{
		m_dYScaleMaxValue =	m_dYesterdayClosePrice * ( 1 + m_dYScaleMaxPercentValue / 100 );
		m_dYScaleMinValue =	m_dYesterdayClosePrice * ( 1 + m_dYScaleMinPercentValue / 100 );
	}
	
	// 4. TR�� ���� ������ ���� Code�� �ٲ��
	if( m_strStockCode != m_strPrevStockCode )
	{
		// ������ �����̸� Lock�� Ǯ�� VERT_MINMAX_OF_VIEW ���·� ���ư���.
		if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_VALUE )
		{
			if( pIBlock )
				pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, FALSE,
												 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_VIEW,
												 m_dYScaleMinValue, m_dYScaleMaxValue);
			
			m_eYScaleSetupApplyState = YSCALE_APPLY_STATE::APPLY_CANCEL;
		}

		// �������� �����̸� Lock�� �ɰ� VERT_MINMAX_OF_USER ���·� �����Ѵ�.
		else if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_PERCENT )
		{
			if( pIBlock )
				pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, TRUE,
												 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
												 m_dYScaleMinValue, m_dYScaleMaxValue);
		}
		 
		m_strPrevStockCode = m_strStockCode;
	}

	// ���� ������ ���
	else
	{
		// YScale�� ������ ���� ���ų� ����� �����̸� �ƹ��� �۾��� ���� �ʴ´�.
		if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_CANCEL )
			return;

		// Lock�� �ɰ� VERT_MINMAX_OF_USER ���·� �����Ѵ�.
		if( pIBlock )
			pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, TRUE,
											 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
											 m_dYScaleMinValue, m_dYScaleMaxValue);
	}

	if( pIBlock )	pIBlock->Release();
	pIBlock = NULL;

	// OCX�� �ٽ� �׷��ش�.
	m_pIChartOCX->InvalidateControl();
}

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CYScaleSetUpAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( p_nToolType != CToolOptionInfo::T_PRICE_YSCALE_SETUP) return;
	InvokeAddInStr( "YSCALE_SETUP_LOAD", "");
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 16
// Return BOOL	: 
// Param  LPCTSTR p_szCommandName : 
// Param  LPCTSTR p_szData : 
// Comments		: Message�� ������ ���� Dialog�� ����.
//-----------------------------------------------------------------------------
BOOL CYScaleSetUpAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	// 1. "LOAD_YSCALE_SETUP" Message�� ������ ���� dialog�� ����.
	if( stricmp( p_szCommandName, "YSCALE_SETUP_LOAD")) return FALSE;

	// 1.1 Block ������ ���´�.
	int nR, nC, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, m_nVertScaleGroupIndex, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	// 1.2 Use Block Interface
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock) return FALSE;

	// 1.3 AddIn�� ���� ������ ������ �� ������ Return �Ѵ�.
	if( !SetYScaleInfo( pIBlock ) )
	{
		if( pIBlock )	pIBlock->Release();
		pIBlock = NULL;
		return FALSE;
	}

	// 1.4 ���� dialog�� �����ϰ� ������ Setting�Ѵ�.
	UINT uDlgID = IDD_YSCALE_SETUP_HTS;
	if( CChartInfo::WORLD_ON == m_eChartMode) uDlgID = IDD_YSCALE_SETUP_WORLDON;
	CDlgYScaleSetUp dlgYScaleSetUp( this, m_pIChartOCX->GetOcxHwnd(), m_eChartMode, uDlgID);
	
	// 1.5 ���� / ������ ���ð� ����
	dlgYScaleSetUp.SetYScaleMaxMinSetupType( m_nYScaleMaxMinSetupType );		

	// 1.6 ȣ�� ���е� ����
	dlgYScaleSetUp.SetValueFormat( m_strFormat );
	
	// 1.7 ��������, ���Ѱ�, ���Ѱ� ������ ����
	dlgYScaleSetUp.SetStockPriceInfo( m_dYesterdayClosePrice, m_dUpperBound, m_dLowerBound );
	
	// 1.8 �ִ밪 / �ּҰ��� ����
	dlgYScaleSetUp.SetYScaleMaxMin( m_dYScaleMaxValue, m_dYScaleMinValue);
	
	// 1.9 �ִ� ������ / �ּ� �������� ����
	dlgYScaleSetUp.SetYScaleMaxMinPercent( m_dYScaleMaxPercentValue, m_dYScaleMinPercentValue);

	// (2006/11/25 - Seung-Won, Bae) Notify ToolState.
	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_PRICE_YSCALE_SETUP, TRUE);

	// (2007/5/2 - Seung-Won, Bae) Check Market Type for Unit.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE));
	if( pIPacket)
	{
		dlgYScaleSetUp.m_eMarketType = pIPacket->GetCommodityBondName();
		pIPacket->Release();
	}
	// 20081016 ����ǥ >>	
	ILPCSTR szType = pIPacket->GetType();
	CString strPacketName(szType);
	dlgYScaleSetUp.SetPacketType(strPacketName);
	// 20081016 ����ǥ <<

	// 1.10 ���� dialog�� Load�Ѵ�.
	ML_SET_LANGUAGE_RES();
	if( dlgYScaleSetUp.DoModal() == IDOK )
	{
		// ����ڰ� ������ �����ϰ� ������ ���� ��� ���������� �ִ� �ּҰ��� ���´�.
		if( m_nYScaleMaxMinSetupType == 1 )
		{
			m_eYScaleSetupApplyState = YSCALE_APPLY_STATE::APPLY_PERCENT;

			m_dYScaleMaxValue =	m_dYesterdayClosePrice * ( 1 + m_dYScaleMaxPercentValue / 100 );
			m_dYScaleMinValue =	m_dYesterdayClosePrice * ( 1 + m_dYScaleMinPercentValue / 100 );
		}
		
		else
			m_eYScaleSetupApplyState = YSCALE_APPLY_STATE::APPLY_VALUE;


		// Block�� �������� �����Ѵ�.
		pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, TRUE,
										 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
										 m_dYScaleMinValue, m_dYScaleMaxValue);
	}

	// �ÿ��ڰ� ��Ҹ� �����ϸ� 
	else
	{
		m_eYScaleSetupApplyState = YSCALE_APPLY_STATE::APPLY_CANCEL;

		// Block�� �������� �����Ѵ�.
		pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, FALSE,
										 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_VIEW,
										 m_dYScaleMinValue, m_dYScaleMaxValue);
	}

	// (2006/11/25 - Seung-Won, Bae) Notify ToolState.
	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_PRICE_YSCALE_SETUP, FALSE);

	pIBlock->Release();
	pIBlock = NULL;
				
	m_pIChartOCX->InvalidateControl();
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 14
// Return void	: 
// Param  IPropertyMap *p_pIAddInEnvMap : 
// Comments		: File���� m_strAddInItemName�� �ش�Ǵ� ������ �о�´�.
//				: ex) "1:000660:%%.%2f:50000:10000:15:-15" 
//				: 1				=> CANCEL : ����, APPLY_VALUE : ������, APPLY_PERCENT : ��������
//				: 000660		=> �����ڵ�
//				: %%.%2f		=> ȣ�� format
//				: 50000:10000	=> �ִ� / ������
//				: 15:-15		=> ���� �ִ� / �ּҰ�.
//-----------------------------------------------------------------------------
void CYScaleSetUpAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap )
{
	// 1. p_pIAddInEnvMap���� "YSCALE_SETUP"���� ����� ���� �о�´�.
	const char *szEnvValue = NULL;
	if( !p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue) )
		return;


	// 2. �о�� ���� ���̰� 0�̸� return�Ѵ�.
	CString strEnvValue = szEnvValue;
	if( strEnvValue.GetLength() <= 0 )
		return;


	// 3. �о�� ������ ���� �������� �и��Ѵ�.
	CString strYScaleMaxMinSetupType = strEnvValue.Mid( 0, 1 );
	if( strYScaleMaxMinSetupType == "" )	return;

	m_eYScaleSetupApplyState = (YSCALE_APPLY_STATE)atoi(strYScaleMaxMinSetupType);
	
	if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_PERCENT )
		m_nYScaleMaxMinSetupType = 1;

	else
		m_nYScaleMaxMinSetupType = 0;

	strEnvValue = strEnvValue.Mid(2);


	// 4. �����ڵ带 ���´�.
	int nIdx = strEnvValue.Find(":");
	if( nIdx < 0 )	return;
	m_strStockCode = strEnvValue.Mid(0, nIdx);
	
	strEnvValue = strEnvValue.Mid(nIdx+1);

	// 5. ���� foramt
	nIdx = strEnvValue.Find(":", 1);
	if( nIdx < 0 )	return;
	m_strFormat = strEnvValue.Mid(0, nIdx);


	strEnvValue = strEnvValue.Mid(nIdx+1);

	// 6. ����ڰ� ������ �ִ밪�� �����´�.
	nIdx = strEnvValue.Find(":", 1);
	if( nIdx < 0 )	return;
	CString strYScaleMaxValue = strEnvValue.Mid(0, nIdx);
	m_dYScaleMaxValue = atof(strYScaleMaxValue);

	strEnvValue = strEnvValue.Mid(nIdx+1);
	

	// 7. ����ڰ� ������ �ּҰ��� �����´�.
	nIdx = strEnvValue.Find(":", 1);
	if( nIdx < 0 )	return;
	CString strYScaleMinValue = strEnvValue.Mid(0, nIdx);
	m_dYScaleMinValue = atof(strYScaleMinValue);

	strEnvValue = strEnvValue.Mid(nIdx+1);


	// 8. ���� �������� ����ڰ� ������ �ִ밪�� �����´�.
	nIdx = strEnvValue.Find(":", 1);
	if( nIdx < 0 )	return;
	CString strYScaleMaxPercentValue = strEnvValue.Mid(0, nIdx);
	m_dYScaleMaxPercentValue = atof(strYScaleMaxPercentValue);

	strEnvValue = strEnvValue.Mid(nIdx+1);


	// 9. ���� �������� ����ڰ� ������ �ּҰ��� �����´�.
	if( strEnvValue.GetLength() <= 0 )	return;
	CString strYScaleMinPercentValue = strEnvValue;
	m_dYScaleMinPercentValue = atof(strYScaleMinPercentValue);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 14
// Return void	: 
// Param  IPropertyMap *p_pIAddInEnvMap : 
// Comments		: File���� m_strAddInItemName�� �ش�Ǵ� ������ �����Ѵ�.
//				: ex) "1:15:-15" 
//				: 1 -> ������ ��� : 0, ���� ��� : 1
//				: 15: -15 -> ���� �ִ� / �ּҰ�.
//-----------------------------------------------------------------------------
void CYScaleSetUpAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap )
{
	// 1. p_pIAddInEnvMap���� "YSCALE_SETUP"���� ����� ���� �о�´�.
	const char *szEnvValue = NULL;
	if( p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue) ) return;

	
	// 2. ����/���� ���� ����, �ִ� / �ּ� ���� ������ �����Ѵ�.
	CString strEnvValue;
	strEnvValue.Format( "%d:%s:%s:%f:%f:%f:%f",
						m_eYScaleSetupApplyState,		\
						m_strStockCode,					\
						m_strFormat,					\
						m_dYScaleMaxValue,				\
						m_dYScaleMinValue,				\
						m_dYScaleMaxPercentValue,		\
						m_dYScaleMinPercentValue );
	
	p_pIAddInEnvMap->SetAt( m_strAddInItemName, strEnvValue);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 15
// Return void	: 
// Param  const char *p_szHelpMsg : 
// Comments		: 
//-----------------------------------------------------------------------------
void CYScaleSetUpAddInImp::ResetYScaleSetUp( const char *p_szHelpMsg)
{
	// 1. ���� Bound ���� �ʱ�ȭ�Ѵ�. (������ �����ϸ�, Clear�ǵ���)
	m_strGraphName.Empty();

	// 2. Message�� Ȯ���Ѵ�.
	if( !p_szHelpMsg) return;
	if( !*p_szHelpMsg) return;

	// 3. Message�� Buffer�� �ű��.
	CString strYScale;
	char *szYScale = strYScale.GetBufferSetLength( strlen( p_szHelpMsg ) );
	strcpy( szYScale, p_szHelpMsg);

	// 4. ���� Graph Name�� ���Ѵ�.
	char *	szGraphName		= szYScale;
	char *	szLowerBound	= NULL;
	char *	szUpperBound	= NULL;
	szYScale = strchr( szYScale, ':');
	if( !szYScale) return;
	*szYScale = '\0';

	// 5. ���Ѱ� Message�� ���Ѵ�.
	szYScale++;
	szUpperBound = szYScale;
	szYScale = strchr( szYScale, ',');
	if( !szYScale) return;
	*szYScale = '\0';

	// 6. ���Ѱ� Message�� ���Ѵ�.
	szYScale++;
	szLowerBound = szYScale;
	
	// (2004.10.25, ��¿�) Ȯ��� ������ �־ ������ ���� �ʵ��� �����Ѵ�. (Ȯ�� ������ ������ �����Ѵ�.)
	szYScale = strchr( szYScale, ':');
	if( szYScale)
	{
		*szYScale = '\0';
		szYScale++;
		m_strExtMsg = szYScale;
		// (2004.10.28, ��¿�) �� Space�� �̸� Clear�� �д�.
		m_strExtMsg.TrimRight();
	}

	if( !*szGraphName || !*szUpperBound || !*szLowerBound) return;
	// 7. ������ Graph Name, ��/���Ѱ��� ���Ͽ� �������Ѵ�.
	m_strGraphName	= szGraphName;
	m_dUpperBound	= atof( szUpperBound);
	m_dLowerBound	= atof( szLowerBound);

	// 8. ������ ��/���Ѱ��� ������ ��츦 �ٷ���´�.
	if( m_dUpperBound < m_dLowerBound)
	{
		double dTemp	= m_dUpperBound;
		m_dUpperBound	= m_dLowerBound;
		m_dLowerBound	= dTemp;
	}

	// 9. ��/���ѹ��� �������� �Ǵ� �������� �����Ѵ�.
	SetYesterdayClosePrice(m_strExtMsg);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 15
// Return void	: 
// Param  const CString& strData : 
// Comments		: 
//-----------------------------------------------------------------------------
void CYScaleSetUpAddInImp::SetYesterdayClosePrice(const CString& strData)
{
	CString strTemp = strData;
	m_strStockCode = CDataConversion::GetStringData(strTemp, ":");	//�����ڵ�
	CDataConversion::GetStringData(strTemp, ":");//??
	CString strPrice = CDataConversion::GetStringData(strTemp, ":");	//��������
	m_dYesterdayClosePrice = atof(strPrice);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 16
// Return BOOL	: 
// Comments		: ���������� ���´�.
//-----------------------------------------------------------------------------
BOOL CYScaleSetUpAddInImp::SetYScaleInfo( IBlock *pIBlock )
{
	if( m_strGraphName.IsEmpty() )
		return FALSE;

	if( !pIBlock) return FALSE;


	// 3. Block���� VScale�� �ִ�, �ּҰ��� ���´�.
	pIBlock->GetVScaleGroupMinMax( m_nVertScaleGroupIndex, m_dYScaleMinValue, m_dYScaleMaxValue );
	
	
	// 4. ȣ�� ���е��� ��� ���� Packet������ ���´�.
	IPacket *pIPacket = NULL;
	double dDataType = 0.0L;
	BOOL bResult = pIBlock->GetVScaleGroupHeadPacketAndDataType( m_nVertScaleGroupIndex, pIPacket, dDataType );
	if( !bResult)
	{
		if( pIPacket) pIPacket->Release();
		return FALSE;
	}


	// 5. Packet ������ ������ ȣ�� ���е��� ��� ���� ������ �ڸ����� �����Ѵ�.
	if( pIPacket )
	{
		// ȣ���� ���е�(Precision)�� �����Ͽ� ȣ�� String�� ������ �����ϵ��� �Ѵ�.
		int nDecimal = 0;

		if( dDataType != 0)
		{
			m_dYesterdayClosePrice = int( m_dYesterdayClosePrice / dDataType + 0.5) * dDataType;
			m_dUpperBound = int( m_dUpperBound / dDataType + 0.5) * dDataType;
			m_dLowerBound = int( m_dLowerBound / dDataType + 0.5) * dDataType;
			
			// double�� Int�� ��ȯ�ϴ� �������� ��Ȯ�� Int�� ��ȯ���� �ʴ� ������ �ذ��ϱ� ����
			// 100�� ��ȿ�ڸ����� �򵵷� �����Ѵ�.
			nDecimal = ( int)( ( log10( dDataType) * 100) / 100);	
		}
	
		if( 0 <= nDecimal)
			m_strFormat = "%.0f";
		else
			m_strFormat.Format( "%%.%df", -nDecimal);
		
#ifdef _DEBUG
	TRACE( "%f	%f	%f\r\n", m_dYesterdayClosePrice, m_dUpperBound, m_dLowerBound);
#endif
		pIPacket->Release();
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 16
// Return void	: 
// Param  int nYScaleSetUpType : ���� / ������ ���� ����
// Param  double dMax : �ִ밪
// Param  double dMin : �ּҰ�
// Param  double dMaxPercent : �ִ� ������ 
// Param  double dMinPercent : �ּ� ������
// Comments		: DlgYScale���� ������ ���� �����ϱ� ���� ���� �Լ�.
//-----------------------------------------------------------------------------
void CYScaleSetUpAddInImp::SetYScaleAllValue( int nYScaleSetUpType, double dMax, double dMin, double dMaxPercent, double dMinPercent )
{
	// 1. ���� / ������ ���� ����
	m_nYScaleMaxMinSetupType = nYScaleSetUpType;
	
	// 2. �ִ밪 / �ּҰ�
	m_dYScaleMaxValue = dMax;
	m_dYScaleMinValue = dMin;

	// 3. �ִ� / �ּ� Percent ��
	m_dYScaleMaxPercentValue = dMaxPercent;
	m_dYScaleMinPercentValue = dMinPercent;
}