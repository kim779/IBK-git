#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

/************************************************************/
/*																													*/
/*	One Clk Condition Server =>  STRUCT/DEFINE/TYPEDEF ����	*/
/*																													*/
/*					2005. 1.  			Doori														*/
/*					�ۼ��� : �쵿��																	*/
/************************************************************/
#define QUERY_STRJONGMOK		    	"p0602"     //��Ʈ ����
//#define TRNO_MSTCODE				"p0827"	//"p0627"		//���񸮽�Ʈ ��ȸ
#define TRNO_THEMEGROUP				"p0828"	//"p0628"		//�׸� �׷� ��ȸ
#define TRNO_THEMEITEM				"p0830"	//"p0630"		//�׸� ���� ��ȸ
#define		S_RQ_JAN_CODEINFO		"a7001"		// �ֽ�	    ���� �ܰ� ������ ��û TR

/*
typedef struct 
{
	char	accno			[11];		// ���¹�ȣ 
} JAN_RQ_INPUT;
*/
/*
#define CONDITION_MAXCNT		50
#define TRNO_MSTCODE        "50008"
#define TRNO_CONDIONECLK_LIST			"50005"
#define TRNO_CONDIONECLK_RESULT		"50004"
#define TRNO_TEMACLK_LIST					"50006"
#define TRNO_TEMACLK_RESULT				"50007"
#define TRNO_CONDITION_RESULT			"50009"
#define TRNO_USER_REALREG				  "50002"
*/

// typedef struct tagMSTCode
// {
//   char    sMarket[1];
//   char    sCode[7];
//   char    sName[20];
//   char    sSetPos[4];
// } MSTCode;

typedef struct tagMSTCode
{
	char    sMarket[1];
	char    sCode[7];
	char    sName[20];
	char    sSetPos[4];
	char    sUsjg[1];               /* �켱��   (0:�ش����,1:�ش�) */
	char    sUnder[1];              /* ��������                     */
	char    sStop[1];               /* �ŷ���������                 */
	char    sDishonest[1];          /* �Ҽ��ǰ�������               */
	char    sConfidencelimit[1];    /* �ſ���������?               */
	char    sJrmm[1];          	    /* �����Ÿ�����                 */
	char    sDrat[3];               /* ���űݺ���                   */
	char    sMrktalrm[1];           /* ����溸
										1:��������, 2:���ڰ��
										3:��������, 4:�������迹��*/
	char	sMonth[2];				/* ���� */
	char	sETF[1];				/* ETF							*/
} MSTCode;


#define COND_MST_VER	1


typedef struct tagTrConVol
{
	char		sDataSection[4];		// �����ͱ���
	char		sPacketCode[4];			// ��Ŷ�����ڵ�
	char		sAlertBody[1];			// System, User
	char		sSignaldate[8];			// ��ȣ �߻�����
	char		sSignaltime[6];			// ��ȣ �߻��ð�
	char		sNowItemCnt[4];			// ���縸�� �����
	char		sSendItemCnt[4];		// ���������
	char		sSignalBirth[1];		// ��ȣ�߻�
	char		sJobflag[1];			// �ű�,������,Ż��
	char		sItemCode[6];			// �����ڵ�
	char		sItemName[20];			// �����
	char		sEntryCnt[4];			// ���� Ƚ��
	char		sCategorybranch_l[4];	// ������з�
	char		sCategorybranch_m[4];	// �����ߺз�
	char		sCategorybranch_s[4];	// �����Һз�
	char		sPreferShare[1];		// �켱��
	char		sManagement[1];			// ��������
	char		sControl[1];			// �̻�޵� ����
	char		sTradestop[1];			// �ŷ����� ����
	char		sInsinceritynotice[1];	// �Ҽ��ǰ��� ����
	char		sConfidencelimit[1];	// �ſ����� ����
	char		sCodenotice[1];			// �������
	char		sJungrimeme[1];			// �����Ÿ� ����
	char		sMarket[1];				// ���屸��
	char		sDepositmoney[3];		// ���ű� ����
	char		sInvestrisk[1];			// �������� ����
	char		sInvestwarn[1];			// ���ڰ�� ����
	char		sInvestattention[1];	// �������� ����
	char		sInvestrisknotice[1];	// �������迹�� ����
	char		sPrice[8];				// ���簡
	char		sSign[1];				// ���ϴ������ȣ
	char		sChange[8];				// ���ϴ��
	char		sChgrate[6];			// ���ϴ������
	char		sVolume[9];				// �ŷ���
	char		sVolumerate[6];			// �ŷ��� ���ϴ����
	char		sAmount[9];				// �ŷ����](��������ü����)   ����:õ��
	char		sCvolume[9];			// ����ü�ᷮ
	char		sCvolumerate[6];		// �����ŷ������ü�ᷮ
	char		sCamount[9];			// ����ü����]  ����:��
	char		sCGubun[1];				// ü�ᱸ�� ('+':�ż� '-':�ŵ�)
	char		sVolumePower[9];        // ü�ᰭ�� (20090408 �߰�)
	char		sMflag[1];				// ���� ���� '1' = �ŷ��� '2' = �ڽ���
} StTrConVol;   //Eugene ���� ü�ᷮ Packet




