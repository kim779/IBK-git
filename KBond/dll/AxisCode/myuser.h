/*
#include <deque>
#include <stack>

#include <vector>
#include <set>

#include <algorithm>
#include <numeric>
#include <functional>

using namespace std;

typedef deque<int> INTDEQUE;  // ���� �� .. int�� ���� deque�� ���� �ִ�.
*/

// swap ��� ����..

#define SAFE_DELETE(p) if(p) { delete p; p = NULL; }
#define COR_BLUE			RGB(0, 0, 255)
#define COR_RED				RGB(255, 0, 0)
#define COR_BLACK			RGB(0, 0, 0)
#define COR_WHITE			RGB(255, 255, 255)
#define USER_BACKCOLOR		RGB(222,219,222)
#define USER_BACKCOLOR2		RGB(199,206,223)
#define USER_BT_NORMALCOR	RGB(199,206,223)
#define USER_LINECOLOR		RGB(169,169,169)
#define USER_TEXT			RGB(45,54,139)
#define USER_FOCUS_COR		RGB(199, 203, 221)
#define COR_BACK			RGB(247,247,255)
#define COR_3DTOP			RGB(128,128,128)
#define COR_3DBOTTOM		RGB(221,221,209)


#define HJCODE_FILE		"hjcode.dat"
#define ELWCODE_FILE	"elwcod.dat"
#define CJCODE_FILE		"cjcode.dat"
#define SFCODE_FILE		"sfcode.dat"
#define PJCODE_FILE		"pjcode.dat"
#define	USCODE_FILE		"uscode.dat"
#define USLIST_FILE		"USLIST.INI"
#define FDCODE_FILE		"fdcode.dat"


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

	char*	Code[12];     //�����ڵ�
	char*	idct[6];      //��ǥ����
	char*	mtry[6];      //���ⱸ��
	char*	man[10];      //������
	char*	itrt[10];      //�ݸ�
	char*	intertype[6];      //��������
	char*	minfore[10];     //����4
	char*	minthree[10];     //����3
	char*	credit[6];     //�ſ���
	char*	Name[100];    //�����
	char*	valday[10];    //������
}LISTITEM;

typedef struct _listItem2 { 

	CString Code;
	CString	Name;
}LISTITEM2;

typedef struct PCode
{
	CString kind;
	CString name;
	CString code;
	CString hnam;
	CString month;
	CString price;
	CString	gcod;
	BOOL	atm;
	char	mchk;
}PCODE;

typedef struct FCode
{
	CString code;
	CString name;
	CString gnam;
	char	mchk;
}FCODE;

typedef struct  COCode  {                       /* ��ǰ���� �����ڵ� ����       */
        CString    codx;               /* �����ڵ� (12)                */
        CString    hnam;               /* �ѱ۸�                       */
        CString    enam;               /* ������                       */
        char    filler;                 /*                              */
        CString    tjgb;                /* �����ڻ� ID                  */
                                        /* B03 : 3�ⱹä                */
                                        /* B05 : 5�ⱹä                */
                                        /* B10 : 10�ⱹä               */
                                        /* CDR : CD                     */
                                        /* MSB : ���ä                 */
                                        /* USD : USD                    */
                                        /* JPY : JPY                    */
                                        /* EUR : ��������               */
                                        /* GLD : ��                     */
                                        /* PMT : ����                   */
                                        /* MGD : �̴ϱ�                 */
        CString    gcjs;               /* �����ڻ� �����ڵ�(7)         */
        CString    gnam;               /* �����ڻ� �����              */
        char    cmgb;                   /* �ֱٿ��� ����                */
                                        /* 1 : �ֱٿ� 2 : ���ش�        */
        CString    fill;               /* reserved                     */
}COCODE;


typedef struct _CJcode{
	CString code;	// ä�� �ڵ�... 
	CString name;	// ä���̸�...
	CString		kind;  // ä������
	CString     idct;  //��ǥ����
	CString		valday;  //������
	CString		mtday; //������
	CString	    mtry; //���ⱸ��	       
	CString		itrt;  //�ݸ� 
	CString		intertype; //��������
	CString		credit;  //�ſ���
	CString		minthree;  //����3
	CString		minfore;  //����4
	CString		update;  //������Ʈ�ð�
}CJCODESTR;

typedef struct _cjallcode
{
	CString		code;
	char    gubn;	// ä�� ����... 
	CString		name;
	CString		idct;  // ��ǥ  indicator
	CString	        mtry; //����    maturity	       
	CString		mtday; //������
	CString		itrt;  //�ݸ� interest rate
	CString		intertype; //��������
	CString		credit;  //�ſ���
	CString		minthree;  //����3
	CString		minfore;  //����4
	CString		balday;   //������
}CJALLCODE;

typedef struct CUnBcode{
	CString code;	// ���ä�� �ڵ�... 
	char    gubn;	// ���ä�� ����... 
	CString name;	// ���ä���̸�...
}CUNBCODE;

typedef struct Upcode
{
	int	 kind;
	CString	 ucod;
	CString  name;

}UPCODE;


struct  _FDCode {                       // �ݵ��ڵ� ����
	char	gubn[1];		// ����
					// 1:ä���� 2:ä��ȥ����
					// 3:�ֽ��� 4:�ֽ�ȥ����
					// 5:MMF    6:�ؿ��ݵ�
	char	code[5];		// �ݵ��ڵ�(5)
	char	hnam[30];		// �ѱ۸�
};

