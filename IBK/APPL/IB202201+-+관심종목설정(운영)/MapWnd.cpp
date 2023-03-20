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

#define TM_DOMINO       999
#define	idGUIDE		5003
#define SAVELOGFILE

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* parent)
{
	m_parent = parent;
	m_status = statRUN;
	m_autosave = false;
	m_nblary.RemoveAll();
	m_nblConfigary.RemoveAll();
	m_bOpentoMap = false;
	m_uploadCount = 0;
	m_isUploadEnd = false;
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
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(TOmapWnd, OnUpDnAction)
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
	const	int buttonW = 75, buttonH = 24, gap9 = 6, gap3 = 3;

	GetClientRect(&clientRC);

	m_sheet = std::make_unique<CSheet>("", m_parent);
	m_sheet->Create(this, WS_CHILD|WS_VISIBLE);
	m_sheet->SetWindowPos(nullptr, 0, 0, 0, 0, SWP_NOSIZE);

	//2016.08.18 KSJ ���ǻ��װ� ��ư �߰�
	btn1RC.left  = clientRC.left + 10;
	btn1RC.right = btn1RC.left + 600;
	btn1RC.top   = clientRC.bottom - (buttonH - gap3);
	btn1RC.bottom= btn1RC.top + buttonH;

	m_stSave.Create("�� ������� �� ������ ���Ͻø� \"����\"��, ����� ��� �Ϸ� �� �����Ͻ÷��� \"Ȯ��\"�� �����ֽʽÿ�.", WS_CHILD|WS_VISIBLE, btn1RC, this, IDC_STATIC);
	m_stSave.SetFont(font);

	btn1RC.left  = clientRC.right - (buttonW*3 + gap9*3);
	btn1RC.right = btn1RC.left + buttonW;
	btn1RC.top   = clientRC.bottom - (buttonH + gap3);
	btn1RC.bottom= btn1RC.top + buttonH;
	
	m_btnSave.Create(idTOstring(IDS_SAVE), WS_CHILD|WS_VISIBLE, btn1RC, this, IDRETRY);
	m_btnSave.SetFont(font);
	//2016.08.18 KSJ ���ǻ��װ� ��ư �߰� End

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

	//Backup�� ���� ���� ����
	uploadBackup();
	Request_GroupList();

	return 0;
}


void CMapWnd::OnClose() 
{
// 	if(m_bOpentoMap == false)
// 	{
// 		if(m_sheet->IsMenuStatusClosed())
// 		{
// 			AfxMessageBox("�޴��˻���â�� ���� �ݾ��ּ���");
// 			return;
// 		}
// 	}


	DestroyWindow(); 
	CWnd::OnClose();
}


