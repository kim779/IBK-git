// BoundMarkerAddInImp.h: interface for the CBoundMarkerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNDMARKERADDINIMP_H__94AF516C_0B05_41DA_B10F_36D86C28A709__INCLUDED_)
#define AFX_BOUNDMARKERADDINIMP_H__94AF516C_0B05_41DA_B10F_36D86C28A709__INCLUDED_

// (2004.09.14, ��¿�) ������ ���Ѱ�/���Ѱ��� ����, ǥ���ϴ� Library Routine Class�̴�.
// (2004.10.22, ��¿�, �м�) �� ��ɵ��� ���� ������Ʈ�� ����/������ ����/������ ����Ʈ�� �����ϸ鼭,
//			��/���Ѱ��� ǥ���ϴ� ����� ó���ϴ� ���̴�.
//		Ư�� �� ����� ��Ʈ�� 1���� Graph���� ������ �� �ִ� �������
//			(����� Data ������ AddIn �� 1���� ���ѵ� ������� �����̴�.)
//			ù��° ������Ʈ���� �����Ǵ� ������� �����Ѵ�.

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/3/10
// Comments		: 
// Using way	: 
// See			: 
//
// Help Message Reference : BOUNDMARK
//	{{{
//		BOUNDMARK : ������Ʈ�� ������ ���� Scale�� �ش� ������ �����Ѽ��� ǥ���ϵ��� �˷��ִ� ���� �޽���
//			[Type I] ������ Bar ǥ��
//				BOUNDMARK=GN:UB,LB
//					GN = ������Ʈ�� Graph Name (Map�� ������ ���뿡 ���� �����)
//					UB = �ش� ������ ���Ѱ�
//					LB = �ش� ������ ���Ѱ�
//			[Type II] CAskingPriceIndicator Class Header ����.
//	}}}
//-----------------------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn_381931/I381931/_ChartAddInBase.h"		// for CChartAddInBase

interface IChartOCX;
interface IChartManager;
class CBoundMarkerAddInImp : public CChartAddInBase
{
public:
	CBoundMarkerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CBoundMarkerAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;
	CMapStringToOb		*m_pmapOrder;
	CString				m_strItemCode;
	BOOL				m_bDraw;
// 0. Bound ���� �����Ѵ�. (Graph Name�� ������ Bound�� �������� ���� �����̴�.)
protected:
	double	m_dUpperBound;
	double	m_dLowerBound;
	double	m_dYesterdayClosePrice;//sy 2005.11.18. ���� ����
	CString	m_strIndicatorName;

// (2004.09.14, ��¿�) Bound���� �����ϴ� Interface�� �����Ѵ�.
protected:
	virtual void	ResetBound( const char *p_szBound);

// (2004.09.30, ��¿�) BoundMark�� �̿����� ���θ� �����޾� ����/�����Ѵ�.
protected:
	bool	m_bExistenceBoundMark; //sy 2005.11.21. m_bEnableMark �� ���õǾ� �ִ����� üũ�Ѵ�.
	BOOL	m_bEnableMark;
protected:
	void	EnableBoundMark( BOOL p_bEnable);
	BOOL	GetBoundMarkFlag( void)				{	return m_bEnableMark;	}
	//sy 2005.11.21.
	bool	IsExistenceBoundMark( void)			{	return m_bExistenceBoundMark;	}

//sy 2005.11.18. �������� (�����ѹ��� ����)
private:
	void SetYesterdayClosePrice(const CString& strData);
	double GetYesterdayClosePrice() const;
	double GetSteadyPrice() const;

// (2004.10.27, ��¿�) View�� ȯ���� �����Ͽ�, CAskingPriceIndicator���� �̿��� �� �ֵ��� �Ѵ�.
protected:
	BOOL		m_bLog;						// ��� Graph�� ��ϵ� Block�� Log ��Ʈ ���� �����̴�.
	BOOL		m_bReverse;					// ��� Graph�� ��ϵ� Block�� ������ ���� �����̴�.
	CRect		m_rctBoundMark;				// ����/���� ǥ�� �����̴�.
	CRect		m_rctGraphRegion;			// View�� ��ü Graph �����̴�. (Block�� View �����̴�.)
	CRect		m_rctGraphDrawingRegion;	// View�� Graph Drawing�����̴�. (���� Graph�� ��ǥ �����̴�.)
	double		m_dViewMin;					// View���� ��� Graph�� View�� Minimun Scale ���̴�.
	double		m_dViewMax;					// View���� ��� Graph�� View�� Maximum Scale ���̴�.
	CString		m_strExtMsg;				// ���� Message�� �ܿ� �κ��̴�.
											//		(�⺻���� '�׷�����:����,����'���� �����Ǹ� �ڿ� ':�����ڵ�'�� �߰��� �� �ִ�.)

// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
protected:
	// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);
};

#endif // !defined(AFX_BOUNDMARKERADDINIMP_H__94AF516C_0B05_41DA_B10F_36D86C28A709__INCLUDED_)
