#ifndef CLIENT_TYPEDEF
#define CLIENT_TYPEDEF

#define  GRID_ROW_HEIGHT 18
#define  DEF_FRAMEHEIGHT 523

#define _countof(array) (sizeof(array)/sizeof(array[0]))

#define STR_CONVERT(pBuff, cData, nSize, strData)	\
  pBuff = new char[nSize + 1];							\
  memset(pBuff, 0x00, nSize + 1);						\
  memcpy(pBuff, cData, nSize);							\
  strData = (CString)pBuff;									\
  strData.TrimRight();											\
delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)	\
  pBuff = new char[nSize + 1];						\
  memset(pBuff, 0x00, nSize + 1);					\
  memcpy(pBuff, cData, nSize);						\
  nData = atoi(pBuff);										\
delete [] pBuff;



const		LPCTSTR		DGN_HEADER_NAME[] = { _T("�ð�"), _T("�ڵ�"), _T("�����"), _T("���簡"), _T("���ϴ��"),
									 _T("�����"), _T("ü�ᷮ"), _T("�����ŷ���"), _T("�ŷ�����"), _T("ü�ᷮ��"), _T("ü�ᰭ��"), _T("ī��Ʈ") };

const		UINT		DGN_HEADER_SIZE[] = { 60, 55, 80, 60, 65, 55, 60, 80, 61, 59, 61, 40 };


const LPCTSTR		SORT_RESULT_NAME[] = { "�⺻", "�����ڵ�", "���簡", "���ϴ��", 
                                        "�����", "����", "����ŷ���" };

const LPCTSTR		MATCHVAL_OPT_LIST[] = { ">", ">=", "=", "<=", "<" };
const LPCTSTR		MATCHVAL_OPT_DATA = ">\n>=\n=\n<=\n<\n";
const		LPCSTR		FIELDEDIT_TAB_STR[] = {_T("�ü�"), _T("ȣ��"), _T("�ð�����"), _T("�ְ�/����"), _T("������"), _T("����"), _T("��Ÿ")};
/////Client////////////////
#define		MARKET_ALL	 "0"
#define		MARKET_KOSPI "1"
#define		MARKET_KODAQ "2"
///////////////////////////


#define CONVOL_FILE						"ConVolSetInfo.ini"
#define PROGRAM_TITLE					"����ü�ᷮ"
#define MAX_FIELD_CNT					17		//# ��� Grid Column ���� Max Count

#define CANDLE_INDEX_FILE				"candleinfo.xml"
#define PATEN_INDEX_FILE				"patterninfo.xml"
#define GRID_MOVEFIELD_POS_NAME			"[������ ��ġ]"

#define XML_VERSION_FIXFIELDFILE			"1.0"
#define FOLDER_DATA							"\\icss"
#define FIXFIELD_SYS_FILE					"fixfield_Pattern_sys.xml"
#define FIXFIELD_USER_PATTERN_FILE			"fixfield_User_Pattern.xml"/*"fixfield_User_section.xml"*/
#define FIXFIELD_USER_TRCONVOL				"fixfield_User_TrConVol.xml"
#define FOLDER_IMAGE						"\\icss\\Image" 

#define FINDERINFO_INIFILE			"FindInfo.ini"
#define FOLDER_FINDDATA				"\\FindData"

#define gszCacheDir "DrCache"
#define gszDataKey_MstCode "MstCode"
#define gszDataKey_WorkDate "WorkDate"
#define usrMasterCondition "SearchTemp"
#define USRDIR "\\user\\"
#define FILE_GROUP "\\portfolio.ini"

#define GETPATH_APP(strAppPath)       \
	char buf[255];														\
	GetModuleFileName(NULL, buf, 255);									\
	int lastlocationofbackslash = 0;									\
	int nBufCnt = strlen(buf);											\
	for (int i = 0; i < nBufCnt; i++)									\
	{																	\
	if (buf[i] == '\\')													\
	lastlocationofbackslash = i;										\
	}                                                                   \
	CString strAppPath = _T("");                                        \
	for (int j = 0; j < lastlocationofbackslash; j++)                   \
	strAppPath += buf[j];  

#define Safe_CheckCopy(xSource, xTarget) if(xTarget.GetLength()) memcpy(xSource, (LPCSTR)xTarget, xTarget.GetLength())

#define FIELD_COUNT	11

