#ifndef __CHINA_BILLING_PROC_H__
#define __CHINA_BILLING_PROC_H__

#pragma once

enum ECHINA_BILLING_CHECK
{
	enum_level_approval		= 1,
	enum_level_free			= 2,
	enum_level_limit		= 3,
};

struct SCHINA_BILLING_INFO
{
	ECHINA_BILLING_CHECK	eType;
	DWORD					dwPoint;
};

// Kick 정보.
struct CHINA_KICK_USER_DATA
{
	DWORD dwCharacterIndex;
	DWORD dwKickDecistionTime;
};

// Kick 체크 타임에 실행되는 callback 함수.
void __stdcall OnChinaKickOut(DWORD dwEventIndex);

// 실제 월드 및 던전에서 Kick 하는 함수.
void ChinaBillingKickOutProc(DWORD dwCharacterIndex);

// Kick 유저 등록
void AddChinaBillingKickList(DWORD dwCharacterIndex);

void AcmdChinaBillingInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);

#endif // __CHINA_BILLING_PROC_H__
