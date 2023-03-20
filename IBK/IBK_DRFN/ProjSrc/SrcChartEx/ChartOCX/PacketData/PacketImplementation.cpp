// PacketImplementation.cpp: implementation of the CPacketImplementation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketImplementation.h"

#include <math.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "FLOAT.H"
#include "PacketList.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPacketImplementation::CPacketImplementation(CPacket* pPacket,
											 CPacketList* pParentList, CString strName,
											 CString strFieldName, CString strType,
											 CString szExpressionType, int nDataLength, 
											 bool bUseReal, int nStartPeriod, 
											 CString strRealFieldName):
	m_pPacket(pPacket),
	m_pParentList(pParentList),
	m_Name(strName),
	m_FieldName(strFieldName),
	m_Type(strType),
	m_ExpressionType(szExpressionType),
	m_nLength(nDataLength),
	m_bUseReal(bUseReal),
	m_nStartPeriod(nStartPeriod),
	m_RealFieldName(strRealFieldName),
	m_lnData(256),
	m_bBaseData(true),
	m_standardValue(false, 0), 
	m_upperBaseValue(false, 0), 
	m_lowerBaseValue(false, 0), 
	m_realDataGap(false, 0),
	m_eCommodityBondName(CPacketType::NONAME),
	m_dMaxBound(-DBL_MAX),
	m_dMinBound(DBL_MAX),
	m_bUseBound(false),
	m_bAcceptFromServer(false),
	m_dExceptionValue( -HUGE_VAL),
	m_dContinueCode( -HUGE_VAL),
	m_strSubName("")				// 20100324_JS.Kim �ַθ�.FX	�ð����� ���� ��Ŷ�� �ð�����ó�� ����ϱ� ���� ó��
{
	// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type.
	m_bAutoScaleType = TRUE;

	// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
	//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
	ClearMinMax();
} 

CPacketImplementation::~CPacketImplementation()
{
	m_lnData.RemoveAll();
	m_lstrData.RemoveAll();
	RemoveAllTempList();
}

CString CPacketImplementation::GetName()
{
	return m_Name ;
}

CString CPacketImplementation::GetFieldName()
{
	return m_FieldName ;
}

//sy 2005.11.08.
bool CPacketImplementation::IsAcceptFromServer() const
{
	return m_bAcceptFromServer;
}

void CPacketImplementation::SetAcceptFromServer(const bool bAcceptFromServer)
{
	m_bAcceptFromServer = bAcceptFromServer;
}
//sy end

void CPacketImplementation::SetRealFieldName(CString strRealFieldName, CString strRealKeyName)
{
	if(!strRealFieldName.IsEmpty())
		m_bUseReal = true ;
	else
		m_bUseReal = false ;

	m_RealFieldName = strRealFieldName ;
	m_RealKeyName = strRealKeyName;
}

int CPacketImplementation::GetCount() const
{
	if(m_Type == _MTEXT( C6_CHARACTER))
		return m_lstrData.GetCount();
	else
		return m_lnData.GetCount();
}

int CPacketImplementation::GetDataCount()
{
	if(m_Type == _MTEXT( C6_CHARACTER))
		return m_lstrData.GetCount() - m_nStartPeriod;
	else
		return m_lnData.GetCount() - m_nStartPeriod;
}

int CPacketImplementation::GetMaxCount()
{
	return m_pParentList->GetMaxCount();
}

void CPacketImplementation::AppendData(double nData, int nIndex/*, BOOL p_bAddPrev*/)
{
	if(m_Type == _MTEXT( C6_CHARACTER))
		return;

	//sy 2003.3.19 -> ������ ���õ� update data�� packet ���� �Ѵ�.
	if(nIndex < 0)
	{
		double dValue = GetRealData_AppendData(nData);
		m_lnData.AddTail(dValue);
		UpdateData( dValue, m_lnData.GetCount() - 1);
	}
	else
	{
		m_lnData.InsertBefore(m_lnData.FindIndex(nIndex), nData);
		ClearMinMax();
	}
}

void CPacketImplementation::AppendData(const CString& strData)
{
	if(m_Type == _MTEXT( C6_CHARACTER))	{
		m_lstrData.AddTail(strData) ;
	}
	else
	{
		double value = atof(strData);
		m_lnData.AddTail(value);
		UpdateMinMax( value, m_lnData.GetCount() - 1);
	}
}

void CPacketImplementation::AppendData(long pData, int nDataLength)
{
	char szData[1024];
	memcpy(szData, (void*)pData, nDataLength) ;
	szData[nDataLength] = NULL ;
	CString strData = szData ;
	if(m_Type == _MTEXT( C6_CHARACTER)){
		m_lstrData.AddTail(strData) ;
	}
	else
	{
		double value = atof(strData);
		m_lnData.AddTail(value) ;
		UpdateMinMax( value, m_lnData.GetCount() - 1);
	}
}

void CPacketImplementation::AppendData(CList<double, double>& lsnewData)
{
	if(lsnewData.IsEmpty())
		return ;
	POSITION pos = lsnewData.GetHeadPosition() ;
	while(pos != NULL)
	{
		double nData = lsnewData.GetNext(pos) ;
		m_lnData.AddTail(nData);
	}
	ClearMinMax();
}

// 03.22.2002  ������
void CPacketImplementation::AppendData(CList<CString, CString>& lsnewData)
{
	if(lsnewData.IsEmpty())
		return ;
	POSITION pos = lsnewData.GetHeadPosition() ;
	while(pos != NULL){
		CString strData = lsnewData.GetNext(pos) ;
		m_lstrData.AddTail(strData) ;
	}
}

void CPacketImplementation::InsertData(CString strData)
{
	if(m_Type == _MTEXT( C6_CHARACTER)){
		m_lstrData.AddHead(strData) ;
	}
	else
	{
		double value = atof(strData);
		m_lnData.AddHead(value);
		ClearMinMax();
	}
}

void CPacketImplementation::InsertData(long pData, int nDataLength)
{
	char szData[1024] ;
	memcpy(szData, (void*)pData, nDataLength) ;
	szData[nDataLength] = NULL ;
	CString strData = szData ;
	if(m_Type == _MTEXT( C6_CHARACTER)){
		m_lstrData.AddHead(strData) ;
	}
	else{
		double value = atof(strData);
		m_lnData.AddHead(value) ;
		ClearMinMax();
	}
}

bool CPacketImplementation::IsTRData() const
{
	return m_bBaseData;
}

int CPacketImplementation::GetLength()
{
	return m_nLength ;
}

void CPacketImplementation::ChangeData(CList<double, double>& lsnewData)
{
	if(!m_lnData.IsEmpty())
		m_lnData.RemoveAll() ;

	POSITION pos = lsnewData.GetHeadPosition() ;
	while(pos != NULL)	{
		double nData = lsnewData.GetNext(pos) ;
		m_lnData.AddTail(nData) ;
	}
	ClearMinMax();
}

// 03.22.2002 ������
void CPacketImplementation::ChangeData(CList<CString, CString>& lsnewData)
{
	if(!m_lstrData.IsEmpty())
		m_lstrData.RemoveAll() ;

	POSITION pos = lsnewData.GetHeadPosition() ;
	while(pos != NULL)	{
		CString strData = lsnewData.GetNext(pos) ;
		m_lstrData.AddTail(strData) ;
	}
}

