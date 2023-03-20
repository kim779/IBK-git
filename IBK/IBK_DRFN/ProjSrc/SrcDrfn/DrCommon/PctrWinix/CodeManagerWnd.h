#if !defined(AFX_CODEMONITOR_WND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_)
#define AFX_CODEMONITOR_WND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CodeManagerWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCodeManagerWnd window

// 2012.2.1 �ڼ��� >>
#include "../../../SrcSite/include/jmcode.h"
#include "../../../SrcSite/include/axisvar.h"
#include "../../../SrcSite/include/user.h"

// KRX ������ ���а�
#define SEC1_AT       0x01  /* �ڵ���                   */
#define SEC1_SM       0x02  /* �ݵ�ü                   */
#define SEC1_HC       0x04  /* �ǰ�                     */
#define SEC1_BK       0x08  /* ����                     */
#define SEC2_IT       0x01  /* �������                 */
#define SEC2_EC       0x02  /* ȭ�п�����               */
#define SEC2_ST       0x04  /* ö��                     */
#define SEC2_CS       0x08  /* �ʼ��Һ���               */
#define SEC3_MT       0x01  /* �̵�����               */
#define SEC3_CO       0x02  /* �Ǽ�                     */
#define SEC3_FB       0x04  /* ��������               */
#define SEC3_SI       0x08  /* ����                     */
#define SEC4_SC       0x01  /* ����                     */
#define SEC4_SR       0x02  /* ��ȸ����å������         */
#define SEC4_IS       0x04  /* ����                     */
#define SEC4_TP       0x08  /* ���                     */
#define SEC5_RT       0x01  /* �Һ�������               */
#define SEC5_LS       0x02  /* ����                     */
#define SEC5_KX       0x04  /* KRX100                   */

//�߰�
template<class T> 
void QSortCArray( T& t, int (__cdecl *compare )(const void *elem1, const void *elem2 ) ) 
{ 	
    if( t.GetSize() <= 0 ) return; 
    qsort( t.GetData(), t.GetSize(), sizeof( t[0] ), compare ); 	
} 

// int �ڷ������Լ� 
int CompareChar( const void *elem1, const void *elem2 );

struct _shjcode			// semi hjcode
{
	CString code;
	CString name;
	CString symb;		// symbol
	char	ecng;		// ECN ����
	char	size;		// �ں��ݱԸ�	2:��, 3:��, 4:��
	char	ucdm[6];		// �����ߺз�
	char	ucds[6];		// �����Һз�
	char	jjug;		// ������ ����(27: ������)
	char	kpgb;		// KOSPI200 ����
	char	kosd;		// ���񱸺�
	char	ssgb;		// �Ҽӱ���
	char	ucmd;		// 0:������, 5:�켱��
	char	wsgb;		// 0:�Ϲ� 1:���豸��������
	char	jsiz;		// �����ں��� ���߼�
	char	itgb;		// KOSPI-IT/KQ-IT50����(1:ä��)
	char	star;		// KOSDAQ ��Ÿ����
	char	prmr;		// �����̾�����
	char	unio;		// �������� (KRX100)
	CHAR	uc10;		/* �Ǽ����		0x00			*/
						/* �������		0x01			*/
						/* ö������		0x02			*/
						/* ������ȭ��	0x03			*/
						/* �������		0x04			*/
						/* ����			0x05			*/
						/* �ʼ��Һ���	0x06			*/
						/* �����Һ���	0x07			*/
	char	uc20;		/* �췮���		0x00			*/
						/* ��ó���		0x01			*/
						/* �߰߱��		0x02			*/
						/* �ż�����	0x03			*/					
	/* ��������                     */                                            
    char    sec1;       /*  �ڵ���/�ݵ�ü/�ǰ�/����     */
    char    sec2;       /*  �������/ȭ��E/ö��/�ʼ�    */
    char    sec3;       /*  �̵��/�Ǽ�/����/����       */
    char    sec4;       /*  ����/��ȸ����/����/���     */
    char    sec5;       /*  �Һ���/����/KRX100          */
};

struct _sjcode			// semi code
{
	CString code;
	CString name;
};

