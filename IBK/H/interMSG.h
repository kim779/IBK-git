#pragma once

#define WM_INTERMSG								 WM_USER + 2022
#define WM_MSG_MAINTOMAP					 WM_USER + 2023

#define MMSG_SET_INTEREMAP							8    //IBXXXX12 �ε��� ������ �ڵ� ����
#define MMSG_SEARCH_INTERGROUP				9   //���ɱ׷� ����Ʈ ��ȸ
#define MMSG_RET_SEARCH_INTERGROUP		10   //���ɱ׷� ����Ʈ ��ȸ ��� ����
#define MMSG_SEARCH_GROUPCODE				11   //���ɱ׷� ���� ��ȸ
#define MMSG_RET_SEARCH_GROUPCODE		12   //���ɱ׷� ���� ��ȸ ��� ����
#define MMSG_INTER_DBBACKUP						13   //���� DB��� -> ���ɼ���â�� ���� ������
#define MMSG_RET_INTER_DBBACKUP				14  //���� DB��� ��ȸ��� ����
#define MMSG_INTER_NEWGROUP					15   //���� ���׷���
#define MMSG_RET_INTER_NEWGROUP				16   //���� ���׷���  ��ȸ ��� ����
#define MMSG_INTER_GROUPCODESAVE			17  //���� �׷쳻����������
#define MMSG_RET_INTER_GROUPCODESAVE	18  //���� �׷쳻����������
#define MMSG_SEARCH_SISE								19  //���� �׷��ڵ��� �ü� ��ȸ
#define MMSG_TIME_INVESTTUJA						20 //ƼĿ ������ Ÿ�̸�(�ӽ�)