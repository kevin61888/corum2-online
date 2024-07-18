#include "stdafx.h"
#include "DBProcItemMall.h"
#include "OwnServer.h"
#include "WorldServerTable.h"
#include "UserTable.h"
#include "recvmsg.h"
#include "ThrowQuery.h"
#include "User.h"
#include "PacketEncrypt.h"
#include "ServerFunction.h"
#include "../CommonServer/CommonHeader.h"
#include "ItemLog.h"


extern IOleDBThread*	g_pDb;
extern CTime			g_RecentCheckTime;
extern LPBASEITEM_HASH	g_pBaseItemHash;
extern MACHINENAME		g_MachineName;


void QueryTypeItemmallCheck(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;

	CBaseItem*	pBaseItem	= NULL;
	int			i			= 0;
	DWORD		dwIndex		= 0;
	int			nSize		= 0;

	if(pResult->nError<0)
	{
		Log(LOG_IMPORTANT, "Itemmall Query Check Failed! (ID:%s)", pUser->m_szLoginID );
		goto lb_ChrInfoQuery;
	}
	
	if(pResult->Query.select.dwRowCount == 0)
	{
		// 받은 아이템몰 선물이 없다. 
		goto lb_ChrInfoQuery;		
	}

	pUser->m_bItemMallCount	= (BYTE)pResult->Query.select.dwRowCount;
	pUser->m_pItemMallBox	= new ITEMMALL_BOX[pUser->m_bItemMallCount];
	memset(pUser->m_pItemMallBox, NULL, sizeof(pUser->m_pItemMallBox));

	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )
	{
		nSize = sizeof(ITEMMALL_BOX) * pUser->m_bItemMallCount;
	}

	for(i = 0; i < pUser->m_bItemMallCount; i++)
	{
		memcpy(&pUser->m_pItemMallBox[ dwIndex ], pResult->Query.select.pResult + (sizeof(ITEMMALL_BOX) * i),
					sizeof(ITEMMALL_BOX));
				
		pBaseItem = g_pBaseItemHash->GetData( pUser->m_pItemMallBox[i].dwItemID );

		// Item 정보가 없는 Item이 들어갔으면 
		if(!pBaseItem)
		{
			Log(LOG_IMPORTANT, "Itemmall Item Info is not found! (ID:%s, Item:%d)"
				, pUser->m_szLoginID, pUser->m_pItemMallBox[i].dwItemID);
			pUser->m_pItemMallBox[i].dwItemID	= 0;
			pUser->m_pItemMallBox[i].dwIdx		= 0;
			pUser->m_pItemMallBox[i].dwCount	= 0;
			continue;
		}

		dwIndex++;
	}

	pUser->m_bItemMallCount = (BYTE)dwIndex;	

	if(dwIndex == 0)
	{
		// 필터링에 걸려서 받을 아이템이 없다면 캐릭터 정보 쿼리!
		goto lb_ChrInfoQuery;	
	}
	else
	{
		// Item Mall에서 받을 아이템이 있다면 이 유저의 Bank정보를 쿼리하자!
		if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
		{
			if( nSize != (int)(pResult->Query.select.dwRowSize * pResult->Query.select.dwRowCount) )
			{
				__asm int 3
			}
		}
		char szQuery[ QUERY_STRING_SIZE ] = {0,};
		wsprintf(szQuery, "select Bank_large, Bank_small from BankInfo Where [ID]= '%s'", pUser->m_szLoginID);
		g_pDb->THOpenRecord(szQuery, QUERY_TYPE_BANKINFO, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)GAME_DB);
		return;
	}	

lb_ChrInfoQuery:
	if(pUser->m_pItemMallBox)
	{
		delete [] pUser->m_pItemMallBox;
		pUser->m_pItemMallBox	= NULL;		
	}

	QueryChrSelectInfo(pUser);
}