/////////////////////



// class C0627OutBlock1
// {
// public:
// 	int  GetDataCount(CHARVECTOR2 &vvRpValueList)
// 	{
// 		CString szTmp;
// 		szTmp	 = vvRpValueList[1][0];	// �˻��� ���� ���� ��
// 		if(szTmp.GetLength()==0) return 0;
// 
// 		int nRet = atoi(szTmp);
// 		return (nRet<0)? 0: nRet;
// 	}
// };
// 
// class C0627OutBlock2
// {
// public:
// 	void SetData(CHARVECTOR2 &vvRpValueList, char* pDataBuf, int nDataCnt)
// 	{
// 		CString sMarket,		//���ϱ���
// 				sCode,		//�����ڵ�
// 				sName,		//�����
// 				sSetPos;	//�ε���
// 		int nPos = 0;
// 
// 		for(int i=0; i<nDataCnt; i++)
// 		{
// 			MSTCode* pOutData = (MSTCode*)&pDataBuf[nPos];
// 			nPos += sizeof(MSTCode);
// 
// 			ZeroMemory(pOutData, sizeof(MSTCode));
// 
// 			sMarket	 = vvRpValueList[i+1][0];
// 			pOutData->sMarket[0] = sMarket[0];
// 
// 			sCode = vvRpValueList[i+1][1];
// 			Safe_CheckCopy(pOutData->sCode ,sCode);
// 			pOutData->sCode[6] = 0;
// 
// 			sName = vvRpValueList[i+1][2];
// 			sName.TrimRight();
// 			//Safe_CheckCopy(pOutData->sName ,sName);
// 			memcpy(pOutData->sName, (LPCSTR)sName, 20);
// 			pOutData->sName[sName.GetLength()]=0;
// 
// 			sSetPos = vvRpValueList[i+1][3];
// 			Safe_CheckCopy(pOutData->sSetPos ,sSetPos);
// 			//memcpy(pOutData->sSetPos, (LPCSTR)sSetPos, 4);
// 		}
// 	};
// };

///////////�׸��׷�
///////////////////
typedef struct
{
	char		giCallIndex[4];
	char		gsCode[6];
	char		gsFilterGubun[1];
}ST_THEME_IN;					//p0628InBlock;
#define SZ_THEME_IN sizeof(ST_THEME_IN)

typedef struct
{
	char		sTrIndex[4];
	char		sCnt[4];
}ST_THEME_OUT1;						//p0628OutBlock1;
#define SZ_THEME_OUT1 sizeof(ST_THEME_OUT1)

typedef struct
{
	char		gsThemeCode[4];
	char		gsThemeName[36];
}ST_THEME_OUT2;						//p0628OutBlock2;
#define SZ_THEME_OUT2 sizeof(ST_THEME_OUT2)


///�׸� ����
typedef struct
{
	char		sTrIndex[4];
	char		sCode[4];
}ST_THEMEITEM_IN;					//p0630InBlock;
#define SZ_THEMEITEM_IN sizeof(ST_THEMEITEM_IN)

typedef struct
{
	char		sTrIndex[4];
	char		sCnt[4];
}ST_THEMEITEM_OUT1;					//p0630OutBlock1;
#define SZ_THEMEITEM_OUT1 sizeof(ST_THEMEITEM_OUT1)

typedef struct
{
	char		sItemCode[6];
	char		sMarketFlag[1];
}ST_THEMEITEM_OUT2;					//p0630OutBlock2;
#define SZ_THEMEITEM_OUT2 sizeof(ST_THEMEITEM_OUT2)
#endif // SERVER_TYPEDEF

