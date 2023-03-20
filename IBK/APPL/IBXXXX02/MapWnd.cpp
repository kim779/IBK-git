// MapWnd.cpp : implementation file

#include "stdafx.h"
#include "IBXXXX01.h"
#include "MapWnd.h"
#include "DefineAll.h"

#include "../../h/fxCommonx.h"
#include "../../h/jmcode.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#define	_FILE_DEBUG
#ifdef	_FILE_DEBUG
#include <fstream.h>
#endif

CMapWnd::CMapWnd()
{
	m_sCurrKey = "";
	m_system = 'D';
	m_arKey.RemoveAll();
	m_hcnt = 0;

	m_hcB = NULL;
	m_hcode.RemoveAll();
	m_hname.RemoveAll();

}

CMapWnd::~CMapWnd()
{
	if (m_hcB) delete [] m_hcB;

	m_hcode.RemoveAll();
	m_hname.RemoveAll();
}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_REMAIN, OnRequestOfCtrl)
	ON_MESSAGE(WM_LAWCHEG, OnRequestLawCheg)
	ON_MESSAGE(WM_REMAIN_CLOSE, OnCloseOfCtrl)
	ON_MESSAGE(WM_ALLACCNREMAIN, OnReceiveRemainData)
END_MESSAGE_MAP()

#ifdef	_FILE_DEBUG
	ofstream dFile("C:\\IBXXXX01.trc",ios::app);
#endif

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		{
			if (!HIWORD(wParam))
				break;
	
			CAccn *pAccn = NULL;
			int key = HIBYTE(LOWORD(wParam));
			// 20070824 �ű����� ü��� �����ɼ���ȸTR ������
			if (key == KEY_SCURR)	// ���� ���簡
			{
				CString sData = CString((char*)lParam, HIWORD(wParam));
				if (!m_AccnMap.Lookup(m_sCurrAccn, (void*&)pAccn))
					return -1;
					
				CRemain *pRemain = NULL;
				if (!pAccn->m_CodeMap.Lookup(m_sCurrKey, (void*&)pRemain))
					return -1;
				if (!pRemain)	return -1;
				
				pRemain->m_curr = parser(sData, "\t");
				pRemain->CalPgsonik("", "");
#ifdef	_FILE_DEBUG
//	dFile	<< "[notice insert]" << m_sCurrAccn  << "\tU\t" << pRemain->TotalData()<< endl;
	//TRACE("[notice insert]%s\n", CString(m_sCurrAccn + "\tU\t"+ pRemain->TotalData()));
#endif
				m_ShMemory->SendTrigger(CString(m_sCurrAccn + "\tI\t" + pRemain->TotalData()));

				return 0;
			}

			// �ܰ� or ü�᳻��(����)
			CString sErrCode = "", sErrText = "", sLedger, sData;
			long	ledgerLength = 0;
			char	nextKey;

/* @@Zeta	Ledger ����
			if (m_system == 'D')
			{
				struct _ledgerDaetooH ledger;
				CopyMemory(&ledger, (void*)lParam, L_ledgerDaetooH);
				sLedger = CString((char*)&ledger, L_ledgerDaetooH);
				sErrText = CString((char*)&ledger.msg, 130);
				sErrCode = CString((char*)&ledger.ecode, 6);
				nextKey = ledger.contf[0];
				ledgerLength = L_ledgerDaetooH;
			}
			else
			{
				struct _ledgerHanaH ledger;
				CopyMemory(&ledger, (void*)lParam, L_ledgerHanaH);
				sLedger = CString((char*)&ledger, L_ledgerHanaH);
				sErrText = CString((char*)&ledger.msg, 130);
				sErrCode = CString((char*)&ledger.ecode, 6);
				nextKey = ledger.contf[0]; 
				ledgerLength = L_ledgerHanaH;
			}
*/
			//CString pbdata; pbdata = (char*)lParam;
			//TRACE("PB DATA["+pbdata+"]\n");
			//if (HIWORD(wParam)<340) return 0;
			struct _ledgerH ledger;
			CopyMemory(&ledger, (void*)lParam, L_ledgerH);
			sLedger = CString((char*)&ledger, L_ledgerH);
			sErrCode = CString((char*)&ledger.emsg, 4);
			sErrText = CString((char*)&ledger.emsg, 98);
			nextKey = ledger.next[0];
			ledgerLength = L_ledgerH;

			lParam += ledgerLength;
	
			
			if (key == KEY_NCONT)	// ü�᳻��
			{
				if (sErrCode.GetAt(0) != _T('0'))
					return 0;
				parsingNContData((char*)lParam);
				if (nextKey == 'Y')
					sendNContTR(sLedger);
			}
			else// if (key == KEY_HREMAIN || key == KEY_FREMAIN)	// �ܰ�
			{
				//if (sErrCode == "    ") sErrText = "�ڽ��� TR ���� �Դϴ�. ����� �ٽ� �õ� �Ͻñ�ٶ��ϴ�.";
				sErrCode.TrimLeft(); sErrCode.TrimRight();
				//TRACE("sErrCode: ["+sErrCode+"]\n");
				if (sErrCode == "")
				{
					sErrCode = "9506";
					sData = "ERR\t";
				}
				else sData.Format("ERR\t%s", sErrText);
				
				if (sErrCode.GetAt(0) != _T('0'))	// ����
				{
					//if (sErrCode != "1511")
					{
						CString trkey, keyHandle; trkey.Format("%d",key);
						removeAccn(trkey);	// 20070824 memory leak �ذ�
						keyHandle = GetHandleByKey(trkey);
						
						detachCB(trkey, sData);
						//TRACE("trkey: "+trkey+"\n");						
						m_AccnKey.RemoveKey(keyHandle);
						//CString tmp; tmp.Format("m_AccnKey: %d\n", m_AccnKey.GetCount());
						//TRACE(tmp);
						
					}
					return 0;
					
				}
				//CString atmp; atmp.Format("Received key[%d]\n",key);
				//TRACE(atmp);				
				CString sAccn = parsingRemainData((char*)lParam, HIWORD(wParam), sData, key);
				sAccn.TrimLeft(); sAccn.TrimRight();
				if (sAccn!="")
				{
					CString trkey; trkey.Format("%d",key);
					sendToControl(trkey);
				}
			}
		}
		break;
	case DLL_ALERT:
		//m_sync.Lock();
		//parsingRTS((CString)(char*)lParam);
		//m_sync.Unlock();
		break;
	case DLL_ALERTx:
		m_sync.Lock();
		parsingRTSx((struct _alertR*)lParam);
		m_sync.Unlock();
	case DLL_SETPAL:
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		break;
	case DLL_NOTICE:
		{
//			TRACE("START - %x %x\n", m_hWnd, ::GetParent(m_hWnd));
			m_sync.Lock();

			parsingNotice((char*)lParam);
			m_sync.Unlock();
			SendLawNotice((char*)lParam);		
//			TRACE("END - %x %x\n", m_hWnd, ::GetParent(m_hWnd));
		}
		break;
	case DLL_GUIDE:
		return true;
		break;
	}

	return 0;
}

// �ܰ���ȸ
LRESULT CMapWnd::OnRequestOfCtrl(WPARAM wParam, LPARAM lParam)
{
	CString sTemp = (char*)lParam,
		sPswd, sAccn, sCredit;
	bool bFuture = (parser(sTemp, "\t")=="1" ? true : false);
	sAccn = parser(sTemp, "\t");
	sPswd = parser(sTemp, "\t");

	CString creS = parser(sTemp, "\t");
	bool bCredit = (atoi(creS) == 1 ? true : false);

	//buffet add
	CString sFee = parser(sTemp, "\t");
	int dFee = atoi(sFee);

	CString sMass = parser(sTemp, "\t");
	double dMass = atof(sMass);

	CString sSave = parser(sTemp, "\t");
	double dSave = atof(sSave);

	CString sCalcType = parser(sTemp, "\t");
	int dCalcType = atoi(sCalcType);
	//end buffet

	sendRemainTR(sAccn, sPswd, bFuture, bCredit, dFee, dMass, dSave, dCalcType, (HWND)wParam);

	return 0;

}

// ���ο����� ü����ȸ
LRESULT CMapWnd::OnRequestLawCheg(WPARAM wParam, LPARAM lParam)
{
	CString sTemp = (char*)lParam,
		sPswd, sAccn, sCredit;
	//AfxMessageBox("TEXT: "+sTemp);
	sendLawCheg("","","","", (HWND)wParam);


	return 0;

	//sendRemainTR(sAccn, sPswd, bFuture, bCredit, (HWND)wParam);
}

