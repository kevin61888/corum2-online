#ifndef __MSGPROC_H__
#define __MSGPROC_H__

#pragma once

class CUser;


void CmdCheckMonitoring(	CUser* pUser ,char* pMsg,DWORD dwLength);
void CmdCMKickUser(			CUser* pUser,char* pMsg,DWORD dwLength);
void CmdCMToolNotice(		CUser* pUser ,char* pMsg,DWORD dwLength);
void CmdIsAliveUser(		CUser* pUser ,char* pMsg,DWORD dwLength);
void CmdLogin(				DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdLoginOverlapped(	CUser* pUser ,char* pMsg,DWORD dwLength);
void CmdCharacterSelect(	CUser* pUser ,char* pMsg,DWORD dwLength);
void CmdCreateNewCharacter(	CUser* pUser ,char* pMsg,DWORD dwLength);
void CmdDeleteCharacter(	CUser* pUser ,char* pMsg,DWORD dwLength);
void CmdCheckGM(			CUser* pUser ,char* pMsg,DWORD dwLength);
void CmdRequestServerAlive(	CUser* pUser ,char* pMsg,DWORD dwLength);


#endif // __MSGPROC_H__