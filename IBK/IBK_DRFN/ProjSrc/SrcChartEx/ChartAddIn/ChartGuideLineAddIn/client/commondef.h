#ifndef __COMMON_DEF_H
#define __COMMON_DEF_H
#endif

/////////////////////////////////////////////////////////////////////////////
//���� : ���ؼ� �۵� AddIn Module���� ���� ���� ���� ����
//��¥ : 2005. 01. 28
//�ۼ��� : Nam 
/////////////////////////////////////////////////////////////////////////////

#define		INI_FILENAME			"ChartGuideLine.ini"
#define		ACCT_NUM				11			// ����
#define		ACCT_NAME				40			// ���¸�
#define		ACCT_PASS				13			// ��й�ȣ
#define     SIZEFULLACCT			64			//����(11) + ���¸�(40) + ��й�ȣ(13)

//InvokeAddIn ȣ��� �� �Ѿ���� ��ȣ�� 

#define INVOKEMSG_SETACCOUNTNQUOTECODE		1
#define INVOKEMSG_GETAVGINFO				2
#define INVOKEMSG_INIT						100 // �����͸� ��ȸ�Ҽ� �ִ� HWND�� �� ��Ʈ����(?)�� �����͸� �޴´�. 
#define INVOKEMSG_RECEIVEDATA				101 // ü�� �����͸� ����.
#define INVOKEMSG_CHANGECODE				102	// �����ڵ� �ٲ���� �ǹ�
#define INVOKEMSG_SHOWORDEREDINFO			103 // ��Ʈ�� �׷���...



#define		UMSG_SEND2SERVERFROMINVOKE			WM_USER+500	 //TR��ȸ ��û
#define		UMSG_GETCODEFROMINVOKE				WM_USER+501	 //�����ڵ� �������� LPARAM�� CString *�� �ش�.
#define		UMSG_GETMAINHANDLEFROMINVOKE		WM_USER+502	 //�����ڵ� ��û�Ѵ�.

const UINT RMSG_GETACCTPASSENCKEY		= ::RegisterWindowMessage(_T("RMSG_GETACCTPASSENCKEY"));	// ���º�й�ȣ ��û
const UINT RMSG_VERIFYACCTPASS			= ::RegisterWindowMessage(_T("RMSG_VERIFYACCTPASS"));		// ���¹�ȣ ����(����, �ܺ� ���о���)

// ����,��й�ȣ ����
typedef struct _ST_CHECKACCTPASS
{
	char	szAccount[11];
	char	szAcctPass[13];
	int		nAcctCheckType;
} ST_CHECKACCTPASS;


//
/*
typedef struct _D10418_O 
{ 
	char sShcode[7];	// Short Code  
	char sName[20];		// Code Name  
	char sAvg1[16];		// Moving Average 1 Value(Type: F9.6)  
	char sAvg2[16];		// Moving Average 2 Value(Type: F9.6)  
	char sAvg3[16];		// Moving Average 3 Value(Type: F9.6)  
	char sAvg4[16];		// Moving Average 4 Value(Type: F9.6)  
	char sBuffer[12];	// Use Only Client  
} D10418_O;*/
 
typedef struct {
	char szBuffer	[12]; /* CWnd Type Pointer */
	char shcode		[7]; /* �����ڵ�*/ 
	char nAvg1		[3]; /* ����1*/ 
	char nAvg2		[3]; /* ����2*/ 
	char nAvg3		[3]; /* ����3*/ 
	char nAvg4		[3]; /* ����4*/ 
	/// don't send to server..
	char	chNull;
} D10418_I; 

typedef struct _D10418_O 
{ 	
	char sShcode[7]; // Short Code 
	char sName[20]; // Code Name 
	char sCurPrice[16]; // Current Price 
	char sSum1[16]; // ���� ��� 5�� �̶� �׷��� ������ ������ ���� 4���� �� 
	char sSum2[16]; // ���� ��� 20�� �̶� �׷��� ������ ������ ���� 19���� �� 
	char sSum3[16]; // ���� ��� 60�� �̶� �׷��� ������ ������ ���� 59���� �� 
	char sSum4[16]; // ���� ��� 120�� �̶� �׷��� ������ ������ ���� 119���� �� 
	
} D10418_O; 