//	return value
//	-2 : type error
//	-1 : error
//	 0 : nEnd - nStart = Data Count
//	 1 : success
// (2009/1/17 - Seung-Won, Bae) Add without-Zero option.
int CPacketImplementation::GetMinMax(int nStart, int nEnd, double& nMin, double& nMax, BOOL p_bWithoutZero)
{
	if(m_Type == _MTEXT( C6_CHARACTER))
		return -2 ;

	if(m_lnData.IsEmpty())
		return -1 ;

	if(nStart < 0)
		return -1 ;

	if(nEnd >= m_lnData.GetCount())
		nEnd = m_lnData.GetCount() - 1 ;

	if(nStart > nEnd)
		return -1;

	if(m_bUseBound)	{
		//sy 2003.10.28. min/max�� �ùٸ� �������� ��Ÿ���� ����
		if(-DBL_MAX == m_dMaxBound && DBL_MAX == m_dMinBound)
			return -1;
		nMax = m_dMaxBound;
		nMin = m_dMinBound;
		return 1;
	}

	//sy 2003.10.28. min/max�� �ùٸ� �������� ��Ÿ���� ����
	// (2007/2/7 - Seung-Won, Bae) Do not return in No Init. Calculate new Min/Max.
	// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
	//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
	if( ( m_bWithoutZero_Pre && p_bWithoutZero || !m_bWithoutZero_Pre && !p_bWithoutZero)
		&& m_dContinueCode_Pre == m_dContinueCode
		&& m_dExceptionValue_Pre == m_dExceptionValue
		&& ( m_nStartPeriod_Pre == m_nStartPeriod
			|| ( m_nStartPeriod_Pre <= m_nStartIndex_Pre
				&& m_nStartPeriod <= m_nStartIndex_Pre))
		&& ( m_nStartIndex_Pre == nStart
			|| ( m_nStartPeriod_Pre == m_nStartPeriod
				&& m_nStartIndex_Pre <= m_nStartPeriod
				&& nStart <= m_nStartPeriod)))
	{
		if( nEnd == m_nEndIndex_Pre
			&& -DBL_MAX != m_dPreMaxValue
			&& DBL_MAX != m_dPreMinValue)
		{
			m_nStartPeriod_Pre = m_nStartPeriod;
			nMax = m_dPreMaxValue;
			nMin = m_dPreMinValue; 
			return 1;
		}
		else if( nEnd == m_nEndIndex_Pre - 1
			&& -DBL_MAX != m_dPreMaxValueWithoutEnd
			&& DBL_MAX != m_dPreMinValueWithoutEnd)
		{
			m_nStartPeriod_Pre = m_nStartPeriod;
			nMax = m_dPreMaxValueWithoutEnd;
			nMin = m_dPreMinValueWithoutEnd; 
			return 1;
		}
	}

	// Ranged Min/Max
	double dMax2 = -DBL_MAX;
	double dMin2 = DBL_MAX;
	double dMaxWE = dMax2;
	double dMinWE = dMin2;
	double nValue;
	int i = nStart;
	// (2009/1/17 - Seung-Won, Bae) Use Pre Max/Min without-End Value.
	if( i == m_nStartIndex_Pre
		&& m_nEndIndex_Pre <= nEnd
		&& m_dPreMaxValueWithoutEnd != -DBL_MAX
		&& m_dPreMinValueWithoutEnd != DBL_MAX)
	{
		dMax2 = m_dPreMaxValueWithoutEnd;
		dMin2 = m_dPreMinValueWithoutEnd;
		i = m_nEndIndex_Pre;
	}
	POSITION pos = m_lnData.FindIndex( i);
	POSITION posBack = pos;
	if( pos == NULL) return -1;
	while( pos && i <= nEnd)
	{
		nValue = m_lnData.GetNext(pos);

		// (2007/2/22 - Seung-Won, Bae) for Continue Code
		if( i == nStart && nValue == m_dContinueCode && -HUGE_VAL != m_dContinueCode)
		{
			// Get Prev Position of Start
			m_lnData.GetPrev( posBack);
			while( posBack && nValue == m_dContinueCode)
				nValue = m_lnData.GetPrev( posBack);
		}

		i++;
		if( i <= m_nStartPeriod) continue;

		// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
		//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
		dMaxWE = dMax2;
		dMinWE = dMin2;

		// (2007/2/7 - Seung-Won, Bae) for Exception Value
		// (2007/2/22 - Seung-Won, Bae) for Continue Code
		// (2009/1/17 - Seung-Won, Bae) Add without-Zero option.
		if( nValue == m_dExceptionValue
			|| nValue == m_dContinueCode
			|| ( nValue == 0.0 && p_bWithoutZero))
				continue;

		if( dMax2 < nValue) dMax2 = nValue;
		if( nValue < dMin2) dMin2 = nValue;
	}

	if( dMax2 == -DBL_MAX || dMin2 == DBL_MAX) return 0;
	
	//{{ 2008.01.09 by LYH - Y�� ȯ�漳������ ��ü������ �ְ��� �������϶� �ּҰ��� 0������ ���� ����
//	if(dMax2 ==0 && dMin2 == 0) return 0;
	//}}
	
	nMax = dMax2;
	nMin = dMin2;

	// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
	//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
	m_bWithoutZero_Pre			= p_bWithoutZero;
	m_dContinueCode_Pre			= m_dContinueCode;
	m_dExceptionValue_Pre		= m_dExceptionValue;
	m_nStartPeriod_Pre			= m_nStartPeriod;
	m_nStartIndex_Pre			= nStart;
	m_nEndIndex_Pre				= nEnd;
	m_dPreMaxValue				= nMax;
	m_dPreMinValue				= nMin;
	m_dPreMaxValueWithoutEnd	= dMaxWE;
	m_dPreMinValueWithoutEnd	= dMinWE;

	return 1 ;
}


//	return value 
//	-2 : type error
//	-1 : error
//	 none zero : success
int CPacketImplementation::SetMinMax(CPacketDataInfo::DATAINSERTTYPE nInsertType, CPoint ptDataRange)
{
	return 1 ;
}

// bClearIncludingTR = true : TR data �� ����
// bClearIncludingTR = false : TR data�� �����ϰ� ����
void CPacketImplementation::Initialize(const bool bClearIncludingTR)
{
	ClearData(bClearIncludingTR);

	RestoreMinMax();

	// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
	//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
	ClearMinMax();

	m_standardValue.Set(false, 0.0);
	m_upperBaseValue.Set(false, 0.0);
	m_lowerBaseValue.Set(false, 0.0);
	m_realDataGap.Set(false, 0.0);

	m_nStartPeriod = 0;
	m_bAcceptFromServer = false; //sy 2005.11.08.
}

//bClearIncludingTR�� true�� �⺻�����͵� ������ ����
//bClearIncludingTR�� false�� �⺻�����ʹ� �������� �ʴ´�.
int CPacketImplementation::ClearData(const bool bClearIncludingTR)
{
	//�⺻�����ʹ� �������� �ʴ´�.
	//m_bBaseData�� true�̸� �⺻������
	if(m_bBaseData && !bClearIncludingTR)
		return 0;

	m_lnData.RemoveAll();
	m_lstrData.RemoveAll();
	RemoveAllTempList();

	// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
	//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
	ClearMinMax();

	// (2009/5/26 - Seung-Won, Bae) Clear start period with data.
	m_nStartPeriod = 0;

	return 1 ;
}

const CList<double, double>* CPacketImplementation::GetnumericDataList() const
{
	if(m_Type == _MTEXT( C6_CHARACTER))	return NULL ;
	return &m_lnData;
}

CList<double, double>* CPacketImplementation::GetnumericDataList()
{
	if(m_Type == _MTEXT( C6_CHARACTER))	return NULL ;
	return &m_lnData ;
}

CList<CString, CString>* CPacketImplementation::GetstrDataList()
{
	if(m_Type != _MTEXT( C6_CHARACTER) && m_lstrData.GetCount() <= 0)
		return NULL;

	return &m_lstrData ;
}

//bIsFulllType
// true �̸� packettype�� ���� Ÿ������ scale, title ����.
// false �̸� ���Ŀ��� scale ǥ�������� �߰��� Ÿ�Ը���.
CString CPacketImplementation::GetType(const bool bIsFullType)
{
	if(bIsFullType)
		return m_Type;
	else
		return m_ExpressionType;
}

