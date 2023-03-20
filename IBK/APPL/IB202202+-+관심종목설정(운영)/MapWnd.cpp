// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "MapWnd.h"
#include "ioformat.h"
#include <sys/stat.h> // _stati64()


#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	idGUIDE		5003
#define idSIZE		5004

#define SAVELOGFILE

/////////////////////////////////////////////////////////////////////////////
// CMapWnd


CMapWnd::CMapWnd(CWnd* parent)
{
	m_parent = parent;
	m_updown = 0;

	m_status = statRUN;
	m_autosave = false;
	m_nblary.RemoveAll();
	m_nblConfigary.RemoveAll();
	m_bOpentoMap = false;
	m_uploadCount = 0;
	m_isUploadEnd = false;
	
	m_bInterestCtrl = FALSE;
	m_nType = 0;

	m_bBtnSave = false;
}

CMapWnd::~CMapWnd()
{
}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDRETRY, OnBtnSave)
	ON_BN_CLICKED(IDOK, OnBtnOK)
	ON_BN_CLICKED(IDCANCEL, OnBtnCC)
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(TOmapWnd, OnUpDnAction)
	ON_MESSAGE(WM_MSG, OnMsg)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Variant(titleCC, idTOstring(IDS_TITLE));
	m_user = Variant(userCC, "");
	m_root = Variant(homeCC, "");

	CFont* font = GetAxFont(idTOstring(IDS_DEFAULTFONT), 9, 0);
	CRect	clientRC, btn1RC, btn2RC;
	const	int buttonW = 75, buttonH = 24, gap9 = 7, gap3 = 3;

	GetClientRect(&clientRC);

	m_sheet = std::make_unique<CSheet>("", m_parent);
	m_sheet->Create(this, WS_CHILD|WS_VISIBLE);
	m_sheet->SetWindowPos(nullptr, 0, 0, 0, 0, SWP_NOSIZE);

	//2016.08.18 KSJ 유의사항과 버튼 추가
	btn1RC.left  = clientRC.left + 10;
	btn1RC.right = btn1RC.left + 600;
	btn1RC.top   = clientRC.bottom - (buttonH - gap3);
	btn1RC.bottom= btn1RC.top + buttonH;
	
	m_stSave.Create("※ 등록진행 중 저장을 원하시면 \"저장\"을, 등록을 모두 완료 후 저장하시려면 \"확인\"을 눌러주십시오.", WS_CHILD|WS_VISIBLE, btn1RC, this, IDC_STATIC);
	m_stSave.SetFont(font);

	btn1RC.left  = clientRC.right - (buttonW*3 + gap9*3);
	btn1RC.right = btn1RC.left + buttonW;
	btn1RC.top   = clientRC.bottom - (buttonH + gap3);
	btn1RC.bottom= btn1RC.top + buttonH;
	
	m_btnSave.Create(idTOstring(IDS_SAVE), WS_CHILD|WS_VISIBLE, btn1RC, this, IDRETRY);
	m_btnSave.SetFont(font);
	//2016.08.18 KSJ 유의사항과 버튼 추가 End

	btn1RC.left  = clientRC.right - (buttonW*2 + gap9*2);
	btn1RC.right = btn1RC.left + buttonW;
	btn1RC.top   = clientRC.bottom - (buttonH + gap3);
	btn1RC.bottom= btn1RC.top + buttonH;

	m_btnOK.Create(idTOstring(IDS_OK), WS_CHILD|WS_VISIBLE, btn1RC, this, IDOK);
	m_btnOK.SetFont(font);

	btn2RC = btn1RC;
	btn2RC.left  = btn1RC.right + gap9;
	btn2RC.right = btn2RC.left + buttonW;

	m_btnCC.Create(idTOstring(IDS_CANCEL), WS_CHILD|WS_VISIBLE, btn2RC, this, IDCANCEL);
	m_btnCC.SetFont(font);
	
	//Backup을 위한 서버 전송
	uploadBackup();

	SetTimer(idSIZE,10,FALSE);
	return 0;
}

#define	tempFILE	"portfolio.ini.tmp"

void CMapWnd::OnClose() 
{
// 	if(m_bOpentoMap == false)
// 	{
// 		if(m_sheet->IsMenuStatusClosed())
// 		{
// 			AfxMessageBox("메뉴검색기창을 먼저 닫아주세요");
// 			return;
// 		}
// 	}
	

	DestroyWindow(); 
	CWnd::OnClose();
}


void CMapWnd::OnDestroy() 
{
	CString root = Variant(homeCC, "");
	CString	name = Variant(nameCC, "");
#ifndef DF_SEARCH
	CString tempN; tempN.Format("%s/%s/%s/%s", root, "user", name, tempFILE);
	DeleteFile(tempN);

	CFileFind finder;
	CString	fileTemp;
	for (int ii = 0; ii < maxGROUP; ii++)
	{
		fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", root, "user", name, ii);

		if (!finder.FindFile(fileTemp))
			continue;

		DeleteFile(fileTemp);
	}
#endif
	m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)_T("CloseIB202201"));

	CWnd::OnDestroy();
}

BOOL CMapWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect clientRC; GetClientRect(&clientRC);
	pDC->FillSolidRect(clientRC, ::GetSysColor(COLOR_ACTIVEBORDER));
	return TRUE;	
	
	return CWnd::OnEraseBkgnd(pDC);
}

BOOL CMapWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch (m_status)
	{
	case statWAIT:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;
	case statRUN:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	default:
		break;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMapWnd::OnBtnSave()
{
	m_uploadCount = 0;
	m_isUploadEnd = false;
	m_bBtnSave = true;

	m_sheet->OkKillaction();
	m_sheet->OKSubtabaction();
	m_sheet->OKactionOne(m_iActivePage);
}

void CMapWnd::OnBtnOK()
{
	m_uploadCount = 0;
	m_isUploadEnd = false;
	m_bBtnSave = false;

	m_sheet->OkKillaction();
	m_sheet->OKSubtabaction();
	m_sheet->OKactionOne(m_iActivePage);
}

void CMapWnd::OnBtnCC()
{
	m_bBtnSave = false;
	uploadCancel();
}



long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		{
			int key = HIBYTE(LOWORD(wParam));
			switch (key)
			{
				case TRKEY_GROUPARR:             //최초화면 열면 우선 그룹먼저 조회해온다
				{
					m_arrGname.RemoveAll();

					CString stmp, seq, sgname;
					char* pdata = (char*)lParam;
					int cnt = atoi(CString(pdata, 4));
					if (cnt > 0)
					{
						int parseL = 4;
						struct _grSearch
						{
							char ngrs[2];
							char grseq[2];
							char gname[30];
						};

						struct _grSearch* stgr{};
						for (int ii = 0; ii < cnt; ii++)
						{
							stgr = (struct _grSearch*)&pdata[parseL];
							seq.Format("%.2s", stgr->grseq);
							sgname.Format("%.30s", stgr->gname);
							sgname.TrimRight();
							seq.TrimRight();
							if (seq.GetLength() == 0)
								break;

							m_arrGname.Add(seq + sgname);

							sgname.Empty();
							seq.Empty();
							parseL += sizeof(struct _grSearch);
						}

						//DominoDataProcess();
						m_sheet->initCombo();
						return 1;
					}
				}
				break;
				case TRKEY_GROUPCODE:
				case TRKEY_GROUPCODE_PAGE4:
				{
					char* pdata = (char*)lParam;

					CString stmp;
					CString strseq = CString(pdata, 2);
					CString strgname = CString(&pdata[2], 20);
					int cnt = atoi(CString(&pdata[22], 4));
					int parseL = 26;
					int len = HIWORD(wParam);

					char* pbuf = new char[len + 1];
					memset(pbuf, 0x00, len + 1);
					memcpy(pbuf, (char*)lParam, len);
					
					m_mapGroupWithCode.SetAt(strseq, pbuf);

					m_sheet->SendMsgToPage(key == TRKEY_GROUPCODE?1:4, MSG_MTP_CPL_SEARCH_GROUPCODE, strseq);
					return 1;
				}
				break;
				case TRKEY_upBackupACTION:			//'b			DB백업  다이알로그 시작시 호출 oub
					backupOubsUP((char*)lParam, HIWORD(wParam));
				break;
				case TRKEY_upEachGroupACTION:  //'G'      그룹콤보에서 다른 그룹 선택하거나 하면 보고있던 그룹내용을 저장한다. oub
				break;
				case TRKEY_upEndACTION:				// 'G'		저장버튼 누르면 그룹종목 전송  oub
					endOubs((char*)lParam, HIWORD(wParam));
				break;
				case TRKEY_upOKACTION:			// 'V'		 최종상태저장 oub
					OKOubs((char*)lParam, HIWORD(wParam));
				break;
				case TRKEY_upsequenceACTION:  //'S'			그룹편집에 대한 결과처리 oub
					sequenceOubsUP((char*)lParam, HIWORD(wParam));
				break;
				case TRKEY_upnewgroupACTION:  //'G			새로운 그룹 추가시 oub
				break;
				case TRKEY_dnACTIONR:
				case TRKEY_upRecoveryACTION:     //서버에서 관심종목 복구 oub
					recoveryOubsUP((char*)lParam, HIWORD(wParam));
					break;
				case TRKEY_upCancelACTION:   //취소버튼 누르면 oub
					cancelOubsUP((char*)lParam, HIWORD(wParam));
					break;
				case TRKEY_dnACTION:  //서버에서 관심종목 받기 oub
					m_sheet->EnablePage1Btn(TRUE);	// 20070620
					parsingOubsDN((char*)lParam, HIWORD(wParam));
					break;
			}
		}

		switch (m_updown)
		{
			case upACTION:									//사용되지 않는다
			{
				AfxMessageBox("upACTION");
				parsingOubsUP((char*)lParam, HIWORD(wParam));
			}
				break;
			case dnACTIONX:  //사용하지 않음 (구버전관심받기)
				AfxMessageBox("dnACTIONX");
				break;
			case ELWIssueDndnACTION:   //사용되지 않는다
			{
				AfxMessageBox("ELWIssueDndnACTION");
				m_sheet->sendELWIssueDN((char*)lParam, HIWORD(wParam));
			}
				break;
			case ELWAssetDndnACTION:  //사용되지 않는다
			{
				AfxMessageBox("ELWAssetDndnACTION");
				m_sheet->sendELWAssetDN((char*)lParam, HIWORD(wParam));
			}
				break;
			case RecommnadDnACTION:  //사용되지 않는다
			{
				AfxMessageBox("RecommnadDnACTION");
				m_sheet->sendRecommandDN((char*)lParam, HIWORD(wParam));
			}
				break;
		}
		break;

	case DLL_DOMINO:
		parsingDomino((char*)lParam);
		break;
	case DLL_GUIDE:
		return 1;
	}
	return 0;
}