void CMapWnd::OnDestroy() 
{
	if (m_sheet)
		m_sheet->DestroyWindow();

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


void CMapWnd::OnBtnOK()
{
	m_uploadCount = 0;
	m_isUploadEnd = false;
	m_bBtnSave = false;

	m_sheet->OkKillaction();
	m_sheet->OKSubtabaction();
	m_sheet->OKactionOne();
}

void CMapWnd::OnBtnCC()
{
	//�ʿ��� �������� ����Ȯ��
// 	if(m_bOpentoMap == false)
// 	{
// 		if(m_sheet->IsMenuStatusClosed())
// 		{
// 			AfxMessageBox("�޴��˻���â�� ���� �ݾ��ּ���");
// 			return;
// 		}
// 	}

	m_bBtnSave = false;
	uploadCancel();

// 	m_sheet->CCaction();
// 	m_parent->SendMessage(WM_USER, closeDLL, 0);
}

void CMapWnd::OnBtnSave()
{
	m_uploadCount = 0;
	m_isUploadEnd = false;
	m_bBtnSave = true;
	
	m_sheet->OkKillaction();
	m_sheet->OKSubtabaction(1);  
	m_sheet->OKactionOne();
}

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	if (!GetSafeHwnd())
		return  0;

	const int datL = HIWORD(wParam);
	//const int key  = HIBYTE(LOWORD(wParam));
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		switch (HIBYTE(LOWORD(wParam)))
		{
		case upEndACTION:
			endOubs((char *) lParam, HIWORD(wParam));
			break;
		case upOKACTION:
			OKOubs((char *) lParam, HIWORD(wParam));
			break;
		case upEachGroupACTION:
			break;
		case upACTION:
			parsingOubsUP((char *) lParam, HIWORD(wParam));
			break;
		case upsequenceACTION:
			sequenceOubsUP((char *) lParam, HIWORD(wParam));
			break;
		case upnewgroupACTION:
			break;
		case upBackupACTION:
			backupOubsUP((char *) lParam, HIWORD(wParam));
			break;
		case upRecoveryACTION:	
			recoveryOubsUP((char *) lParam, HIWORD(wParam));
			break;
		case upCancelACTION:
			cancelOubsUP((char *) lParam, HIWORD(wParam));
			break;
		case dnACTIONR:
			recoveryOubsUP((char *) lParam, HIWORD(wParam));
			break;
		case dnACTION: case dnACTIONX:	
			m_sheet->EnablePage1Btn(TRUE);	// 20070620
			parsingOubsDN((char *) lParam, HIWORD(wParam));
			break;
		case ELWIssueDndnACTION:
			m_sheet->sendELWIssueDN((char *) lParam, HIWORD(wParam));
			break;
		case ELWAssetDndnACTION:
			m_sheet->sendELWAssetDN((char *) lParam, HIWORD(wParam));
			break;
		case RecommnadDnACTION:
			m_sheet->sendRecommandDN((char *) lParam, HIWORD(wParam));
			break;
		case dnGROUPLIST:
			m_sheet->receiveOub(dnGROUPLIST, CString((char*)lParam, datL));
			_binitGroup = true;
			break;
		case dnGROUPCODE:
			m_sheet->receiveOub(dnGROUPCODE, CString((char*)lParam, datL));
			break;
		default:
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

long CMapWnd::OnUpDnAction(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case upACTION:
		updnWaitAction(0);
		uploadAction(true);
		break;
	case upsequenceACTION:
		uploadSequence();
		break;
	case upnewgroupACTION:
		uploadNewGroup();
		break;
	case upEachGroupACTION:
		uploadEachGroup(HIWORD(wParam));
		break;
	case upEndACTION:
		uploadEndGroup(HIWORD(wParam));
		break;
	case upOKACTION:
		uploadOK();
		break;
	case dnACTION:
		m_sheet->EnablePage1Btn(FALSE);	// 20070620
		updnWaitAction(0);
		dnloadAction();
		break;
	case dnACTIONX:
		m_sheet->EnablePage1Btn(FALSE);	// 20070620
		updnWaitAction(0);
		dnloadActionX();
		break;
	case dnACTIONR:
//		m_sheet->EnablePage1Btn(FALSE);	// 20070620
// 		updnWaitAction(0);
// 		dnloadActionR();
		uploadRecovery();
		break;

	case ssACTION:
		sendTR("pooppoop", (char *)lParam, HIWORD(wParam), ssACTION);
		break;
	case ELWIssueDndnACTION:
		elwIssueDnLoadAction(HIWORD(wParam));
		break;
	case ELWAssetDndnACTION:
		elwAssetDnLoadAction(HIWORD(wParam), (CHAR*)lParam);
		break;
	case RecommnadDnACTION:
		recommandDnLoadAction();
		break;
	case dnGROUPCODE:
		Request_GroupCode(lParam);
		break;
	}

	return 0;
}

void CMapWnd::uploadAction(bool init)
{
	int	sendL = 0;
	char	tempB[32]{};
	std::string sendB;
	sendB.resize(16 * 1024);
	int	count{};
	CString	gnox, gnam, gidx;


	if (m_nblc <= 0)
	{
		TRACE("upload file not found.....\n");
		updnWaitAction(1); 
		return;
	}

	gnox = m_nblary.GetAt(0); 
	m_nblary.RemoveAt(0);
	gidx = m_gidary.GetAt(0); 
	m_gidary.RemoveAt(0);

	struct _updn* updn = (struct _updn *)&sendB[sendL];
	sendL += sz_updn;

	FillMemory((char *) updn, sz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = init ? 'F' : 'M';

	sprintf(tempB, "%05d", m_nblc);
	CopyMemory(updn->uinfo.nblc, tempB, sizeof(updn->uinfo.nblc));

	updn->uinfo.retc[0] = 'O';

	count = m_sheet->GetUploadData(atoi(gnox), gnam, &sendB[sendL]);
	sendL += count * sz_jinfo;


//	m_sheet->saveGroup(atoi(gidx)+1, gnam);

	CopyMemory(updn->ginfo.gnox, gidx.operator LPCTSTR(), sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, gnam.operator LPCTSTR(), min(sizeof(updn->ginfo.gnam), gnam.GetLength()));
	sprintf(tempB, "%04d", count); 
	CopyMemory(updn->ginfo.jrec, tempB, sizeof(updn->ginfo.jrec));

	sendB[sendL] = 0x00; 
	sendTR(trUPDOWN, sendB.data(), sendL, upACTION);
}

void	CMapWnd::sequenceOubsUP(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{
		TRACE("upload : response length error....[%d]\n", datL);
		updnWaitAction(1); 
		return;
	}

	struct	_updn2*	updn = (struct _updn2 *)datB;	
	if (updn->uinfo.retc[0] == 'E')
	{
		struct	_uinfo	infoC;
		FillMemory(&infoC, sizeof(_uinfo), ' ');
		
		CopyMemory(&infoC.gubn[0], "MY", sizeof(infoC.gubn));
		infoC.dirt[0] = 'U';
		infoC.cont[0] = 'C';		// ���
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, 0);
		return;
	}

	initSaveFile((char *)(datB+sz_uinfo));
	m_sheet->initCombo();
}


void CMapWnd::recoveryOubsUP(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{
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
		infoC.cont[0] = 'C';		// ���
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, 0);
		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));
	}

	FillMemory((char*)uinfo, sizeof(_uinfo), ' ');
	
	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'D';
	uinfo->cont[0] = 'F';
	CopyMemory(uinfo->nblc, "00000", sizeof(uinfo->nblc));
	
	sendTR(trUPDOWN, (char*)uinfo, sz_uinfo, dnACTION);
	
}

void CMapWnd::cancelOubsUP(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{

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
		infoC.cont[0] = 'C';		// ���
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, 0);
		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));
		
	}
	
//	m_sheet->recoveryFile();
//	m_sheet->deleteBackupFile();
	m_sheet->CCaction();	
	m_parent->SendMessage(WM_USER, closeDLL, 0);

}

void CMapWnd::endOubs(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{
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
		infoC.cont[0] = 'C';		// ���
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, 0);
		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));
	}
	
//	m_sheet->deleteBackupFile();
	uploadOK();
}