struct _elwdata
{
	CString		code;	// ELW �����ڵ�
	CString		name;	// �����
	CString		ktype;	// �Ǹ����� 01:Call, 02:Put, 03:��Ÿ
	CString		pcode;	// ����� �ڵ�
	CString		pname;	// ������
	CString		mjmt;	// ����� (YYYYMMDD)
	CString		bcode[5];
};

typedef struct _pbcode
{
	CString		code;	// ����� �ڵ�
	CString		name;	// ������
}PBCODE;

typedef struct _sortStr
{
	CString code;
	CString name;
}SORTSTR;

// �ؿ� ���� ����
typedef struct _uscode
{
	char	symb[16];		/* ���� SYMBOL			*/
	char	datg;			/* ��з�				*/
							/* 0: ����	1: ����		*/
							/* 2: ����	3: �ݸ�		*/
							/* 4: ��ä	5: ȯ��		*/
							/* 7: others			*/
	char	jsgb;			/* �ߺз�(Combo Box)	*/
	char	knam[40];		/* �ѱ� SYMBOL��		*/
	char	enam[40];		/* ���� SYMBOL��		*/
	char	icod[6];		/* ���� �����ڵ�		*/
	char	ncod[3];		/* �����ڵ�				*/
	char	exid[4];		/* �ŷ����ڵ�			*/
}uscode;
/*****************************************************
 jsgb ���а�
 [����]
 1. �̱�	2. �ƽþ�	3. ����		4. MSCI		5. ��Ÿ
 [����]
 1. �̱�	2. �Ϻ�		3. ȫ��		4. �븸		5. ����		6. ������		7. ����		8. ADR		9. GDR
 [����]
 1. CME��������			2.������	3. ��ǰ����
 [�ݸ�]
 1. �����ݸ�	2. �̱��ݸ�
 [��ä]
 1. �̱�	2. �Ϻ�		3. ����		4. ����
 [ȯ��]
*****************************************************/
// 2012.2.1 �ڼ��� <<

typedef struct _RealKey
{
	char    ucod[3];		/* �����ڵ�                     */
	char    rcod[5];		/* RealTime Code                */
	char    hnam[20];		/* �ѱ� ������                  */
	char    fill[40];		/* filler                       */
	char    lnfd[1];		/* �����ǵ�                     */
} RealKey;

class CCodeManagerWnd : public CWnd
{
	// Construction
public:
	CCodeManagerWnd();

// Attributes
public:
	CString	m_szRootDir, m_szUserDir;
	CString m_strData;

// Operations
public:
	// �ڼ��� >>
	CArray <_shjcode, _shjcode>	m_hjcode;		// ��������
	CArray <_sjcode, _sjcode>	m_fjcode;		// ��������
	CArray <ojcode, ojcode>		m_ojcode;		// �ɼ�����
	CArray <pjcode, pjcode>		m_pjcode;		// �����ɼ�
	CArray <upcode, upcode>		m_upcode;		// �����ڵ�
	CArray <_sjcode, _sjcode>	m_elwbase;		// elwbase
	CArray <_elwdata, _elwdata> m_ELWdata;		// elw���� ����
	CArray<PBCODE, PBCODE> m_arrayPBCode;		// elw Ʈ�����񸮽�Ʈ
	CArray<SFCODEBASE, SFCODEBASE> m_sfbase;	// �ֽļ��� ��ǥ���� ����
	CMap<CString,LPCSTR,SFCODEINFO,SFCODEINFO> m_mapsfinfo;	// �ֽļ��� Ʈ�����񸮽�Ʈ
	CMap<CString,LPCSTR,SFCODE,SFCODE> m_mapsfcode;	// �ֽļ��� ��������
	CArray <uscode, uscode>		m_uscode;		// �ؿ����� ����
	CArray <ccode, ccode>		m_ccode;		// ��ǰ����
	CMap<CString,LPCSTR,CString,CString> m_mapRealKey;		// ����Ű ����
	CMap<CString,LPCSTR,hjcodex,hjcodex> m_maphjcode;	// ELW �����ϴ� ���� �ֱ����� HJCode������ ��

