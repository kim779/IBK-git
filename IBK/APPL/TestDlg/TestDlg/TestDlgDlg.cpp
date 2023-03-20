
// TestDlgDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "TestDlg.h"
#include "TestDlgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestDlgDlg 대화 상자



CTestDlgDlg::CTestDlgDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTDLG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BRN_BIT, &CTestDlgDlg::OnBnClickedBrnBit)
	ON_BN_CLICKED(IDC_BRN_BIT2, &CTestDlgDlg::OnBnClickedBrnBit2)
	ON_BN_CLICKED(IDC_BTN_mfc140, &CTestDlgDlg::OnBnClickedBtnmfc140)
	ON_EN_CHANGE(IDC_EDIT_DATA, &CTestDlgDlg::OnEnChangeEditData)
	ON_BN_CLICKED(IDC_BTN_FORMAT, &CTestDlgDlg::OnBnClickedBtnFormat)
	ON_BN_CLICKED(IDC_BTN_MAP, &CTestDlgDlg::OnBnClickedBtnMap)
	ON_BN_CLICKED(IDC_BTN_RACE, &CTestDlgDlg::OnBnClickedBtnRace)
	ON_BN_CLICKED(IDC_BTN_CONSUMER, &CTestDlgDlg::OnBnClickedBtnConsumer)
	ON_BN_CLICKED(IDC_BTN_CONSUMER2, &CTestDlgDlg::OnBnClickedBtnConsumer2)
	ON_BN_CLICKED(IDC_BTN_FUTURE, &CTestDlgDlg::OnBnClickedBtnFuture)
	ON_BN_CLICKED(IDC_BTN_FUTURE2, &CTestDlgDlg::OnBnClickedBtnFuture2)
	ON_BN_CLICKED(IDC_BTN_ASYNC, &CTestDlgDlg::OnBnClickedBtnAsync)
	ON_BN_CLICKED(IDC_BTN_RLEF, &CTestDlgDlg::OnBnClickedBtnRlef)
	ON_BN_CLICKED(IDC_BTN_PAIR, &CTestDlgDlg::OnBnClickedBtnPair)
	ON_BN_CLICKED(IDC_BTN_TUPLE, &CTestDlgDlg::OnBnClickedBtnTuple)
	ON_BN_CLICKED(IDC_BTN_LAMDA, &CTestDlgDlg::OnBnClickedBtnLamda)
	ON_BN_CLICKED(IDC_BTN_LAMDA_FIND, &CTestDlgDlg::OnBnClickedBtnLamdaFind)
	ON_BN_CLICKED(IDC_BTN_OS, &CTestDlgDlg::OnBnClickedBtnOs)
	ON_BN_CLICKED(IDC_BTN_OSVERSION, &CTestDlgDlg::OnBnClickedBtnOsversion)
	ON_BN_CLICKED(IDC_BTN_HASHKEY, &CTestDlgDlg::OnBnClickedBtnHashkey)
END_MESSAGE_MAP()


// CTestDlgDlg 메시지 처리기

BOOL CTestDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestDlgDlg::OnBnClickedBrnBit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int xval = 0x2a;
	int xdata = 0x08;

	int iret = xval & xdata;
	if (xval & xdata)
		TRACE("TRUE");
	else
		TRACE("FALSE");
}

double RoundDown(double data) //매체수수료(절삭)
{
	return ((int)((data * 0.1)) / 0.1);
}

void CTestDlgDlg::OnBnClickedBrnBit2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dMaip{}, m_dMass{}, m_dSave{}, symt{};
	m_dMass = 0.0010;
	CString m_maip;
	m_maip = "5690000";
	dMaip = atof(m_maip) + RoundDown((atof(m_maip) + symt) * m_dMass) + m_dSave;//매입가
}


