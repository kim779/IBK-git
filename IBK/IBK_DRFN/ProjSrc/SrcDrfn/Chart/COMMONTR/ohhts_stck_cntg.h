#if !defined(AFX_OHHTS_STCK_CNTG__H_318EC91F_0936_47a9_88B8_1911DEA0A108_INCLUDED_)
#define AFX_OHHTS_STCK_CNTG__H_318EC91F_0936_47a9_88B8_1911DEA0A108_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// �ֽ�ü�� �ڵ����� (  )
#pragma pack( push, 1 )

#define NAME_ohhts_stck_cntg     "ohhts_stck_cntg"

// 
typedef struct _ohhts_stck_cntg_InBlock1
{
    char    MKSC_SHRN_ISCD      [   6];    // [string,    6] �������� ���� �����ڵ�
} ohhts_stck_cntg_InBlock1, *LPohhts_stck_cntg_InBlock1;
#define NAME_ohhts_stck_cntg_InBlock1     "InBlock1"

// 
typedef struct _ohhts_stck_cntg_OutBlock1
{
    char    MKSC_SHRN_ISCD      [   6];    // [string,    6] �������� ���� �����ڵ�
    char    TR_SUSP_CLS_CODE    [   2];    // [string,    2] �ŷ� ���� ���� �ڵ�
    char    CNTG_CLS_CODE       [   1];    // [string,    1] ü�� ���� �ڵ�
    char    PRDY_VRSS_SIGN      [   1];    // [string,    1] ���� ��� ��ȣ
    char    STCK_DATA_SRNO      [  21];    // [numstring,   21] �ֽ� ������ �Ϸù�ȣ
    char    STCK_CNTG_HOUR      [   6];    // [string,    6] �ֽ� ü�� �ð�
    char    STCK_PRPR           [  11];    // [numstring,   11] �ֽ� ���簡
    char    PRDY_VRSS           [  11];    // [numstring,   11] ���� ���
    char    PRDY_CTRT           [   9];    // [numstring,  9.2] ���� �����
    char    ASKP                [  11];    // [numstring,   11] �ŵ�ȣ��
    char    BIDP                [  11];    // [numstring,   11] �ż�ȣ��
    char    ASKP_RSQN1          [  13];    // [numstring,   13] �ŵ�ȣ�� �ܷ�1
    char    BIDP_RSQN1          [  13];    // [numstring,   13] �ż�ȣ�� �ܷ�1
    char    STCK_HGPR           [  11];    // [numstring,   11] �ֽ� �ְ�
    char    STCK_LWPR           [  11];    // [numstring,   11] �ֽ� ������
    char    CNTG_VOL            [  19];    // [numstring,   19] ü�� �ŷ���
    char    ACML_VOL            [  19];    // [numstring,   19] ���� �ŷ���
    char    ACML_TR_PBMN        [  19];    // [numstring,   19] ���� �ŷ� ���
    char    TOTAL_ASKP_RSQN     [  13];    // [numstring,   13] �� �ŵ�ȣ�� �ܷ�
    char    TOTAL_BIDP_RSQN     [  13];    // [numstring,   13] �� �ż�ȣ�� �ܷ�
    char    STCK_OPRC           [  11];    // [numstring,   11] �ֽ� �ð�2
    char    TOTAL_SELN_QTY      [  19];    // [numstring,   19] �� �ŵ� ����
    char    TOTAL_SHNU_QTY      [  19];    // [numstring,   19] �� �ż�2 ����
    char    WGHN_AVRG_STCK_PRC  [  20];    // [numstring, 20.2] ���� ��� �ֽ� ����
    char    HTS_AVLS            [  19];    // [numstring,   19] HTS �ð��Ѿ�
    char    HOUR_CLS_CODE       [   1];    // [string,    1] �ð� ���� �ڵ�
    char    CTTR                [  12];    // [numstring, 12.2] ü�ᰭ��
    char    STCK_PRDY_CLPR      [  11];    // [numstring,   11] �ֽ� ���� ����
    char    NEW_MKOP_CLS_CODE   [   2];    // [string,    2] �� �� ���� �ڵ�
    char    SELN_CNTG_QTY       [  19];    // [numstring,   19] �ŵ� ü�� ����
    char    SHNU_CNTG_QTY       [  19];    // [numstring,   19] �ż�2 ü�� ����
    char    NTBY_CNQN           [  19];    // [numstring,   19] ���ż� ü�ᷮ
} ohhts_stck_cntg_OutBlock1, *LPohhts_stck_cntg_OutBlock1;
#define NAME_ohhts_stck_cntg_OutBlock1     "OutBlock1"

