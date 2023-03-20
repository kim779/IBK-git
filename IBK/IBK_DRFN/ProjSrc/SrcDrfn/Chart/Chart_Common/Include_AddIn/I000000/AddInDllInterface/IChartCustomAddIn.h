// IChartCustomAddIn.h: interface for the IChartCustomAddIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTCUSTOMADDIN_H__32B0A34F_2D5F_4B70_A70C_29C44DCB4187__INCLUDED_)
#define AFX_ICHARTCUSTOMADDIN_H__32B0A34F_2D5F_4B70_A70C_29C44DCB4187__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. ����! Chart OCX�� �̿��ϴ� �� DLL���� Special Interface�̴�.
//		���� ������ Chart OCX���� Header�� �����Ͽ����ϸ�,
//		 Ư�� AddIn DLL���� �� Interface Header�� ���� �̿��� �� ����.
//			(�ٸ� �̸����� �������� ��)
//		Chart OCX���� ������ Interface�� ������ �ʿ䰡 ���� ���,
//		 �� Header�� �����ϸ�, CSiteCustomAddin::GetAddInCustomInterface()�� �������� �ʾ�
//		 CSiteCustomAddin::GetAddInCustomInterface()�� �����ǵ��� �ʵ��� �Ѵ�.
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartCustomAddIn : public IUnknown  
{
protected:
	virtual ~IChartCustomAddIn()	{}

// Temp Interface I	: TimeMark Interface ~ �� ���������� �����ϵ��� �ϴ� Interface�̴�.
public:
	// [04/10/11] TimeMark�� Clear��Ű�� Interface�� �����Ѵ�.
	virtual	void	OnClearTimeMark( void) = 0;
	// [04/10/11] TimeMark�� �����ϴ� Interface�� �����Ѵ�.
	virtual	void	OnSetTimeMark( const char *p_szMarkTime, BOOL p_bWithScroll) = 0;
	// [04/10/11] �ð������� ������ ����ϴ� Flag�� �ξ� ������ �� �ֵ��� �Ѵ�.
	//				�߽��� �������� �ʴ´�.
	virtual void	OnEnableReceivingTimeShare(BOOL p_bEnable) = 0;

// Temp Interface II	: BoundMark Interface ~ �� ���������� �����ϵ��� �ϴ� Interface�̴�.
public:
	// [04/10/13] ����/������ ǥ�ø� Ȱ��ȭ��Ű�� Interface�� �����Ѵ�.
	virtual void	OnEnableBoundMark( BOOL p_bEnable) = 0;
	// [04/10/13] Get BoundMark Flag
	virtual BOOL	GetBoundMarkFlag( void)	= 0;
	//sy 2005.11.21. ->BoundMark Flag �� �����Ǿ� �ִ����� ����(�������).
	virtual bool	IsExistenceBoundMark( void) = 0;

// Temp Interface III	: ����Ǵ� Option ����� Interface�� �����Ѵ�.
public:
	// [04/10/18] Image�� �����ϴ� Interface�� �����Ѵ�.
	virtual BOOL	OnSaveToImage( void) = 0;
};

#endif // !defined(AFX_ICHARTCUSTOMADDIN_H__32B0A34F_2D5F_4B70_A70C_29C44DCB4187__INCLUDED_)