void CTestDlgDlg::OnBnClickedBtnmfc140()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileFind ff;
	CString strpath, strfile;
	((CWnd*)GetDlgItem(IDC_EDIT_DATA))->GetWindowText(strfile);
	strfile.Format("C:\\Windows\\System32\\%s", strfile);

	if (ff.FindFile(strfile))
	{
		AfxMessageBox("find!!!");
	}
	else
	{
		AfxMessageBox("no find!!!");
	}
}


void CTestDlgDlg::OnEnChangeEditData()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

inline CString FORMAT(const TCHAR* formatString, ...)
{
	CString s;
	va_list valist;
	va_start(valist, formatString);
	s.FormatV(formatString, valist);
	va_end(valist);
	return s;
}

void CTestDlgDlg::OnBnClickedBtnFormat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	str = FORMAT("%s|%s|%s|%s", "1", "2", "3", "4");
}



using _hjcode = struct hjcodex;
void CTestDlgDlg::OnBnClickedBtnMap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	std::map <CString, _hjcode*> _mapCODEx;
	std::vector<_hjcode*> _vETNx;
	std::map<CString, std::pair<CString, int>> _mapACODE;
	std::map<CString, CString>			      _mapNAME;

	CFile	file;
	CString	string = AxStd::FORMAT("%s", "hjcode3.dat");
	if (AxStd::isFileExist(string))
	{
		if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		const int  size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		if (size > len)
		{
			m_hcB.reset();
			return;
		}

		const int count = len / sizeof(struct hjcodex);
		struct	hjcodex* hjc = (struct hjcodex*)m_hcB.get();

		for (int ii = 0; ii < count; ii++, hjc++)
		{
			CString sCode = CString(hjc->code, HCodeLen).Mid(1);
			_mapCODEx.emplace(std::move(sCode), hjc);
			if (hjc->ssgb == jmELW)
				continue;

			if (hjc->ssgb == jmETN)	//2014.10.28 KSJ ETN 추가
			{
				_vETNx.emplace_back(hjc);
				continue;
			}

			_mapACODE.emplace(std::move(sCode), std::move(std::make_pair(CString(hjc->hnam, HNameLen).Mid(1).Trim(), 0)));
			_mapNAME.emplace(std::move(CString(hjc->hnam, HNameLen).Mid(1).Trim()), std::move(sCode));
		} //for

	} //if

	std::map<CString, struct hjcodex*> _mapCODE;
	auto future = std::async([&]() {
		for (auto mt : _mapCODEx)
			_mapCODE.emplace(mt);
		});

	const std::chrono::milliseconds wtime(100);
	while (future.wait_for(wtime) != std::future_status::ready)
		AxStd::_Msg("Thread runing");

	std::vector<hjcodex*> vData;
	for_each(_mapCODE.begin(), _mapCODE.end(), [&vData](auto item) {
		/*if (item.second->kosd == jmKOSPI &&
			item.second->ssgb != jmSINJU &&
			item.second->ssgb != jmSINJS &&
			item.second->ssgb != jmHYFND &&
			item.second->ssgb != jmELW &&
			item.second->ssgb != jmFOREIGN)*/
		vData.emplace_back(item.second);
		});
}

void worker(int& counter)
{
	for (int i = 0; i < 10000; i++)
		counter += 1;
}
void workerWm(int& result, std::mutex& m)
{
	for (int i = 0; i < 10000; i++)
	{
		m.lock();
		result += 1;
		m.unlock();
	}
}

void CTestDlgDlg::OnBnClickedBtnRace()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int  counter = 0;

	std::vector<std::thread> workers;
	for (int i = 0; i < 4; i++)
	{
		workers.push_back(std::thread(worker, std::ref(counter)));
	}

	for (int i = 0; i < 4; i++)
	{
		workers[i].join();
	}

	CString stmp;
	stmp.Format("%d", counter);
	TRACE(stmp);


	counter = 0;
	std::mutex m;
	std::vector<std::thread> workerWms;
	for (int i = 0; i < 4; i++)
	{
		workerWms.push_back(std::thread(workerWm, std::ref(counter), std::ref(m)));
	}

	for (int i = 0; i < 4; i++)
	{
		workerWms[i].join();
	}

	stmp.Format("%d", counter);
	TRACE(stmp);
}


