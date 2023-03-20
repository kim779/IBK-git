// CommonInterface.h: interface for the CCommonInterface class.
// ����(05/01/20) Chart OCX�� �θ�Ŭ����. ����DLL���� �ʿ���ϴ� OCX�Լ����� ����� ��Ƽ�
//			�� Ŭ���� �����ͷ� �������ϵ��� �Ѵ�. (�� DLL�����������̽��� ���̢�~)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONINTERFACE_H__C214783A_7D85_4235_A569_F76E35F3F96C__INCLUDED_)
#define AFX_COMMONINTERFACE_H__C214783A_7D85_4235_A569_F76E35F3F96C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseEnum.h"

class CIndicatorList;
class CIndicatorInfo;
class CPacket;
class CMainBlock;
class CPacketList;
class CAnalysisToolData;
class CStyleData;

class CCommonInterface  
{
public:
	enum ETCMESSAGETYPE
	{
		PRICE_LOG = 0
	};

public:
	//////////////////////////////////////////////////////////////////////////
	// �⺻ ����
	virtual CString GetDefaultPath() const = 0;
	virtual CString GetUserPath() const = 0;

	//////////////////////////////////////////////////////////////////////////
	// ȯ�漳������ ����� �Լ���...
	virtual void SetImaginaryCandle(const bool bImaginaryCandle, const bool bOutside = true) = 0;	// ���������.
	virtual void SetNumericalInquiryOption(const long nInquiryOption, const bool bOutside = true) = 0;	// ��ġ��ȸâ ǥ�ÿ��� 0:����, 1:Window
	virtual void SetYScalePosition(const int nPosition, const bool bOutside = true) = 0;			// Y�� ����ǥ�� (0:����, 1:������, 2:����, 3:����)
	virtual void SetRightMargin(const int nRightMargin, const bool bOutside = true) = 0;			// ������ ����
	virtual void SetShowIndicatorVariable(const bool bShow, const bool bOutside = true) = 0;		// ��ǥ���� - ��ǥ���� ǥ��
	virtual void SetShowIndicatorValue(const bool bShow, const bool bOutside = true) = 0;			// ��ǥ���� - ��ǥ�� ǥ��
	virtual void SetShowHighLowBound(const bool bShow, const bool bOutside = true) = 0;				// ��/���Ѱ� ǥ��
	virtual void SetShowExRightDividend(const bool bShow, const bool bOutside = true) = 0;			// �Ǹ���/���� ǥ��
	virtual void SetShowCurrentPrice(const int nShowType, const bool bOutside = true) = 0;			// ���ݴ������� ���簡ǥ��
	virtual void SetTrendLineColor(const COLORREF clrLineColor, const bool bOutside = true) = 0;	// �߼��� ����
	virtual void SetShowTrendLineValue(const int nPosition, const bool bOutside = true) = 0;		// �߼����� ǥ�� (0: , 1: , 2: , 3: )
	virtual void SetTimeLinkage(const bool bTimeLinkage, const bool bOutside = true) = 0;			// �ð����� ����
	virtual void SetAutoSave(const bool bAutoSave, const bool bOutside = true) = 0;					// ��Ʈ����� �ڵ� ���� ����
	virtual void SetShowMinMaxPrice(const bool bShow, const bool bOutside = true) = 0;				// �ְ�/������ ǥ��

	virtual bool GetImaginaryCandle() = 0;						// ���������.
	virtual long GetNumericalInquiryOption( void) const = 0;	// ��ġ��ȸâ ǥ�ÿ��� 0:����, 1:Window
	virtual int GetYScalePosition() = 0;						// Y�� ����ǥ�� (0:����, 1:������, 2:����, 3:����)
	virtual int GetRightMargin() = 0;							// ������ ����
	virtual bool GetShowIndicatorVariable() = 0;				// ��ǥ���� - ��ǥ���� ǥ��
	virtual bool GetShowIndicatorValue() = 0;					// ��ǥ���� - ��ǥ�� ǥ��
	virtual bool GetExistenceHighLowBound() const = 0;				// "��/���Ѱ� ǥ��"�� �����Ǿ� �ִ����� ����(�������).
	virtual bool GetShowHighLowBound() = 0;						// ��/���Ѱ� ǥ��
	virtual bool GetShowExRightDividend() = 0;					// �Ǹ���/���� ǥ��
	virtual int GetShowCurrentPrice()  = 0;						// ���ݴ������� ���簡ǥ��
	virtual COLORREF GetTrendLineColor() = 0;					// �߼��� ����
	virtual int	GetShowTrendLineValue() = 0;					// �߼����� ǥ�� (0: , 1: , 2: , 3: )
	virtual bool GetTimeLinkage() = 0;							// �ð����� ����
	virtual bool GetAutoSave() = 0;								// ��Ʈ����� �ڵ� ���� ����
	virtual bool GetShowMinMaxPrice() = 0;						// �ְ�/������ ǥ��

	virtual void ChartEnvSetupOk() = 0;							// ��Ʈ����â OK��ư�� ��������.
	
	virtual CIndicatorList* GetIndicatorList() = 0;
	virtual CMainBlock*	GetMainBlock() = 0;
	virtual CPacketList* GetPacketList() = 0;
	virtual void GetAnalysisToolData(CAnalysisToolData& analysisToolData) = 0;
	virtual void GetStyleData(CStyleData& styleData) const = 0;

