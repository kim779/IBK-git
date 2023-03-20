#ifndef OUTSIDECOMMONINTERFACE_H_
#define OUTSIDECOMMONINTERFACE_H_
//////////////////////////////////////////////////////////////////////////
// OutsideCommonInterface.h
// Chart OCX�� MultiChart DLL�� �̰��� ��ӹ޾� �ʿ��� �Լ��� �����Ѵ�.
// OCX�� DLL�� �� �������̸� ���� ���� �ǻ������ �Ѵ�.
//////////////////////////////////////////////////////////////////////////
#include <AFXTEMPL.H>

#include "BlockBaseEnum.h"			// for CToolOptionInfo

// (2004.10.11, ��¿�) DLL Container�� Communication�� ���� Message�� �����Ѵ�.
// (2004.05.06, ��¿�) Chart OCX���� ������ Event Message�� �����Ѵ�.
//		WPARAM	: ID
//		LPARAM	: MSG Structure Address
//			MSG msg;
//			msg.hwnd	= GetSafeHwnd();
//			msg.message	= message;
//			msg.wParam	= wParam;
//			msg.lParam	= lParam;
#define _CHARTMSG_EVENTMESSAGE	"CHARTMSG_EVENTMESSAGE"
extern const UINT RMSG_CHARTMSG_EVENTMESSAGE;

// (2004.05.18, ��¿�) RMSG_CHARTMSG_EVENTMESSAGE�� ���޵Ǵ� Event Message Structure�� ������ Message�� ���޵ȴ�.
// (2004.05.18, ��¿�) DLL Container���� �� ȭ���� Data �� (ZoomValue)�� ��ȭ�� �����Ѵ�.
//		�̶�, LParam�� ���� ZoomValue�̴�.
#define UWM_ZOOMVALUE				( WM_USER + 0x2D36)
// (2004.06.04, ��¿�) Scroll�� ����� ������ DLL Container���� ���Ͻø� �����Ѵ�.
//		�̶�, LParam�� ���� �� ȭ���� ������ Data�� �Ͻø� Long Value�� �����ϰ� �ȴ�.
#define UWM_VIEWENDTIME				( WM_USER + 0x2D37)
// (2004.10.08, ��¿�) ���ڼ� �̿���� ��ġ��ȸâ �̿뿩�� ����� Multi-Chart���� �˷�,
//		�ٸ� ������Ʈ���� �� ������ ����ǵ��� �Ѵ�.
//		�̶�, WParam�� ���� ������ ������ ��ġ��ȸâ �̿뿩�� �������̴�.
#define UWM_USEINQUIRY_CROSSLINE	( WM_USER + 0x2D39)
// (2004.11.04, ��¿�) ��Ʈ�� Global ���� Data �߻��� DLL Container���� �˸��� Message�̴�.
//		�̶� LParam���� "������;����Data"�� ���·� Data�� ���޵ȴ�.
//		�⺻������ Chart�� �ð�����, �Ϲ�ȣ������, Ư��ȣ������(ȭ�麰���ѺҰ�)�� �߻���Ų��.
#define UWM_FIREGSHAREDATA			( WM_USER + 0x2D3B)
// (2005/2/17 - Seung-Won, Bae) Term Analysis Infomation Notify I (Drag Start Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGSTART				( WM_USER + 0x2D3C)
// (2005/2/17 - Seung-Won, Bae) Term Analysis Infomation Notify II (Drag End Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGEND					( WM_USER + 0x2D3D)
// (2005/10/25 - Sun-Young, Kim) ��Ʈ ��ü�ʱ�ȭ
#define UWM_ORGRETURN				( WM_USER + 0x2D40)
// ���õ� ��Ʈ �˸� : �������� - ojtaso (20070122)
#define UWM_SELECTEDCHART			( WM_USER + 0x2D46)
// ���������� Total Count�� ��Ƽ��Ʈ�� �˷��� : �������� - ojtaso (20070704)
#define UWM_MULTITOTALCOUNT			( WM_USER + 0x2D47)

class COutsideCommonInterface
{
// (2006/11/18 - Seung-Won, Bae) This Interface used with Only COutsideCommonInterfaceFunction class.
friend class COutsideCommonInterfaceFunction;
protected:
	// OutsideCommonInterface �� type
	enum TYPE_OCIF
	{
		BASE_TYPE = 0,
		MC_TYPE_ORG,
		MC_TYPE_061115,		// (2006/11/15 - Seung-Won, Bae) Add New Interface Type.
		// (2006/11/18 - Seung-Won, Bae) Caution!
		//		The Next Version Interface must support the Previous Version Interface Correctly.
		//		Because, Old Version Supported Interface Checking is processed with Version Nember by '<=' (Greater than or Equal to)
	};

protected:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return BASE_TYPE; };

