 //*******************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2003
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : appl.h	application common header
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2003-05	Initial version
//		 02.00  2008-02 Upgrade version (for EU2S)
//		 02.10	2009-06 Customize version (for KATS)
//*****************************************************************************
#ifndef	_APPLCOMMON
#define	_APPLCOMMON

//
// confirm dialog : type option
//
#define	CFD_EDIT	0x00000001		// edit enable(1���� ���)
#define	CFD_FUND	0x00000002		// fund �ֹ�

#define	CFD_ONE		0x00000010		// 1�� �ֹ�
#define	CFD_TWO		0x00000020		// 2�� �ֹ�
#define	CFD_THREE	0x00000040		// 3�� �ֹ�
#define	CFD_OTHER	0x00000080		// 4�� �̻�
#define	CFD_FOUR	0x00000100		// 4�� �ֹ�
#define	CFD_HTWO	0x00000200		// 4�� �ֹ�

#define	CFD_LTPOS	0x000001000		// left top
#define	CFD_MTPOS	0x000002000		// middle top
#define	CFD_RTPOS	0x000004000		// right top
#define	CFD_LBPOS	0x000008000		// left bottom
#define	CFD_MBPOS	0x000010000		// middle bottom
#define	CFD_RBPOS	0x000020000		// right bottom
#define	CFD_WCPOS	0x000040000		// window center
#define	CFD_SCPOS	0x000080000		// screen center
#define	CFD_USPOS	0x000100000		// user define pos : CRect(x, y, 0, 0)

//
// childframe message (to m_parent->GetParent())
//
#define	WM_AXCHILD	(WM_USER + 500)

//
// message : WM_AXCHILD, LOWORD(wparam)
//
#define	apROOTDIR	0x51		// char* return : root 
#define	apLOGINID	0x52		// char* return : loginID
#define	apLOGINIDx	0x53		// char* return : loginID|desc
#define	apGUIDE1	0x54		// lparam : message
#define	apGUIDE2	0x55		// lparam : message, HIWORD(wparam) : level(1, 2, 3)
					//                                    black, red, blue
#define	apGUIDE3	0x56		// lparam : message, HIWORD(wparam) : level(1, 2, 3)
#define	apFIXEDA	0x57		// lparam : LOWORD(lparam) = width, HIWORD(lparam) = height
#define	apFIXEDW	0x58
#define	apFIXEDH	0x59
#define	apHOOKED	0x60		// HIWORD(wparam) 1: hooked

#define apPINCH		0x61		// 
#define	apSETCONFIG	0x62		//
#define apSETTG		0x63

//////////////////////////////////////////////////////////////////////////
/*	
apCODECANCEL	���� ��ü ���	
apBECANCEL	�����ֹ� ���
apBEEDIT	�����ֹ� ����
apSELORDER	�ŵ��ֹ�
apBUYORDER	�ż��ֹ�
apALLORDER	û���ֹ�
apAUTOSORT	��Ŭ�� �ڵ� ����
apAUTOSTOP	�ڵ��ֹ� STOP
apA3MANUAL	�ڵ��ֹ�3 �����ʱ�ȭ
*/
//////////////////////////////////////////////////////////////////////////
#define apCODECANCEL	0x71		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = 0
#define apBECANCEL	0x72		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = 0
#define apBEEDIT	0x73		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = tick
#define apSELORDER	0x74		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = tick
#define apBUYORDER	0x75		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = tick
#define apALLORDER	0x76		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = tick
#define apAUTOSORT	0x77		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = 0
#define apAUTOSTOP	0x78		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = 0
#define apA3MANUAL	0x79		// lparam : LOWORD(lparam) = messange, HIWORD(lparam) = 0

// ȯ�漳�� INI
#define	CONFIGINI	"config.ini"	// config file
#define	ORDER		"ORDER"		// SECTION:�ֹ�
#define	CONFIRM		"CONFIRM"	// key �ֹ�Ȯ��â 1:TRUE, 0:FALSE
#define	PRICE		"PRICE"		// key 0:���ȣ��, 1:���簡, 2:������
#define	DISPLAY		"DISPLAY"	// SECTION
#define	FLASH		"FLASH"		// key Blink��뿩�� 1:TREU, 0:FALSE
//
// �ֹ� TRCODE
//
#define	FJUMUNTR	"pibofjum"		// ����/�ɼ� �ֹ� TR
#define	FFNDJUMUNTR	"piboffnd"		// ����/�ɼ� �ݵ� �ֹ� TR
#define	HJUMUNTR	"pibohjum"		// �����ֹ�TR
#define	PIBOJQTY	"pibojqty"		// �ֹ����ɼ���  (prev : pibo1675)
#define	HO711001	"piboacct"		// ���¼���	 (prev : ho711001)

#define	MAX_JROW	130
#define MAX_HROW	300

