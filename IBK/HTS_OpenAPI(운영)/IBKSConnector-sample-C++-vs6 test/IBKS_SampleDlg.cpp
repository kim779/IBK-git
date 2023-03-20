#include "stdafx.h"
#include "IBKS_Sample.h"
#include "IBKS_SampleDlg.h"
#include "Open_API_OUT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIBKS_SampleDlg::CIBKS_SampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIBKS_SampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIBKS_SampleDlg)
	m_CertPW = _T("ahffkdy123 ");
	m_UserID = _T("rhkd62");
	m_UserPW = _T("ahffkdy3");
	m_odrAccn = _T("");
	m_odrPswd = _T("");
	m_odrCode = _T("");
	m_odrHogb = _T("");
	m_odrJprc = _T("");
	m_odrJqty = _T("");
	m_odrMmgb = _T("");
	m_odrOgno = _T("0");
	m_siseCode = _T("");
	m_jngoAccn = _T("");
	m_jngoPswd = _T("");
	m_ServerIP = _T("211.255.204.137");
//	m_ServerIP = _T("172.16.202.130");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bGreeks1 = FALSE;
	m_bGreeks2 = FALSE;
}

void CIBKS_SampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIBKS_SampleDlg)
	DDX_Control(pDX, IDC_COMBO2, m_cbJType);
	DDX_Control(pDX, IDC_JNGO_ACCN, m_cbAccn2);
	DDX_Control(pDX, IDC_JNGOLIST, m_lbxJngo);
	DDX_Control(pDX, IDC_CHEGLIST, m_cbCheg);
	DDX_Control(pDX, IDC_ODR_RD, m_edRdResult);
	DDX_Control(pDX, IDC_ODR_RESULT, m_edResult);
	DDX_Control(pDX, IDC_ODR_MMGB, m_cbMmgb);
	DDX_Control(pDX, IDC_ODR_HOGB, m_cbHogb);
	DDX_Control(pDX, IDC_ODR_ACCN, m_cbAccn);
	DDX_Text(pDX, IDC_CERTPW, m_CertPW);
	DDV_MaxChars(pDX, m_CertPW, 30);
	DDX_Text(pDX, IDC_USERID, m_UserID);
	DDV_MaxChars(pDX, m_UserID, 12);
	DDX_Text(pDX, IDC_USERPW, m_UserPW);
	DDV_MaxChars(pDX, m_UserPW, 12);
	DDX_Control(pDX, IDC_CONN, m_axIBKS);
	DDX_CBString(pDX, IDC_ODR_ACCN, m_odrAccn);
	DDX_Text(pDX, IDC_ODR_ACPW, m_odrPswd);
	DDX_Text(pDX, IDC_ODR_CODE, m_odrCode);
	DDV_MaxChars(pDX, m_odrCode, 8);
	DDX_CBString(pDX, IDC_ODR_HOGB, m_odrHogb);
	DDX_Text(pDX, IDC_ODR_JPRC, m_odrJprc);
	DDX_Text(pDX, IDC_ODR_JQTY, m_odrJqty);
	DDX_CBString(pDX, IDC_ODR_MMGB, m_odrMmgb);
	DDX_Text(pDX, IDC_ODR_OGNO, m_odrOgno);
	DDX_Text(pDX, IDC_SISE_CODE, m_siseCode);
	DDV_MaxChars(pDX, m_siseCode, 8);
	DDX_CBString(pDX, IDC_JNGO_ACCN, m_jngoAccn);
	DDX_Text(pDX, IDC_JNGO_ACPW, m_jngoPswd);
	DDX_Text(pDX, IDC_EDIT1, m_ServerIP);
	DDV_MaxChars(pDX, m_ServerIP, 20);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIBKS_SampleDlg, CDialog)
	//{{AFX_MSG_MAP(CIBKS_SampleDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_ODR_SEND, OnOdrSend)
	ON_BN_CLICKED(IDC_SISE_SEND, OnSiseSend)
	ON_BN_CLICKED(IDC_JNGO_SEND, OnJngoSend)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	ON_BN_CLICKED(IDC_JNGO_SEND2, OnJngoSend2)
	ON_BN_CLICKED(IDC_SISE_SEND2, OnSiseSend2)
	ON_BN_CLICKED(IDC_SISE_SEND3, OnSiseSend3)
	ON_BN_CLICKED(IDC_SISE_SEND4, OnSiseSend4)
	ON_BN_CLICKED(IDC_SISE_SEND5, OnSiseSend5)
	ON_BN_CLICKED(IDC_GREEKS, OnGreeks)
	ON_BN_CLICKED(IDC_SONIK, OnSonIk)
	ON_BN_CLICKED(IDC_MON_SISE, OnMonSise)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CIBKS_SampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_cbJType.SetCurSel(0);

	// �ݵ�� �ʱ�ȭ �� ����մϴ�.
	if (!m_axIBKS.Initialize())
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "�ʱ�ȭ����");
		OnCancel();
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIBKS_SampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CIBKS_SampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIBKS_SampleDlg::OnBtnConnect() 
{
	UpdateData(TRUE);

	if (m_UserID.IsEmpty())
	{
		MessageBox("���̵� �Է��ϼ���", "IBK��������");
		GetDlgItem(IDC_USERID)->SetFocus();
		return;
	}

	if (m_UserPW.IsEmpty())
	{
		MessageBox("���Ӻ���� �Է��ϼ���", "IBK��������");
		GetDlgItem(IDC_USERPW)->SetFocus();
		return;
	}

	if (m_CertPW.IsEmpty())
	{
		MessageBox("����������� �Է��ϼ���", "IBK��������");
		GetDlgItem(IDC_CERTPW)->SetFocus();
		return;
	}

	// �׽�Ʈ���� : 211.255.204.98

	if(m_ServerIP.IsEmpty())
	{
		if (!m_axIBKS.Login(m_UserID, m_UserPW, m_CertPW, "", 15201))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�α��� ����", MB_OK|MB_ICONERROR);
		}
	}
	else
	{
		if (!m_axIBKS.Login(m_UserID, m_UserPW, m_CertPW, m_ServerIP, 15201))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�α��� ����", MB_OK|MB_ICONERROR);
		}
	}

}