// �׷�����ΰ�� ����ϴµ� ���α׷� �� ���� �ִµ� ��
LRESULT CMapWnd::OnReceiveRemainData(WPARAM wParam, LPARAM lParam)
{
	CString sTemp = (char*)lParam,
		sAccn, keyS, creS, sData;
	bool bFuture = (parser(sTemp, "\t")=="1"?true:false);
	sAccn = parser(sTemp, "\t");
	keyS = parser(sTemp, "\t");
	creS = parser(sTemp, "\t");
	bool bCredit = (atoi(creS) == 1 ? true : false);
	sData = parser(sTemp, "\t");

//TRACE("�׷� sAccn[%s] keyS[%s] sData[%s]\n", sAccn, keyS, sData);

	CAccn *pAccn = NULL;
	if (!m_AccnMap.Lookup(sAccn, (void*&)pAccn))
	{
		pAccn = new CAccn;
		pAccn->m_bFuture = bFuture;
		pAccn->m_pswd = "";
		pAccn->m_bCredit = bCredit;
		m_AccnMap.SetAt(sAccn, pAccn);
	}

	if (!pAccn->m_pswd.IsEmpty())
		return 0;

	if (bFuture)	// ����
	{
		CRemainFuture *pRemain;
		if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
		{
			pRemain = new CRemainFuture(this);
			pRemain->m_system = m_system;
			
		}
		pRemain->ParsingAllaccnData(sData);
		pAccn->m_CodeMap.SetAt(keyS, pRemain);
	}
	else	// �ֽ�
	{
		CString codeS, sygbS, dateS, jggbS;

		jggbS = keyS.Left(LEN_JGGB);
		codeS = keyS.Mid(LEN_JGGB, LEN_ACODE);
		dateS = keyS.Mid(LEN_JGGB + LEN_ACODE, LEN_DATE);
		sygbS = keyS.Mid(LEN_JGGB + LEN_ACODE + LEN_DATE, LEN_SYGB);
		
		keyS = getKey(bFuture, codeS, dateS, sygbS, jggbS);
		bool b_etf = false;
		struct	hjcodex*	hjc;

		if (m_hname.Lookup(codeS.Mid(1,7), (void *&)hjc))
		{
				if (hjc->ssgb == jmETF)	// ETF
				{
					b_etf = true;
				}
		}

		CRemain *pRemain;
		if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
		{
			pRemain = new CRemain;
			pRemain->m_system = m_system;
			pRemain->m_betf   = b_etf;
		}
		pRemain->ParsingAllaccnData(sData);
		pAccn->m_CodeMap.SetAt(keyS, pRemain);
	}

	return 0;

}
#define DF_SHARED
//=================================================================================================
BOOL CMapWnd::CreateMap(CWnd *pParent)
{
#ifdef	_FILE_DEBUG
	dFile	<< ">>>>>>>>>>>>>>>>>�ǽð� �ܰ� ���� start>>>>>>>>>>>>>>>>" << endl << endl;
#endif
	BOOL bCreate = Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0, 0, MAP_WIDTH, MAP_HEIGHT), pParent, 100);
	LoadCode();
	CString path = variant(homeCC, "");
	
	CString systemS = variant(systemCC, "");
	if (systemS.IsEmpty())
		m_system = 'D';
	else
	{
		m_system = systemS.GetAt(0);
	}
	variant(titleCC, "Untitled");
	path.Replace("\\", "_");

	//m_ShMemory = new CShMemory;
	//m_ShMemory->remainSHMEMNAME = path + "_remainCtrl_API";
	//m_ShMemory->InitSharedMemory(this->m_hWnd);
	//m_ShMemory->AddHandle(this->m_hWnd);

#ifdef DF_SHARED
	DWORD processID = GetCurrentProcessId();
	CString sSHMENAME;
	sSHMENAME.Format("%s%s%d", path, "_remainCtrl_API", processID);
	m_ShMemory = new CShMemory;
	m_ShMemory->remainSHMEMNAME = sSHMENAME;
#else
	m_ShMemory = new CShMemory;
	m_ShMemory->remainSHMEMNAME = path + "_remainCtrl_API";
#endif
	m_ShMemory->InitSharedMemory(this->m_hWnd);
	m_ShMemory->AddHandle(this->m_hWnd);


#ifdef	_FILE_DEBUG
	dFile	<< ">>>>>>>>>>>>>>>>>�ǽð� �ܰ� ���� end >>>>>>>>>>>>>>>>" << endl << endl;
#endif
	return bCreate;
}


void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); 
	CRect clipRC;
	dc.GetClipBox(&clipRC);
	dc.FillSolidRect(clipRC, GetSysColor(COLOR_WINDOW));
}


void CMapWnd::OnDestroy() 
{
	m_arKey.RemoveAll();
	m_AccnKey.RemoveAll();
	m_multicb.RemoveAll();///buffet

	POSITION	accPos;		//codePos
	CString		sAccn;		// sKey

	for (accPos = m_AccnMap.GetStartPosition(); accPos != NULL;)
	{
		CAccn	*pAccn = NULL;
		m_AccnMap.GetNextAssoc(accPos, sAccn, (void*&)pAccn);
		removeRemain(pAccn);
		delete pAccn;	pAccn = NULL;
	}
	m_AccnMap.RemoveAll();

	m_ShMemory->Close();
	delete m_ShMemory;	m_ShMemory = NULL;

	CWnd::OnDestroy();
}

BOOL CMapWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

//=================================================================================================
void CMapWnd::sendRemainTR(CString sAccn, CString sPswd, bool bFuture, bool bCredit, int dFee, double dMass,double dSave, int dCalcType, HWND sHwnd)
{
	sPswd.TrimRight();
	//sAccn.TrimRight();
	/*
	if (sPswd.IsEmpty() && (sAccn != "           "))
	{
		AfxMessageBox("��й�ȣ�� �Է��ϼ���");
		return;
	}
	*/
	CAccn *pAccn = NULL;
	CString sData;
	int skey=-1;
	CString AccHandleKey, sHandle; sHandle.Format("%d",(int)sHwnd);
	if (m_AccnKey.Lookup(sHandle,AccHandleKey))
	{
		skey = atoi(AccHandleKey);
		
		if (!m_AccnMap.Lookup(AccHandleKey, (void*&)pAccn))	// 20070824
		{
			pAccn = new CAccn;
			pAccn->m_accn = sAccn;
			pAccn->m_bFuture = bFuture;
			pAccn->m_pswd = sPswd;
			pAccn->m_bCredit = bCredit;
			pAccn->m_dFee = dFee;//buffet
			pAccn->m_dMass = dMass;
			pAccn->m_dSave = dSave;
			pAccn->m_dCalcType = dCalcType;
			

			int kNum = GetNewAccHandleKey();
			AccHandleKey.Format("%d",kNum);
			m_AccnMap.SetAt(AccHandleKey, pAccn);
			//skey = kNum;
			//CString tmp_data;	tmp_data.Format("%d", m_hcnt);
			//m_AccnKey.SetAt(sHandle, tmp_data);
		}
		else
		{
			removeRemain(pAccn);
			pAccn->m_accn = sAccn;
			pAccn->m_pswd = sPswd;
			pAccn->m_dFee = dFee;//buffet
			pAccn->m_dMass = dMass;
			pAccn->m_dSave = dSave;
			pAccn->m_dCalcType = dCalcType;
		
		}
	}
	else
	{
		pAccn = new CAccn;
		pAccn->m_accn = sAccn;
		pAccn->m_bFuture = bFuture;
		pAccn->m_pswd = sPswd;
		pAccn->m_bCredit = bCredit;

		pAccn->m_dFee = dFee;//buffet
		pAccn->m_dMass = dMass;
		pAccn->m_dSave = dSave;
		pAccn->m_dCalcType = dCalcType;

		int kNum = GetNewAccHandleKey();
		AccHandleKey.Format("%d",kNum);
		m_AccnMap.SetAt(AccHandleKey, pAccn);
		m_AccnKey.SetAt(sHandle, AccHandleKey);
		skey = kNum;
	}
	attachCB((HWND)sHwnd, AccHandleKey);

	sData  = ledgerTR("2700", bFuture ? sv_FJGO : sv_SJGO, bFuture ? "2" : "1");
	m_sCurrAccn = sAccn;
	m_sCurrPswd = sPswd;

	CString passX;
	passX.Format("%s\t%s", sPswd, sAccn.Left(8));
	passX = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	sPswd = passX;
	CString text, tmps;
	
	text = "";
	tmps.Format("%d",(int)sHwnd);
		
	if (bFuture)
	{
		struct f_mid mid;
		
		FillMemory(&mid, L_fmid, ' ');
		CopyMemory(&mid.accn, sAccn, sizeof(mid.accn));
		CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
		CopyMemory(&mid.date, "", sizeof(mid.date));
		sData += CString((char*)&mid, L_fmid);	
		sendTR("PIBOFJGO", sData, skey, 0, sAccn);

	}
	else
	{
		struct s_mid mid;
		
		FillMemory(&mid, L_smid, ' ');
		CopyMemory(&mid.accn, sAccn, sizeof(mid.accn));
		CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
		
		if (bCredit)
			mid.allf[0] = '1';
		else
			mid.allf[0] = '0';

		sData += CString((char*)&mid, L_smid);
		sendTR("PIBOSJGO", sData, skey, 0, sAccn);
#ifdef	_FILE_DEBUG
	dFile	<< "�ܰ���ȸ =>" << sAccn << endl;
#endif
	}
}

