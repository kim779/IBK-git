// MainSymbolImplementation.h: interface for the CMainSymbolImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINSYMBOLIMPLEMENTATION_H__1F229275_48B5_490D_B1E9_0E54D5FA62EC__INCLUDED_)
#define AFX_MAINSYMBOLIMPLEMENTATION_H__1F229275_48B5_490D_B1E9_0E54D5FA62EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>

#include "../Include_Analysis/BlockBaseEnum2.h"				// for CSymbol

//��ȣ Dlg
#include "SymbolDlg.h"
//symboldata
#include "SymbolData.h"
//CBlockIndex
#include "BlockBaseData.h"
#include "SymbolSingleton.h"

class CSymbolTool;
class CMainSymbolImplementation  
{
public:
	CMainSymbolImplementation();
	virtual ~CMainSymbolImplementation();

	//Object Dll�� �޴´�.
	void SetMainBlock_Object(IChartManager* pMainBlock,IChartOCX *p_pIChartOCX);
	void InitializeBlock(CDC* pDC, CWnd* pParentWnd);
	void SetCurrentSymbolTool(const CSymbol::SYMBOLTOOL symbolType);
	void SetShapeDrawingType(const COLORREF& crLine,const int& nStyle,const int& nWeight,const bool& bFill);
	//��ȣDlg�� �����ϰ� ����.
	void CreateSymbolDlg();

	bool OnLButtonDown(CDC* pDC, const CPoint &point);
	bool OnLButtonDbClk(CDC* pDC, const CPoint &point);
	bool OnLButtonUp(CDC* pDC, const CPoint &point);
	bool OnMouseMove(CDC *pDC,const UINT &nFlags, const CPoint &point);
	bool OnRButtonUp(const CPoint &point);
	bool OnRButtonMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam);

	void OnDraw(CDC *pDC);	
	void OnSetColor(const COLORREF& color);

	//Block�� Insert�Ѵ�.
	bool InsertBlockIndex(const CBlockIndex& insertBlockIndex);
	//Block�� �̵����� Row, Column�� �ٲ۴�.
	void SetModifiedBlockIndexs(const CBlockIndex& sourceBlockIndex, const CBlockIndex& TargetBlockIndex);

	//�ش� Symbol�� �����.
	bool DeleteCurrentSymbols();
	void DeleteAllSymbols();
	//������ �����.
	void DeleteAllSymbolsInBlock(const CBlockIndex& blockIndex);

	void DeleteSymbolDlg();

	void Initial();
	CString GetSavedSymbolsData();
	void BuildSymbolsFromString(const CString& Data);

	void SetFilePath(const CString& path);

	CSymbol::SYMBOLTOOL GetToolType();
	CSymbol::SYMBOLTOOL GetSelectedToolType();
	void DeleteLastSymbol();
	bool DeleteSymbolPos(const long lAddress);
	bool UndoElement(const long lAddress);
	bool RedoElement(const long lAddress);

	void SetElementCopy();

	void DrawSymbolTool(CDC *pDC);
	CRect GetDrawingTotalRegion(CPoint point);

	void ChangeAllSymbolsColor(const COLORREF& crLine,const COLORREF& crText);
	// 20081007 JS.Kim	����ڽð��� ����
	bool RecalcSymbolXPoint(const char* szName, int nTimeDiff);
	void RecalcElementListXPoint(int row, int col, int nBaseTimeDiff, IPacket* pPacket);

	// 2011.02.07 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CScaleBaseData::HORZSCALEDRAWERTYPE m_eScaleType;
	void SetXScaleDrawType(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType);
	CScaleBaseData::HORZSCALEDRAWERTYPE GetXScaleDrawType();
	// 2011.02.07 by SYS <<

protected:
	static CMap<int, int, CSymbolDlg*, CSymbolDlg*>	g_mapSymbolDlg;
	static CMap<int, int, CSymbolBaseData::TOOLTYPE,CSymbolBaseData::TOOLTYPE> g_mapSymbolType;
	
public:
	static	void	InitSymbolDlg();
	static	void	DestSymbolDlg();
	static	BOOL	IsExistBuletDlg();
	static  void	SetSymbolType(const CSymbolBaseData::TOOLTYPE bulettype);

private:
	IChartOCX *m_pIChartOCX;
	
	CPoint m_ptOldMouse;

	// 2011.02.08 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CPoint m_ptOldLBtnUp;
	// 2011.02.08 by SYS <<

	//��ȣ Dlg�� ������.
	CSymbolDlg* m_pSymbolDlg;
	//���õ� Symbol�� ������.
	CDrawingSymbol* m_pDrawingSymbol;
	//���õ� Symbol Tool.
	CSymbolTool* m_pSymbolTool;	
	//symboldata
	CSymbolData m_symboldata;
	CSymbolSingleton m_singleton;
	
	//mouse�� ���� ��� �־�� �ϴ°����� ����.
	bool IsMyMouse();
	//������ Tool
	void GetSymbolTool();
	//color 
	bool SetColor(const COLORREF& symbolColor);
};

#endif // !defined(AFX_MAINSYMBOLIMPLEMENTATION_H__1F229275_48B5_490D_B1E9_0E54D5FA62EC__INCLUDED_)
