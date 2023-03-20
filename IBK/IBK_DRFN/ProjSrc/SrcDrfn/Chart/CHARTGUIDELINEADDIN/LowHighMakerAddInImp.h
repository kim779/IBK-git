// LowHighMakerAddInImp.h: interface for the CLowHighMakerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOWHIGHMAKERADDINIMP_H__CE421A04_4DE9_4BEC_A1D7_DEB2E9EC620F__INCLUDED_)
#define AFX_LOWHIGHMAKERADDINIMP_H__CE421A04_4DE9_4BEC_A1D7_DEB2E9EC620F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn.h"							// for Ixxxxxx.h

//#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase
#include "Include_AddIn/I503721/_ChartAddInBase.h"		// for CChartAddInBase

//class CMainAnalysisTool;
class CLowHighMakerAddInImp : public CChartAddInBase  
{
public:
	CLowHighMakerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CLowHighMakerAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

protected:
//	CMainAnalysisTool* m_pAnalysisTool;

//{{by numabus 
protected:
	struct CDrawCacheData{
		CDrawCacheData(){
			// �׷�������
			rctGraphDrawingRegion.top = 0;
			rctGraphDrawingRegion.bottom = 0;
			rctGraphDrawingRegion.left = 0; 
			rctGraphDrawingRegion.right = 0;

			nLastXPos = 0;

			// �ִ��ּҰ�
			dViewMin = .0f;
			dViewMax = .0f;

			// log chart
			bLog = false;

			// ��������
			bReverse = false;

			// start index
			nStartIndex = -1;

			// end index
			nEndIndex = -1;

			// �������ε����� ���ϴ���ȣ�� ����ƴ��� üũ�ϴ� ����
			dDir = .0f;

			// ��ǥ������
			pt = NULL;
			// ��/���ѱ���
			pcUpDn = NULL;
			// ��ü������
			nMaxCount = 0;
			// ��ȿ������ī��Ʈ
			nEffectCount = 0;
		}

		~CDrawCacheData(){
			destroy();
		}

		inline void init(){
			destroy();

			nMaxCount = nEndIndex - nStartIndex + 1;
			pt = new POINT[nMaxCount];
			pcUpDn = new char[nMaxCount];
		}

		inline void destroy(){
			if( pt ){
				delete [] pt;
				pt = NULL;
			}
			if( pcUpDn ){
				delete [] pcUpDn;
				pcUpDn = NULL;
			}

			// ��ü������
			nMaxCount = 0;

			// ��ȿ������ī��Ʈ
			nEffectCount = 0;
		}

		inline BOOL IsChanged(CRect &_rc, int _nLastXPos
			                 , double _dViewMin, double _dViewMax
							 , BOOL _bLog, BOOL _bReverse
							 , int _nStartIndex, int _nEndIndex
							 , double _dDir){
			BOOL bChgd = FALSE;
			if( !bChgd && _rc != rctGraphDrawingRegion ){
				bChgd = TRUE;
			}
			if (!bChgd && _nLastXPos != nLastXPos) {
				bChgd = TRUE;
			}
			if( !bChgd && _dViewMin != dViewMin ){
				bChgd = TRUE;
			}
			if( !bChgd && _dViewMax != dViewMax ){
				bChgd = TRUE;
			}
			if( !bChgd && _bLog != bLog ){
				bChgd = TRUE;
			}
			if( !bChgd && _bReverse != bReverse ){
				bChgd = TRUE;
			}
			if( !bChgd && _nStartIndex != nStartIndex ){
				bChgd = TRUE;
			}
			if( !bChgd && _nEndIndex != nEndIndex ){
				bChgd = TRUE;
			}
			if( !bChgd && _dDir != dDir ){
				bChgd = TRUE;
			}
			if( bChgd ){
				rctGraphDrawingRegion = _rc;
				nLastXPos = _nLastXPos;
				dViewMin = _dViewMin;
				dViewMax = _dViewMax;
				bLog = _bLog;
				bReverse = _bReverse;
				nStartIndex = _nStartIndex;
				nEndIndex = _nEndIndex;
				dDir = _dDir;

				init();				
			}

			return bChgd;
		}

		// ��/���Ѱ� ��ġ�� �����Ѵ�.
		void SetPoint(int top, int left){
			pt[nEffectCount].x = left;
			pt[nEffectCount].y = top;
		}

		void SetPoint(int pos, int top, int left){
			if( pos<nEffectCount ){
				pt[pos].x = left;
				pt[pos].y = top;
			}
		}

		void GetDataSet(int pos, int &top, int &left, double &d ){
			if( pos<nEffectCount ){
				left = pt[pos].x;
				top = pt[pos].y;

				if( pcUpDn[pos] == 'U' ){
					d = 1;
				}
				else {
					d = 4;
				}
			}
		}

		// ���Ѱ�ǥ��
		void SetDirUp(){
			pcUpDn[nEffectCount] = 'U';
		}

		// ���Ѱ�ǥ��
		void SetDirDn(){
			pcUpDn[nEffectCount] = 'D';
		}

		// ��ȿ�������� �����Ѵ�.
		int incEffectCount(){
			return nEffectCount++;
		}

		// �׷�������
		CRect rctGraphDrawingRegion;
		// XScale ���� Ȯ���� ����..
		int nLastXPos;
		// �ִ��ּҰ�
		double dViewMin;
		double dViewMax;
		// log chart
		BOOL bLog;
		// ��������
		BOOL bReverse;
		// start index
		int nStartIndex;
		// end index
		int nEndIndex;

		// �������ε����� ���ϴ���ȣ�� ����ƴ��� üũ�ϴ� ����
		double dDir;

		// ��ǥ������
		POINT *pt;
		// ��/���ѱ���
		char * pcUpDn;
		// ��ü������
		int nMaxCount;
		// ��ȿ������ī��Ʈ
		int nEffectCount;

	};

	CDrawCacheData m_dcdCash;
//}}
protected:
	BOOL	m_bEnableMark;
protected:
	void	EnableLowHighMark( BOOL p_bEnable)		{	m_bEnableMark = p_bEnable;	}
	BOOL	GetLowHighMarkFlag( void)				{	return m_bEnableMark;	}
	//sy 2005.11.21.
	bool	IsExistenceLowHighMark( void)			{	return true;	}

	BOOL	m_bLoaded;

	CString	m_strCode;

// [05/01/19] AddIn Object�� ������ Interface�� ������ �� �ֵ��� �����Ѵ�.
public:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);

// (2004.10.11, ��¿�) �⺻���� Interface�� Default�� �����Ѵ�. (Derived Class���� ���������� �ʾƵ� ������ ���� �ʵ���)
protected:
	// [04/10/11] ������ �����ϴ� Interface�� �����Ѵ�.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
//	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare);
	// [04/10/11] AddIn DLL�鿡�� ResetChart ��Ȳ�� �˷� ó���ǵ��� �Ѵ�.
//	virtual void	OnResetChart( void);
	// [04/10/11] Draw Event�� AddIn DLL�� �˷� ó���� �� �ֵ��� �Ѵ�.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
//	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID);

public:
//{{ 2010/02/04 numabus
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);

	// �������� Real�ݿ� : �������� - ojtaso 
	virtual void	OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate);
//}}

//	virtual void OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap) {}	// ���� ȣ���
//	virtual void OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap) {}	// ���� �� ȣ���

	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);

};

#endif // !defined(AFX_LOWHIGHMAKERADDINIMP_H__CE421A04_4DE9_4BEC_A1D7_DEB2E9EC620F__INCLUDED_)
