#ifndef __CTRL_NOTIFY_H__
#define __CTRL_NOTIFY_H__

//
// main�� CNeoChart2001Ctrl���� gateway ������ �Ҷ� �޴� Notify message code
//
#define SEND_CANDLEDATA			0		// ��Ʈ ��Ʈ�ѿ��� ���콺 �̵���, �ص� ��ġ�� ĵ�� ������(��,��,��,��)�� ������
#define APPLY_SIGNAL			1		// ��ȣ���� ���̾�α׿��� íƮ�� ��ȣ�����Ŵ
#define ADD_INDICATOR			2		// ������ǥ�߰�
#define GET_CODEINFO			3		// �ڵ忡 �ش��ϴ� ������� ���´�.
#define ADD_CHART_TOOL			4		// chart tool ����/toggle
#define CHANGE_CHART_CONFIG		5		// '������ǥ�߰�'���� ����ڰ� ������ config�� chart����
#define SAVE_CHART_CONFIG		6		// '������ǥ�߰�'���� ����ڰ� ������ config ����
#define SHOW_SHORTCUT_DLG		7		// ��Ʈ ����Ű ����â ����
#define SHOW_SHORTCUT_CONTEXT	8		// ��Ʈ ����Ű ���� ���� 

#define	DLLVIEW_SIZE_INFO		100		// dll ������ ����
#define	NEWSOUTBAR_RELOAD		500		// ���� �ƿ��ٿ� ���ã�� ���ΰ�ħ
#define	GET_CONCERNGROUP		501		// ���� �׷� ����
#define GET_CONCERNITEM			502		// ���� ���� ����
#define	GET_ACCOUNTINFO			503		// DLLȭ�鿡�� ����� ������ ��û
#define GET_UPJONGMASETR		504		// �� ���� �ڵ�,�̸��� ��´�(�ŷ���, �ڽ���, �׸�)
										// WPARAM 0�ŷ��� 1�ڽ��� 2�׸�
#define	ADD_CONCERNITEM			505		// ���ɱ׷쿡 ���� �߰�
#define GET_USERCOLORTABLE		506		// Color Table ��û
#define GET_SINGLECOLOR			507		// ���� Color ��û
#define DEL_SHARENAME			508


// �ڵ��Ÿ� ����â -> ��Ʈ
#define	AUTO_START				10		// �ڵ��ŸŰ� ���۵Ǿ����ϴ�.			
#define AUTO_SETCHART			11		// �ڵ��ŸŸ� ������ ��Ʈ�ڵ� �Ѱ��ش�
#define AUTO_SETHEADER			12		// "�����(�����ȣ) ��/��/��/��/ƽ ���簡" ���ڿ��� �Ѱ���

// main -> �ڵ��Ÿ� ����â or chart -> �ڵ��Ÿ� ����â
#define AUTO_SETACCOUNTINFO		13		// �������� class(CAcctHelper)�� pointer�� �Ѱ��ش�

// ��Ʈ -> �ڵ��ֹ� Main dlg
#define AUTO_REGISTER			14		// �������� ���¸� ����Ѵ�.
#define AUTO_UNREGISTER			15		// �ش���� �ڵ��Ÿ� ����
#define AUTO_REFRESH			16		// ��ȣ����� �������� update�䱸

#define AUTO_ROBOTCONFIRM		17		// ��Ʈ�� �κ��� ������ : chart���� �ڵ��ֹ�Main���� AUTO_REGISTER
										// �� ������ �ڵ��ֹ�Main�� chart�� robot handle�� �Բ������� msg code

// �ڵ��ֹ� Main dlg -> �ڵ��ֹ� robot
#define AUTO_BUY				18		// �ڵ� �ż� �ֹ�
#define AUTO_SELL				19		// �ڵ� �ŵ� �ֹ�
#define CONFIRM_BUY				20		// Ȯ�� �ż� �ֹ� ?
#define CONFIRM_SELL			21		// Ȯ�� �ŵ� �ֹ� ?

#define AUTO_CLOSE				22		// ���� ��û �޼���

#define AUTO_SOCKET_HANDLE		23		// main���� socket handle�� ������	

#define CHART_LINE_THICKNESS	24		// ��Ʈ ���� �β�
#define CHART_LINE_COLOR		25		// ��Ʈ ���� �÷�

// ���� -> chart
#define AUTO_SET_MAINHANDLE		26		// �ڵ��ֹ� �ڵ��� �ѱ涧

#endif
