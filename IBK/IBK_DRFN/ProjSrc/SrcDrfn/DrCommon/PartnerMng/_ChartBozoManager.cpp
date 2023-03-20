// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PartnerMng.h"

#define CALC_CHGRATE(x,y) \
		(double)(((double)((double)(x)-(double)(y))*100.0)/((double)y))

char *gChtTDC_RATE[] = {"0.01"	, "1" };
char *gChtRDC_RATE[] = {"0.01"	, "1" };
int   gChRATE_DATA[] = {100		, 1};

// #define	RATE_100
// #ifdef RATE_100
// #define	TDC_RATE	_T("0.01")
// #define	RDC_RATE	_T("0.01")
// #define	RATE_DATA	100
// #else
// #define	TDC_RATE	_T("1")
// #define	RDC_RATE	_T("1")
// #define	RATE_DATA	1
// #endif

//---------------------------------------------------------------------------------------
//
//				class CPartnerManager
//
//---------------------------------------------------------------------------------------
CChartBozoManager::CChartBozoManager()
{
}

int CChartBozoManager::_DoCalcBozoSize(CString &rString)
{
	int nLen = rString.GetLength();
	char aSize[5] = {0,};
	sprintf(aSize, "%04d", nLen -4);

	for(int i=0; i<4; i++)	rString.SetAt(i, aSize[i]);

	return nLen;
}

int CChartBozoManager::ChangeChartBozoMsg(CString &rString, List_CChartBozoItem* pList)
{
	CString szTmp, szItem;
	int nStart, nLength;
	for(POSITION pos=pList->GetHeadPosition(); pos ;)
	{
		CChartBozoItem *pItem = pList->GetNext(pos);
		switch(pItem->m_nOption)
		{
			case CChartBozoItem::cbi_chanage:
			case CChartBozoItem::cbi_add:
				if(_FindKeyInBozoString(rString, pItem->m_szKey, nStart, nLength))
				{					
					//rString.Delete(nStart, nLength);
					szItem.Format("%s=%s@", pItem->m_szKey, pItem->m_szValue);
					szTmp = rString.Mid(nStart, nLength);
					if(szTmp.Compare(szItem) != 0)
						rString.Replace(szTmp, szItem);
					//rString += szItem;
				}
				else
				{
					szItem.Format("%s=%s@", pItem->m_szKey, pItem->m_szValue);
					rString += szItem;
				}
				break;
// 			case CChartBozoItem::cbi_add:
// 				{
// 					szItem.Format("%s=%s@", pItem->m_szKey, pItem->m_szValue);
// 					rString += szItem;
// 				}
// 				break;
			case CChartBozoItem::cbi_delete:
				if(_FindKeyInBozoString(rString, pItem->m_szKey, nStart, nLength))
				{
					rString.Delete(nStart, nLength);
				}
				break;
		}
	}
	_DoCalcBozoSize(rString);
	return rString.GetLength();
}

BOOL CChartBozoManager::_FindKeyInBozoString(CString &rString, LPCSTR szKey, int &nStart, int &nLength)
{
	CString szFindKey(szKey);
	szFindKey += "=";

	int nPos = rString.Find(szFindKey);
	if(nPos<0) return FALSE;

	int nPos2 = rString.Find('@', nPos+1);
	if(nPos2<0) return FALSE;

	nStart = nPos;
	nLength = nPos2 - nPos +1;
	return TRUE;
}

void CChartBozoManager::_DoPre_GetChartBozoMsg(CChartBozoInput* pChartBInput)
{
	if(pChartBInput->m_sStartTime.GetLength()==4)
		pChartBInput->m_sStartTime += _T("00");
}

// nOption : �������� ó�����¿� ���� �ɼ�
//			 0 : �����Ϳ� point(.)�� ���� �ö��� ó���ϴ� ����. ������ DRFN����Ʈ�� ó�����.
//				 �ֽ�=> x1, ����/���� x0.01
//			 1 : ������ ��ü�� �̹� ������ ������ ����. ���� x1 ���·� ó���Ѵ�.
int CChartBozoManager::GetChartBozoMsg(LPCSTR szFIDKey, CChartBozoInput* pChartBInput, CString &rString, int nOption)
{
	rString.Empty();
	pChartBInput->DoConvert();

	int nFidKey = GetFidType(szFIDKey);

	_DoPre_GetChartBozoMsg(pChartBInput);

	switch(nFidKey)
	{
		case CMasterTypeInfo::STOCK_CHART:	//case CChartBozoInput::ctstk:
			MakeChtBojo_ctstk(pChartBInput, rString, nOption);
			_DoCalcBozoSize(rString);
			break;

		case CMasterTypeInfo::UPJONG_CHART:		//case CChartBozoInput::ctup:
			MakeChtBojo_ctup(pChartBInput, rString, nOption);
			_DoCalcBozoSize(rString);
			break;

		case CMasterTypeInfo::FUTOPT_CHART:		//case CChartBozoInput::ctfo:
			MakeChtBojo_ctfo(pChartBInput, rString, nOption);
			_DoCalcBozoSize(rString);
			break;

		case CMasterTypeInfo::STOCKFUTOPT_CHART:		//case CChartBozoInput::ctsfo:
			MakeChtBojo_ctsfo(pChartBInput, rString, nOption);
			_DoCalcBozoSize(rString);
			break;

		case CMasterTypeInfo::FOREIGN_CHART:		//case CChartBozoInput::ctfrn:
			MakeChtBojo_ctfrn(pChartBInput, rString, nOption);
			_DoCalcBozoSize(rString);
			break;

		case CMasterTypeInfo::MARKET_CHART:
			MakeChtBojo_market(pChartBInput, rString, nOption);	
			_DoCalcBozoSize(rString);
			break;

		case CMasterTypeInfo::FUOPT_JIPYO_CHART:		//case CChartBozoInput::ctfo:
			MakeChtBojo_ctfo(pChartBInput, rString, nOption);
			_DoCalcBozoSize(rString);
			break;

		case 111:
			// �ؿܼ���
			MakeChtBojo_ctfofor(pChartBInput, rString, nOption);
			_DoCalcBozoSize(rString);
			break;

		//<<20100310_JS.Kim FX
		case CMasterTypeInfo::FX_CHART:
			MakeChtBojo_ctfx(pChartBInput, rString, nOption);
			_DoCalcBozoSize(rString);
			break;
		//>>

		//<<20110106 alzioyes CME
		case CMasterTypeInfo::CME_CHART:
			MakeChtBojo_ctcme(pChartBInput, rString, nOption);
			_DoCalcBozoSize(rString);
			break;
		//>>
		
		case CMasterTypeInfo::NONE_CHART:
		default:
			rString = _T("0000");
			return rString.GetLength();
	}

//	if(gDebugMng)
//	{
//		CString szDebug;
//		szDebug.Format("[SMalzio]ChartBozo=[%s]\n", rString);
//		gDebugMng->ShowOutputDebugString(szDebug);
//	}
	{
		CString szDebug;
		szDebug.Format("ChartBozo size=%d [%s]\n", rString.GetLength(), rString);
		if(g_pDRDebuger) g_pDRDebuger->DRLog_Message(szDebug);
	}

	//g_DRDebuger.DRLog_ResponseData("chartBozo", (LPBYTE)(LPCSTR)rString, rString.GetLength());
	return rString.GetLength();
}
// void CChartBozoManager::MakeChtBojo(LPCTSTR lpszFormat, ...)
// {
// 
// }

int CChartBozoManager::GetFidType(LPCSTR szFIDMajor)
{
	int nFidMajor = atoi(szFIDMajor);
	return GetFidType(nFidMajor);
}