BEGIN_EVENTSINK_MAP(CIBKS_SampleDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CIBKS_SampleDlg)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 1 /* OnLogin */, OnEvtLogin, VTS_BOOL VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 2 /* OnContract */, OnEvtContract, VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 3 /* OnRealData */, OnEvtRealData, VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 4 /* OnRecvData */, OnEvtRecvData, VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 5 /* OnError */, OnEvtError, VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 6 /* OnClose */, OnEvtClose, VTS_NONE)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 7 /* OnAlert */, OnEvtAlert, VTS_I4 VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 9 /* OnVerUpdate */, OnEvtVerUpdate, VTS_NONE)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 10 /* OnUpdateStart */, OnEvtUpdateStart, VTS_NONE)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 11 /* OnUpdating */, OnEvtUpdating, VTS_BSTR VTS_I4)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 12 /* OnUpdateEnd */, OnEvtUpdateEnd, VTS_NONE)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 13 /* OnGuideMsg */, OnEvtGuideMsg, VTS_I4 VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 14 /* OnFBalance */, OnEvtFBalance, VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 15 /* OnSBalance */, OnEvtSBalance, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CIBKS_SampleDlg::OnEvtLogin(BOOL bLogin, LPCTSTR msg) 
{
	MessageBox(msg, "�α��� ���", bLogin ? MB_OK : MB_OK|MB_ICONERROR);

	if (bLogin)
	{
		// ���¹�ȣ �ε�
		m_cbAccn.ResetContent();
		m_cbAccn2.ResetContent();
		{
			// ���¹�ȣ\t���¸�\n���¹�ȣ\t���¸� ����
			CString strSt = m_axIBKS.GetAccounts();
			LPCSTR ed = (LPCSTR)strSt + strSt.GetLength();
			
			vector<StringProxy> vAccn;
			ParseString((LPCSTR)strSt, ed, '\n', vAccn);
			for(int n=0; n<vAccn.size(); ++n)
			{
				m_cbAccn.AddString( vAccn[n].ToCStr() );
				m_cbAccn2.AddString( vAccn[n].ToCStr() );
			}

			if (!vAccn.empty())
			{
				m_cbAccn.SetCurSel(0);
				m_cbAccn2.SetCurSel(0);
			}
		}

		// ���а� ����
		m_cbHogb.ResetContent();
		{
			m_cbHogb.AddString("00 ������");		m_cbHogb.SetItemData(0, 0);
			m_cbHogb.AddString("03 ���尡");		m_cbHogb.SetItemData(1, 3);
			m_cbHogb.AddString("05 ���Ǻ�");		m_cbHogb.SetItemData(2, 5);
			m_cbHogb.AddString("06 ������");		m_cbHogb.SetItemData(3, 6);
			m_cbHogb.AddString("10 ������(IOC)");	m_cbHogb.SetItemData(4,10);
			m_cbHogb.AddString("20 ������(FOK)");	m_cbHogb.SetItemData(5,20);
			m_cbHogb.SetCurSel(0);
		}

		// �Ÿű��� ����
		m_cbMmgb.ResetContent();
		{
			m_cbMmgb.AddString("1 �ŵ�");	m_cbMmgb.SetItemData(0, 1);
			m_cbMmgb.AddString("2 �ż�");	m_cbMmgb.SetItemData(1, 2);
			m_cbMmgb.AddString("3 ����");	m_cbMmgb.SetItemData(2, 3);
			m_cbMmgb.AddString("4 ���");	m_cbMmgb.SetItemData(3, 4);
			m_cbMmgb.SetCurSel(0);
		}
	}
}

void CIBKS_SampleDlg::OnEvtContract(LPCTSTR data) 
{
	LPCTSTR pos;
	LPCTSTR st = data;
	LPCTSTR ed = data + strlen(data);
	op_notice_spliter op;
	
	// �������� �ڼ��� ������ �޴����� �����ϼ��� \r \t \n ������.
	StringProxy key, val;
	CMapStringToString map;
	while(st<ed)
	{
		pos = find_if(st, ed, op);
		key = StringProxy(st, pos);
		st = ++pos;

		pos = find_if(st, ed, op);
		val = StringProxy(st, pos);
		st = ++pos;
		
		map[key.ToCStr()] = val.ToCStr();
	}

	// ����޼��� ���
	CString strRet, strText;

// 	strRet.Format("����[%s] ����[%s] ����[%s] �Ÿ�[%s] ����[%s] ����[%s] �ֹ���ȣ[%s] ���ֹ�[%s] ���������ID[%s]\r\n",
// 		map["988"], map["901"], map["907"], map["912"], map["916"], map["925"], map["904"], map["905"], map["974"]);

	strRet.Format("����[%s] ����[%s] ����[%s] �Ÿ�[%s] ����[%s] ����[%s] �ֹ���ȣ[%s] ���ֹ�[%s] �ֹ�����[%s] ü�����[%s] ��ü�����[%s]\r\n",
		map["988"], map["901"], map["907"], map["912"], map["916"], map["925"], map["904"], map["905"], map["974"], map["909"], map["931"], map["921"]);

	m_edRdResult.GetWindowText(strText);
	m_edRdResult.SetWindowText(strRet + strText);
	m_edResult.SetWindowText(data);
}