long CMapWnd::OnMsg(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case MSG_GET_ACTIVE_PAGE:
			return m_iActivePage;
		break;
		case MSG_SET_ACTIVE_PAGE:
			m_iActivePage = (int)lParam;
		break;
		case MSG_SEARCH_GROUPCODE:
		case MSG_SEARCH_GROUPCODE_PAGE4:
		{
			int igroup = (int)lParam;
			SearchGroupCode(igroup, (int)LOWORD(wParam));
		}
		break;
		case MSG_SET_NEWGROUP:
		{
			CString seq, sgname;
			seq.Format("%.2d", m_arrGname.GetSize() + 1); seq.TrimRight();
			sgname.Format("%s", (char*)lParam); sgname.TrimRight();
			m_arrGname.Add(seq + sgname);
		}
		break;
		case MSG_GET_GROUPCOUNT:
			return m_arrGname.GetSize();
		break;
		case MSG_GET_GROUPARR:
		{
			if (m_arrGname.GetSize() == 0)
				return 0;

			CString seq, sgname, stmp;
			for (int ii = 0; ii < m_arrGname.GetSize(); ii++)
			{
				seq.Format("%s", m_arrGname.GetAt(ii).Left(2));
				sgname.Format("%s", m_arrGname.GetAt(ii).Mid(2, 30));
				sgname.TrimRight();
				stmp += seq;
				stmp += ";";
				stmp += sgname;
				stmp += "\t";
			}
		
			char* pResult = new char[stmp.GetLength()];
			memcpy((char*)lParam, (LPSTR)(LPCTSTR)stmp, stmp.GetLength());
			return 1;
		}
		break;
		case MSG_GET_GROUPCODE:
		{
		
			CString stmp;
			stmp.Format("%02d", HIWORD(wParam));

			char* pdata;
			if (m_mapGroupWithCode.Lookup(stmp, (void*&)pdata))
			{
				memcpy((char*)lParam, pdata, strlen(pdata));    //pdata 에 NULL 이 들어있으면 안된다.
				return TRUE;
			}
			else
				return FALSE;
		}
	}
	return 0;
}

long CMapWnd::OnUpDnAction(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case upACTION:						//사용되지 않는다
		updnWaitAction(0);
		uploadAction(true);
		break;
	case upsequenceACTION:       //'S'			그룹편집에 대한 결과처리
		uploadSequence();
		break;
	case upnewgroupACTION:      //'G			새로운 그룹 추가시
		uploadNewGroup();
		break;
	case upEachGroupACTION:		  //'G'      그룹콤보에서 다른 그룹 선택하거나 하면 보고있던 그룹내용을 저장한다.	
		uploadEachGroup(HIWORD(wParam));
		break;
	case upEndACTION:                  // 'G'		저장버튼 누르면 그룹종목 전송
		uploadEndGroup(HIWORD(wParam), (int)lParam);
		break;
	case upOKACTION:					 // 'V'		 최종상태저장  
		uploadOK();
		break;
	case dnACTION:						//서버에서 관심종목 받기	
		m_sheet->EnablePage1Btn(FALSE);	
		updnWaitAction(0);
		dnloadAction();
		break;
	case dnACTIONX:						//구버전 관심받기 (사용되지 않는다)
		m_sheet->EnablePage1Btn(FALSE);	// 20070620
		updnWaitAction(0);
		dnloadActionX();
		break;
	case dnACTIONR:						//서버에서 관심종목 복구	
		uploadRecovery();
		break;

	case ssACTION:						//사용되지 않는다
		m_updown = ssACTION;
		sendTR("pooppoop", (char *)lParam, HIWORD(wParam));  //사용되지 않는다
		break;
	case ELWIssueDndnACTION:  //사용되지 않는다
		elwIssueDnLoadAction(HIWORD(wParam));
		break;
	case ELWAssetDndnACTION:  //사용되지 않는다
		elwAssetDnLoadAction(HIWORD(wParam), (CHAR*)lParam);
		break;
	case RecommnadDnACTION:  //사용되지 않는다
		recommandDnLoadAction();
		break;
	}

	return 0;
}

void CMapWnd::uploadAction(bool init)  //사용되지 않는다
{
//	m_updown = upACTION;
//
//	int	sendL = 0;
//	char	sendB[16 * 1024]{}, tempB[32]{};
//	int	count{};
//
//	CString	gnox, gnam, gidx;
//
//	if (init)
//	{
//		int	gid = 0;
//		CString	string, stringx, tmpx;
//
//		string = m_sheet->GetGroupORder();
//		CString m_root = Variant(homeCC, "");
//		CString m_name = Variant(nameCC, "");
//		CString saveFile, tempFile;
//		CString tmp;
//		while (!string.IsEmpty())
//		{
//			stringx = parseX(string, ";");
//			m_nblary.Add(stringx);
//			
//			tmpx.Format("%02d", gid); 
//			m_gidary.Add(tmpx); 
//
//			gid += 1;
//			tmp.Format("%02d", gid);
//
//			if (stringx != tmp)
//			{
//				CString saveN;
//				saveFile.Format("bookmark.i%s", stringx);
//				saveN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, saveFile);
//
//				CString tempN; 
//				tempFile.Format("bookmark.i%s.save", tmp);
//				tempN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, tempFile);
//			
//				if (ExistFile(saveN))
//				{
//					CopyFile(saveN, tempN, false);
//				}
//
//				saveFile.Format("portfolio.i%s", stringx);  //사용되지 않는다
//				saveN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, saveFile);	
//				
//				tempFile.Format("portfolio.i%s.save", tmp);  //사용되지 않는다
//				tempN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, tempFile);
//				if (ExistFile(saveN))
//				{
//					CopyFile(saveN, tempN, false);
//					//DeleteFile(saveN);
//				}
//
//			}
//		}
//
//		m_nblc = m_nblary.GetSize();
//	}
//
//	if (m_nblc <= 0)
//	{
//		TRACE("upload file not found.....\n");
//		updnWaitAction(1); 
//		if (init) m_updown = 0;
//		return;
//	}
//
//	gnox = m_nblary.GetAt(0); 
//	m_nblary.RemoveAt(0);
//	gidx = m_gidary.GetAt(0); 
//	m_gidary.RemoveAt(0);
//
//	struct _updn* updn = (struct _updn *)&sendB[sendL];
//	sendL += sz_updn;
//
//	FillMemory((char *) updn, sz_updn, ' ');
//
//	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
//	updn->uinfo.dirt[0] = 'U';
//	updn->uinfo.cont[0] = init ? 'F' : 'M';
//
//	sprintf(tempB, "%05d", m_nblc);
//	CopyMemory(updn->uinfo.nblc, tempB, sizeof(updn->uinfo.nblc));
//
//	updn->uinfo.retc[0] = 'O';
//
//	count = m_sheet->GetUploadData(atoi(gnox), gnam, &sendB[sendL]);
//	sendL += count * sz_jinfo;
//
//	CopyMemory(updn->ginfo.gnox, gidx.operator LPCTSTR(), sizeof(updn->ginfo.gnox));
//	CopyMemory(updn->ginfo.gnam, gnam.operator LPCTSTR(), min(sizeof(updn->ginfo.gnam), gnam.GetLength()));
//	sprintf(tempB, "%04d", count); 
//	CopyMemory(updn->ginfo.jrec, tempB, sizeof(updn->ginfo.jrec));
//
//m_StartTick = GetTickCount();
//
//	sendB[sendL] = 0x00; 
//	sendTR(trUPDOWN, sendB, sendL, 0);
}

