// DX_OptionSens.h: interface for the CDX_OptionSens class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#define CALL 2
#define PUT  3

#define	MAX_R		92

#define MAX(a,b)	(a>=b ? a : b)
#define NODE_CNT	49
#define PI		3.14159265358979 
#define CALL		2
#define PUT		3
#define THETA_VAL	0.002739726

#define GAMMA		0.2316419
#define CNORMAL_1	0.31938153
#define CNORMAL_2	-0.356563782
#define CNORMAL_3	1.781477937
#define CNORMAL_4	-1.821255978
#define CNORMAL_5	1.330274429

typedef struct {
	char	code[8];	// �����ڵ�
	double	remainDays;	// �����ϼ�
	double	strike;		// ��簡
	double	cd;		// cd�ݸ�
	double	baedangJisu;	// �������
	double	currAmt;	// ���簡
	double	underlying;	// �����ڻ�(Kospi200)
	double	volatility;	// ������������
} OPTION, *POPTION;

class CDX_OptionSens  
{
public:
	double	GetOptionRho(POPTION in);
	double	GetOptionTheta(POPTION in);
	double	GetOptionVegga(POPTION in);
	double	GetOptionGamma(POPTION in);
	double	GetOptionDelta(POPTION in);
	double	GetOptionInsideVolatility(POPTION in);
	double	GetOptionTheoryAmt(POPTION in);
	double	GetDouble(double value);
	double	CNormal(double value);
	double	GetFactorial(int value);
	double	GetND(double value);
	int	CallPut(LPSTR code);
	CDX_OptionSens();
	virtual ~CDX_OptionSens();

};