typedef struct tagFieldSet	//�ʵ� ���� Dlg
{
	char	 chkTime[1];
	char	 chkCodeItem[1];
	char	 chkNameItem[1];
	char	 chkPriceNow[1];
	char	 chkChange[1];
	char	 chkChgRate[1];
	char	 chkConVol[1];
	char	 chkAccumDealVol[1];
	char	 chkDealVolRate[1];
	char	 chkConVolRate[1];
	char	 chkConVolPower[1];
	char	 chkCount[1];
	char	 chColNum[3];
	char	 chNull[1];
} StFieldSet;

#define STCOPY_P_StFieldSet(pSrc, pTar) memset(pTar, 0x00, sizeof(StFieldSet)); memcpy(pTar, pSrc, sizeof(StFieldSet)-1);
#define STCOPY_StFieldSet(xSrc, xTar) {		\
		StFieldSet *pxSrc=&xSrc, *pxTar=&xTar; STCOPY_P_StFieldSet(pxSrc, pxTar);	\
	}

typedef struct tagConVolSet	//���� ü�ᷮ ���� Dlg
{
	CString		strVer;
	CString		strScreenNo;
	
	int		nChkConVol;
	long	lConVol;

	int		nChkDealPrice;
	long	lDealPrice;

	int		nConditionCheck;	// 0:�ϳ��̻� ����, 1:�Ѵ� ����(ü�ᷮ, �ŷ����)��, 3:�ŷ���ݸ� ����, 4:ü�ᷮ

	int		nChkAccumDealVol;
	long	lAccumDealVol;

	int		nChkSetPrice;
	long	lSetPriceUp;
	long	lSetPriceDown;

	int		nChkChgRate;
	double	dChgRateUp;
	double	dChgRateDown;

	int		nChkSound;
	CString	strPathSound;

	int		nItemPosition;   //�׸��� Display�� 0: ���� �߰�, 1:�Ʒ��� �߰�

	int		nChkTargetUpjong;		// �������
	CString	strMarketCode;			// �����ڵ�
	
	int		nChkManagement;			// ��������
	int		nChkTidyDeal;			// �����Ÿ�
	int		nChkInvestAttention;	// ��������
	int		nChkPrefferShare;		// �켱��
	int		nChkInvestRisk;			// ��������
	
	int		nChkDealStop;			// �ŷ�����
	int		nChkInsincerityNotice;	// �Ҽ��ǰ���
	int		nChkPreInvestRisk;		// �������迹��
	int		nChkDeposit;			// ���ű� 100% ����
	int		nChkInvestWarning;		// ���ڰ��
} StConVolSet;

#define STCOPY_P_StConVolSet(pSrc, pTar) {								\
			pTar->strVer = pSrc->strVer;								\
			pTar->strScreenNo		= pSrc->strScreenNo;				\
			pTar->nChkConVol		= pSrc->nChkConVol;					\
			pTar->lConVol			= pSrc->lConVol;					\
			pTar->nChkDealPrice 	= pSrc->nChkDealPrice;				\
			pTar->lDealPrice		= pSrc->lDealPrice;					\
			pTar->nConditionCheck	= pSrc->nConditionCheck;			\
			pTar->nChkAccumDealVol	= pSrc->nChkAccumDealVol;			\
			pTar->lAccumDealVol 	= pSrc->lAccumDealVol;				\
			pTar->nChkSetPrice		= pSrc->nChkSetPrice;				\
			pTar->lSetPriceUp		= pSrc->lSetPriceUp;				\
			pTar->lSetPriceDown 	= pSrc->lSetPriceDown;				\
			pTar->nChkChgRate		= pSrc->nChkChgRate;				\
			pTar->dChgRateUp		= pSrc->dChgRateUp;					\
			pTar->dChgRateDown		= pSrc->dChgRateDown;				\
			pTar->nChkSound			= pSrc->nChkSound;					\
			pTar->strPathSound		= pSrc->strPathSound;				\
			pTar->nItemPosition 	= pSrc->nItemPosition;				\
																		\
			pTar->nChkTargetUpjong	= pSrc->nChkTargetUpjong;			\
			pTar->strMarketCode		= pSrc->strMarketCode;				\
																		\
			pTar->nChkManagement		= pSrc->nChkManagement;			\
			pTar->nChkTidyDeal			= pSrc->nChkTidyDeal;			\
			pTar->nChkInvestAttention	= pSrc->nChkInvestAttention;	\
			pTar->nChkPrefferShare		= pSrc->nChkPrefferShare;		\
			pTar->nChkInvestRisk		= pSrc->nChkInvestRisk;			\
			pTar->nChkDealStop			= pSrc->nChkDealStop;			\
			pTar->nChkInsincerityNotice	= pSrc->nChkInsincerityNotice;	\
			pTar->nChkPreInvestRisk		= pSrc->nChkPreInvestRisk;		\
			pTar->nChkDeposit			= pSrc->nChkDeposit;			\
			pTar->nChkInvestWarning		= pSrc->nChkInvestWarning;		\
		}
