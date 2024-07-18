#ifndef __SERVER_FUNCTION_EVENT_H__
#define __SERVER_FUNCTION_EVENT_H__


#pragma once


void __stdcall CreateEventJudgment(DWORD dwVal);
BOOL QueryEventDungeonProperties();
void UnloadEventDungeonProperties();
bool UnloadNoticeMessage();
bool LoadNoticeMessage();
void UnLoadServerMessage();
bool LoadServerMessage();
char* GetServerMessage(int iCount);


#endif 
