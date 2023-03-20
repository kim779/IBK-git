// ConfigSet.h: interface for the CConfigSet class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __CONFIG_SET_H
#define __CONFIG_SET_H
#endif

#if !defined(AFX_CONFIGSET_H__878DBB4B_C0D1_4B63_B675_C1BB4D6ABD42__INCLUDED_)
#define AFX_CONFIGSET_H__878DBB4B_C0D1_4B63_B675_C1BB4D6ABD42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_LINE_CLR	RGB(242,159,36) //��Ȳ��

//--------------------------------------------------------
// Author      : Sang-Yun, Nam  date : 2005/01/28
// Comments    : ����ڰ� �����ϰ� �ִ� ������ ������ �ִ� Global Class 
// using way   : 
// See	       : CConfigSet
//---------------------------------------------------------
class CConfigSet  
{
public :
	CConfigSet();
	virtual ~CConfigSet();

//BEGIN ===========================================================
	
public:
	void		InitMember();
	CConfigSet* GetConfigValue(){return this;}
	void		SetConfigValue(const CConfigSet* p_pConfigSet);
public:
	BOOL m_bNotDrawLine;   //���ؼ� �׸��� ����

	BOOL m_bChkYSprice; //���ϰ��� �ð�				üũ ����
	BOOL m_bChkYHprice; //���ϰ��� ��				üũ ����
	BOOL m_bChkYLprice; //���ϰ��� ����				üũ ����
	BOOL m_bChkYEprice; //���ϰ��� ����				üũ ����
	BOOL m_bChkYSHL;	//���ϰ��� (��+��+��)/3		üũ ����

	BOOL m_bChkTSprice; //���ϰ��� �ð�				üũ ����
	BOOL m_bChkTHprice; //���ϰ��� ��				üũ ����
	BOOL m_bChkTLprice; //���ϰ��� ����				üũ ����
	BOOL m_bChkTEprice; //���ϰ��� ����				üũ ����
	BOOL m_bChkTHL;		//���ϰ��� (��+��)/2		üũ ����
	BOOL m_bChkTSHL;	//���ϰ��� (��+��+��)/3		üũ ����
	BOOL m_bChkTHLE;	//���ϰ��� (��+��+(��*2))/4 üũ ����
	BOOL m_bChkHighLimit;	//���Ѱ�				üũ ����
	BOOL m_bChkLowLimit;	//���Ѱ�				üũ ����

	BOOL m_bChkPivot2Rst;	// �Ǻ� 2�� ���� üũ ���� 
	BOOL m_bChkPivot1Rst;	// �Ǻ� 1�� ���� üũ ����
	BOOL m_bChkPivotGLine;	// �Ǻ� ���ؼ�  üũ ����
	BOOL m_bChkPivot1GG;	// �Ǻ� 1�� ���� üũ ����
	BOOL m_bChkPivot2GG;	// �Ǻ� 2�� ���� üũ ����

	BOOL m_bChkDMKPreHPrice;// Demark �����  üũ ����
	BOOL m_bChkDMKGLine;	// Demark ���ؼ�	üũ ����
	BOOL m_bChkDMKPreLPrice;// Demark ��������  üũ ����

	BOOL m_bChk2Pyung1;		// �̵���� ù��° �Է°� üũ ����
	BOOL m_bChk2Pyung2;		// �̵���� �ι�° �Է°� üũ ����
	BOOL m_bChk2Pyung3;		// �̵���� ����° �Է°� üũ ����
	BOOL m_bChk2Pyung4;		// �̵���� �׹�° �Է°� üũ ����

	BOOL m_bChkPmmDanga;	// ��ո��� �ܰ� üũ ����
	BOOL m_bChkPmmPDanga;	// ��ո��� �ܰ� +n% üũ ����
	BOOL m_bChkPmmMDanga;	// ��ո��� �ܰ� -n% üũ ����

	COLORREF m_clrYSprice;	//���ϰ��� �ð�				�� ������
	COLORREF m_clrYHprice;	//���ϰ��� ��				�� ������
	COLORREF m_clrYLprice;	//���ϰ��� ����				�� ������
	COLORREF m_clrYEprice;	//���ϰ��� ����				�� ������
	COLORREF m_clrYSHL;		//���ϰ��� (��+��+��)/3		�� ������

	COLORREF m_clrTSprice;	//���ϰ��� �ð�				�� ������
	COLORREF m_clrTHprice;	//���ϰ��� ��				�� ������
	COLORREF m_clrTLprice;	//���ϰ��� ����				�� ������
	COLORREF m_clrTEprice;	//���ϰ��� ����				�� ������
	COLORREF m_clrTHL;		//���ϰ��� (��+��)/2		�� ������
	COLORREF m_clrTSHL;		//���ϰ��� (��+��+��)/3		�� ������
	COLORREF m_clrTHLE;		//���ϰ��� (��+��+(��*2))/4 �� ������
	COLORREF m_clrHighLimit;//���Ѱ�					�� ������
	COLORREF m_clrLowLimit;	//���Ѱ�					�� ������

	COLORREF m_clrPivot2Rst;	// �Ǻ� 2�� ���� �� ������
	COLORREF m_clrPivot1Rst;	// �Ǻ� 1�� ���� �� ������
	COLORREF m_clrPivotGLine;	// �Ǻ� ���ؼ�	 �� ������
	COLORREF m_clrPivot1GG;		// �Ǻ� 1�� ���� �� ������
	COLORREF m_clrPivot2GG;		// �Ǻ� 2�� ���� �� ������

	COLORREF m_clrDMKPreHPrice;	// Demark �����  �� ������
	COLORREF m_clrDMKGLine;		// Demark ���ؼ�	�� ������
	COLORREF m_clrDMKPreLPrice;	// Demark ��������  �� ������

	COLORREF m_clr2Pyung1;		// �̵���� ù��° �Է°� �� ������
	COLORREF m_clr2Pyung2;		// �̵���� �ι�° �Է°� �� ������
	COLORREF m_clr2Pyung3;		// �̵���� ����° �Է°� �� ������
	COLORREF m_clr2Pyung4;		// �̵���� �׹�° �Է°� �� ������
	
	COLORREF m_clrPmmDanga;		// ��ո��� �ܰ� �� ������
	COLORREF m_clrPmmPDanga;	// ��ո��� �ܰ� +n% �� ������
	COLORREF m_clrPmmMDanga;	// ��ո��� �ܰ� -n% �� ������

	int m_nLineYSprice;		//���ϰ��� �ð�				���� �β�
	int m_nLineYHprice;		//���ϰ��� ��				���� �β�
	int m_nLineYLprice;		//���ϰ��� ����				���� �β�
	int m_nLineYEprice;		//���ϰ��� ����				���� �β�
	int m_nLineYSHL;			//���ϰ��� (��+��+��)/3		���� �β�
	
	int m_nLineTSprice;		//���ϰ��� �ð�				���� �β�
	int m_nLineTHprice;		//���ϰ��� ��				���� �β�
	int m_nLineTLprice;		//���ϰ��� ����				���� �β�
	int m_nLineTEprice;		//���ϰ��� ����				���� �β�
	int m_nLineTHL;			//���ϰ��� (��+��)/2		���� �β�
	int m_nLineTSHL;			//���ϰ��� (��+��+��)/3		���� �β�
	int m_nLineTHLE;			//���ϰ��� (��+��+(��*2))/4 ���� �β�
	int m_nLineHighLimit;		//���Ѱ�					���� �β�
	int m_nLineLowLimit;		//���Ѱ�					���� �β�
	
	int m_nLinePivot2Rst;		// �Ǻ� 2�� ���� ���� �β�
	int m_nLinePivot1Rst;		// �Ǻ� 1�� ���� ���� �β�
	int m_nLinePivotGLine;		// �Ǻ� ���ؼ�	 ���� �β�
	int m_nLinePivot1GG;		// �Ǻ� 1�� ���� ���� �β�
	int m_nLinePivot2GG;		// �Ǻ� 2�� ���� ���� �β�
	
	int m_nLineDMKPreHPrice;	// Demark �����  ���� �β�
	int m_nLineDMKGLine;		// Demark ���ؼ�	���� �β�
	int m_nLineDMKPreLPrice;	// Demark ��������  ���� �β�
	
	int m_nLine2Pyung1;		// �̵���� ù��° �Է°� ���� �β�
	int m_nLine2Pyung2;		// �̵���� �ι�° �Է°� ���� �β�
	int m_nLine2Pyung3;		// �̵���� ����° �Է°� ���� �β�
	int m_nLine2Pyung4;		// �̵���� �׹�° �Է°� ���� �β�
	
	int m_nLinePmmDanga;		// ��ո��� �ܰ� ���� �β�
	int m_nLinePmmPDanga;		// ��ո��� �ܰ� +n% ���� �β�
	int m_nLinePmmMDanga;		// ��ո��� �ܰ� -n% ���� �β�

	int m_nDay2Pyung1;			// �̵���� ù��° �Է� �Ⱓ
	int m_nDay2Pyung2;			// �̵���� �ι�° �Է� �Ⱓ
	int m_nDay2Pyung3;			// �̵���� ����° �Է� �Ⱓ
	int m_nDay2Pyung4;			// �̵���� �׹�° �Է� �Ⱓ
	
	int	 m_nPmmPDanga;			// ��ո��� �ܰ� +n%		��
	int	 m_nPmmMDanga;			// ��ո��� �ܰ� -n%		��

	CString m_strAcct;			// ����ڰ� ������ ����

	BOOL m_bChkName;			// ����ǥ�� : �̸�	üũ ��ư
	BOOL m_bChkValue;			// ����ǥ�� : ��	üũ ��ư
	int	 m_nAlign;				// ����ǥ�� (����/������/����)
	
	
//END ===========================================================

};

#endif // !defined(AFX_CONFIGSET_H__878DBB4B_C0D1_4B63_B675_C1BB4D6ABD42__INCLUDED_)
