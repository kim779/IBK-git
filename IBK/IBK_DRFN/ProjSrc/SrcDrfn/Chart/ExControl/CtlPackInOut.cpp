#include "stdafx.h"
#include "../../Inc/ExOCXDLL.h"

int CExOCXDLL::CImplPackIn::GetData(char* buf, int bufLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackIn)

	return pThis->Packet_GetData(buf, bufLen);
}


/////////////////////////////////////////////////////////
CExOCXDLL::CImplPackOut::CImplPackOut()
{
//	char *tmp = "------";
//	memcpy(m_aData, tmp, 6);
//	m_nLines = m_nBozoSize = 0;
}

// * ��ǥ�� �������� ��� ����ũ���� ��Ʈ�ѿ��� ũ�⸦ Ȯ���Ѵ�.
// * ���⼱ * ������ �����Ƿ� ó������ �ʴ´�. 
// * Ȥ �ʿ��ϰ� �Ǹ� �����ϱ� �ٶ�.
int CExOCXDLL::CImplPackOut::GetPackDataSize(char* pIn)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackOut)
	return pThis->Packet_GetPackDataSize(pIn);
}

// * ��Ʈ�ѿ��� �Ѱ��ִ� ����Ÿ�� ����� ������ ����� ����Ÿ
int CExOCXDLL::CImplPackOut::SetData(char* buf, int bufLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackOut)
	return pThis->Packet_SetData(buf, bufLen);
}

// * ��Ʈ�ѿ��� �Ѱ��ִ� ����Ÿ�� *�ΰ���϶��� ����Ÿ
// * ���⼱ * ó���� �����Ƿ� ó������ �ʴ´�. 
// * Ȥ �ʿ��ϰ� �Ǹ� �����ϱ� �ٶ�.
int CExOCXDLL::CImplPackOut::SetData2(char* buf, int bufLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackOut)
	return pThis->Packet_SetData2(buf, bufLen);
}

// ��Ʈ�ѿ��� �Ѱ��ִ� ����Ÿ�� !�ΰ���϶��� ����Ÿ
int CExOCXDLL::CImplPackOut::SetData3(char* buf, int bufLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackOut) 
	return pThis->Packet_SetData3(buf, bufLen);
}

// * ����Ÿ������
// * ������ ����� Ÿ�Ե� ����
// * nType  : 1 -> SetData ��Ÿ��
// *          2 -> SetData2 ��Ÿ��
// *          3 -> SetData3 ��Ÿ��
// * Ȥ � ��Ÿ���� �� �ʿ��ϰ� ���� �𸣱� ������ �� ��ƾ�� �߰���.
int CExOCXDLL::CImplPackOut::SetData999(int nType, char* buf, int bufLen)
{
	AZMETHOD_PROLOGUE(CExOCXDLL, ImplPackOut)
	return pThis->Packet_SetData999(nType, buf, bufLen);
}
