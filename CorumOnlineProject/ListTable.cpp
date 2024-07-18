#include "ListTable.h"
#include "InitGame.h"

void DeleteAllChatList()
{
	if(g_pWhisperChatList)
	{
		POSITION_ pos = g_pWhisperChatList->GetHeadPosition();	
		POSITION_ posTemp;
		
		while(pos)
		{		
			posTemp = pos;

			LP_CHAT_MSG lpChatNode = (LP_CHAT_MSG)g_pWhisperChatList->GetNext(pos);
			LALFree(g_pChatPool, lpChatNode);
			lpChatNode	= NULL;
			g_pWhisperChatList->RemoveAt(posTemp);
		}		
	}
}

void DeleteGuildChatList()
{
	if(g_pGuildChatList)
	{
		POSITION_ pos = g_pGuildChatList->GetHeadPosition();	
		POSITION_ posTemp;
		
		while(pos)
		{		
			posTemp = pos;

			LP_CHAT_MSG lpChatNode = (LP_CHAT_MSG)g_pGuildChatList->GetNext(pos);
			LALFree(g_pChatPool, lpChatNode);
			lpChatNode	= NULL;
			g_pGuildChatList->RemoveAt(posTemp);
		}		
	}
}

void DeleteFriendChatList()
{
	if(g_pFriendChatList)
	{
		POSITION_ pos = g_pFriendChatList->GetHeadPosition();	
		POSITION_ posTemp;
		
		while(pos)
		{		
			posTemp = pos;

			LP_CHAT_MSG lpChatNode = (LP_CHAT_MSG)g_pFriendChatList->GetNext(pos);
			LALFree(g_pChatPool, lpChatNode);
			lpChatNode	= NULL;
			g_pFriendChatList->RemoveAt(posTemp);
		}		
	}
}

void DeleteWhisperChatList()
{
	if(g_pWhisperChatList)
	{
		POSITION_ pos = g_pWhisperChatList->GetHeadPosition();	
		POSITION_ posTemp;
		
		while(pos)
		{		
			posTemp = pos;

			LP_CHAT_MSG lpChatNode = (LP_CHAT_MSG)g_pWhisperChatList->GetNext(pos);
			LALFree(g_pChatPool, lpChatNode);
			lpChatNode	= NULL;
			g_pWhisperChatList->RemoveAt(posTemp);
		}		
	}
}

void DeletePartyChatList()
{
	if(g_pPartyChatList)
	{
		POSITION_ pos = g_pPartyChatList->GetHeadPosition();	
		POSITION_ posTemp;
		
		while(pos)
		{		
			posTemp = pos;

			LP_CHAT_MSG lpChatNode = (LP_CHAT_MSG)g_pPartyChatList->GetNext(pos);
			LALFree(g_pChatPool, lpChatNode);
			lpChatNode	= NULL;
			g_pPartyChatList->RemoveAt(posTemp);
		}
	}
}
	

	
	

	
