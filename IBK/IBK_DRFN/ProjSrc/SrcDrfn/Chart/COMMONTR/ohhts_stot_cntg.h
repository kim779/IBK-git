#if !defined(AFX_OHHTS_STOT_CNTG__H_3DA976CE_B086_4c68_A478_D6B8FE4D1765_INCLUDED_)
#define AFX_OHHTS_STOT_CNTG__H_3DA976CE_B086_4c68_A478_D6B8FE4D1765_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽĿɼ� ü�� (  )
#pragma pack( push, 1 )

#define NAME_ohhts_stot_cntg     "ohhts_stot_cntg"

// 
typedef struct _ohhts_stot_cntg_InBlock1
{
    char    OPTN_SHRN_ISCD      [   8];    // [string,    8] �ɼ� ���� �����ڵ�
} ohhts_stot_cntg_InBlock1, *LPohhts_stot_cntg_InBlock1;
#define NAME_ohhts_stot_cntg_InBlock1     "InBlock1"

// 
typedef struct _ohhts_stot_cntg_OutBlock1
{
    char    OPTN_SHRN_ISCD      [   8];    // [string,    8] �ɼ� ���� �����ڵ�
    char    CNTG_CLS_CODE       [   1];    // [string,    1] ü�� ���� �ڵ�
    char    OPTN_SRNO           [  11];    // [numstring,   11] �ɼ� �Ϸù�ȣ
    char    CNTG_HOUR           [   6];    // [string,    6] ü�� �ð�
    char    PRPR                [  19];    // [numstring,   19] ���簡
    char    PRDY_VRSS           [  11];    // [numstring,   11] ���� ���
    char    PRDY_VRSS_SIGN      [   1];    // [string,    1] ���� ��� ��ȣ
    char    PRDY_CTRT           [   9];    // [numstring,  9.2] ���� �����
    char    ASKP                [  11];    // [numstring,   11] �ŵ�ȣ��
    char    BIDP                [  11];    // [numstring,   11] �ż�ȣ��
    char    STCK_HGPR           [  11];    // [numstring,   11] �ֽ� �ְ�
    char    STCK_LWPR           [  11];    // [numstring,   11] �ֽ� ������
    char    STCK_OPRC           [  11];    // [numstring,   11] �ֽ� �ð�2
    char    CNQN                [  19];    // [numstring,   19] ü�ᷮ
    char    ACML_VOL            [  19];    // [numstring,   19] ���� �ŷ���
    char    ACML_TR_PBMN        [  19];    // [numstring,   19] ���� �ŷ� ���
    char    TOTAL_SELN_QTY      [  19];    // [numstring,   19] �� �ŵ� ����
    char    TOTAL_SHNU_QTY      [  19];    // [numstring,   19] �� �ż�2 ����
    char    OTST_STPL_QTY       [  13];    // [numstring,   13] �̰��� ���� ����
    char    OTST_STPL_QTY_ICDC  [  11];    // [numstring,   11] �̰��� ���� ���� ����
    char    OTST_STPL_RGBF_QTY_ICDC[  11];    // [numstring,   11] �̰��� ���� ���� ���� ����
    char    OPTN_THPR           [  12];    // [numstring, 12.2] �ɼ� �̷а�
    char    SELN_CNTG_CSNU      [  11];    // [numstring,   11] �ŵ� ü�� �Ǽ�
    char    SHNU_CNTG_CSNU      [  11];    // [numstring,   11] �ż�2 ü�� �Ǽ�
    char    NTBY_CNTG_CSNU      [  11];    // [numstring,   11] ���ż� ü�� �Ǽ�
} ohhts_stot_cntg_OutBlock1, *LPohhts_stot_cntg_OutBlock1;
#define NAME_ohhts_stot_cntg_OutBlock1     "OutBlock1"

