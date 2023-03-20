// BaseData.h: interface for the CBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEDATA_H__7AC3DE75_7AED_4DDA_893F_BA6368AC1DBE__INCLUDED_)
#define AFX_BASEDATA_H__7AC3DE75_7AED_4DDA_893F_BA6368AC1DBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define	UDM_INDICATOR_GRAPH_INSERT	( WM_USER + 701	)		// MACD Oscillator �߰��϶�� ����� Message


// (2006/4/19 - Seung-Won, Bae) Not Used. So Comment
/*
static const int NUM_ITEM = 50;
// ���ǰ�
static const _TCHAR *_pConditiontitle[ NUM_ITEM ] = 
{
	_T("��ĭ��"),		_T("��ȯĭ"),		_T("��������"),			_T("�ܱ�"),				_T("�߱�"),   
	_T("���"),			_T("Signal"),		_T("�Ⱓ"),				_T("�¼�"),				_T("��Ȱ�Ⱓ"),
	_T("����Ⱓ"),		_T("�����Ⱓ"),		_T("������"),			_T("�ִ밪"),			_T("����"),
	_T("Open����"),		_T("Close����"),	_T("��ȯ�Ⱓ"),			_T("���رⰣ"),			_T("����2�Ⱓ"),
	_T("ADX"),			_T("ADXR"),			_T("�̵����"),			_T("������"),			_T("��������"),
	_T("ǥ�ð���"),		_T("����"),			_T("��������"),			_T("��¥"),				_T("��ġ"),
	_T("Roc �Ⱓ"),		_T("Stoc �Ⱓ"),	_T("Macd signal"),		_T("ǥ������"),			_T("���Ӻ���"),
	_T("�ִ���밪"),	_T("�ҳ��Ⱓ"),		_T("1�� ��Ȱ�Լ�"),		_T("2�� ��Ȱ�Լ�"),		_T("������")
	_T("�ؽ�Ʈ(��ġ)"), _T("")
};
*/

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 18
// Comments		: �⺻ Data�� Setting ���� ������ class
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CBaseData  
{
public:
	// Į�� listbox�� �����ϴ� Item�� string
	enum COLORLISTITEMTYPE
	{
		NONE,					
		PRICE,					// ����
		VOLUME,					// �ŷ���
		UP_DOWN,				// ���, �϶�
		DAEGIMEMUL,				// ���Ź�
		COLOR,					// ����
		SUBGRAPHNAME,			// SubGraph�̸�
		RAINBOW,				// ���ۻ���, �������
		SUB_UPFILL_DOWNFILL,	// SubGraph�̸�, ���ä��, �϶�ä��
		GLANCEBLANCE,			// �ϸ� ��ȯ, ����, ���ེ��, ���ེ��, ���ä��, �϶�ä��.
		INVERSELINE,			
		MACD,					// MACD
		PIVOT					// Pivot
	};

	
	enum TABITEMTYPE
	{
		CONFIGURATION,			// ����
		CONFIGU_BASELINE,		// ����, ���ؼ�
		CONFIGU_HORIZONLINE,	// ����, ����
		CONFIGU_SIGNAL			// ����, signal
	};


	// ��ǥ�� ���� SubTabPage
	enum DLGTYPE
	{
		SeveralSub_FourCondition_WithCombo,		// ���� sub, condition4�����
		SeveralSub_FourCondition,				// ���� sub, condition4
		SeveralSub_ThreeCondition,				// ���� Sub, Condition 3
		SeveralSub_TwoCondition,				// ���� Sub, Condition 2
		SeveralSub_OneCondition,				// ���� Sub, Condition 1
		SeveralSub_NoCondition,					// ���� Sub, No Condition
		OneSub_NoCondition,						// Sub 1, No-Condition
		OneSub_OneCondition,					// Sub 1, Condition 1
		OneSub_TwoCondition,					// Sub 1, Condition 2
		OneSub_ThreeCondition,					// Sub 1, Condition 3
		OneSub_SevenCondition,					// sub 1, Condition 7
		Price,									// price
		Volume,									// volume
		MovingAverage,							// MovingAverage
		PriceMA,								// PriceMA
		Disparity,								// �̰ݵ�
		Daegimenul,								// ���Ź�, ä��
		GLANCEBALANCE,							// �ϸ� ����ǥ 
		OneSub_ThreeCondi_WithCombo,			// Sub 1, �޺��� ���� Condition 3
		OneSub_TwoCondi_WithCombo,				// Sub 1, �޺��� ���� Conditon2 
		SeveralSub_ThreeCondi_WithCmbo,			// ���� Sub, �޺��� ���� Conditon3 
		Market_Indicator_First,					// ������ǥ�� ���� ���� ����.
		Market_Indicator_InvestorTrend,			// �����ڵ��⿡ Packet�����κ� �߰�
		DT_SPECIAL_TYPE_A,						// �Ｑ��ȯ��
		DT_SPECIAL_TYPE_B,						// P&F, ���ð�, Kagi
		DT_SPECIAL_TYPE_C,						// RENKO,�л�íƮ
		DT_SPECIAL_TYPE_D,
		DT_SPECIAL_TYPE_E,						// ���Ź� ���� �и� Sang-Woo, Cho		2006 / 04 / 19
		DT_COMMON_TYPE_A,						// ��κ��� ��簪
		DT_COMMON_TYPE_B						// Sang-Woo, Cho	2006 / 05 / 02 - BinaryWave ������ �߰�.
	};

	// ä�� �Ӽ�
	enum FILLTYPE
	{
		Fill,				// ���, �϶� ä��
		NoFill				// ä�� �ʿ����.
	};

	// ������Ʈ�� ���� �׷�������..�ƴ���..
	enum DRAWPOSITIONTYPE
	{
		DrawWithPrice,		// ������Ʈ�� ���� �׷�����.
		NotDrawWithPrice 
	};

	// ���� �������� ����
	enum LIMITHORIZONTALLINE
	{
		ZeroToOneZeroZero,
		MinusZeroToOneZeroZero,
		Others
	};
	
	// �׷����� ���� ����
	enum GRAPHSTATE
	{
		OnlySetting,
		Insert,
		Delete
	};

	// ����Ʈ ��Ʈ���� �θ�.
	enum PARENTOFLIST
	{
		Tab,
		Dialog
	};
};


#endif // !defined(AFX_BASEDATA_H__7AC3DE75_7AED_4DDA_893F_BA6368AC1DBE__INCLUDED_)