	CArray <_sjcode, _sjcode>	m_mfcode;		// �̴ϼ�������
	CArray <ojcode, ojcode>		m_mocode;		// �̴Ͽɼ�����

	CArray <ojcode, ojcode>		m_wocode;		// ��Ŭ���ɼ�����
	
	void	loadingHJcode();
	void	loadingFJcode();
	void	loadingOJcode();
	void	loadingPJcode();
	void	loadingUPcode();
	void	loadingELWcode();
	void	loadingSfCode();
	void	loadingUSCode();
	void	loadingCFcode();
	void	loadingMFcode();
	void	loadingMOcode();
	void	loadingWOcode();
	void	loadingRealKey();
	
	CString	GetCodeName(CString code);
	int FindPBCode(CString code);
	
	void GetAllCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKospiToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaqToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKospi200ToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaq50ToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKRX100ToList(CStringArray &arrCode, CStringArray &arrName);
	void GetFreeCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetELWPBToList(CString strTitle, CStringArray &arrCode, CStringArray &arrName);
	void GetELWBaseToList(CString strTitle, CStringArray &arrCode, CStringArray &arrName);
	void GetETFCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetSinjuCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetFutureToList(CStringArray &arrCode, CStringArray &arrName);
	void GetCallCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetPutCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKospiJisuToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaqJisuToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKospiUpList(CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaqUpList(CStringArray &arrCode, CStringArray &arrName);
	void GetKRXUpList(CStringArray &arrCode, CStringArray &arrName);
	void GetKNXToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKRXJisuToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKNXUpList(CStringArray &arrCode, CStringArray &arrName);
	void GetThemaList(CStringArray &arrCode, CStringArray &arrName);
	void GetPjcodeToList(CString code, char kind, CStringArray &arrCode, CStringArray &arrName);
	void GetKospiUpToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaqUpToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetKospiExToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaqExToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetThemaCodeToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetGroupCodeToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetInterestToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetSFutCodeToList(CString strKey, CStringArray &arrCode, CStringArray &arrName);
	void GetELWBaseTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetELWPBTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetSFutTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetKRXToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetKOSPI200Index(CStringArray &arrCode, CStringArray &arrName);
	void GetKOSPI100Index(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignIndexTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignItemTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignFutureTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignRateTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignDebtTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignExchangeTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetKOFEXTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetKOFExCodeToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetForeignMajorToList(CStringArray &arrCode, CStringArray &arrName);
	void GetFreeUpToList(CStringArray &arrCode, CStringArray &arrName);
	CString GetRecentChxUpCodeList(CString strCode);

	void GetEtnToList(CStringArray &arrCode, CStringArray &arrName);
	void GetVFToList(CStringArray &arrCode, CStringArray &arrName);
	void GetSF2UpList(CStringArray &arrCode, CStringArray &arrName);	
	void GetSF2CodeToList(CString strKey, CStringArray &arrCode, CStringArray &arrName);
	void GetSF2TreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetETCUpToList(CStringArray &arrCode, CStringArray &arrName);
	
	void GetIBKMasterData(UINT selItem, CStringArray &arrCode, CStringArray &arrName);
	
	void AppendItemToList1(CString code, CString name, CStringArray &arrCode, CStringArray &arrName);
	
	CString	parseX(CString &srcstr, CString substr);
	bool	ExistFile(int gno, bool temp = false);
	// �ڼ��� <<

	void GetMiniFutureToList(CStringArray &arrCode, CStringArray &arrName);
	void GetMiniCallCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetMiniPutCodeToList(CStringArray &arrCode, CStringArray &arrName);

	void GetWeeklyCallCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetWeeklyPutCodeToList(CStringArray &arrCode, CStringArray &arrName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeManagerWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeManagerWnd();
	BOOL	OpenWindow();
	LPCSTR	GetUserDir();
	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeManagerWnd)
	LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT _GetCode(LPARAM lParam);
	LRESULT	_GetCodeType(LPARAM lParam);
	LRESULT _GetCodeName(LPARAM lParam);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEMONITOR_WND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_)
