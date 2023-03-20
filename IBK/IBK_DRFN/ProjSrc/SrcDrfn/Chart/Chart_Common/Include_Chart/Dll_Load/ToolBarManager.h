// ToolBarManager.h: interface for the ToolBarManager class.
// ����DLL�� �������̽�.  ��������Ŭ����(�������̽�)�� ��������� �� ������ ����... T_T
// ��. �׸� ����� COM�� �Ǿ�����±�. ��~ �̷��� COM���Գ�����.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARMANAGER_H__235DAFB3_898C_471D_8156_332FEDFFC006__INCLUDED_)
#define AFX_TOOLBARMANAGER_H__235DAFB3_898C_471D_8156_332FEDFFC006__INCLUDED_

#include "../BlockBaseEnum.h"
#include "../BlockBaseData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2004.05.25, ��¿�) ���� ToolBar�� ���̰�, ����� Interface�� �����Ѵ�.
//		������ Flag�� �ξ�, ToolBar�� OCX�� �������� �Ʒ��ʿ� ��ġ�ǵ��� �Ѵ�.
#define _OCX_INNERTOOLBAR_HEIGHT 17

class CToolBarManagerImplementation;
class CCommonInterface;
class CPacketList;
class CIndicatorList;
class CMainBlock;

interface IToolBarManager
{
// (2006/2/9 - Seung-Won, Bae) Destroy by OCX (Created in ChartToolBar.dll)
public:
	virtual ~IToolBarManager()	{}

public:
	// ���ٸ� �����Ѵ�.
	virtual bool CreateToolBar(const CString& strToolBarOption, const CString& strImageFile) = 0;
	// ������ ��ġ ����
	virtual void ToolBarPosChanged(const CRect rectToolBarPos) = 0;

	// TR/Real/RemovePacket� ���� Start/End Index�� ����Ͽ� mainblock�� �����Ѵ�.
	virtual void SetData(const int nType, const int nTotalDataCount, const int nPageDataCount, const int nGlanceBalanceMargin = 0) = 0;
	// '����', '����'��ư�� �۾��� �����Ѵ�.
	virtual bool SetMoveScrollPage(bool bMove) = 0;
	// Slider�� �ּҰ��� �����Ѵ�.
	virtual void SetSliderMin(const int nSliderMin) = 0;
	// ����ŸŬ�������� �����Ѵ�.
	virtual void SetControlData(CPacketList* pPacketList, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList) = 0;
	// �׷��� Ÿ���� �޺��� �����Ѵ�.
	virtual void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex) = 0;
	// ������ ������ �޺��� �����Ѵ�.
	virtual void SetLineThickness(int nThickness) = 0;
	// ������ǥ���θ� �����Ѵ�.
	virtual void SetOneChart(bool bOneChart) = 0;
	// �׷��� ������ �����Ѵ�.
	virtual void SetGraphColor(COLORREF clrGraphColor) = 0;
	// ScrollBox�� ��ġ�� �����Ѵ�.
	virtual void SetScrollPos(const int nPos) = 0;
	// ScrollBar�� start index�� end index�� �����Ѵ�.
	virtual void SetStartEndIndex(int &nStart, int &nEnd) = 0;
	// �� ��Ʈȭ��� �������� ����Ÿ������ ���Ҷ� ȣ���Ѵ�.
	virtual void ChangeOnePageDataCount(const int nOnePageDataCount) = 0;

	// ������ ����� �����Ѵ�. (OCX ũ�� �������ϱ� ����)
	virtual CRect GetToolBarRect() const = 0;
	// ����Ÿ�� �����ε����� ���ε����� �����´�.
	virtual bool GetStartEndIndex(int& nStartIndex, int& nEndIndex) = 0;
	// MaxRange���� �����´�.
	virtual int GetMaxRange() = 0;
	// ������ ��ũ�ѹڽ��� ��ġ�� �����Ѵ�.
	virtual int GetCurrentScrollPos() = 0;
	// ����Dialog�� �������ڵ��� �����Ѵ�.
	virtual HWND GetToolBarHwnd() = 0;
	virtual int	GetNumericalInquiryDlgType() const = 0;

	// ���ٸ� �ٽñ׸���.
	virtual void OnDraw() = 0;
	// ��ũ�ѹ� ��������� �����Ѵ�.
	virtual bool IsToolScrollBar() = 0;
	// ���ٿ� �ش� ���������� �ִ��� Ȯ���Ѵ�.
	virtual BOOL GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;
	// ���ٿ� �ش� ���������� ����ϴ��� Ȯ���Ѵ�.
	virtual BOOL UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;
	// Drag�� ���� ZoomIn�۾��� �����Ѵ�.
	virtual bool DragZoomIn(CPoint startPoint, CPoint endPoint, int& nStartIndex, int& nEndIndex, const CRect& rectBlock) = 0;
	// ����ư�� ������ ��Ų��.
	virtual void ReleaseButton(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;

	//---------------��ġ��ȸâ����
	virtual bool CreateNumericalInquiryDlg(const int nType = -1) = 0;		// ����
	virtual bool DestroyNumericalInqiuryDlg() = 0;		// ����
	virtual bool IsUseNumericalInquiryDlg() = 0;		// ��뿩��
	virtual void SetNumericalInquiryDlgMoveState(const short nMove) = 0;	// ��ġ��ȸâ �̵�/����/���콺���󰡱� ����.
	virtual void SetNumericalInquiryDlgBrush(COLORREF clrDlgBrush) = 0;	// ��ġ��ȸâ ������.
	virtual void SetNumericalInquiryDlgType(UINT nType) = 0;	// ��ġ��ȸâ���¸� �����Ѵ�.
	virtual void OnMouseMove(CDC* pDC, CPoint point) = 0;			// ���콺�̵��� ��ġ��ȸâ�� point��ġ�� ����Ÿ�� ����, Dialog�� �̵���Ų��.
	// (2006/2/18 - Seung-Won, Bae) Do not support the Horizontal Line in Numerical Inquiry Dialog
	virtual void	EnableHorizontalLineInNIDLG( BOOL p_bEnable = TRUE) = 0;

	//---------------����
	virtual void CreateToolTip() = 0;	// ���� ����.
	virtual bool ShowToolTip(CPoint point) = 0;	// ���� ���̱�.
	virtual void HideToolTip() = 0;	// ���� ���߱�.
};

