// IndexItem.h: interface for the CIndexItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDEXITEM_H__756156D1_ED32_4C77_95F5_D7D93E9D254B__INCLUDED_)
#define AFX_INDEXITEM_H__756156D1_ED32_4C77_95F5_D7D93E9D254B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>											// for CList

#include "../Include_ST/DllSTLoader.h"
#include "./Include_AddIn/I139092/_IChartOCX.h"	// for IChartOCX

enum STINDEXTYPE 
{
	AssistIndex = 0,	// ��ǥ
	STIndex,			// ��������
	STCFG,				//
	MSTIndex			// ��������, �������ķ� ������ �̰�����.
};

interface IPacket;
class CIndexItem  
{
public:
	CIndexItem();
	virtual ~CIndexItem();
	CDllSTLoader m_DllSTLoader;	
	STINDEXTYPE m_indexType;

	// assistIndex /////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	CList<IPacket*, IPacket*> m_ListOutputIPacket;
	void SetInputValue(LPCTSTR lpInput) { m_strInputValue = lpInput; }
	void ClearInputValue() { m_strInputValue = "";}
	CString GetInputValue();
	CString m_strType;
	CString m_strScaleType;
	////////////////////////////////////////////////////////////////////////
	
	// stIndex /////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	CString m_strSTItemName;
	CString m_strPath;
	////////////////////////////////////////////////////////////////////////

private:
	// assistIndex /////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	CString m_strInputValue;
	////////////////////////////////////////////////////////////////////////

// (2006/1/19 - Seung-Won, Bae) Interface of Remove All IPacket Reference
public:
	void	RemoveAllPacket();
};

#endif // !defined(AFX_INDEXITEM_H__756156D1_ED32_4C77_95F5_D7D93E9D254B__INCLUDED_)
