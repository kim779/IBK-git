#include "stdafx.h"
#include "AxChartInfo.h"

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../h/axisgwin.h"
#include "../../h/axisgbinfo.h"
#include "../Common/SavedHead.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_REGION_COUNT 2
#define DEFAULT_GRAPH_COUNT  4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxChartInfo::CAxChartInfo(CWnd* pView, LPCTSTR lpszMapName)
{
	m_view = pView;
	m_mapName = lpszMapName;

	m_noRestoreGraph = FALSE;
}

CAxChartInfo::~CAxChartInfo()
{
}

void CAxChartInfo::DefaultCountInfo()
{
	m_default_rgnCount = DEFAULT_REGION_COUNT;
	m_default_graphCount = DEFAULT_GRAPH_COUNT;
}

void CAxChartInfo::Initialize()
{
	// 20090916 : DELETE :
	CString strTitle = MakeCaption();
	//CString strTitle;

	m_view->SendMessage(WM_USER, MAKEWPARAM(variantDLL, titleCC), (LPARAM)(LPCTSTR)strTitle);

	CString root = (LPCTSTR)m_view->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC));
	CString name = (LPCTSTR)m_view->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC));

	m_gexDir.Format("%s\\%s\\%s\\", root, GEXDIR, name);

	CheckData(root, name);
}

void CAxChartInfo::CheckData(LPCTSTR lpszRoot, LPCTSTR lpszName)
{
	CFileFind ff;

	if (!ff.FindFile(m_gexDir))
		CreateDirectory(m_gexDir, NULL);

	DefaultCountInfo();
	MakeFile();
}

BOOL CAxChartInfo::MakeFile()
{
	CString	envPath = m_gexDir + m_mapName + ENV;
	CString	grpPath = m_gexDir + m_mapName + GRP;
	CString	datPath = m_gexDir + m_mapName + DAT;

	DWORD dwReadLen;

	if (m_noRestoreGraph) 
	{
		MakeEnvironment(envPath);
		MakeGraphInfo(grpPath);
		MakeData(datPath);
		return true;
	}

	// env
	CSavedHead shEnv;
	char* pcData = shEnv.Read(envPath, dwReadLen);
	if (!pcData || dwReadLen < 1 || shEnv.m_sh.dwVersion != VERSION_ENV) // New가 없다면
		MakeEnvironment(envPath); // New 생성

	// grp
	CSavedHead shGrp;
	pcData = shGrp.Read(grpPath, dwReadLen);
	if (!pcData || dwReadLen < 1 || shGrp.m_sh.dwVersion != VERSION_GRP)	// New가 없다면
		MakeGraphInfo(grpPath); // New 생성

	// dat
	CSavedHead shDat;
	pcData = shDat.Read(datPath, dwReadLen);
	if (!pcData || dwReadLen < 1 || shDat.m_sh.dwVersion != VERSION_DAT)	// New가 없다면
		MakeData(datPath); // New 생성

	return true;
}

void CAxChartInfo::MakeData(LPCTSTR lpszFileName)
{
	struct _cntInfo cntInfo;
	cntInfo.wMainCnt = 20;
	cntInfo.wAidCnt = 60;
	cntInfo.wUserCnt = 0;

	int dataSize = SZ_CNTINFO + SZ_GRAPH * (cntInfo.wMainCnt + cntInfo.wAidCnt + cntInfo.wUserCnt);
	
	LPSTR buffer = new char[dataSize];
	ZeroMemory(buffer, dataSize);
	CopyMemory(buffer, &cntInfo, SZ_CNTINFO);

	InitInfo(&cntInfo, &buffer[SZ_CNTINFO]);

	CSavedHead sh;
	LPSTR bufferSH = sh.Make(VERSION_DAT, 1, dataSize);
	CopyMemory(bufferSH, buffer, dataSize);
	sh.Write(lpszFileName);

	delete [] buffer;
}