//*, %�� ���� �ؼ� �ѱ��.
//bIsFullType�� true�� packet�� ���� Ÿ�� �ѱ�
//false�� ǥ��Ÿ�԰� ���� �ѱ�
double CPacketImplementation::GetTypeFormatDouble(const bool bIsFullType)
{
	double ret = 1.0;
	CString tmp;
	
	if(bIsFullType)
		tmp = m_Type;
	else
		tmp = m_ExpressionType;

	// Ÿ���� �����϶���
	int nfind1 = tmp.Find("1");
	if(nfind1 >= 0)	{
		// 03.15.2002	% �� �̹��� ����
		if(tmp.Right(1) != "0" && tmp.Right(1) != "1")
			tmp = tmp.Left(tmp.GetLength() - 1);

		int nfindp = tmp.Find(".");
		if(nfindp >= 0)
			tmp = tmp.Right(tmp.GetLength() - nfindp + 1);
		else
			tmp = tmp.Right(tmp.GetLength() - nfind1);

		ret = atof(tmp);
	}

	return ret;	
}

int CPacketImplementation::GetStartPeriod()
{
	return m_nStartPeriod ;
}

bool CPacketImplementation::GetRealDataGap(double& dDataGap) const
{
	if(!m_realDataGap.IsUsing())
		return false;

	dDataGap = m_realDataGap.GetData();
	return true;
}

void CPacketImplementation::SetRealDataGap( const double &p_dDataGap)
{
	m_realDataGap.Set( true, p_dDataGap);
}

void CPacketImplementation::SetLength(int nLength)
{
	m_nLength = nLength ;
}

void CPacketImplementation::SetType(const CString& strType)
{
	m_Type = strType ;
	m_Type.TrimLeft();
	m_Type.TrimRight();
	m_ExpressionType = m_Type;
}

// m_Type : �Ӽ� page���� ������ type, -> ��ġ��ȸâ, data ��ü���⿡�� ���
// m_ExpressionType : �Ϲ����� type -> ȭ�� ������ ���Ҷ�, scale ǥ���Ҷ� ���
void CPacketImplementation::SetType(const CString& strFullType, const CString& strExpressionType)
{
	m_Type = strFullType;
	m_Type.TrimLeft(" ");
	m_Type.TrimRight(" ");
	m_ExpressionType = strExpressionType;
	m_ExpressionType.TrimLeft(" ");
	m_ExpressionType.TrimRight(" ");
}

void CPacketImplementation::SetYesOrNoBaseData(const bool bBaseData)
{
	m_bBaseData = bBaseData;
}

void CPacketImplementation::SetName(CString strName)
{
	m_Name = strName ;
}

void CPacketImplementation::SetStartPeriod(int nStartPeriod)
{
	m_nStartPeriod = nStartPeriod;
}

bool CPacketImplementation::IsUseStandardValue( const int p_nType) const
{
	switch( p_nType)
	{
		case 0:	return m_standardValue.IsUsing();
		case 1: return m_upperBaseValue.IsUsing();
		case 2: return m_lowerBaseValue.IsUsing();
	}
	return false;
}

double CPacketImplementation::GetStandardValue( const int p_nType)
{
	if( !IsUseStandardValue( p_nType)) return -HUGE_VAL;

	switch( p_nType)
	{
		case 0:	return m_standardValue.GetData();
		case 1: return m_upperBaseValue.GetData();
		case 2: return m_lowerBaseValue.GetData();
	}
	return -HUGE_VAL;
}

void CPacketImplementation::SetStandardValue( const char *p_szValue)
{
	CString strValue;
	char *szValue = strValue.GetBufferSetLength( strlen( p_szValue));
	strcpy( szValue, p_szValue);

	char *szNext = strchr( szValue, ',');
	if( szNext) *szNext = '\0';
	m_standardValue.Set( true, atof( szValue));
	if( !szNext) return;
	szValue = szNext + 1;

	szNext = strchr( szValue, ',');
	if( szNext) *szNext = '\0';
	m_upperBaseValue.Set( true, atof( szValue));
	if( !szNext) return;
	szValue = szNext + 1;

	m_lowerBaseValue.Set( true, atof( szValue));
}

bool CPacketImplementation::GetUseReal()
{
	return m_bUseReal;	
}

CString CPacketImplementation::GetRealFieldName()
{
	return m_RealFieldName;
}

CString CPacketImplementation::GetRealKeyName()
{
	return m_RealKeyName;
}

void CPacketImplementation::UpdateData(const CString& strData)
{
	if(m_Type == _MTEXT( C6_CHARACTER)){
		POSITION pos = m_lstrData.GetTailPosition();
		if(!pos)	return;
		m_lstrData.SetAt(pos, strData);
	}
	else{
		POSITION pos = m_lnData.GetTailPosition();
		if(pos == NULL) return;
		double value = atof(strData);
		double oldValue = m_lnData.GetAt(pos);
		m_lnData.SetAt( pos, value);

		// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
		//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
		UpdateMinMax( value, m_lnData.GetCount() - 1);
	}
}

//bIsCurrentData == true
//���� index�� ��ġ�� ������ �׳� return
//false�� index������ ������ �߰�
void CPacketImplementation::UpdateData(const CString& strData, const int nIndex, const bool bIsCurrentData)
{
	if(nIndex < 0)
		return;

	if(m_Type == _MTEXT( C6_CHARACTER)){
		POSITION pos = FindPosition(nIndex);
		if(pos == NULL){
			if(bIsCurrentData)
				return;
			else 
				m_lstrData.AddTail(strData);
		}
		else
			m_lstrData.SetAt(pos, strData);
	}
	else
		UpdateData(strData, nIndex, bIsCurrentData);
}

void CPacketImplementation::UpdateData(const double nData)
{
	if(m_Type == _MTEXT( C6_CHARACTER))
		return;

	POSITION pos = m_lnData.GetTailPosition();
	if(!pos)
		return;

	//sy 2003.3.19 -> ������ ���õ� update data�� packet ���� �Ѵ�.
	double dRealData = nData;
	if(!GetRealData_UpdateData(nData, dRealData))
		return;

	double oldValue = m_lnData.GetAt(pos);
	if(oldValue == nData)
		return;

	m_lnData.SetAt(pos, dRealData);

	// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
	//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
	UpdateMinMax( dRealData, m_lnData.GetCount() - 1);
}

//bIsCurrentData == true
//���� index�� ��ġ�� ������ �׳� return
//if nIndex == -1, ������ �����͸� ������Ʈ �Ѵ�.
void CPacketImplementation::UpdateData(const double nData, const int nIndex, const bool bIsCurrentData)
{
	if(nIndex == -1) {	//2003.05.26 nykim
		UpdateData(nData);
		return;
	}

	if(m_Type == _MTEXT( C6_CHARACTER))
		return;
	
	POSITION pos = FindPosition(nIndex);
	if(pos == NULL)
	{
		if( bIsCurrentData)	return;
		else				m_lnData.AddTail(nData);

		// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
		//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
		UpdateMinMax( nData, m_lnData.GetCount() - 1);
	}
	else
	{
		m_lnData.SetAt( pos, nData);

		// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
		//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
		UpdateMinMax( nData, nIndex);
	}
}

/*
void CPacketImplementation::UpdateData(CList<double, double>& lsnewData, CPoint ptUpdateRange)
{
	if(lsnewData.IsEmpty())
		return ;

	if(GetCount() <= ptUpdateRange.x)
		AppendData(lsnewData);
	else	{
		POSITION pos = lsnewData.GetHeadPosition();
		POSITION pos_old = FindPosition(ptUpdateRange.x);

		double value;
		while(pos && pos_old)	{
			value = lsnewData.GetNext(pos);
			m_lnData.SetAt(pos_old, value);
			m_lnData.GetNext(pos_old);
		}
		while(pos)	{
			value = lsnewData.GetNext(pos);
			m_lnData.AddTail(value);
		}
		ResetMinMax();
	}
}
*/

