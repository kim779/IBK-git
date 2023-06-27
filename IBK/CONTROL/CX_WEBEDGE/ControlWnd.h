#pragma once


// ControlWnd

class ControlWnd : public CWnd
{
	DECLARE_DYNAMIC(ControlWnd)

public:
	ControlWnd();
	virtual ~ControlWnd();


	BOOL	IsDev();
	CWnd* m_pParent;
	class CAxBrowser* m_pHtml;
	//class Ccx_THtmlExApp	*m_pApp;

	void	SetParam(struct _param* pParam);




	CString Variant(int comm, CString data);
	struct _param m_Param;
	CFont* m_pNFont, * m_pBFont;
	CRect	m_Htmlrc;
	CString	m_sURL;
	CString m_sRoot{};
	bool	m_bScroll; 
	int	m_scrGap;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


