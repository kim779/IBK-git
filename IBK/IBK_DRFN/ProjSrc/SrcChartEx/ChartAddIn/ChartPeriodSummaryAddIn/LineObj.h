#pragma once

// CLineObj ��� ����Դϴ�.

// Ŭ���� ��ġ�� ��� ������ ���ԵǴ� ��
#define N_PT		0	// No point
#define S_PT		1	// Start point
#define E_PT		2	// End point
#define M_PT		4	// Mid point
#define ON_PT		31	// �� ��

#define OFFSET_RC_IN		4			// ��ü RC�� �׸��� RC���� offset
#define OFFSET_LT			20			// ���ΰ� ���̺� ������ �⺻(�ּ�) offset
#define OFFSET_TT_IN		5			// ���̺�� �ؽ�Ʈ ������ �⺻(�ּ�) offset
#define RADIUS_OF_END_RC	2			// ������ �� ������ �� ���� ������
#define DEFAULT_HEIGHT		20			// pixels, ���� �⺻ ����
#define TEXT_HEIGHT			15			// ���� ������ ����
#define TEXT_WIDTH			8			// ���� ������ �� 
#define TEXT_RC_OFFSET	CPoint(30,30)	// �ؽ�Ʈ�� �⺻ ��ġ
#define WH_OF_MID_RC		5			// �߰����� ǥ���� ���簢���� ��/����
#define UP					-1			// ������
#define DOWN				1			// �Ʒ�����
#define MAX_NUM_OF_ROW_OF_TABLE	10		// ���̺��� �ִ� �� ��
#define LINE_ON_SENSITIVITY	8			// ������ ���콺 on���� ���� �ΰ���
enum{STAT_NONE, STAT_UP, STAT_DOWN};

class CLineObj : public CObject
{
public:
	CLineObj( HWND p_hOcxWnd);
	virtual ~CLineObj();
public:
	void GenerateSerial();
	double m_dSClicked;		// �������� �������� Y ��
	double m_dEClicked;		// �������� �������� Y ��
	int m_nDateType;		// ��, �� �� �� ƽ
	BOOL m_bSelected;		// ���� ����
	int m_Num;				// �Ϸ� ��ȣ

	CRect m_rc;				// ��ü ����
	CRect m_rcLine;			// ���� ����
	CRect m_rcTable;		// ���̺� ����
	CRect m_rcText;			// �ؽ�Ʈ ����
	CRect m_rcCut;			// ���� ������ ���� ���ſ�

	CPoint m_ptAOffset;		// ��ü ������ ���� offset
	CPoint m_ptRLineOffset;	// ���� ������ ��ü ���� ��� offset
	CPoint m_ptALineOffset;	// ���� ������ ���� offset = ���� ������ ��ü ���� ��� offset + ��ü ������ ���� offset
	CPoint m_ptRTableOffset;// ���̺� ������ ��ü ���� ��� offset
	CPoint m_ptATableOffset;// ���̺� ������ ���� offset = �ؽ�Ʈ ������ ��ü ���� ��� offset + ��ü ������ ���� offset
	CPoint m_ptRTextOffset;	// �ؽ�Ʈ ������ ��ü ���� ��� offset
	CPoint m_ptATextOffset;	// �ؽ�Ʈ ������ ���� offset = �ؽ�Ʈ ������ ��ü ���� ��� offset + ��ü ������ ���� offset
	CPoint m_ptLTOffset;	// ���̺� ������ ���� ������� offset

	CRect m_oldrc;			// ���� ���
	CRect m_rcDraw;			// �׸��� ���� (text ���� �� Line ����)
	CPoint m_ptAStart;		// ������ ���� ��ġ
	CPoint m_ptAEnd;		// ���� ���� ��ġ
	CPoint m_ptRStart;		// ������ ��� ��ġ
	CPoint m_ptREnd;		// ���� ��� ��ġ
	CPoint m_ptAt;
	CPoint m_ptAExpand;		// Ȯ���Ͽ� �ٿ�忡 ��� ��ġ
	CPoint m_ptAIndexedStart;	// �ε����� �ش��ϴ� ��ġ
	CPoint m_ptAIndexedEnd;		// �ε����� �ش��ϴ� ��ġ