protected:
	// ����ó����.
	virtual void    SetRealInfo		(	void* pDrds,		// Drds�������ε� NULL�� ����.
										HWND hWnd,			// Chart OCX�� ������ �ڵ�
										UINT id, 			// CPacket* 
										LPSTR DataName,		//  ex: "S31"
										LPSTR FieldName, 	//  ex: "time"
										LPSTR KeyName)	{ };//  ex: "000660"

	virtual void    UnSetRealInfo	(	void* pDrds,		// Drds�������ε� NULL�� ����.
										HWND hWnd,			// Chart OCX�� ������ �ڵ�
										UINT id, 			// CPacket* 
										LPSTR DataName,		//  ex: "S31"
										LPSTR FieldName, 	//  ex: "time"
										LPSTR KeyName) { };	//  ex: "000660"


	// ��� �� UserID
	virtual LPCTSTR GetMainPath() const { return _T(""); };
	virtual LPCTSTR GetUserID() const { return _T(""); };

	// TR Data��û
	virtual void RequestTRPacket(const short nID, const LPCTSTR pcszPacketNames, const short nCount, const bool bIsNew) { };

	// ������ǥ Load ������ ���� ����
	virtual bool IsLoad_MarketChart() const { return false; };

};

// ��Ƽ��Ʈ�� �������̽�
class CMChartInterface : public COutsideCommonInterface
{
// (2006/11/18 - Seung-Won, Bae) This Interface used with Only COutsideCommonInterfaceFunction class.
friend class COutsideCommonInterfaceFunction;

protected:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return MC_TYPE_ORG; };