void CIBKS_SampleDlg::OnEvtRealData(LPCTSTR data) 
{
	LPCTSTR pos;
	LPCTSTR st = data;
	LPCTSTR ed = data + strlen(data);
	StringProxy code, key, val;
	CMapStringToString map;

	CString strCode;
	// �������� �ڼ��� ������ �޴����� �����ϼ��� \t ������.

	pos = find(st, ed, '\t');
	code = StringProxy(st, pos);
	st = ++pos;

	strCode = code.ToCStr();

	if(strCode.GetAt(0) == 'A')
	{
		strCode.Delete(0);
	}
	
	// ���� ��ȸ�� ���� �ü�ó���Ѵ�.

	if (strCode != m_siseCode)return;

	while(st<ed)
	{
		pos = find(st, ed, '\t');
		key = StringProxy(st, pos);
		st = ++pos;
		
		pos = find(st, ed, '\t');
		val = StringProxy(st, pos);
		st = ++pos;
		
		map[key.ToCStr()] = val.ToCStr();
	}

	{
		CString key, val;

		if (map.Lookup("034", val))	// ü��ð��� ������ ü�ᵥ����
		{
			GetDlgItem(IDC_CURR)->SetWindowText( map["023"] );	// ���簡
			GetDlgItem(IDC_DIFF)->SetWindowText( map["024"] );	// ���ϴ��
			GetDlgItem(IDC_RATE)->SetWindowText( map["033"] );	// �����
			GetDlgItem(IDC_GVOL)->SetWindowText( map["027"] );	// �ŷ���
			GetDlgItem(IDC_MGJV)->SetWindowText( map["201"] );	// �̰����
			GetDlgItem(IDC_SIGA)->SetWindowText( map["029"] );	// �ð�
			GetDlgItem(IDC_KOGA)->SetWindowText( map["030"] );	// ��
			GetDlgItem(IDC_JEGA)->SetWindowText( map["031"] );	// ����
			//�������� = ���簡 - ���ϴ��
			CString strJongga, strCurr;

			strCurr = map["023"];

			OutputDebugString("[KSJ] Real diff[" + map["024"] + "]");

			if(strCurr.GetAt(0) == '-')
			{
				strCurr.Delete(0);

				strJongga.Format("%8.2lf", atof(strCurr) - atof(map["024"]));

				strJongga.TrimLeft(); strJongga.TrimRight();
				strJongga.Insert(0, "-");

				GetDlgItem(IDC_JONGGA)->SetWindowText( strJongga );	// ��������
			}
			else
			{
				strJongga.Format("%8.2lf", atof(strCurr) - atof(map["024"]));
				GetDlgItem(IDC_JONGGA)->SetWindowText( strJongga );	// ��������
			}

			// ü�ᷮ�� ��ȣ�� �ȿ��� ù��° ���ڴ� ü�ᰭ�� ǥ���̹Ƿ� ��������.
			LPCSTR cvol = map["032"];
			if (cvol[0]!='+' && cvol[0]!='-') ++cvol;

			// ü�᳻�� ���� ( ü��ð�/ü�ᰡ/ü�ᷮ/K200���� )
			CString line;
			line.Format("%s | %6s | %5s | %6s", val, map["023"], cvol, map["002"]);
			m_cbCheg.InsertString(0, line);
		}
		if (map.Lookup("040", val))	// ȣ���ð��� ������ ȣ��������
		{
			GetDlgItem(IDC_MDVOL)->SetWindowText( map["101"] );	// �ŵ��ܷ�
			GetDlgItem(IDC_MSVOL)->SetWindowText( map["106"] );	// �ż��ܷ�
		}
	}
}

