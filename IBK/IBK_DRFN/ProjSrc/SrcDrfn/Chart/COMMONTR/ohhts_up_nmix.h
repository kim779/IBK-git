#if !defined(AFX_OHHTS_UP_NMIX__H_F1246A59_E7DD_4ee8_A803_AF42DCD8A583_INCLUDED_)
#define AFX_OHHTS_UP_NMIX__H_F1246A59_E7DD_4ee8_A803_AF42DCD8A583_INCLUDED_

#include "Data_Common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// �������� (  )
#pragma pack( push, 1 )

#define NAME_ohhts_up_nmix     "ohhts_up_nmix"

// 
typedef struct _ohhts_up_nmix_InBlock1
{
    char    BSTP_CLS_CODE       [   4];    // [string,    4] ���� ���� �ڵ�
} ohhts_up_nmix_InBlock1, *LPohhts_up_nmix_InBlock1;
#define NAME_ohhts_up_nmix_InBlock1     "InBlock1"

// 
typedef struct _ohhts_up_nmix_OutBlock1
{
    char    BSTP_CLS_CODE       [   4];    // [string,    4] ���� ���� �ڵ�
    char    PRPR_HOUR           [   6];    // [string,    6] ���簡 �ð�
    char    BSTP_NMIX_PRPR      [  12];    // [numstring, 12.2] ���� ���� ���簡
    char    DATA_RANK           [  11];    // [numstring,   11] ������ ����
    char    PRDY_VRSS_SIGN      [   1];    // [string,    1] ���� ��� ��ȣ
    char    BSTP_NMIX_PRDY_VRSS [  12];    // [numstring, 12.2] ���� ���� ���� ���
    char    ACML_VOL            [  19];    // [numstring,   19] ���� �ŷ���
    char    ACML_TR_PBMN        [  19];    // [numstring,   19] ���� �ŷ� ���
    char    BSTP_NMIX_HGPR      [  12];    // [numstring, 12.2] ���� ���� �ְ�
    char    BSTP_NMIX_LWPR      [  12];    // [numstring, 12.2] ���� ���� ������
    char    CNTG_VOL            [  19];    // [numstring,   19] ü�� �ŷ���
    char    BSTP_NMIX_OPRC      [  12];    // [numstring, 12.2] ���� ���� �ð�2
    char    BSTP_NMIX_PRDY_CTRT [   9];    // [numstring,  9.2] ���� ���� ���� �����
    char    ASCN_ISSU_CNT       [   8];    // [numstring,    8] ��� ���� ��
    char    UPLM_ISSU_CNT       [   8];    // [numstring,    8] ���� ���� ��
    char    DOWN_ISSU_CNT       [   8];    // [numstring,    8] �϶� ���� ��
    char    LSLM_ISSU_CNT       [   8];    // [numstring,    8] ���� ���� ��
    char    STNR_ISSU_CNT       [   8];    // [numstring,    8] ���� ���� ��
    char    ASCN_ISSU_RATE      [   9];    // [numstring,  9.4] ��� ���� ����
    char    FRGN_NTBY_QTY       [  13];    // [numstring,   13] �ܱ��� ���ż� ����
    char    ORGN_NTBY_QTY       [  19];    // [numstring,   19] ����� ���ż� ����
} ohhts_up_nmix_OutBlock1, *LPohhts_up_nmix_OutBlock1;
#define NAME_ohhts_up_nmix_OutBlock1     "OutBlock1"

