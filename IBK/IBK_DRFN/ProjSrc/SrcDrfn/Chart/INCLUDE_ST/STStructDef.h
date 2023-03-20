#ifndef __STRATEGY__STRUCT__DEFINE__BYALZIO__060914__HEADER__
#define __STRATEGY__STRUCT__DEFINE__BYALZIO__060914__HEADER__

// #include "../Include_ST/STStructDef.h"
typedef struct 
{
	HWND	hSystemTabEx;	// TabSystemTradingEx�� ������ �ڵ�
	HWND	hSTAddin;		// ChartSTAddin�� SiteCustumAddin�� �������ڵ�
} stRunWndInfo;


typedef struct
{
	CString szSTFile;
	long	pChartInterface;
	CString szCode;
	int		nCount;
	CString szEndData;
	CStringArray stArrayPath;
	CString szSTName;		//������ : KHD : 2006.11.16

	stRunWndInfo wndInfo;
	char szCodeName[30+1];
	int  nCodeType;			//CTabSystemTradingEx::CSTAnalysisInput::GetCodeType()���� �Ѱܹ޴� ��.
	// 0:�ֽ�, 1:����, 2:����
} stAddinSTRun;


// �������� ������ Ÿ��
// ���յ� ���·� ����� ���� ������, ��Ƽ��Ʈ�� �ý����ǿ� ���ǵ� ���·� ó���Ѵ�.
// �� (�ý�������)������ǥ �Ǵ� �������ǥ (STRun_Type_Index)
//	  �����ϴ� �����༼ �Ǵ� ����ڰ����༼(STRun_Type_BullBear)
//	  �׿� ST ����
enum
{
	STRun_Type_None=0,	// None
	STRun_Type_Index=1,		// ��ǥ
	STRun_Type_ST=2,		// ����
	STRun_Type_BullBear=4,	// ����,�༼
};

#define	Version_stAddinSTRunInfo		070213
// ChartSTIndexAddin (11) �� ���ؼ� �������� ������ Ȯ���ϰ��� �Ҷ�.
// ChartSTIndexAddin�� ���ؼ� ������ ��쿡�� ��Ƽ��Ʈ���� ���õ� item�� �ش��ϴ� ������ ������ �� ����.
// ���� �����߿� �������� ������ Ȯ���Ϸ��� PcTr�� ���ؼ� �����ؾ���.
typedef struct 
{
	// Input
	LPCSTR szSTFile;		// ���� FullPath��.

	// Output
	BOOL	bIsRun;			// ������ ���࿩��
	CString szRunSTFile;	// �������� �����н�(SD)

	int		nSTType;
	CString szSTName;		// ������

	int		nRunSTType;		// ChartSTIndexAddin���� ����� �Ѱ��ִ� ����.

} stAddinSTRunInfo;

#endif //__STRATEGY__STRUCT__DEFINE__BYALZIO__060914__HEADER__