#ifndef __CODEHELPER_H__
#define __CODEHELPER_H__


#define TRCD_HISTORY		1
#define TRCD_ALLCODE		2
#define TRCD_KOSPIUJ		3
#define TRCD_KOSDAQUJ		4
#define TRCD_COMPANY		5
#define TRCD_INTEREST		6
#define TRCD_THEME			7

#define ID_KOSPI200		111
#define ID_KOSPI100		112
#define ID_KOSPI50		113
#define ID_KOSPIIT		114

#define ID_KOSDAQ50		222
#define ID_KOSDAQIT		223
#define	ID_KOSTAR		224
#define	ID_ORAKMUNHWA		225

#define DTIF_KIND	0x0001
#define DTIF_SUBKIND	0x0002
#define DTIF_INDEX	0x0004


struct JCODE{
	CString Code;
	CString Name;
	//CString SNam;
	char	size;		// �ں��ݱԸ�	2:��, 3:��, 4:��
	char	ucdm;		// �����ߺз�
	char	ucds;		// �����Һз�
	char	jjug;		// ������ ����(27: ������)
	char	kpgb;		// KOSPI200 ����
	char	kosd;		// ���񱸺�
	char	ssgb;		// �Ҽӱ���
	char	ucmd;		// 0:������, 5:�켱��
	char	wsgb;		// 0:�Ϲ� 1:���豸��������
	char	jsiz;		// �����ں��� ���߼�
	char	itgb;		// IT...
	char	star;		// STAR
};


typedef BOOL (*CODEFILTERPROC)(JCODE* p, int index);

CODEFILTERPROC GetCodeFilterProc(int kind, int subkind, int index);


#define GROUP_FILE	"gropcd.ini"
#define THEME_FILE	"themcd.ini"

struct _inters
{
	char	gubn;		// ���񱸺�
				// 0:none, 1:����, 2:����, 3:�ɼ�, 4:�����ֽĿɼ�, 5:����
	char	code[12];	// �����ڵ�
	char	name[30];	// �����
	int		possNum;	// ��������
	double	possPrice;	// �����ܰ�
	char	memo[30];	// �޸�
	char	o_check;	// �ֹ� ���࿩��
				// 0:none, 1:checked
	char	o_kind;		// �ֹ�����
				// 1:�ŵ�, 2:�ż�
	char	o_mmgb;		// �Ÿű���
				// 1:����, 2:���尡
	int		o_num;		// �ֹ�����
	double	o_price;	// �ֹ��ܰ�
	char	a_kind;		// �˶� ����
				// 1:�׸���, 2:�׸��� �Ҹ�
	int		a_num;		// ���� �׸� ����
	char	filler[100];	// Reserved
};

typedef struct {
	int kind;
	int subkind;
	int index;
} KIND, *LPKIND;

#define MODE_READ	0x01
#define MODE_WRITE	0x02

class CAxInformation {
private:
	CString m_fileName;
	CFile*	m_file;
	CString m_buffer;
	int		m_mode;

	CString GetStartTag(LPCTSTR item);
	CString GetEndTag(LPCTSTR item);
public:
	CAxInformation(LPCTSTR fileName, int mode = MODE_READ);
	virtual ~CAxInformation();
public:
	CString GetString(LPCTSTR item, LPCTSTR def = "");
	int		GetInt(LPCTSTR item, int def = 0);
	void	SetString(LPCTSTR item, LPCTSTR value);
	void	SetInt(LPCTSTR item, int value);
	void	DeleteAll();
	void	SetMode(int mode);
};

class CAxTargetItem {
private:
	CString m_fileName;
public:
	CAxTargetItem();
	virtual ~CAxTargetItem();

	void	LoadItemFromFile();
	void	SaveItemToFile();
	static  int		KindToKey(KIND& kind);
	static  void	KeyToKind(int key, KIND& kind);
public:
	void	SetItem(KIND& kind, CString items); 
	void	SetItem(KIND& kind, CListCtrl& list);
	void	SetItem(int key, CString items);

	CString	GetItem(KIND& kind);
	void    GetItem(KIND& kind, CListCtrl& list);

	CMap <int, int, CString, CString> m_items;
};



#endif // __CODEHELPER_H__