class CReal_STOCK
{
public: //Variable
    CString m_szMKSC_SHRN_ISCD      ;    // [string,    6] �������� ���� �����ڵ�
    CString m_szTR_SUSP_CLS_CODE    ;    // [string,    2] �ŷ� ���� ���� �ڵ�
    CString m_szCNTG_CLS_CODE       ;    // [string,    1] ü�� ���� �ڵ�
    CString m_szPRDY_VRSS_SIGN      ;    // [string,    1] ���� ��� ��ȣ
    CString m_szSTCK_DATA_SRNO      ;    // [numstring,   21] �ֽ� ������ �Ϸù�ȣ
    CString m_szSTCK_CNTG_HOUR      ;    // [string,    6] �ֽ� ü�� �ð�
    CString m_szSTCK_PRPR           ;    // [numstring,   11] �ֽ� ���簡
    CString m_szPRDY_VRSS           ;    // [numstring,   11] ���� ���
    CString m_szPRDY_CTRT           ;    // [numstring,  9.2] ���� �����
    CString m_szASKP                ;    // [numstring,   11] �ŵ�ȣ��
    CString m_szBIDP                ;    // [numstring,   11] �ż�ȣ��
    CString m_szASKP_RSQN1          ;    // [numstring,   13] �ŵ�ȣ�� �ܷ�1
    CString m_szBIDP_RSQN1          ;    // [numstring,   13] �ż�ȣ�� �ܷ�1
    CString m_szSTCK_HGPR           ;    // [numstring,   11] �ֽ� �ְ�
    CString m_szSTCK_LWPR           ;    // [numstring,   11] �ֽ� ������
    CString m_szCNTG_VOL            ;    // [numstring,   19] ü�� �ŷ���
    CString m_szACML_VOL            ;    // [numstring,   19] ���� �ŷ���
    CString m_szACML_TR_PBMN        ;    // [numstring,   19] ���� �ŷ� ���
    CString m_szTOTAL_ASKP_RSQN     ;    // [numstring,   13] �� �ŵ�ȣ�� �ܷ�
    CString m_szTOTAL_BIDP_RSQN     ;    // [numstring,   13] �� �ż�ȣ�� �ܷ�
    CString m_szSTCK_OPRC           ;    // [numstring,   11] �ֽ� �ð�2
    CString m_szTOTAL_SELN_QTY      ;    // [numstring,   19] �� �ŵ� ����
    CString m_szTOTAL_SHNU_QTY      ;    // [numstring,   19] �� �ż�2 ����
    CString m_szWGHN_AVRG_STCK_PRC  ;    // [numstring, 20.2] ���� ��� �ֽ� ����
    CString m_szHTS_AVLS            ;    // [numstring,   19] HTS �ð��Ѿ�
    CString m_szHOUR_CLS_CODE       ;    // [string,    1] �ð� ���� �ڵ�
    CString m_szCTTR                ;    // [numstring, 12.2] ü�ᰭ��
    CString m_szSTCK_PRDY_CLPR      ;    // [numstring,   11] �ֽ� ���� ����
    CString m_szNEW_MKOP_CLS_CODE   ;    // [string,    2] �� �� ���� �ڵ�
    CString m_szSELN_CNTG_QTY       ;    // [numstring,   19] �ŵ� ü�� ����
    CString m_szSHNU_CNTG_QTY       ;    // [numstring,   19] �ż�2 ü�� ����
    CString m_szNTBY_CNQN           ;    // [numstring,   19] ���ż� ü�ᷮ

public:
	CReal_STOCK()
	{
	}