void CIBKS_SampleDlg::OnEvtRecvData(long key, long dptr, long size, BOOL bNext, LPCTSTR nkey) 
{
	CString emsg;

	if (key==TK_TR3201)
	{
		struct tr3201_mod *mod = (struct tr3201_mod*)dptr;
		emsg.Format("�ֹ���ȣ[%.6s] ���ֹ���ȣ[%.6s] ���[%.80s]", mod->jmno, mod->ojno, mod->emsg);
		m_edResult.SetWindowText(emsg);
	}
	else if (key==TK_TR3001)
	{
		struct tr3001_mod *mod = (struct tr3001_mod*)dptr;
		
		GetDlgItem(IDC_CURR)->SetWindowText( STR(mod->curr) );
		GetDlgItem(IDC_DIFF)->SetWindowText( STR(mod->diff) );
		GetDlgItem(IDC_RATE)->SetWindowText( STR(mod->rate) );
		GetDlgItem(IDC_GVOL)->SetWindowText( STR(mod->gvol) );
		GetDlgItem(IDC_MGJV)->SetWindowText( STR(mod->mgjv) );
		GetDlgItem(IDC_MDVOL)->SetWindowText( STR(mod->dvol) );
		GetDlgItem(IDC_MSVOL)->SetWindowText( STR(mod->svol) );
		GetDlgItem(IDC_SIGA)->SetWindowText( STR(mod->siga) );
		GetDlgItem(IDC_KOGA)->SetWindowText( STR(mod->koga) );
		GetDlgItem(IDC_JEGA)->SetWindowText( STR(mod->jega) );

		//�������� = ���簡 - ���ϴ��
		CString strJongga, strCurr, strDiff;
		
		strCurr = STR(mod->curr);
		strDiff = STR(mod->diff);
		
		OutputDebugString("[KSJ] 3001 diff[" + strDiff + "]");
		if(strCurr.GetAt(0) == '-')
		{
			strCurr.Delete(0);
			
			strJongga.Format("%8.2lf", atof(strCurr) - atof(STR(mod->diff)));
			
			strJongga.TrimLeft(); strJongga.TrimRight();
			strJongga.Insert(0, "-");
			
			GetDlgItem(IDC_JONGGA)->SetWindowText( strJongga );	// ��������
		}
		else
		{
			strJongga.Format("%8.2lf", atof(strCurr) - atof(STR(mod->diff)));
			GetDlgItem(IDC_JONGGA)->SetWindowText( strJongga );	// ��������
		}
	}
	else if (key==TK_TR3221)
	{
		struct tr3221_mod *mod = (struct tr3221_mod*)dptr;
		
		int nrec = _ttoi( STR(mod->nrec) );
		for(int n=0; n<nrec; ++n)
		{
			CString code = STR(mod->grid[n].cod2);	code.TrimRight();
			CString dsgb = STR(mod->grid[n].dsgb);	dsgb.TrimRight();
			int jqty = _ttoi(STR(mod->grid[n].jqty));	// ��������
			int xqty = _ttoi(STR(mod->grid[n].xqty));	// û�갡�ɼ���
			double pamt = atof(STR(mod->grid[n].pamt));	// ��հ�
			double tamt = atof(STR(mod->grid[n].tamt)); // �򰡼���
			double srat = atof(STR(mod->grid[n].srat));	// ���ͷ�
			
			CString line;
			line.Format("����(%s) �Ÿ�(%s) �ܰ�(%d) û��(%d) ��հ�(%.2f) ����(%.f) ���ͷ�(%.2f)",
				code, dsgb, jqty, xqty, pamt, tamt, srat);
			m_lbxJngo.AddString(line);
			
			if (bNext)
			{
				if (!m_axIBKS.TR3221(TK_TR3221, m_jngoAccn, m_jngoPswd, nkey))
				{
					MessageBox(m_axIBKS.GetLastErrMsg(), "�ܰ�����ȸ����", MB_OK|MB_ICONERROR);
				}
			}
		}
		
	}
	else if (key==TK_TR1201)
	{
		struct tr1201_mod *mod = (struct tr1201_mod*)dptr;
		emsg.Format("�ֹ���ȣ[%.6s] ���ֹ���ȣ[%.6s] ���[%.80s]", mod->jmno, mod->ojno, mod->emsg);
		m_edResult.SetWindowText(emsg);
	}
	else if (key==TK_TR1001)
	{
		struct tr1001_mod *mod = (struct tr1001_mod*)dptr;
		
		GetDlgItem(IDC_CURR)->SetWindowText( STR(mod->curr) );
		GetDlgItem(IDC_DIFF)->SetWindowText( STR(mod->diff) );
		GetDlgItem(IDC_RATE)->SetWindowText( STR(mod->rate) );
		GetDlgItem(IDC_GVOL)->SetWindowText( STR(mod->gvol) );
// 		GetDlgItem(IDC_MGJV)->SetWindowText( STR(mod->mgjv) );
		GetDlgItem(IDC_MDVOL)->SetWindowText( STR(mod->dvol) );
		GetDlgItem(IDC_MSVOL)->SetWindowText( STR(mod->svol) );
		GetDlgItem(IDC_SIGA)->SetWindowText( STR(mod->siga) );
		GetDlgItem(IDC_KOGA)->SetWindowText( STR(mod->koga) );
		GetDlgItem(IDC_JEGA)->SetWindowText( STR(mod->jega) );

		CString strDiff;
		strDiff = STR(mod->diff);
		OutputDebugString("[KSJ] 1001 diff[" + strDiff + "]");
		emsg.Format("1001 [%s]", mod->cod2);
		m_edResult.SetWindowText(emsg);

	}
	else if (key==TK_TR1221)
	{
		struct tr1221_mod *mod = (struct tr1221_mod*)dptr;
		
		int nrec = _ttoi( STR(mod->nrec) );
		for(int n=0; n<nrec; ++n)
		{
			CString code = STR(mod->grid[n].cod2);	code.TrimRight();
// 			CString dsgb = STR(mod->grid[n].dsgb);	dsgb.TrimRight();
			int jqty = _ttoi(STR(mod->grid[n].jqty));	// ��������
			int xqty = _ttoi(STR(mod->grid[n].xqty));	// û�갡�ɼ���
			double pamt = atof(STR(mod->grid[n].pamt));	// ��հ�
			double tamt = atof(STR(mod->grid[n].tamt)); // �򰡼���
			double srat = atof(STR(mod->grid[n].srat));	// ���ͷ�
			
			CString line;
			line.Format("����(%s) �ܰ�(%d) û��(%d) ��հ�(%.2f) ����(%.f) ���ͷ�(%.2f)",
				code, jqty, xqty, pamt, tamt, srat);
			m_lbxJngo.AddString(line);
			
			if (bNext)
			{
				if (!m_axIBKS.TR1221(TK_TR1221, m_jngoAccn, m_jngoPswd, 0, nkey))
				{
					MessageBox(m_axIBKS.GetLastErrMsg(), "�ܰ�����ȸ����", MB_OK|MB_ICONERROR);
				}
			}
		}
	}
	else if (key == TK_TR1802)
	{
		struct tr1802_mod *mod = (struct tr1802_mod*)dptr;
		emsg.Format("����[%s]", mod->nrec);
		m_edResult.SetWindowText(emsg);
	}
	else if (key == TK_TR1801)
	{
		struct tr1801_mod *mod = (struct tr1801_mod*)dptr;
		emsg.Format("�ֽ�[%s]", mod->nrec);
		m_edResult.SetWindowText(emsg);
	}
	else if (key == TK_TR1803)
	{
		struct tr1803_mod *mod = (struct tr1803_mod*)dptr;
		emsg.Format("������ ���[%s]", mod->nrec);
		m_edResult.SetWindowText(emsg);
	}
	else if (key == TK_TR1804)
	{
		struct tr1804_mod *mod = (struct tr1804_mod*)dptr;
		emsg.Format("������ȸ[%s]", mod->nrec);
		m_edResult.SetWindowText(emsg);
	}
	else if(key == TK_GETCODE)
	{
		struct hjcode *mod = (struct hjcode*)dptr;
		emsg.Format("������[%s]", mod->codx);
		m_edRdResult.SetWindowText(emsg);
	}
	else if (key == TK_TR1211)
	{
		struct tr1211_mod *mod = (struct tr1211_mod*)dptr;
		emsg.Format("ü�᳻����ȸ[%s]", mod->acno);
		m_edResult.SetWindowText(emsg);
	}
	else if (key == TK_GREEKS2) //���� ���簡(�����ڻ��� �����ֱٿ����� �������)
	{
		m_bGreeks2 = TRUE;

		CString strResult, strTemp;
	
		struct tr3001_mod *mod = (struct tr3001_mod*)dptr;
		
		m_strGreeks2 = STR(mod->curr);

		if(m_bGreeks1 && m_bGreeks2)
		{
			//�����ڵ�\t�����ϼ�\t��簡\tcd�ݸ�\t���簡\t�����ڻ�
			strTemp = m_strGreeksCode + "\t" + m_strGreeks1 + "\t" + m_strGreeks2;
			strResult = m_axIBKS.GetGreeksData(strTemp);
			m_edResult.SetWindowText(strResult);

			m_bGreeks1 = m_bGreeks2 = FALSE;
		}
	}
	else if (key == TK_GREEKS1) // �ɼ� �����ϼ�, ��簡, cd�ݸ�, ���簡
	{
		m_bGreeks1 = TRUE;
		
		CString strResult, strTemp, strDays;
		strTemp = (char*)dptr;
	
		strDays = strTemp.Left(strTemp.Find("\t") + 1); strDays.TrimRight();

		m_strGreeks1.Format("%f", atof(strDays)/365.0);	//�޷��ϼ����� �����ϼ�
		//m_strGreeks1.Format("%f", atof(strDays)/250.0);	//�����ϼ����� �����ϼ�

		m_strGreeks1 += strTemp.Right(strTemp.GetLength() - strDays.GetLength());

		if(m_bGreeks1 && m_bGreeks2)
		{
			//�����ڵ�\t�����ϼ�\t��簡\tcd�ݸ�\t���簡\t�����ڻ�
			strTemp = m_strGreeksCode + "\t" + m_strGreeks1 + "\t" + m_strGreeks2;
			strResult = m_axIBKS.GetGreeksData(strTemp);
			m_edResult.SetWindowText(strResult);

			m_bGreeks1 = m_bGreeks2 = FALSE;
		}
	}
	else if (key == TK_TR3232)	//���� ���ϼ�����ȸ
	{
		struct tr3232_mod *mod = (struct tr3232_mod*)dptr;
		emsg.Format("���� ���ϼ�����ȸ[%s]", mod->zOut);
		m_edResult.SetWindowText(emsg);

	}
	else if (key == TK_TR3411)	//���� ������ �ü���ȸ
	{
		struct tr3411_mod *mod = (struct tr3411_mod*)dptr;
		emsg.Format("���� ������ �ü���ȸ[%s]", mod->atmg);
		m_edResult.SetWindowText(emsg);	
	}
}

