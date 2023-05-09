// TreeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "TreeWnd.h"
#include "sharemsg.h"
#include "InputPswd.h"
#include "MainWnd.h"

#include "../../h/group.h"
#include "../../h/ledger.h"
#include "qsort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CTreeWnd


#pragma warning(disable : 6387)

CTreeWnd::CTreeWnd(CWnd* pMainWnd)
{
	m_pMainWnd = pMainWnd;
	m_bCustomer = false;
}

CTreeWnd::~CTreeWnd()
{
	m_accnT.RemoveAll();
	m_pSData.Reset();
}

BEGIN_MESSAGE_MAP(CTreeWnd, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeWnd)
	ON_WM_CREATE()
	
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()

int CTreeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	init();

	return 0;
}

void CTreeWnd::init()
{
	m_root.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETROOT));
	m_user.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETUSER));
	m_id.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETID));
	m_pass.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_VARIANT, passCCx)));
	
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	if (pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0))
		m_bCustomer = true;
}

LONG CTreeWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	const int	kind = HIWORD(wParam);
	switch (LOWORD(wParam))
	{
	case MK_RECVDATA:
		RecvOper(kind, (CRecvData*)lParam);
		break;
	case MK_CALLINTEREST:
		{
			SendInterest(lParam);
		}
		break;
	case MK_TREEDATA:
		{
			class CGridData* sdata = (class CGridData*)lParam;
			GetData(*sdata, kind);		//2
		}
		break;
	case MK_REMAIN:
		{
			m_accnT.RemoveAll();
			m_pSData.Reset();

			m_accnT.Add(m_strAccount);

			((CMainWnd*)m_pMainWnd)->m_strAccount = m_strAccount;
			SendTotalRemain(m_strAccount);
		}
		break;
	}

	return ret;
}

BOOL CTreeWnd::SendOper(UINT kind, int opt)
{
	const BOOL ret = TRUE;
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	
	class CGridData	sdata;
	sdata.SetKind(kind);
	const int	max = pWnd->SendMessage(WM_MANAGE, MK_GETMAX);
	GetData(sdata, max, opt); 
	return ret;  //test 우선 여기까지... 너무 복잡하다

	if (ret)
		SendTreeData(sdata);
	return ret;
}

void CTreeWnd::SendTreeData(const class CGridData& sdata)
{
	m_pMainWnd->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)&sdata);	//3
}

CString CTreeWnd::MakePacket(CString& code, CString amount, CString price, CString name, CString bookmark, CString futureGubn, CString creditPrc, CString maeipPrc)
{
	code.TrimLeft(); 
	code.TrimRight();
	if (code.IsEmpty())
		return _T("");

	code += "\t";
	if (!amount.IsEmpty())
		code += amount;

	code += "\t";
	if (!price.IsEmpty())
		code += price;

	code += "\t";
	if (!name.IsEmpty())
		code += name;

	code += "\t";
	if (!bookmark.IsEmpty())
		code += bookmark[0] == '1' ? "1":"0";	

	// 선물/옵션 잔고 조회시 구분값 추가
	code += "\t";
	if (!futureGubn.IsEmpty())
		code += futureGubn;

	code += "\t";
	if (!creditPrc.IsEmpty())
		code += creditPrc;

	code += "\t";
	if (!maeipPrc.IsEmpty())
		code += maeipPrc;

	return code;
}

void CTreeWnd::GetData(class CGridData& sdata, int max, int opt)
{
	((CMainWnd*)m_pMainWnd)->Request_GroupCode(sdata.GetKind());
}

CString CTreeWnd::GetString(char *pChar, int nMinLen)
{
	CString sRtn = CString(pChar, min(nMinLen, (int)strlen(pChar)));
	sRtn.TrimRight();
	return sRtn;
}

CString CTreeWnd::Variant(int comm, CString param /* = _T("" */)
{
	CString	ret;
	ret.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_VARIANT, comm), (LPARAM)(char*)param.operator LPCTSTR()));
	return ret;
}

void CTreeWnd::RecvOper(int kind, CRecvData* data)
{
	if(kind == 0)
		kind = 4;

	switch (kind)
	{
	case TRKEY_REMAIN:
		{
			struct	_extTHx* th = (struct _extTHx*)data->m_lParam;
			oubRemain(th->data, th->size);
		}
		
		break;
	case TRKEY_NEWS:
		parsingNews(data);
		break;
	}

}