typedef struct _D10418_INVOKE_O 
{ 	
	char sShcode[7]; // Short Code 
	char sName[20]; // Code Name 
	char sCurPrice[16]; // Current Price 
	char sSum1[16]; // ���� ��� 5�� �̶� �׷��� ������ ������ ���� 4���� �� 
	char sSum2[16]; // ���� ��� 20�� �̶� �׷��� ������ ������ ���� 19���� �� 
	char sSum3[16]; // ���� ��� 60�� �̶� �׷��� ������ ������ ���� 59���� �� 
	char sSum4[16]; // ���� ��� 120�� �̶� �׷��� ������ ������ ���� 119���� �� 
	
} D10418_INVOKE_O; 

// Server Input Structure
typedef struct __STSEND2SERVER 
{
	CString		m_strTRCode;
	void*		m_pBuffer;
	long		m_lLength;
	CWnd*		m_pWnd;
	int			m_nType;	// 0: Normal 1:Invoke
}stSend2Server;


// Server Output Structure
typedef struct __STRECEIVEFROMSERVER // AP ��ȸ�� 
{
	CString		m_strTRCode;
	void *		m_pBuffer;
	DWORD		m_dwLength;
}stReceiveFromServer;

typedef struct  {
	HWND		m_hChartMng;
	CWnd*		m_pDummy;		// Do not use it. m_pChartItem;
	void *		m_pBuffer;		
	DWORD		m_dwLength;

}stMSGToInvoke;

//===================================================================================
// ü�� ����Ʈ ��ȸ =========================
//===================================================================================
#define GRDMX 100

typedef struct _D48112_ISUB
{
    char    nxtGb        [ 1];       /* 0:ó����ȸ, 1:������ȸ   */
    char    ordseq       [ 1];       /* ��������     *//* start of gwInPacket */
	char    querygb     [ 1];       /* ��ȸ ����   0:��ü, 1:�ŵ�, 2:�ż�, 3:��ü��, 4:ü�� */
    char    gejanum     [11];       /* ���¹�ȣ     */
    char    passwd      [13];       /* ��й�ȣ     */
    char    jmcode       [12];       /* 'A'&�����ڵ� */
    char    frstordno    [ 5];       /* ���ʿ��ֹ���ȣ*/
    char    oriordno     [ 5];       /* ���ֹ���ȣ   */
    char    ordno         [ 5];       /* �ֹ���ȣ     */
    char    chedanga   [ 9];       /* ü��ܰ�     *//* end of gwInPacket */
} D48112_ISUB;

typedef struct _D48112_OG
{
//	char	garvage		[ 4];		/*���翡�� ���� ��*/
    char    frstordno   [ 5];       /* ���ʿ��ֹ���ȣ*/
    char    orgordno    [ 5];       /* ���ֹ���ȣ   */
    char    ordno       [ 5];       /* �ֹ���ȣ     */
    char    cheno       [ 5];       /* ü���ȣ     */
    char    jmcode      [12];       /* �����ڵ�     */
    char    jmnm        [60];       /* �����       */
    char    ordgb       [ 2];       /* �ֹ�����     */
    char    mkgb        [ 2];       /* ����ŷ����� */
    char    ordcnt      [ 9];       /* �ֹ�����     */
    char    orddanga    [ 9];       /* �ֹ��ܰ�     */
    char    checnttot   [ 9];       /* ü�������   */
    char    chedanga    [ 9];       /* ü��ܰ�     */
    char    checnt      [ 9];       /* ü�����     */
    char    jjcancnt    [ 9];       /* ������Ҽ��� */
    char    michecnt    [ 9];       /* ��ü�����   */
    char    mesumedogb  [ 1];       /* �ŵ��ż����� */
    char    singb       [ 2];       /* �ſ뱸��     */
    char    jjcangb     [ 1];       /* ������ұ��� */
    char    ordtime     [ 8];       /* �ֹ��ð�     */
    char    chetime     [ 8];       /* ü��ð�     */
    char    mechegb     [ 2];       /* ��ü����     */
    char    ordjr       [ 2];       /* �ֹ�����     */
    char    ordjg       [ 2];       /* �ֹ�����     */
    char    curigb      [ 4];       /* ó������     */
} D48112_OG;



typedef struct _D48112_OSUB
{
    char        nxtGb       [ 1];
    char        medoyakval  [12];
    char        medoyakvol  [ 9];
    char        mesuyakval  [12];
    char        mesuyakvol  [ 9];
	
    char        grdCnt      [ 7];
	
    D48112_OG   grid        [GRDMX];
} D48112_OSUB;

typedef struct _D48112_I
{
    char        sBuffer     [12];
    D48112_ISUB iData;
} D48112_I;


typedef struct _D48112_O
{
    char        sBuffer     [12];
    D48112_OSUB oData;
} D48112_O;