CString CMapWnd::parsingRemainData(char* pData, int len, CString sErrmsg, int trkey)
{
	CString sAccn = CString((char*)pData, 11),
		sCode, sAllCode, keyS, sygbS, dateS, jggbS;
	int nrec = 0;
	int sizeL = 0;
	int checkL = 0;

	if (sAccn.IsEmpty())	return "";

	CAccn *pAccn = NULL;
	sAccn.Format("%d",trkey);
	
	//CString tmp; tmp.Format("trkey: %d, (%d)\n",trkey,m_AccnMap.GetCount());
	//TRACE(tmp);
	if (!m_AccnMap.Lookup(sAccn, (void*&)pAccn))	// 20070824
		return "";

	bool bFuture = pAccn->m_bFuture;
	pAccn->m_errmsg = sErrmsg;

	if (bFuture)
	{
		struct f_mod	fmod;
		CopyMemory(&fmod, pData, L_fmod);
		nrec = atoi(CString(fmod.nrec, sizeof(fmod.nrec)));
		pData += L_fmod;
		sizeL += L_fmod;	checkL = L_fmod + L_frec * nrec;
	}
	else
	{
		struct s_mod	smod;
		CopyMemory(&smod, pData, L_smod);
		nrec = atoi(CString(smod.nrec, sizeof(smod.nrec)));
		pData += L_smod;
		sizeL += L_smod;	checkL = L_smod + L_srec * nrec;
	}

#ifdef	_FILE_DEBUG
	dFile	<< "�ܰ��� : " << nrec << endl;
#endif
	m_arKey.RemoveAll();

	pAccn->m_CodeMap.RemoveAll();

	CString stmp;
	stmp.Format("[IBXXXX00] parsingRemainData nrec=[%d] \r\n", nrec);
	OutputDebugString(stmp);

	for (int ii = 0; ii < nrec; ii++)
	{
		if (bFuture)
		{
			sCode = CString(pData, 8);
			keyS = getKey(true, sCode, "", "", "", trkey);
			CRemainFuture *pRemain = NULL;
			if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
			{
				pRemain = new CRemainFuture(this);
				pRemain->m_system = m_system;

			}
			//TRACE("["+CString(pData)+"]\n");
			pRemain->ParsingRemainData(pData);

			pAccn->m_CodeMap.SetAt(keyS, pRemain);
			pData += L_frec;	sizeL += L_frec;
		}
		else
		{
			struct s_rec *srec = (struct s_rec *)pData;

			sCode = CString(srec->cod2, sizeof(srec->cod2));	// code
			sCode.TrimRight();
			dateS = CString(srec->sydt, sizeof(srec->sydt));	// ������
			dateS.TrimRight();
			sygbS = CString(srec->sycd, sizeof(srec->sycd));	// �ſ뱸��
			sygbS.TrimRight();
			jggbS = CString(srec->jggb, sizeof(srec->jggb));
			jggbS.TrimRight();
			keyS = getKey(false, sCode, dateS, sygbS, jggbS, trkey);
//TRACE("key[%s][%s]\n", keyS, CString(srec->hnam, sizeof(srec->hnam)));

			bool b_etf = false;
			struct	hjcodex*	hjc;
			
			if (m_hname.Lookup(sCode.Mid(1,7), (void *&)hjc))
			{
					if (hjc->ssgb == jmETF)	// ETF
					{
						b_etf = true;
					}
			}

			CRemain *pRemain = NULL;
			if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
			{
				pRemain = new CRemain;
				pRemain->m_system = m_system;
				pRemain->m_betf = b_etf;
			}
			m_arKey.Add(keyS);
			pRemain->m_dFee = pAccn->m_dFee;//buffet
			pRemain->m_dMass = pAccn->m_dMass;
			pRemain->m_dSave = pAccn->m_dSave;
			pRemain->m_dCalcType = pAccn->m_dCalcType;
			
			pRemain->ParsingRemainData(pData);
			pAccn->m_CodeMap.SetAt(keyS, pRemain);
#ifdef	_FILE_DEBUG
	dFile	<< "�ܰ� " << ii << " => " << keyS << endl;
#endif
			pData += L_srec;	sizeL += L_srec;
		}	
		if (sizeL > len || sizeL > checkL)
			break;
	}
//TRACE("len[%d] sizeL[%d] checkL[%d] \n", len, sizeL, checkL);
	if (bFuture)
		sendNContTR();
	return sAccn;
}

// �ܰ� ����Ÿ ��ȸ ����
void CMapWnd::sendToControl(CString sAccn)
{
	CAccn	*pAccn = NULL;
	if (!m_AccnMap.Lookup(sAccn, (void*&)pAccn))	// 20070824
		return;
	if (pAccn)	sendToControl(sAccn, pAccn);
}

