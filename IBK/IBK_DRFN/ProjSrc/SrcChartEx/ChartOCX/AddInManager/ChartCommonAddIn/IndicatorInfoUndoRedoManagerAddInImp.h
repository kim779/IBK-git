// IndicatorInfoUndoRedoManagerAddInImp.h: interface for the CIndicatorInfoUndoRedoManagerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORINFOUNDOREDOMANAGERADDINIMP_H__CF013FE5_409B_45BF_8CBA_8E3E934331D4__INCLUDED_)
#define AFX_INDICATORINFOUNDOREDOMANAGERADDINIMP_H__CF013FE5_409B_45BF_8CBA_8E3E934331D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn/AddInCommand.h"					// for EAI_DRDS_PACKET_END
#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase
#include "../Include_AddIn/I000000/_IString.h"
#include "../Include_AddIn/I000000/_IStringList.h"
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr
#include "../Include_Addin/I000000/_IGraphInfo.h"
#include "../Include_Addin/I000000/_IGraphInfoManager.h"


interface IChartOCX;


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 11
// Comments		: GraphInfo�� ������.
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CIndiInfoData
{
public:
	CIndiInfoData(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning = false);
	virtual ~CIndiInfoData();

public:
	static CIndiInfoData* Make(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning);
	static void Delete(CIndiInfoData* pIndiInfoData);

	void SetTheBeginning(const bool bTheBeginning);
	void SetIndicatorInfo(IGraphInfo* p_pIGraphInfo);

	bool IsTheBeginning() const;
	IGraphInfo& GetIndicatorInfo();
	IGraphInfo* GetIGraphInfo();

	CString GetIndicatorName() const;

private:
	bool m_bTheBeginning; // ó�� ȭ����� ���� ����
	IGraphInfo *m_pIGraphInfo;
};




//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 11
// Comments		: GraphInfo�� List
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CIndiInfoDataList
{
public:
	CIndiInfoDataList();
	virtual ~CIndiInfoDataList();

	bool AddTail(IGraphInfo* p_pIGraphInfo, const bool bTheBeginning);
	void DeleteAll();

	//����� �ִ� IGraphInfo ���� pTargetIndiList�� �����ϱ�.
	bool ChangeAllIndicatorList(IGraphInfoManager* p_pIGraphInfoManager);

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




//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 30
// Comments		: ��Ÿ Graph ���� ( ��ǥ��, Log, ������ ���� )
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
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
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 30
// Comments		: BackUp�ؾߵǴ� Indicator ����
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CIndicatorInfoBackUp  
{
public:
	CIndicatorInfoBackUp();
	virtual ~CIndicatorInfoBackUp();

	// ����� �ִ� IGraphInfo ���� IGraphInfoManager�� �����ϱ�.
	bool ChangeAllIndicatorList(IGraphInfoManager* p_pIGraphInfoManager);
	
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



class CIndicatorInfoUndoRedoManagerAddInImp : public CChartAddInBase  
{
public:
	CIndicatorInfoUndoRedoManagerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase );
	virtual ~CIndicatorInfoUndoRedoManagerAddInImp();
	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

		
// Attribute
public:
	static CString m_strAddInItemName;		// Multiple Item in DLL


protected:
	CList < CIndicatorInfoBackUp *, CIndicatorInfoBackUp * > m_lstIndicatorInfoBackUpUndo;
	CList < CIndicatorInfoBackUp *, CIndicatorInfoBackUp * > m_lstIndicatorInfoBackUpRedo;

	IStringList *m_pIndicatorNameList;

	bool m_bIsOnlyViewData;
	

// Method
public:
	CIndicatorInfoBackUp* BackUpIndicatorInfo( IStringList *plstIndicatorName, int nIndicatorCount );
	
	bool GetGraphNameList( const bool bIsOnlyViewData, IStringList *indicatorNameList );
	bool GetGraphNameList( IStringList *sourceIndiNameList, IStringList *targetIndiNameList );
	bool IsExistenceIndicatorName( IStringList *indicatorNameList, const char *p_szGraphName );

	bool IsActionInitilizeAllindicator();

	
protected:
	virtual BOOL OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3);

	virtual BOOL OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);

	virtual void OnResetMainBlock( void );
};

#endif // !defined(AFX_INDICATORINFOUNDOREDOMANAGERADDINIMP_H__CF013FE5_409B_45BF_8CBA_8E3E934331D4__INCLUDED_)
