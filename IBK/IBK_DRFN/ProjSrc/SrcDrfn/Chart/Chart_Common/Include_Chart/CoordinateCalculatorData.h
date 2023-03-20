// CoordinateCalculatorData.h: interface for the CCoordinateCalculatorData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORDINATECALCULATORDATA_H__40E43F4C_1FCC_4DF5_9713_EB7152A22CB1__INCLUDED_)
#define AFX_COORDINATECALCULATORDATA_H__40E43F4C_1FCC_4DF5_9713_EB7152A22CB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPacket;

class CVertScaleType
{
public:
	CVertScaleType& operator=(const CVertScaleType& vertScaleType);

public:
	CVertScaleType();
	CVertScaleType(const CVertScaleType& vertScaleType);
	CVertScaleType(const bool bIsLog, const bool bIsInvert);

	void SetScaleType(const bool bIsLog, const bool bIsInvert);
	void SetScaleLogType(const bool bIsLog);
	void SetScaleInverType(const bool bIsInvert);

	bool IsLog() const;
	bool IsInvert() const;

	double ApplyLog(const double& dData) const;
	double ApplyExp(const double& dData) const;

private:
	bool m_bIsLog;		// logŸ��
	bool m_bIsInvert;	// ������ Ÿ��
};


class CDisplayDataVertMinMax
{
public:
	CDisplayDataVertMinMax& operator=(const CDisplayDataVertMinMax& data);

public:
	CDisplayDataVertMinMax();
	CDisplayDataVertMinMax(const double& dMin, const double& dMax, const CVertScaleType& vertScaleType);

	void SetMinMax(const double& dMin, const double& dMax);
	void NormalizeMinMax();

	double GetMin() const;
	double GetMax() const;
	CVertScaleType GetVertScaleType() const;
	CVertScaleType& GetVertScaleType();
	double GetMin_ScaleType() const;
	double GetMax_ScaleType() const;

private:
	double m_dMin;		// block���� ������ data�� �ּҰ�
	double m_dMax;		// block���� ������ data�� �ִ밪
	CVertScaleType m_vertScaleType;	// ���� scale type (log, invert)
};


// ��ǥ�� ���ϱ� ���� �ʿ��� data.
class CCoordinateCalculatorData
{
public:
	CCoordinateCalculatorData& operator=(const CCoordinateCalculatorData& data);

public:
	CCoordinateCalculatorData();

	void SetRegion(const CRect& region);
	void SetDisplayDataStartEnd(const int start, const int end);
	void SetVertDataType(const double& dDataType);
	void SetPacket_X(CPacket* pDataPacket);
	void SetPacket_Y(CPacket* pClosePacket);

	CRect GetRegion() const;
	int GetDisplayDataStart() const;
	int GetDisplayDataEnd() const;
	int GetDisplayDataCount() const;
	double GetDisplayDataMin_ScaleType() const;
	double GetDisplayDataMax_ScaleType() const;
	CDisplayDataVertMinMax GetDisplayDataVertMinMax() const;
	CDisplayDataVertMinMax& GetDisplayDataVertMinMax();
	CVertScaleType GetVertScaleType() const;
	double GetVertDataType() const;
	CPacket* GetPacket_X() const;
	CPacket* GetPacket_Y() const;

	bool DoesCalculator(const CPoint& point) const;
	
	double ApplyLog(const double& dData) const;
	double ApplyExp(const double& dData) const;

private:
	CRect m_region;				// �׸��� ����
	int m_nDisplayDataStart;	// block���� ������ ó�� data
	int m_nDisplayDataEnd;		// block���� ������ ������ data
	CDisplayDataVertMinMax m_displayDataVertMinMax;
	double m_dVertDataType;		// ���� scale�� dataType
	CPacket* m_pPacket_x;		// ���� scale <ex:�ڷ�����> packet
	CPacket* m_pPacket_y;		// ���� scale <ex:����> packet
};

#endif // !defined(AFX_COORDINATECALCULATORDATA_H__40E43F4C_1FCC_4DF5_9713_EB7152A22CB1__INCLUDED_)
