#ifndef _AXCOMMON_HXX
#define _AXCOMMON_HXX

//
//	status bar
//	operator area pane
//
#define	OA_OPERATOR		5001
#define	OA_MANAGER		5002
#define	OA_SEND			5003
#define	OA_PBPR			5004
#define	OA_ENGHAN		5005
#define	OA_FULLHALF		5006
#define	OA_TIMER		5007
#define	OA_TICKER		5008
#define	OA_TICKER2		5009
#define	OA_ALARM		5010

#define	OA_RTMBASE		6000
#define	OA_RTMK0		1		// KOSPI ��������
#define	OA_RTMK2		2		// KOSPI200 ��������
#define	OA_RTMKX		3		// REAL KOSPI200 ��������
#define	OA_RTMKQ		4		// KOSDAQ ����
#define	OA_RTMQ5		5		// KOSDAQ50 ����
#define	OA_RTMQX		6		// REAL KOSDAQ50 ��������
#define	OA_RTMFJ		7		// �����ֱٹ� �ü�(KOSPI200��������)
#define OA_RTMSF		8		// S&P 500��������(�۷ι齺)
#define OA_RTMNF		9		// ����Ź100�����ü�(�۷ι齺)
#define	OA_RTMSH		10		// ������ �Ÿ���Ȳ

#define	OA_CONCLUDE		100		// ����ں� ü��


/*
#define	OA_RTMD0		3		// KOSPI ���յ��
#define	OA_RTMWD		7		// ��/$ ȯ��
#define	OA_RTMJM		8		// ������ MSG
#define	OA_RTMQJ		10		// KOSDAQ�����ü�
#define	OA_RTMKX		15		// KOSPI200 ��ü����(3������)
*/

//
//	realtime define symbol
//
const	int sSHK1 = 14;		// ��Ȳ Ű1
const	int sSHK2 = 15;		// ��Ȳ Ű2
const	int sSHDA = 16;		// ��Ȳ ����Ÿ

const	int sCODX = 21;		// �����ڵ�
const	int sHANM = 22;		// �����
const	int sCURR = 23;		// ���簡
const	int sDIFF = 24;		// ���ϴ��
const	int sMDGA = 25;		// �ŵ�ȣ��
const	int sMSGA = 26;		// �ż�ȣ��
const	int sGVOL = 27;		// �ŷ���
const	int sGAMT = 28;		// �ŷ����(��)
const	int sSIGA = 29;		// �ð�
const	int sKOGA = 30;		// ��
const	int sJEGA = 31;		// ����
const	int sCVOL = 32;		// ü��ŷ���
const	int sUDYL = 33;		// �����
const	int sCTIM = 34;		// ü��ð�
const	int sCCOL = 35;		// �İᱸ��

const	int sSSJS = 252;	// ��������
const	int sSHJS = 251;	// ���������
const	int sBHJS = 253;	// ���������
const	int sHRJS = 255;	// �϶������
const	int sHHJS = 254;	// ���������


//
//	investor item
//
#define	investJK	0x00000001	// ����
#define	investBH	0x00000002	// ����
#define	investTS	0x00000004	// ����
#define	investUH	0x00000008	// ����
#define	investCK	0x00000010	// ����
#define	investKK	0x00000020	// ���
#define	investKT	0x00000040	// ��Ÿ
#define	investKE	0x00000080	// ����
#define	investEK	0x00000100	// �ܱ���
#define	investJU	0x00000200	// ����


#endif	// _AXCOMMON_HXX