void producer(std::queue<std::string>* downloaded_pages, std::mutex* m, int index)
{
	CString slog;
	for (int i = 0; i < 5; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
		std::string content = "웹사이트 : " + std::to_string(i) + " from thread(" + std::to_string(index) + ")";
		slog.Format("+++++ producer [%s]\n", content.c_str());
		OutputDebugString(slog);
		m->lock();
		downloaded_pages->push(content);
		m->unlock();
	}
}

void consumer(std::queue<std::string>* downloaded_pages, std::mutex* m, int* num_processed)
{
	//전체 처리하는 페이지 개수가 5 * 5 = 25 개
	CString slog;
	while (*num_processed < 25)
	{
		m->lock();
		if (downloaded_pages->empty())
		{
			m->unlock();
			std::this_thread::sleep_for(std::chrono::microseconds(10));
			continue;
		}

		//맨 앞의 페이지를 읽고 대기 목록에서 제거한다.
		std::string content = downloaded_pages->front();
		downloaded_pages->pop();

		(*num_processed)++;
		m->unlock();

		//content를 처리한다.
		slog.Format("----consumer %s----\n", content.c_str());
		//	OutputDebugString(slog);
		std::this_thread::sleep_for(std::chrono::microseconds(80));
	}
}


void CTestDlgDlg::OnBnClickedBtnConsumer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::queue<std::string> downloaded_pages;
	std::mutex m;

	std::vector<std::thread> producers;
	for (int i = 0; i < 5; i++)
	{
		producers.push_back(std::thread(producer, &downloaded_pages, &m, i + 1));
	}

	int num_processed = 0;
	std::vector<std::thread> consumers;

	for (int i = 0; i < 3; i++)
	{
		consumers.push_back(std::thread(consumer, &downloaded_pages, &m, &num_processed));
	}

	for (int i = 0; i < 5; i++)
		producers[i].join();

	for (int i = 0; i < 3; i++)
		consumers[i].join();

}

void producer_c(std::queue<std::string>* downloaded_pages, std::mutex* m, int index, std::condition_variable* cv)
{
	for (int i = 0; i < 5; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
		std::string content = "웹사이트 :" + std::to_string(i) + " from thread(" + std::to_string(index) + ")\n";
		m->lock();
		downloaded_pages->push(content);
		m->unlock();

		cv->notify_one();
	}
}

void consumer_c(std::queue<std::string>* downloaded_pages, std::mutex* m, int* num_processed, std::condition_variable* cv)
{
	CString slog;
	while (*num_processed < 25)
	{
		std::unique_lock<std::mutex> lk(*m);
		cv->wait(
			lk, [&] { return !downloaded_pages->empty() || *num_processed == 25; }
		);
		if (*num_processed == 25)
		{
			lk.unlock();
			return;
		}

		//맨앞의 페이지를 읽고 대기 목록에서 제거한다.
		std::string content = downloaded_pages->front();
		downloaded_pages->pop();

		(*num_processed)++;
		lk.unlock();

		//content를 처리한다.
		slog.Format("----consumer [%s]--", content.c_str());
		OutputDebugString(slog);
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
	}
}

void CTestDlgDlg::OnBnClickedBtnConsumer2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::queue <std::string> downloaded_pages;
	std::mutex m;
	std::condition_variable cv;

	std::vector<std::thread> producers;
	for (int i = 0; i < 5; i++) {
		producers.push_back(
			std::thread(producer_c, &downloaded_pages, &m, i + 1, &cv));
	}

	int num_processed = 0;
	std::vector<std::thread> consumers;
	for (int i = 0; i < 3; i++) {
		consumers.push_back(
			std::thread(consumer_c, &downloaded_pages, &m, &num_processed, &cv));
	}
	for (int i = 0; i < 5; i++) {
		producers[i].join();
	}

	cv.notify_all();

	for (int i = 0; i < 3; i++) {
		consumers[i].join();
	}


}