// 03.22.2002  ������
void CPacketImplementation::UpdateData(CList<CString, CString>& lsnewData, CPoint ptUpdateRange)
{
	if(lsnewData.IsEmpty())
		return ;

	if(GetCount() <= ptUpdateRange.x)
		AppendData(lsnewData);
	else	{
		POSITION pos = lsnewData.GetHeadPosition();
//		POSITION pos_old = m_lnData.FindIndex(ptUpdateRange.x);
		POSITION pos_old = FindPosition(ptUpdateRange.x);

		while(pos && pos_old)	{
			m_lstrData.SetAt(pos_old, lsnewData.GetNext(pos));
			m_lstrData.GetNext(pos_old);			
		}
		while(pos)	{
			m_lstrData.AddTail(lsnewData.GetNext(pos));
		}
	}

//	SetMinMax(CPacketDataInfo::REAL_APPEND, ptUpdateRange);
}

int CPacketImplementation::GetDateUnitType()
{
	if(m_pParentList)
		return m_pParentList->GetDateUnitType();
	else
		return -1;
}

int CPacketImplementation::GetTimeInterval()
{
	if(m_pParentList)
		return m_pParentList->GetTimeInterval();
	else
		return -1;
}

bool CPacketImplementation::GetData(const int nIndex, double& dData) const
{
	POSITION pos = FindPosition(nIndex);
	if(pos == NULL)
		return false;

	dData = m_lnData.GetAt(pos);
	return true;
}

double CPacketImplementation::GetData(int nIndex)
{
	if(m_lnData.GetCount() <= 0)	return ERRORVALUE;
	if(nIndex >= m_lnData.GetCount())	return ERRORVALUE;

	POSITION pos = FindPosition(nIndex);
	if(pos == NULL)
		return ERRORVALUE;

	return m_lnData.GetAt(pos);
}

CString CPacketImplementation::GetStrData(int nIndex)
{
	if(m_lstrData.GetCount() <= 0)	return "";
	if(nIndex >= m_lstrData.GetCount())	return "";

	POSITION pos = FindStrPosition(nIndex);
	return m_lstrData.GetAt(pos);
}

bool CPacketImplementation::GetDataPastandFuture(const int nIndex, double& dData)
{
	if(m_lnData.GetCount() <= 0)
		return false;

	// <�ڷ�����> ���� �ƴѰ��
	if(m_Name.Find( _MTEXT( C0_DATE_TIME)) < 0)
		return GetData(nIndex, dData);

	dData = GetDataPastandFuture(nIndex);
	return (dData > 0);
}

// 2010.01.06 by SYS >> ����Ʈ YYYY(��) �߰� <<
//int CPacketImplementation::GetDataPastandFuture(int nIndex)
double CPacketImplementation::GetDataPastandFuture(int nIndex)
{
	if(m_Type.Find("1") >= 0 || m_Type.Find( _MTEXT( C6_CHARACTER)) >= 0 || m_Type.GetLength() == 0)
		return 0;
	if(m_lnData.GetCount() <= 0) return 0;

	if(nIndex >= 0 && nIndex < m_lnData.GetCount())
		//return (int)GetData(nIndex);	
		return GetData(nIndex);			// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰� <<

	if(nIndex >= m_lnData.GetCount())
	{
		if(m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_TIME_TWO ||
			m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_TIME_ONE)
		{
			// (2006/7/11 - Seung-Won, Bae) Do not use prev data for calculating future time.
			//		Because the prev data is not exactly previous time data with few data.
			//		Use Real End Time of this term.
			/*
				//sy ���� �����ϱ�
				//������ �����Ͱ� real�� �ް� ���� ���� �� �´� ������ �ȵǹǷ� 
				//������ ���� data�� ���Ѵ�. 
				//ex)5�� �ε� real�� ������ ���� 0, 5, 10, 12 �̹Ƿ� 10���� ���Ѵ�.
				int nBaseData = (int)m_lnData.GetTail();
				int nBaseIndex = nIndex - m_lnData.GetCount() + 1;
				if(m_lnData.GetCount() > 1)
				{
					POSITION pos = m_lnData.GetTailPosition();
					if(pos != NULL)
						m_lnData.GetPrev(pos);
					if(pos != NULL)
						nBaseData = (int)m_lnData.GetPrev(pos);

					nBaseIndex += 1;
				}
				//sy end
			*/
			// 2011.01.06 by SYS >> ����Ʈ YYYY(��) �߰�
			__int64 nBaseData = m_pParentList->GetStartTimeInTimeRangeOfLastData();
			if( nBaseData < 0) 
				nBaseData = (__int64)m_lnData.GetTail();
			int nBaseIndex = nIndex - m_lnData.GetCount() + 1;

			if (m_Type == "YYYYMMDDHHMMSS")
			{
				__int64 nYear	= nBaseData/10000000000;
				__int64 nSec	= nBaseData%100;
				
				// MMDDHHMM ���� �����.
				nBaseData = (nBaseData%10000000000) - nSec;
				
				__int64 nValue = (__int64)CDataConversion::CalcTimeFormatforCharge((int)nBaseData, 
					nBaseIndex, "MMDDHHMM", GetTimeInterval(), true);
				
				// YYYYMMDDhhmmss �� �����.
				double dValue = (double)(nYear*10000000000) + (nValue*100) + nSec;
				return dValue;
			}
			else
			{
				return CDataConversion::CalcTimeFormatforCharge((int)nBaseData, 
					nBaseIndex, m_Type, GetTimeInterval(), true);
			}

			// 2011.01.06 by SYS <<
		}
		else if(m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_TICK)
		{
			//sy ���� �����ϱ�
			//nami 12.6
			int nDateIndex = nIndex - m_lnData.GetCount() + 1;
			//100�� ���� �и� ������Ʈ �ȴ�. �ʿ� �߸��� �����Ͱ� ����.
			if(nDateIndex >= 100)
			{
				int nBunmo = (nDateIndex / 60) * 100;
				int nBunja = nDateIndex % 60;
				nDateIndex = nBunmo + nBunja;
			}
			return CDataConversion::CalcTimeFormat((int)m_lnData.GetTail(), nDateIndex, m_Type, true);
		}
		else if(m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_TEXT 
			 || m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_NOTYPE)
			return 0;
		else
			return CDataConversion::CalcDateFormat((int)m_lnData.GetTail(), (nIndex - m_lnData.GetCount() + 1), m_Type, GetDateUnitType(), GetTimeInterval());
	}
	else if(nIndex < 0)
	{
		if(m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_TIME_TWO || 
		   m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_TIME_ONE)
			return CDataConversion::CalcTimeFormatforCharge((int)m_lnData.GetHead(), nIndex, m_Type, GetTimeInterval(), true);
		else if(m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_TICK)
			return CDataConversion::CalcTimeFormat((int)m_lnData.GetHead(), nIndex, m_Type, true);
		else if(m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_TEXT 
			 || m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_NOTYPE)
			return 0;
		else
			return CDataConversion::CalcDateFormat((int)m_lnData.GetHead(), nIndex, m_Type, GetDateUnitType(), GetTimeInterval());
	}

	return 0;
}