#define ACC_LEN		11			// ���� �ڸ���(����,����)


#define F_JOLEN		7			// ���� �ֹ� �ڸ���
#define F_FOLEN		7			// ���� �ݵ� �ֹ� �ڸ���
//
// �����ֹ�
//
struct	fojum	{
	char	jmid[6];		// �ֹ� ID		right align, zerofill
	char	jmgb[1];		// �ֹ�����
					// '1' : �ŵ�
					// '2' : �ż�
					// '3' : �ŵ�����
					// '4' : �ż�����
					// '5' : �ŵ����
					// '6' : �ż����
	char	accn[ACC_LEN];		// ���¹�ȣ
	char	pswd[8];		// ��й�ȣ		left align, space fill
	char	codx[15];		// �����ڵ�		left align, space fill
	char	giho[1];		// ���ݱ�ȣ		���� >= 0 : '+', ���� < 0 : '-'
					// '+', '-'
	char	jprc[9];		// �ֹ��ܰ� 9(7)V99	right align, zerofill(000002345)
					// �Ҽ���ǥ�� ����
	char	jqty[8];		// �ֹ�����		right align, zerofill
	char	ojno[F_JOLEN];		// ���ֹ���ȣ
	char	jgub[1];		// �ֹ�����
					// '1' : ������
					// '2' : ���Ǻ�������
					// '3' : ���尡					
					// '4' : ������������
					// '5' : IOC					
					// '6' : FOK
	char	jrgb[1];		// ��������					
					// '1' : ����
					// '2' : ����
					// '3' : ��Ÿ, �Ϲ�
	char	usid[8];		// user id
	char	fdid[2];		// fund id
	char	bjgb[1];		// �ֹ�����
					// '1' : �ٽ���
					// '2' : �����ڵ��ֹ�
					// '3' : �Ϲ��ֹ�
					// '4' : �ݵ��ֹ�
					// '5' : �ý����ֹ�
					// '7' :�����ֹ�  
					// '9' : �������ڵ�1~2
					// 'A' : �������ڵ�3
					// 'B' : �������ڵ�1
	char	bseq[4];		// �ٽ��� seqn
	char	fjno[F_FOLEN];		// �ݵ� �ֹ� ��ȣ
	char    fdix[1];                // �ݵ� index
        char    curr[9];                // ���簡
	char	ptsq[3]; 		/* CD �ֹ�Ƚ��		*/
        char    fill[17];               // filler
};
#define	sz_FOJUM	sizeof(struct fojum)


struct	fojno	{
	char	jmid[6];		// �ֹ�ID
	char	accn[ACC_LEN];		// ���¹�ȣ
	char	codx[15];		// �����ڵ�
	char	juno[F_JOLEN];		// �ֹ���ȣ
	char	ecod[4];		// error code
	char	epos[2];		// error position
	char	emsg[80];		// error message
	char	fjno[F_FOLEN];		/* �ݵ� �ֹ� ��ȣ	*/
	char	fill[13];		// filler
};
#define	sz_FOJNO	sizeof(struct fojno)

struct	fmid	{
	char	nrec[4];			// �ֹ�����
	struct	fojum	fojum[MAX_JROW];	// �ֹ�
};

struct	fmod	{
	char	nrec[4];			// �ֹ�����
	struct	fojno	fojno[MAX_JROW];	// �ֹ�����
};


