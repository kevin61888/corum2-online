#include "StdAfx.h"
#include "MsgProc.h"
#include "ThrowQuery.h"
#include "OwnServer.h"
#include "UserTable.h"
#include "WorldServerTable.h"
#include "ServerFunction.h"
#include "recvmsg.h"
#include "Version.h"
#include "PacketEncrypt.h"
#include "Filter.h"
#include "User.h"
#include "ChinaBillingProc.h"

#include "CPSSOLib.h"
#include "../CommonServer/CommonHeader.h"

extern IOleDBThread*	g_pDb;
extern DWORD			g_dwVersion;
extern MACHINENAME		g_MachineName;

void CmdLoginNormal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTWS_LOGIN *			packet = (CTWS_LOGIN*)pMsg;						

	// 시그니쳐 체크.
	if(	packet->bySigniture[0] != '@' || packet->bySigniture[1] != 'S' || 
		packet->bySigniture[2] != 'A' || packet->bySigniture[3] != 'D' )
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//버젼이 낮음, ㅋㅋ
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	int nNationalCode = 0;	

	switch( MY_NATION_CODE )
	{
	case NC_JAPAN:
		{
			nNationalCode = National_Code_Japan;
		}break;
	case NC_TAIWAN:
		{
			nNationalCode = National_Code_Taiwan;
		}break;
	case NC_CHINA:
		{
			nNationalCode = National_Code_China;
		}break;
	case NC_KOREA:
	default:
		{
			nNationalCode = National_Code_Korea;
		}break;
	}

	if ( nNationalCode != packet->byNationCode )
	{ 
		// 다른 나라에서 접근할려고 든다.. 일단 버전이 낫다고 표시 ..
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//버젼이 낮음 
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	if(packet->dwVersion < g_dwVersion)
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//버젼이 낮음 
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}	

	// 최대 접속 유저수 초과 
	if(g_pUserTable->GetCount() >= g_pThis->GetMaxUserAllowedNum())		
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_USER_OVERFLOW;				//서버 최대 인원 초과...  
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		Log(LOG_NORMAL, "User Overflow ~ ~ ~ !!");
		return;
	}
	
	//유저 Data Alloc
	CUser* pUser = g_pUserTable->AllocNewUser();
	
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "User MemoryPool Overflow!");
		g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
		return;
	}
	
	pUser->SetConnectionIndex(dwConnectionIndex);

#ifdef __PACKET_ENCRYPTION
	// 일단 유저에게 기본키로 세팅.
	pUser->SetBaseEncryptKey();

	// 서버에서 이 녀석을 위한 키를 할당한다.
	WSTC_ENCRYPTION_KEY Packet;
	RandomKeyGenerator(Packet.byServerKey);
	
	// 클라이언트 키와 조합하여 실제 사용할 키를 생성한다.
	BYTE btEncryptionKey[ENCRYPT_KEY_LEN] = {0,};
	CalculateKey(packet->byClientKey, Packet.byServerKey, btEncryptionKey);

	// 클라이언트에게 패킷 암호 키를 전송!
	SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

	pUser->SetEncryptKey(btEncryptionKey);	
#endif 	

	WORD wPort = 0;
	// 우선 임시로 User 구조체에 ID 셋팅 
	__lstrcpyn(pUser->m_szLoginID, packet->szID, MAX_ID_REAL_LENGTH);				
	__lstrcpyn(pUser->m_szLoginPassword, packet->szPassword, MAX_PASSWORD_REAL_LENGTH);
	g_pNet->SetUserInfo(pUser->GetConnectionIndex(), (void*)pUser);	
	g_pNet->GetUserAddress(pUser->GetConnectionIndex(), (LPSTR)pUser->GetIPString(), &wPort);

	ThrowLoginQuery(pUser, packet->szID, packet->szPassword);
}