void	CMapWnd::sequenceOubsUP(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{
		//testSaveFile3("[202202]sequenceOubsUP length Err", "upload : response length error....");

		TRACE("upload : response length error....[%d]\n", datL);
		updnWaitAction(1); 
		return;
	}

	char datTemp[4084];
	ZeroMemory(datTemp, 4084);
	int	parseL = 0;
	
	memcpy(datTemp, datB, sizeof(_updn2));
	struct	_updn2*	updn = (struct _updn2 *)datTemp;
	
	parseL += sz_updn2;
	
	
	if (updn->uinfo.retc[0] == 'E')
	{
		struct	_uinfo	infoC;
		FillMemory(&infoC, sizeof(_uinfo), ' ');
		
		CopyMemory(&infoC.gubn[0], "MY", sizeof(infoC.gubn));
		infoC.dirt[0] = 'U';
		infoC.cont[0] = 'C';		// 취소
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, TRKEY_upCancelACTION);

		//testSaveFile3("[202202]sequenceOubsUP Err", CString(updn->uinfo.emsg, sizeof(updn->uinfo.emsg)));

		TRACE("upload : response error....[%s]\n", CString(updn->uinfo.emsg, sizeof(updn->uinfo.emsg)));
		return;
	}
	//	시퀀스 portfolio 에 저장
	initSaveFile((char *)(datB+sz_uinfo));
//	m_sheet->selectGroup(0);
//	m_sheet->initCombo();
	SearchGroupList();
}


void	CMapWnd::recoveryOubsUP(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{
		//testSaveFile3("[202202]recoveryOubsUP length Err");
		TRACE("upload : response length error....[%d]\n", datL);
		updnWaitAction(1); 
		return;
	}
	
	char datTemp[4084];
	ZeroMemory(datTemp, 4084);
	int	parseL = 0;
	
	memcpy(datTemp, datB, sizeof(_uinfo));
	struct	_uinfo*	uinfo = (struct _uinfo *)datTemp;
	
	parseL += sz_uinfo;
	
	
	if (uinfo->retc[0] == 'E')
	{
		struct	_uinfo	infoC;
		FillMemory(&infoC, sizeof(_uinfo), ' ');
		
		CopyMemory(&infoC.gubn[0], "MY", sizeof(infoC.gubn));
		infoC.dirt[0] = 'U';
		infoC.cont[0] = 'C';		// 취소
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, TRKEY_upCancelACTION);

		//testSaveFile3("[202202]recoveryOubsUP Err", CString(uinfo->emsg, sizeof(uinfo->emsg)));
		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));

	}

	m_updown = dnACTION;                         
	
	FillMemory((char*)uinfo, sizeof(_uinfo), ' ');
	
	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'D';
	uinfo->cont[0] = 'F';
	CopyMemory(uinfo->nblc, "00000", sizeof(uinfo->nblc));
	
	sendTR(trUPDOWN, (char*)uinfo, sz_uinfo, TRKEY_dnACTION);
}

void	CMapWnd::cancelOubsUP(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{
		//testSaveFile3("[202202]cancelOubsUP  length Err");

		TRACE("upload : response length error....[%d]\n", datL);
		updnWaitAction(1); 
		return;
	}
	
	char datTemp[4084];
	ZeroMemory(datTemp, 4084);
	int	parseL = 0;
	
	memcpy(datTemp, datB, sizeof(_uinfo));
	struct	_uinfo*	uinfo = (struct _uinfo *)datTemp;
	
	parseL += sz_uinfo;
	
	
	if (uinfo->retc[0] == 'E')
	{
		struct	_uinfo	infoC;
		FillMemory(&infoC, sizeof(_uinfo), ' ');
		
		CopyMemory(&infoC.gubn[0], "MY", sizeof(infoC.gubn));
		infoC.dirt[0] = 'U';
		infoC.cont[0] = 'C';		// 취소
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, TRKEY_upCancelACTION);

		//testSaveFile3("[202202]cancelOubsUP Err", CString(uinfo->emsg, sizeof(uinfo->emsg)));

		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));
		
	}
	
	m_sheet->recoveryFile();
	m_sheet->deleteBackupFile();
	m_sheet->CCaction();
	
	m_parent->SendMessage(WM_USER, closeDLL, 0);

}
void	CMapWnd::endOubs(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{
		//testSaveFile3("[202202]endOubs length Err");

		TRACE("upload : response length error....[%d]\n", datL);
		updnWaitAction(1); 
		return;
	}
	
	char datTemp[4084];
	ZeroMemory(datTemp, 4084);
	int	parseL = 0;
	
	memcpy(datTemp, datB, sizeof(_uinfo));
	struct	_uinfo*	uinfo = (struct _uinfo *)datTemp;
	
	parseL += sz_uinfo;
	
	
	if (uinfo->retc[0] == 'E')
	{
		struct	_uinfo	infoC;
		FillMemory(&infoC, sizeof(_uinfo), ' ');
		
		CopyMemory(&infoC.gubn[0], "MY", sizeof(infoC.gubn));
		infoC.dirt[0] = 'U';
		infoC.cont[0] = 'C';		// 취소
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, TRKEY_upCancelACTION);

		//testSaveFile3("[202202]endOubs Err", CString(uinfo->emsg, sizeof(uinfo->emsg)));

		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));
	}
	
	m_sheet->deleteBackupFile();
	uploadOK();
// 	CString string; 
// 	string = "OnPortfolio\tok";
// 	m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
//  m_parent->SendMessage(WM_USER, closeDLL, 0);	
}


void	CMapWnd::OKOubs(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{
		//testSaveFile3("[202202]OKOubs length Err");

		TRACE("upload : response length error....[%d]\n", datL);
		updnWaitAction(1); 
		return;
	}
	
	char datTemp[4084];
	ZeroMemory(datTemp, 4084);
	int	parseL = 0;
	
	memcpy(datTemp, datB, sizeof(_uinfo));
	struct	_uinfo*	uinfo = (struct _uinfo *)datTemp;
	
	parseL += sz_uinfo;
	
	
	if (uinfo->retc[0] == 'E')
	{
		struct	_uinfo	infoC;
		FillMemory(&infoC, sizeof(_uinfo), ' ');
		
		CopyMemory(&infoC.gubn[0], "MY", sizeof(infoC.gubn));
		infoC.dirt[0] = 'U';
		infoC.cont[0] = 'C';		// 취소
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, TRKEY_upCancelACTION);
		
		//testSaveFile3("[202202]OKOubs Err", CString(uinfo->emsg, sizeof(uinfo->emsg)));

		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));
	}
	
	CString string; 
	string = "OnPortfolio\tok";
	m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);

	if(!m_bBtnSave)	m_parent->SendMessage(WM_USER, closeDLL, 0);		//2016.08.18 KSJ 저장버튼 추가
}


void	CMapWnd::backupOubsUP(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{
		//testSaveFile3("[202202]backupOubsUP length Err");

		TRACE("upload : response length error....[%d]\n", datL);
		updnWaitAction(1); 
		return;
	}
	
	char datTemp[4084];
	ZeroMemory(datTemp, 4084);
	int	parseL = 0;
	
	memcpy(datTemp, datB, sizeof(_uinfo));
	struct	_uinfo*	uinfo = (struct _uinfo *)datTemp;
	
	parseL += sz_uinfo;
	
	
	if (uinfo->retc[0] == 'E')
	{
		struct	_uinfo	infoC;
		FillMemory(&infoC, sizeof(_uinfo), ' ');
		
		CopyMemory(&infoC.gubn[0], "MY", sizeof(infoC.gubn));
		infoC.dirt[0] = 'U';
		infoC.cont[0] = 'C';		// 취소
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, TRKEY_upCancelACTION);

		//testSaveFile3("[202202]backupOubsUP Err", CString(uinfo->emsg, sizeof(uinfo->emsg)));
		
		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));
	}
}


void CMapWnd::parsingOubsUP(char* datB, int datL)
{
//////////////////////////////////////////////////////////////////////////////////////
	
	if (datL < sz_uinfo)
	{
		//testSaveFile3("[202202]parsingOubsUP length Err");

		TRACE("upload : response length error....[%d]\n", datL);
		updnWaitAction(1); 
		return;
	}

 	char datTemp[4084];
 	ZeroMemory(datTemp, 4084);
	int	parseL = 0;

	memcpy(datTemp, datB, sizeof(_uinfo));
	struct	_uinfo*	uinfo = (struct _uinfo *)datTemp;

	parseL += sz_uinfo;

	
	if (uinfo->retc[0] == 'E')
	{
		struct	_uinfo	infoC;
		FillMemory(&infoC, sizeof(_uinfo), ' ');

		CopyMemory(&infoC.gubn[0], "MY", sizeof(infoC.gubn));
		infoC.dirt[0] = 'U';
		infoC.cont[0] = 'C';		// 취소

		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, TRKEY_upCancelACTION);
		
		//testSaveFile3("[202202]parsingOubsUP Err", CString(uinfo->emsg, sizeof(uinfo->emsg)));

		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));

		return;
	}
	
	int sizeU = strlen((char*)uinfo);
	const int sizeDatB = strlen(datB);

	m_sheet->OKaction();



	if(sizeDatB > sizeU)
	{
	//	initSaveFile((char *)(datB+sizeU));
	}
	else
	{
	//	initSaveFile();
	}

	CString string; 
	string = "OnPortfolio\tok";
	m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
	m_parent->SendMessage(WM_USER, closeDLL, 0);


}

int getFileSize(CString filename) 
{
	struct _stati64 statbuf{};	
	if ( _stati64(filename.GetString(), &statbuf) ) 
		return -1;							// 파일 정보 얻기: 에러 있으면 -1 을 반환
	
	return gsl::narrow_cast<int>(statbuf.st_size);					// 파일 크기 반환
}




