#if !defined(AFX_CHARTREALCONVERTER_H__A82B8802_19BA_49a7_AA4C_94E1AA76C9D5__INCLUDED_)
#define AFX_CHARTREALCONVERTER_H__A82B8802_19BA_49a7_AA4C_94E1AA76C9D5__INCLUDED_

#include "ChartRealDef.h"
// class CRealData_Common
// {
// public:
// 	CString m_strCode;
// 	CString m_strTime;
// 	CString m_strSign;
// 	CString m_strPrice;
// 	CString m_strChange;
// 	CString m_strChrate;
// 	CString m_strVolume;
// 
// 	CString m_strOpen;
// 	CString m_strHigh;
// 	CString m_strLow;
// 
// 	CString m_strValue;
// 
// 	int m_nMarketType;
// };

class CChartRealConverter
{
public:
	// �ֽ�
	static BOOL Convert_KXS3(LPCSTR pRealStruct, CRealData_Common& realData_Common);
	static BOOL Convert_KXYS(LPCSTR pRealStruct, CRealData_Common& realData_Common);

	// ELW
	static BOOL Convert_EWs3(LPCSTR pRealStruct, CRealData_Common& realData_Common);

	// ����
	static BOOL Convert_IXIJ(LPCSTR pRealStruct, CRealData_Common& realData_Common);

	// ����
	static BOOL Convert_FFC0(LPCSTR pRealStruct, CRealData_Common& realData_Common);

	// �ɼ�
	static BOOL Convert_OOC0(LPCSTR pRealStruct, CRealData_Common& realData_Common);

	// �ֽļ���
	static BOOL Convert_JFC0(LPCSTR pRealStruct, CRealData_Common& realData_Common);

	// �ֽĿɼ�
	static BOOL Convert_JOC0(LPCSTR pRealStruct, CRealData_Common& realData_Common);

	// �ؿ�
	static BOOL Convert_HWFJ(LPCSTR pRealStruct, CRealData_Common& realData_Common);

	// ��ǰ����
	static BOOL Convert_GTFJ(LPCSTR pRealStruct, CRealData_Common& realData_Common);

	// FO ����
	static BOOL Convert_FOJS(LPCSTR pRealStruct, CRealData_Common& realData_Common);
};

#endif //AFX_CHARTREALCONVERTER_H__A82B8802_19BA_49a7_AA4C_94E1AA76C9D5__INCLUDED_
