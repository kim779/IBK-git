// GraphColorCollection.h: interface for the CGraphColorCollection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHCOLORCOLLECTION_H__0BDDB464_0354_415D_B4C7_2DAE5769B8A3__INCLUDED_)
#define AFX_GRAPHCOLORCOLLECTION_H__0BDDB464_0354_415D_B4C7_2DAE5769B8A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Include_AddIn.h"							// for Ixxxxxx.h

class CGraphProperty;
class CDlgSubGraphSetting;
class CGraphSettingButton;
class CLineColor;
class CGraphColorCollection  
{
public:
	//GraphSettngDlg���� ��ǥ�� ����������, ��ǥ�� �´� SubGraphDlg�� Button����.
	void CreateSubGraphSettingDialogandButton(CRect& rcSubFrame, CRect& rcButtonFrame, const CString& strIndicatorType, const CString& strIndicatorName, const CString& strSubGraphDatas, CLineColor* pLineColor, IGraphInfoManager *p_pIGraphInfoManager, int idSubGraphDlg, CWnd* pWnd);
	//��ǥ�� �����϶� �����Ǿ� �ִ� ��Į�� �� SubGraph�� Setting.
	void SetLineColor(const CString& strLineColor);

	//DlgGraphColor�� ��Ʈ�ѿ� ocx�� ����Ǿ� �ִ� �� ����
	void SetSubGraphDatas( IGraphInfoManager *p_pIGraphInfoManager, CString strSubGraphDatas);
	//ocx�� �ѱ�� ���� DlgGraphColor�� ��Ʈ�ѿ� �����Ǿ� �ִ� ���� �ϳ��� String���� ���� �� ���.
	CString GetSubGraphDatas( const char *p_szIndicatorName);

    // display the requested subgraphsettingdlgs (by default the active subgraphsettingdlg is displayed)    
    bool ShowSubGraphSettingDlg(int ix = -1);

	CGraphColorCollection( HWND p_hOcxWnd);
	virtual ~CGraphColorCollection();

protected:

private:
	void GetGraphType(const CString& strIndicatorType, CString& strType, IGraphInfoManager *p_pIGraphInfoManager);
	// array with all SubGraphSettingDlgs.
    CArray<CDlgSubGraphSetting*, CDlgSubGraphSetting*> m_ArraySubGraphSettingDlg; 
	// array with all buttons
    CArray<CGraphSettingButton*, CGraphSettingButton*> m_ArrayButton; 
    int m_nActiveSubGraphSettingDlg;  // index to the currently active subgraphsettingdlg
    CRect m_Rect;       // position of the subgraphsettingdlg

	//�� ��ǥ�� �ش��ϴ� Graph�� ����
	void GetSubGraphCount( IGraphInfoManager *p_pIGraphInfoManager, const CString& strIndicatorName, int& nGraphCount);
	//�Ӽ� Dll���� SubGraphName�� Count�� ������.
	void GetSubGraphNameandTypeAndStyle( IGraphInfoManager *p_pIGraphInfoManager, const CString& strIndicatorName, CString& strSubGraphName, CString& strTitle, CString& strType, CString& strStyle, CString& strDrawStyle);

	//��ư ����.
	void CreateButtonlikeasTab(CRect& rcButtonFrame, const int nGraphCount, CString& strSubGraphName, CWnd* pWnd);
	bool CreateButton(int nIndex, CSize FontSize, CString text, CWnd* pParentWnd);
	CSize GetTextSize(CWnd* pWnd);

	// create a subGraphDlg with a given resource template
	void CreateSubGraphDlgs(CRect& rcSubFrame, const int nGraphCount, 
							const CString& strIndicatorType, 
							const CString &strSubGraphName, 
							const CString& strSubGraphTitle, 
							CString &strType,
							CString &strStyle, 
							CString& strDrawStyle,
							const CString& strSubGraphDatas,
							CLineColor* pLineColor, 
							IGraphInfoManager *p_pIGraphInfoManager, 
							int idSubGraphDlg, CWnd* pWnd);
    bool CreateSubGraphDlg(CString& strSubGraphName, CString& strSubGraphTitle, 
		int nType, int nStyle, int nDrawStyle, UINT id,CWnd* pParentWnd);
	// internal method for adding a newly created subgraphsettingdlg
    int AddSubGraphSettingDlg(CDlgSubGraphSetting* dlg) { return(m_ArraySubGraphSettingDlg.Add(dlg));}; 
	int AddButton(CGraphSettingButton* pbutton);  // internal method for adding a newly created button
	
	void WhenIndicatorTypeisLineTypeSetLineColor(const int nGraphCount, const CString& strIndicatorType, CLineColor* pLineColor);    
	bool SetActive(int ix);   // set and display an other subgraphsettingdlgs

	//��ǥ�� ������ ���������� ������ ������ Dlg���� �����ϰ� �ٽ� ����.
	void DeleteSubGraphSettingDlgandButton();

// (2006/11/23 - Seung-Won, Bae) Support Scroll in SubGraph Tab Button
protected:
	BOOL		m_bMoreButtons;
	int			m_nSubGraphButtonStartIndex;
public:
	//�ǿ����� �ϱ����� ������ ��ư�� Dlg���� ��ġ �ο�.
	void		SetButtonPos( CWnd *pParentWnd, int p_nStartIndexChanging);
    // set the position of the subgraphsettingdlgs and switch the center mode on
    void		SetFramePos(const CRect& rect)	{	m_Rect = rect;	}

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
};

#endif // !defined(AFX_GRAPHCOLORCOLLECTION_H__0BDDB464_0354_415D_B4C7_2DAE5769B8A3__INCLUDED_)
