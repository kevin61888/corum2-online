#pragma once

#include "ObjectDefine.h"

#define TOTAL_BUTTON_NUM			14
#define TOTAL_OVER_BUTTON_NUM		9
#define TOTAL_DOWN_BUTTON_NUM		14

#define CHAR_SEL_MOUSE_BUTTON		0
#define CHAR_SEL_MOUSE_DEFAULT		1

#define CHAR_SEL_MOUSE_OVER		0
#define CHAR_SEL_MOUSE_DOWN		1
#define CHAR_SEL_MOUSE_UP		2

struct	WSTC_WORLD_USER_INFO;	
struct	RS_CharacterLogInfo;
struct  OBJECT_DESC;


class	CUser;
extern	BOOL g_bCharacterChk;

// 기본 함수 //
BOOL InitGameCharSelect();
void UpdateGameCharSelect();
void ReleaseGameCharSelect();
void OnTimerEventCharSelect(DWORD dwTimerIndex);
void InitPacketProcCharSelect();

// 렌더 함수 //
DWORD __stdcall AfterRenderGameCharSelect();
DWORD __stdcall BeforeRenderGameCharSelect();

// 키처리 함수 //
void OnKeyDownCharSelect(WPARAM wParam, LPARAM lParam);
void OnKeyUpCharSelect(WPARAM wParam, LPARAM lParam);
void OnCharCharSelect(BYTE bCh);

// 마우스 처리 함수 //
void OnLButtonDownCharSelect(WPARAM wParam, LPARAM lParam);
void OnLButtonUpCharSelect(WPARAM wParam, LPARAM lParam);
void OnRButtonDownCharSelect(WPARAM wParam, LPARAM lParam);
void OnRButtonUpCharSelect(WPARAM wParam, LPARAM lParam);
void OnMouseMoveCharSelect(WPARAM wParam, LPARAM lParam);
void OnMouseWheelCharSelect(WPARAM wParam, LPARAM lParam);

void MouseType(BYTE byType);
void CharSelectMousePointer(WPARAM wParam, LPARAM lParam);
void CharSelectMouseProcess(WPARAM wParam, LPARAM lParam, BYTE byType);
void OnSprButtonProcessCharSelect(DWORD dwSprID);

// 장착 아이템 관련 처리 //
void SetLHandChangeChr(CUser* pUser, DWORD dwId);
void SetRHandChangeChr(CUser* pUser, DWORD dwId);

// by deepdark for character filtering.
BOOL IsSpecialCharacter(char* szCharacterName);

// 기타 함수들 //
void DisplayCharacterInfo(RS_CharacterLogInfo* pInfo);
void HideCreateWindow();
void OnChangeClass(BYTE bClassIndex);
void CharSelectGroundLeft(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void CharSelectGroundRight(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void CharSelectRotateLeft(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void CharSelectRotateRight(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void CharSelectHighlightMotion(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
BOOL CreateMainPlayer(WSTC_WORLD_USER_INFO* pAddInfo);
BOOL CheckCompleteRotate();

BOOL SendCharacterDeleteMsg();