#pragma once

// CPeriodObj ��� ����Դϴ�.

// Ŭ���� ��ġ�� ��� ������ ���ԵǴ� ��
#define N_PT		0	// No point
#define S_PT		1	// Start point
#define E_PT		2	// End point
#define M_PT		4	// Mid point
#define C1_PT		8	// coner1 point
#define C2_PT		16	// coner2 point
#define ON_PT		31	// �� ��

#define OFFSET_RC_IN		4  // ��ü RC�� �׸��� RC���� offset
#define OFFSET_LT			20 // ���ΰ� ���̺� ������ �⺻(�ּ�) offset
#define OFFSET_TT_IN		5	// ���̺�� �ؽ�Ʈ ������ �⺻(�ּ�) offset
#define RADIUS_OF_END_RC	2// ������ �� ������ �� ���� ������
#define DEFAULT_HEIGHT		20 // pixels, ���� �⺻ ����
#define TEXT_HEIGHT			15	// ���� ������ ����
#define TEXT_WIDTH			8	// ���� ������ �� 
#define TEXT_RC_OFFSET	CPoint(30,30)	// �ؽ�Ʈ�� �⺻ ��ġ
#define WH_OF_MID_RC		5// �߰����� ǥ���� ���簢���� ��/����
#define UP					-1	// ������
#define DOWN				1	// �Ʒ�����
#define MAX_NUM_OF_ROW_OF_TABLE	10	// ���̺��� �ִ� �� ��
#define LINE_ON_SENSITIVITY	8	// ������ ���콺 on���� ���� �ΰ���

// 20081009 �ڵ��� >>
class CChartPeriodSummaryAddInImp;
// 20081009 �ڵ��� <<
class CPeriodObj : public CObject
{
public:
	CPeriodObj();
	virtual ~CPeriodObj();