void fworker(std::promise<string>* p)
{//약속을 이행하는 모습 해당 결과는 future 에 들어간다.
	p->set_value("some data");
}

//future 
void CTestDlgDlg::OnBnClickedBtnFuture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::promise<string> p;

	//미래에 string 데이터를 돌려 주겠다는 약속
	std::future<string> data = p.get_future();
	std::thread t(fworker, &p);

	//미래에 약속된 데이터를 받을 때 까지 기다린다.
	data.wait(); //이코드가 실행되면 

	//wait 이 리턴했다는 뜻이 future에 데이터가 준비되었다는 의미

	CString stmp;
	stmp.Format("------[%s] ---- - \n", data.get().c_str());
	t.join();

}


void pcworker(std::promise<string>* p)
{
	try
	{
		throw std::runtime_error("some error");
	}
	catch (...)
	{
		p->set_exception(std::current_exception());
	}
}


void CTestDlgDlg::OnBnClickedBtnFuture2()  //생산자 소비자 패턴을  future-promise로 구현
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::promise<string> p;

	std::future<string> data = p.get_future();

	std::thread t(pcworker, &p);

	data.wait();

	try
	{
		data.get();
	}
	catch (const std::exception& e)
	{
		CString slog;
		slog.Format("except [%s]", e.what());
	}
}




int test_sum(const std::vector<int>& v, int start, int end)
{
	int total = 0;

	for (int i = start; i < end; i++)
	{
		total += v[i];
	}
	return total;
}

int parallel_sum(const std::vector<int>& v)
{
	std::future<int> lower_half_future =
		std::async(std::launch::async, test_sum, cref(v), 0, v.size() / 2);

	int upper_half = test_sum(v, v.size() / 2, v.size());

	return lower_half_future.get() + upper_half;
}

void CTestDlgDlg::OnBnClickedBtnAsync()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	std::vector<int> v;
	v.reserve(1000);

	for (int i = 0; i < 1000; ++i)
	{
		v.push_back(i + 1);
	}

	int result;
	result = parallel_sum(v);


	std::vector<std::pair<CString, CString>> _va;

	CString stmp, stemp;
	for (int ii = 0; ii < 100; ii++)
	{
		stmp.Format("%d", ii);
		stemp.Format("%d", ii + 1);
		_va.emplace_back(std::make_pair(stmp, stemp));
	}

	std::vector<std::tuple<int, CString, CString>> _vta;
	int i = 0;
	auto future = std::async([&]() {
		for (auto mt : _va)
			_vta.emplace_back(std::make_tuple(i++, mt.first, mt.second));
		});

	const std::chrono::milliseconds wtime(100);
	while (future.wait_for(wtime) != std::future_status::ready)
		AxStd::_Msg("Thread runing");


}

class A
{
public:
	int idata;

	A(int ival) { idata = ival; }
	A(const  A& a) { idata = a.idata; }
};


class MyString {
	char* string_content;  // 문자열 데이터를 가리키는 포인터
	int string_length;     // 문자열 길이

	int memory_capacity;  // 현재 할당된 용량

public:
	MyString();

	// 문자열로 부터 생성
	MyString(const char* str);

	// 복사 생성자
	MyString(const MyString& str);

	void reserve(int size);
	MyString operator+(const MyString& s);
	~MyString();

	int length() const;

	void print();
	void println();
};

MyString::MyString() {
	std::cout << "생성자 호출 ! " << std::endl;
	string_length = 0;
	memory_capacity = 0;
	string_content = nullptr;
}

