// IWChartManager050531.h: interface for the CIWChartManager050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWCHARTMANAGER050531_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
#define AFX_IWCHARTMANAGER050531_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����!	050531 Version�� AddIn���� �̿�Ǵ� OCX�� Interface�� Wrapper�� ��� �߰��� �����մϴ�.
//		��� ������ Type ������ ������� �ʽ��ϴ�.
//		������ �ʴ� ������ 050531 Version�� ��� AddIn Module�� ���� Site OCX��
//			�Բ� �����Ǿ� �������� ���� �����Ǿ�� �մϴ�.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			��ȭ			��¿�
//		ChartAssistIndexAddIn		��ȭ			���ٹ�
//////////////////////////////////////////////////////////////////////

// (2004.10.11, ��¿�) MainBlock�� Interface Wrapper Class�� �����Ѵ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"
#include "IChartManager050531.h"

class CMainBlock;
class CIWChartManager050531 : public CIWUnknown, public IChartManager050531
{
public:
	CIWChartManager050531( CMainBlock *p_pMainBlock);
	virtual ~CIWChartManager050531();

// (2004.10.07, ��¿�) Interface Wrapper�� �⺻ Interface (IUnknown��)�� �����Ѵ�.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.11, ��¿�) MainBlock Object�� Pointer�� �����Ѵ�.
protected:
	CMainBlock *	m_pMainBlock;

// [04/10/11] Block�� ��� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	virtual int		GetRowCount( void);
	virtual int		GetColumnCount( void);

// [04/10/11] Block�� ������ ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// Block Region
	virtual CRect	GetBlockRegion( int p_nRowIndex, int p_nColumnIndex);	
	// Graph Region
					// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
	virtual CRect	GetNoFrameGraphRegion( int p_nRowIndex, int p_nColumnIndex);
	virtual CRect	GetNoFrameGraphRegion( CPoint p_pPoint);
					// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
	virtual CRect	GetDrawingGraphRegion( int p_nRowIndex, int p_nColumnIndex);
	virtual CRect	GetDrawingGraphRegion( CPoint p_pPoint);

// [04/10/13] Block�� Graph Data�� ��ȸ�ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/13] Block���� �ش� ��ǥ�� ��ϵǾ� �ִ��� Ȯ���ϴ� Interface�� �����Ѵ�.
	virtual BOOL	CheckGraphInBlock( int p_nRowIndex, int p_nColumnIndex, const char *p_szGraphName);
	// [04/10/26] �����Ǵ� Graph�� Data Packet Name�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetPacketNameOfGraph( const char *p_szGraphName, CString &p_strPacketNames);
	// [04/10/27] Ư�� Graph�� ��ϵ� ù Block�� Index�� ��ȸ�ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetBlockIndexOfGraph( const char *p_szGraphName, int &p_nR, int &p_nC);

// [04/10/13] View�� ������ ���ϴ� Interface�� �����Ѵ�.
public:
	// [04/10/13] �ش� ��ǥ�� �ִ� View�� Min/Max���� �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	GetMinMaxDataInViewWithGraph( const char *p_szGraphName, double &p_dMin, double &p_dMax);
	// [04/10/27] ������ Block�� Y Scale Conversion Type�� ���Ѵ�.
	virtual BOOL	GetVerticalScaleConversionType( int p_nR, int p_nC, int &p_bLog, int &p_bReverse);
};

#endif // !defined(AFX_IWCHARTMANAGER050531_H__7FD49D7A_F392_4267_982D_CBFE6535E786__INCLUDED_)