void CmdLoginNetMarble(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTWS_NETMARBLE_LOGIN *	packet = (CTWS_NETMARBLE_LOGIN *)pMsg;
	
	// 시그니쳐 체크.
	if(	packet->bySigniture[0] != '@' || packet->bySigniture[1] != 'S' || 
		packet->bySigniture[2] != 'A' || packet->bySigniture[3] != 'D' )
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//버젼이 낮음, ㅋㅋ
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	int nNationalCode = 0;	

	switch( MY_NATION_CODE )
	{
	case NC_JAPAN:
		{
			nNationalCode = National_Code_Japan;
		}break;
	case NC_TAIWAN:
		{
			nNationalCode = National_Code_Taiwan;
		}break;
	case NC_CHINA:
		{
			nNationalCode = National_Code_China;
		}break;
	case NC_KOREA:
	default:
		{
			nNationalCode = National_Code_Korea;
		}break;
	}

	if ( nNationalCode != packet->byNationCode )
	{ 
		// 다른 나라에서 접근할려고 든다.. 일단 버전이 낫다고 표시 ..
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//버젼이 낮음 
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	if(packet->dwVersion < g_dwVersion)
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_INVALID_VERSION;		//버젼이 낮음 
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}	

	// 최대 접속 유저수 초과 
	if(g_pUserTable->GetCount() >= g_pThis->GetMaxUserAllowedNum())		
	{
		WSTC_LOGIN_FAIL	packet;
		packet.bResult = LOGIN_FAIL_USER_OVERFLOW;				//서버 최대 인원 초과...  
		SendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		Log(LOG_NORMAL, "User Overflow ~ ~ ~ !!");
		return;
	}
	
	//유저 Data Alloc
	CUser* pUser = g_pUserTable->AllocNewUser();
	
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "User MemoryPool Overflow!");
		g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
		return;
	}
	
	pUser->SetConnectionIndex(dwConnectionIndex);

#ifdef __PACKET_ENCRYPTION
	// 일단 유저에게 기본키로 세팅.
	pUser->SetBaseEncryptKey();

	// 서버에서 이 녀석을 위한 키를 할당한다.
	WSTC_ENCRYPTION_KEY Packet;
	RandomKeyGenerator(Packet.byServerKey);
	
	// 클라이언트 키와 조합하여 실제 사용할 키를 생성한다.
	BYTE btEncryptionKey[ENCRYPT_KEY_LEN] = {0,};
	CalculateKey(packet->byClientKey, Packet.byServerKey, btEncryptionKey);

	// 클라이언트에게 패킷 암호 키를 전송!
	SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

	pUser->SetEncryptKey(btEncryptionKey);	
#endif 	

	if (strlen(packet->szID) != 0)
	{
		goto pass_login;
	}
	else
	{
		char szVal[1024] = {0,};
		if (TRUE == GetCPCookieValue(packet->szCPCookie,"UserID", szVal))
		{ 
			// 성공
			//memset(packet->szID, 0, sizeof(packet->szID));
			__lstrcpyn(packet->szID, szVal, MAX_ID_REAL_LENGTH);
		}
		else
		{
			// 실패
			Log(LOG_IMPORTANT, "GetCPCookieValue[UserID] failed");
			return;
		}

		// 패스워드 (유니큐 id를 얻는다.)
		if (TRUE == GetCPCookieValue(packet->szCPCookie,"UniID", szVal))
		{ 
			// 성공
			//memset(packet->szPassword, 0, sizeof(packet->szPassword));
			__lstrcpyn(packet->szPassword, szVal, MAX_PASSWORD_REAL_LENGTH);
		}
		else
		{
			// 실패	
			Log(LOG_IMPORTANT, "GetCPCookieValue[UniID] failed");
			return;
		}
		
		int iHex;
		char buf[1024] = {0,};
		char buf2[1024] = {0,};

		__lstrcpyn(buf, packet->szPassword+2, MAX_PASSWORD_REAL_LENGTH-2);
		sscanf(buf,"%d", &iHex);
		itoa(iHex, buf, 16);
		iHex = atoi(buf);
		wsprintf(buf2, "NM%010s", buf);
		__lstrcpyn(packet->szPassword, buf2, MAX_PASSWORD_REAL_LENGTH);
	}