MyString::MyString(const char* str) {
	std::cout << "생성자 호출 ! " << std::endl;
	string_length = strlen(str);
	memory_capacity = string_length;
	string_content = new char[string_length];

	for (int i = 0; i != string_length; i++) string_content[i] = str[i];
}
MyString::MyString(const MyString& str) {
	std::cout << "복사 생성자 호출 ! " << std::endl;
	string_length = str.string_length;
	memory_capacity = str.string_length;
	string_content = new char[string_length];

	for (int i = 0; i != string_length; i++)
		string_content[i] = str.string_content[i];
}
MyString::~MyString() { delete[] string_content; }
void MyString::reserve(int size) {
	if (size > memory_capacity) {
		char* prev_string_content = string_content;

		string_content = new char[size];
		memory_capacity = size;

		for (int i = 0; i != string_length; i++)
			string_content[i] = prev_string_content[i];

		if (prev_string_content != nullptr) delete[] prev_string_content;
	}
}
MyString MyString::operator+(const MyString& s) {
	MyString str;
	str.reserve(string_length + s.string_length);
	for (int i = 0; i < string_length; i++)
		str.string_content[i] = string_content[i];
	for (int i = 0; i < s.string_length; i++)
		str.string_content[string_length + i] = s.string_content[i];
	str.string_length = string_length + s.string_length;
	return str;
}
int MyString::length() const { return string_length; }
void MyString::print() {
	for (int i = 0; i != string_length; i++) std::cout << string_content[i];
}
void MyString::println() {
	for (int i = 0; i != string_length; i++) std::cout << string_content[i];

	std::cout << std::endl;
}

int main() {
	MyString str1("abc");
	MyString str2("def");
	std::cout << "-------------" << std::endl;
	MyString str3 = str1 + str2;
	str3.println();
}

void CTestDlgDlg::OnBnClickedBtnRlef()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	A a(1);
	A b(a);
}


void CTestDlgDlg::OnBnClickedBtnPair()
{
	std::map<CString, std::pair<CString, int>> _mapACODE;
	std::map<CString, CString> _mapNAME;

	CFile	file;
	CString	string = AxStd::FORMAT("%s", "hjcode3.dat");
	if (AxStd::isFileExist(string))
	{
		if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		const int size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		if (size > len)
		{
			m_hcB.reset();
			return;
		}

		const int count = len / sizeof(struct hjcodex);
		struct hjcodex* hjc = (struct hjcodex*)m_hcB.get();

		for (int ii = 0; ii < count; ii++, hjc++)
		{
			CString sCode = CString(hjc->code, HCodeLen).Mid(1);
			_mapACODE.emplace(std::move(sCode), std::move(std::make_pair(CString(hjc->hnam, HNameLen).Mid(1).Trim(), 0)));
			_mapNAME.emplace(std::move(CString(hjc->hnam, HNameLen).Mid(1).Trim()), std::move(sCode));
		}

	}
}
//------------------------------------------------------------------------------------------------------------------------------------------
struct _efopcode
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	code[6]{};			/*종목코드*/
	char	hnam[39]{};			/*종목명*/
	char	opcd[6]{};			/*운용사 코드*/
	char	etfm[1]{};			/*테마코드*/
	char	etfl[1]{};			/*국내외 구분*/
};

struct _efopitem
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	opcd[6]{};			/*운용사 코드*/
	char	hnam[30]{};			/*운용사 한글약명*/
	char	lnfd[1]{};			/*라인피드*/

	//	_efopcode *efopcode;
};


struct _efoptema
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	tmcd[1]{};			/*테마코드*/
	char	tmnm[20]{};			/*테마명*/
	char	lnfd[1]{};			/*라인피드*/
};

struct _efopfore
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	frcd[1]{};			/*국내외 구분('D':국내, 'F':해외)*/
	char	frnm[10]{};			/*국내외 구분명*/
	char	lnfd[1]{};			/*라인피드*/
};

//void test_compare(std::vector<std::pair<CString, CString>> a, std::vector<std::pair<CString, CString>> b)
template <typename T>
void test_compare()
{
	CString tmp, temp;
	bool operator()(const T & a, const T & b)  const {}
}

