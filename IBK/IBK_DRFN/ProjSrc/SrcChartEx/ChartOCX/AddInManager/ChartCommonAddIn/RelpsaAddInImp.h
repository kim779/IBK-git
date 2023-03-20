// RelpsaAddInImp.h: interface for the CRelpsaAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RELPSA_H__65FB3CF0_12B9_4FA9_8856_8816E58FE251__INCLUDED_)
#define AFX_RELPSA_H__65FB3CF0_12B9_4FA9_8856_8816E58FE251__INCLUDED_

// (2004.10.14, ��¿�) RelpsaAddInImp (Reset Last PriceS After), �� �����޽����� ������ �ð������� Real�� ���Žÿ���
//			�� ����(��/��/��/��)�� ��� ������ ���Ͻ�Ų��. �ӽ� �Ϻ� Data�� Clear�ϴ� ����̴�.
//		Ư�� �Ϻ����� ���������ν� Real �������Ŀ� ������ ��/��/������ ���Ͻ�Ű�� ������� �����Ѵ�.
//		����, AP������ �Ϻ��̸鼭 Reset ���Ѿ߸� �ϴ� ���񿡼��� �� ���� �޽���(HHMMSS)�� ������� �Ѵ�.
// (2004.10.22, ��¿�) �� ��ɵ��� ���� ������Ʈ�� ����/������ ����/������ ����Ʈ�� �����ϸ鼭,
//			���� ���� ǥ�ø� Reset ��Ű�� ����� ó���ϴ� ���̴�.
//			(Ư�� ���� �������� ������ ����/�������� Real Packet�� �˸´� �ð��� ���� �������� �ʾ�,
//				���� ������ �ð� ���� ������ �ذ��ϱ� ���� ����̴�.)
//		Ư�� �� ��ɵ��� ��Ʈ�� 1���� Graph���� ������ �� �ִ� �������
//			(����� Data ������ AddIn �� 1���� ���ѵ� ������� �����̴�.)
//			'�ð�/��/����/����'��� �̸��� Packet�� �̿��ϴ� Graph���� ȿ���� �����Ѵ�.
// (2004.11.30, ��¿�) �� ����� �����Ϸ��� �ϴ� Real�� �����޽����� ������ �����ڵ�� ��û�� "�ڷ�����" Real��
//		���ŵǴ� ��쿡���� �����ϵ��� �����Ѵ�.
//		*. ����! ��ü��� ������ Real�� ���ŵ� ��� �������ϰ� �ȴ�.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

class CRelpsaAddInImp : public CChartAddInBase
{
public:
	CRelpsaAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CRelpsaAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2004.10.14, ��¿�) �ʱ�ȭ ���� �ð�(����� �ð�)�� ������ �����Ѵ�.
protected:
	double		m_dResetTime;
	CString		m_strResetItemCode;
public:
	void		ResetRELPSA( const char *p_szResetMsg);

// (2004.10.14, ��¿�) Real�� �����Ͽ� Reset�� �ð������� Ȯ��/ó���Ѵ�.
protected:
	// (2004.10.15, ��¿�) OnDrdsPacketEnd()������ ���ϴ� Real�̾����� �ĺ��� �� ���� OnDrdsData���� ��� ��Ȳ �Ǵ��� ���� ���� �����Ѵ�.
	BOOL		m_bOnResetReal;

// (2004.10.15, ��¿�) ���� Data �ʱ�ȭ�� ���� Real ���� ȯ���� Clear�ϴ� Routine�� �����Ѵ�.
protected:
	void		ClearRELPSA( void);

// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
protected:
	// RQ�� Packet : �������� - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2007/2/8 - Seung-Won, Bae) On Real Data
	virtual void	OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);
};

#endif // !defined(AFX_RELPSA_H__65FB3CF0_12B9_4FA9_8856_8816E58FE251__INCLUDED_)
