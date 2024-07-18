#ifndef __CERTAIN_IP_LIST_H__
#define __CERTAIN_IP_LIST_H__

#pragma once

#include "../CommonServer/AbyssHash.h"

class CCertainIPList : public CAbyssHash<DWORD>
{
public:
	void DeleteData( void* pData )
	{
		delete pData;
	}
};

extern CCertainIPList*	g_pCertainIPList;


#endif // __CERTAIN_IP_LIST_H__