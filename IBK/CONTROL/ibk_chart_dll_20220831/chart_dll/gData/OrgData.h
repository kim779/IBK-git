// OrgData.h: interface for the COrgData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORGDATA_H__1BBA9100_41BA_40A7_9AD8_E35EF05C24FB__INCLUDED_)
#define AFX_ORGDATA_H__1BBA9100_41BA_40A7_9AD8_E35EF05C24FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#define	RTM_NO		0
#define	RTM_UPDATE	1
#define	RTM_ALL		2
#define	RTM_SHIFT	3

class AFX_EXT_CLASS COrgData  
{
public:
	COrgData(char *pData, class CDataMgr *pDataMgr);
	virtual ~COrgData();

private:
	bool	m_bCalc;
	bool	m_bCalcV;
	bool	m_bCalcA;
	bool	m_bDateAdd;
	bool	m_bRealtime;
//	bool	m_bNoDelete;

	int	m_iDtKey;	// data key - ����Ÿ ������
	int	m_iDtUnit;	// Data Unit - ����/�ɼ�..
	int	m_iDtIndex;	// Data Index - ��, ��, ��..
	int	m_iDtPattern;	// dpNONE, dpBASIC, dpVIDX
	int	m_iMGap;	// xxx : xx�� ��Ʈ
				// 9xx : xx�� ��Ʈ
	int	m_iTGap;		// xxx : xxƽ ��Ʈ
	int	m_iLastTick;	// last nTick
	int	m_iRyy;		// ��������
	int	m_iRmm;		// ��������
	int	m_iRdd;		// ��������
	int	m_iCyy;		// �ֱ� ��������
	int	m_iCmm;		// �ֱ� ��������
	int	m_iCdd;		// �ֱ� ��������
	int	m_iShh;		// ����۽ð�
	int	m_iSmm;		// ����۽ð�

	int	m_iSign;	// �����ȣ

	double	m_dMax;
	double	m_dMin;
	double	m_dVMax;
	double	m_dVMin;
	double	m_dAMax;
	double	m_dAMin;
	double	m_dCurr;	// ���簡
	double	m_dPCurr;	// ���ϰ�
	double	m_dSahg;	// ���Ѱ�
	double	m_dHahg;	// ���Ѱ�
	double	m_dUdyl;	// �����
	double	m_dStock;	// �� �ֽļ�
	char	m_cStockKind;	// �Ҽ� : 1-�ŷ���, 2-�ڽ���

	CString	m_strCODX;	// realtime code
	CString	m_strJName;	// �����/��Ÿ...

	class	CDataMgr*	m_pDataMgr;

	//CArray	<char*, char*> m_arDataQue;	// Data Array
	CArray	<class CGrpBasic*, class CGrpBasic*> m_arDataQue;	// Data Array

public:
	CString	GetName()	{ return m_strJName; }
	CString	GetCode()	{ return m_strCODX; }
	int	GetKey()	{ return m_iDtKey; }
	int	GetUnit()	{ return m_iDtUnit; }
	int	GetIndex()	{ return m_iDtIndex; }
	int	GetPattern()	{ return m_iDtPattern; }
	double	GetCurr()	{ return m_dCurr; }
	double	GetPCurr()	{ return m_dPCurr; }
	double	GetSahg()	{ return m_dSahg; }
	double	GetHahg()	{ return m_dHahg; }
	int	GetSign()	{ return m_iSign; }
	double	GetUdly()	{ return m_dUdyl; }
	double	GetStocks()	{ return m_dStock; }
	char	GetStockKind()	{ return m_cStockKind;	}
//	bool	GetNoDeleteData()	{ return m_bNoDelete; }
//	void	SetNoDeleteData(bool bNoDelete)	{ m_bNoDelete = bNoDelete; }


	int	GetDataGap();
	int	GetArraySize();
	class CGrpBasic*	GetGraphData(int iIndex);
	bool	AttachGraphData(int iCount, int iDummy, char* pcData, int& riDataL, char* pcCurDate, bool bTodayData);
	bool	AttachGraphData2(int count, int dummy, char *pData, int &dataL, int id);
	bool	AttachMapData(int iCount, char* pcData, int& riDataL, char* pcCurDate, bool bTodayData);
	//int	UpdateRTM();
	int	UpdateRTM(bool& rbIncrease);
	int	UpdateCntRTM();
	void	DetachGraphData();
	bool	CreateDataQue(int date);
	void	SetCurr(CString sData);
	void	SetStock(CString sData);
	void	SetStockKind(char cKind);
	
	bool	CalculateMinMax(int iStart, int iEnd, double *pdMin, double *pdMax, int iCOption = 0, int iCKind = 0, bool bForce = false);
	bool	CalculateLastIndex(int cOption = 0, int cKind = 0);
	bool	CalculateBasic(int iStart, int iEnd, double* pdMin, double* pdMax, int iCOption, int iCKind);
	bool	CalculateTick(int start, int end, double *Min, double *Max, int cOption, int cKind);

private:
	int	CharToInt(const char* cnvB, int cnvL);
	double	CharToDouble(const char* cnvB, int cnvL);
	int	GetToken(char *pData, char *token);
	void	SetTime(int min, int cHH, int cMM, int cSS, unsigned char &nHH, unsigned char &nMM, unsigned char &nSS);
};

//** 2005.11.09 macho
#define ClearInvData() { gBasic->orgnvol = gBasic->orgnamt = gBasic->frgnvol = gBasic->frgnamt = gBasic->mgjy = gBasic->fobtain = gBasic->fexhaust = 0; }
//**


#endif // !defined(AFX_ORGDATA_H__1BBA9100_41BA_40A7_9AD8_E35EF05C24FB__INCLUDED_)
