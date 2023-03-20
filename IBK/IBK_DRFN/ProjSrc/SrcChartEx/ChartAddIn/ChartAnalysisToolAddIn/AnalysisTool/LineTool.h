// LineTool.h: interface for the CLineTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINETOOL_H__D27C27DE_2A4B_4747_9E07_9D4059B5A9FA__INCLUDED_)
#define AFX_LINETOOL_H__D27C27DE_2A4B_4747_9E07_9D4059B5A9FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PointTool.h"
#include "LineDrawer.h"

class CAnalysisData;
class CLineTool : public CTwoPointDownTool
{
public:
	CLineTool(CAnalysisData* pAnalysisData, const CAnalysis::TOOLTYPE p_eToolType, 
			const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextHorzPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight, const bool bRightRate=TRUE);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);
protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextHorzPosition m_textPosition;
	CLineDrawer m_lineDrawer;
protected:
	CAnalysis::TOOLTYPE				m_eToolType;
protected:
	virtual CAnalysis::TOOLTYPE		GetToolType() const;
};

// ----------------------------------------------------------------------------
// ������
class CVertLineTool : public COnePointMoveTool
{
public:
	CVertLineTool(CAnalysisData* pAnalysisData = NULL,const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextVertPosition& position);
	void SetTextPosition(const bool bTop, const bool bBottom);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();

private:
	CTextVertPosition m_textPosition;
	CVertLineDrawer m_vertLineDrawer;
};

// ----------------------------------------------------------------------------
// ����
class CHorzLineTool : public COnePointMoveTool
{
public:
	CHorzLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextHorzPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();

	// �׸��� �����鿡 ���� point ������ ����.
	//virtual bool PointInDrawingRegion(const CPoint& point) const;

private:
	CTextHorzPosition m_textPosition;
	CHorzLineDrawer m_horzLineDrawer;
};

// ----------------------------------------------------------------------------
// ���ڼ�
class CCrossLineTool
{
public:
	CCrossLineTool();

	void SetTextPosition(const CTextHorzVertPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight, const bool bTop, const bool bBottom);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	const CTextPosition* GetTextPosition() const;
	CTextPosition* GetTextPosition();
	COnePointDrawer& GetOnePointDrawer();

private:
	CTextHorzVertPosition m_textPosition;
	CCrossLineDrawer m_crossLineDrawer;
};

class CCrossLineMoveTool : public COnePointMoveTool, public CCrossLineTool
{
public:
	CCrossLineMoveTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();
};

class CCrossLineDownTool : public COnePointDownTool, public CCrossLineTool
{
public:
	CCrossLineDownTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

public:
	virtual void OnLButtonUp(CDC* pDC, const CPoint& point);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();
};

// ----------------------------------------------------------------------------
// ���м�
class CTrisectionLineTool : public CTwoPointDownTool
{
public:
	CTrisectionLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextHorzPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextHorzPosition m_textPosition;
	CTrisectionLineDrawer m_trisectionLineDrawer;
};

// ----------------------------------------------------------------------------
// ���м�
class CQuadrisectLineTool : public CTwoPointDownTool
{
public:
	CQuadrisectLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);
	
	void SetTextPosition(const CTextHorzPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextHorzPosition m_textPosition;
	CQuadrisectLineDrawer m_quadrisectLineDrawer;
};

// ----------------------------------------------------------------------------
// ����� ��ġ��ũ
class CAndrewsPitchforkTool : public CThreePointDownTool
{
public:
	CAndrewsPitchforkTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CThreePointDrawer& GetThreePointDrawer();

private:
	CAndrewsPitchforkDrawer m_andrewsDrawer;
};

// ----------------------------------------------------------------------------
// ����Ŭ ����
class CCycleLinesTool : public CTwoPointDownTool //CTwoPointMoveTool
{
public:
	CCycleLinesTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

	//virtual CPoints GetTwoPoints() const;
private:
	CCycleLinesDrawer m_cycleLinesDrawer;
};

// ----------------------------------------------------------------------------
// ���ǵ� ����
class CSpeedLineTool : public CTwoPointDownTool
{
public:
	CSpeedLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CSpeedLineDrawer m_speedLineDrawer;
};

