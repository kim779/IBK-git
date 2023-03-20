#if !defined(AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_)
#define AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// ��������ü�� (  )
#pragma pack( push, 1 )

#define NAME_ohhts_futs_cntg     "ohhts_futs_cntg"

// 
typedef struct _ohhts_futs_cntg_InBlock1
{
    char    FUTS_SHRN_ISCD      [   8];    // [string,    8] ���� ���� �����ڵ�
} ohhts_futs_cntg_InBlock1, *LPohhts_futs_cntg_InBlock1;
#define NAME_ohhts_futs_cntg_InBlock1     "InBlock1"

// 
typedef struct _ohhts_futs_cntg_OutBlock1
{
    char    FUTS_SHRN_ISCD      [   8];    // [string,    8] ���� ���� �����ڵ�
    char    CNTG_CLS_CODE       [   1];    // [string,    1] ü�� ���� �ڵ�
    char    FUTS_SRNO           [  11];    // [numstring,   11] ���� �Ϸù�ȣ
    char    CNTG_HOUR           [   6];    // [string,    6] ü�� �ð�
    char    FUTS_PRPR           [  12];    // [numstring, 12.2] ���� ���簡
    char    FUTS_PRDY_VRSS      [  12];    // [numstring, 12.2] ���� ���� ���
    char    PRDY_VRSS_SIGN      [   1];    // [string,    1] ���� ��� ��ȣ
    char    FUTS_PRDY_CTRT      [   9];    // [numstring,  9.2] ���� ���� �����
    char    FUTS_ASKP1          [  12];    // [numstring, 12.2] ���� �ŵ�ȣ��1
    char    FUTS_BIDP1          [  12];    // [numstring, 12.2] ���� �ż�ȣ��1
    char    FUTS_HGPR           [  12];    // [numstring, 12.2] ���� �ְ�
    char    FUTS_LWPR           [  12];    // [numstring, 12.2] ���� ������
    char    FUTS_OPRC           [  12];    // [numstring, 12.2] ���� �ð�2
    char    CNQN                [  19];    // [numstring,   19] ü�ᷮ
    char    ACML_VOL            [  19];    // [numstring,   19] ���� �ŷ���
    char    ACML_TR_PBMN        [  19];    // [numstring,   19] ���� �ŷ� ���
    char    TOTAL_SELN_QTY      [  19];    // [numstring,   19] �� �ŵ� ����
    char    TOTAL_SHNU_QTY      [  19];    // [numstring,   19] �� �ż�2 ����
    char    OTST_STPL_QTY       [  13];    // [numstring,   13] �̰��� ���� ����
    char    OTST_STPL_QTY_ICDC  [  11];    // [numstring,   11] �̰��� ���� ���� ����
    char    OTST_STPL_RGBF_QTY_ICDC[  11];    // [numstring,   11] �̰��� ���� ���� ���� ����
    char    BASIS               [   9];    // [numstring,  9.4] ���̽ý�
    char    FUTS_THPR           [  12];    // [numstring, 12.2] ���� �̷а�
    char    DPRT                [   9];    // [numstring,  9.2] ������
    char    SELN_CNTG_CSNU      [  11];    // [numstring,   11] �ŵ� ü�� �Ǽ�
    char    SHNU_CNTG_CSNU      [  11];    // [numstring,   11] �ż�2 ü�� �Ǽ�
    char    NTBY_CNTG_CSNU      [  11];    // [numstring,   11] ���ż� ü�� �Ǽ�
    char    CTTR                [  12];    // [numstring, 12.2] ü�ᰭ��
} ohhts_futs_cntg_OutBlock1, *LPohhts_futs_cntg_OutBlock1;
#define NAME_ohhts_futs_cntg_OutBlock1     "OutBlock1"

class CREAL_FUTURE
{
public: //Variable
    CString m_szFUTS_SHRN_ISCD      ;    // [string,    8] ���� ���� �����ڵ�
    CString m_szCNTG_CLS_CODE       ;    // [string,    1] ü�� ���� �ڵ�
    CString m_szFUTS_SRNO           ;    // [numstring,   11] ���� �Ϸù�ȣ
    CString m_szCNTG_HOUR           ;    // [string,    6] ü�� �ð�
    CString m_szFUTS_PRPR           ;    // [numstring, 12.2] ���� ���簡
    CString m_szFUTS_PRDY_VRSS      ;    // [numstring, 12.2] ���� ���� ���
    CString m_szPRDY_VRSS_SIGN      ;    // [string,    1] ���� ��� ��ȣ
    CString m_szFUTS_PRDY_CTRT      ;    // [numstring,  9.2] ���� ���� �����
    CString m_szFUTS_ASKP1          ;    // [numstring, 12.2] ���� �ŵ�ȣ��1
    CString m_szFUTS_BIDP1          ;    // [numstring, 12.2] ���� �ż�ȣ��1
    CString m_szFUTS_HGPR           ;    // [numstring, 12.2] ���� �ְ�
    CString m_szFUTS_LWPR           ;    // [numstring, 12.2] ���� ������
    CString m_szFUTS_OPRC           ;    // [numstring, 12.2] ���� �ð�2
    CString m_szCNQN                ;    // [numstring,   19] ü�ᷮ
    CString m_szACML_VOL            ;    // [numstring,   19] ���� �ŷ���
    CString m_szACML_TR_PBMN        ;    // [numstring,   19] ���� �ŷ� ���
    CString m_szTOTAL_SELN_QTY      ;    // [numstring,   19] �� �ŵ� ����
    CString m_szTOTAL_SHNU_QTY      ;    // [numstring,   19] �� �ż�2 ����
    CString m_szOTST_STPL_QTY       ;    // [numstring,   13] �̰��� ���� ����
    CString m_szOTST_STPL_QTY_ICDC  ;    // [numstring,   11] �̰��� ���� ���� ����
    CString m_szOTST_STPL_RGBF_QTY_ICDC;    // [numstring,   11] �̰��� ���� ���� ���� ����
    CString m_szBASIS               ;    // [numstring,  9.4] ���̽ý�
    CString m_szFUTS_THPR           ;    // [numstring, 12.2] ���� �̷а�
    CString m_szDPRT                ;    // [numstring,  9.2] ������
    CString m_szSELN_CNTG_CSNU      ;    // [numstring,   11] �ŵ� ü�� �Ǽ�
    CString m_szSHNU_CNTG_CSNU      ;    // [numstring,   11] �ż�2 ü�� �Ǽ�
    CString m_szNTBY_CNTG_CSNU      ;    // [numstring,   11] ���ż� ü�� �Ǽ�
    CString m_szCTTR                ;    // [numstring, 12.2] ü�ᰭ��

public:
	CREAL_FUTURE()
	{
	}