void CMapWnd::uploadSequence()
{
	m_updown = upsequenceACTION;

	int	count{};
	struct _updn2 updn2;
	
	FillMemory((char*)&updn2, sz_updn2, ' ');
	
	//uinfo setting
	CopyMemory(updn2.uinfo.gubn, "MY", sizeof(updn2.uinfo.gubn));
	updn2.uinfo.dirt[0] = 'U';
	updn2.uinfo.cont[0] = 'S';
	updn2.uinfo.retc[0] = 'O';	
		
	count = m_sheet->getManageCount();

	CString tmpCount;

	CString ngrs, ogrs, gnam, mode;
	
	int j=0;

	for(int i=0; i<count ; i++)
	{
		ogrs = m_sheet->getManageGroupdata(i,0);		//old
		mode = m_sheet->getManageGroupdata(i,1);		//mode (D : 삭제) N : 그룹 추가 
		ngrs.Format("%02d",j+1);							//new
		gnam = m_sheet->getManageGroupdata(i,3);		//name
		
		if(mode != "D")
		{
			if(gnam.IsEmpty())		//2015.03.23 KSJ 그룹이름이 없으면 나간다.
			{
				break;
			}

			CopyMemory(updn2.gfoldex.glist[j].ogrs, ogrs, strlen(ogrs));
			CopyMemory(updn2.gfoldex.glist[j].ngrs, ngrs, strlen(ngrs));
			CopyMemory(updn2.gfoldex.glist[j].gnam, gnam, strlen(gnam));

			j++;
		}

	}

	tmpCount.Format("%05d", j);
	CopyMemory(updn2.uinfo.nblc, tmpCount, sizeof(updn2.uinfo.nblc));

	//gfoldex setting
	tmpCount.Format("%04d", j);
	CopyMemory(updn2.gfoldex.nrec, tmpCount, sizeof(updn2.gfoldex.nrec));

	sendTR(trUPDOWN, (char*)&updn2, sz_updn2, TRKEY_upsequenceACTION);
}