void CIBKS_SampleDlg::OnEvtError(LPCTSTR msg) 
{
	MessageBox(msg, "�����߻�", MB_OK|MB_ICONERROR);
}

void CIBKS_SampleDlg::OnEvtGuideMsg(long key, LPCTSTR sMsg) 
{
	CString strTemp;
	strTemp.Format("[%d]%s", key, sMsg);
	MessageBox(strTemp, "�ڽ���", MB_OK);
}

void CIBKS_SampleDlg::OnEvtClose() 
{
	MessageBox("������ ����Ǿ����ϴ�.", "����", MB_OK|MB_ICONINFORMATION);
}

void CIBKS_SampleDlg::OnEvtAlert(long code, LPCTSTR msg) 
{
	CString strMsg;
	strMsg.Format("[%04d] %s", code, msg);
	MessageBox(strMsg, "������޸޼���", MB_OK|MB_ICONINFORMATION);
}

/*
  ���� �ǽð� �ܰ�(tab�����ڷ� ���ڿ� ����, "\n"���� ���񱸺�)
  ���̾ƿ�(14) : Flag�����ڵ�\t�����\t����\t�ܰ����\tû�갡�ɼ���\t��հ�\t���簡\t
  ��հ����\t�򰡱ݾ�\t�򰡼���\t���ͷ�\t���Աݾ�\t���ϴ��\n

  Flag : "A"(��ü "A"\t�Ѱ���\t), "I"(���� "I"\t�����ڵ�), 
       "U"(���� "U"\t�����ڵ�), "D"(���� "D"\t�����ڵ�), "E"(���� "E"\t�����޼���)
*/
void CIBKS_SampleDlg::OnEvtFBalance(LPCTSTR data) 
{
	CString strData, strLine, strField, flag, strFormat;
	CStringArray arrLine;

	CStringArray arrField;
	arrField.Add("�����ڵ�"); arrField.Add("�����"); arrField.Add("����"); arrField.Add("�ܰ����"); arrField.Add("û�갡�ɼ���");
	arrField.Add("��հ�"); arrField.Add("���簡"); arrField.Add("��հ����"); arrField.Add("�򰡱ݾ�"); arrField.Add("�򰡼���");
	arrField.Add("���ͷ�"); arrField.Add("���Աݾ�"); arrField.Add("���ϴ��");

	strData = data;
	
	int i = 0;

	CString strTemp, strOrgData = data;
	strOrgData.Replace("\t", "??");
	strTemp.Format("[KSJ] Data[%s]", strOrgData);
	OutputDebugString(strTemp);

	flag = strData.Left(strData.Find("\t") + 1); flag.TrimRight();
	strData = strData.Right(strData.GetLength() - flag.GetLength() -1);
	

	if(flag == "A")	// ��ü�϶��� ������ �ϳ� �� �ٴ´�.
		strData = strData.Right(strData.GetLength() - strData.Find("\t") -1);
	else if(flag =="U" || flag == "I")	//�����̳� �����϶��� �����ڵ尡 �������� �´�.
		strData = strData.Right(strData.GetLength() - 8);

	//����
	while(AfxExtractSubString(strLine, strData, i++, '\n'))
	{
		arrLine.RemoveAll();
		int n = 0;

		if(strLine.Find("\t") == -1)
			continue;

		//Field�� Array�� ����
		while(AfxExtractSubString(strField, strLine, n++, '\t'))
		{
			arrLine.Add(strField);
		}

		CString line;

		int nSize = min(arrField.GetSize(), arrLine.GetSize());

		//����Ʈ�� �� ���ڿ� ����
		for(int ii = 0; ii < nSize; ii++)
		{
			line += arrField.GetAt(ii) + "(" + arrLine.GetAt(ii) + ")\t";
		}

		//Flag�� ���� �����Ͽ� ����Ʈ���� ����
		if(flag == "A" || flag =="I")
		{
			m_lbxJngo.AddString(line);
		}
		else if(flag == "U")
		{
			int nIndex = m_lbxJngo.FindString(-1, arrField.GetAt(0) + "(" + arrLine.GetAt(0) + ")");
			if(nIndex != LB_ERR)
			{
				m_lbxJngo.DeleteString(nIndex);
				m_lbxJngo.InsertString(nIndex, line);
			}
		}
		else if(flag == "D")
		{
			int nIndex = m_lbxJngo.FindString(-1, arrField.GetAt(0) + "(" + arrLine.GetAt(0) + ")");
			if(nIndex != LB_ERR)
			{
				m_lbxJngo.DeleteString(nIndex);
			}
		}
		else
		{

		}
	}

	CreateHorizontalScroll();
}

