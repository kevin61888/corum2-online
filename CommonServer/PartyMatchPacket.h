#pragma once

#include "ProtocolDefinition.h"

// 매칭 조건
struct PMATCH_CONDITION
{
	BYTE bLevel;		// 레벨차 3, 5, 10
	BYTE bExp;		// 경험치 분배 방식 : 1 공통 , 2 레벨, 3 보너스만 분배, 4 상관 없음
	BYTE bItem;		// 아이템 부내 방식 : 1 습득자, 2 마지막 타격자, 3 랜덤, 4 상관 없음
	BYTE bMaster;	// 파티장으로 참여 여부 : 1 파티장으로, 0 파티원으로(파티장이 될 수도 있음)
};

// 대기자 등록 의뢰
struct CTWS_PMATCH_REGISTER : WORLD_PACKET_HEADER
{
	PMATCH_CONDITION condition;		// 매칭 조건
	CTWS_PMATCH_REGISTER()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_REGISTER;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PMATCH_REGISTER);	}
};

// 대기자 등록 결과
struct WSTC_PMATCH_REGISTER_RESULT : WORLD_PACKET_HEADER
{
	BYTE bResult;	// 0 : 성공, 아니면 에러코드
	WSTC_PMATCH_REGISTER_RESULT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_REGISTER_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PMATCH_REGISTER_RESULT);	}
};

// 대기자 탈되
struct CTWS_PMATCH_ESCAPE : WORLD_PACKET_HEADER
{
	CTWS_PMATCH_ESCAPE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_ESCAPE;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PMATCH_ESCAPE);	}
};

// 대기자 탈되 결과
struct WSTC_PMATCH_ESCAPE_RESULT : WORLD_PACKET_HEADER
{
	BYTE bResult;	// 0 : 성공, 아니면 에러코드
	WSTC_PMATCH_ESCAPE_RESULT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_ESCAPE_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PMATCH_ESCAPE_RESULT);	}
};

// 매칭 의뢰
struct WSTC_PMATCH_REQUEST : WORLD_PACKET_HEADER
{
	BYTE	bLevel;									// 레벨
	BYTE	bClass;									// 클래스
	char	szName[ MAX_CHARACTER_NAME_LENGTH ];	// 이름
	BYTE	bMaster;								// 파장
	WSTC_PMATCH_REQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_REQUEST;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PMATCH_REQUEST);	}
};

// 매칭 의뢰 결과
struct CTWS_PMATCH_REQUEST_RESULT : WORLD_PACKET_HEADER
{
	BYTE bResult;	// 0 : 성공, 아니면 에러코드
	CTWS_PMATCH_REQUEST_RESULT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_REQUEST_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PMATCH_REQUEST_RESULT);	}
};

// 매칭 조건 변경
struct CTWS_PMATCH_CHANGE : WORLD_PACKET_HEADER
{
	PMATCH_CONDITION condition;		// 새로운 매칭 조건
	CTWS_PMATCH_CHANGE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_CHANGE;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PMATCH_CHANGE);	}
};

// 매칭 조건 변경 결과
struct WSTC_PMATCH_CHANGE_RESULT : WORLD_PACKET_HEADER
{
	BYTE bResult;	// 0 : 성공, 아니면 에러코드
	WSTC_PMATCH_CHANGE_RESULT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PMATCH_CHANGE_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PMATCH_CHANGE_RESULT);	}
};