// IChartManager050531.h: interface for the IChartManager050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTMANAGER050531_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
#define AFX_ICHARTMANAGER050531_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		Interface ���� ���� ���� ������ �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//		ChartAssistIndexAddIn		��ȭ			���ٹ�
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) MainBlock �� Object Interface�� �����Ѵ�.
//		AddIn DLL ���� Interface�� Macro Interface�� �����Ѵ�.
#if IChartManagerVer < 050531
	#define	IChartManager		IChartManager050531
	#undef	IChartManagerVer
	#define	IChartManagerVer	050531
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartManager050531 : public IUnknown  
{
protected:
	virtual ~IChartManager050531()	{}

// [04/10/11] Block�� ��� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	virtual int		GetRowCount( void) = 0;
	virtual int		GetColumnCount( void) = 0;

// [04/10/11] Block�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// Block Region
	virtual CRect	GetBlockRegion( int p_nRowIndex, int p_nColumnIndex) = 0;	
	// Graph Region
					// View�� ��ü Graph �����̴�. (Block�� View �����̴�.)
	virtual CRect	GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex) = 0;	
	virtual CRect	GetNoFrameGraphRegion( CPoint p_pPoint) = 0;
					// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
	virtual CRect	GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex) = 0;
	virtual CRect	GetDrawingGraphRegion( CPoint p_pPoint) = 0;

// [04/10/13] Block�� Graph Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/13] Block���� �ش� ��ǥ�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName) = 0;
	// [04/10/26] �����Ǵ� Graph�� Data Packet Name�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames) = 0;
	// [04/10/27] Ư�� Graph�� ��ϵ� Block�� Index�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC) = 0;

// [04/10/13] View�� ������ ���ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/13] �ش� ��ǥ�� �ִ� View�� Min/Max���� �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax) = 0;
	// [04/10/27] ������ Block�� Y Scale Conversion Type�� ���Ѵ�.
	virtual BOOL	GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse) = 0;
};

#endif // !defined(AFX_ICHARTMANAGER050531_H__E56B298C_2C27_477D_8D92_4510983D4417__INCLUDED_)