struct  _USCode {			/* �ؿ� SYMBOL ����             */
        char    symb[16];               /* ���� SYMBOL                  */
        char    datg;                   /* ����Ÿ ����                  */
                                        /* P: �̱�����  Q: �̱�����     */
                                        /* D: ADR       C: ����         */
                                        /* F: CME ����  G: GDR          */
                                        /* X: ȯ��      R: �ݸ�         */
                                        /* L: �����ݸ�  B: �ֿ䱹����ä*/
                                        /* N: �����ݸ�  M: �ݵ�ü       */
                                        /* H: �����ֿ����� E: ECN       */
                                        /* W: �����ְ�����              */
        char    jsgb;                   /* ���� ���� ����               */
                                        /* DOWJONES30 : 0x01            */
                                        /* NASDAQ100  : 0x02            */
                                        /* S&P500     : 0x04            */
        char    knam[48];               /* �ѱ� SYMBOL��                */
        char    enam[48];               /* ���� SYMBOL��                */
        char    ncod[2];                /* �����ڵ�                     */
        char    excd[3];                /* �ŷ����ڵ�                   */
};

struct JCode
{
		CString Code;
		CString Name;

		char	fill;
		char	stgb[2];	// �������� ����
		char	size;		// �ں��ݱԸ�	2:��, 3:��, 4:��
		char	ucdm;		// �����ߺз�
		char	ucds;		// �����Һз�
		char	jjug;		// ������ ����(27: ������)
		char	kpgb;		// KOSPI200 ����
							// 0:������ 1:k200 2:k100 3:k50
		int	kosd;		// ���񱸺�
					// 0:�峻  10:���  30:��3����
		char	ssgb;		// �Ҽӱ���
		char	itgb;		// IT...
		char	wsgb;		// 0:�Ϲ� 1:���豸��������
		char	ucmd;		// 0:������, 5:�켱��
		char	star;		// KOSTAR
		char	unio;		// ��������
		char	jsiz;		// �����ں��� ���߼�

		void*	pInf;		// elwInfo (struct _elwcode* pEcod)
};



typedef struct flist
{
	CString code;
	CString name;
}FLIST;

typedef struct favor
{
	CString fkey;
	CString name;
	int	count;

}FAVOR;

// ELW
typedef struct _elwcode
{
	CString		code;		// ELW �����ڵ�
	CString		name;		// �����
	CString		ktype;		// �Ǹ����� 01:Call, 02:Put, 03:��Ÿ
	CString		pcode;		// ����� �ڵ�
	CString		pname;		// ������
	CString		mjmt;		// ����� (YYYYMMDD)
	CString		bcode[5];	// �����ڻ� �ڵ�� "�����ڻ�1;�����ڻ�2;�����ڻ�3;.."
}ELWCODE;

typedef struct _pbcode
{
	CString		code;		// ����� �ڵ�
	CString		name;		// ������
}PBCODE;

typedef struct _basecode
{
	CString		code;		// �����ڻ� �ڵ�
	CString		name;		// �����ڻ��
}BASECODE;

typedef struct _ncode
{
	CString		code;
	CString		name;
	CString		kind;
	CString		fill;

}NCODE;

template <class T, class PT> class CTArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

template <class T, class TP> void CTArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1)
	{
		::QuickS(GetData(), (int)GetSize(), bAscending);
	}
}

template <class T> BOOL QuickS(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) 
	{
		try 
		{
			int	low = 0,
				high = iSize - 1;

			QuickSortRe(pArr, low, high, bAscending);
		} 
		catch (...) 
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}
	} 
	else 
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}

template <class T> void QuickSortRe(T *pArr, int low, int high, BOOL bAscending)
{
	int ii, jj;
	T str;
	T TTemp;

	ii = high;
	jj = low;

	str = pArr[((int) ((low+high) / 2))];

	do {
		if (bAscending) 
		{
			//while (pArr[jj].Compare(str) < 0) jj++;
			//while (pArr[ii].Compare(str) > 0) ii--;

			while (strcmp(pArr[jj].price, str.price) < 0) jj++;
			while (strcmp(pArr[ii].price, str.price) > 0) ii--;
		}
		else
		{
//			while (pArr[jj].Compare(str) > 0) jj++;
//			while (pArr[ii].Compare(str) < 0) ii--;
			while (strcmp(pArr[jj].price, str.price) > 0) jj++;
			while (strcmp(pArr[ii].price, str.price) < 0) ii--;
		}

		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				TTemp = pArr[ii];
				pArr[ii] = pArr[jj];
				pArr[jj] = TTemp;
			}
			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) QuickSortRe(pArr, low, ii, bAscending);
	if (jj < high) QuickSortRe(pArr, jj, high, bAscending);

}


template <class T, class PT> class CQArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

template <class T, class TP> void CQArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1)
	{
		::QuickSort(GetData(), (int)GetSize(), bAscending);
	}
}

template <class T> BOOL QuickSort(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) 
	{
		try 
		{
			int	low = 0,
				high = iSize - 1;

			QuickSortRecursive(pArr, low, high, bAscending);
		} 
		catch (...) 
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}
	} 
	else 
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}

template <class T> void QuickSortRecursive(T *pArr, int low, int high, BOOL bAscending)
{
	int ii, jj;
	T str;
	T TTemp;

	ii = high;
	jj = low;

	str = pArr[((int) ((low+high) / 2))];

	do {
		if (bAscending) 
		{
			while (pArr[jj].Compare(str) < 0) jj++;
			while (pArr[ii].Compare(str) > 0) ii--;
		}
		else
		{
			while (pArr[jj].Compare(str) > 0) jj++;
			while (pArr[ii].Compare(str) < 0) ii--;
		}
		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				TTemp = pArr[ii];
				pArr[ii] = pArr[jj];
				pArr[jj] = TTemp;
			}
			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) QuickSortRecursive(pArr, low, ii, bAscending);
	if (jj < high) QuickSortRecursive(pArr, jj, high, bAscending);

}

//-------------------------------------------------------------------------------------------