void CMapWnd::uploadNewGroup()
{
	m_updown = upnewgroupACTION;
	
	int	sendL = 0;
	char	sendB[16 * 1024]{}, tempB[32]{};
//	char	strUinfo[500];

	struct _updn* updn = (struct _updn *)&sendB[sendL];
	sendL += sz_updn;
	
	CString ogrs, mode, gnam;
	FillMemory((char *) updn, sz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'G';

	sprintf(tempB, "00001");
	CopyMemory(updn->uinfo.nblc, tempB, sizeof(updn->uinfo.nblc));

	updn->uinfo.retc[0] = 'O';
	
	int count = 0;

	int itest = m_sheet->getManageCount();
	for(int i=0; i<m_sheet->getManageCount() ; i++)
	{
		ogrs = m_sheet->getManageGroupdata(i,0);		//old
		mode = m_sheet->getManageGroupdata(i,1);		//mode (D : 삭제) N : 그룹 추가 
		gnam = m_sheet->getManageGroupdata(i,3);		//name

		if(mode == "N")
		{
			count = m_sheet->GetUploadData(atoi(ogrs), gnam, &sendB[sendL]);
			sendL += count * sz_jinfo;
		}

#ifdef SAVELOGFILE
		CString strTemp;		
		strTemp.Format("count[%d] orgs[%s] mode[%s] gnam[%s]", count, ogrs, mode, gnam);
		//testSaveFile3("[202202]uploadNewGroup", strTemp);
#endif

	}

	CopyMemory(updn->ginfo.gnox, "-1", sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, gnam.operator LPCTSTR(), min(sizeof(updn->ginfo.gnam), gnam.GetLength()));
	sprintf(tempB, "%04d", count); 
	CopyMemory(updn->ginfo.jrec, tempB, sizeof(updn->ginfo.jrec));

	m_StartTick = GetTickCount();

	sendB[sendL] = 0x00; 
	sendTR(trUPDOWN, sendB, sendL, TRKEY_upnewgroupACTION);

}

void CMapWnd::uploadEachGroup(int gno)
{
	m_updown = upEachGroupACTION;

	int	sendL = 0;
	char	sendB[16 * 1024]{}, tempB[32]{};
	//	char	strUinfo[500];

	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += sz_updn;

	CString ogrs, mode, gnam;
	FillMemory((char*)updn, sz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'G';

	sprintf(tempB, "00001");
	CopyMemory(updn->uinfo.nblc, tempB, sizeof(updn->uinfo.nblc));

	updn->uinfo.retc[0] = 'O';

	int count = 0;

	ogrs = m_sheet->getManageGroupdata(gno, 0);		//old
	mode = m_sheet->getManageGroupdata(gno, 1);		//mode (D : 삭제) N : 그룹 추가 
	gnam = m_sheet->getManageGroupdata(gno, 3);		//name

	count = m_sheet->GetUploadData(atoi(ogrs), gnam, &sendB[sendL]);
	sendL += count * sz_jinfo;

#ifdef SAVELOGFILE
	CString strTemp;
	strTemp.Format("count[%d] orgs[%s] mode[%s] gnam[%s]", count, ogrs, mode, gnam);
	//testSaveFile3("[202202]uploadEachGroup", strTemp);
#endif

	CopyMemory(updn->ginfo.gnox, ogrs.operator LPCTSTR(), sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, gnam.operator LPCTSTR(), min(sizeof(updn->ginfo.gnam), gnam.GetLength()));
	sprintf(tempB, "%04d", count);
	CopyMemory(updn->ginfo.jrec, tempB, sizeof(updn->ginfo.jrec));

	m_StartTick = GetTickCount();

	sendB[sendL] = 0x00;
	sendTR(trUPDOWN, sendB, sendL, TRKEY_upEachGroupACTION);
}

void CMapWnd::uploadEndGroup(int gno, int ipage)
{
	m_updown = upEndACTION;
	
	int	sendL = 0;
	char	sendB[16 * 1024]{}, tempB[32]{};

	struct _updn* updn = (struct _updn *)&sendB[sendL];
	sendL += sz_updn;
	
	CString ogrs, mode, gnam;
	FillMemory((char *) updn, sz_updn, ' ');
	
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'G';   //그룹종목전송
	
	sprintf(tempB, "00001");
	CopyMemory(updn->uinfo.nblc, tempB, sizeof(updn->uinfo.nblc));
	
	updn->uinfo.retc[0] = 'O';
	
	int count = 0;
	
	ogrs = m_sheet->getManageGroupdata(gno,0);		//old
	mode = m_sheet->getManageGroupdata(gno,1);		//mode (D : 삭제) N : 그룹 추가 
	gnam = m_sheet->getManageGroupdata(gno,3);		//name
	
	count = m_sheet->GetUploadData(atoi(ogrs), gnam, &sendB[sendL], ipage);
	sendL += count * sz_jinfo;
	
#ifdef SAVELOGFILE
	CString strTemp;		
	strTemp.Format("count[%d] orgs[%s] mode[%s] gnam[%s]", count, ogrs, mode, gnam);
	//testSaveFile3("[202202]uploadEndGroup", strTemp);
#endif

	CopyMemory(updn->ginfo.gnox, ogrs.operator LPCTSTR(), sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, gnam.operator LPCTSTR(), min(sizeof(updn->ginfo.gnam), gnam.GetLength()));
	sprintf(tempB, "%04d", count); 
	CopyMemory(updn->ginfo.jrec, tempB, sizeof(updn->ginfo.jrec));
	
	m_StartTick = GetTickCount();
	
	sendB[sendL] = 0x00; 
	sendTR(trUPDOWN, sendB, sendL, TRKEY_upEndACTION);
}

void	CMapWnd::uploadOK()
{
	m_updown = 	upOKACTION;
	
	int	sendL = 0;
	char	sendB[16 * 1024]{}, tempB[32]{};
//	char	strUinfo[500];
	
	struct _uinfo* uinfo = (struct _uinfo *)&sendB[sendL];
	sendL += sz_uinfo;
	
	FillMemory((char *) uinfo, sz_uinfo, ' ');
	
	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'V';
	
	sprintf(tempB, "00000");
	CopyMemory(uinfo->nblc, tempB, sizeof(uinfo->nblc));
	
	uinfo->retc[0] = 'O';
	
	sendB[sendL] = 0x00; 
	sendTR(trUPDOWN, sendB, sendL, TRKEY_upOKACTION);
}

void	CMapWnd::uploadBackup()
{
	m_updown = 	upBackupACTION;

	int	sendL = 0;
	char	sendB[16 * 1024]{}, tempB[32]{};;
	struct _uinfo* uinfo = (struct _uinfo *)&sendB[sendL];
	sendL += sz_uinfo;

	FillMemory((char *) uinfo, sz_uinfo, ' ');
	
	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'b';
	
	sprintf(tempB, "00000");
	CopyMemory(uinfo->nblc, tempB, sizeof(uinfo->nblc));
	
	uinfo->retc[0] = 'O';

	sendB[sendL] = 0x00; 
	sendTR(trUPDOWN, sendB, sendL, TRKEY_upBackupACTION);

}

void	CMapWnd::uploadCancel()
{
	m_updown = 	upCancelACTION;
	
	int	sendL = 0;
	char	sendB[16 * 1024]{}, tempB[32]{};
//	char	strUinfo[500];
	
	struct _uinfo* uinfo = (struct _uinfo *)&sendB[sendL];
	sendL += sz_uinfo;
	
	FillMemory((char *) uinfo, sz_uinfo, ' ');
	
	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'R';
	
	sprintf(tempB, "00000");
	CopyMemory(uinfo->nblc, tempB, sizeof(uinfo->nblc));
	
	uinfo->retc[0] = 'O';
	
	sendB[sendL] = 0x00; 
	sendTR(trUPDOWN, sendB, sendL, TRKEY_upCancelACTION);
}

void	CMapWnd::uploadRecovery()   //서버에서 관심종목 복구
{
	m_updown = 	upRecoveryACTION;

	int	sendL = 0;
	char	sendB[16 * 1024]{}, tempB[32]{};
//	char	strUinfo[500];
	
	struct _uinfo* uinfo = (struct _uinfo *)&sendB[sendL];
	sendL += sz_uinfo;
	
	FillMemory((char *) uinfo, sz_uinfo, ' ');
	
	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'r';
	
	sprintf(tempB, "00000");
	CopyMemory(uinfo->nblc, tempB, sizeof(uinfo->nblc));
	
	uinfo->retc[0] = 'O';

	sendB[sendL] = 0x00; 
	sendTR(trUPDOWN, sendB, sendL, TRKEY_upRecoveryACTION);
}

void CMapWnd::uploadActionGroup(bool init)  //사용되지 않음
{
	m_updown = upACTION;
	
//	bool isModify = false;			// 수정된게 있는지 파악 

	int	sendL = 0;
	char	sendB[16*1024];
	char	strUinfo[500];

//	int	count;
	CString	gnox, gnam, gidx;

//	int	gid;
	CString	string, stringx, tmpx, configx;
	
	CString m_root = Variant(homeCC, "");
	CString m_name = Variant(nameCC, "");
	CString saveFile, tempFile;
	CString tmp;
	CString saveN, tempN, tempOrigin;

	//수정된 그룹만 저장되도록 변경	
	CString m_manageGroup, m_manageConfig;
	m_manageGroup = "";
	m_manageConfig = "";
	m_manageGroup = m_sheet->GetGroupORder2(m_manageConfig);
	
	ZeroMemory(sendB, 16*1024);
	ZeroMemory(strUinfo, 500);
	struct _uinfo* uinfo = (struct _uinfo *)strUinfo;

	FillMemory((char *)uinfo, sz_uinfo, ' ');
	
	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'E';
	
	uinfo->retc[0] = 'O';	

	int nCount = 0;
//	int nSize = 0;
	tmp = "";

	//기존에 관심그룹들중 내용 수정된 게 있으면 업로드
	while (!m_manageGroup.IsEmpty())
	{
		stringx = parseX(m_manageGroup, ";");
		configx = parseX(m_manageConfig, ";");

		tempFile.Format("portfolio.i%s.tmp", stringx);  //사용되지 않음
		tempN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, tempFile);

		tempOrigin.Format("%s\\%s\\%s\\portfolio.i%s", m_root, "user", m_name, stringx);  //사용되지 않음
	

		//temp가 있는지 확인
		if (ExistFile(tempN))
		{
				const int size1 = getFileSize(tempN);
				const int size2 = getFileSize(tempOrigin);
				
				//있으면 사이즈 비교
				if(size1 != size2)
				{
					if(!configx.IsEmpty())
						tmp += send(stringx, configx);
					else
						tmp += send(stringx, "G");
					
					nCount++;
				}
				else
				{
					if(!configx.IsEmpty())
					{
						tmp += send(stringx, configx);

						nCount++;
					}
				}
		}
		//temp없으면 N, D가 있는지 확인
		else
		{
			if(!configx.IsEmpty())
			{
				tmp += send(stringx, configx);

				nCount++;
			}
		}

	}

	//수정할 그룹 개수
	CString tmpCount;
	tmpCount.Format("%05d", nCount);
	CopyMemory(uinfo->nblc, tmpCount, sizeof(uinfo->nblc));
	
	CString temp2;
	temp2.Format("%s%s", strUinfo, tmp);

	sendL = strlen(temp2);
	CopyMemory(sendB, temp2, sendL-1);

	m_StartTick = GetTickCount();

	sendB[sendL] = 0x00; 

	sendTR(trUPDOWN, sendB, sendL, 0);	   //사용되지 않음

}

CString CMapWnd::send(CString sqen, CString type)
{	
	CString result, jinfo;
	result = "";
	int count = 0;
//	UINT	datL = 0;
	char	tempB[4];

	CString	gnam, temp;

	struct _foldex foldex;
	
	ZeroMemory(&foldex, sz_foldex);
	ZeroMemory(tempB, 4);
	

	memset(foldex.gubn, ' ', sizeof(foldex.gubn));
	memset(foldex.kjid, ' ', sizeof(foldex.kjid));
	memset(foldex.name, ' ', sizeof(foldex.name));
	memset(foldex.nrec, ' ', sizeof(foldex.nrec));

	jinfo = m_sheet->GetUploadData2(atoi(sqen), gnam, count);

	if(type == "M" || type == "N")
	{
		CopyMemory(foldex.gubn, "G", sizeof(foldex.gubn));
	}
	else
	{
		CopyMemory(foldex.gubn, type.operator LPCTSTR(), sizeof(foldex.gubn));
	}
	
	if(type == "N")
	{
		CopyMemory(foldex.kjid, "-1", sizeof(foldex.kjid));
	}
	else
	{
		CopyMemory(foldex.kjid, sqen.operator LPCTSTR(), sizeof(foldex.kjid));
	}
	
	CopyMemory(foldex.name, gnam.operator LPCTSTR(), min(sizeof(foldex.name), gnam.GetLength()));
	sprintf(tempB, "%04d", count);
	CopyMemory(foldex.nrec, tempB, sizeof(foldex.nrec));

	temp = "";

	temp += CString(foldex.gubn, sizeof(foldex.gubn));
	temp += CString(foldex.kjid, sizeof(foldex.kjid));
	temp += CString(foldex.name, sizeof(foldex.name));
	temp += CString(foldex.nrec, sizeof(foldex.nrec));

	result.Format("%s~%s|", temp, jinfo);

	return result;
}

void CMapWnd::dnloadAction()
{
	m_updown = dnACTION;

	struct	_uinfo	uinfo;

	FillMemory((char*) &uinfo, sizeof(_uinfo), ' ');

	CopyMemory(uinfo.gubn, "MY", sizeof(uinfo.gubn));
	uinfo.dirt[0] = 'D';
	uinfo.cont[0] = 'F';
	CopyMemory(uinfo.nblc, "00000", sizeof(uinfo.nblc));

	sendTR(trUPDOWN, (char*)&uinfo, sz_uinfo, TRKEY_dnACTION);
}

void CMapWnd::recommandDnLoadAction()  //사용되지 않는다
{
	m_updown = RecommnadDnACTION;

	struct  mid 
	{
		char    gubn[ 1];   /*  구분('0':헤더, '1':본문)    */
				    /*  '2': 종목코드리스트         */
	};
	
	struct mid mid;
	FillMemory(&mid, sizeof(mid), ' ');	
	
	CopyMemory(&mid.gubn, _T("2"), sizeof(mid.gubn));

	sendTR("pihorsdt", (char*)&mid, sizeof(mid), 0);   //사용되지 않음

}


void CMapWnd::elwIssueDnLoadAction(int selItem)
{
	m_updown = ELWIssueDndnACTION;

	struct mid
	{
		char    flag[1];
		char    ascf[1];            // 0: 상위 1: 하위  
		char    kcod[1];            //권리유형 code    
		char    gcod[12];           // 기초자산 code    
		char    bcod[12];           // 발행사 code      
		char    jjis[1];            // 잔존일수        
		char    sort[2];            // Sorting 구분     
		char    rtsf[9];            /// 1. RTS  0: NOT
		char    fill[1];            // dummy          
		char    gvol[1];            // 1. gvol > 0   
		char    gsrt[1];            // 1: 그리드소팅  
	};
	
	//선택한 Key값을 기준으로 코드값 가져오기
	int readL = 0; 
	char readB[256]{};
	CString	filePath, string, string_code, string_name, skey;
	
	CString root = Variant(homeCC, "");
	filePath.Format("%s/%s/%s", root, "tab", "ELWLP.CBS");
	
	for(int ii=0 ; ii<200 ; ii++)
	{
		skey.Format("%d", ii);
		readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
		if(readL <= 0) break;
		
		if(ii == selItem)
		{
			string = CString(readB, readL);
			string.TrimRight();
			string_code = string.Left(12);
			break;
		}
		
	}	
	
	struct mid mid;
	FillMemory(&mid, sizeof(mid), ' ');
	
	CopyMemory(&mid.flag, _T("1"), sizeof(mid.flag));
	CopyMemory(&mid.ascf, _T("0"), sizeof(mid.ascf));
	CopyMemory(&mid.kcod, _T(" "), sizeof(mid.kcod));
	CopyMemory(&mid.gcod, _T("A99999999999"), sizeof(mid.gcod));
	CopyMemory(&mid.bcod, string_code, sizeof(mid.bcod));
	CopyMemory(&mid.jjis, _T("0"), sizeof(mid.jjis));
	CopyMemory(&mid.sort, _T("01"), sizeof(mid.sort));
	CopyMemory(&mid.rtsf, _T("01"), 2);
	CopyMemory(&mid.fill, _T(" "), sizeof(mid.fill));
	CopyMemory(&mid.gvol, _T(" "), sizeof(mid.gvol));
	CopyMemory(&mid.gsrt, _T(" "), sizeof(mid.gsrt));
	
	sendTR("PIBO2805", (char*)&mid, sizeof(mid), 0);   //사용되지 않음
}

void CMapWnd::elwAssetDnLoadAction(int selItem, CString item)
{
	m_updown = ELWAssetDndnACTION;

	struct mid
	{
		char    flag[1];
		char    ascf[1];            // 0: 상위 1: 하위  
		char    kcod[1];            //권리유형 code    
		char    gcod[12];           // 기초자산 code    
		char    bcod[12];           // 발행사 code      
		char    jjis[1];            // 잔존일수        
		char    sort[2];            // Sorting 구분     
		char    rtsf[9];            /// 1. RTS  0: NOT
		char    fill[1];            // dummy          
		char    gvol[1];            // 1. gvol > 0   
		char    gsrt[1];            // 1: 그리드소팅  
	};
	
	//콜인지 풋인지 확인
	CString kcod;
	if(item == "콜")
	{
		kcod = _T("1");
	}
	else if(item == "풋")
	{
		kcod = _T("2");
	}
	else
	{
		kcod = _T(" ");
	}

	//선택한 Key값을 기준으로 코드값 가져오기
	int readL = 0; 
	char readB[256]{};
	CString	filePath, string, string_code, string_name, skey;
	
	CString root = Variant(homeCC, "");
	filePath.Format("%s/%s/%s", root, "tab", "ELWUD.CBS");
	
	for(int ii=0 ; ii<200 ; ii++)
	{
		skey.Format("%d", ii);
		readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
		if(readL <= 0) break;
		
		if(ii == selItem)
		{
			string = CString(readB, readL);
			string.TrimRight();
			string_code = string.Left(12);
			break;
		}
		
	}	
	
	struct mid mid;
	FillMemory(&mid, sizeof(mid), ' ');
	
	CopyMemory(&mid.flag, _T("1"), sizeof(mid.flag));
	CopyMemory(&mid.ascf, _T("0"), sizeof(mid.ascf));
	CopyMemory(&mid.kcod, kcod, sizeof(mid.kcod));
	CopyMemory(&mid.gcod, string_code, sizeof(mid.gcod));
	CopyMemory(&mid.bcod, _T("A99999999999"), sizeof(mid.bcod));
	CopyMemory(&mid.jjis, _T("0"), sizeof(mid.jjis));
	CopyMemory(&mid.sort, _T("01"), sizeof(mid.sort));
	CopyMemory(&mid.rtsf, _T("01"), 2);
	CopyMemory(&mid.fill, _T(" "), sizeof(mid.fill));
	CopyMemory(&mid.gvol, _T(" "), sizeof(mid.gvol));
	CopyMemory(&mid.gsrt, _T(" "), sizeof(mid.gsrt));
	
	sendTR("PIBO2805", (char*)&mid, sizeof(mid), 0);   //사용되지 않음
}

void CMapWnd::dnloadActionX()
{
	m_updown = dnACTION;

	struct	_uinfo	uinfo;

	FillMemory((char*) &uinfo, sizeof(_uinfo), ' ');

	CopyMemory(uinfo.gubn, "MY", sizeof(uinfo.gubn));
	uinfo.dirt[0] = 'X';
	uinfo.cont[0] = 'F';
	CopyMemory(uinfo.nblc, "00000", sizeof(uinfo.nblc));

	sendTR(trUPDOWN, (char*)&uinfo, sz_uinfo, TRKEY_dnACTION);
}


void CMapWnd::dnloadActionR()
{
	m_updown = dnACTION;

	struct	_uinfo	uinfo;

	FillMemory((char*) &uinfo, sizeof(_uinfo), ' ');

	CopyMemory(uinfo.gubn, "MY", sizeof(uinfo.gubn));
	uinfo.dirt[0] = 'R';
	uinfo.cont[0] = 'F';
	CopyMemory(uinfo.nblc, "00000", sizeof(uinfo.nblc));

	sendTR(trUPDOWN, (char*)&uinfo, sz_uinfo, TRKEY_dnACTION);
}



void CMapWnd::parsingOubsDN(char* datB, int datL)
{
	if (datL < sz_updn)
	{
		TRACE("dnload : response length error....[%d]\n", datL);
		updnWaitAction(1); 
		return;
	}

	int	parseL = 0;
	struct _updn* updn = (struct _updn *) &datB[0];
	parseL += sz_updn;

	if (updn->uinfo.retc[0] == 'E')
	{
		TRACE("dnload : response error....[%s]\n", CString(updn->uinfo.emsg, sizeof(updn->uinfo.emsg)));
		updnWaitAction(1); 
		return;
	}

	CString	gnox, gnam, jcnt;
	
	gnox = CString(updn->ginfo.gnox, sizeof(updn->ginfo.gnox)); 
	gnox.TrimRight();

	gnam = CString(updn->ginfo.gnam, sizeof(updn->ginfo.gnam)); 
	gnam.TrimRight();
	jcnt = CString(updn->ginfo.jrec, sizeof(updn->ginfo.jrec)); 
	jcnt.TrimRight();
	

	if (parseL + (atoi(jcnt) * sz_jinfo) <= datL)
		m_sheet->SetDnloadData(atoi(gnox), gnam, atoi(jcnt), &datB[parseL]);

	if (updn->uinfo.cont[0] != 'L')
	{
		struct	_uinfo	uinfo;
		FillMemory((char*)&uinfo, sz_uinfo, ' ');

		CopyMemory((char *)&uinfo, (char*) &updn->uinfo, sz_uinfo);

		sendTR(trUPDOWN, (char *)&uinfo, sz_uinfo, TRKEY_dnACTION);
		return;
	}
	m_updown = 0; 
	updnWaitAction(2); 
	m_guide = idTOstring(IDS_GUIDE6); 

	((CWnd*)(m_sheet->GetParent()->GetDlgItem(IDOK)))->EnableWindow(TRUE);	//2016.07.12 KSJ 복구 버튼 누르고 전부 받으면 확인버튼을 true 해준다.
}

void CMapWnd::updnWaitAction(int wait)
{
	switch (wait)
	{
	case 0:			// wait
		m_status = statWAIT;
		m_sheet->enableButton(FALSE);
		break;
	case 2:			// run
		m_sheet->reInitInterest();
	case 1:			// error & run
		m_status = statRUN;
		m_sheet->enableButton(TRUE);
		break;
	}
}

void CMapWnd::parsingDomino(CString datB)
{
	CString symbol,entry, mapp, entry2, entry3, entry4;
//#ifdef DF_SEARCH
//	datB = "selectGROUPNCTRLNOT2000\t0\nNO\tSMALL\2000\t";
//	m_strDomino = datB;
//	SearchGroupList();
//	return; 	//vc2019 test
//#endif
	while (!datB.IsEmpty())
	{
		symbol = parseX(datB, "\t");
		entry  = parseX(datB, "\n");
				
		if (symbol == _T("appendCODE"))
		{
			m_sheet->updateCode(entry);
			m_sheet->m_only = true;
		}
		else if (symbol == _T("registerCODE") && !entry.IsEmpty())
		{
			m_sheet->m_only = true;
		}
		else if (symbol == _T("appendGROUP") && !entry.IsEmpty())
		{
			m_sheet->updateGroup(entry);   //관심설정화면을 띄우면서 새그룹창이 뜬다 202202없다.
			m_sheet->m_only = false;
		}
		else if (symbol == _T("appendParentGROUP") && !entry.IsEmpty())
		{
			m_sheet->updateParentGroup(entry);
			m_sheet->m_only = false;
		}
		else if (symbol == _T("selectGROUP") && !entry.IsEmpty())
		{
			m_sheet->selectGroup(atoi(entry));
			entry2  = parseX(datB, "\t");
			if ( entry2 != "NO" ) 
				m_sheet->m_only = true;
		}
		else if (symbol == _T("selectGROUPNCTRL") && !entry.IsEmpty())
		{
			m_sheet->selectGroup(atoi(entry));
			entry2  = parseX(datB, "\t");
			if ( entry2 != "NO" ) 
				m_sheet->m_only = true;
			
			entry3 = parseX(datB, "\t");
			
			if(entry3 != "SMALL")
				m_nType = 0;
			else
				m_nType = 1;
			
			m_bInterestCtrl = TRUE;
			
			m_sheet->SetControlFlag(m_bInterestCtrl,m_nType);
		}
		else if (symbol == _T("selectGROUPNCTRLNOT2000") && !entry.IsEmpty())
		{
			m_sheet->selectGroup(atoi(entry));
			entry2  = parseX(datB, "\t");
			if ( entry2 != "NO" ) 
				m_sheet->m_only = true;
			
			entry3 = parseX(datB, "\t");
			
			if(entry3 != "SMALL")
				m_nType = 0;
			else
				m_nType = 1;
			
			m_bInterestCtrl = TRUE;

			entry4 = parseX(datB, "\t");
			
			m_sheet->SetControlFlag(m_bInterestCtrl,m_nType);
			m_sheet->SetMapData(TRUE,entry4);
		}
		else if (symbol == _T("selectGROUPNCODE") && !entry.IsEmpty())
		{
			entry2  = parseX(entry, "\t");
			m_sheet->updateGroupNCode(atoi(entry2), entry);
			m_sheet->m_only = true;
		}
		if(symbol == _T("map") && !entry.IsEmpty())
		{
			m_bOpentoMap = true;
			if(entry == _T("1"))
			{
				m_sheet->m_only = true;
			}
		}
	}

	if (m_sheet->m_only) 
		m_sheet->RemovePageX();

	SearchGroupList();
}

////////////////////////////////////////////////////////////////////////////////////////
//
CString CMapWnd::parseX(CString &srcstr, CString substr)
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

CString CMapWnd::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

void CMapWnd::sendTR(CString trCode, char* datB, int datL, int key)
{
	auto sendB = std::make_unique<char[]>(L_userTH + datL + 1);

	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB.get();

	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB[L_userTH], datB, datL);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB.get());

