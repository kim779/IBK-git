#ifndef HANA_TYPEDEF
#define HANA_TYPEDEF

#define SCREEN_NO		"HD770000"

#define	MYGETAPP	((CHD770000App *)AfxGetApp())

#define WM_MANAGE	(WM_USER + 1000)

/////////////////////////////////////////////////////////////////
//////  WM_Main_NormalWnd	Msg Kind
/////////////////////////////////////////////////////////////////
// LOWORD(wParam)
#define MK_GETROOT		1	// ROOT Directory
#define MK_GETUSER		2	// USER��
#define MK_SETPAL		3	// set color
#define MK_SENDTR		4	// sendtr
#define MK_VARIANT		5	// variant
#define MK_GETGROUP		6	// get group count
#define MK_GROUP		7	// HIWORD(wParam) 1 => add 0 => minus
#define MK_GETBITMAP		8	// get bitmap handle
#define MK_TREEVISIBLE		9	// get treesize
#define MK_BASKETVISIBLE	10	// get treesize
#define MK_ISEXPAND		11	// �׷��߰��� Ȯ�����ΰ� ���Ҹ���ΰ�...
#define MK_SIZE			12	// ȭ������� ������ ���
#define MK_OPENSCR		13	// ȭ�� ����
#define MK_EXPECT		14	// ���� ����
#define MK_EXCEL		15	// ������ ������
#define MK_SELGROUP		17	// �׷켱�ÿ� ���� �޽���
#define MK_MEMOUPDATE		18	// �޸� ����
#define MK_CLICKGROUP		19	// �׷� Ŭ���� �۾��� ���� �ɼ�
#define MK_CLOSEGRID		20	// �׸��� â�� �ݴ´�
#define MK_SETDATAKIND		21	// ������ ����
#define MK_TREEDATA		22	// Ʈ�����ý� �׷����� �������� �޽���
#define MK_GETDATATITLE		23	// ���� �������� Ÿ��Ʋ�� �䱸�Ѵ�.
#define MK_GETWND		24	// ������ ������ ��������
#define MK_GETAFIELD		25	// ���� ���õ� �ʵ� �ε����� �����´� From ToolWindow
#define MK_TURNCODE		26	// ���񵹷�����
#define MK_HAWKEYE		28	// hawkeye�� ��뿩�� Ȯ��
#define MK_INPUTOVER		29	// �����Ǵ� �����ڵ� �Է��ϱ�
#define MK_SYNC			30	// get critical section
#define MK_RECVDATA		31	// data�� �޾��� �� �޽���
#define MK_GETMAX		32	// get max data
#define MK_RTSDATA		33	// rtsdata
#define MK_HIDECODE		34	// hide code search combo
#define MK_KILLFOCUS		35	// kill focus
#define MK_ENDDRAG		36	// end drag
#define MK_INSERTROW		37	
#define MK_DELETEROW		38	
#define MK_SETUPOK		39	// ���� ����
#define MK_SAVE			40	// ���ɱ׷� ����
#define MK_SELINT		41	// ���ɱ׷� ����
#define MK_SELFIELD		42	// �ʵ� ����
#define MK_SEARCHOPT		43	// get search option
#define MK_CHANGEFIELD		44	// Change Field
#define MK_TOOLSIZE		45	// toolbar size
#define MK_CHART		46	// ��Ʈ ��뿩��
#define MK_GETDATAKIND		47	
#define MK_GETID		48	// user id
#define MK_NEWS			50	// ����
#define MK_MEMO			51	// �޸�
#define MK_PROCDLL		52	// ���ڽſ��� procdll�� �����°��� ������.
#define MK_SISECATCH		53	// �ü�����
#define MK_GROUPHEIGHT		54
#define MK_HSCROLL			55	// ADD PSH 20070911
#define MK_MKPROP			56	// ADD PSH 20070917
#define MK_GETMOVECFG		57	// ADD PSH 20070917
#define MK_SETMOVECFG		58	// ADD PSH 20070917
#define MK_GETMOVESAVE		59	// ADD PSH 20070917
#define MK_SETMOVESAVE		60	// ADD PSH 20070917
#define MK_TOOLBUTTON		61	// ADD PSH 20070917
#define MK_SORTBASE			62	// ADD PSH 20070917
#define MK_AUTOSAVE			63	// ADD PSH 20070917
#define MK_ALERTWAVE		64	// ADD PSH 20070917
#define MK_MARKETCOLOR		65	// ADD PSH 20070917
#define MK_MARKETCODE		66	// ADD PSH 20070917

// parser
#define SYMBOL_GRID		"$90303"
#define CH_DEL			0x7f
#define CH_TAB			char(0x09)
#define CH_LF			0x0A

#define BTN_WIDTH_BASE      12
#define BTN_WIDTH_STEP      13

#define HEIGHT_TAB				24

const UINT RMSG_GRIDSCROLLEND =	::RegisterWindowMessage("RMSG_GRIDSCROLLEND");

enum {
	CODE_JONGMOK = 1,
	CODE_KJONGMOK,
};

// ����ó ��ġ(0: ��, 1: ����, 2: ����)
enum {
	POS_LEFT,
	POS_RIGHT,
	POS_HIDE,
};

enum {
	MOVE_UP,
	MOVE_DOWN,
};


#endif
