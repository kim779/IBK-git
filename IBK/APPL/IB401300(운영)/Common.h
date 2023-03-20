#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

#define USDIR				"user"
#define USER_CONFIG			"userconf.ini"		// ���� ���� ����
#define USER_SECTION		"IB401300"			// ���� ����
#define USER_EVENT_KEY		"USER_EVENT"		// Ŭ�� �̺�Ʈ
#define USER_SCREEN_KEY		"USER_SCREEN"		// ���� ȭ��
#define USER_SCREEN_NO_KEY	"USER_SCREEN_NO"	// ���� ȭ�� ��ȣ
#define USER_SCR_NAME_KEY	"USER_SCR_NAME"		// ���� ȭ�� �̸�
#define USER_FSCREEN_KEY	"USER_FSCREEN"		// ���� ȭ��
#define USER_FSCREEN_NO_KEY	"USER_FSCREEN_NO"	// ���� ȭ�� ��ȣ
#define USER_FSCR_NAME_KEY	"USER_FSCR_NAME"	// ���� ȭ�� �̸�
#define USER_CX				"CX"
#define USER_CY				"CY"
#define USER_STYLE			"STYLE"
#define USER_KIND			"KIND"
#define	deliMITER	";"

#define DEFAULT_SCREEN		"IB400200"		// Default ȭ�� ��ȣ
#define DEFAULT_SCR_NAME	"�ɼ����簡"		// Default ȭ�� �̸�
#define DEFAULT_FSCREEN		"IB400100"	
#define DEFAULT_FSCR_NAME	"�������簡"

#define FCOD			"30301"
#define	OCOD			"40301"

// define Color!
#define COLOR_LINE			93
#define COLOR_TABLE			75
#define COLOR_BK			64
#define COLOR_GRIDHEAD		74
#define COLOR_GUIDEBACK		213
#define COLOR_PANELBACK		66
#define COLOR_BODYBACK		181
#define COLOR_GRIDHEADTXT	76
#define COLOR_DATA			99
#define COLOR_TEXT			69
#define COLOR_TABLEHEAD		96
#define	COLOR_MDB			171		// �ŵ� back
#define	COLOR_MSB			170		// �ż� back
#define COLOR_PLUSTXT		94
#define COLOR_MINUSTXT		95
#define COLOR_FOCUS			78
#define COLOR_CONTROLBACK	66

#define PADDING_VALUE	(6)

// Control ������ ����
#define CONTROL_WND_HEIGHT	(31)

// �����׸��� ����
#define FUTURE_WND_HEIGHT	(146)

// �׸��� �� ����
#define GRID_CELL_HEIGHT	(18)

#define MAX_FHOGA			(5)
#define FCODE_LEN			(8)
#define DOUBLE_PREFIX		(0.00005)

typedef enum { CT_NONE=0, CT_FUTURE, CT_OPTION, CT_SPREAD } CODE_TYPE;