#ifdef SAVELOGFILE
	CString strTemp;
	
	strTemp.Format("%s", datB);
	//testSaveFile3("[202202]sendTR", strTemp);
#endif
}

CFont* CMapWnd::GetAxFont(CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point; fontR.italic = false; fontR.bold = 0;

	switch (style)
	{
	case 0: case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_parent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

COLORREF CMapWnd::GetAxColor(UINT nIndex)
{
	if (nIndex & 0x02000000)
		return nIndex;

	return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)nIndex);
}

CString CMapWnd::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

void CMapWnd::GuideMsg(CString msg)
{
	MessageBox("["+msg+"]", "IBK투자증권");
//	Variant(guideCC, msg);
}

bool CMapWnd::GuideMsg(CString msg, bool question)
{
	UINT nType = MB_OK|MB_ICONEXCLAMATION;
	if (question)
		nType = MB_OKCANCEL|MB_ICONEXCLAMATION;

	if (MessageBox(msg, idTOstring(IDS_REGISTERINTER), nType) == IDCANCEL)
		return false;

	return true;
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	if (nIDEvent == idGUIDE && !m_guide.IsEmpty())
		Variant(guideCC, m_guide);
	m_guide = _T("");

	CWnd::OnTimer(nIDEvent);
}


void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, GetSysColor(COLOR_BTNFACE));
	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CMapWnd::ExistFile(CString fullfile)
{
	CFileFind finder;
	if (!finder.FindFile(fullfile))
		return false;
	
	return true;
}

