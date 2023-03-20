// DataMath.cpp: implementation of the CDataMath class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataMath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CMath

// public ===============================================================================
// �ݿø� �Լ��� ------------------------------------------------------------------------
// double �� �ݿø��ؼ� int������ ��ȯ : round(123.567) -> 124
int CMath::Round(const double x)
{
	return (int)floor(x + 0.5);
}

double CMath::Round_Double(const double& x)
{
	return floor(x + 0.5);
}

// �Ǽ� �ݿø� : �Ҽ��� �Ʒ��� roundPosition�ڸ����� �ݿø� : round(123.567, 2) -> 123.6
double CMath::Round(const double& x, const int roundPosition)
{
	if(roundPosition <= 0)
		return Round_Double(x);

	// �Ҽ��� �Ʒ� �κ��� "0" �� ���
	// �Ʒ� �κ��� ���� ���� ��� 4969221000.0000 -> 4969221000.00001 �� �ȴ�.
	if(x == (int)x)
		return x;

	double value = x * pow(10.0, (double)(roundPosition-1));

	// �Ʒ��κ��� ������ ������ �Ҽ����� �κ��� "0" �̿��� ��� �ڿ� "1"�� �ٴ� ��� �߻�.
	//return (Round_Double(value) * pow(0.1, (double)(roundPosition-1)));
	return Divide10(Round_Double(value), (roundPosition -1));
}

// ���� �ݿø� : roundPosition�ڸ����� �ݿø� : round(123567, 2) -> 123600
int CMath::Round(const int x, const int roundPosition)
{
	if(roundPosition <= 0)
		return x;

	//double value = (double)x * pow(0.1, (double)roundPosition);
	double value = Divide10((double)x, roundPosition);
	value = Round(value) * pow(10.0, (double)roundPosition);
	return (int)value;
}

// ���� �Լ��� --------------------------------------------------------------------------
// �Ǽ� ���� : �Ҽ��� �Ʒ��� roundPosition�ڸ����� ���� : Floor(123.567, 2) -> 123.5
double CMath::Floor(const double& x, const int roundPosition)
{
	if(roundPosition <= 0)
		return floor(x);

	double temp = x * pow(10.0, (double)(roundPosition-1));
	//return (floor(temp) * pow(0.1, (double)(roundPosition-1)));
	return Divide10(floor(temp), (roundPosition-1));
}

// ���� ���� : roundPosition�ڸ����� ���� : Floor(123567, 2) -> 123500
int CMath::Floor(const int x, const int roundPosition)
{
	if(roundPosition <= 0)
		return x;

	//double value = (double)x * pow(0.1, (double)roundPosition);
	double value = Divide10((double)x, roundPosition);
	value = floor(value) * pow(10.0, (double)roundPosition);
	return (int)value;
}

// --------------------------------------------------------------------------------------
// ���� �Լ� 
double CMath::Divide10(const double& dData, const int count)
{
	double dValue = dData;
	for(int nIndex = 0; nIndex < count; nIndex++){
		dValue = dValue / 10.0;
	}
	return dValue;
}

int CMath::Power(const int data, const int count)
{
	return (int)floor(pow((double)data, (double)count));
}

// �������� �Ÿ� --> sqrt(x*x+y*y)
double CMath::Distance(const CPoint& pt1, const CPoint& pt2)
{
	return sqrt(pow((pt1.x - pt2.x), 2) + pow((pt1.y - pt2.y), 2));
}

// ������ �߰� ��ġ
int CMath::Center(const int pt1, const int pt2)
{
	return (pt1 + Round((double)(pt2 - pt1) / 2.0));
}

// log ��
double CMath::Log(const double& data)
{
	//return (log(data) * 10000.0);
	if(data == 0.0)
		return 0.0;

	if(data > 0)
		return log(data);
	return -log(fabs(data));
}

// exp ��
double CMath::Exp(const double& data)
{
	//return exp(data / 10000.0);
	if(data == 0.0)
		return 0.0;

	if(data > 0)
		return exp(data);
	return -exp(fabs(data));
}

// ������ ���ϱ�(%)
double CMath::Modulus(const double& data1, const double& data2)
{
	if(data2 == 0)
		return 0.0;

	double dTemp = int(data1/data2) * data2;
	return (data1 - dTemp);
}

// --------------------------------------------------------------------------------------
int CMath::TanI(const int nAngle)
{
	if((nAngle%180) == 0)
		return 0;
	else if((nAngle%90) == 0)
		return 0;

	int nRealAngle = nAngle%360;
	double pi = 3.1415926535897931;
	double dAngle = pi * (double)nRealAngle / 180.0;
	return Round(tan(dAngle));
}

double CMath::TanD(const double& dAngle)
{
	if(Modulus(dAngle, 180.0) == 0.0)
		return 0.0;
	else if(Modulus(dAngle, 90.0) == 0.0)
		return 0.0;

	double dRealAngle = Modulus(dAngle, 360.0);

	double pi = 3.1415926535897931;
	dRealAngle = pi * dRealAngle / 180.0;
	return tan(dRealAngle);
}

double CMath::Angle(const double& dTan)
{
	if(dTan == 1.0)
		return 45.0;
	else if(dTan == -1.0)
		return 135.0;
	else if(dTan == 0.0)
		return 180.0;

	double pi = 3.1415926535897931;
	double dAngle = atan(dTan) * 180.0 / pi;
	return dAngle;
}

// --------------------------------------------------------------------------------------
// �ڸ����� ���Ѵ�.
int CMath::NumberCount(const int x)
{
	CString data;
	data.Format("%d", x);

	return data.GetLength();
}
