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
//  Components   : uscode.h
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2002-10	Initial version
// *****************************************************************************
//
// ���� symbol file
//

struct	_uscode	{
	char	code[16];		/*	���� symbol	*/
	char	name[40];		/*	���� ��		*/
	char	exgb[1];		/*	exchange ����	*/
	char	jmgb[1];		/*	���� ����	*/
	char	fill[22];		/*	filler		*/
};

struct	_myUScode	{
	CString		code;		/*	���� symbol	*/
	CString		name;		/*	���� ��		*/
	CString		fill;		/*	filler		*/
	char		exgb;		/*	exchange ����	*/
	char		jmgb;		/*	���� ����	*/
};

//	echange ����	exgb
//	'A'	:	CME
//
//	���� ����	jmgb
//	'F'	:	Future
//	'I'	:	Index
//	'O'	:	Option
//	'C'	:	Combination (No Legs)
//	'L'	:	Combination (Legs)
