#if !defined(AFX_INTERFACE_OF_ST_ADDINMANAGER_BY_JUNOKLEE_060721A__INCLUDED_)
#define AFX_INTERFACE_OF_ST_ADDINMANAGER_BY_JUNOKLEE_060721A__INCLUDED_

// #include "../../inc/ISTAddinManager.h" // �ý���Ʈ���̵� ���õ� ������ �������̽�

#define		UUSTID_ISTAddinData		0

// �ý���Ʈ���̵� addin���� �����͸� �������� ���� �������̽� ����
interface ISTAddinData
{
	// �����Ͱ��� ����.
	// ST���� ����ϴ� �⺻ ��Ŷ �̿��� ��� ������ ������ �߰� - ojtaso (20080609)
	STDMETHOD_(int, GetChartData)(LONG& dwDataTime, LONG& dwDataOpen, LONG& dwDataHigh, LONG& dwDataLow, LONG& dwDataClose,LONG& dwDataVolume, LONG& dwDataExt) PURE;
};


#endif // AFX_INTERFACE_OF_ST_ADDINMANAGER_BY_JUNOKLEE_060721A__INCLUDED_