void CAxChartInfo::MakeEnvironment(LPCTSTR lpszFileName)
{
	struct	_envInfo envinfo;
	memset(&envinfo, 0x00, SZ_ENVINFO);
	
	envinfo.wVersion = GRP_VERSION;
	sprintf(envinfo.acObjName, "%s", m_mapName);
	envinfo.wEnvOption = EO_SAVE;

	sprintf(envinfo.display.font.acFName, "굴림");
	envinfo.display.font.btFPoint = 9;
	sprintf(envinfo.display.tfont.acFName, "굴림");
	envinfo.display.tfont.btFPoint = 9;
	sprintf(envinfo.display.atick[0].tfont.acFName, "굴림");
	envinfo.display.atick[0].tfont.btFPoint = 9;
	sprintf(envinfo.display.atick[1].tfont.acFName, "굴림");
	envinfo.display.atick[1].tfont.btFPoint = 9;
	sprintf(envinfo.display.atick[2].tfont.acFName, "굴림");
	envinfo.display.atick[2].tfont.btFPoint = 9;

	envinfo.display.rgbBG = RGB(243,249,254);		//
	envinfo.display.rgbEdge = RGB(192,192,192);
	envinfo.display.rgbChartBG = RGB(255,255,255);
	envinfo.display.rgbChartEdge = RGB(180,192,203);	//

	envinfo.display.wLMargin = 2;
	envinfo.display.wRMargin = 20;

	envinfo.display.dwDspOption = 
		DO_SCREEN  | DO_DISPRT  | 
		DO_TKVGRID | DO_TKHGRID | DO_MOD      | DO_LOCK | DO_CURRENT | 
		DO_MINMAX  | DO_USEGRID; /**DO_VIEWGRID**/

	envinfo.display.toolinfo.rgbLine = DEFCR4;
	envinfo.display.toolinfo.btLineStyle = PS_SOLID;
	envinfo.display.toolinfo.btLineWidth = 1;
	envinfo.display.hline.rgbLColor = RGB(180,192,203);	//
	envinfo.display.hline.btLStyle = PS_SOLID;
	envinfo.display.hline.btLWidth = 1;
	envinfo.display.vline.rgbLColor = RGB(180,192,203);
	envinfo.display.vline.btLStyle = PS_SOLID;
	envinfo.display.vline.btLWidth = 1;
	//envinfo.display.dline.rgbLColor = RGB(255, 0, 0);
	envinfo.display.dline.rgbLColor = RGB(251, 133, 133);
	envinfo.display.dline.btLStyle = PS_DOT;
	envinfo.display.dline.btLWidth = 1;
	envinfo.display.cline.rgbLColor = RGB(0, 128, 0);
	envinfo.display.cline.btLStyle = PS_SOLID;
	envinfo.display.cline.btLWidth = 1;

	envinfo.wViewWidth = 800;
	envinfo.wViewHeight = 550;

	envinfo.panel[0].btPnID = PN_INPUT;
	envinfo.panel[0].wPnOption = PO_USE | PO_SHOW;

	envinfo.panel[1].btPnID = PN_SISE;
	envinfo.panel[1].wPnOption = PO_USE | PO_SHOW;

	envinfo.panel[2].btPnID = PN_CTRL;
	envinfo.panel[2].wPnOption = PO_USE | PO_SHOW;

	envinfo.panel[3].btPnID = PN_TOOL;
	envinfo.panel[3].wPnOption = PO_USE | PO_SHOW;

	envinfo.panel[4].btPnID = PN_CFG;
	envinfo.panel[4].wPnOption = PO_USE | PO_SHOW;

	envinfo.panel[5].btPnID = PN_TABLE;
	envinfo.panel[5].wPnOption = PO_USE;	//|PO_SHOW;

	envinfo.panel[6].btPnID = PN_AID;
	envinfo.panel[6].wPnOption = PO_USE;    // | PO_SHOW;

	for (int ii = 0; ii < MAX_INDEX - 1; ii++)
	{
		envinfo.grpinfo[ii].wRgnCnt = m_default_rgnCount;
		envinfo.grpinfo[ii].wGrpCnt = m_default_graphCount;
	}
	envinfo.grpinfo[MAX_INDEX - 1].wRgnCnt = m_default_rgnCount;
	envinfo.grpinfo[MAX_INDEX - 1].wGrpCnt = m_default_graphCount - 2;

	// 0090923 : CHANGE : RSI Weight 지표 추가
	//char	ulist[10] = {GK_PSY, GK_GAP, GK_BOLB, GK_CCI, GK_MACD, GK_MACDOS, GK_PARB, GK_PIVOT, GK_RSI, GK_STOCHS};
	char	ulist[11] = {GK_PSY, GK_GAP, GK_BOLB, GK_CCI, GK_MACD, GK_MACDOS, GK_PARB, GK_PIVOT, GK_RSI, GK_RSIWEIGHT, GK_STOCHS};
	//for (ii = 0; ii < 10; ii++)
	for (ii = 0; ii < 11; ii++)
		envinfo.ulist[ii] = ulist[ii];
	
	// Graph kind - 종목종합
	envinfo.datainfo.btUnit = GU_CODE;
	envinfo.datainfo.btIndex = GI_DAY;
	envinfo.datainfo.wMGap = 1;
	envinfo.datainfo.wTGap = 1;
	envinfo.datainfo.wMainGrpKind = GK_JPN;
	envinfo.datainfo.wUpdate = DU_RTM|DU_DUMMY;

	for (ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < MAX_INDEX; jj++)
		{
			envinfo.datainfo.aaDayInfo[ii][jj].dwTotCnt = 300;
			envinfo.datainfo.aaDayInfo[ii][jj].dwDisCnt = 150;
		}
	}

	CustomEnvironment(&envinfo);

	if (!(envinfo.wEnvOption & EO_SAVE))
		m_noRestoreGraph = TRUE;

	CSavedHead sh;
	char* pcEnv = sh.Make(VERSION_ENV, 1, SZ_ENVINFO);
	CopyMemory(pcEnv, &envinfo, SZ_ENVINFO);
	sh.Write(lpszFileName);
}


