#if !defined(AFX_INTERFACE_OF_SKINCOLOR_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_)
#define AFX_INTERFACE_OF_SKINCOLOR_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_

// #include "../../_Include/ISkinColorMng.h"
#include <AFXTEMPL.H>
#include "IBaseDefine.h"

//	int nID = 6;
//	ISkinColorManager* pSkinColorMng = (ISkinColorManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	ISkinColorManager* pMng = (ISkinColorManager*)AfxGetPctrInterface(nID);

// ���� : ������ �����Ϳ� �������� ������ ���� ID�� �ο��Ѵ�.
// IPCTRSite�� ������ ���� �����͸� �޴� Client�κ��̸�
// IPCTRManager�� ������ ���� �����͸� �����ϴ� Server�κ��̴�.
//
// ������  : ���ؿ� 2005.11.01(ȭ)
// History : 2005.11.01 �������̽� ����
//

// #include "../../_Include/ISkinColorMng.h"

#define		ISkinColorSite_SkinInfo_File	"Data\\Config.cfg"
#define		ISkinColor_default_File	"Data\\DefaultColorTable.cfg"
#define		ISkinColorSite_Skin_default		"��Ų����|DefaultColorTable.cfg|Main_bg|116,112,159|Treeicon.bmp|1|default"
// AFX_STATIC_DATA const TCHAR ISkinColorSite_SkinInfo_File[] = _T("Data\\Config.cfg");
// AFX_STATIC_DATA const TCHAR ISkinColorSite_Skin_default[] = _T("��Ų����|DefaultColorTable.cfg|Main_bg|116,112,159|Treeicon.bmp|1|default");


typedef struct {
	CString		m_szName;
	CString		m_szColorTable;
	CString		m_szBKImage;
	CString		m_szBKColor;
	CString		m_szTreeIconImage;
	CString		m_szVScreenImageIndex;
	CString		m_szSubColorTableInfo;
} ST_SKININFO;

typedef CArray<ST_SKININFO, ST_SKININFO> ARRAY_ST_SKININFO;
typedef CList<ST_SKININFO*, ST_SKININFO*>  LIST_ST_SKININFO;


DECLARE_INTERFACE(ISkinColorSite)
{
	// ��ŲĮ�����̺��� ����Ǿ��� ��� �Ҹ���.
	// I-PARAM : -
	// O-PARAM : -
	STDMETHOD_(void, ColorTableChanged)() PURE;
};

