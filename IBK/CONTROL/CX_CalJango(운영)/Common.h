#pragma once

#define COLOR_LINE		93
#define COLOR_TABLE		75
#define COLOR_BK		64
#define COLOR_GRIDHEAD		74
#define COLOR_GUIDEBACK		213
#define COLOR_PANELBACK		66
#define COLOR_BODYBACK		181
#define COLOR_GRIDHEADTXT	76
#define COLOR_DATA		99
#define COLOR_TEXT		69
#define COLOR_TABLEHEAD		96
#define	COLOR_MDB		171		// �ŵ� back
#define	COLOR_MSB		170		// �ż� back
#define COLOR_PLUSTXT		94
#define COLOR_MINUSTXT		95
#define COLOR_FOCUS		78

// for speed parsing
class StringProxy : public pair<LPCSTR,LPCSTR>
{
public:
	StringProxy() : pair<LPCSTR,LPCSTR>() {}
	StringProxy(LPCSTR st, LPCSTR ed) : pair<LPCSTR, LPCSTR>(st, ed) {}

	string ToStr()		{ return string(first, second); }
	CString ToCStr()	{ return CString(first, second-first); }
};

class Jango
{
public:
	Jango() { ZeroMemory(this, sizeof(Jango)); }
	void operator=(const Jango &rhs) { CopyMemory(this, &rhs, sizeof(Jango)); }

	char	flag;		// for Window Message
	char	acno[12];	// ���¹�ȣ
	char	cod2[12];	// �����ڵ�
	char	hnam[40];	// �����
	char	jggb[2];	// �ܰ��ڵ�
	int	jqty;		// �ܰ����
	int	xqty;		// �ŵ�����
	double	pamt;		// ������հ�
	double	mamt;		// ���Աݾ�

	int	curr;		// ���簡     (*)
	double	camt;		// �򰡱ݾ�
	double	tamt;		// �򰡼���   (*)
	double	srat;		// �򰡼��ͷ� (*)
	char	sycd[2];	// �ſ��ڵ�
	char	sydt[8];	// ������
	char	samt[15];	// �ſ�ݾ�
};