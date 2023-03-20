#if !defined(AFX_OHHTS_STFT_CNTG__H_B69EB983_8EBF_40a2_B9E6_29F54562219F_INCLUDED_)
#define AFX_OHHTS_STFT_CNTG__H_B69EB983_8EBF_40a2_B9E6_29F54562219F_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽļ��� ü�� (  )
#pragma pack( push, 1 )

#define NAME_ohhts_stft_cntg     "ohhts_stft_cntg"

// 
typedef struct _ohhts_stft_cntg_InBlock1
{
    char    FUTS_NEW_SHRN_ISCD  [   9];    // [string,    9] ���� �� ���� �����ڵ�
} ohhts_stft_cntg_InBlock1, *LPohhts_stft_cntg_InBlock1;
#define NAME_ohhts_stft_cntg_InBlock1     "InBlock1"

// 
typedef struct _ohhts_stft_cntg_OutBlock1
{
    char    FUTS_NEW_SHRN_ISCD  [   9];    // [string,    9] ���� �� ���� �����ڵ�
    char    CNTG_CLS_CODE       [   1];    // [string,    1] ü�� ���� �ڵ�
    char    FUTS_SRNO           [  11];    // [numstring,   11] ���� �Ϸù�ȣ
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
    char    BASIS               [   9];    // [numstring,  9.4] ���̽ý�
    char    FUTS_THPR           [  12];    // [numstring, 12.2] ���� �̷а�
    char    DPRT                [   9];    // [numstring,  9.2] ������
    char    SELN_CNTG_CSNU      [  11];    // [numstring,   11] �ŵ� ü�� �Ǽ�
    char    SHNU_CNTG_CSNU      [  11];    // [numstring,   11] �ż�2 ü�� �Ǽ�
    char    NTBY_CNTG_CSNU      [  11];    // [numstring,   11] ���ż� ü�� �Ǽ�
    char    CTTR                [  12];    // [numstring, 12.2] ü�ᰭ��
} ohhts_stft_cntg_OutBlock1, *LPohhts_stft_cntg_OutBlock1;
#define NAME_ohhts_stft_cntg_OutBlock1     "OutBlock1"

class CREAL_STOCKFUTURE
{
public: //Variable
    CString m_szFUTS_NEW_SHRN_ISCD  ;    // [string,    9] ���� �� ���� �����ڵ�
    CString m_szCNTG_CLS_CODE       ;    // [string,    1] ü�� ���� �ڵ�
    CString m_szFUTS_SRNO           ;    // [numstring,   11] ���� �Ϸù�ȣ
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
    CString m_szBASIS               ;    // [numstring,  9.4] ���̽ý�
    CString m_szFUTS_THPR           ;    // [numstring, 12.2] ���� �̷а�
    CString m_szDPRT                ;    // [numstring,  9.2] ������
    CString m_szSELN_CNTG_CSNU      ;    // [numstring,   11] �ŵ� ü�� �Ǽ�
    CString m_szSHNU_CNTG_CSNU      ;    // [numstring,   11] �ż�2 ü�� �Ǽ�
    CString m_szNTBY_CNTG_CSNU      ;    // [numstring,   11] ���ż� ü�� �Ǽ�
    CString m_szCTTR                ;    // [numstring, 12.2] ü�ᰭ��

public:
	CREAL_STOCKFUTURE()
	{
	}

	virtual void  Convert(ohhts_stft_cntg_OutBlock1* pInput)
	{
		CString sTemp1 ;
		char sTmp[20+1];

		DATA_MEMCOPY(sTmp, pInput, FUTS_NEW_SHRN_ISCD  );    // [string,    9] ���� �� ���� �����ڵ�
		DATA_MEMCOPY(sTmp, pInput, CNTG_CLS_CODE       );    // [string,    1] ü�� ���� �ڵ�
		DATA_MEMCOPY(sTmp, pInput, FUTS_SRNO           );    // [numstring,   11] ���� �Ϸù�ȣ
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

#endif // AFX_OHHTS_STFT_CNTG__H_B69EB983_8EBF_40a2_B9E6_29F54562219F_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