#define STCOPY_StConVolSet(xSrc, xTar) {StConVolSet *pxSrc=&xSrc, *pxTar=&xTar; STCOPY_P_StConVolSet(pxSrc, pxTar); }


typedef struct tagSearchTarget  //�˻����� Dlg
{
	CString strGroupName;		
	CString strSubGroupName;

	int nChkManagement;			//��������
	int nChkTradeStop;			//�ŷ�����
	int nChkPrefferShare;		//�켱��
	int nChkDepositMoney;		//���ű�100%
	int nChkLogicSell;			//�����Ÿ�
	int nChkInsincerityNotice;	//�Ҽ��ǰ���
	int nChkInvestRisk;			//��������
	int nChkInvestWarn;			//���ڰ��
	int nChkInvestAttention;	//��������
	int nChkELW;				
	int nChkInvestRiskNotice;	//�������迹��

	int nSizeTargetcode;
	CStringArray saTargetCode; //�˻��� ���� �׷��� �ڵ�

	int nSizeExceptCode;
	CStringArray saExceptCode; //������ ������ �ڵ�
}ST_SEARCHTARGET;

#define STCOPY_P_ST_SEARCHTARGET(pSrc, pTar) {						\
			pTar->strGroupName		= pSrc->strGroupName;			\
			pTar->strSubGroupName	= pSrc->strSubGroupName;		\
			pTar->nChkManagement	= pSrc->nChkManagement;			\
			pTar->nChkTradeStop		= pSrc->nChkTradeStop;			\
			pTar->nChkPrefferShare	= pSrc->nChkPrefferShare;		\
			pTar->nChkDepositMoney	= pSrc->nChkDepositMoney;		\
			pTar->nChkLogicSell		= pSrc->nChkLogicSell;			\
			pTar->nChkInsincerityNotice = pSrc->nChkInsincerityNotice;	\
			pTar->nChkInvestRisk	= pSrc->nChkInvestRisk;				\
			pTar->nChkInvestWarn	= pSrc->nChkInvestWarn;				\
			pTar->nChkInvestAttention = pSrc->nChkInvestAttention;		\
			pTar->nChkELW			= pSrc->nChkELW;					\
			pTar->nChkInvestRiskNotice = pSrc->nChkInvestRiskNotice;	\
			pTar->nSizeTargetcode	= pSrc->saTargetCode.GetSize();		\
			pTar->nSizeExceptCode	= pSrc->saExceptCode.GetSize();		\
			int nCnt = 0;												\
			pTar->saTargetCode.RemoveAll();								\
			int i = 0;												\
			for(i=0; i<pSrc->saTargetCode.GetSize(); i++ )			\
				pTar->saTargetCode.Add(pSrc->saTargetCode.GetAt(i));	\
			pTar->saExceptCode.RemoveAll();								\
			for(i=0; i<pSrc->saExceptCode.GetSize(); i++ )				\
				pTar->saExceptCode.Add(pSrc->saExceptCode.GetAt(i));	\
		}

#define STCOPY_ST_SEARCHTARGET(xSrc, xTar) {			\
			ST_SEARCHTARGET *pxSrc=&xSrc, *pxTar=&xTar;	\
			STCOPY_P_ST_SEARCHTARGET(pxSrc, pxTar);		\
		}

//#define INIT_ST_SEARCHTARGET(xST) xST.nChkManagement=0;

typedef struct tagInstConVolume
{
	char	cSign[1];
	char	chGridOpt[1];     
	char	sTime[8];
	char	sCode[6];
	char	sItemName[20];
	char	sPriceNow[8];
	char	sComparison[8];
	char	sChgRate[6];
	char	sConVol[9];
	char	sAccumDealVol[9];
	char	sDealVolRate[6];
	char	sConVolRate[6];
	char	chGubun[1];
	char    sVolumePower[9];                // ü�ᰭ�� (20090408 �߰�)
	char	sEntryCnt[4];
}StConVolClient; //���� ü�ᷮ ȭ�� �ʵ�(Display��)

#define STCOPY_P_StConVolClient(pSrc, pTar) memset(pTar, 0x00, sizeof(StConVolClient)); memcpy(pTar, pSrc, sizeof(StConVolClient));
#define STCOPY_StConVolClient(xSrc, xTar) {StConVolClient *pxSrc=&xSrc, *pxTar=&xTar; STCOPY_P_StConVolClient(pxSrc, pxTar) }