	//////////////////////////////////////////////////////////////////////////
	// ���� ���� ���� Ŭ����
	class CPrice
	{
	public:
		double sPrice;	// ������ ����
		double ePrice;	// ������ ����
		double max;		// ��
		double min;		// ��
		double avg;		// ���
		void CalcAvg(int nCnt, double* dData)
		{
			for(int i=0; i<nCnt;i++)
				avg += dData[i];
			if(nCnt)
				avg /= nCnt;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// �ŷ��� ���� ���� Ŭ����
	class CAmount
	{
	public:
		double max;		// ��
		double min;		// ��
		double avg;		// ���
		void CalcAvg(int nCnt, double* dData)
		{
			for(int i=0; i<nCnt;i++)
				avg += dData[i];
			if(nCnt)
				avg /= nCnt;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// �� ���� ���� Ŭ����
	class CCandle
	{
	public:
		int count;		// ����
		int plus;		// ��
		int minus;		// ��
		double ratio; 	// ���� %
		double CalcRatio()
		{
			if(!minus) ratio = 100.;
			else ratio = (double)plus/(double)minus*100.;
			return ratio;
		};
		double CalRatio(int nPlus, int nMinus)
		{		
			if(nMinus)
				return nPlus/nMinus*1.;
			return 1.;
		};

	 };

	public:
	BOOL m_bSaved;			// ����� �� �ҷ��� ������ ����
	double m_dEClicked;		// ���� ��
	double m_dSClicked;		// ������ ��
	int m_nUpDownSymbol;	// ��/�ٿ� �ɺ�
	int m_nDateType;		// ���� ����
	BOOL m_bSelected;		// ���� ����
	int m_Num;				// ������Ʈ ����, �� ��°....

	CRect m_rc;				// ��ü ����
	CRect m_rcLine;			// ���� ����
	CRect m_rcTable;		// ���̺� ����
	CRect m_rcText;			// �ؽ�Ʈ ����
	CRect m_rcCut;			// ���� ������ ���� ���ſ�
	CRect m_rcCutS;			// ���� ������ ���� ���ſ�
	CRect m_rcCutE;			// ���� ������ ���� ���ſ�
	
	CPoint m_ptAOffset;		// ��ü ������ ���� offset
	CPoint m_ptRLineOffset;	// ���� ������ ��ü ���� ��� offset
	CPoint m_ptALineOffset;	// ���� ������ ���� offset = ���� ������ ��ü ���� ��� offset + ��ü ������ ���� offset
	CPoint m_ptRTableOffset;// ���̺� ������ ��ü ���� ��� offset
	CPoint m_ptATableOffset;// ���̺� ������ ���� offset = �ؽ�Ʈ ������ ��ü ���� ��� offset + ��ü ������ ���� offset
	CPoint m_ptRTextOffset;	// �ؽ�Ʈ ������ ��ü ���� ��� offset
	CPoint m_ptATextOffset;	// �ؽ�Ʈ ������ ���� offset = �ؽ�Ʈ ������ ��ü ���� ��� offset + ��ü ������ ���� offset
	CPoint m_ptLTOffset;	// ���̺� ������ ���� ������� offset

	CRect m_oldrc;			// ���� ���
	CPoint m_ptAStart;		// ������ ���� ��ġ
	CPoint m_ptAEnd;		// ���� ���� ��ġ
	CPoint m_ptRStart;		// ������ ��� ��ġ
	CPoint m_ptREnd;		// ���� ��� ��ġ
	CPoint m_ptAt;

	int m_CurClickedPosition;	//Ŭ���� ��ġ

	// �ؽ�Ʈ ȭ�� ǥ�� ����
	int m_nRowCnt;				// �� ��
	BOOL m_bSEPrice;			// ������ ����
	BOOL m_bEEPrice;			// ������ ����
	BOOL m_bHPrice;				// ��
	BOOL m_bLPrice;				// ����
	BOOL m_bMPrice;				// ��հ�
	BOOL m_bHAmount;			//�ִ� �ŷ���
	BOOL m_bLAmount;			// �ּ� �ŷ���
	BOOL m_bMAmount;			// ��� �ŷ���
	BOOL m_bPCNum;				//��� ��
	BOOL m_bMCNum;				// ���� ��
	BOOL m_bPMRatio;			//������ ����

	int m_nTextAlignment;		// �ؽ�Ʈ ����
	//////////////////////////////////////////////////////////////////////////
	// �� �Ӽ�
	int m_nLineThickness;		// �� ����
	int m_nLineStyle;			// �� ��Ÿ��
	COLORREF m_clrLineColor;	// �� ��

	//////////////////////////////////////////////////////////////////////////
	// ������ ����
	//////////////////////////////////////////////////////////////////////////
	int m_nStartIndex;			// ���� �ε���
	int m_nEndIndex;			// �� �ε���
	CString m_strSDate;			// ��������
	CString m_strEDate;			// ��������
	
	// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
	//	long m_lSDate;				// ��������
	//	long m_lEDate;				// ��������
	__int64 m_lSDate;				// ��������
	__int64 m_lEDate;				// ��������
	// 2011.01.06 by SYS <<
	
	CString m_strPeriod;		// �Ⱓ	(2nd raw)
	CString m_strBasicInfo1;	// �⺻ ����(1st raw)
	CString m_strBasicInfo2;	// �⺻ ����(1st raw)
	CString m_strBasicInfo3;	// �⺻ ����(1st raw)
	CPrice m_Price;				// ����
	CAmount m_Amount;			// �ŷ���
	CCandle m_Candle;			// ��
	int m_nTimeDiff;			// ����ڼ��� �ð���	// 20081007 JS.Kim	����ڽð��� ����

	// 20081009 �ڵ��� �θ��� �����͸� ���� >>
	void							SetChartPeriodSummaryAddInImp( CChartPeriodSummaryAddInImp *pChartPeriodSummaryAddInImp);
	CChartPeriodSummaryAddInImp *	GetChartPeriodSummaryAddInImp( void);
	CString							GetPacketType( void);
	// 20081009 �ڵ��� <<
	
	// 2011.01.24 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CString							GetPacketDateType(void);	// ���� �ֱ��� ��¥�ð� Type�� ��´�.
	int								GetPacketDateUnit(void);	// �ֱ������� ��´�.
	// 2011.01.24 by SYS <<

	int IsInRegion(CPoint pt);		// ���콺 �����Ͱ� �ش� ������ ���ϴ��� ����. ��(IsOnLine)/����(IsOnEndPoint) �������� ������ 
	int IsOnLine(CPoint pt);		// ���콺 �����Ͱ� �� ���� �ִ� �� ����
	BOOL IsOnEndPoint(CPoint pt);	// ���콺 �����Ͱ� �� ���� �ִ� �� ����
	void ButtonDownAt(const CPoint& pt);	// ��ġ ����
	void ButtonUpAt(const CPoint& pt);		// ��ġ ����
	void SetPos(CPoint pt, int type);		// ��ġ ����	
	CPoint GetPos(int type);				// ��ġ ���
	void Draw(CDC* pDC, HWND p_hOcxWnd);			// ���׸���
	void DrawEndPoints(CDC* pDC);	// ����(����, ���� ��) �׸���
	void SetUpDown(int nUpDown){m_nUpDown = nUpDown;}
	void DrawTable(CDC* pDC);		// �ڽ� �� �ؽ�Ʈ ���� �׸���
	int m_nUpDown;
private:
	CSize m_size;
	void MakeData( HWND p_hOcxWnd);	// �⺻ ������(ĵ�� ��, ����, �Ⱓ) ���
	CString CommaStr(CString in);	// ���� ��Ʈ���� �޸� �ֱ�
	CRect GetRect();						// ȭ�� ǥ�ø� ���� ���� ���� ����, �׸��� ���� ���

	// 20081009 �ڵ��� >>
	CChartPeriodSummaryAddInImp *	m_pChartPeriodSummaryAddInImp; // �θ������͸� ����
	// 20081009 �ڵ��� <<

	bool GetPacketTypeInfo(int& nDecimalDownCount, CString& strType) const;
	CString GetText_Price( double dPrice, const CString &p_strPacketType, int nDecimalDownCount) const;

// (2008/11/5 - Seung-Won, Bae) for Multiple Item.
public:
	CString		m_strRQ;
	int			m_nRow;
	int			m_nCol;
	CRect		m_rctGraphRegion;
	CRect		m_rctBlockRegion;
	BOOL		m_bLog;
	BOOL		m_bReverse;
	double		m_dViewMin;
	double		m_dViewMax;
	int			m_nStartIndexOfView;
	int			m_nEndIndexOfView;
	CPoint		m_pBaseLine;
};
