#include "stdafx.h"
#include "C_Tick.h"
#include "GrpWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd* pParent, void* cInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	struct _param*	param = (struct _param*)cInfo;
	auto pGrpWnd = std::make_unique<CGrpWnd>(pParent, param);
	
	pGrpWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, param->rect, pParent, 0);
	SetWindowPos(pGrpWnd->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	return (CWnd*)pGrpWnd.release();
}

/*
	data		- _cdTick
	graph		- line chart(1�к�, ƽ)
	parameter	- /d/r/e/u
	d : display count - 0:all
	r : request count
	e : data expand(true-��, false-ƽ)
	u : data unit - 1:����, 2:����, 3:����, 4:����, 5:ECN
	t : draw tick - 0: no 1: left, 2: right
	i : data index - 1:ƽ, 2:��
*/

__declspec(dllexport) CWnd* WINAPI axCreate2(CWnd* pParent, void* cInfo, CWnd* pwndWizard)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	struct _param*	param = (struct _param*)cInfo;
	auto pGrpWnd = std::make_unique<CGrpWnd>(pwndWizard, param);
	
	pGrpWnd->Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, param->rect, pParent, 0);
	SetWindowPos(pGrpWnd->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	return (CWnd*)pGrpWnd.release();
}