	virtual void  Convert(ohhts_stck_cntg_OutBlock1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, MKSC_SHRN_ISCD      );    // [string,    6] �������� ���� �����ڵ�
		DATA_MEMCOPY(sTmp, pInput, TR_SUSP_CLS_CODE    );    // [string,    2] �ŷ� ���� ���� �ڵ�
		DATA_MEMCOPY(sTmp, pInput, CNTG_CLS_CODE       );    // [string,    1] ü�� ���� �ڵ�
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS_SIGN      );    // [string,    1] ���� ��� ��ȣ
		DATA_MEMCOPY(sTmp, pInput, STCK_DATA_SRNO      );    // [numstring,   21] �ֽ� ������ �Ϸù�ȣ
		DATA_MEMCOPY(sTmp, pInput, STCK_CNTG_HOUR      );    // [string,    6] �ֽ� ü�� �ð�
		DATA_MEMCOPY(sTmp, pInput, STCK_PRPR           );    // [numstring,   11] �ֽ� ���簡
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS           );    // [numstring,   11] ���� ���
		DATA_MEMCOPY(sTmp, pInput, PRDY_CTRT           );    // [numstring,  9.2] ���� �����
		DATA_MEMCOPY(sTmp, pInput, ASKP                );    // [numstring,   11] �ŵ�ȣ��
		DATA_MEMCOPY(sTmp, pInput, BIDP                );    // [numstring,   11] �ż�ȣ��
		DATA_MEMCOPY(sTmp, pInput, ASKP_RSQN1          );    // [numstring,   13] �ŵ�ȣ�� �ܷ�1
		DATA_MEMCOPY(sTmp, pInput, BIDP_RSQN1          );    // [numstring,   13] �ż�ȣ�� �ܷ�1
		DATA_MEMCOPY(sTmp, pInput, STCK_HGPR           );    // [numstring,   11] �ֽ� �ְ�
		DATA_MEMCOPY(sTmp, pInput, STCK_LWPR           );    // [numstring,   11] �ֽ� ������
		DATA_MEMCOPY(sTmp, pInput, CNTG_VOL            );    // [numstring,   19] ü�� �ŷ���
		DATA_MEMCOPY(sTmp, pInput, ACML_VOL            );    // [numstring,   19] ���� �ŷ���
		DATA_MEMCOPY(sTmp, pInput, ACML_TR_PBMN        );    // [numstring,   19] ���� �ŷ� ���
		DATA_MEMCOPY(sTmp, pInput, TOTAL_ASKP_RSQN     );    // [numstring,   13] �� �ŵ�ȣ�� �ܷ�
		DATA_MEMCOPY(sTmp, pInput, TOTAL_BIDP_RSQN     );    // [numstring,   13] �� �ż�ȣ�� �ܷ�
		DATA_MEMCOPY(sTmp, pInput, STCK_OPRC           );    // [numstring,   11] �ֽ� �ð�2
		DATA_MEMCOPY(sTmp, pInput, TOTAL_SELN_QTY      );    // [numstring,   19] �� �ŵ� ����
		DATA_MEMCOPY(sTmp, pInput, TOTAL_SHNU_QTY      );    // [numstring,   19] �� �ż�2 ����
		DATA_MEMCOPY(sTmp, pInput, WGHN_AVRG_STCK_PRC  );    // [numstring, 20.2] ���� ��� �ֽ� ����
		DATA_MEMCOPY(sTmp, pInput, HTS_AVLS            );    // [numstring,   19] HTS �ð��Ѿ�
		DATA_MEMCOPY(sTmp, pInput, HOUR_CLS_CODE       );    // [string,    1] �ð� ���� �ڵ�
		DATA_MEMCOPY(sTmp, pInput, CTTR                );    // [numstring, 12.2] ü�ᰭ��
		DATA_MEMCOPY(sTmp, pInput, STCK_PRDY_CLPR      );    // [numstring,   11] �ֽ� ���� ����
		DATA_MEMCOPY(sTmp, pInput, NEW_MKOP_CLS_CODE   );    // [string,    2] �� �� ���� �ڵ�
		DATA_MEMCOPY(sTmp, pInput, SELN_CNTG_QTY       );    // [numstring,   19] �ŵ� ü�� ����
		DATA_MEMCOPY(sTmp, pInput, SHNU_CNTG_QTY       );    // [numstring,   19] �ż�2 ü�� ����
		DATA_MEMCOPY(sTmp, pInput, NTBY_CNQN           );    // [numstring,   19] ���ż� ü�ᷮ
	}
};

#pragma pack( pop )

#endif // AFX_OHHTS_STCK_CNTG__H_318EC91F_0936_47a9_88B8_1911DEA0A108_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
