#pragma once

#include "stdafx.h"

void	CmdWorldMove(char* pMsg, DWORD dwLen);
void	CmdSkillKey( char* pMsg, DWORD dwLen );
void	CmdNotice(char* pMsg, DWORD dwLen);
void	CmdCommandMsg(char* pMsg, DWORD dwLen);
void	CmdDungeonJoinFailed( char* pMsg, DWORD dwLen );
void	CmdConnectDungeonServer( char* pMsg, DWORD dwLen );
void	CmdChatUser(char* pMsg, DWORD dwLen);
void	CmdCommandMessageRQ(char* pMsg, DWORD dwLen);
void	CmdCommandMessage(char* pMsg, DWORD dwLen);
void	CmdWhisper(char* pMsg, DWORD dwLen);
void	CmdCommandFail(char* pMsg, DWORD dwLen);
void	CmdBillingInfo(char* pMsg, DWORD dwLen);
void	CmdWorldAppear(char* pMsg, DWORD dwLen);
void	CmdWorldStop(char* pMsg, DWORD dwLen);
void	CmdPartyUserTryToConnect(char* pMsg, DWORD dwLen);
void	CmdWorldSetPosition(char* pMsg, DWORD dwLen);
void	CmdWorldPartyUserInfo(char* pMsg, DWORD dwLen);
void	CmdCheckConnection(char* pMsg, DWORD dwLen);
void	CmdAirshipRiding(char* pMsg, DWORD dwLen);
void	CmdAirshipGetoff(char* pMsg, DWORD dwLen);
void	CmdWorldDisappear(char* pMsg, DWORD dwLen);
void	CmdGmCommandResult( char* pMsg, DWORD dwLen );
void	CmdInvalidGM( char* pMsg, DWORD dwLen );
void	CmdGMLoginSuccess( char* pMsg, DWORD dwLen );
void	CmdCBMessage( char* pMsg, DWORD dwLen );