void CTreeWnd::parsingNews(CRecvData* data)
{
	struct grid
	{
		char cod2[12]{};
	};

	struct  mod 
	{
	    char    nrec[4]{};
	    struct grid	grid[1]{};
	}*mod{};

	char*	buf = (char*)data->m_lParam;
	mod = (struct mod*)buf;
	CString	nrec, code;
	int	ncnt = 0;
	const int	nsize = sizeof(struct grid);
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);

	CGridData	sdata;
	const UINT	kind = MAKE_TREEID(xISSUE);
	sdata.SetKind(kind);

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'A')
				code = code.Mid(1);
			sdata.m_arDatas.Add(code);
		}
	}
	
	SendTreeData(sdata);
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);
}

BOOL CTreeWnd::IsNumber(CString str)
{
    BOOL bRet = TRUE;
    const  int nLen = str.GetLength();
    for( int i=0 ; i < nLen ; ++i)
    {
        if( isdigit(str.GetAt(i)) == FALSE)
        {
            bRet = FALSE;
            break;
        }
    }
    return bRet;
}

void CTreeWnd::SendTotalRemain(CString strAccount)
{
	if(strAccount.GetLength() != 11)
		return;

	CString tempAccount = strAccount;
	
	strAccount.Remove(_T('-'));
	strAccount.TrimLeft();
	strAccount.TrimRight();
	
	CString pswd, acctdata, strMember;
	bool bIsMember;
	
	acctdata = Variant(accountCC, "");
	strMember = Variant(userCC, "");
	bIsMember = FALSE;

	pswd = m_accn.loadPass(acctdata,strAccount);
	
	//직원 고객 구분
	if(IsNumber(strMember))
	{
		pswd = m_accn.loadPass(acctdata,strAccount);
		if(pswd == "")
		{
			pswd = ((CMainWnd*)m_pMainWnd)->m_Pass;
		}
	}
	else
	{
		pswd = "9999";
	}
	if(strAccount.GetLength() > 0 && strAccount.Mid(3, 1) == "2")
	{
		queryFutureRemain2(strAccount, pswd);
	}
	else
	{
		queryRemain2(strAccount, pswd);
	}
}

void CTreeWnd::SendInterest(int item)
{
	const int opt = 0;

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = ((CMainWnd*)m_pMainWnd)->GetMViewType();
	
	//MO_VISIBLE모드일때는 항상 맨 앞을 선택하도록 한다
	if(nOver == MO_VISIBLE)
	{
		pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
		pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)0);
	}	

	if (SendOper(item, opt))
	{
		const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)item);	
	}
}

void CTreeWnd::queryFutureRemain2(CString strAccount, CString sPswd)
{
	const int skey=-1;
	struct _ledgerH ledger;
	
	FillMemory(&ledger, L_ledgerH, ' ');
	
	const CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	pView->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	
	CopyMemory(&ledger.svcd, FutureService, sizeof(ledger.svcd));
	CString strUser = CString((char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETID));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	
	CString strBrno = (char*)pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, deptCC), 0L);
	CopyMemory(&ledger.brno, strBrno.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '2';
	ledger.odrf[0] = '1';
	
	CString strLedger = CString((char*)&ledger, L_ledgerH);

	struct f_mid mid;
	
	FillMemory(&mid, L_fmid, ' ');
	CopyMemory(&mid.accn, strAccount, sizeof(mid.accn));
	CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
	CopyMemory(&mid.date, "", sizeof(mid.date));


	CString strSendData(_T(""));
	CString strMid = CString((char*)&mid, L_fmid);
	strSendData = strLedger + strMid;
	
	CSendData sData;
	sData.SetData("PIBOFJGO", TRKEY_REMAIN, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
}

void CTreeWnd::queryRemain2(CString strAccount, CString sPswd)
{
	struct _ledgerH ledger;
	
	FillMemory(&ledger, L_ledgerH, ' ');
	
	const CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	pView->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	
	CopyMemory(&ledger.svcd, StockService, sizeof(ledger.svcd));
	CString strUser = CString((char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETID));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	
	CString strBrno = (char*)pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, deptCC), 0L);
	CopyMemory(&ledger.brno, strBrno.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';
	
	CString strLedger = CString((char*)&ledger, L_ledgerH);

	struct s_mid mid;
	
	FillMemory(&mid, L_smid, ' ');
	CopyMemory(&mid.accn, strAccount, sizeof(mid.accn));
	CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
	mid.allf[0] = '1';
	
	CString strSendData(_T(""));
	CString strMid = CString((char*)&mid, L_smid);
	strSendData = strLedger + strMid;
	
	CSendData sData;

	sData.SetData("PIBOSJG2", TRKEY_REMAIN, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");  //20200423 잔고조회TR변경
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);

}