	virtual bool SetAnalysisToolData(const CAnalysisToolData& analysisToolData) = 0;
	virtual bool SetStyleData(const CStyleData& styleData) = 0;
	virtual bool SaveChartCfgInCurrentOCX() = 0;
	//////////////////////////////////////////////////////////////////////////
	// AddIn
	virtual bool CalculateIndicatorInTRData(CIndicatorInfo* pIndicatorInfo, const CList<CPacket*, CPacket*>& packetList) = 0;
	virtual bool DeleteIndicator(const char* pcszIndicatorName) = 0;
	virtual bool CalculateAll(const CCalculateBaseData::CALCULATETYPE enumCalulateType) = 0;

	//////////////////////////////////////////////////////////////////////////
	// ChartObject.dll
	virtual void BehaviorBlock(const long pBlockBehaviorData) = 0;
	virtual void SetGraphSelectedDataIndex(const int nDataIndex, LPCTSTR pcszIndicatorName) = 0;
	virtual void SetGraphThickness(const int nGraphThickness1) = 0;
	virtual void SetGraphTypeStyle(const int nGraphType, const int nDrawStyle) = 0;
	virtual void SetEtc(const int nOption, const int nUserMargin) = 0;
	virtual void SetGraphMargin(LPCTSTR pcszFunctionName, const bool bIsAddMargin) = 0;
	virtual void SetCursorExchange(const CCursorShape::CHARTOBJECT enumChartObject) = 0;
	virtual int	GetUserMargin(void) const = 0;
	virtual HWND GetWndHandle(void) const = 0;
	virtual void InvalidateRect(const CRect& region, const bool  bErase) = 0;
	

	//////////////////////////////////////////////////////////////////////////
	// IndincatorConfiguration.dll
	virtual void InvalidateOCX(const bool bErase = true)	= 0;	// OCX Invalidate
	virtual int SaveCurrentChartCfg() = 0;						// ���� ��Ʈ ȭ�鼳���� �����ϰ� �ش� �ε����� �����Ѵ�.
	virtual bool RestoreChartCfg(const int nIndex) = 0;			// �ε����� �ش��ϴ� ��Ʈȭ������ �����Ѵ�.
	virtual bool AddRemoveGraph(LPCTSTR pcszIndicatorName) = 0;	// ��ǥ�׷����� �߰�/�����Ѵ�.
	virtual bool IsLoad_MarketChart() const = 0;				// ������ǥ Load ������ ���� ����

	//////////////////////////////////////////////////////////////////////////
	// ToolBar.dll
	virtual bool IsWholeView() const = 0;	// ��ü�������� ����.
	virtual void InvalidateOleControl() = 0;	// COleControl�� InvalidateControl()�� ȣ���Ѵ�.
	virtual void SetStartEndIndex(const int nStartIndex, const int nEndIndex) = 0;	// ��ũ�ѹٿ� ���� ���� ����� Start/End Index�� �����Ѵ�.
	virtual void SetScrollInfo(const int nOnePageDataCount) = 0;				// ���������� ����Ÿ������ �����Ѵ�.
	virtual bool RunToolFunction(const CToolOptionInfo::TOOLOPTION enumToolOption, const int nParam) = 0; // ����ɽ���/����
	virtual bool RunToolFuncGraphStyle(const int nParam) = 0;				// �׷��� ���¸� �����Ѵ�.
	virtual bool RunToolFuncLineThickness(const int nParam) = 0;				// �� ���⸦ �����Ѵ�.
	virtual bool RunToolFuncGraphColor(const COLORREF clrGraphColor) = 0;	// �׷��� ���� �����Ѵ�.
	virtual void SetStateWholeView(const int nParam = 0) = 0;				// ��ü���⸦ �����Ѵ�.
	virtual CWnd* GetChartWindowPointer() const = 0;	// OCX�� Window Pointer�� �Ѱ��ش�.
	virtual int	GetMinCountPerPage() const = 0;		// ���������� �ּ� ����Ÿ������ �ѱ��.
	virtual int	GetDataCountForToolBar() const = 0;
	virtual CRect GetToolBarRegion() const = 0;
	virtual CFont* GetRscFont() const = 0;				// ���ҽ���Ʈ�� �����´�.
	virtual CRect GetOcxRegion() const = 0;			// OCX������ �����´�.
	virtual double ConvertYPositionToData(const int nYPosition, const int nYPositionTop, const int nYPositionBottom,
						const double& dDataMax, const double& dDataMin, const bool bLog, const bool bReverse) = 0;	// ����븦 ���ؿ������� �Լ�.
	// TR ��û
	virtual void RequestTRPacket(const CString& strIndicatorName, const CString& strPacketNames) = 0;	// �ش��� ��Ŷ�� TR�� ��û�Ѵ�.
	virtual void RequestTRPacket(const CString& strIndicatorName, const CString& strPacketNames, const int nCount, const bool bIsNew) = 0;	// �ش��� ��Ŷ�� TR�� ��û�Ѵ�.

	//////////////////////////////////////////////////////////////////////////
	// Etc
	virtual void OnScaleSettingMessage(const ETCMESSAGETYPE eEtcMessageType, const int nData) = 0;

};

#endif // !defined(AFX_COMMONINTERFACE_H__C214783A_7D85_4235_A569_F76E35F3F96C__INCLUDED_)