typedef struct  
{
	COLORREF clrF;
	COLORREF clrB;	
	COLORREF clrSF;
	COLORREF clrSB;
	COLORREF clrBACK;
} ST_TABCOLOR;

typedef struct  
{
	COLORREF clrGridHeader;
	COLORREF clrGridHText;  
	COLORREF clrGridHilight;
	COLORREF clrGridSBk;
	COLORREF clrGridSText;  
	COLORREF clrGridUpText;  
	COLORREF clrGridDownText;
	COLORREF clrGridVolText; 
} ST_GRIDCOLOR;

typedef struct tagHeadInfoType
{
	long        lHeadID;
	CString     strHeadName;
	int         nUnitNum;
	CString     strUnitText;
	CString     strDataType;
	int         nDataLength;
	int         nColWidth;
	CString     strAlignment;
} HeadInfoType;

typedef struct tagHeadFixInfo
{
	int           nColNum;        
	HeadInfoType  stHeadType;
	BOOL          bView;
} HeadFixInfo;

typedef struct tagMatchInfo
{
	int			nRow;
	BOOL		bUser;
	int			nOptIndex;
	CString		strField;
	CString		strElement;
	int 		nFieldIndex;

	int			nGridSourceRow;			//# Temp
} MatchInfo;

typedef struct tagGridEventInfo
{
	CString strGridType;

	int nCol;
	int nRow;
	CString strType; //#=> BUTTON / CHECK / COMBO
	CString strData;
} GridEventInfo;

typedef struct tagCandleInfo{
	long    lID;
	CString   strName;
} CandleInfo;

typedef struct tagPatenInfo{
	long    lID;
	CString   strName;
} PatenInfo;

typedef struct 
{
	CString szCtrlName;
	CWnd* pWnd;
} ST_MOVEITEM;
#define SET_ST_MOVEITEM(xST, xName, xpWnd) xST.szCtrlName=xName; xST.pWnd=xpWnd;

struct	Hana_UpMaster	{
	char	jgub;			// �屸��
	// 0:�峻 1:���
	// 1: KOSPI200
	// 3: KOSDAQ50(1), KQ-IT 50(2)
	// 4: K100P(1), K50(2), IT(3
	// 6: SECTOR, 7:�������� 
	char	ucod;			// �����ڵ�
	char	hnam[20];	// �ѱ۸�
};

#define FILE_DATAVERSION		"SvrVersionDataFile.cfg"
#define FILE_DATAVERSION_LOCAL	"LocalVersionDataFile.cfg"
#define FILE_FINDDATA			"finddata"
#define FILE_LANGGUAGEDATA		"data"
#define FILE_EXP				"exp"
#define FILE_MAP				"map"
#define FILE_OTHER				"other"
#define FILE_PATTERNDATA		"patterndata"
#define FILE_SYS				"sys"
#define FILE_EXTENSION			".zip"

#define FOLDER_FINDDATA				"\\FindData"

#define GETMARKET_ALL			1000
#define GETMARKET_KOSPI			0
#define GETMARKET_KOSPI200		100
#define GETMARKET_KOSPI100_50	400
#define	GETMARKET_KOSDAQ		200
#define	GETMARKET_KOSTAR		300
#define GETMARKET_KRX_ALL		900
#define GETMARKET_KRX			600

// KRX ������ ���а�
#define SEC1_AT       0x01  /* �ڵ���                   */
#define SEC1_SM       0x02  /* �ݵ�ü                   */
#define SEC1_HC       0x04  /* �ǰ�                     */
#define SEC1_BK       0x08  /* ����                     */
#define SEC2_IT       0x01  /* �������                 */
#define SEC2_EC       0x02  /* ȭ�п�����               */
#define SEC2_ST       0x04  /* ö��                     */
#define SEC2_CS       0x08  /* �ʼ��Һ���               */
#define SEC3_MT       0x01  /* �̵�����               */
#define SEC3_CO       0x02  /* �Ǽ�                     */
#define SEC3_FB       0x04  /* ��������               */
#define SEC3_SI       0x08  /* ����                     */
#define SEC4_SC       0x01  /* ����                     */
#define SEC4_SR       0x02  /* ��ȸ����å������         */
#define SEC4_IS       0x04  /* ����                     */
#define SEC4_TP       0x08  /* ���                     */
#define SEC5_RT       0x01  /* �Һ�������               */
#define SEC5_LS       0x02  /* ����                     */
#define SEC5_KX       0x04  /* KRX100                   */

typedef struct  
{
	int xPoint;
	int yPoint;
	CString strCode;
} RButtonInfo;

#endif // CLIENT_TYPEDEF
