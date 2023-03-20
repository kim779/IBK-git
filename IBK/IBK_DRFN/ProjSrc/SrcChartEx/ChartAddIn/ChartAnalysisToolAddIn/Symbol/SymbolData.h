// SymbolData.h: interface for the CSymbolData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLDATA_H__B926ECA4_D089_4373_9211_0D0DFFD39E61__INCLUDED_)
#define AFX_SYMBOLDATA_H__B926ECA4_D089_4373_9211_0D0DFFD39E61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//color, font ����
#include "PropertiesData_text.h"
//symboltype
#include "SymbolBaseData.h"
//symbol��ü data
#include "ElementTree.h"
// Undo / Redo
#include "UndoRedoManager.h"

class CDrawingSymbol;
class CSymbolData  
{
public:
	CSymbolData();
	~CSymbolData();

	IChartManager* GetMainBlock() const;
	IPacketManager* GetPacketManager() const;
	IChartOCX* GetChartOCX() const;
	CPropertiesData_text* GetPropertiesData();	
	CString GetFilePath() const;
	CWnd* GetParent() const;
	CElementTree* GetElementTree();

//>> (2008/12/16 - Seung-Lyong Park) Symbol ����
	CDrawingSymbol* CopySymbol(CDrawingSymbol *pDrawingSymbol);
//<< (2008/12/16 - Seung-Lyong Park) Symbol ����
	BOOL UndoElement(long lUndoID);
	BOOL RedoElement(long lRedoID);

	CSymbolBaseData::TOOLTYPE GetSymbolType() const;
	CSymbolBaseData::TOOLTYPE GetBuletType() const;
	CString GetSavedSymbolsData();
	CString GetUserFilePath() const;

	void RegisterElementToOrderManager(long lElementAdress);
	void UnRegisterElementToOrderManager(long lElementAdress);
	void SetMainBlock_Object(IChartManager* pMainBlock,IChartOCX *p_pIChartOCX);
	void SetFilePath(const CString& filepath);
	void SetUserFilePath(const CString& filepath);
	void SetParent(CWnd* pParent);

	void SetSymbolType(const CSymbolBaseData::TOOLTYPE symboltype);
	void SetBuletType(const CSymbolBaseData::TOOLTYPE bulettype);
	void BuildSymbolsFromString(const CString &Data);

	CDrawingSymbol* FindSymbol(const CPoint &point);
	void DrawElement(CDC* pDC, CDrawingSymbol* pSelected);

	void Invalidate();

	// UnDo / Redo 
	const CUndoRedoManager& GetUndoRedoManager() const;
	CUndoRedoManager& GetUndoRedoManager();
	BOOL MoveSymbol(CDrawingSymbol* pSelectedSymbol, CString strSymbolData_Prev = "");
	CBlockIndex GetBlockIndexInSymbol(long lSymbolAddress) const;
	COrder& GetOrder();

	// 2011.02.07 by SYS >> ����Ʈ YYYY(��) �߰� : ��¥/�ð� 14Byte���� ���� (YYYYMMDDhhmmss)
	CScaleBaseData::HORZSCALEDRAWERTYPE m_eScaleType;
	void SetXScaleDrawType(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType);
	CScaleBaseData::HORZSCALEDRAWERTYPE GetXScaleDrawType();
	// 2011.02.07 by SYS <<

public:
	static CString		m_strAddInItemName;
private:
	//�θ�
	CWnd* m_pParent;
	//MainBlock
	IChartManager* m_pMainBlock;
	IPacketManager  *m_pIDefaultPacketManager;
	IChartOCX		*m_pIChartOCX;

	CString m_strFilePath;
	CString m_strUserFilePath;

	//SymbolType
	CSymbolBaseData::TOOLTYPE m_TYPESymbol;
	CSymbolBaseData::TOOLTYPE m_TYPEBulet;

	//��ȣ Dlg�� color,font ����
	CPropertiesData_text m_Properties;
	//��ü ����Ʈ.
	CElementTree m_elementTree;
	CUndoRedoManager m_UndoRedoManager;		// Undo / Redo

// 20081007 JS.Kim	����ڽð��� ����
protected:
	IPacketListManager *	m_pIPacketListManager;
public:
	IPacket *	GetDatePacket( const char *p_szRQ) const;
};

#endif // !defined(AFX_SYMBOLDATA_H__B926ECA4_D089_4373_9211_0D0DFFD39E61__INCLUDED_)
