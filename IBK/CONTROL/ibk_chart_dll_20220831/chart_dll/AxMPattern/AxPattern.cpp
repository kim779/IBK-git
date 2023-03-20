#include "stdafx.h"
#include "AxPattern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* _patternNames[] = 
	{"",
/* ��¹�����	*/
		"����ġ�� ", "�����ġ�� ", "�Ҹ�����ƮȦ�� ", "�������� ", "��������� ", 
		"�϶����������� ", "��������� ", "������ ", "������ ", "���������� ", 
		"�����庣�̺� ", "���� �λ��̵�� ", "���� �ƿ����̵�� ", "�϶������ ", "�����������Ÿ ",
		"�ٴ������� ",
/* ���������	*/
		"����ȭ��Ʈ���� ", "���꽺�Ҷ� ", "��������� ", "�����̵��½�Ű ", "�����̵尸���� ",
/* �϶�������	*/
		"������ġ�� ", "�������ƮȦ�� ", "�϶������ ", "�϶������� ", "��½��������� ", 
		"�϶������� ", "�Ա����� ", "������ ", "���������� ", "�����庣�̺� ", 
		"�����λ��̵�ٿ� ", "�����ƿ����̵�ٿ� ", "��±���� ", "�����������Ÿ ", "õ�������� ",
/* �϶�������	*/
		"������ũ�ο��� ", "���̵�Ƽ�þ���ũ���� ", "�϶����İ��� ", "�ٿ���̵尸���� ", "��������ġ�� "
	};

const char* _patternNames_eng[] = 
	{"",	
/* ��¹����� - 16*/
		"InvertedHammer", "Hammer", "BullishBeltHold", "BullishEngulfing", "Harami",
		"HaramiCross", "BullishCounterAttckLine", "PiercingLine", "MorningStar", "MorningDojiStar",
		"AbandonedBaby", "ThreeInsideUp", "ThreeOutsideUp", "UpsideGap2CrowsDown", "BullishDojiStar",
		"TweezersBottom",
/* ��������� - 5	*/
		"ThreeWhiteSoldier", "AdvancedBlock", "StalledPattern", "UpsideGapTasuki", "UpsideGap3Method",
/* �϶������� - 15	*/
		"HangingMan", "BearishBeltHold", "BearishEngulfing", "HaramiBlack", "HaramiCrossBlack",
		"BearishCounterAttckLine", "DarkCrowsCover", "EveningStar", "EveningDojiStar", "AbandonedBabyBlack",
		"ThreeInsideDown", "ThreeOutsideDown", "UpsideGap2Crows", "BearishDojiStar", "TweezersTop",
/* �϶������� - 5	*/
		"ThreeBlackCrows", "IdenticalThreeCrows", "DownsideGapTasuki", "DownsideGap3Method", "ShootingStar"
	};

CAxPattern::CAxPattern(LPCTSTR root)
{
	ZeroMemory(&m_bar, sizeof(ctBars));
	ZeroMemory(&m_indicators, sizeof(ctArray));
	ZeroMemory(&m_info, sizeof(ctCandleInfo));

	m_root = root;
	if (!m_root.IsEmpty() && m_root[m_root.GetLength() - 1] != '\\')
		m_root += '\\';

	LoadPatternInformation();
	InitCandleInfo();
}

CAxPattern::~CAxPattern()
{
	ClearData();
	ClearInfo();
}

void CAxPattern::ClearData()
{
	m_data.SetSize(0);

	FreeBar();
}

void CAxPattern::ClearInfo()
{
	PATTERNINFO	*pinfo;
	int	Key;
	for (POSITION pos = m_patternInfos.GetStartPosition(); pos; )
	{
		m_patternInfos.GetNextAssoc(pos, Key, pinfo);
		delete pinfo;
		pinfo = NULL;
	}

	m_patternInfos.RemoveAll();
}

void CAxPattern::AddData(DATA& data)
{
	m_data.Add(data);
}

void CAxPattern::AddData(LPDATA pd, int count)
{
	for (int i = 0; i < count; i++) 
		m_data.Add(pd[i]);
}

int CAxPattern::GetCount()
{
	return m_data.GetSize();
}


void CAxPattern::InitCandleInfo()
{
	m_info.largeshadow = 2;
	m_info.smallshadow = 0.75;
	m_info.equalpct    = (float)0.00005;
}

