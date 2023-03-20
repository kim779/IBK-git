#pragma once

#include "afxtempl.h"
#include "../../../h/memDC.h"
#include "../../../h/ledger.h"
#include "../../../h/symbol.h"
#include "../../../h/axisvar.h"
#include "imgBtn.h"

enum { FK_NOT = 0, FK_COMMA, FK_FLOAT, FK_FLOATx, FK_FUTURE, FK_ACCNx, FK_ACCN, FK_TIME, FK_RMVCOMMA, FK_RMVACCN, FK_TRIMLEFT };

void		_SetImgBtn(CWnd *pParent, CString imgDir, CImgBtn *pButton, int nLength, BOOL bImageFit);
void		_SetImgBtnX(CWnd *pParent, CString imgDir, CImgBtn *pButton, CString sImgName, BOOL bImageFit);
void		_SetImgBtnT(CWnd *pParent, CString imgDir, CImgBtn *pButton, CString sImgName, BOOL bImageFit);
CPen*		_GetAxPen(CWnd *pParent, COLORREF clr, int width, int style);
COLORREF	_GetIndexColor(CWnd *pParent, int index);
CFont*		_GetAxFont(CWnd *pParent, CString fName = "����ü", int point = 9, bool bItalic = false, int nBold = 0);
CBitmap*	_GetAxBitmap(CWnd *pParent, CString path);
CString		_Parser(CString &srcstr, CString substr);
CString		_GetAccEdit(CString accn);
CString		_GetRemoveString(CString str);
CString		_GetString(char *pChar, int nMinLen);
CString		_GetStringX(char *pChar, int nMinLen, int digit, bool bColor = false);
double		_Str2Double(CString string, bool bABS = true);
int		_Str2Int(CString string, bool bABS = true);
CString		_GetFormatData(int fkind, CString str);

// macro
#define	LIB_DELETE(XXX) { delete XXX; XXX = NULL; }
#define	BUF_DELETE(XXX)	{ delete[] XXX; XXX = NULL; }

// color
#define	CLRBG			64		// background
#define	CLRGRROWB1		68		// grid row1 back
#define	CLRGRROWB2		77		// grid row2 back
#define	CLRGRROWF		69		// grid row fore
#define	CLRGRROWSF		56		// grid row fore(special)
#define	CLRGRHB			74		// grid head back
#define	CLRGRHSB		0		// grid head select
#define	CLRGRHF			76		// grid head fore
#define	CLRGRHJMGA		163		// grid head �ֹ���
#define	CLRGRHMCHG		164		// grid head ��ü�ᷮ
#define	CLRGRHCHEG		162		// grid head ü�ᷮ
#define	CLRGRSEL		78		// grid select
#define	CLRGRLINE		65		// grid line
#define	CLRREFUSE		39		// �ź�
#define	CLRMDF			57		// �ŵ� font
#define	CLRMSF			56		// �ż� font
#define	CLRMDB			162		// �ŵ� back
#define	CLRMSB			161		// �ż� back
#define	CLRMDT			82		// �ŵ� title
#define	CLRMST			85		// �ż� title
#define	CLRSEL			PALETTERGB(255, 255, 170)	// Y ǥ�� ������ ����
#define	CLRGRROWDISF		PALETTERGB(150, 150, 150)	// ��� ü��� ���

// �ֹ�Ȯ��
#define	CLRCFMTF		PALETTERGB(255, 255, 255)	// title F
#define	CLRCFMJJBG		163				// ��������
#define	CLRCFMJJGHB		163				// ����grid head back
#define	CLRCFMJJGL		208				// ����grid lind
#define	CLRCFMJJGRB		68				// ����grid row back
#define	CLRCFMJJGRS		164				// ����grid row select

#define	CLRCFMCCBG		164				// ��ҹ���
#define	CLRCFMCCGHB		164				// ���grid head back
#define	CLRCFMCCGL		208				// ���grid lind
#define	CLRCFMCCGRB		68				// ���grid row back
#define	CLRCFMCCGRS		164				// ���grid row selec

#define	CLRCFMMDBG		72				// �ŵ�����
#define	CLRCFMMDGHB		72				// �ŵ�grid head back
#define	CLRCFMMDGL		65				// �ŵ�grid lind
#define	CLRCFMMDGRB		68				// �ŵ�grid row back
#define	CLRCFMMDGRS		72				// �ŵ�grid row selec

// Y character
#define	SELCHAR			"V"		// Select ǥ��
#define	MARK_EXPAND		"��"		// Ȯ��
#define	MARK_REDUCE		"��"		// ���
//======================================================================
// grid 
//======================================================================
struct _grInfo {
	char*	heads;
	int	width;
	char*	symbols;
	UINT	format;
	DWORD	attr;
	char*	defaultVal;
	int	param;
};
#define GRIDROW_HEIGHT  18

//======================================================================
// �ֹ� ��� ����
//======================================================================
struct	_jmResult
{
	char    accn[10];			// ���¹�ȣ
	char	anam[40];			// ���¸�
	char	jmno[6];			// �ֹ���ȣ
	char	code[12];			// �����ڵ�
	char	name[32];			// �����
	char	emsg[80];			// �����޼���
	char	ojno[6];			// ���ֹ���ȣ
	char    jqty[8];			// �ֹ�����
	char    jprc[10];			// �ֹ��ܰ� : �����ɼ� (112.13->11213)
	char	mmgb[1];			// �Ÿű��� 1:�ŵ�,2:�ż�
};
#define	sz_jmResult	sizeof(struct _jmResult)

//=================================================================================================
// �׷�����
//=================================================================================================
class 	_acSave	
{
public:
	CString sAccntNum;				//���¹�ȣ
	CString sAccntName;				//�����̸�
public:
	_acSave()
	{
		sAccntNum = _T("");
		sAccntName = _T("");
	};

	_acSave(_acSave const &other)
	{
		Copy(other);
	};

	~_acSave() {}

	inline Copy(_acSave const &other)
	{
		sAccntNum = other.sAccntNum;
		sAccntName = other.sAccntName;
	};
};

//=================================================================================================
class	_gpSave	
{
public:
	CString sGrName;				// �׷��
	CString sAccntCnt;				// ���°���
	CString	sGroupID;				// �׷���̵�
	CArray <_acSave *, _acSave *>	arAcEdit;	// ���� array
public:
	_gpSave()
	{
		sGrName = _T("");
		sAccntCnt = _T("");
		sGroupID = _T("");
		arAcEdit.RemoveAll();
	};

	~_gpSave()
	{
		_RemoveAr();
	};

	inline	void _RemoveAr()
	{
		for (int ii = 0; ii < arAcEdit.GetSize(); ii++)
		{
			_acSave* acSave = arAcEdit.GetAt(ii);
			if (acSave)
			{
				delete acSave;
				acSave = NULL;
			}
		}
		arAcEdit.RemoveAll();
	};
};