void CAxChartInfo::RegionSetting(LPSTR buffer)
{
	struct _region* region;

	const int heights2[] = {
		7000, 3000
	};
	const int heights3[] = {
		4000, 3000, 3000
	};
	// 2007.01.01 schbang : 7520, 7550을 위해
	const int heights4[] = {
		4000, 2000, 2000, 2000
	};

	int y = 0;

	for (int i = 0; i < m_default_rgnCount; i++) {
		region = (struct _region*)(buffer + SZ_REGION * i);

		region->wRTop = y;
		//y += (m_default_rgnCount == 2 ? heights2[i]: heights3[i]);
		// 2007.01.01 schbang : 7550 때문에
		switch (m_default_rgnCount)
		{
		case 2:
			y += heights2[i];
			break;
		case 4:
			y += heights4[i];
			break;
		case 3:
		default:
			y += heights3[i];
			break;

		}
		region->wRBottom = y; 
	}
}


void CAxChartInfo::MakeGraphInfo(LPCSTR lpszFileName)
{
	const COLORREF	MC5 = RGB(255,144,0);
	const COLORREF	MC6 = RGB(220,2,2);
	const COLORREF	MC7 = RGB(165,111,227);
	const COLORREF	MC8 = RGB(10,10,10);

	int iRCnt = m_default_rgnCount;
	int gcnt = m_default_graphCount;

	int iLen = SZ_REGION * iRCnt + SZ_GRAPH * gcnt;
	char* pcBuf = new char[iLen * MAX_INDEX];
	ZeroMemory(pcBuf, iLen);

	struct _region* region;
	struct _graph *graph;
	
	for (int i = 0; i < iRCnt; i++) {
		region = (struct _region*)(pcBuf + SZ_REGION * i);
		region->btRKey = i + 1;
		region->wRLeft = 0;
		region->wRRight = 10000;
	}

	RegionSetting(pcBuf);


	LONG graphBase = SZ_REGION * iRCnt;
///////////////////////////////////////////////////
	graph = (struct _graph *)&pcBuf[graphBase];
	graph->wKey = 1;
	graph->btRgnKey = 1;
	graph->btDataKey = 1;
	graph->btGType = GT_BONG;
	graph->wGKind = GK_JPN;
	graph->wSKind = 1;
	graph->wCalOption = 0;
	graph->dwDraOption = GO_FIILUPBONG|GO_FIILDNBONG;
	graph->rgbLabel = LINCR;

	graph->cInfo.aline[0].rgbLColor = LINCR;
	graph->cInfo.aline[0].btLStyle = PS_SOLID;
	graph->cInfo.aline[0].btLWidth = 1;
	graph->cInfo.aline[1].rgbLColor = BUPCR;
	graph->cInfo.aline[1].btLStyle = PS_SOLID;
	graph->cInfo.aline[1].btLWidth = 1;
	graph->cInfo.aline[2].rgbLColor = BDNCR;
	graph->cInfo.aline[2].btLStyle = PS_SOLID;
	graph->cInfo.aline[2].btLWidth = 1;

//////////////////////////////////////////////////////////////
	graph = (struct _graph *)&pcBuf[graphBase + SZ_GRAPH];
	graph->wKey = 2;
	graph->btRgnKey = 1;
	graph->btDataKey = 1;
	graph->btGType = GT_BONG;
	graph->wGKind = GK_PMA;
	graph->wSKind = 1;
	graph->wCalOption = 0;
	graph->rgbLabel = LINCR;

	graph->cInfo.wCOption = 0x01 | 0x02 | 0x04 | 0x08;
	graph->cInfo.aline[0].rgbLColor = DEFCR8;
	graph->cInfo.aline[0].btLStyle = PS_SOLID;
	graph->cInfo.aline[0].btLWidth = 1;
	graph->cInfo.aline[1].rgbLColor = DEFCR15;
	graph->cInfo.aline[1].btLStyle = PS_SOLID;
	graph->cInfo.aline[1].btLWidth = 1;
	graph->cInfo.aline[2].rgbLColor = DEFCR14;
	graph->cInfo.aline[2].btLStyle = PS_SOLID;
	graph->cInfo.aline[2].btLWidth = 1;
	graph->cInfo.aline[3].rgbLColor = DEFCR4;
	graph->cInfo.aline[3].btLStyle = PS_SOLID;
	graph->cInfo.aline[3].btLWidth = 1;
	graph->cInfo.aline[4].rgbLColor = MC5;
	graph->cInfo.aline[4].btLStyle = PS_SOLID;
	graph->cInfo.aline[4].btLWidth = 1;
	graph->cInfo.aline[5].rgbLColor = MC6;
	graph->cInfo.aline[5].btLStyle = PS_SOLID;
	graph->cInfo.aline[5].btLWidth = 1;
	graph->cInfo.aline[6].rgbLColor = MC7;
	graph->cInfo.aline[6].btLStyle = PS_SOLID;
	graph->cInfo.aline[6].btLWidth = 1;
	graph->cInfo.aline[7].rgbLColor = MC8;
	graph->cInfo.aline[7].btLStyle = PS_SOLID;
	graph->cInfo.aline[7].btLWidth = 1;


	graph->cInfo.awValue[0] = 5;
	graph->cInfo.awValue[1] = 20;
	graph->cInfo.awValue[2] = 60;
	graph->cInfo.awValue[3] = 120;
////////////////////////////////////////////////////////////////
	graph = (struct _graph *)&pcBuf[graphBase + SZ_GRAPH*2];
	graph->wKey = 3;
	graph->btRgnKey = iRCnt;
	graph->btDataKey = 1;
	graph->btGType = GT_BONG;
	graph->wGKind = GK_VOL;
	graph->wSKind = 1;
	graph->wCalOption = GC_VOLVOL;
	graph->rgbLabel = LINCR;

	graph->cInfo.aline[0].rgbLColor = VOLCR;
	graph->cInfo.aline[0].btLStyle = PS_SOLID;
	graph->cInfo.aline[0].btLWidth = 1;
	graph->cInfo.aline[1].rgbLColor = BUPCR;
	graph->cInfo.aline[1].btLStyle = PS_SOLID;
	graph->cInfo.aline[1].btLWidth = 1;
	graph->cInfo.aline[2].rgbLColor = BDNCR;
	graph->cInfo.aline[2].btLStyle = PS_SOLID;
	graph->cInfo.aline[2].btLWidth = 1;

///////////////////////////////////////////////////////////////
	graph = (struct _graph *)&pcBuf[graphBase + SZ_GRAPH*3];
	graph->wKey = 4;
	graph->btRgnKey = iRCnt;
	graph->btDataKey = 1;
	graph->btGType = GT_BONG;
	graph->wGKind = GK_VMA;
	graph->wSKind = 1;
	graph->wCalOption = 0;
	graph->rgbLabel = LINCR;

	graph->cInfo.wCOption = 0x01 | 0x02 | 0x04 | 0x08;
	graph->cInfo.aline[0].rgbLColor = DEFCR8;
	graph->cInfo.aline[0].btLStyle = PS_SOLID;
	graph->cInfo.aline[0].btLWidth = 1;
	graph->cInfo.aline[1].rgbLColor = DEFCR15;
	graph->cInfo.aline[1].btLStyle = PS_SOLID;
	graph->cInfo.aline[1].btLWidth = 1;
	graph->cInfo.aline[2].rgbLColor = DEFCR14;
	graph->cInfo.aline[2].btLStyle = PS_SOLID;
	graph->cInfo.aline[2].btLWidth = 1;
	graph->cInfo.aline[3].rgbLColor = DEFCR4;
	graph->cInfo.aline[3].btLStyle = PS_SOLID;
	graph->cInfo.aline[3].btLWidth = 1;
	graph->cInfo.aline[4].rgbLColor = MC5;
	graph->cInfo.aline[4].btLStyle = PS_SOLID;
	graph->cInfo.aline[4].btLWidth = 1;
	graph->cInfo.aline[5].rgbLColor = MC6;
	graph->cInfo.aline[5].btLStyle = PS_SOLID;
	graph->cInfo.aline[5].btLWidth = 1;
	graph->cInfo.aline[6].rgbLColor = MC7;
	graph->cInfo.aline[6].btLStyle = PS_SOLID;
	graph->cInfo.aline[6].btLWidth = 1;
	graph->cInfo.aline[7].rgbLColor = MC8;
	graph->cInfo.aline[7].btLStyle = PS_SOLID;
	graph->cInfo.aline[7].btLWidth = 1;

	graph->cInfo.awValue[0] = 5;
	graph->cInfo.awValue[1] = 20;
	graph->cInfo.awValue[2] = 60;
	graph->cInfo.awValue[3] = 120;

	if (m_default_rgnCount > DEFAULT_REGION_COUNT) {
		AdditionalGraphInfo((struct _graph*)&pcBuf[graphBase + SZ_GRAPH*4]);
	}

	for (int ii = 1; ii < MAX_INDEX - 1; ii++)
		CopyMemory(&pcBuf[iLen*ii], pcBuf, iLen);

	CopyMemory(&pcBuf[iLen * (MAX_INDEX-1)], pcBuf, graphBase);

	// 틱차트에서 ma제거
	int	iRealIdx = 0;
	for (int jj = 0; jj < gcnt; jj++)
	{
		graph = (struct _graph *)&pcBuf[graphBase + SZ_GRAPH*jj];
		if (graph->wGKind == GK_PMA || graph->wGKind == GK_VMA)
			continue;
		
		CopyMemory(&pcBuf[iLen * (MAX_INDEX-1) + graphBase + iRealIdx*SZ_GRAPH],
			graph, SZ_GRAPH);
		iRealIdx++;
	}
	
	int iDataSize = iLen * (MAX_INDEX - 1) + graphBase + iRealIdx * SZ_GRAPH;


	CSavedHead sh;
	char* pcGrp = sh.Make(VERSION_GRP, 1, iDataSize);
	CopyMemory(pcGrp, pcBuf, iDataSize);
	sh.Write(lpszFileName);

	delete pcBuf;
}

