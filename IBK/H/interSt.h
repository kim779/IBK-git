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

//
//	user/userName����
//	file name : portfolio.i00 ~ portfolio.i99
//


#pragma pack(1)
constexpr int codelen  = 12;
constexpr int namelen  = 32;
constexpr int pricelen = 10;
constexpr int xnumlen  = 10;

struct _inters
{
	char	gubn[1]{};	// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����	
	char	code[12]{};	// �����ڵ�
	char	name[32]{};	// �����
	char	xprc[10]{};	// �����ܰ�
	char	xnum[10]{};	// ��������
	char	xupnum[16]{};	// ���� �ֽļ�
	char	bookmark[1]{};	// �ϸ�ũ
	char	futureGubn[1]{};	// �ſ�Ÿ� ����
	double  creditPrc{};		// �ſ�ݾ�
	double	maeipPrc{};		// ���Աݾ�
	char	filler[6]{};		// Reserved
};


struct _intersx
{
	char	gubn = '0';		// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����	
	CString code;			// �����ڵ�
	CString name;			// �����
	CString	xprc;			// �����ܰ�
	CString	xnum;			// ��������
	CString	xupnum;			// ���� �ֽļ�
	char	bookmark = '0';		// �ϸ�ũ
	char	futureGubn{};		// �ſ�Ÿ� ����
	double  creditPrc{};		// �ſ�ݾ�
	double	maeipPrc{};		// ���Աݾ�

	void copy(void* item);
	void operator=(struct _intersx& ref)
	{
		code       = ref.code;
		name	   = ref.name;
		gubn       = ref.gubn;
		xprc       = ref.xprc;
		xnum       = ref.xnum;
		xupnum     = ref.xupnum;
		bookmark   = ref.bookmark;
		futureGubn = ref.futureGubn;
		creditPrc  = ref.creditPrc;
		maeipPrc   = ref.maeipPrc;
	}
//#define	sz_intersx	 sizeof(struct _intersx)

	void empty()
	{
		code.Empty();       
		name.Empty();	   
		gubn       = '0';
		xprc.Empty();       
		xnum.Empty();       
		xupnum.Empty();     
		bookmark   = '0';
		futureGubn = '0';
		creditPrc  = 0;
		maeipPrc   = 0;
	}
};

struct	_bookmarkinfo {
	char	gubn[1]{};		// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12]{};		// �����ڵ�
	char	name[32]{};		// �����
	char	bookmark[1]{};		// �ϸ�ũ ����
};
#define	sz_inters	sizeof(struct _inters)
#define	sz_bookmark	sizeof(struct _bookmarkinfo)

struct _codeinfo
{
	char	code[12]{};	// �����ڵ�
	char	name[32]{};	// �����
	char	capture[1]{};	// ���࿩�� 	'0':������, '1':�����Ϸ�
	
	struct _alarm
	{
		char	kind[1]{};// �����׸�	0:none, 1:���簡, 2:���ϴ��, 3:�ŵ�ȣ��, 4:�ż�ȣ��, 5:�����, 6:�ŷ���
		char	aval[10]{};//���ú񱳰�
		char	cond[1]{};// ��������	'0':�̻�, '1':����
		char	oper[1]{};// ���ÿ���	'0':AND,  '1':OR
	}alarm[6]{};

	char	o_run[1]{};	// �ֹ�â	'0':�̽���,'1':����
	char	o_kind[1]{};	// �ֹ�����	'0':�ŵ�,  '1':�ż�
	char	o_mmgb[1]{};	// �Ÿű���	'0':������,'1':���尡
	char	o_num[10]{};	// �ֹ�����
	char	o_prc[10]{};	// �ֹ��ܰ�

	char	a_kind[1]{};	// �˶�����	0x01: Ȯ��â, 0x02: ���������ȣ���� 0x04:�Ҹ�����
	char	a_wave[128]{};	// �Ҹ�����
	char	filler[20]{};	// Reserved
};

#define	sz_codeinfo	sizeof(struct _codeinfo)

struct	_hisinfo
{
	char	date[14]{};	// ��������,�ð�
	struct	_codeinfo	codeinfo{};
	char	gval[6][10]{};	// ������ ����
	char	chk[6]{};		// check
};
#define	sz_hisinfo	sizeof(struct _hisinfo)

#pragma pack()