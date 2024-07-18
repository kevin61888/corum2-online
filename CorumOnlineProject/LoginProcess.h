#pragma once
#include "GlobalVariableLogin.h"

#define CHAR_SELECT_STATUS_NORMAL					0
#define CHAR_SELECT_STATUS_CHAR_MAKING				1		//캐릭터 만들고 있는중 
#define CHAR_SELECT_STATUS_CHAR_ROTATING			2		//캐릭터 돌리고 있는중 
#define CHAR_SELECT_STATUS_AWAITING_CREATE_RESULT	3		//
#define CHAR_SELECT_STATUS_AWAITING_DELETE_RESULT	4
#define CHAR_SELECT_STATUS_INPUT_JUMIN_NUM			5		//캐릭 삭제를 위해 주민등록 번호 입력중 
#define CHAR_SELECT_STATUS_STARTED					100		//Start 버튼만 누른상태 
#define CHAR_SELECT_STATUS_START_SUCCESS			101		//Start 버튼 누른후 월드맵 조인 성공상태 

#define INTRO_FLAG_DEMO_SHOWING						1
#define INTRO_FLAG_WINDOW_FADE_IN					2
#define INTRO_FLAG_SERVER_SELECT					0
#define INTRO_FLAG_CONTINUE_SHOWING					3
#define INTRO_FLAG_LOGIN_INPUT						4		// 역시 중국 버전용!

#define LOGIN_FAIL_NO_PAY							0
#define LOGIN_FAIL_SQL_ERROR						7
#define LOGIN_FAIL_INVALID_PASSWORD					3
#define LOGIN_FAIL_NO_ID							2
#define LOGIN_FAIL_OVERLAPPED_ID					4
#define LOGIN_FAIL_USER_OVERFLOW					8
#define LOGIN_FAIL_INVALID_VERSION					9
#define LOGIN_FAIL_NOW_TRY_TO_LOGIN					10
#define LOGIN_FAIL_NOT_MIRI_USER					11
#define LOGIN_FAIL_18_OVER							12
#define LOGIN_FAIL_NOT_REAL_NAME					13
#define LOGIN_FAIL_USER_BLOCK						5
#define LOGIN_FAIL_USER_NOT_TEST_ID					6

#define LOGIN_FAIL_NEXT								18
#define LOGIN_FAIL_ETC								19
#define LOGIN_FAIL_CHARACTER_OVERLAPPED				20	
#define LOGIN_FAIL_INVALID_CHARACTER_INDEX			21	
#define LOGIN_FAIL_14_OVER							22
#define LOGIN_FAIL_OVERLAPPED_ID_ANOTHER_SERVER		23
#define LOGIN_FAIL_EJECT_USER						50	

#define LOGIN_FAIL_NOT_BETA_TESTER					14		//일본, 베타테스터 아님 
#define LOGIN_FAIL_EJECT_USER_JAPAN					15		//일본, 강퇴자 
#define LOGIN_FAIL_SECESSION_USER					16		//일본, 탈퇴한 회원	

struct NETMARBLE_LOGIN;

extern LPGlobalVariable_Login	g_pGVLogin;
extern NETMARBLE_LOGIN			g_NetMarble;
extern BOOL						g_bExitChk;

// 기본 함수.
BOOL InitGameLogin();
void UpdateGameLogin();
void ReleaseGameLogin();
void InitPacketProcLogin();
void OnTimerEventLogin(DWORD dwTimerIndex);

// 렌더 함수.
DWORD __stdcall AfterRenderGameLogin();
DWORD __stdcall BeforeRenderGameLogin();

// 키입력 함수.
void OnKeyDownLogin(WPARAM wParam, LPARAM lParam);
void OnKeyUpLogin(WPARAM wParam, LPARAM lParam);
void OnCharLogin(BYTE bCh);

// 마우스 입력 함수.
void OnLButtonDownLogin(WPARAM wParam, LPARAM lParam);
void OnLButtonUpLogin(WPARAM wParam, LPARAM lParam);
void OnRButtonDownLogin(WPARAM wParam, LPARAM lParam);
void OnRButtonUpLogin(WPARAM wParam, LPARAM lParam);
void OnMouseMoveLogin(WPARAM wParam, LPARAM lParam);
void OnMouseWheelLogin(WPARAM wParam, LPARAM lParam);

// 기타 함수.
void LoginProcess();
void LoginInterface(BOOL bActive);