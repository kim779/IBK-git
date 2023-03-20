// Data_ConditionTitle.cpp: implementation of the CData_ConditionTitle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Data_ConditionTitle.h"

#include "ListCtrlData.h"
#include "BaseData.h"
#include "FindType.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return CString : 
// Param  const CString &title : 
// Param  const CString &compart : 
// Comments		: 
//-----------------------------------------------------------------------------
CString CData_ConditionTitle::MakeString( const CString &title, const CString &compart )
{
	return title + compart;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Return CString : 
// Param  const CString &strIndicatorName : 
// Comments		: 
//-----------------------------------------------------------------------------
CString CData_ConditionTitle::GetConditionTitle( const CString &strIndicatorName )
{	
	CString indicatorname = strIndicatorName;
	indicatorname = CFindType::FindStochastics( strIndicatorName );

// 2008.02.04 by LYH >>	
	CString strMaPeriod, strPeriod, strIncInterval, strMALineNum, strIntervalNum;
	strMaPeriod.LoadString(IDS_MA_PERIOD);
	strPeriod.LoadString(IDS_PERIOD);
	strIncInterval.LoadString(IDS_INC_INTERVAL);
	strMALineNum.LoadString(IDS_MA_LINE_NUM);
	strIntervalNum.LoadString(IDS_INTERVAL_NUM);

	CString strStandardPeriod, strConversePeriod, strSDMulti, strBulkNum, strUpperPer, strPre, strPost;
	strStandardPeriod.LoadString(IDS_STANDARD_PERIOD);
	strConversePeriod.LoadString(IDS_CONVERSE_PERIOD);
	strSDMulti.LoadString(IDS_SD_MULTI);
	strBulkNum.LoadString(IDS_BULK_NUM);
	strUpperPer.LoadString(IDS_UPPER_PER);
	strPre.LoadString(IDS_PRE);
	strPost.LoadString(IDS_POST);

	CString strLowerPer, strReversePer, strDisparity, strDisparityPeriod, strShortTerm;
	strLowerPer.LoadString(IDS_LOWER_PER);
	strReversePer.LoadString(IDS_REVERSE_PER);
	strDisparity.LoadString(IDS_DISPARITY);
	strDisparityPeriod.LoadString(IDS_DISPARITY_PERIOD);
	strShortTerm.LoadString(IDS_SHORT_TERM);

	CString strLongTerm, strSD, strShortMA, strLongMA, strMomentumPeriod;
	strLongTerm.LoadString(IDS_LONG_TERM);
	strSD.LoadString(IDS_STRING_SD);
	strShortMA.LoadString(IDS_SHORT_MA);
	strLongMA.LoadString(IDS_LONG_MA);
	strMomentumPeriod.LoadString(IDS_MOMENTUM_PERIOD);

	CString strMomentum, strComparePeriod, strPsychologyPeriod, strSEMulti, strAccVariable, strMaxValue, strCumPeriod;
	strMomentum.LoadString(IDS_MOMENTUM);
	strComparePeriod.LoadString(IDS_COMPARE_PERIOD);
	strPsychologyPeriod.LoadString(IDS_PSYCHOLOGY_PERIOD);
	strSEMulti.LoadString(IDS_SE_MULTI);
	strAccVariable.LoadString(IDS_ACC_VARIABLE);
	strMaxValue.LoadString(IDS_MAX_VALUE);
	strCumPeriod.LoadString(IDS_CUMULATIVE_PERIOD);

	CString retValue;
	for( int i = 0 ; i < GRAPH_COUNT; i++ )
	{
		CString strData = CString(_MTEXT(C2_PRICE_CHART));
		if( indicatorname.CompareNoCase( _MTEXTN( g_szIndicatorNameList[i]) ) == 0 )
		{
			switch( i )
			{
				// �׹���Ʈ
				case GRAPH_RAINBOW:
					//return _T("����Ⱓ,��������,���򼱼�,");
					{
						retValue.Format("%s,%s,%s,", strMaPeriod, strIncInterval, strMALineNum);
						return retValue;
					}
				// ���Ź�
				case GRAPH_HORIZONTALBAR :
					//return _T("������,");
					{
						retValue.Format("%s,", strIntervalNum);
						return retValue;
					}
				case GRAPH_GLANCEBALANCE :
					//return _T( "���رⰣ,��ȯ�Ⱓ,����1�Ⱓ,����2�Ⱓ,����Ⱓ," );
					{
						retValue.Format("%s,%s,%s1%s,%s2%s,%s%s,", strStandardPeriod, strConversePeriod, strPre, strPeriod, strPre, strPeriod, strPost, strPeriod);
						return retValue;
					}
				
				case GRAPH_BOLLINGER :
				case GRAPH_BandWidth:
					//return _T( "����Ⱓ,ǥ�������¼�," );
					{
						retValue.Format("%s,%s,", strMaPeriod, strSDMulti);
						return retValue;
					}

				case GRAPH_BANDB:
					//return _T( "����Ⱓ,ǥ�������¼�,Signal," );
					{
						retValue.Format("%s,%s,Signal,", strMaPeriod, strSDMulti);
						return retValue;
					}

				case GRAPH_BOXCHART:
					//return _T( "������," );
					{
						retValue.Format("%s,", strBulkNum);
						return retValue;
					}

				case GRAPH_ENVELOPE:
				case GRAPH_MAC:
					//return _T("����Ⱓ,������(%),������(%),");
					{
						retValue.Format("%s,%s(%%),%s(%%),", strMaPeriod, strUpperPer, strLowerPer);
						return retValue;
					}

				case GRAPH_ZIGZAG:
				case GRAPH_CLC:
					//return _T("������(%),");
					{
						retValue.Format("%s(%%),", strReversePer);
						return retValue;
					}

				case GRAPH_AD:
				case GRAPH_NEGATIVE_VOLUME_INDEX:
				case GRAPH_OBV:
				case GRAPH_POSITIVE_VOLUME_INDEX:
				case GRAPH_PRICE_VOLUME_TREND:
				case GRAPH_7BINARYWAVE:
				case GRAPH_VA_OSCILLATOR:
					return _T("Signal,");

				case GRAPH_PRICE_CHANNEL:
				case GRAPH_AROON:
				case GRAPH_FORMULA:
				case GRAPH_TDI:
				case GRAPH_ELDER_RAY_BEAR:
				case GRAPH_ELDER_RAY_BULL:
				case GRAPH_Sigma:
				case GRAPH_STANDARD_DEVIATION:
				case GRAPH_STANDARD_ERROR:
				case GRAPH_CMF:
				case GRAPH_VOLUME_RATIO:
				case GRAPH_VROC:
				case GRAPH_VHF:
				case GRAPH_ADX:
				case GRAPH_UPDOWN_ADL:
				case GRAPH_UPDOWN_ADR:
				case GRAPH_ENERGY:
					//return _T("�Ⱓ,");
					{
						retValue.Format("%s,", strPeriod);
						return retValue;
					}

				case GRAPH_DEMA:
				case GRAPH_TEMA:
					//return _T("����Ⱓ,");
					{
						retValue.Format("%s,", strMaPeriod);
						return retValue;
					}

				case GRAPH_DISPARITY_SIMPLE:
				case GRAPH_DISPARITY_COMPLEX:
					//return _T("�̰�1,�̰�2,�̰�3,�̰�4,�̰�5,");
					{
						retValue.Format("%s1,%s2,%s3,%s4,%s5,", strDisparity, strDisparity, strDisparity, strDisparity, strDisparity);
						return retValue;
					}

				case GRAPH_STARC_BAND:
					//return _T("�Ⱓ,����Ⱓ,");
					{
						retValue.Format("%s,%s,", strPeriod, strMaPeriod);
						return retValue;
					}

				case GRAPH_CCI:
				case GRAPH_LRS:
				case GRAPH_MOMENTUM:
				case GRAPH_NDI:
				case GRAPH_ROC:
				case GRAPH_TRIX:
				case GRAPH_PERCENT_R:
				case GRAPH_ChaikinsVolatility:
				case GRAPH_DPO:
				case GRAPH_NCO:
				case GRAPH_QSTIC:
				case GRAPH_RSI:
				case GRAPH_MFI:
				case GRAPH_FORCE_INDEX:
				case GRAPH_PSYCO_LINE:
				case IMetaTable::EDDC_ST_NEW_PSYCHOLOGY_LINE:
					//return _T("�Ⱓ,Signal,");
					{
						retValue.Format("%s,Signal,", strPeriod);
						return retValue;
					}

				case GRAPH_BinaryWave:
					// ( 2006 / 06 / 05 - Sang-Woo, Cho ) - A title of Binary Wave modification
					//return _T("�̰ݵ��Ⱓ,ROC�Ⱓ,Sto Fast %K�Ⱓ,Sto Fast %D�Ⱓ,�ܱ�����,�������,Signal,");
					//return _T("�̰ݵ��Ⱓ,ROC�Ⱓ,Sto Fast %K,Sto Fast %D,MACD �ܱ�,MACD ���,MACD Signal,");
					{
						retValue.Format("%s,ROC %s,Sto Fast %%K,Sto Fast %%D,MACD %s,MACD %s,MACD Signal,", strDisparityPeriod, strPeriod, strShortTerm, strLongTerm);
						return retValue;
					}

				case GRAPH_PERCENT_B_STOCHASTIC:
					//return _T("����Ⱓ,ǥ������,Slow%K�Ⱓ1,slow%K�Ⱓ2,Slow%D�Ⱓ,");
					{
						retValue.Format("%s,%s,Slow%%K %s1,slow%%K %s2,Slow%%D %s,", strMaPeriod, strSD, strPeriod, strPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_LRS_STOCHASTIC:
					//return _T("LRS�Ⱓ,Slow%K�Ⱓ1,slow%K�Ⱓ2,Slow%D�Ⱓ,");
					{
						retValue.Format("LRS %s,Slow%%K %s1,slow%%K %s2,Slow%%D %s,", strPeriod, strPeriod, strPeriod, strPeriod);
						return retValue;
					}
						
				case GRAPH_MACD_STOCHASTIC:
					//return _T("�ܱ�����,�������,Slow%K�Ⱓ1,slow%K�Ⱓ2,Slow%D�Ⱓ,");
					{
						retValue.Format("%s,%s,Slow%%K %s1,slow%%K %s2,Slow%%D %s,", strShortMA, strLongMA, strPeriod, strPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_MOMENTUM_STOCHASTIC:
					//return _T("����ұⰣ,Slow%K�Ⱓ1,slow%K�Ⱓ2,Slow%D�Ⱓ,");
					{
						retValue.Format("%s,%s,Slow%%K %s1,slow%%K %s2,Slow%%D %s,", strShortMA, strLongMA, strPeriod, strPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_OBV_MOMENTUM:
					//return _T("����ұⰣ,�����Signal,");
					{
						retValue.Format("%s,%s Signal,", strMomentumPeriod, strMomentum);
						return retValue;
					}
									
				case GRAPH_OBV_STOCHASTIC:
					//return _T("Fast%K�Ⱓ,Slow%K�Ⱓ,Slow%D�Ⱓ,");
					{
						retValue.Format("Fast%%K %s,Slow%%K %s,Slow%%D %s,", strPeriod, strPeriod, strPeriod);
						return retValue;
					}
										
				case GRAPH_ROC_STOCHASTIC:
					//return _T("ROC�Ⱓ,Slow%K�Ⱓ1,slow%K�Ⱓ2,Slow%D�Ⱓ,");
					{
						retValue.Format("ROC %s,Slow%%K %s1,slow%%K %s2,Slow%%D %s,", strPeriod, strPeriod, strPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_RSI_MACD:
					//return _T("RSI�Ⱓ,�ܱ�����,�������,Signal,");
					{
						retValue.Format("RSI %s,%s,%s,Signal,", strPeriod, strShortMA, strLongMA);
						return retValue;
					}

				case GRAPH_RSI_STOCHASTIC:
					//return _T("RSI�Ⱓ,Slow%K�Ⱓ1,Slow%K�Ⱓ2,Slow%D�Ⱓ,");
					{
						retValue.Format("RSI %s,Slow%%K %s1,Slow%%K %s2,Slow%%D %s,", strPeriod, strPeriod, strPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_SONAR_PSYCO_LINE:
					//return _T("����Ⱓ,�񱳱Ⱓ,�ɸ����Ⱓ,");
					{
						retValue.Format("%s,%s,%s,", strMaPeriod, strComparePeriod, strPsychologyPeriod);
						return retValue;
					}
				
				case GRAPH_STOCHASTIC_RSI:
					//return _T("Fast%K�Ⱓ,Fast%d�Ⱓ,RSI�Ⱓ,Signal,");
					{
						retValue.Format("Fast%%K %s,Fast%%D %s,RSI %s,Signal,", strPeriod, strPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_STANDARD_ERROR_BAND:
					//return _T("�Ⱓ,ǥ�ؿ����¼�,");
					{
						retValue.Format("%s,%s,", strPeriod, strSEMulti);
						return retValue;
					}

				case GRAPH_STANDARD_ERROR_CHANNEL:
					//return _T("ǥ�ؿ����¼�,");
					{
						retValue.Format("%s,", strSEMulti);
						return retValue;
					}

				case GRAPH_STANDARD_GAP_BAND:
					//return _T("ǥ�������¼�,");
					{
						retValue.Format("%s,", strSDMulti);
						return retValue;
					}

				case GRAPH_PARABOLIC_SAR:
					//return _T("���Ӻ���,�ִ밪,");
					{
						retValue.Format("%s,%s,", strAccVariable, strMaxValue);
						return retValue;
					}

				case GRAPH_ADXR:
					//return _T("ADX�Ⱓ,ADXR�Ⱓ,");
					{
						retValue.Format("ADX %s,ADXR %s,", strPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_MACD:
				case GRAPH_MACD_OSC:
				case GRAPH_MAO:
				case GRAPH_PRICE_OSCILLATOR:
				case GRAPH_CHARKINS_OSCILLATOR:
				case GRAPH_VOLUME_OSCILLATOR:
					//return _T("�ܱ�����,�������,Signal,");
					{
						retValue.Format("%s,%s,Signal,", strShortMA, strLongMA);
						return retValue;
					}

				case GRAPH_SONAR:
					//return _T("����Ⱓ,Signal,");
					{
						retValue.Format("%s,Signal,", strMaPeriod);
						return retValue;
					}

				case GRAPH_SROC:
					//return _T("����Ⱓ,�Ⱓ,Signal,");
					{
						retValue.Format("%s,%s,Signal,", strMaPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_AB_RATIO:
					//return _T("AR�Ⱓ,BR�Ⱓ,");
					{
						retValue.Format("AR %s,BR %s,", strPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_ATR:
				case GRAPH_EOM:
					//return _T("����Ⱓ,Signal,");
					{
						retValue.Format("%s,Signal,", strMaPeriod);
						return retValue;
					}

				case GRAPH_MASS_INDEX:
					//return _T("�����Ⱓ,Signal,");
					{
						retValue.Format("%s,Signal,", strCumPeriod);
						return retValue;
					}

				case GRAPH_RMI:
					//return _T("RS�Ⱓ,RMI�Ⱓ,");
					{
						retValue.Format("RS %s,RMI %s,", strPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_STOCHASTICS_FAST:
					//return _T("Fase%K�Ⱓ,Fast%D�Ⱓ,");
					{
						retValue.Format("Fase%%K %s,Fast%%D %s,", strPeriod, strPeriod);
						return retValue;
					}

				case GRAPH_STOCHASTICS_SLOW:
					//return _T("Fase%K�Ⱓ,Slow%K�Ⱓ,Slow%D�Ⱓ,");
					{
						retValue.Format("Fase%%K %s,Slow%%K %s,Slow%%D %s,", strPeriod, strPeriod, strPeriod);
						return retValue;
					}
			}
		}
	}

	// (2006/4/19 - Seung-Won, Bae) Comment for pConditiontitle
	//		return MakeString( pConditiontitle[39], ","); //""
	//return MakeString( _T("������"), ","); //""
	return MakeString( strIntervalNum, ","); //""
// 2008..02.04 by LYH <<
}


