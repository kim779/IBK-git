#pragma once
  
#include <afxtempl.h>

// swap ��� ����..

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define COR_BLUE		RGB(0, 0, 255)
#define COR_RED			RGB(255, 0, 0)
#define COR_BLACK		RGB(0, 0, 0)
#define COR_WHITE		RGB(255, 255, 255)
#define USER_BACKCOLOR		RGB(222,219,222)
#define USER_BACKCOLOR2		RGB(208,208,208)
#define USER_BT_NORMALCOR	RGB(210, 210, 210)
#define USER_FOCUS_COR		RGB(199, 203, 221)
#define COR_BACK		RGB(247,247,255)
#define COR_3DTOP		RGB(128,128,128)
#define COR_3DBOTTOM		RGB(221,221,209)


//
//	user/userName����
//	file name : portfolio.i00 ~ portfolio.i99
//
// char gName[20];		// �׷��̸�

struct _inters
{
	char	gubn;		// ���񱸺�
				// 0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];	// �����ڵ�
	char	name[30];	// �����
	int	possNum;	// ��������
	double	possPrice;	// �����ܰ�
	char	memo[30];	// �޸�
	char	o_check;	// �ֹ� ���࿩��
				// 0:none, 1:checked
	char	o_kind;		// �ֹ�����
				// 1:�ŵ�, 2:�ż�
	char	o_mmgb;		// �Ÿű���
				// 1:����, 2:���尡
	int	o_num;		// �ֹ�����
	double	o_price;	// �ֹ��ܰ�
	char	a_kind;		// �˶� ����
				// 1:�׸���, 2:�׸��� �Ҹ�
	int	a_num;		// ���� �׸� ����
	
	char	filler[100];	// Reserved
};

struct _alarms
{
	char	a_kind;		// �����׸�
				// 0:none, 1:���簡, 2:�ŵ�ȣ��, 3:�ż�ȣ��, 4:�����, 5:������, 6:���ͷ�
	double	a_value;	// ���� �񱳰�
	char	a_condition;	// ���� ����
				// 1:>=, 2:>, 3:=, 4:<, 5:<=
	char	filler[40];	// Reserved
};

typedef struct _listItem { 

	char*	Code[10];
	char*	Name[20];

}LISTITEM;

typedef struct PCode
{
	CString kind;
	CString name;
	CString code;
	CString hnam;
	CString month;
	CString price;
	BOOL	atm;
	char	mchk;
}PCODE;

typedef struct FCode
{
	CString code;
	CString name;
	char	mchk;
}FCODE;
