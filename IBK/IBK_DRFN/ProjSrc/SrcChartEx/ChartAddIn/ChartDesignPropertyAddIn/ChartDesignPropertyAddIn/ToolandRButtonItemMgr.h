// ToolandRButtonItemMgr.h: interface for the CToolandRButtonItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLANDRBUTTONITEMMGR_H__FB99F7D4_2823_4045_8813_585A32E4484C__INCLUDED_)
#define AFX_TOOLANDRBUTTONITEMMGR_H__FB99F7D4_2823_4045_8813_585A32E4484C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CItemInfo;
class CToolandRButtonItemMgr  
{
public:
	CToolandRButtonItemMgr();
	virtual ~CToolandRButtonItemMgr();

	//Default Item������ �����Ѵ�.
	void ChangeItemInfo(const CString& strItmeName, LV_ITEM* plvItem);
	CItemInfo* ChangeItemInfo(const CString &strItemName, const CString &strLimit1, const CString &strLimit2);

	//strItemName�� �ش��ϴ� CItemInfo�� ��´�.
	CItemInfo* GetItemInfowithItemName(const CString& strItemName);
	//m_ToolRButtonItemList��������� ��� Item�� ������ �ʱ�ȭ�Ѵ�.
	void Init();

private:
	void SetFirst_AnalyticalTool_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	enum {
		FIRST_SUBITEM,
		SECOND_SUBITEM
	};

	enum {
		EXTENSION_REDUCTION,
		NUMERICAL_INQUIRY = 2, //��ġ��ȸâ
		VIEWDATA, //����Ÿ ��ȸ
		NUMERICAL_INQUIRY_DATAVIEW, //��ġ����Ÿ ��ȸ
		INDICATOR_SEQUNCE_INQUIRY = 13, //��ǥ������ȸ
		AUTO_LINE = 34, //�ڵ��߼���
		ANALYTICALTOOL = 35, //���ڼ�
		STOCKPRICEPREDICTION = 64, //�ְ�����
		Raider = 76, //���̴� ��Ʈ
	};

	//SubItem�� ������ Setting�Ѵ�.
	void SetSubItemInfo(const int nIndex, const CString& strItemName, CItemInfo* pItemInfo);

	//strItemName�� ���� ù��°�� ������ �ٸ���.
	//�ٸ� Data ������ Item�� ���� Setting�Ѵ�.
	void SetFirstSubItemInfo(const int nIndex, const CString& strItemName, CItemInfo* pItemInfo);
	void SetFirst_StockPrice_Prediction_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_Auto_Line_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_Indicator_Sequance_Inquiry_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_Numerical_Inquiry_DataView_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_ViewData_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_Numerical_Inquiry_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_Extension_Recuction_SubItem(const CString& strItemName, CItemInfo* pItemInfo);
	void SetFirst_Raider_SubItem(const CString &strItemName, CItemInfo *pItemInfo);

	//strItemName�� ���� �ι�°�� ������ �ٸ���.
	//�ٸ� Data ������ Item�� ���� Setting�Ѵ�.
	void SetSecondSubItemInfo(const int nIndex, const CString &strItemName, CItemInfo *pItemInfo);
	void SetSecond_Numerical_Inquiry_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetSecond_Extension_Recuction_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetSecond_ViewData_SubItem(const CString &strItemName, CItemInfo *pItemInfo);

	//CItemInfo�� ������ Control�ϴ� List�̴�.
	CObList m_ToolRButtonItemList;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}
};

#endif // !defined(AFX_TOOLANDRBUTTONITEMMGR_H__FB99F7D4_2823_4045_8813_585A32E4484C__INCLUDED_)