void CMapWnd::initSaveFile(char* datB, bool isSequence)
{
	#define MAX_JM  120             /* 최대 관심그룹 갯수   */
	#define MAX_SIZE  4084            /* 최대 관심그룹 갯수   */

	struct  glist {
		char    ngrs[2];           /* New Group Seqn       */                                               
		char    ogrs[2];           /* New Group Seqn       */
		char    gnam[30];           /* Group Name           */
	};
	
	struct  grpfold {
		char    nrec[4];           /* Group Count          */
		struct  glist   glist[MAX_JM];
	};

////////////////////////////////////////////////////////////////////

	//CString saveFile, saveN, moveN, portFile, saveportN, savemoveN;
	//CString m_root, m_name;
	//m_root = Variant(homeCC, "");
	//m_name = Variant(nameCC, "");
	//
	//const int count = m_sheet->saveGroup();
	//const int groupCount = m_sheet->getManageCount();

	//CString port, tempPort, saveP, tempP;
	//tempPort.Format("%s", tempFILE);
	//port.Format("portfolio.ini");   //사용되지 않음
	//saveP.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, port);
	//tempP.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, tempPort);

	//if(!isSequence)
	//{
	//	if (ExistFile(tempP))
	//	{
	//		CopyFile(tempP, saveP, false);
	//		DeleteFile(tempP);
	//	}
	//}

	//
	////서버에서 받은 Output이 있으면 portfolio 갱신
	//if(datB != nullptr)
	//{
	//	struct grpfold* gFold;
	//	gFold = (struct grpfold *)(datB);

	//	CString oldFile, newFile, tempFile, oldTempFile, newTempFile;
	//	
	//	int nCount = 0;
	//	char nrec[4];
	//	
	//	CopyMemory(nrec, gFold->nrec, sizeof(gFold->nrec));
	//	
	//	nCount = atoi(nrec);
	//	struct glist list{};

	//	if(nCount > 0)
	//	{
	//		//page1의 관리 배열도 정렬시킨다
	//		if(isSequence == true)
	//		{
	//			m_sheet->setManageGroup(datB);
	//		}

	//		CString	string = _T(""), gnoS, oldnoS, gnameS, saveS, saveTempS, saveBookS, bookS;
	//		
	//		WritePrivateProfileString(_T("GROUPNAME"), nullptr, nullptr, saveP);
	//		
	//		//1베이스 이므로 save에 일단 복사
	//		for(int i=1 ; i<=groupCount ; i++)
	//		{
	//			saveS.Format("%s\\%s\\%s\\portfolio.i%02d.save", m_root, "user", m_name, i);  //사용되지 않음
	//			saveBookS.Format("%s\\%s\\%s\\bookmark.i%02d.save", m_root, "user", m_name, i);
	//			oldFile.Format("%s\\%s\\%s\\portfolio.i%02d", m_root, "user", m_name, i);  //사용되지 않음
	//			bookS.Format("%s\\%s\\%s\\bookmark.i%02d", m_root, "user", m_name, i);
	//				
	//			if(ExistFile(oldFile))
	//				CopyFile(oldFile, saveS, false);
	//			if(ExistFile(bookS))
	//				CopyFile(bookS, saveBookS, false);

	//			if(isSequence == true)
	//			{
	//				saveTempS.Format("%s\\%s\\%s\\portfolio.i%02d.savetmp", m_root, "user", m_name, i);  //사용되지 않음
	//				oldTempFile.Format("%s\\%s\\%s\\portfolio.i%02d.tmp", m_root, "user", m_name, i);  //사용되지 않음
	//				
	//				if(ExistFile(oldTempFile))
	//				CopyFile(oldTempFile, saveTempS, false);get
	//			}
	//		}

	//		
	//		for(int i=0 ; i<nCount ; i++)
	//		{
	//			memcpy(&list, &gFold->glist[i], sizeof(glist));
	//			
	//			gnoS.Format("%.2s", list.ngrs);
	//			oldnoS.Format("%.2s", list.ogrs);
	//			gnameS.Format("%.30s", list.gnam);
	//			gnameS.TrimRight();

	//			gnoS.TrimLeft(); gnoS.TrimRight();
	//			
	//			if(gnoS.IsEmpty() || atoi(gnoS) == 0) break;	//2015.04.09 KSJ 빈값이 들어오면 거기서 멈춘다.

	//			string += gnoS; 
	//			string += ";";
	//			
	//			
	//			oldFile.Format("%s\\%s\\%s\\portfolio.i%s.save", m_root, "user", m_name, oldnoS);
	//			newFile.Format("%s\\%s\\%s\\portfolio.i%s", m_root, "user", m_name, gnoS);
	//			
	//			oldTempFile.Format("%s\\%s\\%s\\portfolio.i%s.savetmp", m_root, "user", m_name, oldnoS);
	//			newTempFile.Format("%s\\%s\\%s\\portfolio.i%s.tmp", m_root, "user", m_name, gnoS);

	//			if(atoi(oldnoS) != atoi(gnoS))
	//			{
	//				if (ExistFile(oldFile))
	//				{	
	//					//save - > new
	//					CopyFile(oldFile, newFile, false);
	//					DeleteFile(oldFile);
	//				}
	//				

	//				//그룹 위치가 변경되었을때 tmp도 같이 바꿔주기
	//				if(isSequence == true)
	//				{
	//					if (ExistFile(oldTempFile))
	//					{	
	//						CopyFile(oldTempFile, newTempFile, false);
	//						DeleteFile(oldTempFile);
	//					}
	//				}

	//				//bookmark도 변경
	//				oldFile.Format("%s\\%s\\%s\\bookmark.i%s.save", m_root, "user", m_name, oldnoS);
	//				newFile.Format("%s\\%s\\%s\\bookmark.i%s", m_root, "user", m_name, gnoS);
	//				
	//				if (ExistFile(oldFile))
	//				{
	//					CopyFile(oldFile, newFile, false);
	//					DeleteFile(oldFile);
	//				}
	//			}

	//			newFile.Format("%s\\%s\\%s\\portfolio.i%s", m_root, "user", m_name, gnoS);
	//			
	//			if(ExistFile(newFile))
	//			{
	//				if(isSequence == true)
	//				{
	//					WritePrivateProfileString(_T("GROUPNAME"), gnoS, gnameS, tempP);
	//				}
	//				else
	//				{
	//					WritePrivateProfileString(_T("GROUPNAME"), gnoS, gnameS, saveP);
	//				}
	//			}

	//			
	//		}

	//		WritePrivateProfileString(_T("GROUPORDER"), "00", string, saveP);
	//		WritePrivateProfileString(nullptr, nullptr, nullptr, saveP);			

	//		CString dele, book, deleFile, deletempFile, delbookFile, tempdel;
	//		
	//		for(int i=1 ; i<=groupCount ; i++)
	//		{	
	//			saveS.Format("%s\\%s\\%s\\portfolio.i%02d.save", m_root, "user", m_name, i);
	//			saveTempS.Format("%s\\%s\\%s\\portfolio.i%02d.savetmp", m_root, "user", m_name, i);
	//			saveBookS.Format("%s\\%s\\%s\\bookmark.i%02d.save", m_root, "user", m_name, i);

	//			if(ExistFile(saveS))
	//			{
	//				DeleteFile(saveS);
	//			}
	//			if(ExistFile(saveTempS))
	//			{
	//				DeleteFile(saveTempS);
	//			}
	//			if(ExistFile(saveBookS))
	//			{
	//				DeleteFile(saveBookS);
	//			}
	//		}

	//		for (int i=nCount+1; i<=100; i++)
	//		{
	//			//count 보다 큰수의 파일은 삭제한다
	//			dele.Format("portfolio.i%02d", i);
	//			book.Format("bookmark.i%02d", i);
	//			deleFile.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, dele);
	//			delbookFile.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, book);

	//			if(ExistFile(deleFile))
	//			{
	//				DeleteFile(deleFile);
	//			}
	//			if(ExistFile(delbookFile))
	//			{
	//				DeleteFile(delbookFile);
	//			}
	//		}

	//		return;

	//	}
	//	else	//2015.04.09 KSJ datB값이 빈값이 아니면 전체삭제다.
	//	{
	//		CString	string = _T(""), gnoS, oldnoS, gnameS, saveS, saveTempS, saveBookS, bookS;
	//		
	//		for(int i=1 ; i<=groupCount ; i++)
	//		{
	//			saveS.Format("%s\\%s\\%s\\portfolio.i%02d.save", m_root, "user", m_name, i);
	//			saveBookS.Format("%s\\%s\\%s\\bookmark.i%02d.save", m_root, "user", m_name, i);
	//			oldFile.Format("%s\\%s\\%s\\portfolio.i%02d", m_root, "user", m_name, i);
	//			bookS.Format("%s\\%s\\%s\\bookmark.i%02d", m_root, "user", m_name, i);
	//			
	//			if(ExistFile(oldFile))
	//				CopyFile(oldFile, saveS, false);
	//			if(ExistFile(bookS))
	//				CopyFile(bookS, saveBookS, false);
	//			
	//			if(isSequence == true)
	//			{
	//				saveTempS.Format("%s\\%s\\%s\\portfolio.i%02d.savetmp", m_root, "user", m_name, i);
	//				oldTempFile.Format("%s\\%s\\%s\\portfolio.i%02d.tmp", m_root, "user", m_name, i);
	//				
	//				if(ExistFile(oldTempFile))
	//					CopyFile(oldTempFile, saveTempS, false);
	//			}
	//		}
	//		
	//		WritePrivateProfileString(_T("GROUPORDER"), "00", string, saveP);
	//		WritePrivateProfileString(nullptr, nullptr, nullptr, saveP);		
	//		
	//		CString dele, book, deleFile, deletempFile, delbookFile, tempdel;
	//		
	//		for (int i=nCount+1; i<=100; i++)
	//		{
	//			//count 보다 큰수의 파일은 삭제한다
	//			dele.Format("portfolio.i%02d", i);
	//			book.Format("bookmark.i%02d", i);
	//			deleFile.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, dele);
	//			delbookFile.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, book);
	//			
	//			if(ExistFile(deleFile))
	//			{
	//				DeleteFile(deleFile);
	//			}
	//			if(ExistFile(delbookFile))
	//			{
	//				DeleteFile(delbookFile);
	//			}
	//		}
	//		
	//		return;
	//	}
	//}


	//for (int ii=1; ii<=100; ii++)
	//{
	//	saveFile.Format("bookmark.i%02d.save", ii);
	//	saveN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, saveFile);
	//	if (ExistFile(saveN))
	//	{
	//		moveN = saveN.Left(saveN.GetLength()-5);
	//		CopyFile(saveN, moveN, false);
	//	}

	//	portFile.Format("portfolio.i%02d.save", ii);
	//	saveportN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, portFile);
	//	if (ExistFile(saveportN))
	//	{
	//		savemoveN = saveportN.Left(saveportN.GetLength()-5);
	//		CopyFile(saveportN, savemoveN, false);
	//	}
	//}

	//for (int ii=1; ii<=100; ii++)
	//{
	//	saveFile.Format("bookmark.i%02d.save", ii);
	//	saveN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, saveFile);

	//	if (ExistFile(saveN))
	//	{
	//		moveN = saveN.Left(saveN.GetLength()-5);
	//		DeleteFile(saveN);
	//		DeleteFile(moveN+".tmp");
	//	}

	//	portFile.Format("portfolio.i%02d.save", ii);
	//	saveportN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, portFile);
	//	
	//	if (ExistFile(saveportN))
	//	{
	//		moveN = saveportN.Left(saveportN.GetLength()-5);
	//		DeleteFile(saveportN);
	//		DeleteFile(moveN+".tmp");
	//	}

	//	//count 보다 큰수의 파일은 삭제한다
	//	if(ii>count) 
	//	{
	//		CString dele, deleFile;
	//		dele.Format("portfolio.i%02d", ii);
	//		deleFile.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, dele);
	//		
	//		if(ExistFile(deleFile))
	//		{
	//			DeleteFile(deleFile);
	//		}
	//			
	//	}
	//}
}