pass_login:

	WORD wPort = 0;
	// 우선 임시로 User 구조체에 ID 셋팅 
	__lstrcpyn(pUser->m_szLoginID, packet->szID, MAX_ID_REAL_LENGTH);				
	__lstrcpyn(pUser->m_szLoginPassword, packet->szPassword, MAX_PASSWORD_REAL_LENGTH);
	g_pNet->SetUserInfo(pUser->GetConnectionIndex(), (void*)pUser);	
	g_pNet->GetUserAddress(pUser->GetConnectionIndex(), (LPSTR)pUser->GetIPString(), &wPort);

	ThrowLoginQuery(pUser, packet->szID, packet->szPassword);
}

void CmdLogin(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	if( IS_ABLE_SERVICE_TYPE( ST_NETMARBLE) )
	{
		CmdLoginNetMarble(dwConnectionIndex, pMsg,dwLength);
	}
	else
	{
		CmdLoginNormal(dwConnectionIndex, pMsg,dwLength);
	}
}

void CmdCheckMonitoring(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	pMsg;dwLength;

	if (NULL == pUser)
	{
		Log(LOG_JUST_DISPLAY, "pUser is NULL at CmdCheckMonitoring function.. ");
		return;
	}	

	// 아이피 얻어내고,
	char szIp[16]	= {0,};	
	WORD wPort		= 0;
	g_pNet->GetUserAddress(pUser->GetConnectionIndex(), szIp, &wPort);

	// 해당 아이피가 등록 되어 있는지 체크 한다.,
	char szQuery[QUERY_STRING_SIZE]={0,};
	memset(szQuery, 0, QUERY_STRING_SIZE);
	wsprintf(szQuery, "Select * From MONITORING_LIST where ip = '%s'", szIp);
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_CHECK_MONITOR, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)TOTAL_DB);	
}


void CmdCMKickUser(CUser* pUser, char* pMsg,DWORD dwLength)
{
	dwLength;

	if( pUser && TRUE == pUser->m_bIsMonitor )
	{
		CTAS_KICK_USER* pRecievePacket = (CTAS_KICK_USER*)pMsg;
		CUser* pPrevUser = g_pUserTable->GetUserInfo(pRecievePacket->dwPropID);

		if( NULL == pPrevUser )
		{
			return;
		}

		if(pPrevUser->m_dwChar_Index)
		{
			// 다른 월드나 던전 서버에 유저가 있다면 모두 종료시켜 버려라 
			ASTWS_KICK_USER	KickPacket;
			KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
			KickPacket.btKickCode = 0;
			g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
		}
		
		// Agent에 접속되어 있는 유저 
		if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	
		{
			DWORD dwConnectionIndex = pPrevUser->GetConnectionIndex();
			OnDisconnectUser(dwConnectionIndex);
			g_pNet->SetUserInfo(dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);			
		}
		else
		{
			ThrowLogoutQuery(pPrevUser);

			Log(LOG_JUST_DISPLAY, "Monitoring Tool User Kick...(ID:%s, Name:%s)",pPrevUser->m_szLoginID, pPrevUser->GetSelectedCharacter()->szName);

			// 유저 테이블에서 이 유저 삭제 Free도 함 
			g_pUserTable->Remove(pPrevUser); 
		}
	}
}

void CmdCMToolNotice(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	if( pUser && TRUE == pUser->m_bIsMonitor )
	{
		CTAS_CMTOOL_NOTICE* pRecievePacket = (CTAS_CMTOOL_NOTICE*) pMsg;
		ASTWS_CMTOOL_NOTICE NoticePacket;

		__lstrcpyn(NoticePacket.szMsg, pRecievePacket->szMsg, 1024);	
		NoticePacket.wMsgLen = pRecievePacket->wMsgLen;

		g_pServerTable->BroadCastWorldServer((char*)&NoticePacket,NoticePacket.GetPacketSize());
	}
}

