#ifndef OUTSIDECOMMONINTERFACE_H_
#define OUTSIDECOMMONINTERFACE_H_
//////////////////////////////////////////////////////////////////////////
// OutsideCommonInterface.h
// Chart OCX�� MultiChart DLL�� �̰��� ��ӹ޾� �ʿ��� �Լ��� �����Ѵ�.
// OCX�� DLL�� �� �������̸� ���� ���� �ǻ������ �Ѵ�.
//////////////////////////////////////////////////////////////////////////
#include <AFXTEMPL.H>

class COutsideCommonInterface
{
public:
	// OutsideCommonInterface �� type
	enum TYPE_OCIF
	{
		BASE_TYPE = 0,
		MC_TYPE
	};

public:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return BASE_TYPE; };

public:
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
public:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return MC_TYPE; };

public:
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

#endif