void CMapWnd::testSaveFile3(CString code, CString datB)
{
	CString strPath(_T("")), titletime;
	strPath.Format("%s\\2022test.dat", m_root);
	
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	
	CString strCurTime;
	strCurTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	
	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 	
	
	titletime.Format("[%s][%s][%s]\n", strCurTime, code, datB);
	
	file.WriteString(titletime);
	file.Close();	
}

void CMapWnd::SearchGroupList()
{
	m_arrGname.RemoveAll();
	int	sendL = 0;
	char	sendB[16 * 1024] = { 0, };
	int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';

	sendTR("PIDOMYST", sendB, sendL,  TRKEY_GROUPARR);
}

void CMapWnd::SearchGroupCode(int iseq, int iPageGubn)
{
	int index = iseq;
	int	sendL = 0;
	CString stmp;
	char	sendB[16 * 1024] = { 0, };
	int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';

	stmp.Format("%02d", index);
	memcpy((char*)&sendB[sz_uinfo], stmp, 2);
	sendTR("PIDOMYST", sendB, sendL, iPageGubn == MSG_SEARCH_GROUPCODE ? TRKEY_GROUPCODE : TRKEY_GROUPCODE_PAGE4);
}

void CMapWnd::DominoDataProcess()
{
	if (m_strDomino.IsEmpty())
	{
		m_sheet->selectGroup(0);
		return;
	}
	CString symbol, entry, mapp, entry2, entry3, entry4;
	while (!m_strDomino.IsEmpty())
	{
		symbol = parseX(m_strDomino, "\t");  
		entry = parseX(m_strDomino, "\n");    //실제 열어야 할 그룹이다

		if (symbol == _T("appendCODE"))
		{
			m_sheet->updateCode(entry);
			m_sheet->m_only = true;
		}
		else if (symbol == _T("registerCODE") && !entry.IsEmpty())
		{
			m_sheet->m_only = true;
		}
		else if (symbol == _T("appendGROUP") && !entry.IsEmpty())
		{
			m_sheet->updateGroup(entry);   //관심설정화면을 띄우면서 새그룹창이 뜬다 202202없다.
			m_sheet->m_only = false;
		}
		else if (symbol == _T("appendParentGROUP") && !entry.IsEmpty())
		{
			m_sheet->updateParentGroup(entry);
			m_sheet->m_only = false;
		}
		else if (symbol == _T("selectGROUP") && !entry.IsEmpty())
		{
			m_sheet->selectGroup(atoi(entry));
			entry2 = parseX(m_strDomino, "\t");
			if (entry2 != "NO")
				m_sheet->m_only = true;
		}
		else if (symbol == _T("selectGROUPNCTRL") && !entry.IsEmpty())
		{
			m_sheet->selectGroup(atoi(entry));
			entry2 = parseX(m_strDomino, "\t");
			if (entry2 != "NO")
				m_sheet->m_only = true;

			entry3 = parseX(m_strDomino, "\t");

			if (entry3 != "SMALL")
				m_nType = 0;
			else
				m_nType = 1;

			m_bInterestCtrl = TRUE;

			m_sheet->SetControlFlag(m_bInterestCtrl, m_nType);
		}
		else if (symbol == _T("selectGROUPNCTRLNOT2000") && !entry.IsEmpty())
		{
			m_sheet->selectGroup(atoi(entry));
			entry2 = parseX(m_strDomino, "\t");
			if (entry2 != "NO")
				m_sheet->m_only = true;

			entry3 = parseX(m_strDomino, "\t");

			if (entry3 != "SMALL")
				m_nType = 0;
			else
				m_nType = 1;

			m_bInterestCtrl = TRUE;

			entry4 = parseX(m_strDomino, "\t");

			m_sheet->SetControlFlag(m_bInterestCtrl, m_nType);
			m_sheet->SetMapData(TRUE, entry4);
		}
		else if (symbol == _T("selectGROUPNCODE") && !entry.IsEmpty())
		{
			entry2 = parseX(entry, "\t");
			m_sheet->updateGroupNCode(atoi(entry2), entry);
			m_sheet->m_only = true;
		}
		if (symbol == _T("map") && !entry.IsEmpty())
		{
			m_bOpentoMap = true;
			if (entry == _T("1"))
			{
				m_sheet->m_only = true;
			}
		}
	}

	if (m_sheet->m_only)
		m_sheet->RemovePageX();
}