#define H_JOLEN		7			// ���� �ֹ� �ڸ���
#define H_FOLEN		7			// ���� �ݵ� �ֹ� �ڸ���
//
// �����ֹ�
//
struct	hjum	{
	char	jmid[4];		// �ֹ� ID
	char	jmgb[1];		// �ֹ�����
					// '1' : �ŵ�
					// '2' : �ż�
					// '3' : ����
					// '4' : ���
	char	accn[ACC_LEN];		// ���¹�ȣ
	char	pswd[8];		// ��й�ȣ
	char	codx[12];		// �����ڵ�
	char	jprc[9];		// �ֹ��ܰ�	zerofill
	char	jqty[9];		// �ֹ�����	zerofill
	char	ojno[H_JOLEN];		// ���ֹ���ȣ
	char	jgub[2];		// �ֹ�����
					// "00" : ������  
					// "03" : ���尡  
					// "05" : ���Ǻ�������  
					// "06" : ������������  
					// "07" : �ֿ켱������  
					// "91" : ���� �ð���  
					// "95" : �ð��ܴ��ϰ�  
					// "99" : �帶���ð������� 
	char	ogub[1];		// �ֹ� ����   
					// '0' ���� '1'.IOC , '2' FOK  
	char	bsno[2];		// �ٽ��Ϲ�ȣ
	char	mdgb[1];		// �ŵ�����
					// '0' : �Ϲݸŵ�
					// '1' : ���ŵ�
	char	gong[1];		// ���ŵ�ȣ������
					// '0' : �Ϲݸŵ�
					// '1' : �������� �ִ� ���ŵ�=>�����ֽĸŵ� 2005.07.06 ����
					// '2' : �������� ���� ���ŵ�=>��Ÿ���ŵ� 2005.07.06 ����
	char	phgb[2];		// ���α׷� ȣ������
					// '00' : �Ϲ��ֹ�
					// '01' : ���Ͱŷ�
					// '02' : �����Ͱŷ�
	char	usid[8];		// user id
	char	fdid[2];		// fund id
	char	bjgb[1];		// �ֹ�����
					// '1' : �ٽ���
					// '2' : CD�ֹ�
					// '3' : �Ϲ��ֹ�
					// '4' : �ݵ��ֹ�
					// '5' : �ý����ֹ�
					// '7' :�����ֹ�  
					// '9' : �������ڵ�1~2
					// 'A' : �������ڵ�3
					// 'B' : �������ڵ�1
	char	bseq[4];		// �ٽ��� seqn
	char	tcod[12];		// ��ڵ� ('0')
	char	fjno[H_FOLEN];		/* �ݵ� �ֹ���ȣ		*/
	char	fdix[2];		 /* �ݵ� index   */
	char	curr[9];		 /* ���簡   */
	char	ptsq[3]; 		/* ���߼��� /CD �ֹ�Ƚ��	*/
	char	fill[7];		// filler
};
#define	sz_HJUM		sizeof(struct hjum)

struct	hjno	{
	char	jmid[4];		// �ֹ�ID
	char	accn[ACC_LEN];		// ���¹�ȣ
	char	codx[12];		// �����ڵ�
	char	juno[H_JOLEN];		// �ֹ���ȣ
	char	ecod[4];		// error code
	char	epos[2];		// error position
	char	emsg[80];		// error message
	char	fjno[H_FOLEN];		/* �ݵ� �ֹ���ȣ		*/
	char	fill[13];		// filler
};
#define	sz_HJNO		sizeof(struct hjno)

struct	hmid	{
	char	nrec[4];		// �ֹ�����
	struct	hjum	hjum[MAX_HROW];	// �ֹ�
};

struct	hmod	{
	char	nrec[4];		// �ֹ�����
	struct	hjno	hjno[MAX_HROW];	// �ֹ�����
};

//
// �ֹ����ɼ���
//
// �ִ� 20�� ��ȸ�����մϴ�.
// 20�� �̻��Ͻÿ��� ����ȸ �ϼž� �մϴ�.
struct  jqtyQry	{			// prev : pibo1657itr
        char    usid[8];                // ����� ���̵�
        char    code[8];                // �����ڵ�     
        char    jmgb[1];                // �ֹ��������� 
                                        // L:������     
                                        // M.���尡     
                                        // C.���Ǻ�������
                                        // B.������������
        char    jprc[9];                // �ֹ�����      
        char    nrec[3];
};
#define	sz_JQTYQRY	sizeof(struct jqtyQry)
struct  jqtyQrymid {
        char    accn[ACC_LEN];               // ���¹�ȣ             
        char    pswd[8];                // �н�����             
};
#define sz_JQTYQRYMID	sizeof(struct jqtyQrymid)

struct  jqtyRsp {			
        char    err[1];                 // prev : pibo1657otr
        char    nrec[3];		// ERR CHK FIELD 0:����  1:����
};
#define	sz_JQTYRSP	sizeof(struct jqtyRsp)
struct  jqtyRspmod {
        char    accn[ACC_LEN];               // ���¹�ȣ             
        char    dqty[10];               // �ŵ����ɼ���         
        char    sqty[10];               // �ż����ɼ���         
};
#define	sz_JQTYRSPMOD	sizeof(struct jqtyRspmod)

//
// ���¼���
//
struct	acctQry	{			// prev : ho711001itr
	char	gubn[1];		// dir
					// 1: ��ȸ
					// 2: ����
	char	accn[ACC_LEN];		// ���¹�ȣ
	char	limt[4];		// �ɼǸŵ��ż��Ѱ�	99V99
	char	jlmt[4];		// �����ŵ��ż��Ѱ�	99V99
	char	jmjc[9];		// �ֹ����� ����	right align, zerofill
};
#define	sz_ACCTQRY	sizeof(struct acctQry)

struct	acctRsp	{			// prev : ho711001otr
	char	err[1];			// ERR CHK FIELD
					// 0:���� 1:���� 
	char	limt[4];		// �ɼǸŵ��ż��Ѱ�	99V99
	char	jlmt[4];		// �����ŵ��ż��Ѱ�	99V99
	char	jmjc[9];		// �ֹ����� ����	right align, zerofill
};
#define	sz_ACCTRSP	sizeof(struct acctRsp)

#endif