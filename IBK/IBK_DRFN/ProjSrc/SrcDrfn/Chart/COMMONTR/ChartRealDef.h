#if !defined(AFX_CHARTREALDEFINE_H__A82B8802_19BA_49a7_AA4C_94E1AA76C9D5__INCLUDED_)
#define AFX_CHARTREALDEFINE_H__A82B8802_19BA_49a7_AA4C_94E1AA76C9D5__INCLUDED_

class CRealData_Common
{
public:
	CString m_strCode;		// [�����ڵ�]
	CString m_strTime;		// [ü��ð�]
//@solomon	�����ð�(Ȯ�εǸ� �ٽ� ����..)
//	CString m_strTime2;		// [����ü��ð�]
	CString m_strSign;		// [1:����]
	CString m_strPrice;		// [���簡,     ��]
	CString m_strChange;	// [���ϴ��,   ��]
	CString m_strChrate;	// [�����]
	CString m_strVolume;	// [�����ŷ���, ��]

	CString m_strOpen;		// [�ð�,       ��]
	CString m_strHigh;		// [��,       ��]
	CString m_strLow;		// [����,       ��]

	CString m_strValue;		// [�ŷ����,       ��]

	int		m_nMarketType;
	BOOL	m_bIsRealJang;	// ����� ���� (TRUE : ��ð�, FALSE : �ð���, ������..)

	CString	m_strOpenyak;
};

class CRealData_Common_FX
{
public:
	CString m_strCode;						// [�����ڵ�]
	CString m_strTime;						// [ü��ð�]

	CRealData_Common	m_RealData_S;		// �ŵ�
	CRealData_Common	m_RealData_B;		// �ż�
};
#endif //AFX_CHARTREALDEFINE_H__A82B8802_19BA_49a7_AA4C_94E1AA76C9D5__INCLUDED_
