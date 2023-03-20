// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2001
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information.
//
//  Components   : interStruct.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2002-10	Initial version
// *****************************************************************************
//
// 
//
#pragma once
#pragma pack(1)
//
//	user/userName����
//	file name : portfolio.i00 ~ portfolio.i99
//

struct _inters
{
	char	gubn{};		// ���񱸺�
				// 0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12]{};	// �����ڵ�
	char	name[30]{};	// �����
	int	possNum{};	// ��������
	double	possPrice{};	// �����ܰ�
	char	memo[30]{};	// �޸�
	char	o_check{};	// �ֹ� ���࿩��
				// 0:none, 1:checked
	char	o_kind{};		// �ֹ�����
				// 1:�ŵ�, 2:�ż�
	char	o_mmgb{};		// �Ÿű���
				// 1:����, 2:���尡
	int	o_num{};		// �ֹ�����
	double	o_price{};	// �ֹ��ܰ�
	char	a_kind{};		// �˶� ����
				// 1:�׸���, 2:�׸��� �Ҹ�
	int	a_num{};		// ���� �׸� ����
	char	a_condition{};	// �˶����� 1:OR, 2:AND
	char	filler[99]{};	// Reserved
};

struct _alarms
{
	char	a_kind{};		// �����׸�
				// 0:none, 1:���簡, 2:�ŵ�ȣ��, 3:�ż�ȣ��, 4:�����, 5:������, 6:���ͷ�
	double	a_value{};	// ���� �񱳰�
	char	a_condition{};	// ���� ����
				// 1:>=, 2:>, 3:=, 4:<, 5:<=
	char	filler[40]{};	// Reserved
};

/////////////////////// �������� ///////////////////////
#define INTER_UP	1
#define INTER_DOWN	2
#define INTER_RANK	3
#define INTER_GUBUN	"MY"
#define GUBUN_LEN	2

struct  usrinfo {
       char    gubn[2]{};                /* ���� ����                    */
                                       /* MY: ���� ����                */
                                       /* UL: ����� ����Ÿ ���       */
                                       /* UD: ����� ����Ÿ            */
       char    dirt[1]{};                /* ���� ����                    */
                                       /* U: PC=>HOST  D: HOST=>PC     */
       char    cont[1]{};                /* ���� ����                    */
                                       /* F: First     M: Middle       */
                                       /* L: Last      C: Cancel       */
       char    name[80]{};               /* file name                    */
       char    nblc[5]{};                /* �� ����                    */
                                       /* ȭ�� SIZE (max. 64K)         */
       char    retc[1]{};                /* O: ����      E: ����         */
       char    emsg[40]{};               /* error message                */
};

#define INTER_TRCODE	"chhomyst"
#define MAX_GR	100			/* �ִ� ���ɱ׷� ����		*/
#define MAX_JM  120                     /* �ִ� �������� ����           */

struct jlist {				/* ��������                     */
        char    code[12]{};               /* �����ڵ�                     */
                                        /* ����(6), ����/�ɼ�(8)        */
                                        /* �����ϵ��ݵ�(9)              */
                                        /* �����ڵ�(5)                  */
                                        /* �������� �ش�(���� + ����) */
        char    sqty[9]{};                /* �����ֽļ���                 */
        char    msga[9]{};                /* �ż�����                     */
};

struct  fold {				/* ��������                     */
        char    fcod[2]{};                /* ���ɱ׷��ڵ� (00 ~ 99)       */
        char    name[20]{};               /* ���ɱ׷��                   */
        char    nrec[4]{};                /* ���񰹼�(space ����)         */
        struct  jlist   jlist[MAX_JM]{};
};

struct  _usrmid     {
	struct  usrinfo usrinfo{};
	struct  fold    fold{};	
};

struct  _usrmod     {
        struct  usrinfo usrinfo{};
        struct  fold    fold{};
};

////// �������
#define RANK_TRCODE	"chbojrnk"

struct  rank_mid     {
        char    gubn[2]{};                /* ��ȸ����                     */
        char    mark[1]{};                /* ���屸��                     */
                                        /* 1 : KOSPI,   2 : KOSDAQ      */
                                        /* 3 : ��3����                  */
};

struct  rank_grid    {
        char    code[6]{};                /* �����ڵ�                     */
        char    hnam[16]{};               /* �����                       */
};

struct  rank_mod     {
        char    nrec[4]{};                /* # of records                 */
        struct  rank_grid    grid[1]{};
};
#pragma pack()