// OrgViewData.h: interface for the COrgViewData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORGVIEWDATA_H__DCBD417D_6148_4171_B09C_2D25FB7736A2__INCLUDED_)
#define AFX_ORGVIEWDATA_H__DCBD417D_6148_4171_B09C_2D25FB7736A2__INCLUDED_

#include "Include_AddIn.h"		// for IGraphInfo

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndiInfoData
{
public:
	static CIndiInfoData* Make(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning);
	static void Delete(CIndiInfoData* pIndiInfoData);

public:
	CIndiInfoData(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning = false);
	virtual ~CIndiInfoData();

	void SetTheBeginning(const bool bTheBeginning);
	void SetIndicatorInfo(IGraphInfo* p_pIGraphInfo);

	bool IsTheBeginning() const;
	IGraphInfo& GetIndicatorInfo();
	IGraphInfo* GetIGraphInfo();

	CString GetGraphName() const;
	CString GetIndicatorName() const;

private:
	bool m_bTheBeginning; // ó�� ȭ����� ���� ����
	IGraphInfo *m_pIGraphInfo;
};


class CIndiInfoDataList
{
public:
	CIndiInfoDataList();
	virtual ~CIndiInfoDataList();

	bool AddTail(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning);
	void DeleteAll();

	//����� �ִ� IGraphInfo ���� pTargetIndiList�� �����ϱ�.
	bool ChangeAllIndicatorList( IGraphInfoManager* p_pIGraphInfoManager);

	POSITION GetHeadPosition() const;
	POSITION GetHeadPosition();
	POSITION GetTailPosition() const;
	POSITION GetTailPosition();
	CIndiInfoData* GetPrev(POSITION& pos) const;
	CIndiInfoData* GetPrev(POSITION& pos);
	CIndiInfoData* GetNext(POSITION& pos) const;
	CIndiInfoData* GetNext(POSITION& pos);
	CIndiInfoData* GetIndiInfoData(const CString& strIndicatorName);
	
	//ó�� ȭ�鿡 road �Ǿ� �ִ� IndiInfo ���� ��������.
	bool GetIndiInfoInTheBeginning(CList<IGraphInfo*, IGraphInfo*>& indiInfoList) const;
	bool GetIndiInfoIntheSave(CList<IGraphInfo*, IGraphInfo*>& indiInfoList) const;
	
	bool IsIndiInfoData( const char *p_szGraphName) const;

private:
	CList<CIndiInfoData*, CIndiInfoData*> m_indiInfoDataList;
};

class CGraphEtcData
{
public:
	CGraphEtcData();
	CGraphEtcData(const CString& strFunctionName, const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType);

	void SetFunctionName(const CString& strFunctionName);
	void SetVertScaleLogType(const bool bIsVertScaleLogType);
	void SetVertScaleInvertType(const bool bIsVertScaleInvertType);

	CString GetFunctionName() const;
	bool IsVertScaleLogType() const;
	bool IsVertScaleInvertType() const;

private:
	CString m_strFunctionName;
	bool m_bIsVertScaleLogType;
	bool m_bIsVertScaleInvertType;
};



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 9 / 6
// Explain		: 
// Using way	: �ʱ� IndicatorInfo�� �����Ѵ�.
//-----------------------------------------------------------------------------
class COrgViewData  
{
public:
	COrgViewData();
	virtual ~COrgViewData();

	// ����� �ִ� IGraphInfo ���� IGraphInfoManager�� �����ϱ�.
	bool ChangeAllIndicatorList( IGraphInfoManager* p_pIGraphInfoManager);
	
	// ó�� ȭ�鿡 road �Ǿ� �ִ� IndiInfo ���� ��������.
	bool GetIndiInfoInTheBeginning( CList< IGraphInfo *, IGraphInfo * >&indiInfoList ) const;
	bool GetIndiInfoIntheSave( CList< IGraphInfo *, IGraphInfo * >&indiInfoList ) const;
	
	bool IsIndicatorInfo( IGraphInfo* p_pIGraphInfo);				// IndicatorInfo�� �ִ��� Ȯ���Ѵ�.

	CGraphEtcData& GetGraphEtcData();
	CIndiInfoDataList& GetIndiInfoDataList();

	bool AddTail( IGraphInfo *p_pIGraphInfo, const bool bTheBeginning = false);
	void Initialize();

private:
	CIndiInfoDataList m_indiInfoDataList;
	CGraphEtcData m_graphEtcData;
	
	//bool m_bAllHiding_PriceMA;
};

#endif // !defined(AFX_ORGVIEWDATA_H__DCBD417D_6148_4171_B09C_2D25FB7736A2__INCLUDED_)