int CAxPattern::Data2Bar()
{
	FreeBar();

	int i = 0, err = 0;
	int count;

	CString str;

	if (m_data.GetSize() == 0) return -2;

	count = m_data.GetSize();
	
	err = l_AllocBars(&m_bar, count);
	
	m_bar.type = ctTXT;
	m_bar.reccnt = count;
	m_bar.size   = count;

	strcpy_s(m_bar.description, sizeof(m_bar.description), m_code);
	strcpy_s(m_bar.symbol, sizeof(m_bar.symbol), m_code);
	memset(m_bar.cusip, 0x00, 12);
	m_bar.begindate = 0;
	m_bar.enddate = 0;
	m_bar.frequency = 0;
	m_bar.datatype = 0;
	m_bar.optiontype = 0;
	m_bar.deliverydate = 0;
	m_bar.strikeprice = 0;

	for (i = 0; i < count; i++) {
		str.Format("%04d/%02d/%02d", m_data[i].date.yy, m_data[i].date.mm, m_data[i].date.dd);

		ctArrayItem(m_bar.dt, i) = l_DateTxtToJulian((LPSTR)(LPCTSTR)str, 2);
		ctArrayItem(m_bar.ti, i) = (m_data[i].time.hh * 1800 + m_data[i].time.mm * 30 + m_data[i].time.ss / 2);
		ctArrayItem(m_bar.op, i) = m_data[i].open;
		ctArrayItem(m_bar.hi, i) = m_data[i].high;
		ctArrayItem(m_bar.lo, i) = m_data[i].low;
		ctArrayItem(m_bar.cl, i) = m_data[i].close;
		ctArrayItem(m_bar.vol,i) = m_data[i].volume;
		ctArrayItem(m_bar.oi, i) = m_data[i].xvolume;
	}
	l_ReSizeBars(&m_bar, i);
	return (0);
}

void CAxPattern::FreeBar()
{
	if (m_bar.size) {
		l_FreeBars(&m_bar);
	}

	if (m_indicators.data) {
		l_FreeArray(&m_indicators);
	}

	if (m_info.candle) {
		l_CandleFree(&m_info);
	}

	ZeroMemory(&m_indicators, sizeof(ctArray));
	ZeroMemory(&m_info, sizeof(m_info));
	ZeroMemory(&m_bar, sizeof(m_bar));
}

int CAxPattern::FindPattern(int days, BOOL bBull, BOOL bBear)
{
	if (bBull && bBear)
		return FindPatternAll(days);
	else if (bBull)
		return FindPatternBull(days);

	return FindPatternBear(days);
}

