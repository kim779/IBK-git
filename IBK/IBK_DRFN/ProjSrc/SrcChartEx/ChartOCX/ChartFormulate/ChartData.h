#ifndef __CHARTDATA_H__
#define __CHARTDATA_H__
// ChartData.h : header file
//
#define		DT_NEW		0
#define		DT_ADD		1
#define		DT_INSERT	2
#define		DT_UPDATE	3

/////////////////////////////////////////////////////////////////////////////
// CChartLink window

class CChartDataUpjong
{
public:
	int			m_nDataCount ;
	int			m_nType ;

	CString		m_strData[100] ;
};


//	heeyong
class CChartTimeInfo
{
public:
	CChartTimeInfo()
	{
		m_bInStock			= FALSE;
		m_iTimeXCoordCount	= 0;
		m_iExTime1Index		= 0;
		m_iExTime1Count		= 0;
		m_iExTime2Index		= 0;
		m_iExTime2Count		= 0;
		m_fIncXCoord		= 0;
	};

public:
	BOOL	m_bInStock;
	int		m_iTimeXCoordCount;
	int		m_iExTime1Index;
	int		m_iExTime1Count;
	int		m_iExTime2Index;
	int		m_iExTime2Count;
	float	m_fIncXCoord;
};

class CChartData
{
	// LeeYong ÷����
public:
	CChartData()	{ m_pChartTimeInfo = NULL; }
	~CChartData()	{ if(m_pChartTimeInfo != NULL) delete m_pChartTimeInfo; }
	//<- LeeYong
public:
	int			m_nDataCount;			// ����Ÿ �Ѱ���
	CString		m_strTitle;	
	BOOL		m_bFixMinMax ;
	int			m_nDataType;			//������ 2001. 1. 11. --> ��Ʈ����Ÿ�� ��,��,��,��,ƽ,����,���� ���� ����  
										
	int			m_nDate[2048];			//	��¥
	int			m_nStart[2048];			//  �ð�
	int			m_nTop[2048];			//	��
	int			m_nBottom[2048];		//	����
	int			m_nEnd[2048];			//	����
	int			m_nVolume[2048];		//	�ŷ���

	CPoint		m_nMinMax;				//	���簡�� �ִ��ּ�
	int			m_nVMax;				//	�ŷ��� �ִ�

	//////////////////////////////////////////////////
	// LeeYong ÷��
	class CChartTimeInfo *	m_pChartTimeInfo;
	//////////////////////////////////////////////////

};

class CChartData2nd
{
public:
	int			m_nDataCount;
	CString		m_strTitle;
	int			m_nChartType;
	int			m_nDataType;
										
	int			m_nDate[1024];
	int			m_nData[1024];
										
	CPoint		m_nMinMax;
};

class CChartDataYesterday
{
public:
	int		m_nDate ;
	int		m_nStart ;
	int		m_nTop ;
	int		m_nBottom ;
	int		m_nEnd ;
	int		m_nVolume ;
};

class CChartDataAV
{
public:
	int			m_nDataCount;
	int			m_nEnd[1024] ;
	int			m_nVolume[1024] ;
};

class CChartDataOne
{
public:
	int		m_nDataCount ;
	CString	m_strRealField ;
	CString	m_strTitle ;
	CString	m_strUnit ;
	BOOL	m_bFixMinMax ;
	BOOL	m_bScaleJoin ;

	int		m_nData[2048] ;

	CPoint	m_nMinMax ;
};

class CChartDataTime
{
public:
	int		m_nDataCount ;
	int		m_nDate[2048] ;
};
/////////////////////////////////////////////////////////////////////////////
#endif 

