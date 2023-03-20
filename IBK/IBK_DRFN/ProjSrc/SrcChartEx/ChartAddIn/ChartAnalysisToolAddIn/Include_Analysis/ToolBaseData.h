// ToolBaseData.h: interface for the CToolBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBASEDATA_H__AB2749C8_2642_4A1B_9267_ADFEF7D5C985__INCLUDED_)
#define AFX_TOOLBASEDATA_H__AB2749C8_2642_4A1B_9267_ADFEF7D5C985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToolEnumData
{
public:
	enum ElementType
	{
		Not_Element		= -1,
		Select_Element	= 0,
		Cross_Line		= 1,		// ���ڼ�
		Vert_Line		= 3,		// ������
		Horz_Line		= 4,		// ����
		Line			= 5,		// �߼���
		Trisection		= 6,		// ���м�
		Quadrisect		= 7,		// ���м�
		Fibo_Semicircle = 8,		// �Ǻ� �ݿ�
		Fibo_Circle		= 9,		// �Ǻ� ��
		Fibo_Fan_Line	= 10,		// �Ǻ� �����
		Fibo_Retrace	= 11,		// �Ǻ� �ǵ���(������)
		Fibo_Time		= 12,		// �Ǻ� �ð���
		Gann_Line1		= 13,		// ������(45)
		Gann_Line2		= 14,		// ������(315)
		Gann_Fan1		= 15,		// ����(45)
		Gann_Fan2		= 16,		// ����(135)
		Gann_Fan3		= 17,		// ����(225)
		Gann_Fan4		= 18,		// ����(315)
		Gann_Grid		= 19,		// ���׸���
		AndrewsPitchfork= 20,		// ����� ��ġ��ũ
		ElliotWave_Line	= 21,		// ������Ʈ �ĵ���
		Cycle_Line		= 22,		// ����Ŭ ����
		Cut_Line		= 23,		// ������ �м�����
		Speed_Line		= 24,		// ���ǵ� ����
		Speed_Fan		= 25,		// ���ǵ� Fan
		Speed_Arc		= 26,		// ���ǵ� ȣ
		Linear_Regression=27,		// ���� ȸ�ͼ�(ȸ�ͼ�)
		Raff_Regression	= 28,		// Raff ä�μ�(ȸ��ä��)
		Angle_Line		= 29,		// ���� �߼���
		Candle_Line		= 30,		// Candle�߽ɼ�
		Candle_Bong		= 31,		// Candle�����
		Adjust_Ratio	= 32,		// ���������
		Period_Summary	= 33,		// �Ⱓ���
		Tracing_ToolTip	= 34,		// TracingToolTip
		Pencil_Line		= 35,		// ���� �߼���		
		Balance_Basic   = 36,		// �ϸ��ġ				// 2014.09.24 �ڼ��� : 0010862: [7111] �Ｚ���� ��Ʈ ������ [�ϸ��ġ] ��� IBK��Ʈ���� �����ϰ� �߰� ��û
	};

	enum DrawType
	{
		DRAW_NONE,
		DRAW_PEN,
		DRAW_NODROP
	};

	enum ModifyType
	{
		MODIFY_NONE,
		MODIFY_CENTER,
		MODIFY_START,
		MODIFY_MIDDLE,
		MODIFY_END,
		MODIFY_X,
		MODIFY_Y
	};

	enum PenStyle
	{
		Solid = 0,
		Dot,
		Dash,
		DashDot,
		DashDotDot
	};
};

#endif // !defined(AFX_TOOLBASEDATA_H__AB2749C8_2642_4A1B_9267_ADFEF7D5C985__INCLUDED_)