class CREAL_UPJONG
{
public: //Variable
    CString m_szBSTP_CLS_CODE       ;    // [string,    4] ���� ���� �ڵ�
    CString m_szPRPR_HOUR           ;    // [string,    6] ���簡 �ð�
    CString m_szBSTP_NMIX_PRPR      ;    // [numstring, 12.2] ���� ���� ���簡
    CString m_szDATA_RANK           ;    // [numstring,   11] ������ ����
    CString m_szPRDY_VRSS_SIGN      ;    // [string,    1] ���� ��� ��ȣ
    CString m_szBSTP_NMIX_PRDY_VRSS ;    // [numstring, 12.2] ���� ���� ���� ���
    CString m_szACML_VOL            ;    // [numstring,   19] ���� �ŷ���
    CString m_szACML_TR_PBMN        ;    // [numstring,   19] ���� �ŷ� ���
    CString m_szBSTP_NMIX_HGPR      ;    // [numstring, 12.2] ���� ���� �ְ�
    CString m_szBSTP_NMIX_LWPR      ;    // [numstring, 12.2] ���� ���� ������
    CString m_szCNTG_VOL            ;    // [numstring,   19] ü�� �ŷ���
    CString m_szBSTP_NMIX_OPRC      ;    // [numstring, 12.2] ���� ���� �ð�2
    CString m_szBSTP_NMIX_PRDY_CTRT ;    // [numstring,  9.2] ���� ���� ���� �����
    CString m_szASCN_ISSU_CNT       ;    // [numstring,    8] ��� ���� ��
    CString m_szUPLM_ISSU_CNT       ;    // [numstring,    8] ���� ���� ��
    CString m_szDOWN_ISSU_CNT       ;    // [numstring,    8] �϶� ���� ��
    CString m_szLSLM_ISSU_CNT       ;    // [numstring,    8] ���� ���� ��
    CString m_szSTNR_ISSU_CNT       ;    // [numstring,    8] ���� ���� ��
    CString m_szASCN_ISSU_RATE      ;    // [numstring,  9.4] ��� ���� ����
    CString m_szFRGN_NTBY_QTY       ;    // [numstring,   13] �ܱ��� ���ż� ����
    CString m_szORGN_NTBY_QTY       ;    // [numstring,   19] ����� ���ż� ����

public:
	CREAL_UPJONG()
	{
	}

	virtual void  Convert(ohhts_up_nmix_OutBlock1* pInput)
	{
		CString sTemp1 ;
		char sTmp[50+1];

		DATA_MEMCOPY(sTmp, pInput, BSTP_CLS_CODE       );    // [string,    4] ���� ���� �ڵ�
		DATA_MEMCOPY(sTmp, pInput, PRPR_HOUR           );    // [string,    6] ���簡 �ð�
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_PRPR      );    // [numstring, 12.2] ���� ���� ���簡
		DATA_MEMCOPY(sTmp, pInput, DATA_RANK           );    // [numstring,   11] ������ ����
		DATA_MEMCOPY(sTmp, pInput, PRDY_VRSS_SIGN      );    // [string,    1] ���� ��� ��ȣ
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_PRDY_VRSS );    // [numstring, 12.2] ���� ���� ���� ���
		DATA_MEMCOPY(sTmp, pInput, ACML_VOL            );    // [numstring,   19] ���� �ŷ���
		DATA_MEMCOPY(sTmp, pInput, ACML_TR_PBMN        );    // [numstring,   19] ���� �ŷ� ���
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_HGPR      );    // [numstring, 12.2] ���� ���� �ְ�
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_LWPR      );    // [numstring, 12.2] ���� ���� ������
		DATA_MEMCOPY(sTmp, pInput, CNTG_VOL            );    // [numstring,   19] ü�� �ŷ���
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_OPRC      );    // [numstring, 12.2] ���� ���� �ð�2
		DATA_MEMCOPY(sTmp, pInput, BSTP_NMIX_PRDY_CTRT );    // [numstring,  9.2] ���� ���� ���� �����
		DATA_MEMCOPY(sTmp, pInput, ASCN_ISSU_CNT       );    // [numstring,    8] ��� ���� ��
		DATA_MEMCOPY(sTmp, pInput, UPLM_ISSU_CNT       );    // [numstring,    8] ���� ���� ��
		DATA_MEMCOPY(sTmp, pInput, DOWN_ISSU_CNT       );    // [numstring,    8] �϶� ���� ��
		DATA_MEMCOPY(sTmp, pInput, LSLM_ISSU_CNT       );    // [numstring,    8] ���� ���� ��
		DATA_MEMCOPY(sTmp, pInput, STNR_ISSU_CNT       );    // [numstring,    8] ���� ���� ��
		DATA_MEMCOPY(sTmp, pInput, ASCN_ISSU_RATE      );    // [numstring,  9.4] ��� ���� ����
		DATA_MEMCOPY(sTmp, pInput, FRGN_NTBY_QTY       );    // [numstring,   13] �ܱ��� ���ż� ����
		DATA_MEMCOPY(sTmp, pInput, ORGN_NTBY_QTY       );    // [numstring,   19] ����� ���ż� ����
	}
};

#pragma pack( pop )

#endif // AFX_OHHTS_UP_NMIX__H_F1246A59_E7DD_4ee8_A803_AF42DCD8A583_INCLUDED_

///////////////////////////////////////////////////////////////////////////////////////////////////
