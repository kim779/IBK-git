#ifndef __PACKET__HEADER__KLDK_FJJHD9383_KKDDFH
#define __PACKET__HEADER__KLDK_FJJHD9383_KKDDFH

//@��������
//// �����Ŷ ���� 
//typedef struct {
//	UCHAR	Stx;				// 0x02
//	UCHAR	Packet_Len[2];		// ��Ŷ ����Ÿ ������
//	UCHAR	Packet_Attri;		// ��Ŷ �Ӽ� : ����, ��ȣȭ, ����
//	UCHAR	Packet_Seq;			// AP Packet�� �Ϸù�ȣ 
//	UCHAR	PacketData[1017];   // PACKET DATA
//	UCHAR	Etx;				// 0x03
//	UCHAR	Bcc;				// BCC CHECK
//} HtsPacket, *LPHTSPACKET;

// STTRHEADER�� ������ 108
typedef struct {
	long	WindowID;			// 4 byte
	UCHAR	TRCode[5];
	UCHAR	MsgCode[6];
	UCHAR	Message[82];
	UCHAR	CursorPos[2];
	UCHAR	Message2Len[3];		// ���� �޽��� ����
	UCHAR	APDataLen[3];		// AP���� ����ϴ� ����Ÿ
	char	TRData[1];			// 3 byte
} STTRHEADER, *LPSTTRHEADER;
#define		SZSTTRHEADER		sizeof(STTRHEADER)
#define		STTRHEADER_TRDATAHEAD		SZSTTRHEADER-sizeof(char)

typedef struct {
	UCHAR	Command;
	STTRHEADER  pTrData;
} STPACKETDATA, *LPSTPACKETDATA;

// SOCKET���� ������ WM_COPYDATA ������
#define		TYPE300			300
#define		TYPE301			301
#define		TYPE400			400
#define		TYPE401			401
#define		TYPE410			410
#define		TYPE500			500
#define		TYPE900			-900			// -900����� ������ȣ����
			// 900			Cert ��� �� ��й�ȣ ����

//- ��(ȭ��)���� ����Ÿ�� ������쿡�� ��ȣȭ���ΰ� ���Եȴ�.
typedef struct {
	BOOL	bEnc;
	int		nLen;
	STTRHEADER trHeader;
} STTYPE400, *LPSTTYPE400;

typedef struct {
	BOOL	bEnc;
	int		nLen;
	char	pData[2];		// Data
} STTYPE300, *LPSTTYPE300;


typedef struct {
	BOOL	bEnc;
	int		nLen;
	char	pData[20];		// Data
} STTYPE301, *LPSTTYPE301;




#define		SOCKET_CONNECT_FLAG		1	//	(LPARAM :������ : 0�� �ƴ� 
#define		SOCKET_ERROR_PLAG		2	//	(LPARAM : �����޼���)
#define		SOCKET_PACKET_RECV		3	//	(LPARAM : �����޼���)
#define		SOCKET_CLOSE_RECV		4	//	(LPARAM : �����޼���)


#endif // __PACKET__HEADER__KLDK_FJJHD9383_KKDDFH