void QueryTypeBankinfo(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;

	ITEM_MALL	cBankItem = {0,};
	CItem		Item;
	DWORD		dwLargeEmptyCount = 0, dwSmallEmptyCount = 0, dwIndex = 0, dwItemMalLarge = 0, dwItemMalSmall = 0;
	DBBINDING*	pBindingBank	= 0;
	DWORD		dwParamOffset	= 0;
	char		szQuery[QUERY_STRING_SIZE]	= {0,};
	DWORD		dwItemId		= 0;
	int			nBankSmall		= 0;
	int			nBankLarge		= 0;
	int			i				= 0;

	if( pResult->nError<0 )
	{
		Log(LOG_IMPORTANT, "QueryTypeBankinfo Failed! (ID:%s)", pUser->m_szLoginID );
		goto lb_ChrInfoQuery2;
	}
	
	// 아이템몰에 상품은 있는데 기존에 생성된 캐릭터가 한개도 없는 경우 이런 경우에는 캐릭을 생성한 이후 첫 로그인시에 지급된다. 
	if( pResult->Query.select.dwRowCount == 0 )
	{
		goto lb_ChrInfoQuery2;
	}

	// Bank정보를 받아오고
	if( sizeof(cBankItem) < pResult->Query.select.dwRowSize  )
	{// 사이즈 체크 : hwoarang  050218 
		Log(LOG_IMPORTANT, "QueryTypeBankinfo Result To Large! (ID:%s)", pUser->m_szLoginID );
		goto lb_ChrInfoQuery2;
	}

	memcpy(&cBankItem, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

	// Bank빈칸 갯수 체크
	for( i=0; i<MAX_BANK_LARGE; i++ )
	{
		if( cBankItem.sBankItemLarge[i].GetID() == 0 )
			dwLargeEmptyCount++;
	}

	for( i=0; i<MAX_BANK_SMALL; i++ )
	{
		if( cBankItem.sBankItemSmall[i].GetID() == 0 )
			dwSmallEmptyCount++;
	}

	for( i = 0; i < pUser->m_bItemMallCount; i++ )
	{
		DWORD dwItemId = pUser->m_pItemMallBox[i].dwItemID / ITEM_DISTRIBUTE;
		
		if(dwItemId < 21)
			dwItemMalLarge +=1;
		else
			dwItemMalSmall +=1;
	}

	if( ( dwLargeEmptyCount == 0 && dwItemMalLarge != 0 && dwItemMalSmall == 0 ) || 
		( dwSmallEmptyCount == 0 && dwItemMalSmall != 0 && dwItemMalLarge == 0 )	)
	{
		pUser->m_wItemMallSupplyCount = 150;
		goto lb_ChrInfoQuery2;
	}
	
	const size_t full_size = sizeof(ITEMMALL_SUPPLY)*(dwLargeEmptyCount + dwSmallEmptyCount);

	pUser->m_pItemMallSupply = new ITEMMALL_SUPPLY[dwLargeEmptyCount + dwSmallEmptyCount];
	
	memset( pUser->m_pItemMallSupply, NULL, full_size);

	for( i = 0 ; i < pUser->m_bItemMallCount ; i++ )
	{
		dwItemId = pUser->m_pItemMallBox[ i ].dwItemID / ITEM_DISTRIBUTE;

		// 빈칸이 없다. 그만 돌아라.
		if( nBankLarge > MAX_BANK_LARGE 
		&&	nBankSmall > MAX_BANK_SMALL )
		{
			break;
		}

		if( dwItemId > 20 )		// Small Item
		{
			while( nBankSmall < MAX_BANK_SMALL )
			{
				if( cBankItem.sBankItemSmall[ nBankSmall ].GetID() == 0 && dwIndex < pUser->m_bItemMallCount )
				{
					memset( &Item, 0, sizeof(CItem) );
					Item.m_Serial	= g_pThis->GetNewSerial();
					Item.m_wItemID	= (WORD)pUser->m_pItemMallBox[ i ].dwItemID;
					CreateItem(&Item, (BYTE)pUser->m_pItemMallBox[ i ].dwCount);
					cBankItem.sBankItemSmall[nBankSmall] = Item;
					pUser->m_pItemMallSupply[dwIndex].dwItemMallIndex = pUser->m_pItemMallBox[ i ].dwIdx;
					dwIndex++;
					nBankSmall++;

					SetItemLog(&Item, ITEM_LOG_ITEM_MALL, 0, pUser->m_szLoginID, "CORUM");
					break;
				}

				nBankSmall++;
			}
		}
		else					// Large Item
		{
			while( nBankLarge < MAX_BANK_LARGE )
			{
				if( cBankItem.sBankItemLarge[ nBankLarge ].GetID() == 0 && dwIndex < pUser->m_bItemMallCount )
				{
					memset(&Item, 0, sizeof(CItem));
					Item.m_Serial	= g_pThis->GetNewSerial();
					Item.m_wItemID	= (WORD)pUser->m_pItemMallBox[ i ].dwItemID;
					CreateItem(&Item, (BYTE)pUser->m_pItemMallBox[ i ].dwCount);
					cBankItem.sBankItemLarge[nBankLarge] = Item;
					pUser->m_pItemMallSupply[dwIndex].dwItemMallIndex = pUser->m_pItemMallBox[ i ].dwIdx;
					dwIndex++;
					nBankLarge++;

					SetItemLog(&Item, ITEM_LOG_ITEM_MALL, 0, pUser->m_szLoginID, "CORUM");
					break;
				}

				nBankLarge++;
			}
		}
	}

	// 지급된 총갯수를 넣는다.
	pUser->m_wItemMallSupplyCount = (WORD)dwIndex;	

	// DB에 저장 
	pBindingBank = g_pDb->THCreateParamInfo(2);

	pBindingBank[0].obValue	 =	dwParamOffset;
	pBindingBank[0].cbMaxLen =	sizeof(cBankItem.sBankItemLarge);
	pBindingBank[0].wType	 =	DBTYPE_BYTES;
	dwParamOffset			+=	sizeof(cBankItem.sBankItemLarge);

	pBindingBank[1].obValue	 =	dwParamOffset;
	pBindingBank[1].cbMaxLen =	sizeof(cBankItem.sBankItemSmall);
	pBindingBank[1].wType	 =	DBTYPE_BYTES;
	dwParamOffset			+=	sizeof(cBankItem.sBankItemSmall);

	wsprintf(szQuery, "Update BankInfo Set Bank_large=?, Bank_small=? where [ID] = '%s'", pUser->m_szLoginID);
	g_pDb->THExecuteSQLByParam(szQuery, pBindingBank, &cBankItem, dwParamOffset, 2, TRUE, QUERY_TYPE_BANK_UPDATE, (void*)pUser->m_dwConnectionIndex, (BYTE)GAME_DB);
	
	
	return;

lb_ChrInfoQuery2:
	if(pUser->m_pItemMallBox)
	{
		delete [] pUser->m_pItemMallBox;
		pUser->m_pItemMallBox = NULL;		
	}

	QueryChrSelectInfo(pUser);
}


void QueryTypeBankUpdate(DBRECEIVEDATA* pResult)
{

	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;

	char	szQuery[QUERY_STRING_SIZE]	= {0,};
	int		i				= 0;

	if(pResult->nError < -1)
	{
		Log(LOG_IMPORTANT, "QueryTypeBankinfo Failed! (ID:%s)", pUser->m_szLoginID );
		pUser->m_bItemMallCount = 0xff;
		goto lb_ChrInfoQuery3;
	}

	// Item Mall Item 저장 성공 
	if(pResult->Query.execute_by_param.dwEffected > 0)
	{
		for(i = 0; i < pUser->m_wItemMallSupplyCount; i++)
		{
			if(pUser->m_pItemMallBox)
			{
				wsprintf(szQuery, "EXEC CZP_SET_ITEM_RECEIVE %d", pUser->m_pItemMallSupply[i].dwItemMallIndex);
				g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)MEMBER_DB);
			}			
		}		
	}