void CMapWnd::sendToControl(CString sAccn, CAccn *pAccn)
{
	CString sData, sCode;
	POSITION pos;
	CString keyS;
	if (pAccn->m_bFuture)
	{
		for (pos = pAccn->m_CodeMap.GetStartPosition(); pos != NULL;)
		{
			CRemainFuture *pRemain = NULL;
			pAccn->m_CodeMap.GetNextAssoc(pos, keyS, (void*&)pRemain);
			if (pRemain)	sData += pRemain->TotalData() + "\n";
		}
	}
	else
	{
#ifdef	_FILE_DEBUG
		dFile	<< "*�ܰ��� ȭ������ ������->>>>> "<< endl;
#endif
		CRemain *pRemain = NULL;
		for (int ii = 0; ii < m_arKey.GetSize(); ii++)
		{
			keyS = m_arKey.GetAt(ii);
			if (pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
			{
				sData += pRemain->TotalData() + "\n";
//TRACE("key[%s]total[%.400s]\n",keyS, pRemain->TotalData());
#ifdef	_FILE_DEBUG
	//TRACE("key[%s]total[%.400s]\n",keyS, pRemain->TotalData());
	dFile	<< "�ܰ� " << keyS << " :: " << pRemain->TotalData() << endl;
#endif
			}
		}
#ifdef	_FILE_DEBUG
		dFile	<< "===================== "<< endl;
#endif
		/*
		for (pos = pAccn->m_CodeMap.GetStartPosition(); pos != NULL;)
		{
			CRemain *pRemain = NULL;
			pAccn->m_CodeMap.GetNextAssoc(pos, keyS, (void*&)pRemain);

			if (pRemain)
			{
				sData += pRemain->TotalData() + "\n";
TRACE("total[%.400s]\n",pRemain->TotalData());
			}

		}	
		*/
	}

	detachCB(sAccn, sData, pAccn->m_errmsg);
}

// �ܰ���ȸ��û��(��û�� HWND�� ������ ����)
void CMapWnd::attachCB(HWND hWnd, CString accn)
{
	CString	tmps, text;

	m_sync.Lock();
	BOOL ret = m_multicb.Lookup(accn, text);

	if ( ret == TRUE)
	{
		RemoveCB(accn);
	}

	tmps.Format("%ld\t", (int)hWnd);
//	text += tmps;
	text = tmps;//DOUBLED HWND BLOCK

	m_multicb.SetAt(accn, text);

	m_sync.Unlock();
}

// �ܰ���ȸ��(��û�� HWND�� ����)
void CMapWnd::detachCB(CString accn, CString sData, CString emsg)
{
	int	idx;
	HWND	hWnd;
	CString	tmps, text;

	m_sync.Lock();
	//TRACE("accn: "+accn+"\n");
	if (m_multicb.Lookup(accn, text))
	{
		for (;;)
		{
			idx = text.Find('\t');
			if (idx == -1)
				break;
			tmps = text.Left(idx++);
			text = text.Mid(idx);
			hWnd = (HWND)atol(tmps);
			// �����޼���
			if (!emsg.IsEmpty())	::SendMessage(hWnd, WM_REMAIN, 0, (LPARAM)(LPCTSTR)emsg);
			// �ܰ���Ÿ
			::SendMessage(hWnd, WM_REMAIN, 0, (LPARAM)(LPCTSTR)sData);
		}
//		m_multicb.RemoveKey(accn);//block buffet
	}
	/*
	if (!m_multicb.Lookup(accn, text))
	{
		//asdfasdf
		AfxMessageBox("1");
	}
	else
	{
		if (text == "") AfxMessageBox("2");
		else AfxMessageBox("text: "+text);
	}
	*/
	m_sync.Unlock();
}

void CMapWnd::sendNContTR(CString sLedger)
{
/* @@ Zeta  ����.. �� �ڵ�...
	CString sData = ledgerTR("2700", "", "1");
	if (!sLedger.IsEmpty())
		sData = sLedger;
*/
	struct _ledgerH ledger;
	
	if (sLedger.IsEmpty())
	{
		CopyMemory(&ledger, ledgerTR("2700", "", "1"), L_ledgerH);
	}
	else
	{
		CopyMemory(&ledger, (LPCTSTR)sLedger, L_ledgerH);
		CString strNext = CString((char*)&ledger.nkey, sizeof(ledger.nkey));
		CopyMemory(&ledger, ledgerTR("2700", "", "1"), L_ledgerH);
		CopyMemory(&ledger.nkey, (LPCTSTR)strNext, sizeof(ledger.nkey));
		ledger.fkey[0] = '7';
	}

	CString sData = CString((char*)&ledger, L_ledgerH);

	struct f_cmid cmid;
	
	FillMemory(&cmid, L_fcmid, ' ');
	CopyMemory(&cmid.accn, m_sCurrAccn, sizeof(cmid.accn));

	CString passX;
	passX.Format("%s\t%s", m_sCurrPswd, m_sCurrAccn.Left(8));
	passX = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	CopyMemory(&cmid.pswd, passX, passX.GetLength());

	CopyMemory(&cmid.gubn, "2", sizeof(cmid.gubn));
	CopyMemory(&cmid.sort, "1", sizeof(cmid.sort));
	sData += CString((char*)&cmid, L_fcmid);
	sendTR("PIHOFCHG", sData, KEY_NCONT, 0,cmid.accn);	
}

void CMapWnd::parsingNContData(char *pData)
{
	CString sCode, sGubn, sAmt, keyS;
	int nrec = 0;
	
	CAccn *pAccn = NULL;
	if (!m_AccnMap.Lookup(m_sCurrAccn, (void*&)pAccn))
		return;
	
	struct f_cmod	fmod;
	CopyMemory(&fmod, pData, L_fcmod);
	nrec = atoi(CString(fmod.nrec, sizeof(fmod.nrec)));
	pData += L_fcmod;	
	
	for (int ii = 0; ii < nrec; ii++)
	{
		struct f_crec frec;
		CopyMemory(&frec, pData, L_fcrec);
		sCode = CString(frec.cod2, sizeof(frec.cod2));
		keyS = getKey(true, sCode, "", "", "");
		CRemainFuture *pRemain;
		if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
		{
			pData += L_fcrec;
			continue;
		}
		
		sGubn = CString(frec.odgb, sizeof(frec.odgb));
		sAmt =  CString(frec.wqty, sizeof(frec.wqty));
		pRemain->SetNoCont(sGubn.Left(5), atoi(sAmt));
		
		pData += L_fcrec;
	}
}

void  CMapWnd::parsingRTSx(struct _alertR* alertR)
{
	DWORD* data{};
	//CString sCode = parser(sRtsData, "\t"), sSym, sData, sCurr, sDiff = "", sRate = "", sAccn;
	CString sCode = alertR->code, sSym, sData, sCurr, sDiff = "", sRate = "", sAccn;

	CString tot_str(_T(""));

	//while (true)
	//{
	//	if (sRtsData.Find("\n") != -1)
	//	{
	//		tot_str = parser(sRtsData, "\n");
	//		tot_str.TrimLeft(); tot_str.TrimRight();
	//		if (tot_str != "") sRtsData = tot_str;
	//		if (tot_str == "") break;
	//	}
	//	else break;

	//}


	/*
	if (sCode == "A000660")
	{
		/*
		if (sRtsData.Find("\n")!=-1)
		{
			sRtsData.TrimLeft(); sRtsData.TrimRight();
			//MessageBox("Next Line Included");
			TRACE("sRtsData: %s]\n", sRtsData);
		}


		//TRACE("sRtsData: 067250]\n");
		TRACE("sRtsData: %s]\n",sRtsData);
	}
	*/

	CMapStringToString symMap;
	POSITION AccnPos;

	if (sCode.GetLength() < 6)
		return;


	data = (DWORD*)alertR->ptr[0];

	if (!data) return;

	if (!data[23])
		return;

	sCurr = (char*)data[23];

	if (data[24]) {
		sDiff = (char*)data[24];
	}

	if (data[33]) {
		sRate = (char*)data[33];
	}

	if (sRate == "0.00") sRate = " ";

	//if (sCode == "A000660")  TRACE("sCode[%s] currS[%s] diff[%s]\n", sCode, sCurr, sDiff);
	/*
	//if (sCode == "A000100")
	{
		CWnd* dd = FindWindow(NULL, "test form");
		if(dd)
		{
			int isCurr, isDiff;
			CString tsCurr, tsDiff;
			tsCurr = sCurr; tsDiff = sCode;
			tsDiff.Delete(0,1);

			tsCurr.Remove(_T(','));

			dd->SendMessage(WM_USER,atoi(tsDiff),atoi(tsCurr));
		}
	}
	*/
	if (!m_AccnMap.GetCount())
		return;

	for (AccnPos = m_AccnMap.GetStartPosition(); AccnPos != NULL;)
	{
		CAccn* pAccn = NULL;
		m_AccnMap.GetNextAssoc(AccnPos, sAccn, (void*&)pAccn);
		if (pAccn->m_bFuture == (sCode.GetLength() == 8 ? false : true))
			continue;

		if (pAccn->m_bFuture)
		{
			CRemainFuture* pRemain;
			CString keyS = getKey(true, sCode, "", "", "");			// key
			if (pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
			{
				if (!pRemain->CalPgsonik(sCurr, sDiff, sRate))
					continue;

				//				TRACE("[TRIGGER@@]%s\n", (LPCTSTR)pRemain->TotalData());
				m_ShMemory->SendTrigger(pAccn->m_accn + "\tU\t" + pRemain->TotalData());
			}
		}
		else
		{
			CRemain* pRemain;
			CString keyS;
			CString keyCodeS;
			for (POSITION codePos = pAccn->m_CodeMap.GetStartPosition(); codePos != NULL;)
			{
				pAccn->m_CodeMap.GetNextAssoc(codePos, keyS, (void*&)pRemain);
				//				keyCodeS = keyS.Left(LEN_ACODE);
				keyCodeS = keyS.Mid(2, LEN_ACODE);
				keyCodeS.TrimRight();

				if (keyCodeS == sCode)
				{
					//TRACE("keyS[%s] codeS[%s] curr[%s]\n", keyS, keyCodeS, sCurr);
					if (!pRemain->CalPgsonik(sCurr, sRate))
					{
						//TRACE("keyS[%s] codeS[%s] curr[%s]\n", keyS, keyCodeS, sCurr);
						continue;
					}
#ifdef	_FILE_DEBUG
					//	dFile	<< "[sise update]" << sAccn  << "\tU\t" << pRemain->TotalData() << endl;
#endif
					HWND hwnd = GetRightHwnd(keyS);
					//TRACE(pAccn->m_accn  + "\tU\t" + pRemain->TotalData()+"\n");
					m_ShMemory->SendTrigger(pAccn->m_accn + "\tU\t" + pRemain->TotalData(), hwnd);
				}
				else
				{

				}
			}
		}
	}
}


void CMapWnd::parsingRTS(CString sRtsData)
{
	CString sCode = parser(sRtsData, "\t"), sSym, sData, sCurr, sDiff = "", sRate = "", sAccn;
	
	CString tot_str(_T(""));
	
	while(true)
	{
		if (sRtsData.Find("\n")!=-1)
		{
			tot_str = parser(sRtsData, "\n");
			tot_str.TrimLeft(); tot_str.TrimRight();
			if (tot_str != "") sRtsData = tot_str;
			if (tot_str == "") break;
		} else break;	

	}
	
			
	/*
	if (sCode == "A000660")
	{	
		/*
		if (sRtsData.Find("\n")!=-1)
		{
			sRtsData.TrimLeft(); sRtsData.TrimRight();
			//MessageBox("Next Line Included");
			TRACE("sRtsData: %s]\n", sRtsData);
		}
	
		
		//TRACE("sRtsData: 067250]\n");
		TRACE("sRtsData: %s]\n",sRtsData);
	}
	*/

	CMapStringToString symMap;
	POSITION AccnPos;

	if (sCode.GetLength() < 6)
		return;

	while (!sRtsData.IsEmpty())
	{
		sSym = parser(sRtsData, "\t");
		sData = parser(sRtsData, "\t");
		if (sData.IsEmpty())
			continue;
		symMap.SetAt(sSym, sData);
		//if (sSym == "024") break;
	}
	if (!symMap.Lookup("023", sCurr))
		return;
	symMap.Lookup("024", sDiff);
	symMap.Lookup("033", sRate);
	
	if (sRate=="0.00") sRate = " ";
	
	//if (sCode == "A000660")  TRACE("sCode[%s] currS[%s] diff[%s]\n", sCode, sCurr, sDiff);
	/*
	//if (sCode == "A000100")
	{
		CWnd* dd = FindWindow(NULL, "test form");
		if(dd)
		{
			int isCurr, isDiff;
			CString tsCurr, tsDiff;
			tsCurr = sCurr; tsDiff = sCode;
			tsDiff.Delete(0,1);

			tsCurr.Remove(_T(','));
			
			dd->SendMessage(WM_USER,atoi(tsDiff),atoi(tsCurr));
		}
	}
	*/
	if (!m_AccnMap.GetCount())
		return;

	for (AccnPos = m_AccnMap.GetStartPosition(); AccnPos != NULL;)
	{
		CAccn	*pAccn = NULL;
		m_AccnMap.GetNextAssoc(AccnPos, sAccn, (void*&)pAccn);
		if (pAccn->m_bFuture == (sCode.GetLength()==8 ? false : true))
			continue;

		if (pAccn->m_bFuture)
		{
			CRemainFuture *pRemain;
			CString keyS = getKey(true, sCode, "", "", "");			// key
			if (pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
			{
				if (!pRemain->CalPgsonik(sCurr, sDiff, sRate))
					continue;
				
//				TRACE("[TRIGGER@@]%s\n", (LPCTSTR)pRemain->TotalData());
				m_ShMemory->SendTrigger(pAccn->m_accn + "\tU\t" + pRemain->TotalData());
			}
		}
		else
		{
			CRemain *pRemain;
			CString keyS;
			CString keyCodeS;
			for (POSITION codePos = pAccn->m_CodeMap.GetStartPosition(); codePos != NULL;)
			{
				pAccn->m_CodeMap.GetNextAssoc(codePos, keyS, (void*&)pRemain);
//				keyCodeS = keyS.Left(LEN_ACODE);
				keyCodeS = keyS.Mid(2, LEN_ACODE);
				keyCodeS.TrimRight();
				
				if (keyCodeS == sCode)
				{
//TRACE("keyS[%s] codeS[%s] curr[%s]\n", keyS, keyCodeS, sCurr);
					if (!pRemain->CalPgsonik(sCurr, sRate))
					{
						//TRACE("keyS[%s] codeS[%s] curr[%s]\n", keyS, keyCodeS, sCurr);
						continue;
					}
#ifdef	_FILE_DEBUG
//	dFile	<< "[sise update]" << sAccn  << "\tU\t" << pRemain->TotalData() << endl;
#endif
					HWND hwnd = GetRightHwnd(keyS);
					//TRACE(pAccn->m_accn  + "\tU\t" + pRemain->TotalData()+"\n");
					m_ShMemory->SendTrigger(pAccn->m_accn  + "\tU\t" + pRemain->TotalData(),hwnd);
				}
				else
				{
					
				}
			}
		}
	}
}

HWND CMapWnd::GetRightHwnd(CString key)
{
	CString sCode,text,tmps;
	HWND	hWnd;
	int	idx;
	int i = key.Find("|");

	sCode = key.Mid(i+1);	

	m_sync.Lock();

	if (m_multicb.Lookup(sCode, text))
	{
		for (;;)
		{
			idx = text.Find('\t');
			if (idx == -1)
				break;
			tmps = text.Left(idx++);
			text = text.Mid(idx);
			hWnd = (HWND)atol(tmps);
			//TRACE("__FINDING__[%ld][%s]\n",hWnd,sCode);
		}
	}
	m_sync.Unlock();

	return hWnd;
}

CString CMapWnd::parsingNotice(CString str)
{ 
//TRACE("notice[%.400s]\n", str);
	CString s;
	s.Format("JANGO RD : [%s]\n",str);
	OutputDebugString(s);
	CString dataS = parser(str, "\n"), sAcc = "", sTemp = "", sOrder = "", sJCode = "", sDate = "", keyS = "", 
		sSygb = "", sSymt = "",	sSendData = "", sJGType = "";
	bool	bNewCode = false;

	while(!dataS.IsEmpty())
	{
		CString sKey = parser(dataS, "\t");
		if (atoi(sKey) == 901)
		{				//	���¹�ȣ
			sAcc = parser(dataS, "\r");
			if (sAcc.GetLength() > 11)
			{
				sTemp = sAcc;
				sAcc = parser(sTemp, "\t");
				parser(sTemp, "\t");	// 902�� ����
			}
			break;
		}
	}
	
	dataS = str;
	
	CMapStringToString	mapNotice;
	mapNotice.RemoveAll();

	CString sSym, sDat, sJggb;
	while (!dataS.IsEmpty())
	{
		sSym = parser(dataS, "\t"); 
		sDat = parser(dataS, "\t");
		sDat.TrimLeft();	sDat.TrimRight();
		mapNotice.SetAt(sSym, sDat);
	}

	mapNotice.Lookup(RT_JCODE, sJCode);		// �����ڵ�
	mapNotice.Lookup(RT_CTDATE, sDate);		// �ſ������
	mapNotice.Lookup(RT_CREDIT, sSygb);		// �ſ뱸��
	mapNotice.Lookup(RT_JGTYPE, sJggb);

	sSygb.Format("%02d", atoi(sSygb));
	sJggb.Format("%02d", atoi(sJggb));
	mapNotice.SetAt(RT_CREDIT, sSygb);
	mapNotice.SetAt(RT_JGTYPE, sJggb);

	mapNotice.Lookup(RT_CTSYMT, sSymt);		// �ſ����ݾ�
//TRACE("�����ڵ�[%s] �ſ볯¥[%s] �ſ뱸��[%s] �����[%s]\n", sJCode, sDate, sSygb, sSymt);

	bool m_bSpread = false;
	if ((sJCode.GetLength()==8) && (sJCode.Mid(0,1)=="4"))	m_bSpread = true;
	
	if ((sJCode[0] == 'J')&&(sJCode.GetLength() == 9)) //�����μ����ΰ�� 
	{
		mapNotice.RemoveAll();
		return "";
	}
	if (sAcc.IsEmpty() || sJCode.IsEmpty())
	{
		OutputDebugString("EMPTY ACCOUNT\n");
		mapNotice.RemoveAll();
		return "";
	}
	//asdfasdf
	CAccn *pAccn = NULL;
	bool bFind = false;
	POSITION	accPos;		//codePos
	CString sTRKey;

	s.Format("JANGO COUNT : [%d]\n",m_AccnMap.GetCount());
	OutputDebugString(s);

	for (accPos = m_AccnMap.GetStartPosition(); accPos != NULL;)
	{
		m_AccnMap.GetNextAssoc(accPos, sTRKey, (void*&)pAccn);
		//TRACE("pAccn->m_accn: [%s], sAcc: [%s]", pAccn->m_accn, sAcc);
		s.Format("pAccn->m_accn: [%s], sAcc: [%s]", pAccn->m_accn, sAcc);
		OutputDebugString(s);
		if (pAccn->m_accn == sAcc)
		{
			//AfxMessageBox("bFind=true");
			bFind = true;
			break;
		}
	}
	if (!bFind)
	{
		OutputDebugString("CANNOT FIND ACCOUNT\n");
		mapNotice.RemoveAll();
		return "";
	}

	OutputDebugString("JANGO RD PROCESSING.\n");

	if (pAccn->m_bFuture)	// ����
	{
		CRemainFuture *pRemain;
		int type = 0;
		if (m_bSpread)
		{
			m_ShMemory->SendTrigger(sAcc + "\tU\t" + sJCode+"\t ");
		}

		for (accPos = m_AccnMap.GetStartPosition(); accPos != NULL;)
		{
			m_AccnMap.GetNextAssoc(accPos, sTRKey, (void*&)pAccn);
			if (pAccn->m_accn==sAcc)
			{
 				keyS = getKey(true, sJCode, "", "", "", atoi(sTRKey));		// key

				if (!pAccn->m_bCredit)	// �ſ� ��ȸ�� �ƴѰ��
				{
					if (atoi(sSygb) > 0 && atoi(sSygb) < 10)
					{
						mapNotice.RemoveAll();
					}
				}
				type = 0;
				bNewCode = false;
				if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
				{
					pRemain = new CRemainFuture(this);
					pRemain->m_system = m_system;
					bNewCode = true;

				}
				
				type = pRemain->CalRemainData(mapNotice, keyS, bNewCode);
				
				if ((type == 3) || (type==1)) //insert or update
				{
					pAccn->m_CodeMap.SetAt(keyS, pRemain);
				}
				else if (type == 2) //delete
				{
					if (pRemain)
					{
						//�����߰�
						pRemain->DestroyWindow();
						delete pRemain;	pRemain = NULL;
					}
					pAccn->m_CodeMap.RemoveKey(keyS);
				}

				if (bNewCode && type == 0 && pRemain)
				{
					pRemain->DestroyWindow();
					delete pRemain;	pRemain = NULL;
				}
			}
		}
		
		//1: update, 2: delete, 3: OOP Send(insert)
		if (type == 1)
		{


			m_ShMemory->SendTrigger(sAcc + "\tU\t" + pRemain->TotalData());
		}
		else if (type == 2)
		{

			m_ShMemory->SendTrigger(sAcc + "\tD\t" + keyS);
		}
		else if (type == 3)
		{

			CString sData, sSym = "4";
			if (sJCode.GetAt(0) == '1')
			{
				if (sJCode.GetAt(1) == '0')
					sSym = "3";
				else
					sSym = "5";
			}
			
			// 20070823 ����
			CString currS = "", diffS = "";;
			
			mapNotice.Lookup(RT_CCURR, currS);
			mapNotice.Lookup(RT_CDIFF, diffS);
			
			if (atof(currS) == 0.0)	// ���簡 ������ ü�ᰡ��
			{
				mapNotice.Lookup(RT_CPRICE, currS);
				currS.Format("%.2f", atof(currS)/(float)100.0);
			}
			
			pRemain->m_curr = currS;
			pRemain->m_diff = diffS;
			pRemain->CalPgsonik();


			m_ShMemory->SendTrigger(sAcc + "\tI\t" + pRemain->TotalData());
		}
	}
	else	// �ֽ�
	{
		CRemain *pRemain;
		int type = 0;
		for (accPos = m_AccnMap.GetStartPosition(); accPos != NULL;)
		{
			m_AccnMap.GetNextAssoc(accPos, sTRKey, (void*&)pAccn);
			if (pAccn->m_accn==sAcc)
			{
//				TRACE("pAccn->m_accn: %s, sAcc: %s\n",pAccn->m_accn,sAcc);
				//AfxMessageBox("sSygb: "+sSygb+", sJggb: "+sJggb);
				keyS = getKey(false, sJCode, sDate, sSygb, sJggb, atoi(sTRKey));		// key
				//AfxMessageBox("1["+keyS+"]\n");
				if (!pAccn->m_bCredit)	// �ſ� ��ȸ�� �ƴѰ��
				{
					if (atoi(sSygb) > 0 && atoi(sSygb) < 10)
					{
						mapNotice.RemoveAll();
					}
				}
			

				mapNotice.Lookup(RT_JGTYPE, sJGType);	// �ܰ���
				type = 0;
				bNewCode = false;
				if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
				{
					bool b_etf = false;
					struct	hjcodex*	hjc;

					if (m_hname.Lookup(sJCode.Mid(1,7), (void *&)hjc))
					{
							if (hjc->ssgb == jmETF)	// ETF
							{
								b_etf = true;
							}
					}
					pRemain = new CRemain;
					pRemain->m_system = m_system;
					pRemain->m_betf = b_etf;

					pRemain->FillString(sDate, LEN_DATE);
					pRemain->FillString(sSygb, LEN_SYGB);
					pRemain->FillString(sSymt, LEN_SYMT);
					pRemain->FillString(sJGType, LEN_JGGB);
					pRemain->m_date = sDate;
					pRemain->m_creditS = sSygb;
		//@@ Zeta  �ſ���� �ּ�ó�� �Ǿ� �ִ��� ���� ��.
					pRemain->m_symt = sSymt;

					///buffet
					pRemain->m_dFee = pAccn->m_dFee;
					pRemain->m_dMass = pAccn->m_dMass;
					pRemain->m_dSave = pAccn->m_dSave;
					pRemain->m_dCalcType = pAccn->m_dCalcType;

					//buffet
					bNewCode = true;
				}
				type = pRemain->CalRemainData(mapNotice, keyS, bNewCode);
				if ((type == 3) || (type==1)) //insert or update
				{
					pAccn->m_CodeMap.SetAt(keyS, pRemain);
				}else if (type == 2) //delete
				{
					if (pRemain)
					{
						//�����߰�
						pRemain->DestroyWindow();
						delete pRemain;	pRemain = NULL;
					}
					pAccn->m_CodeMap.RemoveKey(keyS);
				}

				if (type==1)
				{
					m_ShMemory->SendTrigger(sAcc + "\tU\t" + pRemain->TotalData(), GetRightHwnd(keyS));
				}
				else if (type==2)
				{
					m_ShMemory->SendTrigger(sAcc + "\tD\t"+ keyS, GetRightHwnd(keyS));
				}
				else if (type==3)
				{
					CString currS = "";
					CString rateS = "";
					mapNotice.Lookup(RT_CCURR, currS);
					
					if (atof(currS) == 0.0)	// ���簡 ������ ü�ᰡ��
					{
						mapNotice.Lookup(RT_CPRICE, currS);
						currS.Format("%.f", atof(currS)/(float)100.0);
					}
					
					pRemain->m_curr = currS;
					
					mapNotice.Lookup(RT_CRATE, rateS);
					pRemain->m_srate = rateS;
					pRemain->CalPgsonik("", "", TRUE);
					m_ShMemory->SendTrigger(sAcc + "\tI\t" + pRemain->TotalData(), GetRightHwnd(keyS));
				}
				
				if (bNewCode && type == 0 && pRemain)
				{
					//�����߰�
					pRemain->DestroyWindow();
					delete pRemain;	pRemain = NULL;
				}
			}
		}
	}

	OutputDebugString("JANGO RD END\n");
	mapNotice.RemoveAll();
	return "";
}

// 2006.10.30 ���Դܰ� ������ ������ �߰�
BOOL CMapWnd::getRemainFee()
{
	CString root = variant(homeCC, "");
	CString	name = variant(nameCC, "");
	CString userPath;
	userPath.Format("%s\\user\\%s\\%s", root, name, "ORDCFG.INI");
	BOOL bFee = FALSE;
	bFee = (BOOL)GetPrivateProfileInt("Common", "NoFee", 0, userPath);
	return bFee;
}

CString	CMapWnd::getKey(bool bFuture, CString codeS, CString dateS, CString sygbS, CString sJggb, int trkey)
{
	if (bFuture)	return codeS;
	/*
	if (atoi(sygbS) >= 10 || atoi(sygbS) == 0)	// �ſ뱸�� �ƴϸ� 0
		sygbS = "  ";
	*/
	CString keyS;
	keyS.Format("%-*s%-*s%-*s%-*s|%d", LEN_JGGB, sJggb, LEN_ACODE, codeS, LEN_DATE, dateS, LEN_SYGB, sygbS, trkey);
	
	return keyS;
}

//=================================================================================================
CString CMapWnd::variant(int comm, CString data)
{
	return VariantA(m_pParent, comm, data);
}

int CMapWnd::openView(int type, CString data)
{
	return openViewA(m_pParent, type, data);
}

CString CMapWnd::parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

CString CMapWnd::ledgerTR(CString sTran, CString sSvcn, CString sGubn)
{
	CString strUser;
	struct _ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');

	strUser = variant(userCC, "");

	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(&ledger.svcd, sSvcn, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, m_sCurrAccn.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = sGubn.GetAt(0);
	ledger.odrf[0] = '1';

	return CString((char*)&ledger, L_ledgerH);
}

BOOL CMapWnd::sendTR(CString sTR, CString sData, int nKey, int nStat, CString Acnt)
{
	CString sTRData = "";
	CString sKey = "";
	int ikey = -1;
	struct _userTH user;
	strncpy(&user.trc[0], sTR, 8);

	/*
	m_sync.Lock();
	if (m_AccnKey.Lookup(Acnt, sKey))
	{	
		CString tmp;
		tmp.Format("%d",nKey);
		ikey = atoi(sKey);
		TRACE("Key: "+sKey+"<=>"+tmp+"\n");
	}
	m_sync.Unlock();
	*/
	/*
	CString tmp;
	tmp.Format("%d",nKey);
	TRACE("Key: "+tmp+"\n");
	*/
	user.key  = nKey;//ikey;

	user.stat = nStat;
	
	sTRData  = CString((char*)&user, sizeof(_userTH));
	sTRData += sData;
	variant(guideCC, "IBxxxx02 invokeTR");
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, sData.GetLength()), 
		(LPARAM)(const char*)sTRData); 
}


CString CMapWnd::getTime()
{
	CTime curr = CTime::GetCurrentTime();
	CString timeS;
	timeS.Format("[%02d/%02d %02d:%02d:%02d]", curr.GetMonth(), curr.GetDay(), curr.GetHour(), curr.GetMinute(), curr.GetSecond());
	return timeS;
}

void CMapWnd::removeAccn(CString accn)
{
	CAccn *pAccn = NULL;
	if (m_AccnMap.Lookup(accn, (void*&)pAccn))
	{
		if (pAccn)
		{
			removeRemain(pAccn);
			delete pAccn;	pAccn = NULL;
		}
	}
	m_AccnMap.RemoveKey(accn);
}

void CMapWnd::removeRemain(CAccn *pAccn)
{
	if (!pAccn)	return;

	POSITION codePos;
	CString	sKey;
	for (codePos = pAccn->m_CodeMap.GetStartPosition(); codePos != NULL;)
	{
/* @@ Zeta �Ƴ�.. �� �̷���..
		if (pAccn->m_bFuture)
		{
			CRemainFuture *pRemain = NULL;
			pAccn->m_CodeMap.GetNextAssoc(codePos, sKey, (void*&)pRemain);
			if (pRemain)
			{
				pRemain->DestroyWindow();
				delete pRemain;	pRemain = NULL;
			}
		}
		else
		{
			CRemain *pRemain = NULL;
			pAccn->m_CodeMap.GetNextAssoc(codePos, sKey, (void*&)pRemain);
			if (pRemain)
			{
				pRemain->DestroyWindow();
				delete pRemain;	pRemain = NULL;
			}
		}
*/

		CWnd* pRemain = NULL;
		pAccn->m_CodeMap.GetNextAssoc(codePos, sKey, (void*&)pRemain);
		
		if (pRemain)
		{
			//TRACE("sKey: "+sKey+"\n");
			pRemain->DestroyWindow();
			delete pRemain;
			pRemain = NULL;
		}
	}

	pAccn->m_CodeMap.RemoveAll();
}

LRESULT CMapWnd::OnCloseOfCtrl(WPARAM wParam, LPARAM lParam)
{
	//return;
	CString accn = (char*) lParam;
	HWND shwnd = (HWND) wParam;
	CString text = "";

	CString tmp;
	tmp.Format("%d",(int)shwnd);
	
	if (m_AccnKey.Lookup(tmp,text))
	{
		CAccn *pAccn = NULL;
		
		if (m_AccnMap.Lookup(text, (void*&)pAccn))	// 20070824
		{
			if (pAccn)
			{
				removeRemain(pAccn);
				delete pAccn;
			}
			m_AccnMap.RemoveKey(text);
		}
		
		//�ǽð� �ܰ� key������ �ʱ�ȭ��
		//TRACE("������ Ű��: "+text+"\n");
		m_AccnKey.RemoveKey(tmp);
		
		bool bFuture = (accn.Mid(3,1) == "2");
		sendRemainTR("           ","        ",bFuture,true,0,0,0,0,shwnd);//buffet
		
		/*
		CString sData, sCode = "A005930";

		if (sCode.Left(1) == "A")+
			sCode = sCode.Mid(1, 6);
		
		sData.Format("1301%c%s\t2023\t", 0x7f, sCode);
		sendTR("POOPPOOP", sData, atoi(text), 0, US_OOP);
		*/
		
		
	}
	else
	{
		if (m_LawKey.Lookup(tmp, text))	// 20070824
		{
			m_LawKey.RemoveKey(tmp);
		}
		CString stext;
		
	}

	
	return 0;

}

int CMapWnd::GetNewAccHandleKey()
{
	int itmp = 1;
	POSITION	pos;		//keyPos
	CString tmp_key, tmp_val, tmp_data;
	bool bfind = false;
	while(true)
	{
		bfind = false;
		for (pos = m_AccnKey.GetStartPosition(); pos != NULL;)
		{
			m_AccnKey.GetNextAssoc(pos,tmp_key,tmp_val);
			tmp_data.Format("%d",itmp);
			if(tmp_val == tmp_data)
			{
				bfind = true;
				break;
			}	
		}
		if (bfind) itmp++;
		else
		{
			m_hcnt = itmp;
			break;
		}	
	}
	//m_hcnt++;
	if (m_hcnt>247)
	{
		MessageBox("�Ҵ簡���� �ǽð� �ܰ� ���� �ʰ��Ͽ����ϴ�.\n �������Ͻø� �ǽð� �ܰ��Ҵ��� �ʱ�ȭ �Ҽ� �ֽ��ϴ�.","IBK��������");
		return -1;
	}	
	m_hcnt;
	return m_hcnt;
}

int CMapWnd::GetNewLawHandleKey()
{
	int itmp = 1;
	POSITION	pos;		//keyPos
	CString tmp_key, tmp_val, tmp_data;
	bool bfind = false;
	while(true)
	{
		bfind = false;
		for (pos = m_LawKey.GetStartPosition(); pos != NULL;)
		{
			m_LawKey.GetNextAssoc(pos,tmp_key,tmp_val);
			tmp_data.Format("%d",itmp);
			if(tmp_val == tmp_data)
			{
				bfind = true;
				break;
			}	
		}
		if (bfind) itmp++;
		else
		{
			m_hcnt = itmp;
			break;
		}	
	}
	//m_hcnt++;
	if (m_hcnt>247)
	{
		MessageBox("�Ҵ簡���� �ǽð� �ܰ� ���� �ʰ��Ͽ����ϴ�.\n �������Ͻø� �ǽð� �ܰ��Ҵ��� �ʱ�ȭ �Ҽ� �ֽ��ϴ�.","IBK��������");
		return -1;
	}	
	m_hcnt;
	return m_hcnt;
}

CString CMapWnd::GetHandleByKey(CString key)
{
	int	idx;
	CString	tmps, text;
	tmps = "-1";
	m_sync.Lock();
	if (m_multicb.Lookup(key, text))
	{
		
			idx = text.Find('\t');
			tmps = text.Left(idx++); //�ڵ�
			text = text.Mid(idx);
	}
	m_sync.Unlock();
	return tmps;
}

void CMapWnd::sendLawCheg(CString acc_gb, CString mng_no, CString mk_cd, CString grp_no, HWND sHwnd)
{
	CAccn *pAccn = NULL;
	CString sData;
	int skey=-1;
	CString AccHandleKey, sHandle; sHandle.Format("%d",(int)sHwnd);
	
	if (m_LawKey.Lookup(sHandle,AccHandleKey))
	{
		skey = atoi(AccHandleKey);
	}
	else
	{
		int kNum = GetNewLawHandleKey();
		AccHandleKey.Format("%d",kNum);
		m_LawKey.SetAt(sHandle, AccHandleKey);
		skey = kNum;
	}
	//TRACE("KEY: %d\n",skey);
	//CString asdf; asdf.Format("KEY: %d", skey);


}

void CMapWnd::SendLawNotice(CString sData)
{
	//TRACE("SendLawNotice: "+sData+"\n");
	CString dataS = parser(sData, "\n"), sAcc = "", sTemp = "", sOrder = "", sJCode = "", sDate = "", keyS = "", sOrdNo = "", sOrdGb = "",
		sSygb = "", sSymt = "",	sSendData = "", sJGType = "", sCprice = "", sCamt = "", sOrdQty = "", sJName = "", sJGB = "", sAccName = "", sMCamt = "", sState = "", sJtype = "";

	CString full_data;
	bool	bNewCode = false;
	
	while(!dataS.IsEmpty())
	{
		CString sKey = parser(dataS, "\t");
		if (atoi(sKey) == 901)
		{	//	���¹�ȣ
			sAcc = parser(dataS, "\r");
			if (sAcc.GetLength() > 11)
			{
				sTemp = sAcc;
				sAcc = parser(sTemp, "\t");
				parser(sTemp, "\t");	// 902�� ����
			}
			break;
		}
	}
	/*
	if (sAcc == "00110000449") 
	{
		TRACE("%s\n",sData);
		//TRACE("sState: %s\n",sState);
	}
	*/
	dataS = sData;
	
	CMapStringToString	mapNotice;
	mapNotice.RemoveAll();

	CString sSym, sDat, sJggb;
	while (!dataS.IsEmpty())
	{
		sSym = parser(dataS, "\t"); 
		sDat = parser(dataS, "\t");
		sDat.TrimLeft();	sDat.TrimRight();
		mapNotice.SetAt(sSym, sDat);
	}
	mapNotice.Lookup(RT_ANAME , sAccName);		// ���¸�
	mapNotice.Lookup(RT_JCODE , sJCode );		// �����ڵ�
	mapNotice.Lookup(RT_JNAME , sJName );		// �����
	mapNotice.Lookup(RT_GUBN  , sJGB   );		// ����
	mapNotice.Lookup(RT_ORDQTY , sOrdQty);		// �ֹ�����
	mapNotice.Lookup(RT_CAMT  , sCamt  );		// ü�����
	mapNotice.Lookup(RT_MCAMT , sMCamt );		// ��ü�����
	mapNotice.Lookup(RT_CPRICE, sCprice);		// ü��ܰ�
	mapNotice.Lookup(RT_JOTYPE, sState);		// �ֹ�����
	mapNotice.Lookup(RT_ORDNO , sOrdNo);		// �ֹ���ȣ
	mapNotice.Lookup(RT_ORDGB , sOrdGb);		// �ֹ�����
	mapNotice.Lookup(RT_JTYPE , sJtype);		// �������


	mapNotice.Lookup(RT_CREDIT, sSygb  );		// �ſ뱸��
	mapNotice.Lookup(RT_JGTYPE, sJggb  );		// �ܰ��ڵ�
	if (sJCode.GetLength()==8) return; //�����ɼ� �����ڵ��϶� ���͸��ϱ����� ó��
	if (sState=="1") sCamt = "0";  //�����϶� ü������� �ֹ������� �����ͼ� "0"���� �ʱ�ȭ��Ŵ
	if (sState=="3") return; //�������
	if (sState=="2") return; //��������
	if (sState=="12") return; //����Ȯ��
	if ((sState=="14")||(sState=="15")||(sState=="16")) return; //�ź�
	
	if (sState=="13") //���Ȯ��
	{
		sOrdQty.Format("%d",atoi(sOrdQty)*-1);
			
	}
	if (sJGB == "�ż�")
	{
		if (sJtype == "KSE")
		{
			sJGB = "2\t02\t(E)���ݸż�";
		}
		else if (sJtype == "KODAQ")
		{
			sJGB = "2\t02\t(Q)���ݸż�";
		}
		else
			sJGB = "2\t02\t���ݸż�";
		
	}
	else
	{
		if (sJtype == "KSE")
		{
			sJGB = "1\t01\t(E)���ݸŵ�";
		}
		else if (sJtype == "KODAQ")
		{
			sJGB = "1\t01\t(Q)���ݸŵ�";
		}
		else
			sJGB = "1\t01\t���ݸŵ�";
		
	}
	//mapNotice.Lookup(RT_CTSYMT, sSymt);		// �ſ����ݾ�
	//TRACE("���¹�ȣ[%s] ���¸�[%s] �����ڵ�[%s] �����[%s] �ֹ�����[%s] �ֹ�����[%s] ü�����[%s] ü��ܰ�[%s] �ܰ��ڵ�[%s]\n", sAcc, sAccName, sJCode, sJName, sJGB, sOrdQty, sCamt, sMCamt, sCprice, sJggb);

	full_data.Format("|%11s%-12s%c%11s\t%-40s\t%7s\t%-40s\t%-40s\t+\t%15s\t+\t%15s\t+\t+\t%9s\t.00",sAcc,sJCode,sJGB[0],sAcc, sAccName, sJCode, sJName, sJGB, sOrdQty, sCamt, sCprice);

	//���¹�ȣ(11) �����ڵ�(7) �Ÿű���(1) �ֹ�����(16) ü�����(16) ��ü�����(16) ü��ܰ�(16) ���¸�(40)

	sData.Format("%11s%-12s%c%16s%16s%16s%16s%-40s",sAcc,sJCode,sJGB[0], sOrdQty, sCamt, sMCamt, sCprice, sAccName);
	//TRACE(sData+"\n");	
	//AfxMessageBox(sData+full_data);
	

	if (sAcc.IsEmpty() || sJCode.IsEmpty())
	{
		mapNotice.RemoveAll();
		return;
	}
	sAcc.Insert(3,"-");
	sAcc.Insert(6,"-");
	CString sMsg;
	if ((sState == "1")) 
	{
	}
	if (sState.GetLength()==1)  //�ֹ�����
	{
		if (sJGB[0]!='1')
		{
			sMsg.Format("#[��    ��] %s %s(%s) %s(%s) %s %s��", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sOrdQty);
		}
		else
		{
			sMsg.Format("#[��    ��] %s %s(%s) %s(%s) %s %s��", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sOrdQty);
		}
	}else //�ֹ�ü��
	{
		if (sJGB[0]!='1')
		{
			if (sState=="13")
			{
				sMsg.Format("#[�ż����] %s %s(%s) %s(%s) %s %s��", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sCamt);
			}
			else
			{
				sMsg.Format("#[�ż�ü��] %s %s(%s) %s(%s) %s %s��", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sCamt);
			}
		}
		else
		{
			if (sState=="13")
			{
				sMsg.Format("#[�ŵ����] %s %s(%s) %s(%s) %s %s��", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sCamt);
			}
			else
			{
				sMsg.Format("#[�ŵ�ü��] %s %s(%s) %s(%s) %s %s��", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sCamt);
			}
		}
	}

	sData += full_data;
	sData += sMsg;


	// ������ ����
	CString tmp_key, tmp_val, tmp_data;
	POSITION	pos;		//keyPos
	HWND m_sendhwnd;
	for (pos = m_LawKey.GetStartPosition(); pos != NULL;)
	{
		m_LawKey.GetNextAssoc(pos,tmp_key,tmp_val);
		m_sendhwnd = (HWND) atoi(tmp_key);
		::SendMessage(m_sendhwnd, WM_USER, MAKEWPARAM(MAKEWORD(DLL_TRIGGER, -1), false), (LPARAM)(LPCTSTR)sData);
	}
}

void CMapWnd::RemoveCB(CString accn)
{
	CString	text;

	m_sync.Lock();

	if (m_multicb.Lookup(accn, text))
	{
		m_multicb.RemoveKey(accn);//buffet block
	}

	m_sync.Unlock();
}

BOOL CMapWnd::LoadCode()
{
	if (m_hcode.GetSize() > 0) 
		return TRUE;
	CString root = variant(homeCC, "");

	CString	string; 
	CFile	file;
	int	pos, size; 

	string.Format("%s/%s/%s", root, TABDIR, HJCODEx);
	if (!file.Open(string, CFile::modeRead|CFile::typeBinary))
		return FALSE;

	size = (int)file.GetLength();
	if (m_hcB)
		delete[] m_hcB;
	 
	m_hcB  = new char[size];
	file.Read(m_hcB, size);
	file.Close();

	size /= sizeof(struct hjcodex);
	if (size <= 0)
	{
		delete[] m_hcB;
		m_hcB = NULL;
		return FALSE;
	}

	struct	hjcodex*	hjc = (struct hjcodex *)m_hcB;
	for (int ii = 0; ii < size; ii++, hjc++)
	{
		m_hcode.Add(hjc);
		string = CString(hjc->code, HCodeLen);
		if (string.GetAt(0) == 'A')
			string = string.Mid(1);
		pos = string.Find('\0');
		if (pos != -1)
			string = string.Left(pos);
		string.TrimRight();
		m_hname.SetAt(string, hjc);
	}

	return TRUE;
}

CString CMapWnd::GetRoot() 
{ 
	return variant(homeCC, ""); 
}

CString CMapWnd::GetUser() 
{ 
	return variant(userCC, "");
}

CString CMapWnd::GetName() 
{ 
	return variant(nameCC, ""); 
}

void CMapWnd::testSaveFile(CString data)
{
	CString strPath(_T("")), m_root, m_user, titletime;
	strPath.Format("%s/jangoTest.dat", GetRoot());
	
	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 

	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());	

	titletime.Format("[%s]%s\n", strCurTime, data);
	
	file.WriteString(titletime);
	file.Close();
}