void CMapWnd::OKOubs(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{

		TRACE("upload : response length error....[%d]\n", datL);
		updnWaitAction(1); 
		return;
	}
	
	struct	_uinfo*	uinfo = (struct _uinfo *)datB;	
	if (uinfo->retc[0] == 'E')
	{
		struct	_uinfo	infoC;
		FillMemory(&infoC, sizeof(_uinfo), ' ');
		
		CopyMemory(&infoC.gubn[0], "MY", sizeof(infoC.gubn));
		infoC.dirt[0] = 'U';
		infoC.cont[0] = 'C';		// ���
		

		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, 0);
		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));
	}
	
	CString string; 
	string = "OnPortfolio\tok";
	m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);

	if(m_bBtnSave)		uploadBackup();						//2016.10.27 KSJ �����ư ���������� �������� 'b' �����ֵ��� ����
	else			m_parent->SendMessage(WM_USER, closeDLL, 0);		//2016.08.18 KSJ �����ư �߰�
}


void	CMapWnd::backupOubsUP(char* datB, int datL)
{
	if (datL < sz_uinfo)
	{
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
		infoC.cont[0] = 'C';		// ���
		
		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, 0);
		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));
	}
}


void CMapWnd::parsingOubsUP(char* datB, int datL)
{
//////////////////////////////////////////////////////////////////////////////////////
	if (datL < sz_uinfo)
	{

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
		infoC.cont[0] = 'C';		// ���


		sendTR(trUPDOWN, (char*)&infoC, sz_uinfo, 0);
		TRACE("upload : response error....[%s]\n", CString(uinfo->emsg, sizeof(uinfo->emsg)));


		return;
	}
	
	int sizeU = strlen((char*)uinfo);
	const int sizeDatB = strlen(datB);

/*	if (m_nblary.GetSize() <= 0)		// last ó��
	{
		struct	_uinfo	infoL;
		FillMemory(&infoL, sizeof(_uinfo), ' ');

		CopyMemory(&infoL.gubn[0], "MY", sizeof(infoL.gubn));
		infoL.dirt[0] = 'U';
		infoL.cont[0] = 'L';
TRACE("kwon L\n");
		sendTR(trUPDOWN, (char*)&infoL, sz_uinfo, 0);
		m_updown = 0; 
		updnWaitAction(1);
		if (!m_autosave) GuideMsg(idTOstring(IDS_GUIDE6));


		initSaveFile();
		return;
	}

	uploadAction();
*/

//	m_sheet->OKactionOne(m_uploadCount);
//	uploadActionGroup(false);
	
	m_sheet->OKaction();



	if(sizeDatB > sizeU)
	{
		initSaveFile((char *)(datB+sizeU));
	}
	else
	{
		initSaveFile();
	}

	CString string; 
	string = "OnPortfolio\tok";
	m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
	m_parent->SendMessage(WM_USER, closeDLL, 0);


}

//__int64 getFileSize(CString filename) 
//{
//	struct _stati64 statbuf {};
//	
//	char* fname = new char [filename.GetLength() + 1];
//
//	strcpy(fname, filename);
//	
//	if ( _stati64(fname, &statbuf) ) 
//		return -1;							// ���� ���� ���: ���� ������ -1 �� ��ȯ
//	
//	delete [] fname;
//	return statbuf.st_size;					// ���� ũ�� ��ȯ
//}