	int m_CurClickedPosition;	//Ŭ���� ��ġ

	CString strType;		//packet type;

	//ȭ�� ǥ�� �ؽ�Ʈ
	int m_nRowCnt;			// �� ��
	int m_nTextAlignment;	// �ؽ�Ʈ ����
	//////////////////////////////////////////////////////////////////////////
	// �� �Ӽ�
	//////////////////////////////////////////////////////////////////////////
	int m_nLineThickness;	// �� ����
	int m_nLineStyle;		// �� ��Ÿ��
	COLORREF m_clrLineColor;// �� ��

	//////////////////////////////////////////////////////////////////////////
	// ������ ����
	//////////////////////////////////////////////////////////////////////////
	CString m_strSerial;	// �����ڵ�+���غ�+ó��������¥, ��)05930��20070215165013
	long m_lCode;			// ���� �ڵ�
	CString m_strCode;		// ���� �ڵ�
	CString m_strName;		// �����
	CString m_strCandle;		// ���غ�

	int m_nSIndex;		// ������ �ε���
	int m_nEIndex;		// ������ �ε���

	CString m_strSDate;		// ��������
	CString m_strEDate;		// ��������

	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//	long m_lSDate;			// ��������
	//	long m_lEDate;			// ��������
	__int64 m_lSDate;			// ��������
	__int64 m_lEDate;			// ��������
	// 2011.01.06 by SYS <<


	double m_dSData;		// ������ ����
	double m_dEData;		// ������ ����

	CString m_strOrderFlag;	// �ι����� ǥ�� ��,��,��,��

	// 20081007 JS.Kim	����ڽð��� ����
	int		m_nTimeDiff;	// ���� �ð���

	//////////////////////////////////////////////////////////////////////////
	// ȭ�� ǥ�� �׸�
	//////////////////////////////////////////////////////////////////////////
	BOOL m_bUpSell;		// ��� �ŵ�
	BOOL m_bUpBuy;		// ��� �ż�
	BOOL m_bDownSell;	// �϶� �ŵ�
	BOOL m_bDownBuy;	// �϶� �ż�

	//////////////////////////////////////////////////////////////////////////
	// Alert ���� �׸�
	//////////////////////////////////////////////////////////////////////////
	int m_nState;		//Alert ����
	double m_dAlertPrice;	//Alert ����

	//////////////////////////////////////////////////////////////////////////
	// �Լ�
	//////////////////////////////////////////////////////////////////////////
	void SetInfo();
	void DrawExpand(CDC* pDC);							// ������� ����
	CString CommaStr(CString in);						// ������� ����
	int IsInRegion(CPoint pt);							
	int IsOnLine(CPoint pt);
	BOOL IsOnEndPoint(CPoint pt);
	void Initilize(void);								// �ʱ�ȭ - ������� ����
	void ButtonDownAt(const CPoint& pt);				// ��ư Ŭ���ٿ� ��ġ ����
	void ButtonUpAt(const CPoint& pt);					// ��ư Ŭ���� ��ġ ����
	CRect GetRect();									// ���� �� offset ���
	void SetPos(CPoint pt, int type);					// Ÿ�Կ� ���� ������ ����
	CPoint GetPos(int type);							// Ÿ�Կ� ���� ������ ���
	CRect	Draw( CDC* pDC);								// �� �׸���
	void DrawEndPoints(CDC* pDC);						// ���� �׸���
	void DrawTable(CDC* pDC);							// �ؽ�Ʈ ǥ��
	void Erase(CDC* pDC);								// ������� ����
	void SetUpDown(int nUpDown){m_nUpDown = nUpDown;}	// ������� ����
private:
	CSize m_size;	// �׸��� ���� ũ��
	int m_nUpDown;	// ������� ����

	void ReCalcRect();	// ���� �� ���
	void MakeData(void);

// (2008/3/4 - Seung-Won, Bae) for Multi-Language
protected:
	HWND		m_hOcxWnd;
	CString		m_strStar;
};
