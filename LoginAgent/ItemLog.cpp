#include "StdAfx.h"
#include "ItemLog.h"
#include "ItemManager.h"
#include "OwnServer.h"
#include "../CommonServer/Log.h"
#include "./../CommonServer/StringFunc.h"

extern DWORD	LOG_DB;
extern	IOleDBThread*	g_pLogDb;

BOOL SetItemLog( CItem* pItem, BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller )
{
	if( IS_ABLE_SERVICE_TYPE(ST_NO_LOG) )// : hwoarang 050201 
	{
		return FALSE;
	}
	
	if(!pItem)
		return FALSE;
	
	if( pItem->m_wItemID == 0 ) 
		return 0;
	
	if( pItem->m_wItemID == 25000 )	//카르츠는 여기서 거르자 
		return 0;			
	
	// 로그를 남기지 말아야할 아이템인지 확인한다.		//마일리지 아이템에는 필요가 없다.
//	if( g_pInvalidLogItemHash->bSameKey( pItem->m_wItemID ) ) 
//		return 0;
	
	ITEM_LOG	Log;
	
	memset( &Log, 0, sizeof( ITEM_LOG ) );
	
	Log.bLogCode	= bLogCode;
	Log.bServerSet	= (BYTE)g_pThis->GetServerSetNum();
	Log.dwDungeonID	= wDungeonID;
	memcpy( &Log.Serial, &pItem->m_Serial, sizeof( ITEM_SERIAL ) );
	memcpy( &Log.Item, pItem, sizeof( CItem ) );
	__lstrcpyn( Log.szOwnerName, szOwner, MAX_CHARACTER_NAME_LENGTH );
	if( szSeller )
		__lstrcpyn( Log.szSellerName, szSeller, MAX_CHARACTER_NAME_LENGTH );
	

	//DB Updates..
	char		szQuery[QUERY_STRING_SIZE]={0,};
	WORD		nColumn			= 7;	
	int			col				= 0;
	DWORD		dwParamOffset	= 0;
	DBBINDING*	pBinding		= g_pLogDb->THCreateParamInfo(nColumn);	
	
	if(!pBinding)
	{
		::Log(LOG_IMPORTANT, "ParameterPool is Full!!  in SetItemLog Function()!");
		return 0;
	}
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.Serial);
	pBinding[col++].wType	= DBTYPE_UI8;		
	dwParamOffset			+= sizeof(Log.Serial);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.bLogCode);
	pBinding[col++].wType	= DBTYPE_I1;		
	dwParamOffset			+= sizeof(Log.bLogCode);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.bServerSet);
	pBinding[col++].wType	= DBTYPE_I1;		
	dwParamOffset			+= sizeof(Log.bServerSet);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.dwDungeonID);
	pBinding[col++].wType	= DBTYPE_UI4;		
	dwParamOffset			+= sizeof(Log.dwDungeonID);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.Item);
	pBinding[col++].wType	= DBTYPE_BYTES;		
	dwParamOffset			+= sizeof(Log.Item);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.szOwnerName);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(Log.szOwnerName);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.szSellerName);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(Log.szSellerName);
	
	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	wsprintf( szQuery, "Insert into ItemLog%d_%d%02d%02d (Serial, LogCode, ServerSet, DungeonID, Item, OwnerName, SellerName) values( ?, ?, ?, ?, ?, ?, ? )",
		g_pThis->GetServerSetNum(), CurTime.wYear, CurTime.wMonth, CurTime.wDay);


	g_pLogDb->THExecuteSQLByParam( szQuery, pBinding, &Log.Serial, sizeof(Log), nColumn, FALSE, 0, NULL, LOG_DB);
	
	return TRUE;
}