int CChartBozoManager::GetFidType(int nFIDMajor)
{
	if (nFIDMajor <= 0)
		return CMasterTypeInfo::NONE_CHART;

	if (nFIDMajor < 20)
		return CMasterTypeInfo::STOCK_CHART;
	else if (nFIDMajor < 30)
		return CMasterTypeInfo::UPJONG_CHART;
	else if (nFIDMajor < 40)
		return CMasterTypeInfo::FUTOPT_CHART;
	else
	{
		switch (nFIDMajor)
		{
			case 101:
				return CMasterTypeInfo::STOCK_CHART;
			case 102:
				return CMasterTypeInfo::UPJONG_CHART;
			case 103:
				return CMasterTypeInfo::FUTOPT_CHART;
			case 104:
				return CMasterTypeInfo::FOREIGN_CHART;
			case 109:
				return CMasterTypeInfo::FUOPT_JIPYO_CHART;
			case 133:
				return CMasterTypeInfo::MARKET_CHART;
			case 134:
				return CMasterTypeInfo::STOCKFUTOPT_CHART;
			case 111:
				return 111;
			//<<20100310_JS.Kim FX
			case 112:
				return CMasterTypeInfo::FX_CHART;
			//>>
			case 110:	//20110106 alzioyes CME
				return CMasterTypeInfo::CME_CHART;

			default:
				break;
		}
	}

	return CMasterTypeInfo::NONE_CHART;
}

int CChartBozoManager::GetTrType(LPCSTR szTrName)
{
	return CMasterTypeInfo::NONE_CHART;
}

