// CWrapper.cpp: 구현 파일
//

#include "StdAfx.h"
#include "IBXXXX99.h"
#include "CWrapper.h"

#include "defines.h"


// CWrapper

//IMPLEMENT_DYNAMIC(CWrapper, CWnd)

CWrapper::CWrapper()
{
	CString str;
}

CWrapper::~CWrapper()
{
}


BEGIN_MESSAGE_MAP(CWrapper, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CWrapper 메시지 처리기



#include <TlHelp32.h>
bool CWrapper::GetProcessModule(DWORD dwPID, char* sProcessName)
{
	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32 = { 0 };
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	if (hModuleSnap == (HANDLE)-1)
		return (FALSE);

	me32.dwSize = sizeof(MODULEENTRY32);
	CString slog;
	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면
	if (Module32First(hModuleSnap, &me32))
	{
		do
		{
			printf("process name : %s\n", me32.szModule);
			slog.Format("[mac] GetProcessModule =[%s] \r\n", me32.szModule);
			//	OutputDebugString(slog);
			if (strcmp(me32.szModule, sProcessName) == 0)
			{
				CloseHandle(hModuleSnap);
				return true;
			}
		} while (Module32Next(hModuleSnap, &me32));
	}

	CloseHandle(hModuleSnap);
	return false;
}

HANDLE CWrapper::ProcessFind(char* strProcessName)
{
	HANDLE         hProcessSnap = NULL;
	BOOL           bRet = FALSE;
	PROCESSENTRY32 pe32 = { 0 };
	CString strProcess, strTarget;
	strTarget.Format("%s", strProcessName);
	strTarget.TrimRight();

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == (HANDLE)-1)
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	CString slog;
	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32))
	{
		BOOL          bCurrent = FALSE;
		MODULEENTRY32 me32 = { 0 };

		do
		{
			//	bCurrent = GetProcessModule(pe32.th32ProcessID, strProcessName, pe32.szExeFile);
			slog.Format("[MAC] [%s] \r\n", pe32.szExeFile);
			OutputDebugString(slog);
			OutputDebugString("\r\n-----------------------------------------------\r\n");
			strProcess.Format("%s", pe32.szExeFile);
			if (strProcess.Find(strTarget) >= 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				//if (hProcess)
				//	TerminateProcess(hProcess, 0);
				return hProcess;
			}
		} while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	}
	CloseHandle(hProcessSnap);

	return nullptr;
}

int CWrapper::InitSharedMemory()
{

	return 0;
}

int CWrapper::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CString strtmp;
	strtmp = "SubAxis";
	m_Targethandle = ProcessFind((char*)strtmp.GetBuffer(0));
	m_hTarget = GetHwndFromProcessHandle(m_Targethandle);

	DWORD processID = GetCurrentProcessId();
	strtmp.Format("%s%d", "Axis", processID);
	m_hSharedM = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, strtmp);

	if (m_hTarget != nullptr)
	{
		COPYDATASTRUCT data;
		memset(&data, 0x00, sizeof(data));

		ST_COPY_DATA stdata{};
		stdata.iGubn = PS_CREATE_WND;
		stdata.hwnd = this->m_hWnd;
		stdata.pWizard = m_pWizard;

		memcpy(stdata.pdata, strtmp.GetBuffer(0), strtmp.GetLength());

		struct	_param param;
		param.rect = CRect(0, 0, 650, 600);
		stdata.pParam = param;
		stdata.pParam.rect = CRect(0, 0, 650, 600);

		data.dwData = 0;
		data.cbData = sizeof(stdata);
		data.lpData = &stdata;

		m_pSubWnd = (CWnd*) ::SendMessage(m_hTarget, WM_COPYDATA, 0, (LPARAM)&data);
		m_slog.Format("[wrapper]  m_pSubWnd=[%x]", m_pSubWnd);
		OutputDebugString(m_slog);
	}

	return 0;
}

LRESULT CWrapper::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	COPYDATASTRUCT* pdata = (COPYDATASTRUCT*)lParam;
	switch (message)
	{
	case WM_COPYDATA:
	{
		CString retvalue{};
		_ST_COPY_DATA_TO_HTS stData;
		memcpy(&stData, pdata->lpData, sizeof(stData));

		switch (stData.iGubn)
		{
			case PS_SUB2HTS_MSG:
			{

				switch (stData.iMsgType)
				{
					case PS_SUB2HTS_FP_WPTYPE1:  //MAKEWPARAM
					{
						BYTE byte = LOWORD(stData.wparam);
						switch (byte)
						{
							case variantDLL:
							{
								int ival = HIWORD(stData.wparam);
								BYTE* pbyte = (BYTE*)stData.lpdata;
								char* dta = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, ival), (LPARAM)pbyte);
								retvalue.Format("%s", dta); 
								retvalue.TrimRight();
								LPVOID pshared = SetData_SharedM((BYTE*)retvalue.GetBuffer(0), retvalue.GetLength(), variantDLL);

								if (retvalue.GetLength() > 0)
									return (LRESULT)1;
								else
									return (LRESULT)0;
							}
						}
					}
					break;
					case PS_SUB2HTS_FP_WPTYPE2:
					{
						switch (stData.wparam)
						{
							case getBRUSH:
							{
								//(CBrush*)m_pWizard->SendMessage(WM_USER, getBRUSH, (long)clr);


							}
							break;
							case getBITMAP:
							{
								BYTE* pbyte = (BYTE*)stData.lpdata;
								CBitmap* bitmap = (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)pbyte);
								LPVOID pshared = SetData_SharedM((BYTE*)bitmap, 0, getBITMAP);
							}
							break;
							case 0x99:
							{
								//char* presult = (char*)m_pWizard->SendMessage(WM_USER, getBRUSH, (long)lParam);
								//LPVOID pshared = SetData_SharedM((BYTE*)m_pWizard, 4);
								///TestCode();
							}
							break;
						}
					}
					break;
				}
				/*HDC hdc = ::GetDC(stData.hwnd);
				CDC* cdc = CDC::FromHandle(hdc);


				struct	_param pParam = stData.pParam;

				CWrapWnd* m_pWnd = new CControlWnd(stData.pWizard);
				m_pWnd->m_pParentHwnd = stData.hwnd;

				m_pWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP, pParam.rect, CWnd::FromHandle(stData.hwnd), 100);
				return (LRESULT)this->m_hWnd;*/
			}
			break;
			}
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

