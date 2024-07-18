#ifndef __RECV_MSG_H__
#define __RECV_MSG_H__


#pragma once


#include "WorldUserTable.h"
#include "ServerTable.h"
#include "OwnServer.h"
#include "../CommonServer/AbyssHash.h"


void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);
void __stdcall OnRecvFromServerTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall OnRecvFromUserTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall FailToConnectLoginAgentServer(void* pExt);	
void __stdcall SuccessToConnectLoginAgentServer(DWORD dwConnectionIndex, void* pExt); 


void	ReceivedFromDB(DBRECEIVEDATA* pResult);
void	InitializePacketProc();
int		GetPlayTime(DWORD dwCurTick, DWORD dwStartTick, DWORD dwFlag);
void	SaveCharacterByBlockMode(CWorldUser* pUser);


void	__SaveCharacter(CWorldUser* pUser);
void	__GuildDisconnectUser(CWorldUser* pUser);			
void	__PartyDisconnectUser(CWorldUser* pUser);			
void	__MessengerDisconnectUser(CWorldUser* pUser);

void ReceivedFromDBLog(DBRECEIVEDATA* pResult);	


#define AGENTINDEX		g_pThis->GetAgentConnectionIndex()		


#pragma pack(push,1)
struct SKILL_KEY
{
	BYTE	bySkillKey[16];
	BYTE	byPosKey[16];
};
#pragma pack(pop)


#endif 