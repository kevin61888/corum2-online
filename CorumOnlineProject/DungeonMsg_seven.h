#pragma once

#include "struct.h"

void	InitPacketProcDungeonSeven();

void	CmdEmoticon(char* pMsg, DWORD dwLen);
void	CmdDungeonRequestMatchM2M(char* pMsg, DWORD dwLen);
void	CmdDungeonReplyMatchM2M(char* pMsg, DWORD dwLen);
void	CmdReadyMatchM2M(char* pMsg, DWORD dwLen);
void	CmdStartMatchM2M(char* pMsg, DWORD dwLen);
void	CmdEndMatchM2M(char* pMsg, DWORD dwLen);
void	CmdMatchUserInfo(char* pMsg, DWORD dwLen);