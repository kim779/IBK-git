/**
	@file		unitdatablock.h
	@brief		������ ��.
	@author		sashia
	@date		2009/08/17
*/

#if !defined(AFX_UNITDATABLOCK_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_UNITDATABLOCK_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TrData.h"

#pragma	pack(1)


/**
	@class		CTrItem
	@brief		���������� Base
	@author		sashia
	@date		2009/08/13
*/
class CTrItem : public CObject
{
	DECLARE_DYNAMIC(CTrItem);

public:
	CTrItem();
	CTrItem(const CTrItem& obj)	{ *this = obj;	};
	CTrItem(CTrItem *pItem);
	virtual ~CTrItem();

protected:
	CBaseValue	*m_pValue;	//	value pointer.
	BOOL	m_bByteOrd;		//	ByteOrder Type	TRUE:����.
	BYTE	m_uDataType;	//	Data Type
							//	1:char
							//	2:integer
							//	3:bool
							//	4:long
							//	5:double
							//	6:string
	int		m_nSize;		//	Data Size(byte ����)
	BOOL	m_bArray;		//	�迭���Ŀ���	TRUE:�迭.
	BOOL	m_bABS;			//	���밪 ����

	CString	m_strItemID;	//	������ ���� ���̵�.

	CString		m_strRealID;	//	�ǽð� ������ ID.
	CString		m_strChartID;	//	��Ʈ�� �׷��� ID.

public:
	void			SetDataType(BYTE uDataType);
	BYTE			GetDataType()					{ return m_uDataType;	};
	void			SetIsArray(BOOL bArr)	{ m_bArray = bArr;	};
	BOOL			IsArray()				{ return m_bArray;	};
	void			SetIsByteOrd(BOOL bOrd)	{ m_bByteOrd = bOrd;	};
	BOOL			IsByteOrd()				{ return m_bByteOrd;	};
	void			SetSize(int nSize);
	int				GetSize()			{ return m_nSize;	};
	CString			GetItemID()					{ return m_strItemID;	};
	void			SetItemID(CString strID)	{ m_strItemID = strID;	};
	void			SetABS(BOOL bABS)		{ m_bABS = bABS;	};
	BOOL			GetABS()				{ return m_bABS;	};

	BOOL			IsValue()	{ return (BOOL)m_pValue;	};

public:
	BOOL	m_bUseBuf;	//	��������ü ���� ������ ������� ����.
	void	SetUseBuf(BOOL bUse)	{ m_bUseBuf = bUse;	};
	BOOL	GetUseBuf()				{ return m_bUseBuf;	};

public:
	virtual void	Copy(const CTrItem& item);
	virtual CTrItem& operator = (const CTrItem& item);

	virtual int		SetDataFromBuffer(char* pszData, int nIndex);
	virtual void	SetDataFromFile(CFile *pFile);
	virtual void	RemoveAllData();

	virtual void	SetValue(void *pValue, int nIndex = -1);
	virtual int		GetValue(void **pValue, int nIndex = -1);
	virtual void	SetstrValue(CString strData, int nIndex = -1);
	virtual CString	GetstrValue(int nIndex = -1);
	virtual void	SetbValue(BOOL bValue, int nIndex = -1);
	virtual BOOL	GetbValue(int nIndex = -1);
	virtual void	SetnValue(int nValue, int nIndex = -1);
	virtual int		GetnValue(int nIndex = -1);
	virtual void	SetlValue(long lValue, int nIndex = -1);
	virtual long	GetlValue(int nIndex = -1);
	virtual void	SetdValue(double dValue, int nIndex = -1);
	virtual double	GetdValue(int nIndex = -1);

public:
	CBaseValue*			CreateValue(BYTE ucType);
	void				CopyValue(BYTE ucType, CBaseValue *pFromItem, CBaseValue *pToItem);
	void	Serialize(CArchive &ar, int nVer);

};

typedef	CArray<CTrItem*, CTrItem*>					CArrTrItem;
typedef	CMap<CString, LPCTSTR, CTrItem*, CTrItem*>	CMapTrItem;


/**
	@class		CTrItemArr
	@brief		Array ����������.
	@author		sashia
	@date		2009/08/13
*/
class CTrItemArr : public CTrItem
{
	DECLARE_DYNAMIC(CTrItemArr);

public:
	CTrItemArr();
	CTrItemArr(const CTrItemArr& obj)	{ *this = obj;	};
	CTrItemArr(CTrItemArr *pItem);
	virtual ~CTrItemArr();

protected:
	CArrValue	m_arrValue;	//	Array Value Pointer.

public:
	virtual void	Copy(const CTrItemArr& item);
	virtual CTrItemArr& operator = (const CTrItemArr& item);

	virtual int		SetDataFromBuffer(char* pszData, int nIndex);
	virtual int		SetDataFromData(char* pszData, int nIndex, CString strData);	
	virtual void	SetDataFromFile(CFile *pFile);
	virtual void	RemoveAllData();

	virtual void	SetValue(void *pValue, int nIndex = -1);
	virtual int		GetValue(void **pValue, int nIndex = -1);
	virtual void	SetstrValue(CString strData, int nIndex = -1);
	virtual CString	GetstrValue(int nIndex = -1);
	virtual void	SetbValue(BOOL bValue, int nIndex = -1);
	virtual BOOL	GetbValue(int nIndex = -1);
	virtual void	SetnValue(int nValue, int nIndex = -1);
	virtual int		GetnValue(int nIndex = -1);
	virtual void	SetlValue(long lValue, int nIndex = -1);
	virtual long	GetlValue(int nIndex = -1);
	virtual void	SetdValue(double dValue, int nIndex = -1);
	virtual double	GetdValue(int nIndex = -1);

public:
	int				GetValueCount()	{ return m_arrValue.GetSize();	};
	CBaseValue*		GetValueAt(int nIndex)	{ return m_arrValue.GetAt(nIndex);	};
};

/**
	@class		CBlockBase
	@brief		Data Structure ���� Ŭ����.
	@author		sashia
	@date		2009/08/13
*/
class CTrBlock : public CObject
{
	DECLARE_DYNAMIC(CTrBlock);

public:
	CTrBlock();
	CTrBlock(const CTrBlock& obj)	{ *this = obj;	};
	CTrBlock(CTrBlock *pItem);
	virtual ~CTrBlock();

	void			RemoveAll();
	void			Serialize(CArchive& ar, int nVer);
	BOOL			IsArray()		{ return m_bArray;	};

	BOOL			m_bArray;			//	�迭���Ŀ���	TRUE:�迭.
	CString			m_strBlockName;

	//	�迭 ������ ���� ���..
	//	�迭 ������ �����ϴ� �������� ID�� ������.
	CString			m_strCntItemID;

	CArrTrItem	m_TrItemArr;	//	Array Value Pointer.

public:
	virtual void	Copy(const CTrBlock& item);
	virtual CTrBlock& operator = (const CTrBlock& item);

public:
	int			GetItemSize()	{ return m_TrItemArr.GetSize();	};
	CTrItem*	GetItemAt(CString strID);
	void		AddItem(CTrItem* pItem)	{ m_TrItemArr.Add(pItem);	};
};

typedef	CArray<CTrBlock*, CTrBlock*>				CArrTrBlock;

#pragma	pack()

#endif // !defined(AFX_UNITDATABLOCK_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
