// SymbolBaseData.h: interface for the CSymbolBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLBASEDATA_H__49A88E5A_FF3D_446F_A8FA_034B4F86E91F__INCLUDED_)
#define AFX_SYMBOLBASEDATA_H__49A88E5A_FF3D_446F_A8FA_034B4F86E91F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSymbolBaseData  
{
public:
	//���ø�ũ�� ��ȣ�� ���̰�.
	enum INTERVAL
	{
		interval = 3
	};

	enum SIZETYPE
	{
		Small,
		Medium,
		Large
	};

	enum ModifyType
	{
		MODIFY_NONE,
		MODIFY_DROP,
		MODIFY_END
/*
		MODIFY_NONE,
		MODIFY_NODROP,
		MODIFY_CENTER,
		MODIFY_START,
		MODIFY_END,
		MODIFY_X,
		MODIFY_Y*/
	};

	enum TOOLTYPE
	{
		SYMBOL_NONE,
		SYMBOL_SQUARE,		//�簢����
		SYMBOL_CIRCLE,		//��
		SYMBOL_TRIANGLE,	//�ﰢ��
		SYMBOL_TEXT,		//����
		SYMBOL_BULET,		//��
		SYMBOL_SELECTION,	//����
		SYMBOL_ARROW_LEFT,  //���� ȭ��ǥ
		SYMBOL_ARROW_RIGHT, //������ ȭ��ǥ
		SYMBOL_ARROW_UP,	//���� ȭ��ǥ
		SYMBOL_ARROW_DOWN,  //�Ʒ��� ȭ��ǥ
		SYMBOL_ARROW_LEFTTOP,	//���� �� ���� ȭ��ǥ
		SYMBOL_ARROW_LEFTBOTTOM,//���� �Ʒ� ���� ȭ��ǥ
		SYMBOL_ARROW_RIGHTTOP,  //������ �� ���� ȭ��ǥ
		SYMBOL_ARROW_RIGHTBOTTOM, //������ �Ʒ� ���� ȭ��ǥ
		SYMBOL_NUM_ONE,		//���� 1
		SYMBOL_NUM_TWO,		//2
		SYMBOL_NUM_THREE,	//3
		SYMBOL_NUM_FOUR,	//4
		SYMBOL_NUM_FIVE,	//5
		SYMBOL_NUM_SIX,		//6
		SYMBOL_NUM_SEVEN,	//7
		SYMBOL_NUM_EIGHT,	//8
		SYMBOL_NUM_NINE,	//9
		SYMBOL_NUM_TEN,		//10
		SYMBOL_TEXT_A,		//a
		SYMBOL_TEXT_B,		//b
		SYMBOL_TEXT_C,		//c
		SYMBOL_TEXT_D,		//d
		SYMBOL_TEXT_E,		//e
		SYMBOL_TEXT_F,		//f
		SYMBOL_TEXT_G,		//g
		SYMBOL_TEXT_H,		//h
		SYMBOL_CHAR_1,		//��
		SYMBOL_CHAR_2,		//��
		SYMBOL_CHAR_3,		//��
		SYMBOL_CHAR_4,		//��
		SYMBOL_CHAR_5,		//��
		SYMBOL_CHAR_6		//��
	};
};

#endif // !defined(AFX_SYMBOLBASEDATA_H__49A88E5A_FF3D_446F_A8FA_034B4F86E91F__INCLUDED_)
