// AxisCode.h : main header file for the AXISCODE DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/jmcode.h"
#include <afxtempl.h>

//using HCODE  = struct hjcode;
using HCODEX = struct hjcodex;

typedef struct listitem
{
	CString code;
	CString name;
	CString date;
}ITEMS;

typedef struct _elwcode
{
	CString		code;	// ELW �����ڵ�
	CString		name;	// �����
	CString		ktype;	// �Ǹ����� 01:Call, 02:Put, 03:��Ÿ
	CString		htype;	// �Ǹ������ 01:������, 02:�̱���, 03:��Ÿ
	CString		pcode;	// ����� �ڵ�
	CString		pname;	// ������
	CString		mjmt;	// ����� (YYYYMMDD)
	CString		lpnum[5];	// LP ȸ����ȣ (3�ھ� 5��)
	CString		bcode[5];	// �����ڻ� �ڵ�� "�����ڻ�1;�����ڻ�2;�����ڻ�3;.."
	BOOL		bkoba{};		// ��������ELW����
}ELWCODE;

typedef struct _pbcode
{
	CString		code;	// ����� �ڵ�
	CString		name;	// ������
}PBCODE;

typedef struct _basecode
{
	CString		code;	// �����ڻ� �ڵ�
	CString		name;	// �����ڻ��

	_basecode(CString scode = "", CString sname = "")
	{
		code = scode;
		name = sname;
	}

}BASECODE;

#define ASCENDING	0
#define	DESCENDING	1

typedef struct _sortStr
{
	CString code;
	CString name;
}SORTSTR;

struct _efopcode
{
	char	type[1]{};			/*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char	code[6]{};			/*�����ڵ�*/
	char	hnam[39]{};			/*�����*/
	char	opcd[6]{};			/*���� �ڵ�*/
	char	etfm[1]{};			/*�׸��ڵ�*/
	char	etfl[1]{};			/*������ ����*/
};

struct _efopitem
{
	char	type[1]{};			/*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char	opcd[6]{};			/*���� �ڵ�*/
	char	hnam[30]{};			/*���� �ѱ۾��*/
	char	lnfd[1]{};			/*�����ǵ�*/
	
	//	_efopcode *efopcode;
};


struct _efoptema
{
	char	type[1]{};			/*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char	tmcd[1]{};			/*�׸��ڵ�*/
	char	tmnm[20]{};			/*�׸���*/
	char	lnfd[1]{};			/*�����ǵ�*/
};

struct _efopfore
{
	char	type[1]{};			/*������ Ÿ��*/
	/*'1':���纰*/
	/*'2':�׸���*/
	/*'3':������ ���к�*/
	/*'4':����*/
	char	frcd[1]{};			/*������ ����('D':����, 'F':�ؿ�)*/
	char	frnm[10]{};			/*������ ���и�*/
	char	lnfd[1]{};			/*�����ǵ�*/
};
/////////////////////////////////////////////////////////////////////////////
// CAxisCodeApp
// See AxisCode.cpp for the implementation of this class
//

class CAxisCodeApp : public CWinApp
{
public:
	BOOL	LoadCode();
	BOOL	LoadETFCode();
	BOOL    loadCJCode();
	int	GetVirtualScreenX();

public:
	CString m_root;
	CString m_folder;
//	std::vector<HCODE*>  _vETN;
	std::vector<HCODEX*> _vETNx;
	std::vector<std::tuple<char, CString, CString>>	_vETFitem;
	std::vector<std::tuple<char, char, CString>>	_vETFtema;
	std::vector<std::tuple<char, char, CString>>	_vETFfore;
	std::vector<std::tuple<char, CString, CString, CString, char, char>>	_vETFcode;
//	std::map<CString, HCODE*>			      _mapCODE;
	std::map<CString, HCODEX*>			      _mapCODEx;
	std::map<CString, std::pair<CString, int>>	      _mapACODE;
	std::map<CString, CString>			      _mapNAME;
	std::vector<std::tuple<CString, CString, char, char>> _vBond;

private:
	std::unique_ptr<char[]> m_hcB;
	
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisCodeApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxisCodeApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