void CmdIsAliveUser(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	if( pUser && TRUE == pUser->m_bIsMonitor )
	{
		CTAS_IS_ALIVE_USER* pRecievePacket = (CTAS_IS_ALIVE_USER*) pMsg;
		
		char szQuery[QUERY_STRING_SIZE] = {0,};

		wsprintf(szQuery, "Select * From LoginTable Where user_id = '%s' AND kind = '%d'", pRecievePacket->szID, pRecievePacket->Kind);

		// 최종 처리는 RecieveDB 에서.
		g_pDb->THOpenRecord(szQuery, QUERY_TYPE_IS_ALIVE_USER, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)MEMBER_DB);
		
	}
}

void CmdRequestServerAlive(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength; pMsg;

	if (pUser && TRUE == pUser->m_bIsMonitor)
	{
		// 붙어 있는 월드 서버들에게 정보를 보내달라 요청.
		ASTWS_REQUEST_SERVER_ALIVE RequestServerAlivePacket;
		g_pServerTable->BroadCastWorldServer((char*)&RequestServerAlivePacket, RequestServerAlivePacket.GetPacketSize());		

		// 전병렬
		// 유저에게 현재 서버의 정보를 보냄.
		ASTC_SERVER_ALIVE_INFO		SendPacket;

		SendPacket.wServerType			= g_pThis->GetServerType();
		SendPacket.dwServerID			= g_pThis->GetServerID();
		SendPacket.wConnectedUserCount	= g_pUserTable->GetCount();
		SendPacket.dwExpGainRate		= 0;
		SendPacket.dwMagicItemDropRate	= 0;

		g_pUserTable->BroadCastToMonitoringTool((char*)&SendPacket, SendPacket.GetPacketSize());
	}
}


// pUser를 Alloc 받은 상태에서 또 한번 Login 메세지가 들어온 경우 
void CmdLoginOverlapped(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	CTWS_LOGIN*	pPacket = (CTWS_LOGIN*)pMsg;

	WSTC_LOGIN_FAIL	packet;
	packet.bResult = LOGIN_FAIL_NOW_TRY_TO_LOGIN;
	SendToUser( pUser, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

	Log(LOG_NORMAL, "Login Message Overlapped!! (pUser->m_szLoginID:%s, pPacket->szLoginID:%s)", pUser->m_szLoginID, pPacket->szID);
}

void CmdCharacterSelect(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	CTWS_CHARACTER_SELECT* pPacket = (CTWS_CHARACTER_SELECT*)pMsg;

	if(pPacket->bCharacterIndex > 3)
	{
		Log(LOG_IMPORTANT, "Invalid CharacterIndex Entered! (Login ID:%s)", pUser->m_szLoginID);
		ASTC_CHARACTER_SELECT_FAIL FailPacket;
		FailPacket.bErrorCode = 1;
		SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	if(pUser->m_dwStatus == USER_STATUS_CHARACTER_SELECTED)	
	{
		Log(LOG_NORMAL, "Character is already selected!");
		return;	
	}
	
	if(pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwRecentWorldmapID == 0)
	{
		pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwRecentWorldmapID = 1;
	}	

	// 혹시 이 캐릭터가 다른 서버에 남아있을경우를 대비하여 모두 종료시켜 버려라~
	ASTWS_KICK_USER	KickPacket;
	KickPacket.dwCharIndex = pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwCharacterIndex;
	g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
	
	if( pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwRecentWorldmapID )
	{
		LPWorldServerData pServer = g_pServerTable->GetServer( 1 );
		
		if(!pServer)
		{
			Log(LOG_IMPORTANT, "WorldServer Data is Not Found or Invalid WorldmapID Entered! (WorldMapID=%d)"
				, pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwRecentWorldmapID );
			
			ASTC_CHARACTER_SELECT_FAIL FailPacket;
			FailPacket.bErrorCode = 2;
			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		// 해당 월드서버가 접속되어 있지 않음 
		if(!pServer->dwConnectionIndex)		
		{
			Log(LOG_IMPORTANT, "WorldServer is Not Attached! (WorldMapID=%d)"
				, pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwRecentWorldmapID );

			ASTC_CHARACTER_SELECT_FAIL FailPacket;
			FailPacket.bErrorCode = 3;
			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
		
		pUser->m_bSelectedChrIndex	= pPacket->bCharacterIndex;	// 선택한 배열 Index 저장
		pUser->m_dwChar_Index		= pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwCharacterIndex;
		
		if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 Test : 050104 hwoarang
		{
			if(pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwLevel<10)
			{
				pUser->m_sChinaBillingData.dwChinaBillingPoint		= 0;
				pUser->m_sChinaBillingData.eChinaBillingUserData	= china_billing_info_limit;
			}
			else
			{
				SendChinaBillingLogin(pUser->GetPropID(), pUser->GetUserID(), pUser->GetIP());
			}
		}

		// World 서버 접속 허용 리스트에 캐릭터 추가 시키고 
		ASTWS_ADD_NEW_USER packet;
		packet.dwPropID		= pUser->GetPropID();
		packet.dwChar_Index = pUser->m_dwChar_Index = pUser->m_EnumChar[ pPacket->bCharacterIndex ].dwCharacterIndex;	//DB상의 Character Index번호 
		packet.dwSerialCode = pUser->m_dwSerialCode = g_pThis->GetNewSerial( pPacket->bCharacterIndex );
		packet.bBillingType = pUser->m_bBillingType;
		packet.BillingEday  = pUser->m_BillingEday;
		packet.dwRemainTime = pUser->m_dwRemainTime;

		if( IS_ABLE_NATION(NC_TAIWAN) )
		{
			if( IS_ABLE_SERVICE_TYPE(ST_BILLING) )//일반빌링 :050104 hwoarang
			{
				packet.BillingInfo = pUser->m_BillingInfo;	//결제 정보 
			}
		} // if( IS_ABLE_NATION(NC_TAIWAN) )

		g_pNet->SendToServer(pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

		// 캐릭터를 선택하고 월드서버의 Accept허용을 대기하고 있는상태
		pUser->m_dwStatus = USER_STATUS_CHARACTER_SELECTED; 
	}
	else
	{
		ASTC_CHARACTER_SELECT_FAIL FailPacket;
		FailPacket.bErrorCode = 4;
		SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		Log(LOG_IMPORTANT, "Character WorldMapID is not found! (Name:%s)"
			, pUser->m_EnumChar[ pPacket->bCharacterIndex ].szName );
	}
}

void CmdCreateNewCharacter(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	CTWS_CREATE_NEW_CHARACTER* pPacket = (CTWS_CREATE_NEW_CHARACTER*)pMsg;

	if(pPacket->bChrNum > 3)
	{
		Log(LOG_IMPORTANT, "Invalid ChrNum Entered! at CmdCreateNewCharacter() bChrNum=%d", pPacket->bChrNum);
		return;
	}
	
	if(lstrlen(pUser->m_EnumChar[ pPacket->bChrNum ].szName))
	{
		Log(LOG_IMPORTANT, "Character Place Overlapped! at CmdCreateNewCharacter() bChrNum=%d", pPacket->bChrNum);
		return;
	}

	switch( MY_NATION_CODE )
	{
	case NC_CHINA:
		{
			if(FilterWordsIncluded(pPacket->szCharacterName)) 
			{
				Log(LOG_IMPORTANT, "Prohibited Character Name! at CmdCreateNewCharacter() Requested Name: %s", pPacket->szCharacterName);

				WSTC_CREATE_CHARACTER_FAIL FailPacket;
				FailPacket.nResultValue = -3;

				SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				return;
			} // if(FilterWordsIncluded(pPacket->szCharacterName))
		}break;
	}

	// 우선 임시로 저장시켜놓고 
	pUser->m_bSelectedChrIndex = pPacket->bChrNum;
	pUser->m_bClass = pPacket->bClass;
	pUser->m_EnumChar[ pPacket->bChrNum].dwRecentWorldmapID = 1;
	lstrcpyn(pUser->m_EnumChar[ pPacket->bChrNum ].szName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH+1);
	pUser->m_bTryCreateCharSlot = pPacket->bChrNum;

	ThrowInsertCharQuery(pUser, pPacket->szCharacterName,(BYTE)(pPacket->bChrNum + 1),(WORD)(pPacket->wHead + 1000), pPacket->bClass, pPacket->wStartMapID);
}

void CmdDeleteCharacter(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength;

	CTWS_DELETE_CHARACTER* pPacket = (CTWS_DELETE_CHARACTER*)pMsg;
	
	char szJumin[8] = {0,};	
	lstrcpyn(szJumin, pPacket->szJumin, 7+1);	szJumin[7] = '\0';

	if(IS_ABLE_NATION(NC_KOREA))//주민등록 번호가 맞는지 체크  :050104 hwoarang
	{	
		char szQuery[ QUERY_STRING_SIZE ]={0,};
		wsprintf(szQuery, "CZP_JUMIN_CHECK '%s', '%s', '%s'", pUser->m_szLoginID, pPacket->szCharacterName, szJumin);
		g_pDb->THOpenRecord(szQuery, QUERY_TYPE_JUMIN_CHECK, (void*)pUser->m_dwConnectionIndex, 1, MEMBER_DB);
	} // if(IS_ABLE_NATION(NC_KOREA))
	else
	{
		// 생성 날짜 체크 //
		if(	pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.year==0 || 
			pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.month==0 || 
			pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.day==0)
		{
			ThrowDeleteCharQuery(pUser, pPacket->szCharacterName);
		}
		else
		{		
			CTime cCreateTime(pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.year, 
				pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.month, 
				pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.day, 
				pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.hour, 
				pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.minute, 
				pUser->m_EnumChar[pPacket->bChrNum].sCreateTime.second);
			
			CTime cCurrentTime = CTime::GetCurrentTime();

			CTimeSpan ts = cCurrentTime - cCreateTime;

			if(ts.GetTotalHours()>=24)
			{
				if(pUser->m_EnumChar[pPacket->bChrNum].dwGuildId!=0)
				{
					// WorldServer Message //
					ASTWS_GUILD_CHARACTER_DELETE pSendServerPacket;
					memset(pSendServerPacket.szCharacterName, 0, sizeof(pSendServerPacket.szCharacterName));
					memcpy(pSendServerPacket.szCharacterName, pUser->m_EnumChar[pPacket->bChrNum].szName, MAX_CHARACTER_NAME_REAL_LENGTH);
					pSendServerPacket.dwGuildId	= pUser->m_EnumChar[pPacket->bChrNum].dwGuildId;
					pSendServerPacket.byRank	= pUser->m_EnumChar[pPacket->bChrNum].byRank;
					pSendServerPacket.dwIndex	= pUser->m_EnumChar[pPacket->bChrNum].dwCharacterIndex;		
					g_pServerTable->SendToWorldServer((char*)&pSendServerPacket, pSendServerPacket.GetPacketSize(), 1);
				}
				ThrowDeleteCharQuery(pUser, pPacket->szCharacterName);
			}
			else
			{
				WSTC_DELETE_CHARACTER_RESULT pPacket;
				pPacket.bSuccess		= FALSE;
				pPacket.nResultValue	= 10;
				SendToUser(pUser, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			}
		}
	}
}

void CmdCheckGM(CUser* pUser ,char* pMsg,DWORD dwLength)
{
	dwLength; pMsg; pUser;
/*
	CTWS_CHECK_GM* pPacket = (CTWS_CHECK_GM*)pMsg;

	char szQuery[ 0xff ];
	wsprintf(szQuery, "Select [Ip] from GM_LIST where [Name]= '%s'", pUser->m_szCharacterName);
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_GM_CHECK, pUser, 1, TOTAL_DB);	
*/
}


