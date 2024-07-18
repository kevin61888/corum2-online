#ifndef _CHATLOG_H_
#pragma once

#include "../CommonServer/CommonHeader.h"

class CWorldUser;

BOOL SetChatLogInWorld(CHAT_TYPE eChatType, CWorldUser * pSendUser, CWorldUser * pRecvUser, char * pszChat, int ChatLlen);


#endif
// _CHATLOG_H_