// ����Ʈ �����ð� ���� - ojtaso (20080811)
int CPacketImplementation::GetIndexFromData(double nData, CString strType/* = _T("")*/)
{
	if(m_lnData.GetCount() <= 0)	return -1;
	
	POSITION pos = m_lnData.GetHeadPosition();
	int nIndex = 0;
	while(pos)	{
		if(m_lnData.GetNext(pos) == nData)
			return nIndex;
		nIndex++;
	}
	
	if(m_Name == _MTEXT( C0_DATE_TIME))
	{
		if(m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_TIME_TWO ||
		   m_pParentList->GetDateUnitType() == CScaleBaseData::HORZ_TIME_ONE)	
			// ����Ʈ �����ð� ���� - ojtaso (20080811)
		    return GetIndexInCaseTypeIsBun(nData, strType);
		else
			return GetIndexInCaseTypeIsnotBun(nData);
	}


	return -1;
}

//sy 2004.06.18.
// return : true -> ���� ���� ���
//			false -> ������ ����ų� ���� ���� ���� ���
bool CPacketImplementation::GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData) const
{
	dCurOrPrevIndexData = 0.0;
	nCurOrPrevIndex = -1;

//	if(m_Name == "�ڷ�����")
		return GetCurOrPrevIndexFromData_Date(dData, dCurOrPrevIndexData, nCurOrPrevIndex);

	return false;
}

bool CPacketImplementation::GetCurOrPrevIndexFromData_Date(const double& dData, double& dCurOrPrevIndexData, int& nCurOrPrevIndex) const
{
	if(m_lnData.GetCount() <= 0)
		return false;

	// data ���� ���� ���
	if(m_lnData.GetHead() > dData)
		return false;
	if(m_lnData.GetTail() < dData)
	{
		dCurOrPrevIndexData = m_lnData.GetTail();
		nCurOrPrevIndex = m_lnData.GetCount() -1;
		return true;
	}

	// data ���� ���� ���
	POSITION pos = m_lnData.GetHeadPosition();
	int nIndex = 0;
	double dPrevData = m_lnData.GetHead();
	while(pos)
	{
		double dCurData = m_lnData.GetNext(pos);
		if(dCurData == dData)
		{
			dCurOrPrevIndexData = dCurData;
			nCurOrPrevIndex = nIndex;
			return true;
		}
		
		if(dCurData > dData && dPrevData < dData)
		{
			dCurOrPrevIndexData = dPrevData;
			nCurOrPrevIndex = --nIndex;
			return true;
		}

		dPrevData = dCurData;
		nIndex++;
	}
	return false;
}
//sy end

void CPacketImplementation::SetBoundLimit(double nUpperBound, double nLowerBound)
{
	m_bUseBound = true;
	m_dMaxBound = nUpperBound;
	m_dMinBound = nLowerBound;
}

void CPacketImplementation::SetBaseData(const CString &strType, 
										const CString &strExpressionType,
										const int nStartPeriod, 
										const bool bIsAutoMinMax)
{
	m_Type = strType ;
	m_ExpressionType = strExpressionType ;

//	//sy 2005.06.16. ���� �Ⱓ�� üũ�Ѵ�.
//	if(bIsAutoMinMax && m_nStartPeriod != nStartPeriod)
//	{
//		m_nStartPeriod = nStartPeriod;
//		ResetMinMax();
//	}
	m_nStartPeriod = nStartPeriod;
}

//sy 2004.07.16.
bool CPacketImplementation::RemoveData(int nStart, int nEnd)
{
	if(GetCount() < nEnd || nStart != 0)
		return false;

	// ����
	int nIntDataCount = m_lnData.GetCount();
	if(nIntDataCount > 0)
	{
		for(int i = nStart; i < nEnd; i++)
			m_lnData.RemoveHead();
	}

	// ����
	int nStrDataCount = m_lstrData.GetCount();
	if(nStrDataCount > 0)
	{
		for(int i = nStart; i < nEnd; i++)
			m_lstrData.RemoveHead();
	}

	// min/max �� ���ڸ� �ش�.
	if(nIntDataCount != m_lnData.GetCount())
		ClearMinMax();

	return (nIntDataCount != m_lnData.GetCount() || nStrDataCount != m_lstrData.GetCount());
}

POSITION CPacketImplementation::FindPosition(const int nIndex) const
{	
	int nTailIndex = m_lnData.GetCount() -1;

	if(nIndex < 0 || nIndex > nTailIndex)
		return NULL;

	if(nIndex > nTailIndex/2){
		int nCount = nTailIndex;
		POSITION pos = m_lnData.GetTailPosition();
		while(pos != NULL){
			if(nCount-- == nIndex)
				return pos;
			m_lnData.GetPrev(pos);
		}
	}
	else{
		int nCount = 0;
		POSITION pos = m_lnData.GetHeadPosition();
		while(pos != NULL){
			if(nCount++ == nIndex)
				return pos;
			m_lnData.GetNext(pos);
		}
	}

	return NULL;
}



POSITION CPacketImplementation::FindStrPosition(const int nIndex) const
{	
	int nTailIndex = m_lstrData.GetCount() -1;

	if(nIndex < 0 || nIndex > nTailIndex)
		return NULL;

	if(nIndex > nTailIndex/2){
		int nCount = nTailIndex;
		POSITION pos = m_lstrData.GetTailPosition();
		while(pos != NULL){
			if(nCount-- == nIndex)
				return pos;
			m_lstrData.GetPrev(pos);
		}
	}
	else{
		int nCount = 0;
		POSITION pos = m_lstrData.GetHeadPosition();
		while(pos != NULL){
			if(nCount++ == nIndex)
				return pos;
			m_lstrData.GetNext(pos);
		}
	}

	return NULL;
}

CPacket* CPacketImplementation::GetDatePacket()
{
	if(m_Name == _MTEXT( C0_DATE_TIME))
		return m_pPacket;
	
	if(m_pParentList)
		return m_pParentList->GetBaseData( _MTEXT( C0_DATE_TIME));

	return NULL;
}

// �屸�� (�ֽ�/����/����/�ɼ�)
CPacketType::COMMODITY_BOND_NAME CPacketImplementation::GetCommodityBondName() const
{
	return m_eCommodityBondName;
}

void CPacketImplementation::SetCommodityBondName(const CPacketType::COMMODITY_BOND_NAME eCommodityBondName)
{
	m_eCommodityBondName = eCommodityBondName;
}

bool CPacketImplementation::SetMinMax(const double nMin, const double nMax)
{
	m_dMaxBound = nMax;
	m_dMinBound = nMin;
	m_bUseBound = ( m_dMaxBound != -DBL_MAX && m_dMinBound != DBL_MAX);

	return true;
}

bool CPacketImplementation::RestoreMinMax()
{
	m_bUseBound = false;
	return true;
}

bool CPacketImplementation::SetMin(const double nMin)
{
	m_dMinBound = nMin;
	m_bUseBound = true;

	return true;
}

bool CPacketImplementation::SetMax(const double nMax)
{
	m_dMaxBound = nMax;
	m_bUseBound = true;

	return true;
}

bool CPacketImplementation::RestoreMin()
{
	m_bUseBound = false;
	return true;
}

bool CPacketImplementation::RestoreMax()
{
	m_bUseBound = false;
	return true;
}

CPacketList* CPacketImplementation::GetPacketList()
{	
	return m_pParentList;
}

//������ packet�� count return
int CPacketImplementation::RemoveFictionData(const double dReallastDate, const bool bDate)
{
	//�ڷ������� ���
	//dReallastDate �� ���� ��¥ ����Ÿ�̴�.
	if(bDate == true) 
		return RemoveDateData(dReallastDate);
	else  //���⼭ dReallastDate�� �����ؾ� �ϴ� packet�� count��.
		return RemoveBaseData(dReallastDate);

	return false;
}