int CAxPattern::FindPatternAll(int days)
{
	if (days == 0)	
		days = m_data.GetSize();

	if (days == 0)	return -2;

	int err = Data2Bar();
	if (err)	return err;

	err = l_AllocArray(&m_indicators, m_bar.size);
	if (err)	return err;

	InitCandleInfo();

	err = l_CandleConstruct(&m_bar, &m_info);
	if (err)	return err;

	int start = 0, stop = m_bar.size - 1;
	
	l_CandleFill(&m_bar, &m_info, &m_indicators, "l_BeltHoldLineBull", start, stop);

	for (int pos = 0; pos <= stop; pos++) 
	{
		if (n_InvertedHammer(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = InvertedHammer;
		else if (n_Hammer(&m_bar, &m_info, pos) != 0)				m_data[pos].pattern = Hammer;
		else if (n_BullishBeltHold(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BullishBeltHold;
		else if (n_BullishEngulfing(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BullishEngulfing;
		else if (n_Harami(&m_bar, &m_info, pos) != 0)				m_data[pos].pattern = Harami;
		else if (n_HaramiCross(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = HaramiCross;
		else if (n_BullishCounterAttckLine(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = BullishCounterAttckLine;
		else if (n_PiercingLine(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = PiercingLine;
		else if (n_MorningStar(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = MorningStar;
		else if (n_MorningDojiStar(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = MorningDojiStar;
		else if (n_AbandonedBaby(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = AbandonedBaby;
		else if (n_ThreeInsideUp(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = ThreeInsideUp;
		else if (n_ThreeOutsideUp(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = ThreeOutsideUp;
		else if (n_UpsideGap2CrowsDown(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = UpsideGap2CrowsDown;
		else if (n_BullishDojiStar(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BullishDojiStar;
		else if (n_TweezersBottom(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = TweezersBottom;
		else if (n_ThreeWhiteSoldier(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = ThreeWhiteSoldier;
		else if (n_AdvancedBlock(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = AdvancedBlock;
		else if (n_StalledPattern(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = StalledPattern;
		else if (n_UpsideGapTasuki(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = UpsideGapTasuki;
		else if (n_UpsideGap3Method(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = UpsideGap3Method;
		else if (n_HangingMan(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = HangingMan;
		else if (n_BearishBeltHold(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BearishBeltHold;
		else if (n_BearishEngulfing(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BearishEngulfing;
		else if (n_HaramiBlack(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = HaramiBlack;
		else if (n_HaramiCrossBlack(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = HaramiCrossBlack;
		else if (n_BearishCounterAttckLine(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = BearishCounterAttckLine;
		else if (n_DarkCrowsCover(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = DarkCrowsCover;
		else if (n_EveningStar(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = EveningStar;
		else if (n_EveningDojiStar(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = EveningDojiStar;
		else if (n_AbandonedBabyBlack(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = AbandonedBabyBlack;
		else if (n_ThreeInsideDown(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = ThreeInsideDown;
		else if (n_ThreeOutsideDown(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = ThreeOutsideDown;
		else if (n_UpsideGap2Crows(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = UpsideGap2Crows;
		else if (n_BearishDojiStar(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BearishDojiStar;
		else if (n_TweezersTop(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = TweezersTop;
		else if (n_ThreeBlackCrows(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = ThreeBlackCrows;
		else if (n_IdenticalThreeCrows(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = IdenticalThreeCrows;
		else if (n_DownsideGapTasuki(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = DownsideGapTasuki;
		else if (n_DownsideGap3Method(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = DownsideGap3Method;
		else if (n_ShootingStar(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = ShootingStar;
		else
			m_data[pos].pattern = 0;
	}

	return 0;
}

int CAxPattern::FindPatternBull(int days)
{
	if (days == 0)	
		days = m_data.GetSize();

	if (days == 0)	return -2;

	int err = Data2Bar();
	if (err)	return err;

	err = l_AllocArray(&m_indicators, m_bar.size);
	if (err)	return err;

	InitCandleInfo();

	err = l_CandleConstruct(&m_bar, &m_info);
	if (err)	return err;

	int start = 0, stop = m_bar.size - 1;
	
	l_CandleFill(&m_bar, &m_info, &m_indicators, "l_BeltHoldLineBull", start, stop);

	for (int pos = 0; pos <= stop; pos++) 
	{
		if (n_InvertedHammer(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = InvertedHammer;
		else if (n_Hammer(&m_bar, &m_info, pos) != 0)				m_data[pos].pattern = Hammer;
		else if (n_BullishBeltHold(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BullishBeltHold;
		else if (n_BullishEngulfing(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BullishEngulfing;
		else if (n_Harami(&m_bar, &m_info, pos) != 0)				m_data[pos].pattern = Harami;
		else if (n_HaramiCross(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = HaramiCross;
		else if (n_BullishCounterAttckLine(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = BullishCounterAttckLine;
		else if (n_PiercingLine(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = PiercingLine;
		else if (n_MorningStar(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = MorningStar;
		else if (n_MorningDojiStar(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = MorningDojiStar;
		else if (n_AbandonedBaby(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = AbandonedBaby;
		else if (n_ThreeInsideUp(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = ThreeInsideUp;
		else if (n_ThreeOutsideUp(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = ThreeOutsideUp;
		else if (n_UpsideGap2CrowsDown(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = UpsideGap2CrowsDown;
		else if (n_BullishDojiStar(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BullishDojiStar;
		else if (n_TweezersBottom(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = TweezersBottom;
		else if (n_ThreeWhiteSoldier(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = ThreeWhiteSoldier;
		else if (n_AdvancedBlock(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = AdvancedBlock;
		else if (n_StalledPattern(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = StalledPattern;
		else if (n_UpsideGapTasuki(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = UpsideGapTasuki;
		else if (n_UpsideGap3Method(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = UpsideGap3Method;
		else
			m_data[pos].pattern = 0;
	}

	return 0;
}

int CAxPattern::FindPatternBear(int days)
{
	if (days == 0)	
		days = m_data.GetSize();

	if (days == 0)	return -2;

	int err = Data2Bar();
	if (err)	return err;

	err = l_AllocArray(&m_indicators, m_bar.size);
	if (err)	return err;

	InitCandleInfo();

	err = l_CandleConstruct(&m_bar, &m_info);
	if (err)	return err;

	int start = 0, stop = m_bar.size - 1;
	
	l_CandleFill(&m_bar, &m_info, &m_indicators, "l_BeltHoldLineBull", start, stop);

	for (int pos = 0; pos <= stop; pos++) 
	{
		if (n_HangingMan(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = HangingMan;
		else if (n_BearishBeltHold(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BearishBeltHold;
		else if (n_BearishEngulfing(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BearishEngulfing;
		else if (n_HaramiBlack(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = HaramiBlack;
		else if (n_HaramiCrossBlack(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = HaramiCrossBlack;
		else if (n_BearishCounterAttckLine(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = BearishCounterAttckLine;
		else if (n_DarkCrowsCover(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = DarkCrowsCover;
		else if (n_EveningStar(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = EveningStar;
		else if (n_EveningDojiStar(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = EveningDojiStar;
		else if (n_AbandonedBabyBlack(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = AbandonedBabyBlack;
		else if (n_ThreeInsideDown(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = ThreeInsideDown;
		else if (n_ThreeOutsideDown(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = ThreeOutsideDown;
		else if (n_UpsideGap2Crows(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = UpsideGap2Crows;
		else if (n_BearishDojiStar(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = BearishDojiStar;
		else if (n_TweezersTop(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = TweezersTop;
		else if (n_ThreeBlackCrows(&m_bar, &m_info, pos) != 0)		m_data[pos].pattern = ThreeBlackCrows;
		else if (n_IdenticalThreeCrows(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = IdenticalThreeCrows;
		else if (n_DownsideGapTasuki(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = DownsideGapTasuki;
		else if (n_DownsideGap3Method(&m_bar, &m_info, pos) != 0)	m_data[pos].pattern = DownsideGap3Method;
		else if (n_ShootingStar(&m_bar, &m_info, pos) != 0)			m_data[pos].pattern = ShootingStar;
		else
			m_data[pos].pattern = 0;
	}

	return 0;
}

LPDATA CAxPattern::GetData(int index)
{
	if (index >= 0 && index < m_data.GetSize())
		return &m_data[index];
	else
		return NULL;
}

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])
#endif

LPCTSTR CAxPattern::GetPatternName(int pattern)
{
	if (pattern >= 0 && pattern < ARRAY_SIZE(_patternNames)) 
		return _patternNames[pattern];
	else
		return "";
}

LPCTSTR CAxPattern::GetPatternNameEng(int pattern)
{
	if (pattern >= 0 && pattern < ARRAY_SIZE(_patternNames_eng))
		return _patternNames_eng[pattern];
	else
		return "";
}


void CAxPattern::LoadPatternInformation()
{
	CString fileName;

	fileName.Format(_T("%stab\\patterndesc.txt"), LPCTSTR(m_root));

	CFile* pFile;
	TRY 
	{
		pFile = new CFile(fileName, CFile::modeRead);
	}
	CATCH (CFileException, e) 
	{
		return;
	}
	END_CATCH;


	UINT fileL = static_cast<UINT>(pFile->GetLength());
	LPSTR buffer = new char[fileL + 1];

	pFile->Read(buffer, fileL);
	buffer[fileL] = 0x00;

	delete pFile;
	
	CString str = buffer;
	CString pattern;
	delete [] buffer;

	LPPATTERNINFO p;

	for (int i = 1; i < sizeof(_patternNames_eng) / sizeof(_patternNames_eng[0]); i++) 
	{

		p = new PATTERNINFO;
		
		p->alias = _patternNames_eng[i];

		pattern = GetTagString(str, p->alias);
		p->description = GetTagString(pattern, "description");

		if (!p->description.IsEmpty()) 
		{
			m_patternInfos.SetAt(i, p);
		}
		else 
		{
			delete p;
		}
	}
}

CString CAxPattern::GetTagString(CString buffer, LPCTSTR tag)
{
	CString tag1, tag2;

	tag1.Format("<%s>", tag);
	tag2.Format("</%s>", tag);

	int find1 = buffer.Find(tag1);
	int find2 = buffer.Find(tag2);

	if (find1 < 0 || find2 < 0) 
		return CString("");


	CString str;

	str = buffer.Mid(find1 + tag1.GetLength(), find2 - (find1 + tag1.GetLength()));

	str.TrimLeft();
	str.TrimRight();

	return str;
}


LPCTSTR CAxPattern::GetDescription(int pattern)
{
	LPPATTERNINFO p;

	if (m_patternInfos.Lookup(pattern, p)) 
	{
		return p->description;
	}
	else 
	{
		return NULL;
	}
}

LPCTSTR CAxPattern::GetPatternImage(int pattern)
{
	CString patternName = GetPatternNameEng(pattern);
	if (patternName.IsEmpty())
		return "";

	patternName.Replace(" ", "");
	CString fileName;
	fileName.Format(_T("%sImage\\GTP_%s.bmp"), LPCTSTR(m_root), LPCTSTR(patternName));
	static char buffer[MAX_PATH];
	strcpy_s(buffer, sizeof(buffer), fileName);

	return buffer;
}