void CTestDlgDlg::OnBnClickedBtnTuple()
{
	std::vector<std::tuple<char, CString, CString>>	_vETFitem;
	std::vector<std::tuple<char, char, CString>>	_vETFtema;
	std::vector<std::tuple<char, char, CString>>	_vETFfore;
	std::vector<std::tuple<char, CString, CString, CString, char, char>> _vETFcode;

	CString sfile;
	sfile.Format("%s", "efcode.dat");

	CStdioFile cfile(sfile, CFile::modeRead);

	CString line;
	while (cfile.ReadString(line))
	{
		switch (const char gubn = line.GetAt(0); gubn)
		{
		case '1':
		{
			const _efopitem* efopitem = (struct _efopitem*)line.GetBuffer();
			_vETFitem.emplace_back(std::move(std::tuple(efopitem->type[0],
				CString(efopitem->opcd, sizeof(efopitem->opcd)).Trim(),
				CString(efopitem->hnam, sizeof(efopitem->hnam)).Trim())));
		}
		break;
		case '2':
		{
			const _efoptema* efoptema = (struct _efoptema*)line.GetBuffer();
			_vETFtema.emplace_back(std::move(std::tuple(efoptema->type[0],
				efoptema->tmcd[0],
				CString(efoptema->tmnm, sizeof(efoptema->tmnm)).Trim())));
		}
		break;
		case '3':
		{
			const _efopfore* efopfore = (struct _efopfore*)line.GetBuffer();
			_vETFfore.emplace_back(std::move(std::tuple(efopfore->type[0], efopfore->frcd[0],
				CString(efopfore->frnm, sizeof(efopfore->frnm)).Trim())));
		}
		break;
		case '4':
		{
			const _efopcode* efopcode = (struct _efopcode*)line.GetBuffer();
			_vETFcode.emplace_back(std::move(std::tuple(efopcode->type[0],
				CString(efopcode->code, sizeof(efopcode->code)).Trim(),
				CString(efopcode->hnam, sizeof(efopcode->hnam)).Trim(),
				CString(efopcode->opcd, sizeof(efopcode->opcd)).Trim(),
				efopcode->etfm[0],
				efopcode->etfl[0]
			)));
		}
		break;
		}
	}

	std::vector <std::pair<CString, CString>> _vData;
	for_each(_vETFcode.begin(), _vETFcode.end(), [&](const auto item) {
		auto [type, code, hnam, opcd, etfm, etfl] = item;
		_vData.emplace_back(std::move(std::make_pair(code, hnam)));
		});

	std::sort(_vData.begin(), _vData.end());

	CString name;
	for_each(_vData.begin(), _vData.end(), [&](const auto item) {
		name = item.second;
		auto [CString, CSting] = item;
		name.MakeUpper();
		});

	auto item = _vData.at(0);
	CString strdata;
	strdata = std::get<0>(item);
	/*
	for_each(vSearch.begin(), vSearch.end(), [&](const auto item){
		name = item.second;
		name.MakeUpper();
		sName.MakeUpper();
		if (name.Find(sName) >= 0)
			vText.emplace_back(item);
	});
	*/
}

template <typename Iter>
void testprint(Iter begin, Iter end)
{
	OutputDebugString("--------------------------\n");
	CString stmp;
	while (begin != end)
	{
		stmp.Format(" %d\n", *begin);
		OutputDebugString(stmp);
		begin++;
	}
}

void CTestDlgDlg::OnBnClickedBtnLamda()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	auto func = [](int i) { return i % 2 == 1; };
//	auto ret = func(4);


	std::vector<int> vec;
	vec.push_back(5);
	vec.push_back(3);
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);

	testprint(vec.begin(), vec.end());

	int num_erased = 0;
	vec.erase(std::remove_if(vec.begin(), vec.end(),
		[&num_erased](int i) {
			if (num_erased >= 2)
				return false;
			else if (i % 2 == 1) {
				num_erased++;
				return true;
			}
			return false;
		}),
		vec.end());
	testprint(vec.begin(), vec.end());


}

