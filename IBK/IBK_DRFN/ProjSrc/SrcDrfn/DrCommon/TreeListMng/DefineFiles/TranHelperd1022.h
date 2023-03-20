/*************************************************************************************/
/*  1. ���α׷�ID : KB_d1022                                                         */
/*  2. ���α׷��� : ���ǰ˻�_��Ŭ��������ȸ                                          */
/*  3. ���񽺸�   : KB_d1022                                                         */
/*  4. ������̺� :                                                                  */
/*  5. �ۼ���     : drfn                                                             */
/*  6. �Է��׸�   :                                                                  */
/*  7. ����׸�   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TREELIST_TRAN_KB_d1022_H__
#define __EU_TREELIST_TRAN_KB_d1022_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
//#include "../../../TRDll/Dr_Include/Condition_MngDef.h"
#include "./Condition_MngDef.h"
#include "./client_typedef.h"
#include "./server_typedef.h"

#define INT2STR_BUF(xStr, nSize, nStr) sprintf(xStr, "%#0*.*d", nSize, nSize, nStr)
#define Safe_CheckCpy(xSource, xTarget) if(::lstrlen(xTarget) > 0) memcpy(xSource, (LPCSTR)xTarget, ::lstrlen(xTarget))
#define Safe_CheckCopy2(xSource, xTarget, xLen, xStr) \
	xStr.Format("%-*.*s", xLen, xLen, xTarget); \
	xStr.TrimRight();	\
	if(xStr.GetLength()) memcpy(xSource, (LPCSTR)xStr, xStr.GetLength())


/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define NAME_d1022     "d1022"
#define LLONG          long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
    char    giCallGubun[4];                       /* giCallGubun(4)                  */
	char	giType[1];
    char    gsFilterGubun[1];                     /* ���ͱ���(1)                     */
} KB_d1022_InRec1;

/* InRec2:                                                                           */
typedef struct
{
	char    giCallGubun[4];                       /* giCallGubun(4)                  */
	char    giTargetGubun[2];                     /* Ÿ�ٱ���(2)                     */
	char    giTargetIdxCnt[2];                    /* �����ǥ����(2)                 */
	char    giTargetResultViewCnt[2];             /* �������ʵ尹��(2)               */
	char    gsTargetExp[80];                      /* �����������ǥ����(80)          */
	char    giComplexIdxCnt[2];                   /* ������ǥ����(2)                 */
	char    giComplexResultViewCnt[2];            /* ���հ����ⰹ��(2)               */
	char    gsComplexExp[80];                     /* ������������ǥ����(80)          */
	char    giFieldEditerCnt[2];                  /* �����ʵ���������(2)             */
	char    giFieldList[240];                      /* �ʵ�������ȣ����Ʈ(240)          */
	char    giResultIdxCnt[4];                    /* ������񱳰���(4)               */
	char    gsResultExp[80];                      /* �����������(80)              */
	char    gsTargetBitChk[1];                    /* TargetBit(1)                    */
	char    gsJongmokIndex[4000];           		/* �����ڵ�Bitset(4000)            */	//3000 -> 4000
	char    gsSort_flag[1];                       /* ���Ĺ���(1)                     */
	char    giSort_Pos[4];                        /* ���Ĺ�ȣ(4)                     */
	char    giBackDate[4];                        /* backtest����(4)                 */
	char    giBackTime[4];                        /* backtest�ð�(4)                 */
}KB_d1022_InRec2;

