#if !defined(AFX_FXBASKETTREE_H__FDFAA808_97FC_4950_8108_E4F96F879E25__INCLUDED_)
#define AFX_FXBASKETTREE_H__FDFAA808_97FC_4950_8108_E4F96F879E25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "afxtempl.h"

// basket type
#define	bskNEW	0x0001		// 신규
#define	bskCHS	0x0002		// 청산
#define	bskETF	0x0004		// ETF

// basket status
#define	bskWAIT		1	// 대기
#define	bskJUMUN	2	// 주문
#define	bskEND		3	// 완료

class _tInfo {
public:
	int	depth;		// tree depth
	char	gubn[1];	// 구분(조회/ETF)
	CString	code;		// basket 코드
	CString	gicd;		// 기관코드 
};

// gubn
#define		bsks_SEARCH	'1'	// 조회
#define		bsks_ETF	'9'	// ETF

////////////////////////////////////////////////////////////////////////////
// 바스켓 Tree 
// 조회 bmid
// 내용 bgroup + blist*acnt
////////////////////////////////////////////////////////////////////////////
#define	PIHOBLST	"pihoblst"
const int BSKTRKEY = 10;

struct  bmid {
	char	usid[8];		// 사용자ID
};

struct	bgroup {
	char	bskg[1];		// field id
	char	fnam[20];		// field name
	char	acnt[3];		// basklist count
};
#define	sz_BGROUP	sizeof(struct bgroup)

struct	blist {
	char	gicd[3];		// 기관코드
	char	ginm[20];		// 기관명
	char	bseq[12];		// bask 일련번호
	char	jsta[1];		// 주문STEP
					// 1:대기, 2:주문, 3:완료
	char	chgb[2];		// 차익/비차익 구분
					// 1 : 차익, 2 : 비차익
	char	bskn[50];		// bask name
};
#define sz_BLIST	sizeof(struct blist)

// bgroup.bskg
#define		bskg_NEWBK	'1'	// 신규바스켓
#define		bskg_CHUNGBK	'2'	// 청산바스켓
#define		bskg_ETFBK	'3'	// ETF
////////////////////////////////////////////////////////////////////////////

class  AFX_EXT_CLASS CfxBasketTree : public CTreeCtrl
{
public:
	CfxBasketTree();
	virtual ~CfxBasketTree();

	//{{AFX_VIRTUAL(CfxBasketTree)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CfxBasketTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void	Init(CWnd* pParent, CWnd* pSendWnd, WORD bskType = bskNEW|bskCHS);
	void	BasketSearch(CString userID);
	void	SetTreeInfo(char *buf);
	_tInfo*	GetCodeInfo(HTREEITEM item);
	void	SetBasketStatus(HTREEITEM item, int status);	
	int	GetBasketStatus(HTREEITEM item);
	CString	GetETFInfo();
	HTREEITEM	InsertItemEx(CString text, HTREEITEM rItem, _tInfo* tInfo);
private:
	void	removeAll();

private:
	CWnd		*m_pParent;
	CWnd		*m_pSendWnd;
	WORD		m_bskType;
	CImageList	*m_ImageList;

	CFont		m_font;
	LOGFONT		m_lf;
	CPtrArray	m_arPtr;
	struct	_etfcode {
		CString code;
		CString name;
	};
	CArray <_etfcode, _etfcode>	m_arETF;

	
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_FXBASKETTREE_H__FDFAA808_97FC_4950_8108_E4F96F879E25__INCLUDED_)
