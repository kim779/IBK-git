#if !defined(AFX_0D1DA4A3_849F_11D4_B318_0048543A0D2E__INCLUDED_)
#define AFX_0D1DA4A3_849F_11D4_B318_0048543A0D2E__INCLUDED_

// #include "../include/CtrlPackBase.h"
#define		BCP_OK					0	// ����
#define		BCP_ERROR				-1	// UnKnown Error

// Input Error	-100����
#define		BCP_INPUT				-100

// Output Error	-200 ����
#define		BCP_OUTPUT				-200

class CBaseCtrlPackIn
{
public:
	// bufLen ũ�⸦ ���� buf �� �����͸� �޴´�.
	// ��Ʈ�ѿ����� NULL ���� ���� �ȵǰ�, �տ������� �����͸� ä���. 
	// ���� ������ �����̽��� �ִ´�.
	// �Է��� �̷������ �ʾ����� ���� ���� �Ѱ��ش�.
	// �����̸� 0, �����̸� ������
	// �ش���� ���� ���۸� ���Ƿ� �մ�� �ȵȴ�.
	// NULL���� ������ ���� �ȵȴ�.
	virtual int GetData(char* buf, int bufLen){ASSERT(FALSE); return 0;}
};

class CBaseCtrlPackOut
{
public:
	// ����ũ�⸦ ���� ��Ʈ���� ��쿡 ȣ��ȴ�.
	// ��Ʈ�ѿ��� ����� ����Ÿ�� ũ�⸦ ����ؼ� �Ѱ��ش�.
	virtual int	GetPackDataSize(char* pIn){ASSERT(FALSE); return 0;}

	// ��Ʈ�ѿ��� �Ѱ��� ��Ŷ�����͸� ���̿� �Բ� �Ѱ��ش�.
	// ����Ÿ�� ���̰� ������ ����϶��� ����Ÿ
	virtual int SetData(char* buf, int bufLen){ASSERT(FALSE); return 0;}

	// ��Ʈ�ѿ��� �Ѱ��ִ� ����Ÿ�� *�ΰ���϶��� ����Ÿ
	virtual int SetData2(char* buf, int bufLen){ASSERT(FALSE); return 0;}

	// ��Ʈ�ѿ��� �Ѱ��ִ� ����Ÿ�� !�ΰ���϶��� ����Ÿ
	virtual int SetData3(char* buf, int bufLen){ASSERT(FALSE); return 0;}

	// * ����Ÿ������
	// * ������ ����� Ÿ�Ե� ����
	// * nType  : 1 -> SetData ��Ÿ��
	// *          2 -> SetData2 ��Ÿ��
	// *          3 -> SetData3 ��Ÿ��
	// * Ȥ � ��Ÿ���� �� �ʿ��ϰ� ���� �𸣱� ������ �� ��ƾ�� �߰���.
	virtual int SetData999(int nType, char* buf, int bufLen) {ASSERT(FALSE); return 0;}
};



#endif // AFX_0D1DA4A3_849F_11D4_B318_0048543A0D2E__INCLUDED_