/* InRec3:                                                                           */
typedef struct
{
    char    giIndexNo[2];                         /* ��ǥ��ȣ(2)                     */
    char    giGroupNo[2];                         /* �׷��ȣ(2)                     */
    char    giGroupIdxCnt[2];                     /* �׷쳻����ǥ����(2)             */
    char    giTargetBase[2];                      /* Ÿ�ٵ���Ÿ(2)                   */
    char    giType[4];                            /* ��ǥ������(4)                   */
    char    giType1[2];                           /* Type1��������(2)                */
    char    giType2[2];                           /* ������(2)                     */
    char    giSTerm[4];                           /* �׷�˻�������ġ(4)             */
    char    giETerm[4];                           /* �׷�˻�����ġ(4)               */
    char    giRank[4];                            /* �����˻������(4)               */
    char    giTrend[4];                           /* �߼�Ƚ��(4)                     */
    char    giSignalCnt1[4];                      /* ��ȣ�߻�Ƚ��1(4)                */
    char    giSignalCnt2[4];                      /* ��ȣ�߻�Ƚ��2(4)                */
    char    giResultViewNum[2];                   /* ����������ʵ尹��(2)           */
    char    gsIndexType[1];                       /* �˻�����0:�Ϲ�1:����2:����(1)   */
    char    gsSearchType[1];                      /* �˻�����(1)                     */
    char    gsOperator[1];                        /* ������0:OR1:AND2:NEXT(1)        */
    char    gsReverse[1];                         /* �������0:A1:NotA(1)            */
    char    gsIdxContiGubun[1];                   /* 0:���Ϲ߻�1:���ӹ߻���ǥ(1)     */
    char    gsElement[1];                         /* ���ǽĻ�빮��(A)(1)            */
    char    gsScode[6];                           /* shortcode(6)                    */
//	char    gdParam[48];                          /* ��ǥ���ú�����(48)              */
//	char    gdCondition[96];                      /* ��Ÿ����������(96)              */
    char    gdParam[96];                          /* ��ǥ���ú�����(48)              */
    char    gdCondition[192];                      /* ��Ÿ����������(96)              */
}KB_d1022_InRec3;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
typedef struct
{
    int     giCallIndex;                       /* Ŭ���̾�Ʈ��ȸ����(1)           */
    char    gsJongCnt[4];                         /* �˻������(4)                   */
    char    gsHigh[4];                            /* ���������(4)                   */
    char    gsUp[4];                              /* ��������(4)                   */
    char    gsNoch[4];                            /* ���������(4)                   */
    char    gsDown[4];                            /* �϶������(4)                   */
    char    gsLow[4];                             /* ���������(4)                   */
    char    gsUpRate[6];                          /* ��º���(6)                     */
    char    gsChgrateAvg[6];                      /* ��յ����(6)                   */
}KB_d1022_OutRec1;

/* OutRec2:                                                                          */
typedef struct
{
    char    gsMflag[1];                           /* ���屸��(1)                     */
    char    gsCode[6];                            /* �����ڵ�(6)                     */
    char    gsHname[20];                          /* �����(20)                      */
    char    gsSignCnt[3];                         /* ������Ӻ���(3)                 */
    char    gsClose[8];                           /* ���簡(8)                       */
    char    gsSign[1];                            /* �����ȣ(1)                     */
    char    gsChange[8];                          /* ���ϴ��(8)                     */
    char    gsChgRate[6];                         /* ���ϴ������(6)               */
    char    gsVolume[9];                          /* �ŷ���(9)                       */
    char    gsVolumeRate[6];                      /* �ŷ������ϴ����(6)             */
	char    gsFiller[4];    //   4 Filler
	char    gdResultField[960];       /*  ����ʵ� (16 * 60 = 960)    */
}KB_d1022_OutRec2;

/*************************************************************************************/