	virtual void  Convert(ohhts_futs_cntg_OutBlock1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, FUTS_SHRN_ISCD      );    // [string,    8] ���� ���� �����ڵ�
		DATA_MEMCOPY(sTmp, pInput, CNTG_CLS_CODE       );    // [string,    1] ü�� ���� �ڵ�
		DATA_MEMCOPY(sTmp, pInput, FUTS_SRNO           );    // [numstring,   11] ���� �Ϸù�ȣ
		DATA_MEMCOPY(sTmp, pInput, CNTG_HOUR           );    // [string,    6] ü�� �ð�
		DATA_MEMCOPY(sTmp, pInput, FUTS_PRPR           );    // [numstring, 12.2] ���� ���簡
		DATA_MEMCOPY(sTmp, pInput, FUTS_PRDY_VRSS      );    // [numstring, 12.2] ���� ���� ���
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS_SIGN      );    // [string,    1] ���� ��� ��ȣ
		DATA_MEMCOPY(sTmp, pInput, FUTS_PRDY_CTRT      );    // [numstring,  9.2] ���� ���� �����
		DATA_MEMCOPY(sTmp, pInput, FUTS_ASKP1          );    // [numstring, 12.2] ���� �ŵ�ȣ��1
		DATA_MEMCOPY(sTmp, pInput, FUTS_BIDP1          );    // [numstring, 12.2] ���� �ż�ȣ��1
		DATA_MEMCOPY(sTmp, pInput, FUTS_HGPR           );    // [numstring, 12.2] ���� �ְ�
		DATA_MEMCOPY(sTmp, pInput, FUTS_LWPR           );    // [numstring, 12.2] ���� ������
		DATA_MEMCOPY(sTmp, pInput, FUTS_OPRC           );    // [numstring, 12.2] ���� �ð�2
		DATA_MEMCOPY(sTmp, pInput, CNQN                );    // [numstring,   19] ü�ᷮ
		DATA_MEMCOPY(sTmp, pInput, ACML_VOL            );    // [numstring,   19] ���� �ŷ���
		DATA_MEMCOPY(sTmp, pInput, ACML_TR_PBMN        );    // [numstring,   19] ���� �ŷ� ���
		DATA_MEMCOPY(sTmp, pInput, TOTAL_SELN_QTY      );    // [numstring,   19] �� �ŵ� ����
		DATA_MEMCOPY(sTmp, pInput, TOTAL_SHNU_QTY      );    // [numstring,   19] �� �ż�2 ����
		DATA_MEMCOPY(sTmp, pInput, OTST_STPL_QTY       );    // [numstring,   13] �̰��� ���� ����
		DATA_MEMCOPY(sTmp, pInput, OTST_STPL_QTY_ICDC  );    // [numstring,   11] �̰��� ���� ���� ����
		DATA_MEMCOPY(sTmp, pInput, OTST_STPL_RGBF_QTY_ICDC);    // [numstring,   11] �̰��� ���� ���� ���� ����
		DATA_MEMCOPY(sTmp, pInput, BASIS               );    // [numstring,  9.4] ���̽ý�
		DATA_MEMCOPY(sTmp, pInput, FUTS_THPR           );    // [numstring, 12.2] ���� �̷а�
		DATA_MEMCOPY(sTmp, pInput, DPRT                );    // [numstring,  9.2] ������
		DATA_MEMCOPY(sTmp, pInput, SELN_CNTG_CSNU      );    // [numstring,   11] �ŵ� ü�� �Ǽ�
		DATA_MEMCOPY(sTmp, pInput, SHNU_CNTG_CSNU      );    // [numstring,   11] �ż�2 ü�� �Ǽ�
		DATA_MEMCOPY(sTmp, pInput, NTBY_CNTG_CSNU      );    // [numstring,   11] ���ż� ü�� �Ǽ�
		DATA_MEMCOPY(sTmp, pInput, CTTR                );    // [numstring, 12.2] ü�ᰭ��
	}
};

#pragma pack( pop )

#endif // AFX_OHHTS_FUTS_CNTG__H_BFF8EF3A_A8B6_4aa0_A693_2B5A9E3CEAE9_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