class CToolBarManager : public IToolBarManager
{
public:
	CToolBarManager(CCommonInterface* pCommonInterface);
	virtual ~CToolBarManager();

	// ���ٸ� �����Ѵ�.
	virtual bool CreateToolBar(const CString& strToolBarOption, const CString& strImageFile);
	// ������ ��ġ ����
	virtual void ToolBarPosChanged(const CRect rectToolBarPos);

	// TR/Real/RemovePacket� ���� Start/End Index�� ����Ͽ� mainblock�� �����Ѵ�.
	virtual void SetData(const int nType, const int nTotalDataCount, const int nPageDataCount, const int nGlanceBalanceMargin = 0);
	// '����', '����'��ư�� �۾��� �����Ѵ�.
	virtual bool SetMoveScrollPage(bool bMove);
	// Slider�� �ּҰ��� �����Ѵ�.
	virtual void SetSliderMin(const int nSliderMin);
	// ����ŸŬ�������� �����Ѵ�.
	virtual void SetControlData(CPacketList* pPacketList, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList);
	// �׷��� Ÿ���� �޺��� �����Ѵ�.
	virtual void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex);
	// ������ ������ �޺��� �����Ѵ�.
	virtual void SetLineThickness(int nThickness);
	// ������ǥ���θ� �����Ѵ�.
	virtual void SetOneChart(bool bOneChart);
	// �׷��� ������ �����Ѵ�.
	virtual void SetGraphColor(COLORREF clrGraphColor);
	// ScrollBox�� ��ġ�� �����Ѵ�.
	virtual void SetScrollPos(const int nPos);
	// ScrollBar�� start index�� end index�� �����Ѵ�.
	virtual void SetStartEndIndex(int &nStart, int &nEnd);
	// �� ��Ʈȭ��� �������� ����Ÿ������ ���Ҷ� ȣ���Ѵ�.
	virtual void ChangeOnePageDataCount(const int nOnePageDataCount);

	// ������ ����� �����Ѵ�. (OCX ũ�� �������ϱ� ����)
	virtual CRect GetToolBarRect() const;
	// ����Ÿ�� �����ε����� ���ε����� �����´�.
	virtual bool GetStartEndIndex(int& nStartIndex, int& nEndIndex);
	// MaxRange���� �����´�.
	virtual int GetMaxRange();
	// ������ ��ũ�ѹڽ��� ��ġ�� �����Ѵ�.
	virtual int GetCurrentScrollPos();
	// ����Dialog�� �������ڵ��� �����Ѵ�.
	virtual HWND GetToolBarHwnd();
	virtual int	GetNumericalInquiryDlgType() const;

	// ���ٸ� �ٽñ׸���.
	virtual void OnDraw();
	// ��ũ�ѹ� ��������� �����Ѵ�.
	virtual bool IsToolScrollBar();
	// ���ٿ� �ش� ���������� �ִ��� Ȯ���Ѵ�.
	virtual BOOL GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);	
	// ���ٿ� �ش� ���������� ����ϴ��� Ȯ���Ѵ�.
	virtual BOOL UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);
	// Drag�� ���� ZoomIn�۾��� �����Ѵ�.
	virtual bool DragZoomIn(CPoint startPoint, CPoint endPoint, int& nStartIndex, int& nEndIndex, const CRect& rectBlock);
	// ����ư�� ������ ��Ų��.
	virtual void ReleaseButton(CToolOptionInfo::TOOLOPTION enumToolOption);

	//---------------��ġ��ȸâ����
	virtual bool CreateNumericalInquiryDlg(const int nType = -1);		// ����
	virtual bool DestroyNumericalInqiuryDlg();		// ����
	virtual bool IsUseNumericalInquiryDlg();		// ��뿩��
	virtual void SetNumericalInquiryDlgMoveState(const short nMove);	// ��ġ��ȸâ �̵�/����/���콺���󰡱� ����.
	virtual void SetNumericalInquiryDlgBrush(COLORREF clrDlgBrush);	// ��ġ��ȸâ ������.
	virtual void SetNumericalInquiryDlgType(UINT nType);	// ��ġ��ȸâ���¸� �����Ѵ�.
	virtual void OnMouseMove(CDC* pDC, CPoint point);			// ���콺�̵��� ��ġ��ȸâ�� point��ġ�� ����Ÿ�� ����, Dialog�� �̵���Ų��.
	// (2006/2/18 - Seung-Won, Bae) Do not support the Horizontal Line in Numerical Inquiry Dialog
	virtual void	EnableHorizontalLineInNIDLG( BOOL p_bEnable);

	//---------------����
	virtual void CreateToolTip();	// ���� ����.
	virtual bool ShowToolTip(CPoint point);	// ���� ���̱�.
	virtual void HideToolTip();	// ���� ���߱�.

private:
	CToolBarManagerImplementation* m_pManagerImpl;
};

#endif // !defined(AFX_TOOLBARMANAGER_H__235DAFB3_898C_471D_8156_332FEDFFC006__INCLUDED_)
