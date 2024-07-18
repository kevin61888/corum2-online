#pragma once


void CmdRequestUserAcceptForPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdUserAcceptAllowedForPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdUserAcceptFailedForPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdUserPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