/*
		�ֽ� �ǽð� �ܰ�(tab�����ڷ� ���ڿ� ����, "\n"���� ���񱸺�)
		���̾ƿ�(22) : Flag�����ڵ�\t�����\t�ܰ���\t�����ܰ�\t�ֹ��ܷ�\t������հ�\t
		�ŵ������ܰ�\t���簡\t�򰡱ݾ�\t�򰡼���\t�򰡼��ͷ�\t���Աݾ�\t
		������\t�ſ뱸��\t�ſ뱸�г���\t�ſ뱸�о���\t�ſ����ݾ�\t
		�ܰ���\tBEP�ܰ�\t���\t�����\n

		Flag : "A"(��ü "A"\t�Ѱ���\t), 
		"I"(���� "I"\t�ܰ���(2)+�����ڵ�(11)+������(8)+�ſ뱸��(2)), 
		"U"(���� "U"\t�ܰ���(2)+�����ڵ�(11)+������(8)+�ſ뱸��(2)),
		"D"(���� "D"\t�ܰ���(2)+�����ڵ�(11)+������(8)+�ſ뱸��(2)), 
		"E"(���� "E"\t�����޼���)*/
void CIBKS_SampleDlg::OnEvtSBalance(LPCTSTR data) 
{
	CString strData, strLine, strField, flag, strFormat;
	CStringArray arrLine;
	
	CStringArray arrField;
	arrField.Add("�����ڵ�"); arrField.Add("�����"); arrField.Add("�ܰ���"); arrField.Add("�����ܰ�"); arrField.Add("�ֹ��ܷ�"); arrField.Add("������հ�");
	arrField.Add("�ŵ������ܰ�"); arrField.Add("���簡"); arrField.Add("�򰡱ݾ�"); arrField.Add("�򰡼���"); arrField.Add("�򰡼��ͷ�");
	arrField.Add("���Աݾ�"); arrField.Add("������"); arrField.Add("�ſ뱸��"); arrField.Add("�ſ뱸�г���"); arrField.Add("�ſ뱸�о���");
	arrField.Add("�ſ����ݾ�"); arrField.Add("�ܰ���"); arrField.Add("BEP�ܰ�"); arrField.Add("���"); arrField.Add("�����");
	
	strData = data;
	
	CString strTemp, strOrgData = data;
	strOrgData.Replace("\t", "??");
	strTemp.Format("[KSJ] Data[%s]", strOrgData);
	OutputDebugString(strTemp);

	int i = 0;
	
	flag = strData.Left(strData.Find("\t") + 1); flag.TrimRight();
	strData = strData.Right(strData.GetLength() - flag.GetLength() -1);
	
	strTemp.Format("[KSJ] flag[%s] strData[%s]", flag, strData);
	OutputDebugString(strTemp);

	//����	
	if(flag == "A")	// ��ü�϶��� ������ �ϳ� �� �ٴ´�.
		strData = strData.Right(strData.GetLength() - strData.Find("\t") -1);
	else if(flag =="U" || flag == "I" || flag == "D")	//�����̳� �����϶��� �����ڵ尡 �������� �´�.
		strData = strData.Right(strData.GetLength() - 24);	//�ܰ���(2)+�����ڵ�(11)+������(8)+�ſ뱸��(2)(23�ڸ�)
	
	strTemp.Format("[KSJ] flag[%s] strData[%s]", flag, strData);
	OutputDebugString(strTemp);

	while(AfxExtractSubString(strLine, strData, i++, '\n'))
	{
		arrLine.RemoveAll();
		int n = 0;
		
		if(strLine.Find("\t") == -1)
			continue;
		
		//Field�� Array�� ����
		while(AfxExtractSubString(strField, strLine, n++, '\t'))
		{
			arrLine.Add(strField);
		}
		
		CString line;
		
		int nSize = min(arrField.GetSize(), arrLine.GetSize());
		
		//����Ʈ�� �� ���ڿ� ����
		for(int ii = 0; ii < nSize; ii++)
		{
			line += arrField.GetAt(ii) + "(" + arrLine.GetAt(ii) + ")\t";
		}
		
		//Flag�� ���� �����Ͽ� ����Ʈ���� ����
		if(flag == "A" || flag =="I")
		{
			m_lbxJngo.AddString(line);
		}
		else if(flag == "U")
		{
			int nIndex = m_lbxJngo.FindString(-1, arrField.GetAt(0) + "(" + arrLine.GetAt(0) + ")");

			strTemp.Format("[KSJ] %s(%s) ", arrField.GetAt(0), arrLine.GetAt(0));
			OutputDebugString(strTemp);

			if(nIndex != LB_ERR)
			{
				m_lbxJngo.DeleteString(nIndex);
				m_lbxJngo.InsertString(nIndex, line);
			}
		}
		else if(flag == "D")
		{
			int nIndex = m_lbxJngo.FindString(-1, arrField.GetAt(0) + "(" + arrLine.GetAt(0) + ")");
			if(nIndex != LB_ERR)
			{
				m_lbxJngo.DeleteString(nIndex);
			}
		}
		else
		{
			
		}
	}

	CreateHorizontalScroll();
}