int CPacketImplementation::RemoveDateData(const double dReallastDate)
{
	int nDeleteCount = 0;
	if(m_lnData.GetCount() > 0)	{
		POSITION pos = m_lnData.GetTailPosition();
		while(pos)
		{
			POSITION temp = pos;
			double dData = m_lnData.GetPrev(pos);
			//dData�� fiction ����Ÿ �̴ϱ�.
			//fiction�� ������ dData�� �翬�� ũ��.
			if( dData < dReallastDate) return nDeleteCount;
			if( dData > dReallastDate)
			{
				m_lnData.RemoveAt(temp);
				nDeleteCount++;
				ClearMinMax();
			}
			else if(dData == dReallastDate) 
				return nDeleteCount;
		}
	}
	return nDeleteCount;
}

int CPacketImplementation::RemoveBaseData(const double nCount)
{
	int nDeleteCount = 0;
	if(m_lnData.GetCount() > 0)	{
		POSITION pos = m_lnData.GetTailPosition();
		while(pos) {
			if(nDeleteCount >= nCount)
				break;

			POSITION temp = pos;
			double dData = m_lnData.GetPrev(pos);

			m_lnData.RemoveAt(temp);
			nDeleteCount++;
		}
	}
	ClearMinMax();
	return nDeleteCount;
}

// "��"��Ʈ �ΰ��
// ����Ʈ �����ð� ���� - ojtaso (20080811)
int CPacketImplementation::GetIndexInCaseTypeIsBun(double dData, CString strType/* = _T("")*/)
{
	if(m_lnData.GetCount() <= 0)
		//<<20100426_JS.Kim ����Ÿ ������ -1 �����ϴ°� �´µ�
		//return 0;
		return -1;
		//>>

	//����
	// ����Ʈ �����ð� ���� - ojtaso (20080811)
	int nIndex = GetIndexInCaseTypeIsBun_CurData(dData, strType);
	if(nIndex >= 0)
		return nIndex;

	int nTailNum = 0, nHeadNum = 0, nDataNum = 0;
	GetComparisionDatas(dData, nHeadNum, nTailNum, nDataNum);

	//������ ����Ÿ�� ���ڰ��� ���� ù����Ÿ�� ���ڰ��� ������ �۰ų� ������
	//�̷�
	if(abs(nTailNum - nDataNum) <= abs(nHeadNum - nDataNum))
		return GetIndexOfFuture(dData);
	else
		return GetIndexOfPast(dData);

	//<<20100426_JS.Kim ����Ÿ ������ -1 �����ϴ°� �´µ�
	//return 0;
	return -1;
	//>>
}

// ����Ʈ �����ð� ���� - ojtaso (20080811)
int CPacketImplementation::GetIndexInCaseTypeIsBun_CurData(const double dData, CString strType/* = _T("")*/)
{
	double dTimeData = dData;
	if(!strType.IsEmpty())
	{
		CString strCurType = GetType();
		if(strCurType != strType)
		{
			if(strCurType == "DDHHMMSS")
			{
				long lTimeData = (long)dTimeData;
				long lTemp = lTimeData % 1000000;
				lTemp *= 100;

				dTimeData = lTemp;
			}
			else if(strCurType == "MMDDHHMM")
			{
				long lTimeData = (long)dTimeData;
				long lTemp = lTimeData / 100;
				long lMonth = (long)GetData(0) / 1000000;
				lMonth *= 1000000;
				lTemp += lMonth;
	
				dTimeData = lTemp;
			}
		}
		
		if(GetData(0) > dTimeData)
			dTimeData = dData;
	}
/*
	if(GetType() == "DDHHMMSS")
	{
		long lTimeData = (long)dTimeData;
		long lTemp = lTimeData % 1000000;
		lTemp *= 100;

		dTimeData = lTemp;
	}
*/

	double dTail = m_lnData.GetTail();
	if(dTimeData > dTail)
		return -1;
	else if(dTimeData == dTail)
		return (m_lnData.GetCount() - 1);

	//�̹� ���� ���� �̷� �ð��� ������..
	//���� ���� �̷� �ð��� ��Ȯ�ϰ� ��¥�� ���� �ʴ°��. ƽ
	//ƽ����Ÿ�� ���� �ǽð��� ���� �̷��ð��� �ش��ϴ� ƽ�� ���� �� �ִ�.
	//���� ����� ����Ÿ�� index�� �ѱ��.
	POSITION pos = m_lnData.GetTailPosition();
	int nIndex = m_lnData.GetCount() - 1;
	double dPrev = 0.0, dNext = 0.0;
	while(pos)	{
		dNext = dTail;
		dPrev = m_lnData.GetPrev(pos);
		if((dPrev < dTimeData) && (dTimeData < dNext)) 
//			return (dTimeData - dPrev) < (dNext - dTimeData) ? nIndex : nIndex++;
			return min(nIndex + 1, m_lnData.GetCount() - 1);
		nIndex--;
	}

	return -1;
}

// "��"��Ʈ �ƴ� ��� -> ��, ��, ��, ƽ
int CPacketImplementation::GetIndexInCaseTypeIsnotBun(const double dData)
{
	if(m_lnData.GetCount() <= 0)
		return 0;

	//����
	int nIndex = GetIndexInCaseTypeIsnotBun_CurData(dData);
	if(nIndex >= 0)
		return nIndex;

	//����
	if(m_lnData.GetHead() > dData)	
		return GetIndexOfPast(dData);
	//�̷�
	else if(m_lnData.GetTail() < dData)	
		return GetIndexOfFuture(dData);
	
	//<<20100426_JS.Kim ����Ÿ ������ -1 �����ϴ°� �´µ�
	//return 0;
	return -1;
	//>>
}

int CPacketImplementation::GetIndexInCaseTypeIsnotBun_CurData(const double dData)
{
	double dTail = m_lnData.GetTail();
	if(dData > dTail)
		return -1;
	else if(dData == dTail)
		return (m_lnData.GetCount() - 1);

	//�̹� ���� ���� �̷� �ð��� ������..
	//���� ���� �̷� �ð��� ��Ȯ�ϰ� ��¥�� ���� �ʴ°��. ƽ
	//ƽ����Ÿ�� ���� �ǽð��� ���� �̷��ð��� �ش��ϴ� ƽ�� ���� �� �ִ�.
	//���� ����� ����Ÿ�� index�� �ѱ��.
	POSITION pos = m_lnData.GetTailPosition();
	int nIndex = m_lnData.GetCount() - 1;
	double dPrev = 0.0, dNext = 0.0;
	while(pos)	{
		dNext = dTail;
		dPrev = m_lnData.GetPrev(pos);
		if((dPrev < dData) && (dData < dNext)) 
			return (dData - dPrev) < (dNext - dData) ? nIndex : nIndex++;
		nIndex--;
	}

	return -1;
}

int CPacketImplementation::GetIndexOfPast(const double dData)
{
	//sy 2004.12.01.
	// packet type �� dData �� �´��� ����.
	if(!IsDataInPacketType(dData))
		//<<20100426_JS.Kim ����Ÿ ������ -1 �����ϴ°� �´µ�
		//return 0;
		return -1;
		//>>

	int j = -1;
	//while(true)	{
	while(j > -10000) //sy ���� �����ϱ�
	{
		double dCalcDate = GetDataPastandFuture(j);
		if(dCalcDate <= 0)
			//<<20100426_JS.Kim ����Ÿ ������ -1 �����ϴ°� �´µ�
			//return 0;
			return -1;
			//>>
		if(dCalcDate <= dData)
			return j;
		j--;
	}
	//<<20100426_JS.Kim ����Ÿ ������ -1 �����ϴ°� �´µ�
	//return 0;
	return -1;
	//>>
}