void CAxChartInfo::InitInfo(struct _cntInfo *cntInfo, LPSTR pData)
{
	int	count = sizeof(basicInfo)/sizeof(basicInfo[0]);
	struct _graph	*graph;
	struct _basicInfo	*bInfo;

	for (int ii = 0; ii < count; ii++)
	{
		bInfo = (_basicInfo*)&basicInfo[ii];
		graph = (struct _graph*)&pData[SZ_GRAPH*ii];

		graph->wKey = 1;
		graph->btDataKey = 1;
		graph->btGType = bInfo->btType;
		graph->wGKind = bInfo->wGrpKind;
		graph->wSKind = 1;
		graph->wCalOption = bInfo->wCOption;
		graph->dwDraOption = bInfo->dwDOption;
		graph->rgbLabel = LINCR;
		graph->tInfo.rgbOverBuy = bInfo->tickInfo.rgbOverBuy;
		graph->tInfo.rgbOverSell = bInfo->tickInfo.rgbOverSell;
		CopyMemory(&graph->cInfo, &bInfo->chartInfo, SZ_CHARTINFO);
		CopyMemory(&graph->tInfo, &bInfo->tickInfo, SZ_TICKINFO);
	}
}

void CAxChartInfo::SetChartOption(_graph* graph, int index, int value, COLORREF color, int width, UINT style)
{
	graph->cInfo.awValue[index] = value;
	graph->cInfo.aline[index].rgbLColor = color;
	graph->cInfo.aline[index].btLStyle = style;
	graph->cInfo.aline[index].btLWidth = width;
}

void CAxChartInfo::SetTickOption(_graph* graph, int index, float value, int fillOverStyle, COLORREF color, int width, UINT style)
{
	if (index >= 0 && index < MAX_LINE) {
		graph->tInfo.afValue[index] = value;
		graph->tInfo.abtFillOver[index] = fillOverStyle;
		graph->tInfo.aline[index].rgbLColor = color;
		graph->tInfo.aline[index].btLWidth = width;
		graph->tInfo.aline[index].btLStyle = style;
	}
}

_basicInfo* CAxChartInfo::GetChartBasicInfo(WORD graphKind)
{
	for (int i = 0; i < sizeof(basicInfo) / sizeof(basicInfo[0]); i++) {
		if (basicInfo[i].wGrpKind == graphKind)
			return (_basicInfo*)&basicInfo[i];
	}
	return NULL;
}