protected:
	// ����Ÿâ.
	virtual bool	IsActiveDataWindow()										{ return false; };
	virtual void	SetDataWindowData(CList<CString, CString&>* pListData)		{ };

	// ȯ�漳��.
	virtual bool	SetImaginaryCandleAndGapRevisionToOutside( const bool bImaginaryCandle, const bool bUseGapRevision)
																				{ return false; };	// ��������� �� ������
	virtual bool	SetInquiryDataWindowTypeToOutside(const int nWindowType)	{ return false; };	// ��ġ��ȸâ ǥ�ÿ��� 0:����, 1:Window
	virtual bool	SetInquiryDataWindowMoveTypeToOutside(const int nMoveType)	{ return false; };	// ��ġ��ȸâ �̵����¼��� 0:�̵�, 1:����
	virtual bool	SetYScalePositionToOutside(const int nPosition)				{ return false; };	// Y�� ����ǥ�� (0:����, 1:������, 2:����, 3:����)
	virtual bool	SetRightMarginToOutside(const int nRightMargin)				{ return false; };	// ������ ����
	virtual bool	SetShowIndicatorVariableToOutside(const bool bShow)			{ return false; };	// ��ǥ���� - ��ǥ���� ǥ��
	virtual bool	SetShowIndicatorValueToOutside(const bool bShow)			{ return false; };	// ��ǥ���� - ��ǥ�� ǥ��
	virtual bool	SetShowHighLowBoundToOutside(const bool bShow)				{ return false; };	// ��/���Ѱ� ǥ��
	virtual bool	SetShowExRightDividendToOutside(const bool bShow)			{ return false; };	// �Ǹ���/���� ǥ��
	virtual bool	SetShowCurrentPriceToOutside(const int nShowType)			{ return false; };	// ���ݴ������� ���簡ǥ��
	virtual bool	SetTrendLineColorToOutside(const COLORREF clrLineColor)		{ return false; };	// �߼��� ����
	virtual bool	SetShowTrendLineValueToOutside(const int nPosition)			{ return false; };	// �߼����� ǥ�� (0: , 1: , 2: , 3: )
	virtual bool	SetTimeLinkageToOutside(const bool bTimeLinkage)			{ return false; };	// �ð����� ����
	virtual bool	SetAutoSaveToOutside(const bool bAutoSave)					{ return false; };	// ��Ʈ����� �ڵ� ���� ����
	virtual bool	SetShowTrendLineTextToOutside(const bool bShow)				{ return false; };	// �߼���Text ���̱�
	virtual bool	SetShowTimeLineTextToOutside(const bool bShow)				{ return false; };	// �ð���Text ���̱�
	virtual bool	SetShowPriceLineLeftTextToOutside(const bool bShow)			{ return false; };	// ���ݼ� ���� Text ���̱�
	virtual bool	SetShowPriceLineRightTextToOutside(const bool bShow)		{ return false; };	// ���ݼ� ������ Text ���̱�
	virtual bool	SetShowTrisectionLineTextToOutside(const bool bShow)		{ return false; };	// ���м� Text ���̱�
	virtual bool	SetShowQuadrisectionLineTextToOutside(const bool bShow)		{ return false; };	// ���м� Text ���̱�
	virtual bool	SetShowControlLineTextToOutside(const bool bShow)			{ return false; };	// ������ Text ���̱�
	virtual bool	SetShowCrossLineToOutside(const bool bShow)					{ return false; };	// ��ġ��ȸ�� ���ڼ�ǥ�ÿ���

	virtual bool	GetImaginaryCandleToOutside(bool& bImaginaryCandle)			{ return false; };	// ���������.
	virtual bool	GetUseGapRevisionToOutside(bool& bUseGapRevision)			{ return false; };	// ������
	virtual bool	GetInquiryDataWindowTypeToOutside(int& nWindowType)			{ return false;	};	// ��ġ��ȸâ ǥ�ÿ���
	virtual bool	GetInquiryDataWindowMoveTypeToOutside(int& nMoveType)		{ return false; };	// ��ġ��ȸâ �̵����¼���
	virtual bool	GetYScalePositionToOutside(int& nPosition)					{ return false; };	// Y�� ���� ǥ��
	virtual bool	GetRightMarginToOutside(int& nRightMargin)					{ return false; };	// ������ ����
	virtual bool	GetShowIndicatorVariableToOutside(bool& bShow)				{ return false; };	// ��ǥ���� - ��ǥ���� ǥ��
	virtual bool	GetShowIndicatorValueToOutside(bool& bShow)					{ return false; };	// ��ǥ���� - ��ǥ��
	virtual bool	GetShowHighLowBoundToOutside(bool& bShow)					{ return false; };	// ��/���Ѱ� ǥ��
	virtual bool	GetShowExRightDividendToOutside(bool& bShow)				{ return false; };	// �Ǹ���/���� ǥ��
	virtual bool	GetShowCurrentPriceToOutside(int& nShowType)				{ return 0;		};	// ���� �������� ���簪 ǥ��
	virtual bool	GetTrendLineColorToOutside(COLORREF& clrLineColor)			{ return false; };	// �߼��� ����
	virtual bool	GetShowTrendLineValueToOutside(int& nPosition)				{ return false; };	// �߼����� ǥ��
	virtual bool	GetTimeLinkageToOutside(bool& bTimeLinkage)					{ return false; };	// �ð����� ����
	virtual bool	GetAutoSaveToOutside(bool& bAutoSave)						{ return false; };	// ��Ʈ����� �ڵ�����
	virtual bool	GetShowTrendLineTextToOutside(bool& bShow)					{ return false; };	// �߼���Text ���̱�
	virtual bool	GetShowTimeLineTextToOutside(bool& bShow)					{ return false; };	// �ð���Text ���̱�
	virtual bool	GetShowPriceLineLeftTextToOutside(bool& bShow)				{ return false; };	// ���ݼ� ���� Text ���̱�
	virtual bool	GetShowPriceLineRightTextToOutside(bool& bShow)				{ return false; };	// ���ݼ� ������ Text ���̱�
	virtual bool	GetShowTrisectionLineTextToOutside(bool& bShow)				{ return false; };	// ���м� Text ���̱�
	virtual bool	GetShowQuadrisectionLineTextToOutside(bool& bShow)			{ return false; };	// ���м� Text ���̱�
	virtual bool	GetShowControlLineTextToOutside(bool& bShow)				{ return false; };	// ������ Text ���̱�
	virtual bool	GetShowCrossLineToOutside(bool& bShow)						{ return false; };	// ��ġ��ȸ�� ���ڼ� ǥ�ÿ���.

	virtual bool	OnChartEnvSetupOk() { return false; };

	virtual bool	SetPriceLog(const bool bLog) { return false; };	// ������Ʈ log ����.
};