int CPacketImplementation::GetIndexOfFuture(const double dData)
{
	//sy 2004.12.01.
	// packet type �� dData �� �´��� ����.
	if(!IsDataInPacketType(dData))
		//<<20100426_JS.Kim ����Ÿ ������ -1 �����ϴ°� �´µ�
		//return 0;
		return -1;
		//>>

	int j = m_lnData.GetCount();
	//while(true)	{
	while(j < 10000) //sy ���� �����ϱ�
	{
		double dCalcDate = GetDataPastandFuture(j);
		if(dCalcDate <= 0)
			//<<20100426_JS.Kim ����Ÿ ������ -1 �����ϴ°� �´µ�
			//return 0;
			return -1;
			//>>
		if(dCalcDate >= dData)
			return j;
		j++;
	}
	return m_lnData.GetCount() - 1;
}

void CPacketImplementation::GetComparisionDatas(const double dData, int &HeadNum, int &tailNum, int &dataNum)
{
	int nLength = m_Type.GetLength();

	CString strTail = CDataConversion::IntToString((int)m_lnData.GetTail(), nLength);
	CString strHead = CDataConversion::IntToString((int)m_lnData.GetHead(), nLength);
	CString strData = CDataConversion::IntToString((int)dData, nLength);

	tailNum = atoi(strTail.Left(2));
	HeadNum = atoi(strHead.Left(2));
	dataNum = atoi(strData.Left(2));

	//���̳� ���� �������
	if(tailNum == HeadNum) {
		tailNum = atoi(strTail.Mid(2, 2));
		HeadNum = atoi(strHead.Mid(2, 2));
		dataNum = atoi(strData.Mid(2, 2));
	}
}

// ----------------------------------------------------------------------------
//sy 2003.3.19 -> ������ ���õ� update data�� packet ���� �Ѵ�.
// �ǽð��϶� update �� �������� �����͸� ���Ѵ�.
// "�⺻�ŷ���"�� "�ŷ����" �� gap�� �̟G�Ͽ� �ǽð��� �����Ѵ�.
bool CPacketImplementation::GetRealData_UpdateData(const double& dData, double& dRealData)
{
	dRealData = dData;
	if(!IsChangeRealData())
		return true;

	// gap ���� ���� -> ó�� ������ �ǽð� data�� gap���θ� �����ϰ� data list�� �߰� ���� �ʴ´�.
	if(!m_realDataGap.IsUsing())
	{
		double dData2 = fabs( dData);
		m_realDataGap.Set( true, dData2);
		return false;
	}

	// gap ���� ��
	// 20121025 �̻��	�ŷ������ ���� int ���� ŭ. double �� ����.	--------->>>
//	dRealData = (int)m_lnData.GetTail() + abs((int)dData - (int)m_realDataGap.GetData());
	dRealData = (double)m_lnData.GetTail() + fabs((double)dData - (double)m_realDataGap.GetData());
	//	------------<<<<
	double dData3 = fabs(dData);
	m_realDataGap.SetData( dData3);
	return (m_lnData.GetCount() > 0);
}

// �ǽð��϶� append �� �������� �����͸� ���Ѵ�.
// "�⺻�ŷ���"�� "�ŷ����" �� gap�� �̟G�Ͽ� �ǽð��� �����Ѵ�.
double CPacketImplementation::GetRealData_AppendData(const double& dData)
{
	if(!IsChangeRealData())
		return dData;

	// gap ���� ���� -> ó�� ������ �ǽð� data�� gap���θ� �����ϰ� data list�� �߰� ���� �ʴ´�.
	if(!m_realDataGap.IsUsing())
	{
		double dData1 = fabs( dData);
		m_realDataGap.Set(true, dData1);
	}

	// gap ���� ��
	double dRealData = abs((int)dData - (int)m_realDataGap.GetData());
	double dData2 = fabs( dData);
	m_realDataGap.SetData( dData2);
	return dRealData;
}

// data �⺻�� "0" �� packet
bool CPacketImplementation::IsZeroBasePacket() const
{
	if(m_Name.IsEmpty())
		return false;

	// "�⺻�ŷ���", "�ŷ����"
	return (m_Name == _MTEXT( C0_VOLUME) || m_Name == _MTEXT( C2_VOLUME_AMOUNT));
}

// update/append �� ���� real data�� �������� ����
// true : real data�� ���� ���Ѵ�.
bool CPacketImplementation::IsChangeRealData() const
{
	// "�⺻�ŷ���", "�ŷ����"
	if(!IsZeroBasePacket())
		return false;

	// �ϰ� �ϰ�쿣 ���� real data�� ������ �ʴ´�.
	return (m_pParentList->GetDateUnitType() != CScaleBaseData::HORZ_DAILY);
}

/*
double CPacketImplementation::GetMin() const
{
	return m_dMinValue;
}

double CPacketImplementation::GetMax() const
{
	return m_dMaxValue;
}
*/

//sy 2004.12.01.
// data �� packet type �� �´� data ������ �����Ѵ�.
// packet type : YYYYMMDDHHMMSS �� ������ ��� �����ؾ� �Ѵ�.
bool CPacketImplementation::IsDataInPacketType(const double& dData) const
{
	if(m_pParentList == NULL || m_Type.IsEmpty() || dData <= 0)
		return false;

	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnitType = m_pParentList->GetDateUnitType();
	// "�ϰ�", "�ְ�", "����"
	if(eDateUnitType == CScaleBaseData::HORZ_DAILY || 
		eDateUnitType == CScaleBaseData::HORZ_WEEKLEY || 
		eDateUnitType == CScaleBaseData::HORZ_MONTHLY)
	{
		return IsDataInPacketType_Date(dData);
	}
	// "��", "ƽ"
	else if(eDateUnitType == CScaleBaseData::HORZ_TIME_ONE || 
		eDateUnitType == CScaleBaseData::HORZ_TIME_TWO || 
		eDateUnitType == CScaleBaseData::HORZ_TICK)
	{
		return IsDataInPacketType_Time(dData);
	}

	return false;
}

//-> "�ϰ�", "�ְ�", "����" �κ�.
bool CPacketImplementation::IsDataInPacketType_Date(const double& dData) const
{
	int nYear = GetTypePartData(dData, "YYYY");
	if(nYear < 0)
		nYear = GetTypePartData(dData, "YY");
	int nMonth = GetTypePartData(dData, "MM");
	int nDaily = GetTypePartData(dData, "DD");
	return true;
}

//-> "��", "ƽ" �κ�.
bool CPacketImplementation::IsDataInPacketType_Time(const double& dData) const
{
	int nYear = GetTypePartData(dData, "SS");
	if(nYear < 0)
		nYear = GetTypePartData(dData, "MM");
	return true;
}

int CPacketImplementation::GetTypePartData(const double& dOrgData, const CString& strType) const
{
	if(strType.IsEmpty() || dOrgData <= 0)
		return -1;

	int nFindIndex = m_Type.Find(strType);
	if(nFindIndex < 0)
		return -1;

	return 0;
	int nTypeCount = m_Type.GetLength();
	for(int i = m_Type.GetLength() -1; i > nFindIndex; i--)
	{
	}

	return 0;
}
//sy end

//sy 2005.07.11.
void CPacketImplementation::AddTailTempList(const double& dData)
{
	m_dTempDataList.AddTail(dData);
}

void CPacketImplementation::RemoveAllTempList()
{
	m_dTempDataList.RemoveAll();
}

// bIsAutoTail == true �̰� �ش� index �� ���� ��� AddTail �Ѵ�.
bool CPacketImplementation::SetTempListData(const int nIndex, const double& dData, const bool bIsAutoTail)
{
	POSITION pos = m_dTempDataList.FindIndex(nIndex);
	if(pos != NULL)
	{
		m_dTempDataList.InsertBefore(pos, dData);
		return true;
	}

	if(bIsAutoTail)
	{
		AddTailTempList(dData);
		return true;
	}

	return false;
}

bool CPacketImplementation::GetTempListData(const int nIndex, double& dData) const
{
	POSITION pos = m_dTempDataList.FindIndex(nIndex);
	if(pos == NULL)
		return false;

	dData = m_dTempDataList.GetAt(pos);
	return true;
}

