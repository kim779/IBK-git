#pragma once

#define WM_INTERMSG								 WM_USER + 2022
#define WM_MSG_MAINTOMAP					 WM_USER + 2023

#define MMSG_SET_INTEREMAP							8    //IBXXXX12 로드후 윈도우 핸들 저장
#define MMSG_SEARCH_INTERGROUP				9   //관심그룹 리스트 조회
#define MMSG_RET_SEARCH_INTERGROUP		10   //관심그룹 리스트 조회 결과 전달
#define MMSG_SEARCH_GROUPCODE				11   //관심그룹 종목 조회
#define MMSG_RET_SEARCH_GROUPCODE		12   //관심그룹 종목 조회 결과 전달
#define MMSG_INTER_DBBACKUP						13   //관심 DB백업 -> 관심설정창을 열면 날린다
#define MMSG_RET_INTER_DBBACKUP				14  //관심 DB백업 조회결과 전달
#define MMSG_INTER_NEWGROUP					15   //관심 새그룹등록
#define MMSG_RET_INTER_NEWGROUP				16   //관심 새그룹등록  조회 결과 전달
#define MMSG_INTER_GROUPCODESAVE			17  //관심 그룹내부종목저장
#define MMSG_RET_INTER_GROUPCODESAVE	18  //관심 그룹내부종목저장
#define MMSG_SEARCH_SISE								19  //관심 그룹코드의 시세 조회
#define MMSG_TIME_INVESTTUJA						20 //티커 투자자 타이머(임시)