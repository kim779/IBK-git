// BaseObj.h: interface for the CBaseObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEBASEOBJ_H__4058CFD8_D932_4354_9DCF_F85543CA7678__INCLUDED_)
#define AFX_LINEBASEOBJ_H__4058CFD8_D932_4354_9DCF_F85543CA7678__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
// ���ǹ�

// Ŭ���� ��ġ�� ��� ������ ���ԵǴ� ��
#define N_PT		0	// No point
#define S_PT		1	// Start point
#define E_PT		2	// End point
#define M_PT		4	// Mid point
#define ON_PT		31	// �� ��

#define OFFSET_RC_IN		4			// ��ü RC�� �׸��� RC���� offset
#define OFFSET_LT			20			// ���ΰ� ���̺� ������ �⺻(�ּ�) offset
#define OFFSET_TT_IN		5			// ���̺�� �ؽ�Ʈ ������ �⺻(�ּ�) offset
#define RADIUS_OF_END_RC	2			// ������ �� ������ �� ���� ������
#define DEFAULT_HEIGHT		20			// pixels, ���� �⺻ ����
#define TEXT_HEIGHT			15			// ���� ������ ����
#define TEXT_WIDTH			8			// ���� ������ �� 
#define TEXT_RC_OFFSET	CPoint(30,30)	// �ؽ�Ʈ�� �⺻ ��ġ
#define WH_OF_MID_RC		5			// �߰����� ǥ���� ���簢���� ��/����
#define UP					-1			// ������
#define DOWN				1			// �Ʒ�����
#define MAX_NUM_OF_ROW_OF_TABLE	10		// ���̺��� �ִ� �� ��
#define LINE_ON_SENSITIVITY	4			// ������ ���콺 on���� ���� �ΰ���


class CLineBaseObj : public CObject  
{
public:
	virtual CPoint GetPos(int nType);
	virtual void SetPos(CPoint pt, int nType);
	virtual void Draw(CDC* pDC);
	CLineBaseObj();
	virtual ~CLineBaseObj();

};

#endif // !defined(AFX_LINEBASEOBJ_H__4058CFD8_D932_4354_9DCF_F85543CA7678__INCLUDED_)
