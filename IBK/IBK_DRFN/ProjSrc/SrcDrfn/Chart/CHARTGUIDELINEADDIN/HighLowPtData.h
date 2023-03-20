#if !defined(AFX_HIGHLOWPTDATA_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_)
#define AFX_HIGHLOWPTDATA_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HighLowPtData.h : header file
//


// Init varialbe value with Zero
#define VALUE_ZERO				0


// Changing Point Satte
#define CHANGINGPT_HIGHTOLOW	0x0001		// �ֿ����
#define CHANGINGPT_LOWTOHIGH	0x0002		// �ֿ�����

/////////////////////////////////////////////////////////////////////////////
// CCheChartData Object	-- ��Ʈ�� �׷��� ü�� ����Ʈ 
class CCheChartData : public CObject
{
public:	
	CString			m_strJmCode	;	//�����ڵ�
	CString			m_strChetime	;	//ü��ð�	
	double			m_dChedanga	;	//ü��ܰ�	
	int				m_nMesuMedoGb;	//�ŵ��ż�����
	

// Construction, Destruction
	CCheChartData();
	virtual ~CCheChartData();
};


/////////////////////////////////////////////////////////////////////////////
// CHighLowPtData Object
class CHighLowPtData : public CObject
{
public:
	int				m_nPointGb;					// ���� ���� ����

	int				m_nDataSequence;			// ������ ����
	double			m_dTime;					// �ð�
	
	double			m_dOpen;					// �ð�
	double			m_dHigh;					// ��
	double			m_dLow;						// ����
	double			m_dClose;					// ����

// Construction, Destruction
	CHighLowPtData();
	virtual ~CHighLowPtData();
};


/////////////////////////////////////////////////////////////////////////////
// CMAData Object
class CMAData : public CObject
{
public:
	int				m_nDataSequence;			// ������ ����

	double			m_dMMa;						// �����̵����
	
// Construction, Destruction
	CMAData();
	virtual ~CMAData();
};


/////////////////////////////////////////////////////////////////////////////
// CRegistNSupportData Object
class CRegistNSupportData : public CObject
{
public:
	int				m_nPointGb;					// ����, ���� ����

	int				m_nDataSeq1;				// ù��° ������ ����
	double			m_dYPoint1;					// ù��° ������ ��

	int				m_nDataSeq2;				// �ι�° ������ ����
	double			m_dYPoint2;					// �ι�° ������ ��
		
// Construction, Destruction
	CRegistNSupportData();
	virtual ~CRegistNSupportData();
};


/////////////////////////////////////////////////////////////////////////////
// CPatternEnvData Object
class CPatternEnvData : public CObject
{
public:
	int				m_nMAPeriod;					// �̵���� �Ⱓ

	BOOL			m_bDrawMMa;						// �����̵���� ����
	BOOL			m_bDrawHighLowPtLine;			// ����, ���� �������
	
	BOOL			m_bDrawHighPtHLine;				// ���� �������
	BOOL			m_bDrawLowPtHLine;				// ���� �������

	BOOL			m_bDrawHighRegistLine;			// ���� ���׼�
	BOOL			m_bDrawLowSupportLine;			// ���� ������

	double			m_dHLPtAllowalbeError;			// �ֿ����, ���� ���� ����, ���� ���̰� n% �̳��� ��� �����ͷ� ������� ����
	double			m_dNextPtAllowableError;		// �ΰ��� �ֿ������� ����α��� ���������� ���̰� n%�̳��� ��� �����ͷ� ���

	BOOL			m_bFilterSideWalkPeriod;		// Ⱦ������ ���͸� ����

	

// Construction, Destruction
	CPatternEnvData();
	virtual ~CPatternEnvData();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIGHLOWPTDATA_H__F56E67E4_D461_4DD4_A04E_087E6261AD1A__INCLUDED_)
