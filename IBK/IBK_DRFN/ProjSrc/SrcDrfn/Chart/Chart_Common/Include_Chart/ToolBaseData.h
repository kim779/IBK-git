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
		Cross_Line = 1,	// ���ڼ�
		Vert_Line,		// ������
		Horz_Line,		// ����
		Line,			// �߼���
		Trisection,		// ���м�
		Quadrisect,		// ���м�
		Fibo_Semicircle,// �Ǻ� �ݿ�
		Fibo_Circle,	// �Ǻ� ��
		Fibo_Fan_Line,	// �Ǻ� �����
		Fibo_Retrace,	// �Ǻ� �ǵ���(������)
		Fibo_Time,		// �Ǻ� �ð���
		Gann_Line,		// ������
		Gann_Fan,		// ����
		Gann_Grid,		// ���׸���
		AndrewsPitchfork,// ����� ��ġ��ũ
		ElliotWave_Line,// ������Ʈ �ĵ���
		Cycle_Line,		// ����Ŭ ����
		Cut_Line,		// ������ �м�����
		Speed_Line,		// ���ǵ� ����
		Linear_Regression,// ���� ȸ�ͼ�(ȸ�ͼ�)
		Raff_Regression	// Raff ä�μ�(ȸ��ä��)
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