void CMapWnd::uploadSequence()
{

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

	for(int i=0; i< count ; i++)
	{
		ogrs = m_sheet->getManageGroupdata(i,0);		//old
		mode = m_sheet->getManageGroupdata(i,1);		//mode (D : ����) N : �׷� �߰� 
		ngrs.Format("%02d",j+1);				//new
		gnam = m_sheet->getManageGroupdata(i,3);		//name

		if(mode != "D")
		{
			if(gnam.IsEmpty())		//2015.03.23 KSJ 2016.11.16 KSJ �׷��̸��� ������ �����°� �ƴ϶� ���� �̸��� ����� �ش�.
			{
				MessageBox("������������ ������ �߻��Ͽ����ϴ�. ����ȭ���� �ٽ� ���� �ֽñ�ٶ��ϴ�.", "���������� ����");

				OnClose();
				gnam.Format("GROUP%03d", j);
				//break;
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

	sendTR(trUPDOWN, (char*)&updn2, sz_updn2, upsequenceACTION);
}

void CMapWnd::uploadNewGroup()
{
	
	int	sendL = 0;
	std::string	sendB;
	sendB.resize(1024 * 16);
	char tempB[32]{};
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

	for(int i=0; i<m_sheet->getManageCount() ; i++)
	{
		ogrs = m_sheet->getManageGroupdata(i,0);		//old
		mode = m_sheet->getManageGroupdata(i,1);		//mode (D : ����) N : �׷� �߰� 
		gnam = m_sheet->getManageGroupdata(i,3);		//name

		if(mode == "N")
		{
			count = m_sheet->GetUploadData(atoi(ogrs), gnam, &sendB[sendL]);
			sendL += count * sz_jinfo;
		}
	}

	CopyMemory(updn->ginfo.gnox, "-1", sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, gnam.operator LPCTSTR(), min(sizeof(updn->ginfo.gnam), gnam.GetLength()));
	sprintf(tempB, "%04d", count); 
	CopyMemory(updn->ginfo.jrec, tempB, sizeof(updn->ginfo.jrec));


	sendB[sendL] = 0x00; 
	sendTR(trUPDOWN, sendB.data(), sendL, upnewgroupACTION);

}

void CMapWnd::uploadEndGroup(int gno)
{
	int	sendL = 0;
	char	tempB[32]{};
	std::string	sendB;
	sendB.resize(1024 * 16);

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
	if (m_sheet->_ball)
		CopyMemory(updn->uinfo.name, "ALL", 3);

	int count = 0;

	ogrs = m_sheet->getManageGroupdata(gno, 0);		//old
	mode = m_sheet->getManageGroupdata(gno, 1);		//mode (D : ����) N : �׷� �߰� 
	gnam = m_sheet->getManageGroupdata(gno, 3);		//name

	count = m_sheet->GetUploadData(atoi(ogrs), gnam, &sendB[sendL]);
	

	sendL += count * sz_jinfo;

	CopyMemory(updn->ginfo.gnox, ogrs.operator LPCTSTR(), sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, gnam.operator LPCTSTR(), min(sizeof(updn->ginfo.gnam), gnam.GetLength()));
	sprintf(tempB, "%04d", count);
	CopyMemory(updn->ginfo.jrec, tempB, sizeof(updn->ginfo.jrec));


	sendB[sendL] = 0x00;
	sendTR(trUPDOWN, sendB.data(), sendL, upEndACTION);
}

void	CMapWnd::uploadOK()
{	
	int	sendL = 0;
	char	tempB[32]{};
	std::string	sendB;
	sendB.resize(1024 * 16);
	
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
	sendTR(trUPDOWN, sendB.data(), sendL, upOKACTION);
}


// �׷�� ���񸮽�Ʈ ���ε�...
void CMapWnd::uploadEachGroup(int gno)
{	
	int	sendL = 0;
	char	tempB[32]{};
	std::string	sendB;
	sendB.resize(1024 * 16);
	
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
	
	ogrs = m_sheet->getManageGroupdata(gno,0);		//old
	mode = m_sheet->getManageGroupdata(gno,1);		//mode (D : ����) N : �׷� �߰� 
	gnam = m_sheet->getManageGroupdata(gno,3);		//name
	
	count = m_sheet->GetUploadData(atoi(ogrs), gnam, &sendB[sendL]);

	if (count == 0)
		return;

	sendL += count * sz_jinfo;
		
	CopyMemory(updn->ginfo.gnox, ogrs.operator LPCTSTR(), sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, gnam.operator LPCTSTR(), min(sizeof(updn->ginfo.gnam), gnam.GetLength()));
	sprintf(tempB, "%04d", count); 
	CopyMemory(updn->ginfo.jrec, tempB, sizeof(updn->ginfo.jrec));
	
	
	sendB[sendL] = 0x00; 
	sendTR(trUPDOWN, sendB.data(), sendL, upEachGroupACTION);
}

void	CMapWnd::uploadBackup()
{
	int	sendL = 0;
	char	tempB[32]{};
	std::string	sendB;
	sendB.resize(1024 * 16);

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
	sendTR(trUPDOWN, sendB.data(), sendL, upBackupACTION);
}

void	CMapWnd::uploadCancel()
{	
	int	sendL = 0;
	char	tempB[32]{};
	std::string	sendB;
	sendB.resize(1024 * 16);


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
	sendTR(trUPDOWN, sendB.data(), sendL, upCancelACTION);
}

void	CMapWnd::uploadRecovery()
{
	int	sendL = 0;
	char	tempB[32]{};
	std::string	sendB;
	sendB.resize(1024 * 16);
	
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
	sendTR(trUPDOWN, sendB.data(), sendL, upRecoveryACTION);
}

//void CMapWnd::uploadActionGroup(bool init)
//{
//	int	sendL = 0;
//	std::string	sendB;
//	sendB.resize(1024 * 16);
//	char	strUinfo[500];
//
////	int	count;
//	CString	gnox, gnam, gidx;
//
////	int	gid;
//	CString	string, stringx, tmpx, configx;
//	
//	CString m_root = Variant(homeCC, "");
//	CString m_name = Variant(nameCC, "");
//	CString saveFile, tempFile;
//	CString tmp;
//	CString saveN, tempN, tempOrigin;
//
//	//������ �׷츸 ����ǵ��� ����	
//	CString m_manageGroup, m_manageConfig;
//	m_manageGroup = "";
//	m_manageConfig = "";
//	m_manageGroup = m_sheet->GetGroupORder2(m_manageConfig);
//	
//	ZeroMemory(sendB.data(), 16*1024);
//	ZeroMemory(strUinfo, 500);
//	struct _uinfo* uinfo = (struct _uinfo *)strUinfo;
//
//	FillMemory((char *)uinfo, sz_uinfo, ' ');
//	
//	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
//	uinfo->dirt[0] = 'U';
//	uinfo->cont[0] = 'E';
//	
//	uinfo->retc[0] = 'O';	
//
//	int nCount = 0;
//	//int nSize = 0;
//	tmp = "";
//
//	//������ ���ɱ׷���� ���� ������ �� ������ ���ε�
//	while (!m_manageGroup.IsEmpty())
//	{
//		stringx = parseX(m_manageGroup, ";");
//		configx = parseX(m_manageConfig, ";");
//
//		tempFile.Format("portfolio.i%s.tmp", stringx);
//		tempN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, tempFile);
//
//		tempOrigin.Format("%s\\%s\\%s\\portfolio.i%s", m_root, "user", m_name, stringx);
//	
//
//		//temp�� �ִ��� Ȯ��
//		if (ExistFile(tempN))
//		{
//				const int size1 = gsl::narrow_cast<int>(getFileSize(tempN));
//				const int size2 = gsl::narrow_cast<int>(getFileSize(tempOrigin));
//				
//				//������ ������ ��
//				if(size1 != size2)
//				{
//					if(!configx.IsEmpty())
//						tmp += send(stringx, configx);
//					else
//						tmp += send(stringx, "G");
//					
//					nCount++;
//				}
//				else
//				{
//					if(!configx.IsEmpty())
//					{
//						tmp += send(stringx, configx);
//
//						nCount++;
//					}
//				}
//		}
//		//temp������ N, D�� �ִ��� Ȯ��
//		else
//		{
//			if(!configx.IsEmpty())
//			{
//				tmp += send(stringx, configx);
//
//				nCount++;
//			}
//		}
//
//	}
//
//	//������ �׷� ����
//	CString tmpCount;
//	tmpCount.Format("%05d", nCount);
//	CopyMemory(uinfo->nblc, tmpCount, sizeof(uinfo->nblc));
//	
//	CString temp2;
//	temp2.Format("%s%s", strUinfo, tmp);
//
//	sendL = strlen(temp2);
//	CopyMemory(sendB.data(), temp2, sendL-1);
//
//
//	sendB[sendL] = 0x00; 
//
//	sendTR(trUPDOWN, sendB.data(), sendL, upACTION);
//
//}

// CString CMapWnd::send(CString sqen, CString type)
// {	
// XMSG("���� ??");
// 	CString result, jinfo;
// 	result = "";
// 	int count = 0;
// 	//UINT	datL = 0;
// 	char	tempB[4];

// 	CString	gnam, temp;

// 	struct _foldex foldex;
	
// 	ZeroMemory(&foldex, sz_foldex);
// 	ZeroMemory(tempB, 4);
	

// 	memset(foldex.gubn, ' ', sizeof(foldex.gubn));
// 	memset(foldex.kjid, ' ', sizeof(foldex.kjid));
// 	memset(foldex.name, ' ', sizeof(foldex.name));
// 	memset(foldex.nrec, ' ', sizeof(foldex.nrec));

// 	jinfo = m_sheet->GetUploadData2(atoi(sqen), gnam, count);

// 	if(type == "M" || type == "N")
// 	{
// 		CopyMemory(foldex.gubn, "G", sizeof(foldex.gubn));
// 	}
// 	else
// 	{
// 		CopyMemory(foldex.gubn, type.operator LPCTSTR(), sizeof(foldex.gubn));
// 	}
	
// 	if(type == "N")
// 	{
// 		CopyMemory(foldex.kjid, "-1", sizeof(foldex.kjid));
// 	}
// 	else
// 	{
// 		CopyMemory(foldex.kjid, sqen.operator LPCTSTR(), sizeof(foldex.kjid));
// 	}
	
// 	CopyMemory(foldex.name, gnam.operator LPCTSTR(), min(sizeof(foldex.name), gnam.GetLength()));
// 	sprintf(tempB, "%04d", count);
// 	CopyMemory(foldex.nrec, tempB, sizeof(foldex.nrec));

// 	temp = "";

// 	temp += CString(foldex.gubn, sizeof(foldex.gubn));
// 	temp += CString(foldex.kjid, sizeof(foldex.kjid));
// 	temp += CString(foldex.name, sizeof(foldex.name));
// 	temp += CString(foldex.nrec, sizeof(foldex.nrec));

// 	result.Format("%s~%s|", temp, jinfo);

// 	return result;
// }

void CMapWnd::dnloadAction()
{

	struct	_uinfo	uinfo;

	FillMemory((char*) &uinfo, sizeof(_uinfo), ' ');

	CopyMemory(uinfo.gubn, "MY", sizeof(uinfo.gubn));
	uinfo.dirt[0] = 'D';
	uinfo.cont[0] = 'F';
	CopyMemory(uinfo.nblc, "00000", sizeof(uinfo.nblc));

	sendTR(trUPDOWN, (char*)&uinfo, sz_uinfo, dnACTION);
}

void CMapWnd::recommandDnLoadAction()
{

	struct  mid 
	{
		char    gubn[ 1];   /*  ����('0':���, '1':����)    */
							/*  '2': �����ڵ帮��Ʈ         */
	};
	
	struct mid mid;
	FillMemory(&mid, sizeof(mid), ' ');	
	
	CopyMemory(&mid.gubn, _T("2"), sizeof(mid.gubn));

	sendTR("pihorsdt", (char*)&mid, sizeof(mid), RecommnadDnACTION);

}


void CMapWnd::elwIssueDnLoadAction(int selItem)
{
	struct mid
	{
		char    flag[1]{};
		char    ascf[1]{};            // 0: ���� 1: ����  
		char    kcod[1]{};            //�Ǹ����� code    
		char    gcod[12]{};           // �����ڻ� code    
		char    bcod[12]{};           // ����� code      
		char    jjis[1]{};            // �����ϼ�        
		char    sort[2]{};            // Sorting ����     
		char    rtsf[9]{};            /// 1. RTS  0: NOT
		char    fill[1]{};            // dummy          
		char    gvol[1]{};            // 1. gvol > 0   
		char    gsrt[1]{};            // 1: �׸������  
	};
	
	//������ Key���� �������� �ڵ尪 ��������
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
[[gsl::suppress(6385)]]
	CopyMemory(&mid.rtsf, _T("01"), sizeof(mid.rtsf));
	CopyMemory(&mid.fill, _T(" "), sizeof(mid.fill));
	CopyMemory(&mid.gvol, _T(" "), sizeof(mid.gvol));
	CopyMemory(&mid.gsrt, _T(" "), sizeof(mid.gsrt));
	
	sendTR("PIBO2805", (char*)&mid, sizeof(mid), ELWIssueDndnACTION);
}

void CMapWnd::elwAssetDnLoadAction(int selItem, CString item)
{

	struct mid
	{
		char    flag[1];
		char    ascf[1];            // 0: ���� 1: ����  
		char    kcod[1];            //�Ǹ����� code    
		char    gcod[12];           // �����ڻ� code    
		char    bcod[12];           // ����� code      
		char    jjis[1];            // �����ϼ�        
		char    sort[2];            // Sorting ����     
		char    rtsf[9];            /// 1. RTS  0: NOT
		char    fill[1];            // dummy          
		char    gvol[1];            // 1. gvol > 0   
		char    gsrt[1];            // 1: �׸������  
	};
	
	//������ ǲ���� Ȯ��
	CString kcod;
	if(item == "��")
	{
		kcod = _T("1");
	}
	else if(item == "ǲ")
	{
		kcod = _T("2");
	}
	else
	{
		kcod = _T(" ");
	}

	//������ Key���� �������� �ڵ尪 ��������
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
[[gsl::suppress(6385)]]
	CopyMemory(&mid.rtsf, _T("01"), sizeof(mid.rtsf));
	CopyMemory(&mid.fill, _T(" "), sizeof(mid.fill));
	CopyMemory(&mid.gvol, _T(" "), sizeof(mid.gvol));
	CopyMemory(&mid.gsrt, _T(" "), sizeof(mid.gsrt));
	
	sendTR("PIBO2805", (char*)&mid, sizeof(mid), ELWAssetDndnACTION);
}

void CMapWnd::dnloadActionX()
{

	struct	_uinfo	uinfo;

	FillMemory((char*) &uinfo, sizeof(_uinfo), ' ');

	CopyMemory(uinfo.gubn, "MY", sizeof(uinfo.gubn));
	uinfo.dirt[0] = 'X';
	uinfo.cont[0] = 'F';
	CopyMemory(uinfo.nblc, "00000", sizeof(uinfo.nblc));

	sendTR(trUPDOWN, (char*)&uinfo, sz_uinfo, dnACTION);
}


void CMapWnd::dnloadActionR()
{
	struct	_uinfo	uinfo;

	FillMemory((char*) &uinfo, sizeof(_uinfo), ' ');
	CopyMemory(uinfo.gubn, "MY", sizeof(uinfo.gubn));
	uinfo.dirt[0] = 'R';
	uinfo.cont[0] = 'F';
	CopyMemory(uinfo.nblc, "00000", sizeof(uinfo.nblc));

	sendTR(trUPDOWN, (char*)&uinfo, sz_uinfo, dnACTION);
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

		sendTR(trUPDOWN, (char *)&uinfo, sz_uinfo, 0);
		return;
	}
	updnWaitAction(2); 
	m_guide = idTOstring(IDS_GUIDE6); 

	((CWnd*)(m_sheet->GetParent()->GetDlgItem(IDOK)))->EnableWindow(TRUE);	//2016.07.12 KSJ ���� ��ư ������ ���� ������ Ȯ�ι�ư�� true ���ش�.

//	SetTimer(idGUIDE, 100, nullptr);
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
	//	m_sheet->reInitInterest();
	case 1:			// error & run
		m_status = statRUN;
		m_sheet->enableButton(TRUE);
		break;
	}
}

void CMapWnd::parsingDomino(CString datB)
{
	//2012.02.10 KSJ 
	// appendITEMS\t�׷��ȣ\t�ڵ�\t�ڵ�.....
	// �׷��ȣ�� 1������ ���� ���׷��̸� 0
	// ���׷��̸� ���Ƿ� �׷��̸��� ���ؼ� �־��ش�.
	SetTimer(TM_DOMINO, 50, nullptr);
	_sDomino = datB;
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
	std::string sendB;
	sendB.resize(L_userTH + datL + 1);
	struct	_userTH* uTH = (struct _userTH *) sendB.data();

	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB[L_userTH], datB, datL);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB.data());
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
	MessageBox("["+msg+"]", "IBK��������");
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
	else if (nIDEvent == TM_DOMINO)
	{
		if (_binitGroup)
		{
			CString symbol, entry, mapp, entry2, strGroup;
			while (!_sDomino.IsEmpty())
			{
				symbol = parseX(_sDomino, "\t");
				entry = parseX(_sDomino, "\n");

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
					m_sheet->updateGroup(entry);
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
					entry2 = parseX(_sDomino, "\t");
					if (entry2 != "NO")
						m_sheet->m_only = true;
				}
				else if (symbol == _T("selectGROUPNCODE") && !entry.IsEmpty())
				{
					entry2 = parseX(entry, "\t");
					m_sheet->updateGroupNCode(atoi(entry2), entry);
					m_sheet->m_only = true;
				}
				// 2012.02.10 KSJ
				else if (symbol == _T("appendITEMS") && !entry.IsEmpty())
				{
					strGroup = parseX(entry, "\t");
					m_sheet->appendITEMS(atoi(strGroup), entry);
					m_sheet->m_only = true;
				}
				// KSJ
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
			_sDomino.Empty();
		}
		else
		{
			SetTimer(TM_DOMINO, 50, nullptr);
		}
	}

		
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
	#define MAX_JM    120             /* �ִ� ���ɱ׷� ����   */
	#define MAX_SIZE  4084            /* �ִ� ���ɱ׷� ����   */

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

	CString saveFile, saveN, moveN, portFile, saveportN, savemoveN;
	CString m_root, m_name;
	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");
	
	
	//�������� ���� Output�� ������ portfolio ����
	if(datB != nullptr)
	{
		struct grpfold* gFold;
		gFold = (struct grpfold *)(datB);

		CString oldFile, newFile, tempFile, oldTempFile, newTempFile;
		
		char nrec[4];
		CopyMemory(nrec, gFold->nrec, sizeof(gFold->nrec));
		
		const int nCount = atoi(nrec);
		struct glist list {};

		if(nCount > 0)
		{
			m_sheet->setManageGroup(datB);
			const int groupCount = m_sheet->getManageCount();


			CString	string = _T(""), gnoS, oldnoS, gnameS, saveS, saveTempS, saveBookS, bookS;			
			//1���̽� �̹Ƿ� save�� �ϴ� ����
			for(int i=1 ; i<= groupCount ; i++)
			{
//				saveS.Format("%s\\%s\\%s\\portfolio.i%02d.save", m_root, "user", m_name, i);
				saveBookS.Format("%s\\%s\\%s\\bookmark.i%02d.save", m_root, "user", m_name, i);
//				oldFile.Format("%s\\%s\\%s\\portfolio.i%02d", m_root, "user", m_name, i);
				bookS.Format("%s\\%s\\%s\\bookmark.i%02d", m_root, "user", m_name, i);
					
				//if(ExistFile(oldFile))
				//	CopyFile(oldFile, saveS, false);
				if(ExistFile(bookS))
					CopyFile(bookS, saveBookS, false);

				//if(isSequence == true)
				/*{
					saveTempS.Format("%s\\%s\\%s\\portfolio.i%02d.savetmp", m_root, "user", m_name, i);
					oldTempFile.Format("%s\\%s\\%s\\portfolio.i%02d.tmp", m_root, "user", m_name, i);
					
					if(ExistFile(oldTempFile))
					CopyFile(oldTempFile, saveTempS, false);
				}*/
			}
	
			for(int i= 0 ; i< nCount ; i++)
			{
				memcpy(&list, &gFold->glist[i], sizeof(glist));
				
				gnoS.Format("%.2s", list.ngrs);
				oldnoS.Format("%.2s", list.ogrs);
				gnameS.Format("%.30s", list.gnam);
				gnameS.TrimRight();

				gnoS.TrimLeft(); gnoS.TrimRight();

				if(gnoS.IsEmpty() || atoi(gnoS) == 0) break;	//2015.04.09 KSJ ���� ������ �ű⼭ �����.

				string += gnoS; 
				string += ";";
				
				
				//oldFile.Format("%s\\%s\\%s\\portfolio.i%s.save", m_root, "user", m_name, oldnoS);
				//newFile.Format("%s\\%s\\%s\\portfolio.i%s", m_root, "user", m_name, gnoS);
				
			//	oldTempFile.Format("%s\\%s\\%s\\portfolio.i%s.savetmp", m_root, "user", m_name, oldnoS);
			//	newTempFile.Format("%s\\%s\\%s\\portfolio.i%s.tmp", m_root, "user", m_name, gnoS);

				if(atoi(oldnoS) != atoi(gnoS))
				{
					//if (ExistFile(oldFile))
					//{	
					//	//save - > new
					//	CopyFile(oldFile, newFile, false);
					//	DeleteFile(oldFile);
					//}
					//

					////�׷� ��ġ�� ����Ǿ����� tmp�� ���� �ٲ��ֱ�
					//if(isSequence == true)
					//{
					//	if (ExistFile(oldTempFile))
					//	{	
					//		CopyFile(oldTempFile, newTempFile, false);
					//		DeleteFile(oldTempFile);
					//	}
					//}

					//bookmark�� ����
					oldFile.Format("%s\\%s\\%s\\bookmark.i%s.save", m_root, "user", m_name, oldnoS);
					newFile.Format("%s\\%s\\%s\\bookmark.i%s", m_root, "user", m_name, gnoS);
					
					if (ExistFile(oldFile))
					{
						CopyFile(oldFile, newFile, false);
						DeleteFile(oldFile);
					}
				}

				//newFile.Format("%s\\%s\\%s\\portfolio.i%s", m_root, "user", m_name, gnoS);
				
				//if(ExistFile(newFile))
				//{
				//	if(isSequence == true)
				//	{
				//		WritePrivateProfileString(_T("GROUPNAME"), gnoS, gnameS, tempP);
				//	}
				//	else
				//	{
				//		WritePrivateProfileString(_T("GROUPNAME"), gnoS, gnameS, saveP);
				//	}
				//}

				
			}

			//WritePrivateProfileString(_T("GROUPORDER"), "00", string, saveP);
			//WritePrivateProfileString(nullptr, nullptr, nullptr, saveP);			


			CString dele, book, deleFile, deletempFile, delbookFile, tempdel;
			
			for(int i=1 ; i<=groupCount ; i++)
			{	
				//saveS.Format("%s\\%s\\%s\\portfolio.i%02d.save", m_root, "user", m_name, i);
				//saveTempS.Format("%s\\%s\\%s\\portfolio.i%02d.savetmp", m_root, "user", m_name, i);
				saveBookS.Format("%s\\%s\\%s\\bookmark.i%02d.save", m_root, "user", m_name, i);

				/*
				if(ExistFile(saveS))
				{
					DeleteFile(saveS);
				}
				if(ExistFile(saveTempS))
				{
					DeleteFile(saveTempS);
				}
				*/
				
				if(ExistFile(saveBookS))
					DeleteFile(saveBookS);
			}

			for (int i=nCount+1; i<=100; i++)
			{
				//count ���� ū���� ������ �����Ѵ�
			//	dele.Format("portfolio.i%02d", i);
				book.Format("bookmark.i%02d", i);
			//	deleFile.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, dele);
				delbookFile.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, book);

				/*	
				if(ExistFile(deleFile))
				{
					DeleteFile(deleFile);
				}
				*/
				if(ExistFile(delbookFile))
				{
					DeleteFile(delbookFile);
				}
			}

			return;

		}
		else	
		{
			m_sheet->setManageGroup(datB);
			CString	string = _T(""), gnoS, oldnoS, gnameS, saveS, saveTempS, saveBookS, bookS;
			const int groupCount = m_sheet->getManageCount();
			for(int i=1 ; i<= groupCount ; i++)
			{
				//saveS.Format("%s\\%s\\%s\\portfolio.i%02d.save", m_root, "user", m_name, i);
				saveBookS.Format("%s\\%s\\%s\\bookmark.i%02d.save", m_root, "user", m_name, i);
				//oldFile.Format("%s\\%s\\%s\\portfolio.i%02d", m_root, "user", m_name, i);
				bookS.Format("%s\\%s\\%s\\bookmark.i%02d", m_root, "user", m_name, i);
				
				//if(ExistFile(oldFile))
				//	CopyFile(oldFile, saveS, false);
				if(ExistFile(bookS))
					CopyFile(bookS, saveBookS, false);
				
				//if(isSequence == true)
				//{
				//	saveTempS.Format("%s\\%s\\%s\\portfolio.i%02d.savetmp", m_root, "user", m_name, i);
				//	oldTempFile.Format("%s\\%s\\%s\\portfolio.i%02d.tmp", m_root, "user", m_name, i);
				//	
				//	if(ExistFile(oldTempFile))
				//		CopyFile(oldTempFile, saveTempS, false);
				//}
			}

//			WritePrivateProfileString(_T("GROUPORDER"), "00", string, saveP);
//			WritePrivateProfileString(nullptr, nullptr, nullptr, saveP);		
			
			CString dele, book, deleFile, deletempFile, delbookFile, tempdel;

			for (int i=nCount+1; i<=100; i++)
			{
				//count ���� ū���� ������ �����Ѵ�
			//	dele.Format("portfolio.i%02d", i);
				book.Format("bookmark.i%02d", i);
			//	deleFile.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, dele);
				delbookFile.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, book);
				
			/*	if(ExistFile(deleFile))
				{
					DeleteFile(deleFile);
				}*/
				if(ExistFile(delbookFile))
				{
					DeleteFile(delbookFile);
				}
			}
			
			return;
		}
	}


	for (int ii=1; ii<=100; ii++)
	{
		saveFile.Format("bookmark.i%02d.save", ii);
		saveN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, saveFile);
		if (ExistFile(saveN))
		{
			moveN = saveN.Left(saveN.GetLength()-5);
			CopyFile(saveN, moveN, false);
		}

		//portFile.Format("portfolio.i%02d.save", ii);
		//saveportN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, portFile);
		//if (ExistFile(saveportN))
		//{
		//	savemoveN = saveportN.Left(saveportN.GetLength()-5);
		//	CopyFile(saveportN, savemoveN, false);
		//}
	}

	for (int ii=1; ii<=100; ii++)
	{
		saveFile.Format("bookmark.i%02d.save", ii);
		saveN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, saveFile);

		if (ExistFile(saveN))
		{
			moveN = saveN.Left(saveN.GetLength()-5);
			DeleteFile(saveN);
			DeleteFile(moveN+".tmp");
		}

		//portFile.Format("portfolio.i%02d.save", ii);
		//saveportN.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, portFile);
		//
		//if (ExistFile(saveportN))
		//{
		//	moveN = saveportN.Left(saveportN.GetLength()-5);
		//	DeleteFile(saveportN);
		//	DeleteFile(moveN+".tmp");
		//}

		////count ���� ū���� ������ �����Ѵ�
		//if(ii>count) 
		//{
		//	CString dele, deleFile;
		//	dele.Format("portfolio.i%02d", ii);
		//	deleFile.Format("%s\\%s\\%s\\%s", m_root, "user", m_name, dele);
		//	
		//	if(ExistFile(deleFile))
		//	{
		//		DeleteFile(deleFile);
		//	}		
		//}
	}
}

BOOL CMapWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class	
	return CWnd::PreTranslateMessage(pMsg);
}


void CMapWnd::Request_GroupList()
{
	int	sendL = 0;
	std::string sendB;
	sendB.resize(16 * 1024);
	const int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL]; 
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	sendTR(trUPDOWN, sendB.data(), sendL, dnGROUPLIST);
}


void CMapWnd::Request_GroupCode(int iseq)
{
	const  int index = iseq;
	int sendL = 0;
	CString stmp;
	std::string sendB;
	sendB.resize(16 * 1024);

	const int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	stmp.Format("%02d", index);
	memcpy((char*)&sendB[sz_uinfo], stmp, 2);
	sendTR(trUPDOWN, sendB.data(), sendL, dnGROUPCODE);
}