class CTranHelperd1022
{
public:
	// --------------------------------------------------------------------------------------
	// Inputó��
	// --------------------------------------------------------------------------------------
//	void TR_RESULT_IN_To_KB_d1022_InRec1(TR_RESULT_IN* pIn, KB_d1022_InRec1* pOut)
//	{
//		memset(pOut, 0x20, sizeof(KB_d1022_InRec1));
//
//		char tmpVal[100]={0,};
//
//		INT2STR_BUF(tmpVal, 4, pIn->nTrIndex);
//		memcpy(pOut->giCallGubun, tmpVal, 4);
//
//		pOut->gsFilterGubun[0] = pIn->sType + '0';
//	}
	void TR_RESULT_IN_To_KB_d1022_InRec1(TR_RESULT_IN* pIn, KB_d1022_InRec1* pOut)
	{
		memset(pOut, 0x20, sizeof(KB_d1022_InRec1));

		char tmpVal[100]={0,};
		CString strOp="";
		CString strVal="";

		INT2STR_BUF(tmpVal, 4, pIn->nTrIndex);
		if(tmpVal[0] == '-') 
		{
			
			strVal.Format("%s", tmpVal);
			strOp.Format("%s", strVal.Left(1));
			if(strVal.Mid(1, 1) == '0')
			{
				if(strVal.Mid(2, 1) == '0')
				{
					if(strVal.Mid(3, 1) == '0')
					{
						strVal.Format("%s", strVal.Right(1) + "  ");
						strVal.Format("%s", strOp+strVal);
					}
					else
					{
						strVal.Format("%s", strVal.Right(2) + " ");
						strVal.Format("%s", strOp+strVal);
					}
				}
				else
				{
					strVal.Format("%s", strVal.Right(3));
					strVal.Format("%s", strOp+strVal);
				}				
			}
			memcpy(pOut->giCallGubun, strVal, strVal.GetLength());
		}
		else
			memcpy(pOut->giCallGubun, tmpVal, 4);

		pOut->giType[0] = pIn->sType ;

		pOut->gsFilterGubun[0] = '1';
	}

//	void User_Finder_To_KB_d1022_InRec2(User_Finder* pUserFinder, char* pOut, int &nPos)
//	{
//		KB_d1022_InRec2* pInRec2 = (KB_d1022_InRec2*)&pOut[nPos];
//		memset(pInRec2, 0x20, sizeof(KB_d1022_InRec2));
//
//		CString szVal;
//		char tmpVal[100]={0,};
//		int  nTargetIdxCnt=0, nComplexIdxCnt=0;
//
//		INT2STR_BUF(tmpVal, 4, pUserFinder->TrIndex);				// giCallGubun(4)
//		memcpy(pInRec2->giCallGubun, tmpVal, 4);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->nTargetType);			// 0 :���� , 10 : ����	���� [NEW]
//		memcpy(pInRec2->giTargetGubun, tmpVal, 2);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetIdxCnt);          // ��� ��ǥ ����       
//		memcpy(pInRec2->giTargetIdxCnt, tmpVal, 2);
//		tmpVal[2] = 0;
//		nTargetIdxCnt = atoi(tmpVal);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetResultViewCnt);	// ��� �������ʵ� ���� 
//		memcpy(pInRec2->giTargetResultViewCnt, tmpVal, 2);
//
//		//INT2STR_BUF(tmpVal, 80, pUserFinder->TargetExp[EXPLEN]);	// ������� ���� ǥ�� ��	
//		//memcpy(pInRec2->gsTargetExp, pUserFinder->TargetExp, 80);
//		Safe_CheckCopy2(pInRec2->gsTargetExp, pUserFinder->TargetExp, 80, szVal);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexIdxCnt);			// ���� ��ǥ ����       
//		memcpy(pInRec2->giComplexIdxCnt, tmpVal, 2);
//		tmpVal[2] = 0;
//		nComplexIdxCnt = atoi(tmpVal);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexResultViewCnt);	// ���� �������ʵ� ���� 
//		memcpy(pInRec2->giComplexResultViewCnt, tmpVal, 2);
//
//		//INT2STR_BUF(tmpVal, 80, pUserFinder->ComplexExp[EXPLEN]);	// �������� ���� ǥ�� ��	
//		//memcpy(pInRec2->gsComplexExp, pUserFinder->ComplexExp, 80);
//		Safe_CheckCopy2(pInRec2->gsComplexExp, pUserFinder->ComplexExp, 80, szVal);
//
//		INT2STR_BUF(tmpVal, 4, pUserFinder->ResultIdxCnt);		// �ʵ����� ����   [Fix + Move] 
//		memcpy(pInRec2->giResultIdxCnt, tmpVal, 4);
//
//		//INT2STR_BUF(tmpVal, 80, pUserFinder->FieldList[REAL_IDXCNT]);// �ʵ����� ��ȣ����Ʈ [Just Fix + Move] 
//		//memcpy(pInRec2->gsResultExp, pUserFinder->FieldList, 80);
//		Safe_CheckCopy2(pInRec2->gsResultExp, pUserFinder->ResultExp, 80, szVal);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->FieldEditerCnt);			// ������� ����          [NEW]
//		memcpy(pInRec2->giFieldEditerCnt, tmpVal, 2);
//
//		INT2STR_BUF(tmpVal, 80, pUserFinder->FieldList);	// ����� ������ ǥ�� ��  [NEW]		
//		//memcpy(pInRec2->giFieldList, pUserFinder->ResultExp, 80);
//		Safe_CheckCopy2(pInRec2->giFieldList, pUserFinder->ResultExp, 80, szVal);
//
//		//INT2STR_BUF(tmpVal, 1, pUserFinder->TargetBitChk);				// TargetBit ��� ���� üũ
//		pInRec2->gsTargetBitChk[0] = pUserFinder->TargetBitChk;
//
//		//INT2STR_BUF(tmpVal, 2500, &pUserFinder->TargetBit[0]);	// �˻� ��� ����			
//		//memcpy(pInRec2->gsJongmokIndex, pUserFinder->TargetBit, RESULT_MAX);
//		Safe_CheckCopy2(pInRec2->gsJongmokIndex, pUserFinder->TargetBit, 2500, szVal);
//
//		INT2STR_BUF(tmpVal, 1, pUserFinder->Sort_flag);					// ���� ����(0:����, 1:����)
//		pInRec2->gsSort_flag[0] = pUserFinder->Sort_flag;
//
//		INT2STR_BUF(tmpVal, 4, pUserFinder->Sort_Num);					// ���� ��ȣ(�������ʵ�pos) 	    
//		memcpy(pInRec2->giSort_Pos, tmpVal, 4);
//
//		if(pUserFinder->BackDay==-1)	sprintf(tmpVal,"-001");
//		else		INT2STR_BUF(tmpVal, 4, pUserFinder->BackDay);					// backtest ����(�̻��:-1) 	
//		memcpy(pInRec2->giBackDate, tmpVal, 4);
//
//		if(pUserFinder->BackTime==-1)	sprintf(tmpVal,"-001");
//		else	INT2STR_BUF(tmpVal, 4, pUserFinder->BackTime);					// backtest �ð�(�̻��:-1) 
//		memcpy(pInRec2->giBackTime, tmpVal, 4);
//
//		nPos += sizeof(KB_d1022_InRec2);
//
//		KB_d1022_InRec3* pInRec3;
//		for(int i=0; i<nTargetIdxCnt+nComplexIdxCnt; i++)
//		{
//			pInRec3 = (KB_d1022_InRec3*)&pOut[nPos];
//			memset(pInRec3, 0x20, sizeof(KB_d1022_InRec3));
//			nPos += sizeof(KB_d1022_InRec3);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].IndexNo);	// ��ǥ��ȣ(2)
//			memcpy(pInRec3->giIndexNo, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].GroupNo);	// �׷��ȣ(2)
//			memcpy(pInRec3->giGroupNo, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].GroupIdxCnt);	// �׷쳻����ǥ����(2)
//			memcpy(pInRec3->giGroupIdxCnt, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].TargetBase);	// Ÿ�ٵ���Ÿ(2)
//			memcpy(pInRec3->giTargetBase, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Type);	// ��ǥ������(4)
//			memcpy(pInRec3->giType, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].Type1);	// Type1��������(2)
//			memcpy(pInRec3->giType1, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].Type2);	// ������(2)
//			memcpy(pInRec3->giType2, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].STerm);	// �׷�˻�������ġ(4)
//			memcpy(pInRec3->giSTerm, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].ETerm);	// �׷�˻�����ġ(4)
//			memcpy(pInRec3->giETerm, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Rank);	// �����˻������(4)
//			memcpy(pInRec3->giRank, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Trend);	// �߼�Ƚ��(4)
//			memcpy(pInRec3->giTrend, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].SignalCnt1);	// ��ȣ�߻�Ƚ��1(4)
//			memcpy(pInRec3->giSignalCnt1, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].SignalCnt2);	// ��ȣ�߻�Ƚ��2(4)
//			memcpy(pInRec3->giSignalCnt2, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].ResultViewNum);	// ����������ʵ尹��(2)
//			memcpy(pInRec3->giResultViewNum, tmpVal, 2);
//
//			pInRec3->gsIndexType[0] = pUserFinder->stUser_Define[i].IndexType;          // �˻�����0:�Ϲ�1:����2:����(1) 
//			pInRec3->gsSearchType[0] = pUserFinder->stUser_Define[i].SearchType;        // �˻�����(1)                   
//			pInRec3->gsOperator[0] = pUserFinder->stUser_Define[i].Operator;            // ������0:OR1:AND2:NEXT(1)      
//			pInRec3->gsReverse[0] = pUserFinder->stUser_Define[i].Reverse;              // �������0:A1:NotA(1)          
//			pInRec3->gsIdxContiGubun[0] = pUserFinder->stUser_Define[i].IdxContiGubun;	// 0:���Ϲ߻�1:���ӹ߻���ǥ(1)   
//			pInRec3->gsElement[0] = pUserFinder->stUser_Define[i].Element;              // ���ǽĻ�빮��(A)(1)
//
//			memcpy(pInRec3->gsScode, pUserFinder->stUser_Define[i].Code, 6);	// shortcode(6)  
//
//			for(int j=0; j<MAX_PARAM; j++)
//			{
//				sprintf(tmpVal, "%016f", pUserFinder->stUser_Define[i].Param[j]);
//				memcpy(&pInRec3->gdParam[16*j], tmpVal, 16);	// ��ǥ���ú�����(48)
//			}
//
//			for(j=0; j<MAX_CONDI; j++)
//			{
//				sprintf(tmpVal, "%016f", pUserFinder->stUser_Define[i].Condition[j]);
//				memcpy(&pInRec3->gdCondition[16*j], tmpVal, 16);	// ��Ÿ����������(96) 
//			}
//		}
//	}
	void User_Finder_To_KB_d1022_InRec2(User_Finder* pUserFinder, char* pOut, int &nPos)
	{
		KB_d1022_InRec2* pInRec2 = (KB_d1022_InRec2*)&pOut[nPos];
		memset(pInRec2, 0x20, sizeof(KB_d1022_InRec2));

		CString szVal;
		char tmpVal[300]={0,};
		int  nTargetIdxCnt=0, nComplexIdxCnt=0;
		int i = 0;

		INT2STR_BUF(tmpVal, 4, pUserFinder->TrIndex);				// giCallGubun(4)
		memcpy(pInRec2->giCallGubun, tmpVal, 4);

		INT2STR_BUF(tmpVal, 2, pUserFinder->nTargetType);			// 0 :���� , 10 : ����	���� [NEW]
		memcpy(pInRec2->giTargetGubun, tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetIdxCnt);          // ��� ��ǥ ����       
		memcpy(pInRec2->giTargetIdxCnt, tmpVal, 2);
		tmpVal[2] = 0;
		nTargetIdxCnt = atoi(tmpVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetResultViewCnt);	// ��� �������ʵ� ���� 
		memcpy(pInRec2->giTargetResultViewCnt, tmpVal, 2);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->TargetExp[EXPLEN]);	// ������� ���� ǥ�� ��	
		//memcpy(pInRec2->gsTargetExp, pUserFinder->TargetExp, 80);
		Safe_CheckCopy2(pInRec2->gsTargetExp, pUserFinder->TargetExp, EXPLEN, szVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexIdxCnt);			// ���� ��ǥ ����       
		memcpy(pInRec2->giComplexIdxCnt, tmpVal, 2);
		tmpVal[2] = 0;
		nComplexIdxCnt = atoi(tmpVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexResultViewCnt);	// ���� �������ʵ� ���� 
		memcpy(pInRec2->giComplexResultViewCnt, tmpVal, 2);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->ComplexExp[EXPLEN]);	// �������� ���� ǥ�� ��	
		//memcpy(pInRec2->gsComplexExp, pUserFinder->ComplexExp, 80);
		Safe_CheckCopy2(pInRec2->gsComplexExp, pUserFinder->ComplexExp, EXPLEN, szVal);

		INT2STR_BUF(tmpVal, 4, pUserFinder->ResultIdxCnt);		// �ʵ����� ����   [Fix + Move] 
		memcpy(pInRec2->giResultIdxCnt, tmpVal, 4);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->FieldList[REAL_IDXCNT]);// �ʵ����� ��ȣ����Ʈ [Just Fix + Move] 
		//memcpy(pInRec2->gsResultExp, pUserFinder->FieldList, 80);
		Safe_CheckCopy2(pInRec2->gsResultExp, pUserFinder->ResultExp, EXPLEN, szVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->FieldEditerCnt);			// ������� ����          [NEW]
		memcpy(pInRec2->giFieldEditerCnt, tmpVal, 2);