bool CPacketImplementation::GetHeadTempListData(double& dData) const
{
	if(m_dTempDataList.GetCount() <= 0)
		return false;

	dData = m_dTempDataList.GetHead();
	return true;
}

bool CPacketImplementation::GetTailTempListData(double& dData) const
{
	if(m_dTempDataList.GetCount() <= 0)
		return false;

	dData = m_dTempDataList.GetTail();
	return true;
}

CList<double, double>* CPacketImplementation::GetTempList() const
{
	return const_cast<CList<double, double>*>(&m_dTempDataList);
}
//sy end


// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
void CPacketImplementation::AppendTrData( const char *p_szData)
{
	m_lstrData.AddTail( p_szData);
}
void CPacketImplementation::AppendTrData( const double dData)
{
	m_lnData.AddTail( dData) ;
	UpdateMinMax( dData, m_lnData.GetCount() - 1);
}
void CPacketImplementation::InsertTrData( const char *p_szData)
{
	m_lstrData.AddHead( p_szData);
}
void CPacketImplementation::InsertTrData( const double dData)
{
	m_lnData.AddHead( dData);
	ClearMinMax();
}

// (2007/2/7 - Seung-Won, Bae) for Exception Value
void CPacketImplementation::SetExceptionValue( const double &p_dExceptionValue)
{
	m_dExceptionValue = p_dExceptionValue;
}

// (2007/2/22 - Seung-Won, Bae) for Continue Code
void CPacketImplementation::SetContinueCode( const double &p_dContinueCode)
{
	m_dContinueCode = p_dContinueCode;
}

//*, %�� ���� �ؼ� �ѱ��.
//bIsFullType�� true�� packet�� ���� Ÿ�� �ѱ�
//false�� ǥ��Ÿ�԰� ���� �ѱ�
double CPacketImplementation::GetScaleTypeFormatDouble( void)
{
	double ret = 1.0;

	// 20080910 JS.Kim		8����/32���� ó��.  ���� ǥ���� ���	<<
	if( m_strScaleType.Find("1/") >= 0 )
		return ret;
	//>>

	CString tmp( m_strScaleType);

	// Ÿ���� �����϶���
	int nfind1 = tmp.Find("1");
	if(nfind1 >= 0)	{
		// 03.15.2002	% �� �̹��� ����
		if(tmp.Right(1) != "0" && tmp.Right(1) != "1")
			tmp = tmp.Left(tmp.GetLength() - 1);

		int nfindp = tmp.Find(".");
		if(nfindp >= 0)
			tmp = tmp.Right(tmp.GetLength() - nfindp + 1);
		else
			tmp = tmp.Right(tmp.GetLength() - nfind1);

		ret = atof(tmp);
	}

	return ret;	
}


// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
void CPacketImplementation::ClearMinMax( void)
{
	m_bWithoutZero_Pre			= FALSE;
	m_dContinueCode_Pre			= -HUGE_VAL;
	m_dExceptionValue_Pre		= -HUGE_VAL;
	m_nStartPeriod_Pre			= -1;
	m_nStartIndex_Pre			= -1;
	m_nEndIndex_Pre				= -1;
	m_dPreMaxValue				= -DBL_MAX;
	m_dPreMinValue				= DBL_MAX;
	m_dPreMaxValueWithoutEnd	= -DBL_MAX;
	m_dPreMinValueWithoutEnd	= DBL_MAX;
}
void CPacketImplementation::UpdateMinMax( double p_dData, int p_nIndex)
{
	if( p_nIndex < m_nStartIndex_Pre || m_nEndIndex_Pre + 1 < p_nIndex) return;

	if( m_dPreMaxValue == -DBL_MAX
		|| m_dPreMinValue == DBL_MAX)
			return;

	if( m_bWithoutZero_Pre && p_dData == 0) return;
	if( m_dContinueCode_Pre == p_dData) return;
	if( m_dExceptionValue_Pre == p_dData) return;
	if( p_nIndex < m_nStartPeriod_Pre) return;

	if( p_nIndex == m_nEndIndex_Pre)
	{
		m_dPreMaxValue	= m_dPreMaxValueWithoutEnd;
		m_dPreMinValue	= m_dPreMinValueWithoutEnd;
		// (2009/6/2 - Seung-Won, Bae) Get min/max for only one data.
		if( m_dPreMaxValue < p_dData || m_dPreMaxValue == -DBL_MAX) m_dPreMaxValue = p_dData;
		if( p_dData < m_dPreMinValue || m_dPreMinValue == -DBL_MAX) m_dPreMinValue = p_dData;
		return;
	}

	if( p_nIndex == m_nEndIndex_Pre + 1)
	{
		m_dPreMaxValueWithoutEnd	= m_dPreMaxValue;
		m_dPreMinValueWithoutEnd	= m_dPreMinValue;
		if( m_dPreMaxValue < p_dData) m_dPreMaxValue = p_dData;
		if( p_dData < m_dPreMinValue) m_dPreMinValue = p_dData;
		m_nEndIndex_Pre = p_nIndex;
		return;
	}

	m_dPreMaxValue				= -DBL_MAX;
	m_dPreMinValue				= DBL_MAX;
	m_dPreMaxValueWithoutEnd	= -DBL_MAX;
	m_dPreMinValueWithoutEnd	= DBL_MAX;
}

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
BOOL CPacketImplementation::ShiftData( long p_szShiftCount)
{
	if(m_Type == _MTEXT( C6_CHARACTER))
			for( long i = 0; i < p_szShiftCount; i++) m_lstrData.AddHead( "");
	else	for( long i = 0; i < p_szShiftCount; i++) m_lnData.AddHead( 0.0);
	m_nStartPeriod += p_szShiftCount;
	return TRUE;
}

// ���簡 ����� - ojtaso (20080602)
void CPacketImplementation::OffsetPacketData(double dData)
{
	POSITION pos = m_lnData.GetHeadPosition();
	POSITION posTail = m_lnData.GetTailPosition();
	POSITION posCur = NULL;
	while(pos)
	{
		if(posTail == pos) break;

		posCur = pos;
		double dPacketData = m_lnData.GetNext(pos) + dData;
		m_lnData.SetAt(posCur, dPacketData);

		UpdateMinMax( dPacketData, m_lnData.GetCount() - 1);
	}
}

// 20081007 JS.Kim	����ڽð��� ����
void CPacketImplementation::CalcPacketTimeDiff(int nTimeDiff)
{
	CString strType = GetType();
	int nTime = 0, nPosition;
	if( (nPosition = strType.Find("HH")) < 0 )	// Packet�� �ð� ����Ÿ�� �����Ƿ� ���� ����� �ʿ� ����.
		return;

	int nOldTimeDiff = 0;
	if(m_pParentList)
		nOldTimeDiff = m_pParentList->GetBaseTimeDifference();
	m_pParentList->SetBaseTimeDifference(nTimeDiff);
	int nBaseTimeDiff = nTimeDiff - nOldTimeDiff;

	double lTime, newTime;
	POSITION pos = m_lnData.GetHeadPosition(), prePos;
	while( pos )
	{
		prePos = pos;
		lTime = m_lnData.GetNext(pos);
		newTime = CDataConversion::CalcTimeDiff(lTime, nBaseTimeDiff, nTimeDiff, strType);
		m_lnData.SetAt(prePos, newTime);
	}
}
double CPacketImplementation::CalcTimeDiff(double dData, int nTimeDiff)
{
	return CDataConversion::CalcTimeDiff(dData, nTimeDiff, m_pParentList->GetBaseTimeDifference(), GetType());
}

//	double	CalcTimeDiff(double dData);

//	double	CalcBackTimeDiff(double dData);		// ���� �ð���� �ǵ����� �Լ�
