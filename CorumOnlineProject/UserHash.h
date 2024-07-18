#pragma once
#include "../CommonServer/AbyssHash.h"
#include "User.h"


class CUserHash : public CAbyssHash<CUser>
{
//	void	Destroy( BOOL bDeleteData = FALSE );
//	void	Delete( Td* pdata, DWORD dwKey, BOOL bDeleteData = FALSE );
};