class CREAL_STOCKOPTION
{
public: //Variable
    CString m_szOPTN_SHRN_ISCD      ;    // [string,    8] �ɼ� ���� �����ڵ�
    CString m_szCNTG_CLS_CODE       ;    // [string,    1] ü�� ���� �ڵ�
    CString m_szOPTN_SRNO           ;    // [numstring,   11] �ɼ� �Ϸù�ȣ
    CString m_szCNTG_HOUR           ;    // [string,    6] ü�� �ð�
    CString m_szPRPR                ;    // [numstring,   19] ���簡
    CString m_szPRDY_VRSS           ;    // [numstring,   11] ���� ���
    CString m_szPRDY_VRSS_SIGN      ;    // [string,    1] ���� ��� ��ȣ
    CString m_szPRDY_CTRT           ;    // [numstring,  9.2] ���� �����
    CString m_szASKP                ;    // [numstring,   11] �ŵ�ȣ��
    CString m_szBIDP                ;    // [numstring,   11] �ż�ȣ��
    CString m_szSTCK_HGPR           ;    // [numstring,   11] �ֽ� �ְ�
    CString m_szSTCK_LWPR           ;    // [numstring,   11] �ֽ� ������
    CString m_szSTCK_OPRC           ;    // [numstring,   11] �ֽ� �ð�2
    CString m_szCNQN                ;    // [numstring,   19] ü�ᷮ
    CString m_szACML_VOL            ;    // [numstring,   19] ���� �ŷ���
    CString m_szACML_TR_PBMN        ;    // [numstring,   19] ���� �ŷ� ���
    CString m_szTOTAL_SELN_QTY      ;    // [numstring,   19] �� �ŵ� ����
    CString m_szTOTAL_SHNU_QTY      ;    // [numstring,   19] �� �ż�2 ����
    CString m_szOTST_STPL_QTY       ;    // [numstring,   13] �̰��� ���� ����
    CString m_szOTST_STPL_QTY_ICDC  ;    // [numstring,   11] �̰��� ���� ���� ����
    CString m_szOTST_STPL_RGBF_QTY_ICDC;    // [numstring,   11] �̰��� ���� ���� ���� ����
    CString m_szOPTN_THPR           ;    // [numstring, 12.2] �ɼ� �̷а�
    CString m_szSELN_CNTG_CSNU      ;    // [numstring,   11] �ŵ� ü�� �Ǽ�
    CString m_szSHNU_CNTG_CSNU      ;    // [numstring,   11] �ż�2 ü�� �Ǽ�
    CString m_szNTBY_CNTG_CSNU      ;    // [numstring,   11] ���ż� ü�� �Ǽ�

public:
	CREAL_STOCKOPTION()
	{
	}

	virtual void  Convert(ohhts_stot_cntg_OutBlock1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, OPTN_SHRN_ISCD      );    // [string,    8] �ɼ� ���� �����ڵ�
		DATA_MEMCOPY(sTmp, pInput, CNTG_CLS_CODE       );    // [string,    1] ü�� ���� �ڵ�
		DATA_MEMCOPY(sTmp, pInput, OPTN_SRNO           );    // [numstring,   11] �ɼ� �Ϸù�ȣ
		DATA_MEMCOPY(sTmp, pInput, CNTG_HOUR           );    // [string,    6] ü�� �ð�
		DATA_MEMCOPY(sTmp, pInput, PRPR                );    // [numstring,   19] ���簡
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS           );    // [numstring,   11] ���� ���
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS_SIGN      );    // [string,    1] ���� ��� ��ȣ
		DATA_MEMCOPY(sTmp, pInput, PRDY_CTRT           );    // [numstring,  9.2] ���� �����
		DATA_MEMCOPY(sTmp, pInput, ASKP                );    // [numstring,   11] �ŵ�ȣ��
		DATA_MEMCOPY(sTmp, pInput, BIDP                );    // [numstring,   11] �ż�ȣ��
		DATA_MEMCOPY(sTmp, pInput, STCK_HGPR           );    // [numstring,   11] �ֽ� �ְ�
		DATA_MEMCOPY(sTmp, pInput, STCK_LWPR           );    // [numstring,   11] �ֽ� ������
		DATA_MEMCOPY(sTmp, pInput, STCK_OPRC           );    // [numstring,   11] �ֽ� �ð�2
		DATA_MEMCOPY(sTmp, pInput, CNQN                );    // [numstring,   19] ü�ᷮ
		DATA_MEMCOPY(sTmp, pInput, ACML_VOL            );    // [numstring,   19] ���� �ŷ���
		DATA_MEMCOPY(sTmp, pInput, ACML_TR_PBMN        );    // [numstring,   19] ���� �ŷ� ���
		DATA_MEMCOPY(sTmp, pInput, TOTAL_SELN_QTY      );    // [numstring,   19] �� �ŵ� ����
		DATA_MEMCOPY(sTmp, pInput, TOTAL_SHNU_QTY      );    // [numstring,   19] �� �ż�2 ����
		DATA_MEMCOPY(sTmp, pInput, OTST_STPL_QTY       );    // [numstring,   13] �̰��� ���� ����
		DATA_MEMCOPY(sTmp, pInput, OTST_STPL_QTY_ICDC  );    // [numstring,   11] �̰��� ���� ���� ����
		DATA_MEMCOPY(sTmp, pInput, OTST_STPL_RGBF_QTY_ICDC);    // [numstring,   11] �̰��� ���� ���� ���� ����
		DATA_MEMCOPY(sTmp, pInput, OPTN_THPR           );    // [numstring, 12.2] �ɼ� �̷а�
		DATA_MEMCOPY(sTmp, pInput, SELN_CNTG_CSNU      );    // [numstring,   11] �ŵ� ü�� �Ǽ�
		DATA_MEMCOPY(sTmp, pInput, SHNU_CNTG_CSNU      );    // [numstring,   11] �ż�2 ü�� �Ǽ�
		DATA_MEMCOPY(sTmp, pInput, NTBY_CNTG_CSNU      );    // [numstring,   11] ���ż� ü�� �Ǽ�
	}
};

#pragma pack( pop )

#endif // AFX_OHHTS_STOT_CNTG__H_3DA976CE_B086_4c68_A478_D6B8FE4D1765_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
