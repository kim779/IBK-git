// DataMath.h: interface for the CDataMath class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAMATH_H__B43F3C19_DB3A_462F_902F_3C447D8C9BC4__INCLUDED_)
#define AFX_DATAMATH_H__B43F3C19_DB3A_462F_902F_3C447D8C9BC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>

/* �ݿø� yiy 2006.12.21
	�Ǽ������� ���ؼ� ȭ�鿡 �� ���� ����ϱ� ���ؼ��� ������ ��ȯ�ϴ� �۾��� �ʿ��ϰ� �˴ϴ�.
	�̶� �ݿø� ������ ����ϰ� �Ǵµ��� �� �ݿø� ������ ����� ȭ�鿡 �������� ���� ����϶� ����
	ū �ǹ̸� �����ϴ�. ȭ���� ��ǥ�� (0,0) ���� �����ϱ� ����������...
	�׷��ٰ� ����϶� �ǹ��ִ� ���� ����� �ִ� ���3�� ���� �Ǹ� �������ʴ� �������� ���̴� �������� ���� ���� ��쿡
	������ �ʴ� ����(���� ����) �� 1px ������ �߻��Ҽ� �ְ� �װ��� ������ �ʴ� ����ϼ� �ֽ��ϴ�.(������ ����)
	�ؼ� ���� ū���� ���ؼ� ������ ��ȯ�ϰ� �ٽ� ���� ū���� ���� ����� ����Ҽ� �ֽ��ϴ�.
	�̷��� �Ұ�� ���� �ָ� ������ ���� ������ ��ǥ�� 1px �޶����ٰ� �ص� �������� ������ ������ ��ġ�� ������ ���� �̺�(���� ����)
	�ϱ� ������ ���2�� ���� �ֽ��ϴ�.
	
	���1�� ����� ������ ���ؼ� ��Ȯ�� �ݿø� ������ �Ҽ� �ֵ��� �ϴ� ������ �̰�� ���ǹ��� ���� �˴ϴ�.
	������ �� ����� ������ ������� �����Ҽ� �ִ� ���� ��κ��� �ݿø� ������ �Ķ���ͷ� ������ ���� ����� �ɰ��̱� ������
	CPU�� �б� ������ ���ؼ� ������� Ŀ���� �ɼ� �ִ� ���Դϴ�.

	�Ϲ����� �ý��ۿ��� �����ϱ⿡ (��Ȳ�� ���� �ٸ��� �ֽ��ϴ�.)
	���ɻ����� ���� [���3 > ���2 > ���1]�� ������ ����� ���ð��Դϴ�.
	��Ȯ���� ����    [��� 1> ���2 > ���3]�� ������ ����� ���ɴϴ�.

	����� ���� ������ ������ ���1�� ����ϰ� �ֽ��ϴ�.
	�� ���� ������ ����� �ʹٸ� ���2�� �ᵵ �ɰ����� �����˴ϴ�.
*/

#define ROUND(x) int((x>0)?(x+0.5):(x-0.5))		// ���1
//#define ROUND(x) (int(x+5000000.5) - 5000000)		// ���2
//#define ROUND(x) int(x+0.5)				// ���3

class CMath
{
public:
	// �ݿø�
	static int Round(const double x);
	static double Round_Double(const double& x);
	static double Round(const double& x, const int roundPosition);
	static int Round(const int x, const int roundPosition);

	// ����
	static double Floor(const double& x, const int floorPosition);
	static int Floor(const int x, const int floorPosition);

	// �ŵ�����
	static double Divide10(const double& dData, const int count);
	static int Power(const int data, const int count);
	// �������� �Ÿ�
	static double Distance(const CPoint& pt1, const CPoint& pt2);
	// ������ �߰� ��ġ
	static int Center(const int pt1, const int pt2);

	// log�� ���� ����
	static double Log(const double& data);
	static double Exp(const double& data);

	// ������ ���ϱ� (%)
	static double Modulus(const double& data1, const double& data2);

	// ����
	static int TanI(const int nAngle);
	static double TanD(const double& dAngle);
	static double Angle(const double& dTan);

	// �ڸ����� ���Ѵ�.
	static int NumberCount(const int x);
};

#endif // !defined(AFX_DATAMATH_H__B43F3C19_DB3A_462F_902F_3C447D8C9BC4__INCLUDED_)
