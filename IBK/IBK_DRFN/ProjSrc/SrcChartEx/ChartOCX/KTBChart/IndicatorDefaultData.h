// IndicatorDefaultData.h: interface for the CIndicatorDefaultData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORDEFAULTDATA_H__82FB44FF_900E_4743_8883_FA3C716CB4D7__INCLUDED_)
#define AFX_INDICATORDEFAULTDATA_H__82FB44FF_900E_4743_8883_FA3C716CB4D7__INCLUDED_

#include "BlockBaseData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKTBChartCtrl;
class CIndicatorList;
class CIndicatorInfo;
class CGraphProperty;
class CIndicatorDefaultData  
{
public:
	CIndicatorDefaultData( const CKTBChartCtrl *p_pChartOCX, const CScaleBaseData::SCALEGRIDTYPE eBaseGridType = CScaleBaseData::SLNONE, 
		const CScaleBaseData::VERTSCALEPOSITION eVertScalePosition = CScaleBaseData::VERT_RIGHT);

	// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
	//	void	SetBaseScaleGridType(const CScaleBaseData::SCALEGRIDTYPE eGridType);
	void	SetVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition);

	BOOL	GetBlockDataForMaking( CString &p_strBlockData, CIndicatorList* pIndicatorList, const CString& strIndicatorNames, 
				const CScaleBaseData::SCALEGRIDTYPE eHorzGridType, const CScaleBaseData::SCALEGRIDTYPE eVertGridType, CString &p_strGraphName);
	BOOL	GetVertScaleGroupDataForMaking( CString &p_strVertScaleGroupData, CIndicatorList* pIndicatorList, const CString &strIndicatorName, CString &p_strGraphName) const;

private:
	enum BLOCKMAKINGTYPE
	{
		NONE = 0,
		ONE_BLOCK,			// block 1���� ���
		START_TWO_BLOCK,	// block 2�� �̻� �� ó���� ���
		MIDDEL_THREE_BLOCK,	// block 3�� �̻� �� ����� ���
		END_TWO_BLOCK		// block 2�� �̻� �� ���� ���
	};

private:
	void	GetVertScaleGroupDataForMaking( CString &p_strVertScaleGroupData, CIndicatorInfo* pIndicatorInfo) const;

	CString GetHorizontalScaleData(const CString& strGraphName, const BOOL bIsEndBlock) const;
	void	GetVerticalScaleData( CString &p_strVerticalScaleData, const CString& strGraphName) const;

	CString GetHorizontalPacketName(const CString& strGraphName) const;
	CScaleBaseData::HORZSCALEPOSITION GetHorizontalPosition(const CString& strGraphName, const BOOL bIsEndBlock) const;

	CIndicatorDefaultData::BLOCKMAKINGTYPE GetBlockMakingType(const int nCount, const int nIndex) const;

private:
	CScaleBaseData::SCALEGRIDTYPE m_eBaseGridType;				// It is for X & Y Scale Grid Type.
	CScaleBaseData::VERTSCALEPOSITION m_eVertScalePosition;

// (2004.10.06, ��¿�) ������Ʈ�� Graph Drawing Data�� �����޾� DefaultType�� ���� ����, ������� �Ѵ�.
protected:
	const CKTBChartCtrl	*m_pChartOCX;
};

#endif // !defined(AFX_INDICATORDEFAULTDATA_H__82FB44FF_900E_4743_8883_FA3C716CB4D7__INCLUDED_)