BOOL CTreeWnd::sendTR(CString sTR, CString sData, int nKey, int nStat, CString Acnt)
{
	CString sTRData = "";
	CString sKey = "";
	//int ikey = -1;
	struct _userTH user;
	strncpy(&user.trc[0], sTR, 8);
	user.key  = nKey;//ikey;
	
	user.stat = nStat;
	
	sTRData  = CString((char*)&user, sizeof(_userTH));
	sTRData += sData;
	
	return m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, sData.GetLength()), 
		(LPARAM)(const char*)sTRData); 
}

void CTreeWnd::oubRemain(char* buf, int len)
{
	struct _ledgerH ledger;
	CopyMemory(&ledger, (void*)buf, L_ledgerH);
	CString strLedger = CString((char*)&ledger, L_ledgerH);
	CString strErrCode = CString((char*)&ledger.emsg, 4);
	CString strErrText = CString((char*)&ledger.emsg, 98);
	CString strServiceName = CString((char*)&ledger.svcd, 8);

	m_pSData.SetKind(m_kind);

	CString strMsg = "ERR\t" + strErrText;
	if (strErrCode.GetAt(0) != _T('0'))	// 오류
	{
		strMsg.Format("[%s]%s", strErrCode, strErrText.Right(strErrText.GetLength() - 4));
		if(strErrCode.Find("4589") >= 0 )	
			MessageBox(strMsg, "IBK투자증권");
	}
	else
	{
		buf += L_ledgerH;
		
		if(strServiceName == FutureService)		//선물잔고
		{
			settingDataFuture2(buf, m_pSData);	
		}
		else			//현물잔고
		{
			settingDataStock2(buf, m_pSData);
		}
	}

	SendTreeData(m_pSData);
}

void CTreeWnd::settingDataStock2(char* buf, class CGridData& sdata)
{
	struct s_mod	mod {};
	
	int sizeJango = 0;
	int nRec = 0;
	const int sBuf = strlen(buf);
	CString strTotalCode;

	strTotalCode = "";
	
	sizeJango = sizeof(mod);
	CopyMemory(&mod, (void*)buf, min(sizeJango, sBuf));
	nRec = atoi((LPCTSTR)CString(mod.nrec, sizeof(mod.nrec)));
	
	CString qty, AvrUprc, loanDtlcode, creditPrc, maeipPrc;
	int iQty{}, icreditPrc{}, iMaeipPrc{};
	double dPrice{};
	CString result_qty, result_Price, result_CPrice, result_MPrice;

	if(nRec > 100)
		nRec = 100;

	for (int i = 0 ; i<nRec ; i++)
	{
		const struct s_rec rec = mod.rec[i];
		CString strCode(rec.cod2, sizeof(rec.cod2));
		strCode.TrimRight();
		

		if (!strCode.IsEmpty())
		{
			if (strCode.GetAt(0) == _T('A') || strCode.GetAt(0) == _T('J') || strCode.GetAt(0) == _T('Q')) //202004 ETN 종목
			{
				if(strlen(strCode) == 7)
					strCode.Delete(0);
			}
			
			qty = CString(rec.jqty, sizeof(rec.jqty));
			AvrUprc = CString(rec.pamt, sizeof(rec.pamt));
			loanDtlcode = CString(rec.sycd, sizeof(rec.sycd));
			creditPrc = CString(rec.samt, sizeof(rec.samt));
			maeipPrc = CString(rec.mamt, sizeof(rec.mamt));
			
			//			if(loanDtlcode != "80")		//신용은 표기하지 않음
			{
				if(qty.Left(1) == '+')
					qty.Delete(0);
				
				if(AvrUprc.Left(1) == '+')
					AvrUprc.Delete(0);
				
				if(creditPrc.Left(1) == '+')
					creditPrc.Delete(0);
				
				if(maeipPrc.Left(1) == '+')
					maeipPrc.Delete(0);
				
				iQty = atoi(qty);
				dPrice = atof(AvrUprc);
				iMaeipPrc = atoi(maeipPrc);
				icreditPrc = atoi(creditPrc);
				
				result_qty.Format("%d", iQty);
				result_Price.Format("%.0f", dPrice);
				result_CPrice.Format("%d", icreditPrc);
				result_MPrice.Format("%d", iMaeipPrc);
				
				strTotalCode += "\t";
				strTotalCode += strCode;

				//패킷생성(종목번호, 잔고, 매입단가)
				sdata.m_arDatas.Add(MakePacket(strCode, result_qty, result_Price, "", "", "", result_CPrice, result_MPrice));
			}
			
		}
	}	


	m_pMainWnd->SendMessage(WM_MANAGE,MK_BOUCODE,(LPARAM)strTotalCode.operator LPCTSTR());

}

