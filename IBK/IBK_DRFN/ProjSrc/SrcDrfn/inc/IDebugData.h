#if !defined(AFX_DEBUGDATA_HEADER_BY_JUNOK_LEE_090805_INCLUDED_)
#define AFX_DEBUGDATA_HEADER_BY_JUNOK_LEE_090805_INCLUDED_

#include <afxtempl.h>

#pragma	pack(1)

// #include "../../inc/IDebugData.h"

enum{
	IDM2_TAG_UM=0,		//User Message
	IDM2_TAG_TI,		//TR Input
	IDM2_TAG_TO,		//TR Output
	IDM2_TAG_RA,		//Real Advise
	IDM2_TAG_RU,		//Real UnAdvise
	IDM2_TAG_RD,		//Real Data
	IDM2_TAG_FI,		//Real FID Input
	IDM2_TAG_FO,		//Real FID Output
	IDM2_TAG_FILLER1,	//���� �����.
	IDM2_TAG_FILLER2,	//���� �����.
	IDM2_TAG_FILLER3,	//���� �����.
	IDM2_TAG_FILLER4,	//���� �����.
	IDM2_TAG_FILLER5,	//���� �����.
	IDM2_TAG_FILLER6,	//���� �����.
	IDM2_TAG_SR,		//System Reques
						//������ �ޱ� ��û��.
	IDM2_TAG_END,		//End TAG, ��������� ����.

	// UM : User Message
	// TI : TR Input
	// TO : TR Output
	// RA : Real Advise
	// RD : Real Data
	// RU : Real Unadvise
	// SR : System Reques
};
#define	MAX_IDM2_TAG	IDM2_TAG_END

//	StdAfx.cpp�� ����
extern char *gIDM2_TAGSTRING[];// = {"UM", "TI", "TO", "RA", "RU", "RD", "FI", "FD",
				//"F1", "F2", "F3", "F4", "F5", "F6", "SR"};

//�����ǻ���STIDM2Info_RA ����ü���� +1�� �Ǿ��ִ� �ǹ̴� Null���ڿ��� ���� ä���� �ǹ���.
//	���ο��� �ٷλ���ϱ� ���ؼ� Null���ڿ��� ó����.
typedef struct 
{
	int  nInfoSize;
	char aKey[20+1];
	char aDataName[20+1];
	char aFieldName[30+1];	//NULL����.
	int  nKeyNameSize;	//aRealData�� ���� ����.
	char aKeyName[1];
} STIDM2Info_RA;

//STIDM2Info_Data ����ü���� +1�� �Ǿ��ִ� �ǹ̴� Null���ڿ��� ���� ä���� �ǹ���.
//	���ο��� �ٷλ���ϱ� ���ؼ� Null���ڿ��� ó����.
//  ����Ÿ(aData)�� NULL�� ���Ե� �� �����Ƿ� NULL���Կ��� ������.
typedef struct 
{
	BYTE	aDataKey1[ 1];		// I/O ���� 'I' or 'O'
	char	aDataKey2[30+1];	// ��Ŷ���а�. NULL����.
	BYTE	pData[1];
} STIDM2Info_TranData;

#define SizeOutOneData(xStr)	(sizeof(xStr)-sizeof(char))

#define	IDebugManager_sizeTranKey	(32)	//aDataKey1+aDataKey2

// RA�� RD�� ��������.
#define STIDM2Info_RU  STIDM2Info_RA

typedef struct
{
	//	char sDataTime[19];	//YYYY-MM-DD hh:mm:ss
	char sYYYY[4];
	char sMM[2];
	char sDD[2];
	char shh[2];
	char smm[2];
	char sss[2];
	char stt[2];
} StDRTimeFormat;
#define	StDRTimeFormat_LEN	sizeof(StDRTimeFormat)

// IDM2_TAG_SR�����϶��� ������ó��
typedef struct
{
	UINT wParam;
}STIDM2Info_SR_Base;

#define	IDM2_KEY_SR0  0x01		// type : HIWORD(wParam)
								// Data Request

typedef struct : public STIDM2Info_SR_Base
{
	BYTE	aDataKey;		// 'A':Advise, 'U' UnAdvise
	BYTE	aDataKey1[ 1];	// I/O ���� 'I' or 'O'
	char	aDataKey2[30+1];// ��Ŷ���а�.NULL����.
	char	aXHandle[8];	// WM_COPYDATA�� ���� �ڵ�.
	WORD	dwData;			// WM_COPYDATA������ pCopyDataStruct->dwData
}STIDM2Info_SR_A;

typedef CList<STIDM2Info_SR_A*, STIDM2Info_SR_A*> List_STIDM2Info_SR_A;

enum  IDebugM_MSG 
{
	IDebugM_UM	= 0,			// ����� �޼���
		IDebugM_TR,				// ��ȸ ������
};

typedef struct
{
	DWORD	dwTotalSize;	// ������������ ���̸� ������ ��ü����.
	BYTE	cSiteFlag;		// Site Flag.
	BYTE	cIO;			// TR�� 'I' or 'O'
	//CString	szTranName;
	char	aTrName[30+1];	// NULL���� ���ڿ�.
	DWORD	dwSize;			// Data�� ��������.
	BYTE	pData[1];			// Data ����.
}STIDM2CompData;

#define STIDM2CompData_PreSize	(sizeof(STIDM2CompData)-sizeof(BYTE))

#pragma	pack()

#endif //AFX_INTERFACE_OF_DEBUGMANAGER_BY_JUNOK_LEE__INCLUDED_