void getHjcodeMap(std::map<CString, _hjcode>& map)
{
	std::map <CString, _hjcode*> _mapCODEx;
	std::vector<_hjcode*> _vETNx;
	std::map<CString, std::pair<CString, int>> _mapACODE;
	std::map<CString, CString>			      _mapNAME;
	std::unique_ptr<char[]> m_hcB;

	CFile	file;
	CString	string = AxStd::FORMAT("%s", "hjcode3.dat");
	if (AxStd::isFileExist(string))
	{
		if (!file.Open(string, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		const int  size = gsl::narrow_cast<int>(file.GetLength());
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		if (size > len)
		{
			m_hcB.reset();
			return;
		}

		const int count = len / sizeof(struct hjcodex);
		struct	hjcodex* hjc = (struct hjcodex*)m_hcB.get();

		for (int ii = 0; ii < count; ii++, hjc++)
		{
			CString sCode = CString(hjc->code, HCodeLen).Mid(1);
			_mapCODEx.emplace(std::move(sCode), hjc);

			map.emplace(sCode, *hjc);

			//	_mapACODE.emplace(std::move(sCode), std::move(std::make_pair(CString(hjc->hnam, HNameLen).Mid(1).Trim(), 0)));
			//	_mapNAME.emplace(std::move(CString(hjc->hnam, HNameLen).Mid(1).Trim()), std::move(sCode));
		} //for
	} //if

	//std::map<CString, struct hjcodex*> _mapCODE;
	//auto future = std::async([&]() {
	//	for (auto mt : _mapCODEx)
	//		_mapCODE.emplace(mt);
	//	});

	//const std::chrono::milliseconds wtime(100);
	//while (future.wait_for(wtime) != std::future_status::ready)
	//	AxStd::_Msg("Thread runing");

}

void CTestDlgDlg::OnBnClickedBtnLamdaFind()
{
	std::map<CString, _hjcode> _map;
	getHjcodeMap(_map);
	//strdata = std::get<0>(item);const auto hjc
	std::vector< _hjcode> _vt;
	CString name;
	for (auto& item : _map)
	{
		_hjcode hjc = item.second;
		name.Format("%s", hjc.hnam);

		if( name.Find("타") >= 0)
		{
			_vt.emplace_back(std::move(hjc));
		}
	}
	//auto it  = std::remove_if(_map.begin(), _map.end(), [&](const auto item)->bool{
	//		name = CString(hjc->hnam, sizeof(hjc->hnam)).Mid(1);
	//	
	//		CString strUpperS, strUpperO;
	//		strUpperS = cName;
	//		strUpperO = name.GetAt(ii);
	//		strUpperS.MakeUpper();
	//		strUpperO.MakeUpper();

	//		if (strUpperS != strUpperO)
	//			return true;
	//		return false;
	//	});

}
#include <stdio.h>
typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;
void CTestDlgDlg::OnBnClickedBtnOs()
{
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	
	if (fnIsWow64Process == NULL)
		AfxMessageBox("check fail");

	bool bProcess64 = false;
#if defined(_WIN64)
	bProcess64 = FALSE;
#elif defined(_WIN32)
	BOOL f64 = FALSE;
	bProcess64 = fnIsWow64Process(GetCurrentProcess(), &f64) && f64;
#endif

}


void CTestDlgDlg::OnBnClickedBtnOsversion()
{
	/*OSVERSIONINFOEX info{};
	SYSTEM_INFO sysinfo{};
	MEMORYSTATUS memStatus{};

	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx((OSVERSIONINFO *)&info);*/
}

UINT AFXAPI Hashdata(LPCSTR key)
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;
	return nHash;
}

void CTestDlgDlg::OnBnClickedBtnHashkey()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sname;
	sname = "김현식";
	CString sdata;
	sdata.Format("%08u", Hashdata((LPCSTR)sname));
}