// ----------------------------------------------------------------------------
// ���ǵ� Fan
class CSpeedFanTool : public CTwoPointDownTool
{
public:
	CSpeedFanTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CSpeedFanDrawer m_speedFanDrawer;
};

// ----------------------------------------------------------------------------
// ���ǵ� Arc
class CSpeedArcTool : public CTwoPointDownTool
{
public:
	CSpeedArcTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CSpeedArcDrawer m_speedArcDrawer;
};

// ----------------------------------------------------------------------------
// ����ȸ�ͼ� 
class CLinearRegressionTool : public CTwoPointDownTool
{
public:
	CLinearRegressionTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();
private:
	CLinearRegressionDrawer m_linearRegressionDrawer;
};

// ----------------------------------------------------------------------------
// ����ȸ��ä�� 
class CRaffRegressionTool : public CTwoPointDownTool
{
public:
	CRaffRegressionTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();
private:
	CRaffRegressionDrawer m_raffRegressionDrawer;
};

// ----------------------------------------------------------------------------
// �����߼��� 
class CAngleLineTool : public CTwoPointDownTool
{
public:
	CAngleLineTool(CAnalysisData* pAnalysisData,const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextShowPosition& position);
	void SetTextPosition(const bool bShow);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);
	
protected:
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CAnalysis::TOOLTYPE	GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();
private:
	CTextShowPosition m_textPosition;
	CAngleLineDrawer m_angleLineDrawer;
};

// ----------------------------------------------------------------------------
// Candle�߽ɼ� 
class CCandleLineTool : public COnePointMoveTool
{
public:
	CCandleLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

	void SetTextPosition(const CTextShowPosition& position);
	void SetTextPosition(const bool bShow);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual COnePointDrawer& GetOnePointDrawer();
private:
	CTextShowPosition m_textPosition;
	CCandleLineDrawer m_candleLineDrawer;
};

// ----------------------------------------------------------------------------
// �����
class CCandleBongTool : public CTwoPointDownTool
{
public:
	CCandleBongTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CCandleBongDrawer m_candleBongDrawer;
};

// ----------------------------------------------------------------------------
// �����߼���
class CPencilLineTool : public CMultiPointDownTool
{
public:
	CPencilLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CMultiPointDrawer& GetMultiPointDrawer();

private:
	CPencilLineDrawer m_pencilLineDrawer;
};

// ----------------------------------------------------------------------------
// �Ⱓ���
class CPeriodSumLineTool : public CTwoPointDownTool
{
public:
	CPeriodSumLineTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);
	
	void SetTextPosition(const CTextHorzPosition& position);
	void SetTextPosition(const bool bLeft, const bool bRight);
	void SetTextEnvironment(const CTextEnvironment& textEnvironment);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual const CTextPosition* GetTextPosition() const;
	virtual CTextPosition* GetTextPosition();
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CTextHorzPosition m_textPosition;
	CPeriodSumLineDrawer m_periodSumLineDrawer;
};

// ----------------------------------------------------------------------------
// ElliotWaveLine
class CElliotLineTool : public CTwoPointDownTool
{
public:
	CElliotLineTool(CAnalysisData* pAnalysisData, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);
	
protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CTwoPointDrawer& GetTwoPointDrawer();

private:
	CElliotLineDrawer m_elliotLineDrawer;
};

// ----------------------------------------------------------------------------
// ���� ����� 
class CAdjustRatioTool : public CThreePointDownTool
{
public:
	CAdjustRatioTool(CAnalysisData* pAnalysisData = NULL, const CAnalysis::COORDINATETYPE eCoordinateType = CAnalysis::RELATIVE_POINT);

protected:
	virtual CAnalysis::TOOLTYPE GetToolType() const;
	virtual CThreePointDrawer& GetThreePointDrawer();

private:
	CAdjustRatioDrawer m_adjustRatioDrawer;
};
#endif // !defined(AFX_LINETOOL_H__D27C27DE_2A4B_4747_9E07_9D4059B5A9FA__INCLUDED_)