// (2006/11/15 - Seung-Won, Bae) Add New Interface Type.
class CMChartInterface061115 : public CMChartInterface
{
// (2006/11/18 - Seung-Won, Bae) This Interface used with Only COutsideCommonInterfaceFunction class.
friend class COutsideCommonInterfaceFunction;

protected:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return MC_TYPE_061115; };

// (2006/11/15 - Seung-Won, Bae) Revised Price
protected:
	virtual BOOL	GetRevisedPriceFlag( void)									{	return FALSE;	}
	virtual BOOL	SetRevisedPriceFlag( BOOL p_bRevisedFlag)					{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) Notify the Event of Indicator Added or Removed.
protected:
	virtual BOOL	OnIndicatorAddOrRemoved( const char *p_szIndicatorName,	const BOOL p_bAdded)				{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) User Zoom Changed.
protected:
	virtual BOOL	OnUserZoomChanged( const int p_nDataCountInView)											{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
protected:
	virtual BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)		{	return FALSE;	}
};

// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
class COldMChartInterface : public COutsideCommonInterface
{
// (2006/11/18 - Seung-Won, Bae) This Interface used with Only COutsideCommonInterfaceFunction class.
friend class COutsideCommonInterfaceFunction;

protected:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return MC_TYPE_ORG; };