// 		INT2STR_BUF(tmpVal, 240, pUserFinder->FieldList);	// ����� ������ ǥ�� ��  [NEW]		
// 		memcpy(pInRec2->giFieldList, pUserFinder->ResultExp, 240);
// 		Safe_CheckCopy2(pInRec2->giFieldList, pUserFinder->FieldList, 240, szVal);
// 		CString strNULL; strNULL.Format("%240s", " ");
// 		memcpy(pInRec2->giFieldList,strNULL, 240);
		for( i = 0; i < (int)pUserFinder->FieldEditerCnt; i++)
		{
			INT2STR_BUF(tmpVal, 4, pUserFinder->FieldList[i]);
			memcpy(&pInRec2->giFieldList[i * 4], tmpVal, 4);
		}
	
		//INT2STR_BUF(tmpVal, 1, pUserFinder->TargetBitChk);				// TargetBit ��� ���� üũ
		pInRec2->gsTargetBitChk[0] = pUserFinder->TargetBitChk;

		//INT2STR_BUF(tmpVal, 2500, &pUserFinder->TargetBit[0]);	// �˻� ��� ����			
		//memcpy(pInRec2->gsJongmokIndex, pUserFinder->TargetBit, RESULT_MAX);
		Safe_CheckCopy2(pInRec2->gsJongmokIndex, pUserFinder->TargetBit, 4000, szVal);	//3000 -> 4000

		INT2STR_BUF(tmpVal, 1, pUserFinder->Sort_flag);					// ���� ����(0:����, 1:����)
		pInRec2->gsSort_flag[0] = pUserFinder->Sort_flag;

		INT2STR_BUF(tmpVal, 4, pUserFinder->Sort_Num);					// ���� ��ȣ(�������ʵ�pos) 	    
		memcpy(pInRec2->giSort_Pos, tmpVal, 4);

		if(pUserFinder->BackDay==-1)	sprintf(tmpVal,"-001");
		else		INT2STR_BUF(tmpVal, 4, pUserFinder->BackDay);					// backtest ����(�̻��:-1) 	
		memcpy(pInRec2->giBackDate, tmpVal, 4);

		if(pUserFinder->BackTime==-1)	sprintf(tmpVal,"-001");
		else	INT2STR_BUF(tmpVal, 4, pUserFinder->BackTime);					// backtest �ð�(�̻��:-1) 
		memcpy(pInRec2->giBackTime, tmpVal, 4);

		nPos += sizeof(KB_d1022_InRec2);

		KB_d1022_InRec3* pInRec3;
		for(i=0; i<nTargetIdxCnt+nComplexIdxCnt; i++)
		{
			pInRec3 = (KB_d1022_InRec3*)&pOut[nPos];
			memset(pInRec3, 0x20, sizeof(KB_d1022_InRec3));
			nPos += sizeof(KB_d1022_InRec3);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].IndexNo);	// ��ǥ��ȣ(2)
			memcpy(pInRec3->giIndexNo, tmpVal, 2);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].GroupNo);	// �׷��ȣ(2)
			memcpy(pInRec3->giGroupNo, tmpVal, 2);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].GroupIdxCnt);	// �׷쳻����ǥ����(2)
			memcpy(pInRec3->giGroupIdxCnt, tmpVal, 2);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].TargetBase);	// Ÿ�ٵ���Ÿ(2)
			memcpy(pInRec3->giTargetBase, tmpVal, 2);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Type);	// ��ǥ������(4)
			memcpy(pInRec3->giType, tmpVal, 4);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].Type1);	// Type1��������(2)
			memcpy(pInRec3->giType1, tmpVal, 2);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].Type2);	// ������(2)
			memcpy(pInRec3->giType2, tmpVal, 2);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].STerm);	// �׷�˻�������ġ(4)
			memcpy(pInRec3->giSTerm, tmpVal, 4);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].ETerm);	// �׷�˻�����ġ(4)
			memcpy(pInRec3->giETerm, tmpVal, 4);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Rank);	// �����˻������(4)
			memcpy(pInRec3->giRank, tmpVal, 4);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Trend);	// �߼�Ƚ��(4)
			memcpy(pInRec3->giTrend, tmpVal, 4);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].SignalCnt1);	// ��ȣ�߻�Ƚ��1(4)
			memcpy(pInRec3->giSignalCnt1, tmpVal, 4);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].SignalCnt2);	// ��ȣ�߻�Ƚ��2(4)
			memcpy(pInRec3->giSignalCnt2, tmpVal, 4);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].ResultViewNum);	// ����������ʵ尹��(2)
			memcpy(pInRec3->giResultViewNum, tmpVal, 2);

			pInRec3->gsIndexType[0] = pUserFinder->stUser_Define[i].IndexType;          // �˻�����0:�Ϲ�1:����2:����(1) 
			pInRec3->gsSearchType[0] = pUserFinder->stUser_Define[i].SearchType;        // �˻�����(1)                   
			pInRec3->gsOperator[0] = pUserFinder->stUser_Define[i].Operator;            // ������0:OR1:AND2:NEXT(1)      
			pInRec3->gsReverse[0] = pUserFinder->stUser_Define[i].Reverse;              // �������0:A1:NotA(1)          
			pInRec3->gsIdxContiGubun[0] = pUserFinder->stUser_Define[i].IdxContiGubun;	// 0:���Ϲ߻�1:���ӹ߻���ǥ(1)   
			pInRec3->gsElement[0] = pUserFinder->stUser_Define[i].Element;              // ���ǽĻ�빮��(A)(1)

			memcpy(pInRec3->gsScode, pUserFinder->stUser_Define[i].Code, 6);	// shortcode(6)  

			int j = 0;
			for(j=0; j<MAX_PARAM; j++)
			{
				sprintf(tmpVal, "%016f", pUserFinder->stUser_Define[i].Param[j]);
				memcpy(&pInRec3->gdParam[16*j], tmpVal, 16);	// ��ǥ���ú�����(96)
			}

			for(j=0; j<MAX_CONDI; j++)
			{
				sprintf(tmpVal, "%016f", pUserFinder->stUser_Define[i].Condition[j]);
				memcpy(&pInRec3->gdCondition[16*j], tmpVal, 16);	// ��Ÿ����������(192) 
			}
		}
	}

	// --------------------------------------------------------------------------------------
	// Outputó��
	// --------------------------------------------------------------------------------------
	int  D1022OutRec1_GetDataCount(KB_d1022_OutRec1* pOutRec)
	{
		char szTmp[5] = {NULL, };
		::memcpy(szTmp, pOutRec->gsJongCnt, sizeof(pOutRec->gsJongCnt)); // �˻��� ���� ���� ��
		if(::lstrlen(szTmp) == 0) return 0;

		int nRet = atoi(szTmp);
		{
//			if(szTmp.CompareNoCase("-900")==0) AfxMessageBox("��� 0");
//			else
			if(!::strcmp(szTmp, "-901"))
				AfxMessageBox("Input����");
		}
		// ��ȸ�� ������ ���� ��� (-900)�� ���ϵȴٰ� ��.
		return (nRet<0)? 0: nRet;
	}

	void D1022OutRec1_SetData(KB_d1022_OutRec1* pOutRec, TR_RESULT_HEADER* pTrResult)
	{
		//TR_RESULT_HEADER* pTrResult = (TR_RESULT_HEADER*)pOutBuf;
		//ZeroMemory(pTrResult, sizeof(TR_RESULT_HEADER));
		memset(pTrResult, 0x20, sizeof(TR_RESULT_HEADER));

		char* szTemp = new char[7];
		
		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec, 4);

		pTrResult->giCallIndex = atoi(szTemp);				// Ŭ���̾�Ʈ��ȸ����(1)

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsJongCnt, sizeof(pOutRec->gsJongCnt));
		Safe_CheckCpy(pTrResult->sJongCnt, szTemp);		// �˻������(4)
		int nRet = atoi(szTemp);
		if(nRet<=0)
		{
			//memset(pTrResult->sJongCnt, 0x20, )
			return;
		}

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsHigh, sizeof(pOutRec->gsHigh));
		Safe_CheckCpy(pTrResult->sHigh, szTemp);		// ���������(4)        

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsUp, sizeof(pOutRec->gsUp));
		Safe_CheckCpy(pTrResult->sUp, szTemp);			// ��������(4)        

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsNoch, sizeof(pOutRec->gsNoch));
		Safe_CheckCpy(pTrResult->sNoch, szTemp);		// ���������(4)        

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsDown, sizeof(pOutRec->gsDown));
		Safe_CheckCpy(pTrResult->sDown, szTemp);		// �϶������(4)        

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsLow, sizeof(pOutRec->gsLow));
		Safe_CheckCpy(pTrResult->sLow, szTemp);			// ���������(4)        

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsUpRate, sizeof(pOutRec->gsUpRate));
		Safe_CheckCpy(pTrResult->sUpRate, szTemp);		// ��º���(6)

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsChgrateAvg, sizeof(pOutRec->gsChgrateAvg));
		Safe_CheckCpy(pTrResult->sChgrateAvg, szTemp);	// ��յ����(6)

		delete [] szTemp;
	};

	void D1022OutRec2_SetData(KB_d1022_OutRec2* pOutRec2, char* pDataBuf, int nCnt)
		//TR_RESULT_LIST* pResultList
	{
		//TR_RESULT_LIST* pResultList = (TR_RESULT_LIST*)pOutBuf;

		int nPos=0;
		char szTemp[21];
		for(int i=0; i<nCnt; i++)
		{
			TR_RESULT_LIST* pResultList = (TR_RESULT_LIST*)&pDataBuf[nPos];
			nPos += sizeof(TR_RESULT_LIST);
			ZeroMemory(pResultList, sizeof(TR_RESULT_LIST));

			int nSize = sizeof(KB_d1022_OutRec2);
			KB_d1022_OutRec2* pOutRec = (KB_d1022_OutRec2*)(pOutRec2 + i);

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsMflag, sizeof(pOutRec->gsMflag));
			pResultList->sMarket[0] = szTemp[0];			// ���屸��(1)

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsCode, sizeof(pOutRec->gsCode));
			Safe_CheckCpy(pResultList->sCode, szTemp);	// �����ڵ�(6)    

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsHname, sizeof(pOutRec->gsHname));
			Safe_CheckCpy(pResultList->sName, szTemp);       // �����(20)         

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsSignCnt, sizeof(pOutRec->gsSignCnt));
			Safe_CheckCpy(pResultList->sSignCnt, szTemp);        // ������Ӻ���(3)       

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsClose, sizeof(pOutRec->gsClose));
			Safe_CheckCpy(pResultList->sPrice, szTemp);        // ���簡(8)          

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsSign, sizeof(pOutRec->gsSign));
			//Safe_CheckCpy(pResultList->sSign , szTemp);        // �����ȣ(1)        
			pResultList->sSign[0] = szTemp[0];

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsChange, sizeof(pOutRec->gsChange));
			Safe_CheckCpy(pResultList->sChange , szTemp);        // ���ϴ��(8)        

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsChgRate, sizeof(pOutRec->gsChgRate));
			Safe_CheckCpy(pResultList->sChgRate, szTemp);       // ���ϴ������(6)  

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsVolume, sizeof(pOutRec->gsVolume));
			Safe_CheckCpy(pResultList->sVolume, szTemp);       // �ŷ���(9)          

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsVolumeRate, sizeof(pOutRec->gsVolumeRate));
			Safe_CheckCpy(pResultList->sVolumeRate, szTemp);        // �ŷ������ϴ����(6)
		}
	}
};

#endif	// __EU_TREELIST_TRAN_KB_d1022_H__