// ---------------------------------------------------------------------------
//			�ֽ� �����޽���(MakeChtBojo_ctstk)
// ---------------------------------------------------------------------------
int CChartBozoManager::MakeChtBojo_ctstk(CChartBozoInput* pChartBInput, CString& rString, int nOption)
{
	char    lsBMrk[800], lsBojo[800], lsRelpsa[128], lcChtAE;
	//ULONG   lspChartBInput->lVolume = 0;
	long    lsrecprice=0;

    memset(lsBMrk      , 0x20, sizeof(lsBMrk      ));
    memset(lsBojo      , 0x20, sizeof(lsBojo      ));
    memset(lsRelpsa    , 0x20, sizeof(lsRelpsa    ));

	if (pChartBInput->sNxtFlg[0] == '1')	lcChtAE = 'A';
	else									lcChtAE = 'E';

	if(pChartBInput->sDataType[0] == '5') lsrecprice = 0;
	else	 lsrecprice   = pChartBInput->lPrice;

	pChartBInput->nMarket = 1; //���������� ������ ���� �ʱ⶧���� ������Ŵ.

	//	��Ʈ : 0-������Ʈ, 1-�����Ʈ, 2-�޷���Ʈ, 3-���簡, 4-_Open_��Ʈ
	if(pChartBInput->sDataType[0] == '0')
	{
		sprintf(lsBMrk, "BOUNDMARK=������Ʈ:%ld,%ld:%.6s:1:%s:%c@PREVPRICE=_Close_:%s|_Volume_:%s|_VolumeAmount_:%s@OHL=:%ld:%ld:%ld@PREVPRICE2=%ld:%ld:%ld:%ld@",
						pChartBInput->lUpLmtPrice, 
						pChartBInput->lDnLmtPrice, 
						pChartBInput->sShcode, 
						pChartBInput->m_sPreprice, //lsrecprice,
						'1', 
						pChartBInput->m_sPreprice, 
						pChartBInput->m_sVolume,
						pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
						pChartBInput->lOpen, pChartBInput->lHigh, pChartBInput->lLow,
						pChartBInput->lPreHigh, pChartBInput->lPreOpen, pChartBInput->lPreprice, pChartBInput->lPreLow);
	} 
	else if(pChartBInput->sDataType[0] == '2')
	{
        if (pChartBInput->lPrice == 0x00 || pChartBInput->lPrice == 0)
        {
		    sprintf(lsBMrk, "BOUNDMARK=�޷���Ʈ:%.2f,%.2f:%.6s:1:%.2f:2@ER=%.0f:3:_Close_,@PREVPRICE=_Close_:%.2f|_Volume_:%s|_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%ld:%ld:%ld@PREVPRICE2=%ld:%ld:%ld:%ld@", 
                    (double)0,
                    (double)0,
                    pChartBInput->sShcode,
                    (double)0, (0*gChRATE_DATA[nOption]),
                    (double)0,
					pChartBInput->m_sVolume,
					pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
					gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
                    (long)pChartBInput->lOpen, (long)pChartBInput->lHigh, (long)pChartBInput->lLow,
					pChartBInput->lPreHigh, pChartBInput->lPreOpen, pChartBInput->lPreprice, pChartBInput->lPreLow);
        }
        else
        {
		    sprintf(lsBMrk, "BOUNDMARK=�޷���Ʈ:%.2f,%.2f:%.6s:1:%.2f:2@ER=%.0f:3:_Close_,@PREVPRICE=_Close_:%.2f|_Volume_:%s|_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%ld:%ld:%ld@", 
                    (double)(pChartBInput->lUpLmtPrice/100.0),
                    (double)(pChartBInput->lDnLmtPrice/100.0),
                    pChartBInput->sShcode,
                    (double)(pChartBInput->lPreprice/100.0),
					(double)(pChartBInput->lWonDollar),
                    (double)(pChartBInput->lPreprice/100.0),
					pChartBInput->m_sVolume,
					pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
					gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
                    (long)(pChartBInput->lOpen/100.0), (long)(pChartBInput->lHigh/100.0), (long)(pChartBInput->lLow/100.0));
        }
	}
	else 
	{
		sprintf(lsBMrk, "BOUNDMARK=�����Ʈ:%.2f,%.2f:%.6s:1:%.2f:%c@PREVPRICE=_Close_:0.00|_Volume_:%s|_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%ld:%ld:%ld@PREVPRICE2=%ld:%ld:%ld:%ld@",
			CALC_CHGRATE((double)pChartBInput->lUpLmtPrice, lsrecprice),
			CALC_CHGRATE((double)pChartBInput->lDnLmtPrice, lsrecprice),
			pChartBInput->sShcode,
			CALC_CHGRATE(lsrecprice,   lsrecprice),
			'2',
			pChartBInput->m_sVolume,
			pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			pChartBInput->lOpen, pChartBInput->lHigh, pChartBInput->lLow,
			pChartBInput->lPreHigh, pChartBInput->lPreOpen, pChartBInput->lPreprice, pChartBInput->lPreLow);
	} 

	// ��Ʈ �޽���
	if (pChartBInput->sTerm[0] == '0')	//����(0:ƽ/1:��/2:��/3:��/4:��/6:��)
    	sprintf(lsBojo, "####UM=%c@UD=5@UT=%ld@RDATET=DDHHMMSS@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%ld@", lcChtAE, pChartBInput->lTick, lsBMrk, pChartBInput->lFirstClose);
	else if (pChartBInput->sTerm[0] == '1' )
		sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=MMDDHHMM@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%ld@", lcChtAE, pChartBInput->lTick, lsBMrk, pChartBInput->lFirstClose);
	else if (pChartBInput->sTerm[0] == '2')
	{
		BOOL bMarket = pChartBInput->nMarket == 1 || pChartBInput->nMarket == 2 || pChartBInput->nMarket == 3 ;

        if (bMarket && pChartBInput->m_sStartTime.IsEmpty()==FALSE)
        {
            sprintf(lsRelpsa, "RELPSA=%.6s:%.6s", pChartBInput->m_sStartTime, pChartBInput->sShcode);
           	sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@RESETUSEREAL=1@%s@%s@CB=_Close_:0|@SCD=%ld@", lcChtAE, lsBMrk, lsRelpsa, pChartBInput->lFirstClose);
        }
		else
           	sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%ld@", lcChtAE, lsBMrk, pChartBInput->lFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '3')
		sprintf(lsBojo, "####UM=%c@UD=2@RDATET=YYYYMMDD@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%ld@", lcChtAE, lsBMrk, pChartBInput->lFirstClose);
	else if (pChartBInput->sTerm[0] == '4')
		sprintf(lsBojo, "####UM=%c@UD=3@RDATET=YYYYMM@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%ld@", lcChtAE, lsBMrk, pChartBInput->lFirstClose);
	else if (pChartBInput->sTerm[0] == '5')
		sprintf(lsBojo, "####UM=%c@UD=3@RDATET=YYYYMM@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%ld@DATE=%s@", lcChtAE, lsBMrk, pChartBInput->lFirstClose, pChartBInput->m_sLastDataTime);
	else if (pChartBInput->sTerm[0] == '6')
    	sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=DDHHMMSS@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%ld@", lcChtAE, pChartBInput->lTick, lsBMrk, pChartBInput->lFirstClose);

	rString = lsBojo;

	CString szTmpExt;
	if(pChartBInput->m_szMarketTime.IsEmpty()==FALSE)
	{
		szTmpExt.Format("MARKETTIME=%s@", pChartBInput->m_szMarketTime);
		rString += szTmpExt;
		
	}
	if(pChartBInput->m_szBaseTime.IsEmpty()==FALSE)
	{
		szTmpExt.Format("BASETIME=%s@", pChartBInput->m_szBaseTime);
		rString += szTmpExt;		
	}

	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	szTmpExt.Format("DATATIME=%s@", pChartBInput->m_sLastDataTime);
	rString += szTmpExt;		

    return rString.GetLength();
}

// ---------------------------------------------------------------------------
//			���� �����޽���(MakeChtBojo_ctup)
// ---------------------------------------------------------------------------
int CChartBozoManager::MakeChtBojo_ctup(CChartBozoInput* pChartBInput, CString& rString, int nOption)
{
	char    lsBojo[800], lsRelpsa[128], lcChtAE;
	
	memset(lsBojo      , 0x20, sizeof(lsBojo      ));
	memset(lsRelpsa    , 0x20, sizeof(lsRelpsa    ));
	
	if (pChartBInput->sNxtFlg[0] == '1')	lcChtAE = 'A';
	else									lcChtAE = 'E';

	if (pChartBInput->sTerm[0] == '0')
	{
		sprintf(lsBojo, "####UM=%c@UD=5@UT=%ld@RDATET=DDHHMMSS@@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%ld@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE, 
			pChartBInput->lTick, 
			(double)(atof(pChartBInput->m_sPreprice)/gChRATE_DATA[nOption]), 
			(double)pChartBInput->lVolume, 
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			pChartBInput->lFirstClose, //(double)pChartBInput->lPrice/gChRATE_DATA[nOption], 
			atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else if (pChartBInput->sTerm[0] == '1')
	{
		sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=MMDDHHMM@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%ld@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE, pChartBInput->lTick,
			(double)pChartBInput->lPreprice/gChRATE_DATA[nOption], 
			(double)pChartBInput->lVolume, 
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			pChartBInput->lFirstClose, //(double)pChartBInput->lPrice/gChRATE_DATA[nOption], 
			atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else if (pChartBInput->sTerm[0] == '2')
	{
		if(pChartBInput->m_sStartTime.IsEmpty()==FALSE)
		{
			sprintf(lsRelpsa, "RELPSA=%.6s:%.4s", pChartBInput->m_sStartTime, pChartBInput->sShcode);
			sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@%s@CB=_Close_:1|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%ld@OHL=:%.2f:%.2f:%.2f@",
				lcChtAE, lsRelpsa,
				gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
				pChartBInput->lFirstClose,	//(double)(atof(pChartBInput->m_sPrice)/gChRATE_DATA[nOption]),
				atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));			
		}
		else
		{
			sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@@CB=_Close_:1|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%ld@OHL=:%.2f:%.2f:%.2f@",
				lcChtAE,
				gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
				pChartBInput->lFirstClose, //(double)(atof(pChartBInput->m_sPrice)/gChRATE_DATA[nOption]),
				atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));			
		}
	}
	else if (pChartBInput->sTerm[0] == '3')
	{
		sprintf(lsBojo, "####UM=%c@UD=2@RDATET=YYYYMMDD@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%ld@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE,  (double)pChartBInput->lPreprice/gChRATE_DATA[nOption],
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			pChartBInput->lFirstClose, //(double)(atof(pChartBInput->m_sPrice)/gChRATE_DATA[nOption]),
			atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));			
	}
	else if (pChartBInput->sTerm[0] == '4')
	{
		sprintf(lsBojo, "####UM=%c@UD=3@RDATET=YYYYMM@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%ld@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE, (double)pChartBInput->lPreprice/gChRATE_DATA[nOption],
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			pChartBInput->lFirstClose, atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else if (pChartBInput->sTerm[0] == '5')
	{
		sprintf(lsBojo, "####UM=%c@UD=3@RDATET=YYYYMM@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%ld@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE, (double)pChartBInput->lPreprice/gChRATE_DATA[nOption],
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			pChartBInput->lFirstClose, atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else if (pChartBInput->sTerm[0] == '6')
	{
		sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=MMDDHHMM@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%ld@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE, pChartBInput->lTick, (double)pChartBInput->lPreprice/gChRATE_DATA[nOption], (double)pChartBInput->lVolume, 
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			pChartBInput->lFirstClose, atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else
	{
		return 0;
	}

	rString = lsBojo;
	
	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	CString szTmpExt;
	szTmpExt.Format("DATATIME=%s@", pChartBInput->m_sLastDataTime);
	rString += szTmpExt;		

	return rString.GetLength();
}

// ---------------------------------------------------------------------------
//			�ֽļ��� �����޽���(MakeChtBojo_ctsfo)
// ---------------------------------------------------------------------------
int CChartBozoManager::MakeChtBojo_ctsfo(CChartBozoInput* pChartBInput, CString& rString, int nOption)
{
	char    lsBMrk[800], lsBojo[800], lsRelpsa[128], lcChtAE;

	long sPrice = pChartBInput->lPrice;

	memset(lsBMrk      , 0x20, sizeof(lsBMrk      ));
	memset(lsBojo      , 0x20, sizeof(lsBojo      ));
	memset(lsRelpsa    , 0x20, sizeof(lsRelpsa    ));

	if (pChartBInput->sNxtFlg[0] == '1')	lcChtAE = 'A';
	else									lcChtAE = 'E';

	if (pChartBInput->nMarket == 1)	// ����
	{
	}
	else // �ɼ�
	{
		pChartBInput->lUpLmtPrice  = 0;
		pChartBInput->lDnLmtPrice = 0;
	}

	//  ��Ʈ : 0-������Ʈ, 1-�����Ʈ, 2-�޷���Ʈ, 3-���簡, 4-_Open_��Ʈ
	if(pChartBInput->sDataType[0] == '0')
	{
		sprintf(lsBMrk, "BOUNDMARK=������Ʈ:%ld,%ld:%.8s:%s:%c@PREVPRICE=_Close_:%s,_VolumeAmount_:%s@OHL=:%ld:%ld:%ld@",
			pChartBInput->lUpLmtPrice, pChartBInput->lDnLmtPrice,
			pChartBInput->sShcode,  
			pChartBInput->m_sPreprice, 
			'1',
			pChartBInput->m_sBaseValue,
			pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
			pChartBInput->lOpen, pChartBInput->lHigh, pChartBInput->lLow);
	}
	else
	{
		sprintf(lsBMrk, "BOUNDMARK=�����Ʈ:%.2f,%.2f:%.8s:%.2f:%c@PREVPRICE=_Close_:%s,_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%ld:%ld:%ld@",
			CALC_CHGRATE(pChartBInput->lUpLmtPrice,sPrice),
			CALC_CHGRATE(pChartBInput->lDnLmtPrice,sPrice),
			pChartBInput->sShcode, 
			CALC_CHGRATE(sPrice,sPrice),
			'1',
			pChartBInput->m_sPreprice,
			pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			pChartBInput->lOpen, pChartBInput->lHigh, pChartBInput->lLow);
	}

	if (pChartBInput->sTerm[0] == '0')
	{
    	sprintf(lsBojo, "####UM=%c@UD=5@UT=%ld@RDATET=DDHHMMSS@RESETUSEREAL=1@%s@CB=_Close_:0|@PREVPRICE=_Close_:%s|_Volume_:%10ld@SCD=%ld@",
	       	lcChtAE, pChartBInput->lTick,lsBMrk, pChartBInput->m_sBaseValue, pChartBInput->lVolume, pChartBInput->lFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '1')
	{
		sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=MMDDHHMM@RESETUSEREAL=1@%s@CB=_Close_:0|@PREVPRICE=_Close_:%s|_Volume_:%10ld@SCD=%ld@",
			lcChtAE, pChartBInput->lTick, lsBMrk, pChartBInput->m_sBaseValue, pChartBInput->lVolume, pChartBInput->lFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '2')
	{
		if(pChartBInput->m_sStartTime.IsEmpty()==FALSE)
		{
			sprintf(lsRelpsa, "RELPSA=%.6s:%.8s@", pChartBInput->m_sStartTime, pChartBInput->sShcode);
			sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%ld@", lcChtAE, lsBMrk, lsRelpsa, pChartBInput->lFirstClose);
		}
		else
		{
			sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%ld@", lcChtAE, lsBMrk, pChartBInput->lFirstClose);
		}
	}
	else if (pChartBInput->sTerm[0] == '3')
	{
		sprintf(lsBojo, "####UM=%c@UD=2@RDATET=YYYYMMDD@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%ld@", lcChtAE, lsBMrk, pChartBInput->lFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '4')
	{
		sprintf(lsBojo, "####UM=%c@UD=3@RDATET=YYYYMM@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%ld@", lcChtAE, lsBMrk, pChartBInput->lFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '6')
	{
		sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=DDHHMMSS@RESETUSEREAL=1@%s@CB=_Close_:0|@PREVPRICE=_Close_:%ld|_Volume_:%10ld@SCD=%ld@",
			lcChtAE, pChartBInput->lTick, lsBMrk, sPrice, pChartBInput->lVolume, pChartBInput->lFirstClose);
	}
	else
	{
		return 0;
	}

	rString = lsBojo;

	CString szTmpExt;
	if(pChartBInput->m_szMarketTime.IsEmpty()==FALSE)
	{
		szTmpExt.Format("MARKETTIME=%s@", pChartBInput->m_szMarketTime);
		rString += szTmpExt;
		
	}
	if(pChartBInput->m_szBaseTime.IsEmpty()==FALSE)
	{
		szTmpExt.Format("BASETIME=%s@", pChartBInput->m_szBaseTime);
		rString += szTmpExt;		
	}

	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	szTmpExt.Format("DATATIME=%s@", pChartBInput->m_sLastDataTime);
	rString += szTmpExt;		

	return rString.GetLength();
}


// ---------------------------------------------------------------------------
//			���� �����޽���(MakeChtBojo_ctfo)
// ---------------------------------------------------------------------------
int CChartBozoManager::MakeChtBojo_ctfo(CChartBozoInput* pChartBInput, CString& rString, int nOption)
{
	char 	lsBojo[800], lsRelpsa[128], lcChtAE;
	double 	recprice=0.0, volume=0.0, price=0.0, high=0.0, low=0.0;

	recprice = (double)pChartBInput->lPrice;
	volume	 = (double)pChartBInput->lVolume;

	memset(lsBojo      , 0x20, sizeof(lsBojo      ));
	memset(lsRelpsa    , 0x20, sizeof(lsRelpsa    ));

	if (pChartBInput->sNxtFlg[0] == '1')	lcChtAE = 'A';
	else									lcChtAE = 'E';

	if 	(pChartBInput->sTerm[0] == '0')
	{
		sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%.2f,%.2f:%s:1:%s:%c@UD=5@UT=%ld@RDATET=DDHHMMSS@CB=_Close_:2|@PREVPRICE=_Close_:%s|_Volume_:%12.0f,_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%.2f@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE, 
			atof(pChartBInput->m_sUpLmtPrice), 
			atof(pChartBInput->m_sDnLmtPrice), 
			pChartBInput->sShcode,
			pChartBInput->m_sPreprice,	//recprice/100.0, 
			'1',
			pChartBInput->lTick, 
			pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,
			volume, 
			pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			atof(pChartBInput->m_sFirstClose), 
			atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else if (pChartBInput->sTerm[0] == '1')
	{
		if(pChartBInput->lTick == 0)
		{
			sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%.2f,%.2f:%s:1:%s:%c@UD=4@UT=%d0@RDATET=DDHHMMSS@CB=_Close_:2|@PREVPRICE=_Close_:%s|_Volume_:%12.0f,_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%.2f@OHL=:%.2f:%.2f:%.2f@", 
				lcChtAE,
				atof(pChartBInput->m_sUpLmtPrice), 
				atof(pChartBInput->m_sDnLmtPrice), 
				pChartBInput->sShcode,
				pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,	//recprice/100.0,
				'1',
				5,
				pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,
				volume,
				pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
				gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
				atof(pChartBInput->m_sFirstClose),
				atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
		}
		else
		{
			sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%.2f,%.2f:%s:1:%s:%c@UD=4@UT=%ld@RDATET=MMDDHHMM@CB=_Close_:2|@PREVPRICE=_Close_:%s|_Volume_:%12.0f,_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%.2f@OHL=:%.2f:%.2f:%.2f@",
				lcChtAE,
				atof(pChartBInput->m_sUpLmtPrice), 
				atof(pChartBInput->m_sDnLmtPrice), 
				pChartBInput->sShcode,
				pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,	//recprice/100.0,
				'1',
				pChartBInput->lTick,
				pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,
				volume,
				pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
				gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
				atof(pChartBInput->m_sFirstClose),
				atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
		}
	}
	else if (pChartBInput->sTerm[0] == '2')
	{
		//if(pChartBInput->lOpen == 0)
		if(pChartBInput->m_sStartTime.IsEmpty()==FALSE)
		{
			sprintf(lsRelpsa, "RELPSA=%.6s:%.8s", pChartBInput->m_sStartTime, pChartBInput->sShcode);
			sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%.2f,%.2f:%s:1:%s:%c@UD=1@RDATET=YYYYMMDD@CB=_Close_:2|@%s@PREVPRICE=_Close_:%s,_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%.2f@OHL=:%.2f:%.2f:%.2f@",
				lcChtAE,
				atof(pChartBInput->m_sUpLmtPrice), 
				atof(pChartBInput->m_sDnLmtPrice), 
				pChartBInput->sShcode,
				pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,	//recprice/100.0,
				'1',
				lsRelpsa,
				pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,
				pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
				gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
				atof(pChartBInput->m_sFirstClose),
				atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
		}
		else
		{
			sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%.2f,%.2f:%s:1:%s:%c@UD=1@RDATET=YYYYMMDD@CB=_Close_:2|@@PREVPRICE=_Close_:%s,_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%.2f@OHL=:%.2f:%.2f:%.2f@",
				lcChtAE,
				atof(pChartBInput->m_sUpLmtPrice), 
				atof(pChartBInput->m_sDnLmtPrice), 
				pChartBInput->sShcode,
				pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,	//recprice/100.0,
				'1',
				pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,
				pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
				gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
				atof(pChartBInput->m_sFirstClose),
				atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
		}
	}
	else if (pChartBInput->sTerm[0] == '3')
	{
		sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%.2f,%.2f:%s:1:%s:%c@UD=2@RDATET=YYYYMMDD@CB=_Close_:2|@PREVPRICE=_Close_:%s,_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%.2f@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE,
			atof(pChartBInput->m_sUpLmtPrice), 
			atof(pChartBInput->m_sDnLmtPrice), 
			pChartBInput->sShcode,
			pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,	//recprice/100.0,
			'1',
			pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,
			pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			atof(pChartBInput->m_sFirstClose),
			atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else if (pChartBInput->sTerm[0] == '4')
	{
		sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%.2f,%.2f:%s:1:%s:%c@UD=3@RDATET=YYYYMM@CB=_Close_:2|@PREVPRICE=_Close_:%s,_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%.2f@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE,
			atof(pChartBInput->m_sUpLmtPrice), 
			atof(pChartBInput->m_sDnLmtPrice), 
			pChartBInput->sShcode,
			pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,	//recprice/100.0,
			'1',
			pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,
			pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			atof(pChartBInput->m_sFirstClose),
			atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else if (pChartBInput->sTerm[0] == '6')
	{
		sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%.2f,%.2f:%s:1:%s:%c@UD=4@UT=%ld@RDATET=DDHHMMSS@CB=_Close_:2|@PREVPRICE=_Close_:%s|_Volume_:%12.0f,_VolumeAmount_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@SCD=%.2f@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE,
			atof(pChartBInput->m_sUpLmtPrice), 
			atof(pChartBInput->m_sDnLmtPrice), 
			pChartBInput->sShcode,
			pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,	//recprice/100.0,
			'1',
			pChartBInput->lTick,
			pChartBInput->m_sPreprice,	//(double)pChartBInput->lPreprice,
			volume,
			pChartBInput->m_sVolAmt, //[A00000651][A00000652]�����ŷ����.
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			atof(pChartBInput->m_sFirstClose), 
			atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else
	{
		return 0;
	}

	rString = lsBojo;

	CString szTmpExt;
	if(pChartBInput->m_szMarketTime.IsEmpty()==FALSE)
	{
		szTmpExt.Format("MARKETTIME=%s@", pChartBInput->m_szMarketTime);
		rString += szTmpExt;
		
	}
	if(pChartBInput->m_szBaseTime.IsEmpty()==FALSE)
	{
		szTmpExt.Format("BASETIME=%s@", pChartBInput->m_szBaseTime);
		rString += szTmpExt;		
	}

	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	szTmpExt.Format("DATATIME=%s@", pChartBInput->m_sLastDataTime);
	rString += szTmpExt;		

	return rString.GetLength();
}

// ---------------------------------------------------------------------------
//			�ؿ� �����޽���(MakeChtBojo_ctfrn)
// ---------------------------------------------------------------------------
int CChartBozoManager::MakeChtBojo_ctfrn(CChartBozoInput* pChartBInput, CString& rString, int nOption)
{
 	char lsBojo[800], lsRelpsa[128], lcChtAE;
 	
 	memset(lsBojo      , 0x20, sizeof(lsBojo      ));
 	memset(lsRelpsa    , 0x20, sizeof(lsRelpsa    ));

	if (pChartBInput->sNxtFlg[0] == '1')	lcChtAE = 'A';
	else									lcChtAE = 'E';
	
	if	(pChartBInput->sTerm[0] == '0')
		//sprintf(lsBojo, "####UM=%c@UD=5@UT=%ld@RDATET=DDHHMMSS@@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|_Volume_:%12ld@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%10ld:%10ld:%10ld@",
		sprintf(lsBojo, "####UM=%c@UD=5@UT=%ld@RDATET=DDHHMMSS@@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|_Volume_:%12ld@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%.2f:%.2f:%.2f@",
		lcChtAE,
		pChartBInput->lTick,
		(double)pChartBInput->lPreprice/gChRATE_DATA[nOption],
		pChartBInput->lVolume,
		gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
		atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	else if (pChartBInput->sTerm[0] == '1')
		//sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=MMDDHHMM@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|_Volume_:%12ld@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%10ld:%10ld:%10ld@",
		sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=MMDDHHMM@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|_Volume_:%12ld@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%.2f:%.2f:%.2f@",
		lcChtAE,
		pChartBInput->lTick, 
		(double)pChartBInput->lPreprice/gChRATE_DATA[nOption],
		pChartBInput->lVolume, 
		gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
		atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	else if (pChartBInput->sTerm[0] == '2')
	{
		//sprintf(envpath, "%s/DRfn/env", getenv("HOME"));
		//GetProfileStr(envpath, "Sise.cfg", "MINTIME_SET", "MKSTIME", "090000", pChartBInput->m_sStartTime);
		
		//if (atof(pChartBInput->m_sOpen) == 0)
		if(pChartBInput->m_sStartTime.IsEmpty()==FALSE)
		{
			sprintf(lsRelpsa, "RELPSA=%.6s:%.4s", pChartBInput->m_sStartTime, pChartBInput->sShcode);
			//sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@%s@CB=_Close_:1|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%10ld:%10ld:%10ld@",
			sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@%s@CB=_Close_:1|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%.2f:%.2f:%.2f@",
				lcChtAE, lsRelpsa, 
				gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
				atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
		}
		else
		{
			//sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@@CB=_Close_:1|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%10ld:%10ld:%10ld@",
			sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%.2f:%.2f:%.2f@",
				lcChtAE, (double)pChartBInput->lPreprice/gChRATE_DATA[nOption], 
				gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
				atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
		}
	}
	else if (pChartBInput->sTerm[0] == '3')
	{
		//sprintf(lsBojo, "####UM=%c@UD=2@RDATET=YYYYMMDD@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%10ld:%10ld:%10ld@",
		sprintf(lsBojo, "####UM=%c@UD=2@RDATET=YYYYMMDD@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE, (double)pChartBInput->lPreprice/gChRATE_DATA[nOption], 
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else if (pChartBInput->sTerm[0] == '4')
	{
		//sprintf(lsBojo, "####UM=%c@UD=3@RDATET=YYYYMM@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%10ld:%10ld:%10ld@",
		sprintf(lsBojo, "####UM=%c@UD=3@RDATET=YYYYMM@CB=_Close_:1|@PREVPRICE=_Close_:%.2f|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:3:_Open_,|%s:3:_High_,|%s:3:_Low_,|%s:3:_Close_,@OHL=:%.2f:%.2f:%.2f@",
			lcChtAE,
			(double)pChartBInput->lPreprice/gChRATE_DATA[nOption], 
			gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtTDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption], gChtRDC_RATE[nOption],
			atof(pChartBInput->m_sOpen), atof(pChartBInput->m_sHigh), atof(pChartBInput->m_sLow));
	}
	else
	{
		return 0;
	}
	rString = lsBojo;
	
	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	CString szTmpExt;
	szTmpExt.Format("DATATIME=%s@", pChartBInput->m_sLastDataTime);
	rString += szTmpExt;		

	return rString.GetLength();
}


// ---------------------------------------------------------------------------
//			�������� �����޽���(MakeChtBojo_market)
// ---------------------------------------------------------------------------
int CChartBozoManager::MakeChtBojo_market(CChartBozoInput* pChartBInput, CString& rString, int nOption)
{
	char cDec = '0', cChtAP;

	if (pChartBInput->sNxtFlg[0] == '1')	cChtAP = 'A';
	else									cChtAP = 'P';

	rString.Format(_T("0396UM=%c@"), cChtAP);

	return rString.GetLength();
}

// ---------------------------------------------------------------------------
//			�ؿܼ��� �����޽���(MakeChtBojo_ctfofor)
// ----------------------------------------------------------------------------
int CChartBozoManager::MakeChtBojo_ctfofor(CChartBozoInput* pChartBInput, CString& rString, int nOption)
{
	char 	lsBojo[800], lsRelpsa[128], lcChtAE;
	double 	recprice=0.0, volume=0.0, price=0.0, high=0.0, low=0.0;

	recprice = (double)pChartBInput->lPrice;
	volume	 = (double)pChartBInput->lVolume;

	memset(lsBojo      , 0x20, sizeof(lsBojo      ));
	memset(lsRelpsa    , 0x20, sizeof(lsRelpsa    ));

	if (pChartBInput->sNxtFlg[0] == '1')	lcChtAE = 'A';
	else									lcChtAE = 'E';

//vntsorl_20110420:[A00000410] ����Ʈ���� ù���� ���ο� ���� ����� ��������
	CString strPacketTime;
	CString strTDC;
	strTDC.Format(_T("%.*f"), nOption, 1.0/pow(10, nOption));
	if 	(pChartBInput->sTerm[0] == '0')
	{
		// ƽ
		//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=5@UT=%ld@RDATET=DDHHMMSS@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
    	
		// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=5@UT=%ld@RDATET=DDHHMMSS@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=5@UT=%ld@RDATET=YYYYMMDDHHMMSS@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",

		if(pChartBInput->nPacketType==1)
			strPacketTime = "DDHHMMSS";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=5@UT=%ld@RDATET=%s@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@DATATIME=%s@",
		// 2011.01.24 by SYS <<
		//>>
			lcChtAE, 
			pChartBInput->m_sUpLmtPrice, 
			pChartBInput->m_sDnLmtPrice, 
			pChartBInput->sShcode,
			recprice/100.0, 
			'1',
			pChartBInput->lTick, 
			strPacketTime,
			//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
			pChartBInput->m_sPreprice,//(double)pChartBInput->lPreprice/100.0,
			volume, 
			//>>
			strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC,
			pChartBInput->m_sFirstClose,//(double)pChartBInput->lFirstClose/100.0, 
			pChartBInput->m_sOpen, pChartBInput->m_sHigh, pChartBInput->m_sLow,
			pChartBInput->m_sLastDataTime);
	}
	else if (pChartBInput->sTerm[0] == '1')
	{
		if(pChartBInput->lTick == 0)
		{
			//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%d0@RDATET=DDHHMMSS@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%.2f@OHL=:%s:%s:%s@", 
			// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%d0@RDATET=DDHHMMSS@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%.2f@OHL=:%s:%s:%s@", 
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%d0@RDATET=YYYYMMDDHHMMSS@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%.2f@OHL=:%s:%s:%s@", 

			if(pChartBInput->nPacketType==1)
				strPacketTime = "DDHHMMSS";
			else
				strPacketTime = "YYYYMMDDHHMMSS";

			sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%d0@RDATET=%s@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%.2f@OHL=:%s:%s:%s@DATATIME=%s@", 
			// 2011.01.24 by SYS <<
			//>>
				lcChtAE,
				pChartBInput->m_sUpLmtPrice, 
				pChartBInput->m_sDnLmtPrice, 
				pChartBInput->sShcode,
				recprice/100.0,
				'1',
				5,
				strPacketTime,
				//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
				pChartBInput->m_sPreprice,	
				volume,
				//>>
				strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC,
				pChartBInput->m_sFirstClose,//(double)pChartBInput->lFirstClose/100.0, 
				pChartBInput->m_sOpen, pChartBInput->m_sHigh, pChartBInput->m_sLow,
				pChartBInput->m_sLastDataTime);
		}
		else
		{
			// �а�
			//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%ld@RDATET=MMDDHHMM@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
			
			// 2011.01.17 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%ld@RDATET=MMDDHHMM@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%ld@RDATET=YYYYMMDDHHMMSS@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",

			if(pChartBInput->nPacketType==1)
				strPacketTime = "MMDDHHMM";
			else
				strPacketTime = "YYYYMMDDHHMMSS";

			sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%ld@RDATET=%s@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@DATATIME=%s@",
			// 2011.01.17 by SYS <<
			
			//>>
				lcChtAE,
				pChartBInput->m_sUpLmtPrice, 
				pChartBInput->m_sDnLmtPrice, 
				pChartBInput->sShcode,
				recprice/100.0,
				'1',
				pChartBInput->lTick,
				strPacketTime,
				//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
				pChartBInput->m_sPreprice,//(double)pChartBInput->lPreprice/100.0,
				volume,
				//>>
				strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC,
				pChartBInput->m_sFirstClose,
				pChartBInput->m_sOpen, pChartBInput->m_sHigh, pChartBInput->m_sLow,
				pChartBInput->m_sLastDataTime);
		}
	}
	else if (pChartBInput->sTerm[0] == '2')
	{
		// �ϰ�
		//if(pChartBInput->lOpen == 0)
		if(pChartBInput->m_sStartTime.IsEmpty()==FALSE)
		{
			sprintf(lsRelpsa, "RELPSA=%.6s:%.8s", pChartBInput->m_sStartTime, pChartBInput->sShcode);
			// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=1@RDATET=YYYYMMDD@CB=_Close_:2|_Volume_:6|@%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=1@RDATET=YYYYMMDDHHMMSS@CB=_Close_:2|_Volume_:6|@%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",

			if(pChartBInput->nPacketType==1)
				strPacketTime = "YYYYMMDD";
			else
				strPacketTime = "YYYYMMDDHHMMSS";

			sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=1@RDATET=%s@CB=_Close_:2|_Volume_:6|@%s@PREVPRICE=_Close_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@DATATIME=%s@",
			// 2011.01.20 by SYS <<
				lcChtAE,
				pChartBInput->m_sUpLmtPrice, 
				pChartBInput->m_sDnLmtPrice, 
				pChartBInput->sShcode,
				recprice/100.0,
				'1',
				strPacketTime,
				lsRelpsa,
				pChartBInput->m_sPreprice,//JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
				strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC,
				pChartBInput->m_sFirstClose,//(double)pChartBInput->lFirstClose/100.0, 
				pChartBInput->m_sOpen, pChartBInput->m_sHigh, pChartBInput->m_sLow,
				pChartBInput->m_sLastDataTime);
		}
		else
		{
			//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=1@RDATET=YYYYMMDD@CB=_Close_:2|_Volume_:6|@@PREVPRICE=_Close_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
			// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=1@RDATET=YYYYMMDD@CB=_Close_:2|_Volume_:6|@@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
			//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=1@RDATET=YYYYMMDDHHMMSS@CB=_Close_:2|_Volume_:6|@@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",

			if(pChartBInput->nPacketType==1)
				strPacketTime = "YYYYMMDD";
			else
				strPacketTime = "YYYYMMDDHHMMSS";

			sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=1@RDATET=%s@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@DATATIME=%s@",
			// 2011.01.20 by SYS 
			//>>
				lcChtAE,
				pChartBInput->m_sUpLmtPrice, 
				pChartBInput->m_sDnLmtPrice, 
				pChartBInput->sShcode,
				recprice/100.0,
				'1',
				strPacketTime,
				pChartBInput->m_sPreprice,//JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
				strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC,
				pChartBInput->m_sFirstClose,//(double)pChartBInput->lFirstClose/100.0, 
				pChartBInput->m_sOpen, pChartBInput->m_sHigh, pChartBInput->m_sLow,
				pChartBInput->m_sLastDataTime);
		}
	}
	else if (pChartBInput->sTerm[0] == '3')
	{
		//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
   		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=2@RDATET=YYYYMMDD@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
		// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
   		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=2@RDATET=YYYYMMDD@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=2@RDATET=YYYYMMDDHHMMSS@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",

		if(pChartBInput->nPacketType==1)
			strPacketTime = "YYYYMMDD";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=2@RDATET=%s@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@DATATIME=%s@",
		// 2011.01.24 by SYS <<
		//>>
			lcChtAE,
			pChartBInput->m_sUpLmtPrice, 
			pChartBInput->m_sDnLmtPrice, 
			pChartBInput->sShcode,
			recprice/100.0,
			'1',
			strPacketTime,
			pChartBInput->m_sPreprice,//JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
			strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC,
			pChartBInput->m_sFirstClose,//(double)pChartBInput->lFirstClose/100.0, 
			pChartBInput->m_sOpen, pChartBInput->m_sHigh, pChartBInput->m_sLow,
			pChartBInput->m_sLastDataTime);
	}
	else if (pChartBInput->sTerm[0] == '4')
	{
		//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=3@RDATET=YYYYMM@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
		// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=3@RDATET=YYYYMM@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=3@RDATET=YYYYMMDDHHMMSS@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",

		if(pChartBInput->nPacketType==1)
			strPacketTime = "YYYYMM";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=3@RDATET=%s@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@DATATIME=%s@",
		// 2011.01.24 by SYS <<
		//>>
			lcChtAE,
			pChartBInput->m_sUpLmtPrice, 
			pChartBInput->m_sDnLmtPrice, 
			pChartBInput->sShcode,
			recprice/100.0,
			'1',
			strPacketTime,
			pChartBInput->m_sPreprice,//JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
			strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC,
			pChartBInput->m_sFirstClose,//(double)pChartBInput->lFirstClose/100.0, 
			pChartBInput->m_sOpen, pChartBInput->m_sHigh, pChartBInput->m_sLow,
			pChartBInput->m_sLastDataTime);
	}
	else if (pChartBInput->sTerm[0] == '6')
	{
		//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%ld@RDATET=DDHHMMSS@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
		// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%ld@RDATET=DDHHMMSS@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",
		//sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%ld@RDATET=YYYYMMDDHHMMSS@CB=_Close_:2|_Volume_:6|@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@",

		if(pChartBInput->nPacketType==1)
			strPacketTime = "DDHHMMSS";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@BOUNDMARK=������Ʈ:%s,%s:%s:1:%.2f:%c@UD=4@UT=%ld@RDATET=%s@CB=_Close_:2|_Volume_:6|@PREVPRICE=_Close_:%s|_Volume_:%12.0f@TDC=%s:_Open_,|%s:_High_,|%s:_Low_,|%s:_Close_,@RDC=%s:-1:_Open_,|%s:-1:_High_,|%s:-1:_Low_,|%s:-1:_Close_,@SCD=%s@OHL=:%s:%s:%s@DATATIME=%s@",
		// 2011.01.24 by SYS <<
		//>>
			lcChtAE,
			pChartBInput->m_sUpLmtPrice, 
			pChartBInput->m_sDnLmtPrice, 
			pChartBInput->sShcode,
			recprice/100.0,
			'1',
			pChartBInput->lTick,
			strPacketTime,
			//<<JS.Kim_20101012 ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
			pChartBInput->m_sPreprice,
			volume,
			//>>
			strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC, strTDC,
			pChartBInput->m_sFirstClose,//(double)pChartBInput->lFirstClose/100.0, 
			pChartBInput->m_sOpen, pChartBInput->m_sHigh, pChartBInput->m_sLow,
			pChartBInput->m_sLastDataTime);
	}
	else
	{
		return 0;
	}


	rString = lsBojo;
	rString += _T("REALTIMET=YYYYMMDDHHMMSS@RESETUSEREAL=1@");

	return rString.GetLength();
}


//<<20100310_JS.Kim FX
// ---------------------------------------------------------------------------
//			FX �����޽���(MakeChtBojo_ctfx)
// ---------------------------------------------------------------------------
int CChartBozoManager::MakeChtBojo_ctfx(CChartBozoInput* pChartBInput, CString& rString, int nOption)
{
	char    lsBMrk[800], lsBojo[800], lsRelpsa[128], lcChtAE;
	long    lsrecprice=0;

    memset(lsBMrk      , 0x20, sizeof(lsBMrk      ));
    memset(lsBojo      , 0x20, sizeof(lsBojo      ));
    memset(lsRelpsa    , 0x20, sizeof(lsRelpsa    ));

	if (pChartBInput->sNxtFlg[0] == '1')	lcChtAE = 'A';
	else									lcChtAE = 'E';

	if(pChartBInput->sDataType[0] == '5') lsrecprice = 0;
	else	 lsrecprice   = pChartBInput->lPrice;

	pChartBInput->nMarket = 1; //���������� ������ ���� �ʱ⶧���� ������Ŵ.

	//	��Ʈ : 0-������Ʈ, 1-�����Ʈ, 2-�޷���Ʈ, 3-���簡, 4-_Open_��Ʈ
//	if(pChartBInput->sDataType[0] == '0')
	{
		sprintf(lsBMrk, "BOUNDMARK=������Ʈ:%ld,%ld:%.6s:1:%ld:%c@PREVPRICE=_Close_:%s@OHL=:%s:%s:%s@PREVPRICE2=%s:%s:%s:%s@",
			//>>
			pChartBInput->lUpLmtPrice, 
			pChartBInput->lDnLmtPrice, 
			pChartBInput->sShcode, 
			lsrecprice,
			'1', 
			pChartBInput->m_sPreprice, //JS.Kim ������ǥ�� StandartValue�� ���ϰ����� ���õǸ鼭 y�� ���� �Ҽ������� ���� �̻�����
			pChartBInput->m_sOpen, pChartBInput->m_sHigh, pChartBInput->m_sLow,
						pChartBInput->m_sPreHigh, pChartBInput->m_sPreOpen, pChartBInput->m_sPreprice, pChartBInput->m_sPreLow);

		if( nOption > 1 )
		{
			CString strPacketType;
			strPacketType.Format("x %.*f", (int)log10(nOption), (double)1/(double)nOption); 

			CString strBMsg;
			strBMsg.Format("RESETPACKET=_Close_:10:%s|_Open_:10:%s|_High_:10:%s|_Low_:10:%s@", strPacketType, strPacketType, strPacketType, strPacketType);
			strcat(lsBMrk, strBMsg);
		}
	} 

//vntsorl_20110420:[A00000410] ����Ʈ���� ù���� ���ο� ���� ����� ��������
	CString strPacketTime;
	// ��Ʈ �޽���
	if 		(pChartBInput->sTerm[0] == '0')	//����(0:ƽ/1:��/2:��/3:��/4:��/6:��)
	{
		// ƽ �ֱ�
		// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//sprintf(lsBojo, "####UM=%c@UD=5@UT=%ld@RDATET=DDHHMMSS@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",
		//sprintf(lsBojo, "####UM=%c@UD=5@UT=%ld@RDATET=YYYYMMDDHHMMSS@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",

		if(pChartBInput->nPacketType==1)
			strPacketTime = "DDHHMMSS";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@UD=5@UT=%ld@RDATET=%s@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",
		// 2011.01.24 by SYS <<
	       	lcChtAE, pChartBInput->lTick, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '1' )
	{
		// �� �ֱ�
		// 2011.01.17 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=MMDDHHMM@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",
		//sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=YYYYMMDDHHMMSS@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",

		if(pChartBInput->nPacketType==1)
			strPacketTime = "MMDDHHMM";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=%s@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",
		// 2011.01.17 by SYS <<
	       	lcChtAE, pChartBInput->lTick, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '2')
	{
		// �� �ֱ�
		BOOL bMarket = (pChartBInput->nMarket == 1 )|| 
			(pChartBInput->nMarket == 2 )|| 
			(pChartBInput->nMarket == 3 );

        if (bMarket && pChartBInput->m_sStartTime.IsEmpty()==FALSE)
        {
            sprintf(lsRelpsa, "RELPSA=%.6s:%.6s", 
				pChartBInput->m_sStartTime, 
				pChartBInput->sShcode);
           	
			// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
			//sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@RESETUSEREAL=1@%s@%s@CB=_Close_:0|@SCD=%s@",
			//sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDDHHMMSS@RESETUSEREAL=1@%s@%s@CB=_Close_:0|@SCD=%s@",

			if(pChartBInput->nPacketType==1)
				strPacketTime = "YYYYMMDD";
			else
				strPacketTime = "YYYYMMDDHHMMSS";

			sprintf(lsBojo, "####UM=%c@UD=1@RDATET=%s@RESETUSEREAL=1@%s@%s@CB=_Close_:0|@SCD=%s@",
				lcChtAE, strPacketTime, lsBMrk, lsRelpsa, pChartBInput->m_sFirstClose);
			// 2011.01.20 by SYS 
        }
		else
        {
			// 2011.01.20 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
           	//sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDD@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",
			//sprintf(lsBojo, "####UM=%c@UD=1@RDATET=YYYYMMDDHHMMSS@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",

			if(pChartBInput->nPacketType==1)
				strPacketTime = "YYYYMMDD";
			else
				strPacketTime = "YYYYMMDDHHMMSS";

			sprintf(lsBojo, "####UM=%c@UD=1@RDATET=%s@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",
				lcChtAE, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
			// 2011.01.20 by SYS 
        }

	}
	else if (pChartBInput->sTerm[0] == '3')
	{
		// �� �ֱ�
		// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//sprintf(lsBojo, "####UM=%c@UD=2@RDATET=YYYYMMDD@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",
		//sprintf(lsBojo, "####UM=%c@UD=2@RDATET=YYYYMMDDHHMMSS@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",

		if(pChartBInput->nPacketType==1)
			strPacketTime = "YYYYMMDD";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@UD=2@RDATET=%s@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",
		// 2011.01.24 by SYS <<
		lcChtAE, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '4')
	{
		// �� �ֱ�
		// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//sprintf(lsBojo, "####UM=%c@UD=3@RDATET=YYYYMM@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",
		//sprintf(lsBojo, "####UM=%c@UD=3@RDATET=YYYYMMDDHHMMSS@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",

		if(pChartBInput->nPacketType==1)
			strPacketTime = "YYYYMM";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@UD=3@RDATET=%s@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",
		// 2011.01.24 by SYS <<
		lcChtAE, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '6')
	{
		// �� �ֱ�
		// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
		//sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=DDHHMMSS@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",
		//sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=YYYYMMDDHHMMSS@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",

		if(pChartBInput->nPacketType==1)
			strPacketTime = "DDHHMMSS";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=%s@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",
		// 2011.01.24 by SYS <<
	       	lcChtAE, pChartBInput->lTick, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
	}

	rString = lsBojo;

	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	CString szTmpExt;
	szTmpExt.Format("DATATIME=%s@", pChartBInput->m_sLastDataTime);
	rString += szTmpExt;		

    return rString.GetLength();
}
//>>

//>>20110106 alzioyes CME
// ---------------------------------------------------------------------------
//			CME �����޽���(MakeChtBojo_ctcme)
// ---------------------------------------------------------------------------
int CChartBozoManager::MakeChtBojo_ctcme(CChartBozoInput* pChartBInput, CString& rString, int nOption)
{
	char    lsBMrk[800], lsBojo[800], lsRelpsa[128], lcChtAE;
	long    lsrecprice=0;

    memset(lsBMrk      , 0x20, sizeof(lsBMrk      ));
    memset(lsBojo      , 0x20, sizeof(lsBojo      ));
    memset(lsRelpsa    , 0x20, sizeof(lsRelpsa    ));

	if (pChartBInput->sNxtFlg[0] == '1')	lcChtAE = 'A';
	else									lcChtAE = 'E';

	if(pChartBInput->sDataType[0] == '5') lsrecprice = 0;
	else	 lsrecprice   = pChartBInput->lPrice;

	pChartBInput->nMarket = 1; //���������� ������ ���� �ʱ⶧���� ������Ŵ.

	{
		sprintf(lsBMrk, "BOUNDMARK=_PriceChart_:%s,%s:%s:1:%ld:%c@PREVPRICE=_Close_:%s@OHL=:%s:%s:%s@PREVPRICE2=%s:%s:%s:%s",
						pChartBInput->m_sUpLmtPrice, 
						pChartBInput->m_sDnLmtPrice, 
						pChartBInput->sShcode, 
						lsrecprice,
						'1', 
						pChartBInput->m_sPreprice,
						pChartBInput->m_sOpen, pChartBInput->m_sHigh, pChartBInput->m_sLow,
						pChartBInput->m_sPreHigh, pChartBInput->m_sPreOpen, pChartBInput->m_sPreprice, pChartBInput->m_sPreLow);

		if( nOption > 1 )
		{
			CString strPacketType;
			strPacketType.Format("x %.*f", (int)log10(nOption), (double)1/(double)nOption); 

			CString strBMsg;
			strBMsg.Format("RESETPACKET=_Close_:10:%s|_Open_:10:%s|_High_:10:%s|_Low_:10:%s@", strPacketType, strPacketType, strPacketType, strPacketType);
			strcat(lsBMrk, strBMsg);
		}
	} 

	CString strPacketTime;
	// ��Ʈ �޽���
	if 		(pChartBInput->sTerm[0] == '0')	//����(0:ƽ/1:��/2:��/3:��/4:��/6:��)
	{
		if(pChartBInput->nPacketType==1)
			strPacketTime = "DDHHMMSS";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@UD=5@UT=%ld@RDATET=%s@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",
	       	lcChtAE, pChartBInput->lTick, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '1' )
	{
		// �� �ֱ�
		if(pChartBInput->nPacketType==1)
			strPacketTime = "MMDDHHMM";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=%s@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",
	       	lcChtAE, pChartBInput->lTick, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '2')
	{
		// �� �ֱ�
		BOOL bMarket = (pChartBInput->nMarket == 1 )|| 
			(pChartBInput->nMarket == 2 )|| 
			(pChartBInput->nMarket == 3 );

        if (bMarket && pChartBInput->m_sStartTime.IsEmpty()==FALSE)
        {
            sprintf(lsRelpsa, "RELPSA=%.6s:%.6s", 
				pChartBInput->m_sStartTime, 
				pChartBInput->sShcode);
           	
			if(pChartBInput->nPacketType==1)
				strPacketTime = "YYYYMMDD";
			else
				strPacketTime = "YYYYMMDDHHMMSS";

			sprintf(lsBojo, "####UM=%c@UD=1@RDATET=%s@RESETUSEREAL=1@%s@%s@CB=_Close_:0|@SCD=%s@",
				lcChtAE, strPacketTime, lsBMrk, lsRelpsa, pChartBInput->m_sFirstClose);
        }
		else
        {
			if(pChartBInput->nPacketType==1)
				strPacketTime = "YYYYMMDD";
			else
				strPacketTime = "YYYYMMDDHHMMSS";

			sprintf(lsBojo, "####UM=%c@UD=1@RDATET=%s@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",
				lcChtAE, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
        }

	}
	else if (pChartBInput->sTerm[0] == '3')
	{
		// �� �ֱ�
		if(pChartBInput->nPacketType==1)
			strPacketTime = "YYYYMMDD";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@UD=2@RDATET=%s@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",
		lcChtAE, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '4')
	{
		// �� �ֱ�
		if(pChartBInput->nPacketType==1)
			strPacketTime = "YYYYMM";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@UD=3@RDATET=%s@RESETUSEREAL=1@%s@@CB=_Close_:0|@SCD=%s@",
		lcChtAE, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
	}
	else if (pChartBInput->sTerm[0] == '6')
	{
		// �� �ֱ�
		if(pChartBInput->nPacketType==1)
			strPacketTime = "DDHHMMSS";
		else
			strPacketTime = "YYYYMMDDHHMMSS";

		sprintf(lsBojo, "####UM=%c@UD=4@UT=%ld@RDATET=%s@RESETUSEREAL=1@%s@CB=_Close_:0|@SCD=%s@",
	       	lcChtAE, pChartBInput->lTick, strPacketTime, lsBMrk, pChartBInput->m_sFirstClose);
	}

	

	rString = lsBojo;

	CString szTmpExt;
	if(pChartBInput->m_szMarketTime.IsEmpty()==FALSE)
	{
		szTmpExt.Format("MARKETTIME=%s@", pChartBInput->m_szMarketTime);
		rString += szTmpExt;
		
	}
	if(pChartBInput->m_szBaseTime.IsEmpty()==FALSE)
	{
		szTmpExt.Format("BASETIME=%s@", pChartBInput->m_szBaseTime);
		rString += szTmpExt;		
	}

	// 20130222 �̻�� : �ֺ��ϰ�� ������ �������� ���ڸ� ������ �ش�.(�� ù ������ ����)
	szTmpExt.Format("DATATIME=%s@", pChartBInput->m_sLastDataTime);
	rString += szTmpExt;		

    return rString.GetLength();
}
//<<20110106 alzioyes CME