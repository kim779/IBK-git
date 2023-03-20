#ifndef AFX_INDUSTRYTHEMECONVERT__HEADER
#define AFX_INDUSTRYTHEMECONVERT__HEADER

#include "EU_p0622.h"
#include "../IndustryTheme/DefineFiles/Server_typedef.h"

#define INT2STR_BUF(xStr, nSize, nStr) sprintf(xStr, "%#0*.*d", nSize, nSize, nStr)
#define Safe_CheckCopy(xSource, xTarget) if(xTarget.GetLength()) memcpy(xSource, (LPCSTR)xTarget, xTarget.GetLength())
#define Safe_CheckCopy2(xSource, xTarget, xLen, xStr) \
	xStr.Format("%-*.*s", xLen, xLen, xTarget); \
	xStr.TrimRight();	\
	if(xStr.GetLength()) memcpy(xSource, (LPCSTR)xStr, xStr.GetLength())

class CIndustryThemeTR
{
public:
	// --------------------------------------------------------------------------------------
	// Inputó��
	// --------------------------------------------------------------------------------------
	void TR_RESULT_IN_To_EU_p0622_InRec1(TR_RESULT_IN* pIn, EU_p0622_InRec1* pOut)
	{
		memset(pOut, 0x20, sizeof(EU_p0622_InRec1));

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

	void User_Finder_To_EU_p0622_InRec2(User_Finder* pUserFinder, char* pOut, int &nPos, BOOL bTheme = FALSE)
	{
		EU_p0622_InRec2* pInRec2 = (EU_p0622_InRec2*)&pOut[nPos];
		memset(pInRec2, 0x20, sizeof(EU_p0622_InRec2));

		CString szVal;
		char tmpVal[100]={0,};
		int  nTargetIdxCnt=0, nComplexIdxCnt=0;

		INT2STR_BUF(tmpVal, 4, pUserFinder->TrIndex);				// giCallGubun(4)
		memcpy(pInRec2->giCallGubun, tmpVal, 4);

		if(bTheme)
		{
			memcpy(pInRec2->giTargetGubun, "20", 2);
		}
		else
		{
			INT2STR_BUF(tmpVal, 2, pUserFinder->nTargetType);			// 0 :���� , 10 : ����	���� [NEW]
			memcpy(pInRec2->giTargetGubun, tmpVal, 2);
		}

		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetIdxCnt);          // ��� ��ǥ ����       
		memcpy(pInRec2->giTargetIdxCnt, tmpVal, 2);
		tmpVal[2] = 0;
		nTargetIdxCnt = atoi(tmpVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetResultViewCnt);	// ��� �������ʵ� ���� 
		memcpy(pInRec2->giTargetResultViewCnt, tmpVal, 2);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->TargetExp[EXPLEN]);	// ������� ���� ǥ�� ��	
		//memcpy(pInRec2->gsTargetExp, pUserFinder->TargetExp, 80);
		Safe_CheckCopy2(pInRec2->gsTargetExp, pUserFinder->TargetExp, 80, szVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexIdxCnt);			// ���� ��ǥ ����       
		memcpy(pInRec2->giComplexIdxCnt, tmpVal, 2);
		tmpVal[2] = 0;
		nComplexIdxCnt = atoi(tmpVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexResultViewCnt);	// ���� �������ʵ� ���� 
		memcpy(pInRec2->giComplexResultViewCnt, tmpVal, 2);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->ComplexExp[EXPLEN]);	// �������� ���� ǥ�� ��	
		//memcpy(pInRec2->gsComplexExp, pUserFinder->ComplexExp, 80);
		Safe_CheckCopy2(pInRec2->gsComplexExp, pUserFinder->ComplexExp, 80, szVal);

		INT2STR_BUF(tmpVal, 4, pUserFinder->ResultIdxCnt);		// �ʵ����� ����   [Fix + Move] 
		memcpy(pInRec2->giResultIdxCnt, tmpVal, 4);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->FieldList[REAL_IDXCNT]);// �ʵ����� ��ȣ����Ʈ [Just Fix + Move] 
		//memcpy(pInRec2->gsResultExp, pUserFinder->FieldList, 80);
		Safe_CheckCopy2(pInRec2->gsResultExp, pUserFinder->ResultExp, 80, szVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->FieldEditerCnt);			// ������� ����          [NEW]
		memcpy(pInRec2->giFieldEditerCnt, tmpVal, 2);

	//	INT2STR_BUF(tmpVal, 80, pUserFinder->FieldList);	// ����� ������ ǥ�� ��  [NEW]		
		//memcpy(pInRec2->giFieldList, pUserFinder->ResultExp, 80);
	//	Safe_CheckCopy2(pInRec2->giFieldList, pUserFinder->FieldList, 80, szVal);
		CString strNULL; strNULL.Format("%80s", " ");
		memcpy(pInRec2->giFieldList,strNULL, 80);
	

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

		nPos += sizeof(EU_p0622_InRec2);

		EU_p0622_InRec3* pInRec3;
		for(int i=0; i<nTargetIdxCnt+nComplexIdxCnt; i++)
		{
			pInRec3 = (EU_p0622_InRec3*)&pOut[nPos];
			memset(pInRec3, 0x20, sizeof(EU_p0622_InRec3));
			nPos += sizeof(EU_p0622_InRec3);

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

			for(int j=0; j<MAX_PARAM; j++)
			{
				sprintf(tmpVal, "%016f", pUserFinder->stUser_Define[i].Param[j]);
				memcpy(&pInRec3->gdParam[16*j], tmpVal, 16);	// ��ǥ���ú�����(48)
			}

			for(j=0; j<MAX_CONDI; j++)
			{
				sprintf(tmpVal, "%016f", pUserFinder->stUser_Define[i].Condition[j]);
				memcpy(&pInRec3->gdCondition[16*j], tmpVal, 16);	// ��Ÿ����������(96) 
			}
		}
	}

	// --------------------------------------------------------------------------------------
	// Outputó��
	// --------------------------------------------------------------------------------------
	int  P0622OutRec1_GetDataCount(CHARVECTOR2 &vvRpValueList)
	{
		CString szTmp;
		szTmp	 = vvRpValueList[1][1];	// �˻��� ���� ���� ��
		if(szTmp.GetLength()==0) return 0;

		int nRet = atoi(szTmp);
		{
//			if(szTmp.CompareNoCase("-900")==0) AfxMessageBox("��� 0");
//			else
				if(szTmp.CompareNoCase("-901")==0) AfxMessageBox("Input����");
		}
		// ��ȸ�� ������ ���� ��� (-900)�� ���ϵȴٰ� ��.
		return (nRet<0)? 0: nRet;
	}

	void P0622OutRec1_SetData(CHARVECTOR2 &vvRpValueList, TR_RESULT_HEADER* pTrResult)
	{
		//TR_RESULT_HEADER* pTrResult = (TR_RESULT_HEADER*)pOutBuf;
		//ZeroMemory(pTrResult, sizeof(TR_RESULT_HEADER));
		memset(pTrResult, 0x20, sizeof(TR_RESULT_HEADER));

		CString szTmp;
		szTmp	 = vvRpValueList[1][0];
		pTrResult->nTrIndex = atoi(szTmp);				// Ŭ���̾�Ʈ��ȸ����(1)

		szTmp	 = vvRpValueList[1][1];
		Safe_CheckCopy(pTrResult->sJongCnt, szTmp);		// �˻������(4)
		int nRet = atoi(szTmp);
		if(nRet<=0)
		{
			//memset(pTrResult->sJongCnt, 0x20, )
			return;
		}

		szTmp	 = vvRpValueList[1][2];
		Safe_CheckCopy(pTrResult->sHigh, szTmp);		// ���������(4)        

		szTmp	 = vvRpValueList[1][3];
		Safe_CheckCopy(pTrResult->sUp, szTmp);			// ��������(4)        

		szTmp	 = vvRpValueList[1][4];
		Safe_CheckCopy(pTrResult->sNoch, szTmp);		// ���������(4)        

		szTmp	 = vvRpValueList[1][5];
		Safe_CheckCopy(pTrResult->sDown, szTmp);		// �϶������(4)        

		szTmp	 = vvRpValueList[1][6];
		Safe_CheckCopy(pTrResult->sLow, szTmp);			// ���������(4)        

		szTmp	 = vvRpValueList[1][7];
		Safe_CheckCopy(pTrResult->sUpRate, szTmp);		// ��º���(6)

		szTmp	 = vvRpValueList[1][8];
		Safe_CheckCopy(pTrResult->sChgrateAvg, szTmp);	// ��յ����(6)
	};

	void P0622OutRec2_SetData(CHARVECTOR2 &vvRpValueList, char* pDataBuf, int nCnt)
		//TR_RESULT_LIST* pResultList
	{
		//TR_RESULT_LIST* pResultList = (TR_RESULT_LIST*)pOutBuf;

		int nPos=0;
		CString szTmp;
		for(int i=0; i<nCnt; i++)
		{
			TR_RESULT_LIST* pResultList = (TR_RESULT_LIST*)&pDataBuf[nPos];
			nPos += sizeof(TR_RESULT_LIST);
			ZeroMemory(pResultList, sizeof(TR_RESULT_LIST));

			szTmp	 = vvRpValueList[i+1][0];
			pResultList->sMarket[0] = szTmp[0];			// ���屸��(1)

			szTmp	 = vvRpValueList[i+1][1];
			Safe_CheckCopy(pResultList->sCode, szTmp);	// �����ڵ�(6)    

			szTmp	 = vvRpValueList[i+1][2];
			Safe_CheckCopy(pResultList->sName, szTmp);       // �����(20)         

			szTmp	 = vvRpValueList[i+1][3];
			Safe_CheckCopy(pResultList->sSignCnt, szTmp);        // ������Ӻ���(3)       

			szTmp	 = vvRpValueList[i+1][4];
			Safe_CheckCopy(pResultList->sPrice, szTmp);        // ���簡(8)          

			szTmp	 = vvRpValueList[i+1][5];
			//Safe_CheckCopy(pResultList->sSign , szTmp);        // �����ȣ(1)        
			pResultList->sSign[0] = szTmp[0];

			szTmp	 = vvRpValueList[i+1][6];
			Safe_CheckCopy(pResultList->sChange , szTmp);        // ���ϴ��(8)        

			szTmp	 = vvRpValueList[i+1][7];
			Safe_CheckCopy(pResultList->sChgRate, szTmp);       // ���ϴ������(6)  

			szTmp	 = vvRpValueList[i+1][8];
			Safe_CheckCopy(pResultList->sVolume, szTmp);       // �ŷ���(9)          

			szTmp	 = vvRpValueList[i+1][9];
			Safe_CheckCopy(pResultList->sVolumeRate, szTmp);        // �ŷ������ϴ����(6)
		}
	}
};

#endif //AFX_INDUSTRYTHEMECONVERT__HEADER
