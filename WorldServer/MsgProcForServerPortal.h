#pragma once


struct SERVER_DATA;


int	 ChkDungeonPortalCondition( WORD wDungeonID );
void CmdRequestUserAcceptForPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdUserAcceptAllowedForPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdUserAcceptFailedForPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
