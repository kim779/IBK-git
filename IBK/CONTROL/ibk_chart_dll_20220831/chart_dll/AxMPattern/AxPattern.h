#ifndef __AXPATTERN_H__
#define __AXPATTERN_H__

#include <afxtempl.h>


typedef struct {
	short yy;
	short mm;
	short dd;
} YMD;

typedef struct {
	short hh;
	short mm;
	short ss;
} HMS;


#define ARR_UP	0x01
#define ARR_DN	0x02

typedef struct {
	YMD	  date;		// ����
	HMS	  time;		// �ð�
	float open;		// �ð�
	float high;		// ��
	float low;		// ����
	float close;		// ����
	char  sign;		// ��񱸺� +:��� -:�϶�
	float diff;		// ���
	float volume;		// ü�ᷮ
	float xvolume;		// �����ŷ���

	int	  pattern;	// 0 = no pattern, other = pattern found.
	RECT  rc;		// draw pattern rect, if pattern is 0 then invalid.
	BYTE  arrow;		// ARR_UP or ARR_DN
	BYTE  draw;		// 0 = not drawing, other = drawing
}	DATA, *LPDATA;

typedef struct {
	CString alias;
	CString description;
}   PATTERNINFO, *LPPATTERNINFO;

#define MIN_COUNT	5

enum
{	/* ��¹����� - 16*/
	InvertedHammer = 1,	Hammer, BullishBeltHold, BullishEngulfing, Harami,
	HaramiCross, BullishCounterAttckLine, PiercingLine, MorningStar, MorningDojiStar,
	AbandonedBaby, ThreeInsideUp, ThreeOutsideUp, UpsideGap2CrowsDown, BullishDojiStar,
	TweezersBottom,

	/* ��������� - 5	*/
	ThreeWhiteSoldier, AdvancedBlock, StalledPattern, UpsideGapTasuki, UpsideGap3Method,

	/* �϶������� - 15	*/
	HangingMan, BearishBeltHold, BearishEngulfing, HaramiBlack, HaramiCrossBlack,
	BearishCounterAttckLine, DarkCrowsCover, EveningStar, EveningDojiStar, AbandonedBabyBlack,
	ThreeInsideDown, ThreeOutsideDown, UpsideGap2Crows, BearishDojiStar, TweezersTop,

	/* �϶������� - 5	*/
	ThreeBlackCrows, IdenticalThreeCrows, DownsideGapTasuki, DownsideGap3Method, ShootingStar
};

class AFX_EXT_CLASS CAxPattern
{
public:
	CAxPattern(LPCTSTR root);
	virtual ~CAxPattern();
private:
protected:
	CString	m_code;
	CString m_date;

	CString m_root;
	

	//** candle information
	ctCandleInfo	m_info;
	void			InitCandleInfo();	

	//** bar data
	ctBars			m_bar;
	ctArray			m_indicators;

	//** data
	CArray<DATA, DATA> m_data;

	//** bar function
	int	Data2Bar();
	void	FreeBar();
//	int	ResizeBar();

	//** pattern information
	void	LoadPatternInformation();
	CMap <int, int, LPPATTERNINFO, LPPATTERNINFO> m_patternInfos;

	CString GetTagString(CString buffer, LPCTSTR tag);
public:
	void	ClearData();
	void	ClearInfo();
	void	AddData(LPDATA	pd, int count);
	void	AddData(DATA& data);

	int	GetCount();
	int	FindPattern(int days, BOOL bBull, BOOL bBear);
	int	FindPatternAll(int days);
	int	FindPatternBull(int days);
	int	FindPatternBear(int days);
	LPDATA  GetData(int index);

	LPCTSTR GetPatternName(int pattern);
	LPCTSTR GetPatternNameEng(int pattern);

	LPCTSTR GetDescription(int pattern);
	LPCTSTR GetPatternImage(int pattern);
};

#endif // __AXPATTERN_H__