BOOL CIBKS_SampleDlg::PreTranslateMessage(MSG* pMsg) 
{
	// ���ͷ� ���� ���� ����
	if (pMsg->message==WM_RBUTTONUP)
	{
		MessageBox("XXX");
		if (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE) 
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CIBKS_SampleDlg::ParseString( LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval )
{
	retval.clear();
	LPCSTR pos;
	while(st<ed)
	{
		pos = find(st, ed, del);
		retval.push_back( StringProxy(st, pos) );
		st = pos + 1;
	}
}

void CIBKS_SampleDlg::ParseString( StringProxy &sp, CHAR del, vector<StringProxy> &retval )
{
	ParseString(sp.first, sp.second, del, retval);
}

void CIBKS_SampleDlg::OnOdrSend() 
{
	UpdateData(TRUE);

	// �ֹ��� �ֹ� ������ ��������X100 �� ���� ������ �ѱ⼼��
	// ex) 288.05 -> 28805
	DWORD mmgb = m_cbMmgb.GetItemData( m_cbMmgb.GetCurSel() );
	DWORD hogb = m_cbHogb.GetItemData( m_cbHogb.GetCurSel() );
	m_odrAccn = m_odrAccn.Left(11);

	if(m_cbJType.GetCurSel() == 0)
	{
		if (!m_axIBKS.TR3201(TK_TR3201, mmgb, m_odrAccn, m_odrPswd, _ttoi(m_odrOgno), m_odrCode, _ttoi(m_odrJqty), _ttoi(m_odrJprc), hogb, 0))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�ֹ�����", MB_OK|MB_ICONERROR);
		}
	}
	else
	{
		if (!m_axIBKS.TR1201(TK_TR1201, mmgb, m_odrAccn, m_odrPswd, _ttoi(m_odrOgno), m_odrCode, _ttoi(m_odrJqty), _ttoi(m_odrJprc), hogb, 0))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�ֹ�����", MB_OK|MB_ICONERROR);
		}
	}
}

void CIBKS_SampleDlg::OnSiseSend() 
{
	UpdateData(TRUE);

	m_cbCheg.ResetContent();
	GetDlgItem(IDC_CURR)->SetWindowText( "" );
	GetDlgItem(IDC_DIFF)->SetWindowText( "" );
	GetDlgItem(IDC_RATE)->SetWindowText( "" );
	GetDlgItem(IDC_GVOL)->SetWindowText( "" );
	GetDlgItem(IDC_MGJV)->SetWindowText( "" );

	if(m_cbJType.GetCurSel() == 0)
	{
		if (m_siseCode.GetLength()!=8)
		{
			MessageBox("�ùٸ� �����ڵ带 �Է��ϼ���", "�������", MB_OK|MB_ICONERROR);
			return;
		}
		
		if (!m_axIBKS.TR3001(TK_TR3001, m_siseCode))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�ü���ȸ����", MB_OK|MB_ICONERROR);
			return;
		}
	}
	else
	{
		if (m_siseCode.GetLength()!=6)
		{
			MessageBox("�ùٸ� �����ڵ带 �Է��ϼ���", "�������", MB_OK|MB_ICONERROR);
			return;
		}
		
		if (!m_axIBKS.TR1001(TK_TR1001, m_siseCode))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�ü���ȸ����", MB_OK|MB_ICONERROR);
			return;
		}
	}
}

LPCSTR CIBKS_SampleDlg::GetString( LPCSTR dat, int len )
{
	// No thread safe!!
	static char buff[1024];
	sprintf(buff, "%.*s", len, dat);
	return buff;
}

void CIBKS_SampleDlg::OnJngoSend() 
{
	UpdateData(TRUE);

	m_lbxJngo.ResetContent();
	m_jngoAccn = m_jngoAccn.Left(11);

	if(m_cbJType.GetCurSel() == 0)
	{
		if (!m_axIBKS.TR3221(TK_TR3221, m_jngoAccn, m_jngoPswd, ""))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�ܰ���ȸ����", MB_OK|MB_ICONERROR);
			return;
		}
	}
	else
	{
		if (!m_axIBKS.TR1221(TK_TR1221, m_jngoAccn, m_jngoPswd, 0, ""))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�ܰ���ȸ����", MB_OK|MB_ICONERROR);
			return;
		}
	}
}