LPVOID CWrapper::SetData_SharedM(BYTE* pdata, int len, int igubn)
{
	if (m_hSharedM == nullptr)
		return 0;

	LPVOID pSharedMemory = MapViewOfFile(
		m_hSharedM,  // 공유 메모리 핸들
		FILE_MAP_WRITE, // 공유 메모리 영역에 대한 액세스 제어
		0,              // 공유 메모리 영역 오프셋 상위 32비트
		0,              // 공유 메모리 영역 오프셋 하위 32비트
		0               // 공유 메모리 영역 크기 (0이면 전체 영역)
	);

	InitializeCriticalSection(&csMapHandle);
	EnterCriticalSection(&csMapHandle);

	if (pSharedMemory != NULL) {
		// 공유 메모리에 데이터를 씁니다.
		//공유 메모리의 맨앞의 데이터 8 바이트는 예약되어있는 주소
		//4바이트 = 전달해주는 주소가 시작하는 index
		//4바이트 = 전달해주는 데이터의 길이
		memset(pSharedMemory, 0x00, 100);
		int idata = 12;
		BYTE* pbyte = (BYTE*)pSharedMemory;
		memcpy(pbyte + SIZE_DATA_DEFINE, (int*)&idata, SIZE_DATA_START);								//Data 시작 index
		memcpy(pbyte + SIZE_DATA_DEFINE + SIZE_DATA_START, (int*)&len, SIZE_DATA_LEN);     //Data 길이
		
		switch (igubn)
		{
			case getBITMAP:
			{
				CBitmap* pbitmap = new CBitmap;
				(CBitmap*)(pbyte + SIZE_DATA_DEFINE + SIZE_DATA_START + SIZE_DATA_LEN);
				pbitmap->Attach((CBitmap*)pdata);
			}
			break;
			case variantDLL:
			{
				memcpy((BYTE*)pbyte + SIZE_DATA_DEFINE + SIZE_DATA_START + SIZE_DATA_LEN, (BYTE*)pdata, len);  //Data 실제
			}
			break;
			case 0:
			{
				memcpy((BYTE*)pbyte + SIZE_DATA_DEFINE + SIZE_DATA_START + SIZE_DATA_LEN, (BYTE*)pdata, len);  //Data 실제
			}
			break;
		}

		LeaveCriticalSection(&csMapHandle);
		DeleteCriticalSection(&csMapHandle);
		// 공유 메모리 매핑 해제
	//	UnmapViewOfFile(pSharedMemory);
		return pSharedMemory;
	}
	return nullptr;
}

void CWrapper::TestCode()
{


	if (m_hSharedM != NULL)
	{
		// 공유 메모리 영역을 프로세스의 메모리에 매핑
		LPVOID pSharedMemory = MapViewOfFile(
			m_hSharedM,  // 공유 메모리 핸들
			FILE_MAP_READ,  // 공유 메모리 영역에 대한 액세스 제어
			0,              // 공유 메모리 영역 오프셋 상위 32비트
			0,              // 공유 메모리 영역 오프셋 하위 32비트
			0               // 공유 메모리 영역 크기 (0이면 전체 영역)
		);

		if (pSharedMemory != NULL) {
			// 공유 메모리에서 데이터를 읽어옵니다.
			// 읽은 데이터를 사용합니다.

			// 읽은 데이터를 사용합니다.
			CString strdata;
			BYTE* pShare = (BYTE*)pSharedMemory;
			int iStart{}, iLen{};
			// 공유메모리 
			int istart = *(int*)(pShare + SIZE_DATA_DEFINE);
			int ilen = *(int*)(pShare + SIZE_DATA_DEFINE + SIZE_DATA_START);
			ilen = 5;
			BYTE* pResult = new BYTE[ilen + 1];
			memset(pResult, 0x00, ilen + 1);
			memcpy(pResult, pShare + 12, ilen);

			CWnd* pwnd = (CWnd*)m_pWizard->Attach(m_pWizard->GetSafeHwnd());
			CBrush* pbrush = (CBrush*)m_pWizard->SendMessage(WM_USER, getBRUSH, (long)RGB(255, 0, 0));
			pbrush = (CBrush*)((pwnd)->SendMessage(WM_USER, getBRUSH, (long)RGB(255, 0, 0)));

			// 공유 메모리 매핑 해제
			UnmapViewOfFile(pSharedMemory);
		}

		// 공유 메모리 핸들 닫기
		CloseHandle(pSharedMemory);
	}
}