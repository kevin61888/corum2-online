#include "stdafx.h"
#include "ThrowQuery.h"
#include "OwnServer.h"
#include "DBProc.h"
#include "ChinaBillingProc.h"
#include "../CommonServer/BaseItem.h"
#include "ServerFunction.h"

extern IOleDBThread*	g_pDb;
extern COwnServer*		g_pThis;
extern MACHINENAME		g_MachineName;
extern LPBASEITEM_HASH	g_pBaseItemHash;

void ThrowLogoutQuery(CUser* pUser)
{
	char szProc[0xff]={0,};

	switch( MY_NATION_CODE )
	{
	case NC_CHINA:
		{
			wsprintf(szProc, "EXEC CorumGameLogout_OpenBeta '%s'", pUser->m_szLoginID );
		}break;
	case NC_TAIWAN:
		{
			wsprintf(szProc, "EXEC up_GameLogout '%s'", pUser->m_szLoginID );
		}break;
	case NC_JAPAN:
		{
			wsprintf(szProc, "EXEC usp_CorumGameLogout '%s'", pUser->m_szLoginID );	
		}break;
	default:
		{
			if( IS_ABLE_SERVICE_TYPE(ST_BILLING) )//일반빌링 :050104 hwoarang
			{
				wsprintf(szProc, "EXEC up_GameLogout '%s'", pUser->m_szLoginID );
			}
			else
			{
				wsprintf(szProc, "EXEC up_GameLogout_FREE '%s'", pUser->m_szLoginID );
			}

			if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
			{
				wsprintf(szProc, "EXEC CorumGameLogout_OpenBeta '%s'", pUser->m_szLoginID );
			}
		}break;
	}

	g_pDb->THExecuteSQL(szProc, TRUE, QUERY_TYPE_USER_LOGOUT, (void*)pUser->GetPropID(), (BYTE)MEMBER_DB);

	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 : 050104 hwoarang
	{
		SendChinaBillingLogout(pUser->m_dwPropID, pUser->m_szLoginID, pUser->GetIP() );
	}
}

int ThrowLogoutAllQuery(BOOL bCallback)
{
	char szQuery[QUERY_STRING_SIZE]={0,};
	
	switch( MY_NATION_CODE )
	{
	case NC_CHINA:
		{
			wsprintf(szQuery, "CorumGameLogout_all_OpenBeta '%s'", g_pThis->GetServerSetCode() );
		}break;
	case NC_TAIWAN:
		{
			wsprintf(szQuery, "up_GameLogoutAll '%s'", g_pThis->GetServerSetCode() );
		}break;
	case NC_JAPAN:
		{
			wsprintf(szQuery, "usp_CorumGameLogout_all '%s'", g_pThis->GetServerSetCode() );
		}break;
	default:
		{
			if( IS_ABLE_SERVICE_TYPE(ST_BILLING) )//일반빌링 :050104 hwoarang
			{
				wsprintf(szQuery, "up_GameLogoutAll '%s'", g_pThis->GetServerSetCode() );
			}
			else
			{
				wsprintf(szQuery, "up_GameLogoutAll_FREE '%s'", g_pThis->GetServerSetCode() );
			}

			if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
			{
				wsprintf(szQuery, "CorumGameLogout_all_OpenBeta '%s'", g_pThis->GetServerSetCode() );
			}
		}break;
	}

	if(bCallback)
	{
		g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_LOGOUT_ALL, NULL, (BYTE)MEMBER_DB);
		return 1;
	}
	else
	{
		return g_pDb->ExecuteSQL(szQuery, (BYTE)MEMBER_DB);
	}
}

void ThrowLoginQuery(CUser* pUser, char* szID, char* szPassword)
{
	char szProc[0xff]= {0,};

	switch( MY_NATION_CODE )
	{
	case NC_CHINA:
		{
			wsprintf(szProc,"EXEC CorumGameLogin_OB '%s', '%s', '%s', '%s'", szID, szPassword, pUser->GetIPString(), g_pThis->GetServerSetCode());	
		}break;
	case NC_TAIWAN:
		{
			wsprintf(szProc,"EXEC up_GameLogin '%s', '%s', '%s', '%s'", szID, szPassword, pUser->GetIPString(), g_pThis->GetServerSetCode());	
		}break;
	case NC_JAPAN:
		{
#ifndef ADULT_MODE
			wsprintf(szProc,"EXEC usp_CorumGameLogin '%s', '%s', '%s', '%s'", szID, szPassword, pUser->GetIPString(), g_pThis->GetServerSetCode());	
#else
			wsprintf(szProc,"EXEC usp_CorumGameLogin_Adult '%s', '%s', '%s', '%s'", szID, szPassword, pUser->GetIPString(), g_pThis->GetServerSetCode());	
#endif
		}break;
	default:
		{
			if( IS_ABLE_SERVICE_TYPE(ST_BILLING) )//일반빌링 :050104 hwoarang
			{
	#ifndef ADULT_MODE
				wsprintf(szProc,"EXEC up_GameLogin '%s', '%s', '%s', '%s'", szID, szPassword, pUser->GetIPString(), g_pThis->GetServerSetCode());	
	#else
				wsprintf(szProc,"EXEC up_GameLoginAdult '%s', '%s', '%s', '%s'", szID, szPassword, pUser->GetIPString(), g_pThis->GetServerSetCode());	
	#endif
			} // if( IS_ABLE_SERVICE_TYPE(ST_BILLING) )//일반빌링 :050104 hwoarang
			else
			{
	#ifndef ADULT_MODE
				wsprintf(szProc,"EXEC up_GameLogin_Free '%s', '%s', '%s', '%s'", szID, szPassword, pUser->GetIPString(), g_pThis->GetServerSetCode());	
	#else
				wsprintf(szProc,"EXEC up_GameLoginAdult_Free '%s', '%s', '%s', '%s'", szID, szPassword, pUser->GetIPString(), g_pThis->GetServerSetCode());
	#endif
			}
			if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
			{
				wsprintf(szProc,"EXEC CorumGameLogin_OB '%s', '%s', '%s', '%s'", szID, szPassword, pUser->GetIPString(), g_pThis->GetServerSetCode());
			}
		}break;
	}

	g_pDb->THOpenRecord(szProc, QUERY_TYPE_LOGIN, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)MEMBER_DB);
}

void ThrowInsertWebBoardQuery(CUser* pUser, char* szName, WORD wdClass)
{
	switch( MY_NATION_CODE )
	{
	case NC_CHINA:
	case NC_TAIWAN:
	case NC_JAPAN:
		{
			return;
		}break;
	}

	char szQuery[QUERY_STRING_SIZE] = {0,};

	// WebBoard용 Insert
	wsprintf(szQuery, "CZP_INSERT_NAME_FOR_WEB_BOARD '%s', '%s', '%s', %d", g_pThis->GetServerSetCode(), pUser->m_szLoginID, szName, wdClass);
	g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)MEMBER_DB);
}

void ThrowDeleteWebBoardQuery(CUser* pUser, char* szCharName)
{
	switch( MY_NATION_CODE )
	{
	case NC_CHINA:
	case NC_TAIWAN:
	case NC_JAPAN:
		{
			return;
		}break;
	}

	char szQuery[QUERY_STRING_SIZE]={0,};
	wsprintf(szQuery, "CZP_DELETE_NAME_FOR_WEB_BOARD '%s', '%s', '%s'", g_pThis->GetServerSetCode(),pUser->m_szLoginID, szCharName);
	g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)MEMBER_DB);
}

void ThrowInsertCharQuery(CUser* pUser, char* szCharName, BYTE btCharNum, WORD wdHead, BYTE btClass, WORD wdMapID)
{
	int nLen = strlen(szCharName);

	for(int i = 0; i < nLen; i++)
	{
		if(szCharName[i]==32)
			wsprintf(&szCharName[i], "");
	}

	char szQuery[ QUERY_STRING_SIZE ] = {0,};
	wsprintf(szQuery, "CZP_CREATE_NEW_CHARACTER '%s', '%s', %d, %d, %d, %d, '%s'"
		, pUser->m_szLoginID, szCharName, btCharNum, wdHead, btClass, wdMapID, pUser->GetIPString());

	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_CREATE_NEW_CHAR, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)GAME_DB);
}

void ThrowDeleteCharQuery(CUser* pUser, char* szCharName)
{
	char szQuery[ QUERY_STRING_SIZE ]={0,};
	wsprintf(szQuery, "CZP_DELETE_CHARACTER '%s', '%s', '%s'", pUser->m_szLoginID, szCharName, pUser->GetIPString());
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_DELETE_CHARACTER, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)GAME_DB);
}

void ThrowItemMallQuery(CUser* pUser)
{
	char szQuery[QUERY_STRING_SIZE]={0,};
	wsprintf(szQuery, "EXEC CZP_QUERY_ITEMMALL_INFO '%s', %s", pUser->m_szLoginID, g_pThis->GetServerSetCode());
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_ITEMMALL_CHECK, (void*)pUser->m_dwConnectionIndex, 255, (BYTE)MEMBER_DB );
}

void ThrowBlockTitleQuery(CUser* pUser, BOOL bBlock)
{
	if( IS_ABLE_NATION(NC_JAPAN))
	{
		char szQuery[QUERY_STRING_SIZE]={0,};

		int iType = bBlock ? 1 : 2; // 1: 경고, 2: 제재
		wsprintf(szQuery, "usp_block_title %d, '%s'", iType, pUser->m_szLoginID);		//제재 		//not billing check
		g_pDb->THOpenRecord(szQuery, QUERY_TYPE_BLOCK_TITLE, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)MEMBER_DB);
	}
}


void ThrowCreateCharInitBinaryQuery(CUser* pUser)
{
	DBBINDING*	pBinding		= 0;
	WORD		nColumn			= 6;
	DWORD		dwParamOffset	= 0;

	INIT_BINARY_VALUE value;
	memset(&value, 0, sizeof(INIT_BINARY_VALUE));

	value.m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID					= 1;
	value.m_pEquip[EQUIP_TYPE_RHAND1].m_Item_Weapon.wMin_Damage	= 1;
	value.m_pEquip[EQUIP_TYPE_RHAND1].m_Item_Weapon.wMax_Damage	= 3;
	value.m_pEquip[EQUIP_TYPE_RHAND1].m_Serial					= g_pThis->GetNewSerial();

	value.m_pInv_Small[0].m_Item_Supplies.bQuantity				= 20;
	value.m_pInv_Small[0].m_wItemID								= 10000;
	value.m_pInv_Small[0].m_Serial								= g_pThis->GetNewSerial();

	if(MY_NATION_CODE == NC_KOREA)
	{
		// 루디로스, 이다 귀환 스크롤 추가 : 최덕석 2005.3.7
		value.m_pInv_Small[1].m_Item_Zodiac.bType					= 1;
		value.m_pInv_Small[1].m_Item_Zodiac.wMapId					= 1;
		value.m_pInv_Small[1].m_Item_Zodiac.bQuantity				= 1;
		value.m_pInv_Small[1].m_wItemID								= 8012;
		value.m_pInv_Small[1].m_Serial								= g_pThis->GetNewSerial();
		value.m_pInv_Small[2].m_Item_Zodiac.bType					= 1;
		value.m_pInv_Small[2].m_Item_Zodiac.wMapId					= 1;
		value.m_pInv_Small[2].m_Item_Zodiac.bQuantity				= 1;
		value.m_pInv_Small[2].m_wItemID								= 8012;
		value.m_pInv_Small[2].m_Serial								= g_pThis->GetNewSerial();
		value.m_pInv_Small[3].m_Item_Zodiac.bType					= 1;
		value.m_pInv_Small[3].m_Item_Zodiac.wMapId					= 3;
		value.m_pInv_Small[3].m_Item_Zodiac.bQuantity				= 1;
		value.m_pInv_Small[3].m_wItemID								= 8013;
		value.m_pInv_Small[3].m_Serial								= g_pThis->GetNewSerial();
		value.m_pInv_Small[4].m_Item_Zodiac.bType					= 1;
		value.m_pInv_Small[4].m_Item_Zodiac.wMapId					= 3;
		value.m_pInv_Small[4].m_Item_Zodiac.bQuantity				= 1;
		value.m_pInv_Small[4].m_wItemID								= 8013;
		value.m_pInv_Small[4].m_Serial								= g_pThis->GetNewSerial();


	
		//050323 재원 
		//첫번째 만드는 캐릭일경우 && 계정생성일이 특정일(2004년5월31일23시59분59초) 이하 일경우 이벤트 아이템을 준다.				

		if( 1 == pUser->m_dwCharCount 
			 && pUser->m_ID_RegDate.year != 0 
			  && pUser->m_ID_RegDate.month != 0 
			 && pUser->m_ID_RegDate.day != 0 )
		{

			CTime cCreateTime(pUser->m_ID_RegDate.year, 
			pUser->m_ID_RegDate.month, 
			pUser->m_ID_RegDate.day, 
			pUser->m_ID_RegDate.hour, 
			pUser->m_ID_RegDate.minute, 
			pUser->m_ID_RegDate.second);			
			
			CTime cEventTime(2004,5,31,23,59,59);//2004년5월31일23시59분59초 

			CTimeSpan ts = cEventTime - cCreateTime;

			if( ts.GetTimeSpan() >= 0 )
			{
				// 경험치 3배 아이템. 6개 주기.
				// ** Consumable 아이템은 3가지 항목(bQuantity,ItemID,Serial)만 세팅해야 처음부터 사용중이 되지 않음 .. 희안하게도...ㅡ,.ㅡ;;;**				
				const WORD EVENT_ITEMID = 20213 ;
				for( int i = 0 ; i < 6 ; ++i )
				{
					value.m_pInv_Small[i+5].m_Item_Consumable.bQuantity		= 1;//수량
					value.m_pInv_Small[i+5].m_wItemID						= EVENT_ITEMID;												
					value.m_pInv_Small[i+5].m_Serial						= g_pThis->GetNewSerial();
				}
			}

									
		}
		
	}

	switch( MY_NATION_CODE )
	{
	case NC_CHINA:
	case NC_TAIWAN:
	case NC_JAPAN:
		{
			value.dwMoney = 0;	// 일본측의 요구로 돈은 0으로 세팅!
		}break;
	default:
		{
			value.dwMoney = 200; 
		}break;
	}
	
	pBinding = g_pDb->THCreateParamInfo(nColumn);
	if(!pBinding)
	{
		MessageBox(NULL,"Failed THCreateParamInfo",NULL,NULL);
	}

	pBinding[0].obValue		= dwParamOffset;	
	pBinding[0].cbMaxLen	= sizeof(value.m_pEquip);
	pBinding[0].wType		= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(value.m_pEquip);

	pBinding[1].obValue		= dwParamOffset;	
	pBinding[1].cbMaxLen	= sizeof(value.m_pInv_Large);
	pBinding[1].wType		= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(value.m_pInv_Large);

	pBinding[2].obValue		= dwParamOffset;	
	pBinding[2].cbMaxLen	= sizeof(value.m_pInv_Small);
	pBinding[2].wType		= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(value.m_pInv_Small);

	pBinding[3].obValue		= dwParamOffset;	
	pBinding[3].cbMaxLen	= sizeof(value.m_pBelt);
	pBinding[3].wType		= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(value.m_pBelt);

	pBinding[4].obValue		= dwParamOffset;	
	pBinding[4].cbMaxLen	= sizeof(value.m_pwSkillLevel);
	pBinding[4].wType		= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(value.m_pwSkillLevel);

	pBinding[5].obValue		= dwParamOffset;	
	pBinding[5].cbMaxLen	= sizeof(value.dwMoney);
	pBinding[5].wType		= DBTYPE_I4;					
	dwParamOffset			+= sizeof(value.dwMoney);

	char szQuery[QUERY_STRING_SIZE]= {0,};
	wsprintf(szQuery, "Update CharacterInfo Set Equip = ?, Inv_large = ?, Inv_small = ?, Belt = ?, SkillLevel = ?, [Money] = ? Where "
	 "[Name] = '%s'", pUser->GetSelectedCharacter()->szName);

	g_pDb->THExecuteSQLByParam(szQuery, pBinding, &value, sizeof(INIT_BINARY_VALUE), (BYTE)nColumn, TRUE, 
				QUERY_TYPE_CREATE_CHAR_INIT_BINARY, (void*)pUser->m_dwConnectionIndex, (BYTE)GAME_DB);
}