DECLARE_INTERFACE(ISkinColorManager)
{
	// Į��Ű�̸����� �����ϱ�
	// I-PARAM : Į��Ű
	// O-PARAM : ����
	STDMETHOD_(COLORREF, GetColor)(LPCSTR szKeyName) PURE;

	// Į��Ű�� �ش��ϴ� �ε������ϱ�
	// I-PARAM : Į��Ű
	// O-PARAM : Į���ε���
	STDMETHOD_(int, GetIndex)(LPCSTR szKeyName) PURE;

	// Į���ε����� �����ϱ�
	// I-PARAM : Į���ε���
	// O-PARAM : ����
	STDMETHOD_(COLORREF, GetColor)(int nIndex) PURE;

	// �÷����̺� ���� Array ������, (ARR_COLOR_TABLE*) �� ����
	// I-PARAM : -
	// O-PARAM : (ARR_COLOR_TABLE*) ������
	STDMETHOD_(long, GetColorTableArray)() PURE; 

	// Į�����̺��� ������ �˱����� ISkinColorSite ���
	// I-PARAM : ISkinColorSite* ��
	// O-PARAM : ISkinColorSite*�� ��ϵ� Ű��
	STDMETHOD_(long, AdviseSite)(ISkinColorSite* pSite) PURE;

	// ISkinColorSite ����.
	// I-PARAM : ISkinColorSite*�� ��ϵ� Ű��
	// O-PARAM : ����(TRUE), ����(FALSE)
	STDMETHOD_(BOOL, UnadviseSite)(long dwKey) PURE;

	// Į�����̺��� Loading�ϰ�, Notify �Ѵ�.
	// ������ Į�����̺�������� ���ο� Į�����̺������� �о���δ�.
	// �ַ� �� ����� ���ο��� �Ѵ�.
	// I-PARAM : Į�����̺���������
	//           Į�����̺� ����� ISkinColorSite�� Notify�� ������ ����, Default�� TRUE
	// O-PARAM : ����(TRUE), ����(FALSE)
	STDMETHOD_(BOOL, LoadColorTable)(LPCSTR szTableFile, BOOL bNotify=TRUE) PURE;

	// ISkinColorSite �� ColorTableChanged()�� ȣ���Ѵ�.
	// �ַ� �� ����� ���ο��� �Ѵ�.
	// I-PARAM : -
	// O-PARAM : -
	STDMETHOD_(void, NotifyToSite)() PURE;

	// Į�����̺��� ������ ������ �ִ� Config.cfg ���������� �д´�.
	// ������δ� DATA/Config.cfg �����̸�, �����ϰ��� �� �� ����Ѵ�.
	// ���������� �Ʒ��� ����.
	// {{ Config.cfg
	// [SKIN]
	// count=6
	// 01=��Ų����|DefaultColorTable.cfg|Main_bg|116,112,159|Treeicon.bmp|1|default
	// 02=��Ų�׸�1|DefaultColorTable_1.cfg|Main_bg1|169,123,125|Treeicon1.bmp|2|skin1
	// 03=��Ų�׸�2|DefaultColorTable_2.cfg|Main_bg2|114,141,100|Treeicon2.bmp|3|skin2
	// 04=��Ų�׸�3|DefaultColorTable_3.cfg|Main_bg3|157,134,113|Treeicon3.bmp|4|skin3
	// 05=��Ų�׸�4|DefaultColorTable_4.cfg|Main_bg4|115,115,115|Treeicon4.bmp|5|skin4
	// 06=��Ų�׸�5|DefaultColorTable_5.cfg|Main_bg5|121,138,140|Treeicon5.bmp|6|skin5
	// }} Config.cfg
	// 
	// I-PARAM : �������ϸ�(FullPath)
	// O-PARAM : CFG������ �����ϸ� TRUE, �ƴϸ� FALSE
	STDMETHOD_(BOOL, SetSkinConfig)(LPCSTR szCfgFileName) PURE;

	// �������Ͽ� ���õ� SKIN������ �д´�.
	// I-PARAM : -
	// O-PARAM : [SKIN]������ count�� ����.
	STDMETHOD_(int, GetSkinCount)() PURE;

	// �������Ͽ� ���õ� ST_SKININFO ����
	// example.
	//    ST_SKININFO skInfo;
	//    pSkinMng->GetSkinInfo(nIndex, skInfo);
	//
	// I-PARAM : �ε���
	// O-PARAM : �ε����� �ش��ϴ� ST_SKININFO����, ������ NULL
	STDMETHOD_(BOOL, GetSkinInfo)(int nIndex, ST_SKININFO& skInfo) PURE;

	// �⺻ Į��Ű�̸����� �����ϱ�
	// I-PARAM : Į��Ű
	// O-PARAM : ����
	STDMETHOD_(COLORREF, GetDefaultColor)(LPCSTR szKeyName) PURE;

	// �⺻ Į��Ű�� �ش��ϴ� �ε������ϱ�
	// I-PARAM : Į��Ű
	// O-PARAM : Į���ε���
	STDMETHOD_(int, GetDefaultIndex)(LPCSTR szKeyName) PURE;

	// �⺻ Į���ε����� �����ϱ�
	// I-PARAM : Į���ε���
	// O-PARAM : ����
	STDMETHOD_(COLORREF, GetDefaultColor)(int nIndex) PURE;

	// �⺻ �÷����̺� ���� Array ������, (ARR_COLOR_TABLE*) �� ����
	// I-PARAM : -
	// O-PARAM : (ARR_COLOR_TABLE*) ������
	STDMETHOD_(long, GetDefaultColorTableArray)() PURE; 

	// �⺻ Į�����̺��� ������ ������ �ִ� Config.cfg ���������� �д´�.
	STDMETHOD_(BOOL, LoadDefaultColorTable)(LPCSTR szTableFile) PURE;
};

typedef struct _tabUserColorTable
{
	CString szName;
	COLORREF rgb;
} STUSERCOLORTABLE;
typedef CArray<STUSERCOLORTABLE, STUSERCOLORTABLE>	ARR_COLOR_TABLE;


class CST_SKININFO
{
public:
	static void Convert(ST_SKININFO& stSkinInfo, LPCSTR szInoBuf)
	{
		int nCol = 0;
		AfxExtractSubString(stSkinInfo.m_szName, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szColorTable, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szBKImage, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szBKColor, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szTreeIconImage, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szVScreenImageIndex, szInoBuf, nCol++, '|');
		AfxExtractSubString(stSkinInfo.m_szSubColorTableInfo, szInoBuf, nCol++, '|');

		stSkinInfo.m_szBKImage += ".bmp";
	}
};

#endif //AFX_INTERFACE_OF_SKINCOLOR_H__320E_1464_BY_JUNOK_LEE_8B1A__INCLUDED_
