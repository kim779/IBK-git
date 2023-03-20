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
#if !defined(INTER_H__58CA3F42_F818_496D_BB4E_8B978E6BE3D6__INCLUDED_)
#define INTER_H__58CA3F42_F818_496D_BB4E_8B978E6BE3D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
//	user/userName����
//	file name : portfolio.i00 ~ portfolio.i99
//

struct _inters
{
	char	gubn[1];	// ���񱸺�	0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];	// �����ڵ�
	char	name[32];	// �����
	char	xprc[10];	// �����ܰ�
	char	xnum[10];	// ��������
	char	xupnum[16];	// ���� �ֽļ�
	char	filler[24];	// Reserved
};

#define	sz_inters	sizeof(struct _inters)

struct _codeinfo
{
	char	code[12];	// �����ڵ�
	char	name[32];	// �����
	char	capture[1];	// ���࿩�� 	'0':������, '1':�����Ϸ�
	
	struct _alarm
	{
		char	kind[1];// �����׸�	0:none, 1:���簡, 2:���ϴ��, 3:�ŵ�ȣ��, 4:�ż�ȣ��, 5:�����, 6:�ŷ���
		char	aval[10];//���ú񱳰�
		char	cond[1];// ��������	'0':�̻�, '1':����
		char	oper[1];// ���ÿ���	'0':AND,  '1':OR
	}alarm[6];

	char	o_run[1];	// �ֹ�â	'0':�̽���,'1':����
	char	o_kind[1];	// �ֹ�����	'0':�ŵ�,  '1':�ż�
	char	o_mmgb[1];	// �Ÿű���	'0':������,'1':���尡
	char	o_num[10];	// �ֹ�����
	char	o_prc[10];	// �ֹ��ܰ�

	char	a_kind[1];	// �˶�����	0x01: Ȯ��â, 0x02: ���������ȣ���� 0x04:�Ҹ�����
	char	a_wave[128];	// �Ҹ�����
	char	filler[20];	// Reserved
};

#define	sz_codeinfo	sizeof(struct _codeinfo)

struct	_hisinfo
{
	char	date[14];	// ��������,�ð�
	struct	_codeinfo	codeinfo;
	char	gval[6][10];	// ������ ����
	char	chk[6];		// check
};
#define	sz_hisinfo	sizeof(struct _hisinfo)

#endif // !defined(INTER_H__58CA3F42_F818_496D_BB4E_8B978E6BE3D6__INCLUDED_)