void CIBKS_SampleDlg::OnBtnDisconnect() 
{
	// TODO: Add your control notification handler code here
	m_axIBKS.Logout();
}

void CIBKS_SampleDlg::OnEvtVerUpdate()
{
	MessageBox("�÷��� ������Ʈ �Ϸ�\n���Ϻ��� �� ��⵿ �Ͻñ� �ٶ��ϴ�.", "�÷��� ������Ʈ", MB_OK);
}

void CIBKS_SampleDlg::OnEvtUpdateStart()
{
	MessageBox("���� ������Ʈ", "���� ������Ʈ ����", MB_OK);
}

void CIBKS_SampleDlg::OnEvtUpdating(LPCSTR sData, long nProgress)
{
	CString emsg;
	emsg.Format("sData[%s] nProgress[%d]", sData, nProgress);
	m_edResult.SetWindowText(emsg);
}

void CIBKS_SampleDlg::OnEvtUpdateEnd()
{
	MessageBox("���� ������Ʈ", "���� ������Ʈ ��", MB_OK);
}

void CIBKS_SampleDlg::OnJngoSend2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	m_lbxJngo.ResetContent();
	m_jngoAccn = m_jngoAccn.Left(11);
	
	if(m_cbJType.GetCurSel() == 0)
	{
		if (!m_axIBKS.TR3222(TK_TR3222, m_jngoAccn, m_jngoPswd))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�ǽð� �ܰ���ȸ����", MB_OK|MB_ICONERROR);
			return;
		}
	}
	else
	{
		if (!m_axIBKS.TR1222(TK_TR1222, m_jngoAccn, m_jngoPswd, 2))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�ǽð� �ܰ���ȸ����", MB_OK|MB_ICONERROR);
			return;
		}
	}	
}

//���ν�ũ�� ����
void CIBKS_SampleDlg::CreateHorizontalScroll()
{
    CString str; CSize sz; int dx=0;
    CDC *pDC = m_lbxJngo.GetDC();
	
	for(int i = 0; i < m_lbxJngo.GetCount(); i++)
    {
        m_lbxJngo.GetText(i, str);
        sz = pDC->GetTextExtent(str);
		
        if (sz.cx > dx)
			dx = sz.cx;
    }
    m_lbxJngo.ReleaseDC(pDC);
	
    if (m_lbxJngo.GetHorizontalExtent() < dx)
    {
        m_lbxJngo.SetHorizontalExtent(dx);
        ASSERT(m_lbxJngo.GetHorizontalExtent() == dx);
    }
}

void CIBKS_SampleDlg::OnSiseSend2() 
{
	// TODO: Add your control notification handler code here
	
	if(m_cbJType.GetCurSel() == 0)
	{
		if (!m_axIBKS.TR1802(TK_TR1802, "101GC000101H3000101H6000101H9000"))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "���� �������� �ü�", MB_OK|MB_ICONERROR);
			return;
		}
	}
	else
	{
		if (!m_axIBKS.TR1801(TK_TR1801, "006910078000000660005930"))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "�ֽ� �������� �ü�", MB_OK|MB_ICONERROR);
			return;
		}
	}	

}

void CIBKS_SampleDlg::OnSiseSend3() 
{
	// TODO: Add your control notification handler code here
	if (!m_axIBKS.TR1803(TK_TR1803, "0001"))
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "�����������Ȳ", MB_OK|MB_ICONERROR);
		return;
	}

}

void CIBKS_SampleDlg::OnSiseSend4() 
{
	// TODO: Add your control notification handler code here
	if (!m_axIBKS.TR1804(TK_TR1804, "0001"))
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "������ȸ", MB_OK|MB_ICONERROR);
		return;
	}
}

void CIBKS_SampleDlg::OnSiseSend5() 
{
	UpdateData(TRUE);

	m_jngoAccn = m_jngoAccn.Left(11);

	// TODO: Add your control notification handler code here
	if (!m_axIBKS.TR1211(TK_TR1211, m_jngoAccn, m_jngoPswd, "A000660", 0, 0, 0, 1, ""))
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "ü�᳻����ȸ", MB_OK|MB_ICONERROR);
		return;
	}
}

//�����ڵ�\t�����ϼ�\t��簡\tcd�ݸ�\t���簡\t�����ڻ�
void CIBKS_SampleDlg::OnGreeks() 
{
	// TODO: Add your control notification handler code here

	CString strData, strResult;
	m_axIBKS.TR3001(TK_GREEKS2, "101H6000" );//���� ���簡(�����ڻ��� �����ֱٿ����� �������)

	m_strGreeksCode = "201H4260";
	m_axIBKS.TR3002(TK_GREEKS1, m_strGreeksCode, "40314\t40310\t40332\t41023\t" );//�����ϼ�, ��簡, cd�ݸ�, ���簡
}

void CIBKS_SampleDlg::OnSonIk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	m_jngoAccn = m_jngoAccn.Left(11);

	if (!m_axIBKS.TR3232(TK_TR3232, m_jngoAccn, m_jngoPswd, 0, 1))//���ɴ������, ���ͷ�(��տ�Ź�ڻ����)
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "���� ���ϼ��� ��ȸ", MB_OK|MB_ICONERROR);
		return;
	}
}

void CIBKS_SampleDlg::OnMonSise() 
{
	// TODO: Add your control notification handler code here
	
	if (!m_axIBKS.TR3411(TK_TR3411, "201304"))//YYYYMM
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "���� ������ �ü� ��ȸ", MB_OK|MB_ICONERROR);
		return;
	}
}