void CTreeWnd::settingDataFuture2(char* buf, class CGridData& sdata)
{
	struct f_mod mod {};
	
	int sizeJango = 0;
	int nRec = 0;
	const int sBuf = strlen(buf);
	
	sizeJango = sizeof(mod);
	CopyMemory(&mod, (void*)buf, min(sizeJango, sBuf));
	nRec = atoi((LPCTSTR)CString(mod.nrec, sizeof(mod.nrec)));
	
	CString qty, AvrUprc, gubn, maeipPrc, pyungaPrc, sonikPrc;
	int iQty = 0;
	
	double dPrice = 0;
	CString result_qty, result_Price, result_maeipPrice, result_gubn;
	double dmaeipPrice{};
	
	for (int i = 0 ; i<nRec ; i++)
	{
		const struct f_rec rec = mod.rec[i];
		CString strCode(rec.cod2, sizeof(rec.cod2));
		strCode.TrimRight();
		
		if (!strCode.IsEmpty())
		{
			qty = CString(rec.bqty, sizeof(rec.bqty));
			AvrUprc = CString(rec.curr, sizeof(rec.curr));
			gubn = CString(rec.dsgb, sizeof(rec.dsgb));
			maeipPrc = CString(rec.tamt, sizeof(rec.tamt));
			
			if(qty.Left(1) == '+')
				qty.Delete(0);
			
			if(AvrUprc.Left(1) == '+')
				AvrUprc.Delete(0);
			
			if(maeipPrc.Left(1) == '+')
				maeipPrc.Delete(0);

			if(gubn.Left(1) == '+' || gubn.Left(1) == '-')
				gubn.Delete(0);
			
			gubn.TrimLeft();gubn.TrimRight();

			if(gubn == "매수")
			{
				result_gubn = "2";
			}
			else
			{
				result_gubn = "1";	
			}

			iQty = atoi(qty);
			dPrice = atof(AvrUprc);
			dmaeipPrice = atof(maeipPrc);
			
			result_qty.Format("%d", iQty);
			result_Price.Format("%.2f", dPrice);
			result_maeipPrice.Format("%.2f", dmaeipPrice);
			
			//패킷생성(종목번호, 잔고, 매입단가)
			sdata.m_arDatas.Add(MakePacket(strCode, result_qty, result_Price, "", "", result_gubn, "", result_maeipPrice));
			
		}
	}
}

void CTreeWnd::receiveOub(CString& data, int key)
{	
	CGridData sdata;
	
	sdata.m_kind = 0;
	const CString skey = data.Left(2);
	const CString sGroupName = data.Mid(2, 20);

	((CMainWnd*)m_pMainWnd)->_groupName = sGroupName;
	const CString sCount = data.Mid(22, 4);
	data = data.Mid(26);

	constexpr UINT newbooksize = sizeof(_bookmarkinfo);

	UINT	fileSize = 0;
	UINT	readL{};
	int     count = 0;
	CFile   fileB;
	CString sBookBuffer;
	CString filePath;
	CString	code, amount, price, name, bookmark;

	filePath.Format("%s/%s/%s/bookmark.i%0*d", m_root, USRDIR, m_user, 2, key);
	if (fileB.Open(filePath, CFile::modeRead | CFile::shareDenyNone))
	{
		fileSize = gsl::narrow_cast<UINT>(fileB.GetLength());
		readL = fileB.Read(sBookBuffer.GetBuffer(fileSize + 1), fileSize);
		sBookBuffer.ReleaseBuffer();
		if (readL != fileSize)
			return;

		fileB.Close();
	}

	if (fileSize)
		count = fileSize / sizeof(struct _bookmarkinfo);
	const gsl::span<struct _bookmarkinfo> bookspan((struct _bookmarkinfo*)sBookBuffer.GetString(), count);
	const gsl::span<struct _jinfo> codelist((struct _jinfo*)data.GetString(), atoi(sCount));

	int ii = 0;
	for_each(codelist.begin(), codelist.end(), [&](auto item) {

		if (bookspan.size() > ii)
			bookmark = CString(bookspan.at(ii).bookmark[0], 1);
		else
			bookmark = "0";

		code = CString(item.code, codelen).Trim();
		name = "";
		if (code.IsEmpty())
			code = "emptyrow";

		if (code[0] == 'm' && name.IsEmpty() && bookspan.size() > ii)
		{
			CString bookCode = CString(bookspan.at(ii).code, codelen).Trim();
			if (code.CompareNoCase(bookCode) == 0)
				name = CString(bookspan.at(ii).name, namelen).Trim();
		}

		amount = CString(item.xnum, xnumlen).Trim();
		price = CString(item.xprc, pricelen).Trim();

		sdata.m_arDatas.Add(MakePacket(code, amount, price, name, bookmark));
		if (sdata.GetCount() == 100)
			return;
		ii++;
	});

	SendTreeData(sdata);
}



































/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

