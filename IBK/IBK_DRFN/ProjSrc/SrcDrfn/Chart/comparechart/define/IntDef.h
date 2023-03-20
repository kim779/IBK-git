#ifndef _INT_DEF_HEADER
#define _INT_DEF_HEADER

#define  SH_KSIZE  7

#define REQUEST_ERROR      "0"   // ERROR
#define REQUEST_INTALL     "1"   // �׷� ����Ʈ ��������
#define REQUEST_REG_IF     "2"   // ���� ���
#define REQUEST_REG_ITEMS  "3"   // ��   ���� ���
#define REQUEST_REG_SIGNAL "4"   // �ñ׳ο��� ���� ���
#define REQUEST_OPENDLG	   "5"   // ���� ���̾�α׸� ���½�ų ���

typedef struct
{
	char Ino[2];		// �׷� ��ȣ 
	char Ititle[20];	// �׷� ��
	char Icnt[3];		// �׷� �ȿ� ���� ��
	void* Idata;		//���� �����͵�(CPtrArray)
}INT_GROUP_ST;

typedef struct
{
	char Igubun[2]; // ���� REQUEST ����
	char Icnt[3]; // �׷� ��
	char IopenDlg[1]; // 1 : Open, 0 : No Open
	void* Idata;  // �׷� �����͵�(CPtrArray) 
	char Itime[6];
	char Idate[8];
}INT_TOTAL;

typedef struct
{
	char Icode[SH_KSIZE];  // �ڵ�  
	char Igubun[2];		   // 0:Kospi, 1:kosdoq,,,
	char Itime[6];		   // HHMMSS   ��,��,��
	char Idate[8];		   // YYYYMMDD ��,��,��
	char Ivol[10];		   // ���� 	
	char Ival[10];		   // ���԰���
	void* Idata;
} INT_ITEM_ST;

const UINT RMSG_INTREQUEST = ::RegisterWindowMessage("RMSG_INTREQUEST");
const UINT RMSG_CONCERNHWND	= ::RegisterWindowMessage(_T("RMSG_CONCERNHWND"));

#endif //_INT_DEF_HEADER