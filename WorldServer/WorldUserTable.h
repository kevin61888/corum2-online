#ifndef __WORLD_USER_TABLE__
#define __WORLD_USER_TABLE__

#pragma once

#include "../4DyuchiGXGFunc/global.h"

#include "OnlyList.h"
#include "WorldUser.h"
#include "NameSearchTable.h"

#define MAX_CLASS		5

typedef struct BASE_CLASS_INFO
{
	int		iEGO;
	int		iSTR;
	int		iINT;
	int		iDEX;
	int		iVIT;
	int		iBHP;
	int		iBMP;
	int		iLHP;
	int		iLMP;
	int		iHP;
	int		iMP;
	int		iAA;
	int		iAD;
	int		iMD;
	int		iDP;
	int		iBR;
	int		iDR;
	int		iHR;
	int		iMS;
	int		iAS;
	int		iCS;
	int		iRFire;
	int		iRIce;
	int		iRLighting;
	int		iRPhisics;
	int		iRPoison;
	int		iMax_Aura;
	int		Max_Magic;
	int		Max_Divine;
	int		Max_Chakra;
	int		Max_Summon;
}* LPBASE_CLASS_INFO;



class CWorldUserTable
{
	
	DWORD					m_dwUserNum;			//Global User Count	

	STMPOOL_HANDLE			m_pUserPool;		//유저정보 Alloc할 메모리풀 포인터 
	CNameSearchTable*		m_pNameSearch;		//유저 이름으로 찾기 위한 String Hash Table

	void					RemoveWorldUserTable();
	
	BASE_CLASS_INFO			m_BaseClassInfo[ MAX_CLASS + 1 ];	

public:								  

	COnlyList*				m_pTotalUserList;		//전체 유저 리스트 
	COnlyList*				m_pUserListInWorld;		//월드맵에 있는 유저 리스트 
	DWORD					m_dwMaxBucketNum;	
	CWorldUser**			m_ppInfoTable;
	LPBASE_CLASS_INFO		GetBaseClassInfo( BYTE bClassType )	{	return &m_BaseClassInfo[ bClassType ];	}

	CWorldUser*				AllocNewUser();
	CWorldUser*				GetUserInfo(DWORD dwUserIndex);
	CWorldUser*				GetUserInfoByName(const char* szCharacterName);		//Name으로 서치 
	CWorldUser*				GetUserInfoByNameWithoutCase(const char* szCharacterName);	//Name으로 서치, case 무시.
	BOOL					Add(CWorldUser* pUser);
	void					Remove(DWORD dwConnectionIndex);
	void					Remove(CWorldUser* pInfo);
	void					BroadCast(char* pPacket, DWORD dwLength);
	void					SendUserInWorld(char* pPacket, DWORD dwLength);
	DWORD					GetCount()	{	return m_dwUserNum;		}
	void					RemoveUserResource(CWorldUser* pUser);
	BOOL					AddCharacterToSearchTable(CWorldUser* pUser);

//	void					DeleteUserFromDBLoginTable(char* szID);	//SYSTEM DB의 ConnectingTable에서 유저 삭제 
	
	CWorldUserTable( DWORD dwBucketNum, DWORD dwPoolSize );
	~CWorldUserTable();
};


extern CWorldUserTable* g_pUserTable;


#endif // __WORLD_USER_TABLE__