protected:
	// ����Ÿâ.
	virtual bool	IsActiveDataWindow()										{ return false; };
	virtual void	SetDataWindowData(CList<CString, CString&>* pListData)		{ };

	// ȯ�漳��.
	virtual bool	SetImaginaryCandleToOutside(const bool bImaginaryCandle)	{ return false; };	// ���������.
	virtual bool	SetInquiryDataWindowTypeToOutside(const int nWindowType)	{ return false; };	// ��ġ��ȸâ ǥ�ÿ��� 0:����, 1:Window
	virtual bool	SetInquiryDataWindowMoveTypeToOutside(const int nMoveType)	{ return false; };	// ��ġ��ȸâ �̵����¼��� 0:�̵�, 1:����
	virtual bool	SetYScalePositionToOutside(const int nPosition)				{ return false; };	// Y�� ����ǥ�� (0:����, 1:������, 2:����, 3:����)
	virtual bool	SetRightMarginToOutside(const int nRightMargin)				{ return false; };	// ������ ����
	virtual bool	SetShowIndicatorVariableToOutside(const bool bShow)			{ return false; };	// ��ǥ���� - ��ǥ���� ǥ��
	virtual bool	SetShowIndicatorValueToOutside(const bool bShow)			{ return false; };	// ��ǥ���� - ��ǥ�� ǥ��
	virtual bool	SetShowHighLowBoundToOutside(const bool bShow)				{ return false; };	// ��/���Ѱ� ǥ��
	virtual bool	SetShowExRightDividendToOutside(const bool bShow)			{ return false; };	// �Ǹ���/���� ǥ��
	virtual bool	SetShowCurrentPriceToOutside(const int nShowType)			{ return false; };	// ���ݴ������� ���簡ǥ��
	virtual bool	SetTrendLineColorToOutside(const COLORREF clrLineColor)		{ return false; };	// �߼��� ����
	virtual bool	SetShowTrendLineValueToOutside(const int nPosition)			{ return false; };	// �߼����� ǥ�� (0: , 1: , 2: , 3: )
	virtual bool	SetTimeLinkageToOutside(const bool bTimeLinkage)			{ return false; };	// �ð����� ����
	virtual bool	SetAutoSaveToOutside(const bool bAutoSave)					{ return false; };	// ��Ʈ����� �ڵ� ���� ����
	virtual bool	SetShowTrendLineTextToOutside(const bool bShow)				{ return false; };	// �߼���Text ���̱�
	virtual bool	SetShowTimeLineTextToOutside(const bool bShow)				{ return false; };	// �ð���Text ���̱�
	virtual bool	SetShowPriceLineLeftTextToOutside(const bool bShow)			{ return false; };	// ���ݼ� ���� Text ���̱�
	virtual bool	SetShowPriceLineRightTextToOutside(const bool bShow)		{ return false; };	// ���ݼ� ������ Text ���̱�
	virtual bool	SetShowTrisectionLineTextToOutside(const bool bShow)		{ return false; };	// ���м� Text ���̱�
	virtual bool	SetShowQuadrisectionLineTextToOutside(const bool bShow)		{ return false; };	// ���м� Text ���̱�
	virtual bool	SetShowControlLineTextToOutside(const bool bShow)			{ return false; };	// ������ Text ���̱�
	virtual bool	SetShowCrossLineToOutside(const bool bShow)					{ return false; };	// ��ġ��ȸ�� ���ڼ�ǥ�ÿ���

	virtual bool	GetImaginaryCandleToOutside(bool& bImaginaryCandle)			{ return false; };	// ���������.
	virtual bool	GetInquiryDataWindowTypeToOutside(int& nWindowType)			{ return false;	};	// ��ġ��ȸâ ǥ�ÿ���
	virtual bool	GetInquiryDataWindowMoveTypeToOutside(int& nMoveType)		{ return false; };	// ��ġ��ȸâ �̵����¼���
	virtual bool	GetYScalePositionToOutside(int& nPosition)					{ return false; };	// Y�� ���� ǥ��
	virtual bool	GetRightMarginToOutside(int& nRightMargin)					{ return false; };	// ������ ����
	virtual bool	GetShowIndicatorVariableToOutside(bool& bShow)				{ return false; };	// ��ǥ���� - ��ǥ���� ǥ��
	virtual bool	GetShowIndicatorValueToOutside(bool& bShow)					{ return false; };	// ��ǥ���� - ��ǥ��
	virtual bool	GetShowHighLowBoundToOutside(bool& bShow)					{ return false; };	// ��/���Ѱ� ǥ��
	virtual bool	GetShowExRightDividendToOutside(bool& bShow)				{ return false; };	// �Ǹ���/���� ǥ��
	virtual bool	GetShowCurrentPriceToOutside(int& nShowType)				{ return 0;		};	// ���� �������� ���簪 ǥ��
	virtual bool	GetTrendLineColorToOutside(COLORREF& clrLineColor)			{ return false; };	// �߼��� ����
	virtual bool	GetShowTrendLineValueToOutside(int& nPosition)				{ return false; };	// �߼����� ǥ��
	virtual bool	GetTimeLinkageToOutside(bool& bTimeLinkage)					{ return false; };	// �ð����� ����
	virtual bool	GetAutoSaveToOutside(bool& bAutoSave)						{ return false; };	// ��Ʈ����� �ڵ�����
	virtual bool	GetShowTrendLineTextToOutside(bool& bShow)					{ return false; };	// �߼���Text ���̱�
	virtual bool	GetShowTimeLineTextToOutside(bool& bShow)					{ return false; };	// �ð���Text ���̱�
	virtual bool	GetShowPriceLineLeftTextToOutside(bool& bShow)				{ return false; };	// ���ݼ� ���� Text ���̱�
	virtual bool	GetShowPriceLineRightTextToOutside(bool& bShow)				{ return false; };	// ���ݼ� ������ Text ���̱�
	virtual bool	GetShowTrisectionLineTextToOutside(bool& bShow)				{ return false; };	// ���м� Text ���̱�
	virtual bool	GetShowQuadrisectionLineTextToOutside(bool& bShow)			{ return false; };	// ���м� Text ���̱�
	virtual bool	GetShowControlLineTextToOutside(bool& bShow)				{ return false; };	// ������ Text ���̱�
	virtual bool	GetShowCrossLineToOutside(bool& bShow)						{ return false; };	// ��ġ��ȸ�� ���ڼ� ǥ�ÿ���.

	virtual bool	OnChartEnvSetupOk() { return false; };

	virtual bool	SetPriceLog(const bool bLog) { return false; };	// ������Ʈ log ����.
};
class COldMChartInterface061115 : public COldMChartInterface
{
// (2006/11/18 - Seung-Won, Bae) This Interface used with Only COutsideCommonInterfaceFunction class.
friend class COutsideCommonInterfaceFunction;

protected:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return MC_TYPE_061115; };

// (2006/11/15 - Seung-Won, Bae) Revised Price
protected:
	virtual BOOL	GetRevisedPriceFlag( void)									{	return FALSE;	}
	virtual BOOL	SetRevisedPriceFlag( BOOL p_bRevisedFlag)					{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) Notify the Event of Indicator Added or Removed.
protected:
	virtual BOOL	OnIndicatorAddOrRemoved( const char *p_szGraphName,	const BOOL p_bAdded)				{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) User Zoom Changed.
protected:
	virtual BOOL	OnUserZoomChanged( const int p_nDataCountInView)											{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
protected:
	virtual BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)		{	return FALSE;	}
};


#endif