lb_ChrInfoQuery3:	//메모리 지우기전에 체크 : hwoarang 050218
	const BOOL bCanWriteMall = IsBadWritePtr( pUser->m_pItemMallBox, sizeof(ITEMMALL_BOX) );

	if(!bCanWriteMall && pUser->m_pItemMallBox)
	{
		delete [] pUser->m_pItemMallBox;
		pUser->m_pItemMallBox		= NULL;
	}

	const BOOL bCanWriteSupply = IsBadWritePtr( pUser->m_pItemMallSupply, sizeof( ITEMMALL_SUPPLY) );

	if(!bCanWriteSupply && pUser->m_pItemMallSupply)	
	{
		delete [] pUser->m_pItemMallSupply;
		pUser->m_pItemMallSupply	= NULL;
	}

	QueryChrSelectInfo(pUser);
}


void QueryChrSelectInfo(CUser* pUser)
{
	char szProc[500] = {0,};//이름이 20자 꽉 차거나 하면 넘침 : hwoarang 050218 
	wsprintf(szProc,	"Select Char_Index, [Name], ChrNum, Head, Class, Grade, Level, Exp, Ego, Str, Vit, Dex, Int, Honor,"
						" Luck, RecentOutMap, Equip, CurrentHand, GuildId, Rank, InvItemCheck, CreateTime from CharacterInfo where [ID] = '%s'", pUser->m_szLoginID);	

	g_pDb->THOpenRecord(szProc, QUERY_TYPE_CHR_SELECT_INFO, (void*)pUser->m_dwConnectionIndex, 4, (BYTE)GAME_DB);
}