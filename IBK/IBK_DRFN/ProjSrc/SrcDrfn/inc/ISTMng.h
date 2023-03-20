#if !defined(AFX_INTERFACE_OF_MULTISTMANAGER_1464_45BB_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_MULTISTMANAGER_1464_45BB_BY_JUNOK_LEE__INCLUDED_

// �뵵 : ������ ���õ� ������ �����Ѵ�.

// #include "../../_include/ISTMng.h"
#include "IBaseDefine.h"

#define	UUID_ISTManager				41

DECLARE_INTERFACE(ISTManager)
{
	// �������࿩�θ� �����Ѵ�.
	// I-PARAM : szSTPath ����FullPath, bAdd:TRUE�̸� +, FALSE�̸� -
	// O-PARAM : 
	STDMETHOD_(int, AddSTRunINfo)(LPCSTR szSTPath, BOOL bAdd) PURE;

	/* src\Include_ST\IStrategyItem.h ����.
	enum OPERATTIONSTATUS {
		STRATEGY_NONE = 0,
		STRATEGY_RUN,
		STRATEGY_PAUSE,
		STRATEGY_STOP,
		STRATEGY_STANDBY
	*/

	// �������� ������ �˾ƿ´�.
	// I-PARAM : szSTPath ����FullPath
	// O-PARAM : ����ǰ� �ִ� ī��Ʈ, ����ǰ� ������ 0�̻�
	STDMETHOD_(int, GetSTRunINfo)(LPCSTR szSTPath) PURE;
};


#endif //AFX_INTERFACE_OF_MULTISTMANAGER_1464_45BB_BY_JUNOK_LEE__INCLUDED_
