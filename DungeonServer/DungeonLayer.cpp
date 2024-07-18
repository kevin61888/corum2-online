#include "stdafx.h"
#include <math.h>
#include <crtdbg.h>
#include "DungeonLayer.h"
#include "OwnServer.h"
#include "Section.h"
#include "Dungeon.h"
#include "DungeonTable.h"
#include "MonsterTable.h"
#include "MonsterManager.h"
#include "effect.h"
#include "recvmsg.h"
#include "Map.h"
#include "LayerFormation.h"
#include "PathFinding\\SearchModule_Astar.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "Monster.h"
#include "DUNGEON_DATA_EX.h"
#include "user.h"
#include "DBProc.h"
#include "GameSystemNative.h"
#include "EventDungeonManager.h"
#include "../CommonServer/CommonHeader.h"

#include ".\tm_killingfield.h"

//-----------------------------------------------------------------------------
// 던전의 한층(Layer)을 생성한다(일반 던전서버일 경우) 
//	
//※ 주의 : 일반던전일 경우에만 서버 초기 실행시 실행된다. 서버 돌아가고 있는 도중
//			사용하지 말자..    외부 파일 읽어드리고 new로 메모리 할당해서 느림.ㅡㅡ 
//			초보자 던전의 경우 COwnServer::SpawnNewSession()을 통해 메모리풀로 할당한다.
//-----------------------------------------------------------------------------
void CDungeonLayer::Create(DWORD dwID, BYTE bLayer, CDungeon* pParent)
{
	memset(this, 0, sizeof(CDungeonLayer));	
	m_pMap = new CMap;
	m_pLayerFormation = new LAYER_FORMATION;
	
	m_bLayer	= bLayer;		
	m_pParent	= pParent;
	
	m_pTeamMatch = NULL;//팀매치 초기화 : hwoarang 050202

	InitList();
	
	//속성파일, 섹션, 몬스터 Spawn 지점등의 정보 읽어드림 By *.ttb 파일 
	m_pMap->Create(dwID, this);
	
	m_dwLastFrameSec		= 0;
	m_dwLastOwnerFrameSec	= 0;
	
	// 대전 초기값.
	InitMatch();
	
	// PathFinder
	m_pSw = new Sw;
	m_pSw->Initialize( 20, 20, m_pMap->m_dwTileNumWidth, m_pMap->m_dwTileNumHeight, sizeof(MAP_TILE), (PVOID)&( m_pMap->GetTile(0, 0)->wAttr ) );
	
	// Make Layer Formation.
	char szQuery[ 0xff ]={0,};
	wsprintf(szQuery, "Select DungeonID, Layer, MonsterFormation, MonsterKind from LayerFormation where DungeonID=%d and Layer=%d", m_pParent->GetID(), bLayer );
	int nRet = 	g_pDb->OpenRecord(szQuery, m_pLayerFormation, 1, GAME_DB);

	if(nRet < 0) Log(LOG_IMPORTANT, "Make Layer Formation Failed! Dungeon %d, Layer %d", m_pParent->GetID(), bLayer );
	
	CreateMonsterAll();

	//길드전 레이어에 팀 매치 생성: hwoarang 050202
	DUNGEON_DATA_EX* pDungeonData = pParent->GetDungeonDataEx();
	if(pDungeonData)
	{
		if( pDungeonData->IsStadium() )
		{//Layer
			FLAG_TYPE match_type= M_T_KILLING_FIELD;
			CreateTeamMatch( match_type );
			return;
		}
	}
	else
	{
		assert( NULL && "!!! Created Layer Can't Get DungeonData");
	}
}

bool CDungeonLayer::CreateTeamMatch(const FLAG_TYPE dwMatchType)
{
	switch (dwMatchType & M_T_CMP_BASE)
	{
	case M_T_DEAH_MATCH:
	case M_T_KING_OF_THE_HILL:
	case M_T_CAPTURE_THE_FLAG:
	case M_T_LEADER_KILL:
		{
			m_pTeamMatch = NULL;
		}break;
	case M_T_KILLING_FIELD:
		{
			m_pTeamMatch = new CTM_KillingField( this->m_pParent->GetID(), this->m_bLayer);
		}break;
	} // switch (dwMatchType & M_T_CMP_BASE)

	return false;
}


//-----------------------------------------------------------------------------
// 던전의 한층(Layer)을 해제한다.(일반 던전서버일 경우) 
//	
//※ 주의 : 일반던전일 경우에만 서버 종료시 실행된다.
//			초보자 던전서버일경우 이리로 들어오면 큰일난다. ㅡㅡ;
//-----------------------------------------------------------------------------
void CDungeonLayer::Destroy()
{
	for(int i=0; i<m_wTotalSectionNum; i++)
	{
		if( m_pSection[i]->m_pItemHash )
		{
			m_pSection[i]->m_pItemHash->Destroy();
			delete m_pSection[i]->m_pItemHash;
			m_pSection[i]->m_pItemHash = NULL;
		}

		delete m_pSection[i];
		m_pSection[i] = NULL;
	}
	
	if( m_pItemHash )
	{
		m_pItemHash->Destroy();
		delete m_pItemHash;
		m_pItemHash = NULL;
	}
	
	m_pMap->Destroy();

	if( m_pMoveSpot )
	{
		delete[] m_pMoveSpot;
		m_pMoveSpot = NULL;
	}
	
	if( m_pStartSpot )
	{
		delete [] m_pStartSpot;
		m_pStartSpot = NULL;
	}
	
	if( m_pSw )
	{
		delete m_pSw;
		m_pSw = NULL;
	}

	if (m_pPcList)
	{
		m_pPcList->RemoveAll();
		delete m_pPcList;
		m_pPcList = NULL;
	}

	if (m_pMonsterList)
	{
		m_pMonsterList->RemoveAll();
		delete m_pMonsterList;
		m_pMonsterList	= NULL;
	}

	if (m_pResponseMonsterList)
	{
		m_pResponseMonsterList->RemoveAll();
		delete m_pResponseMonsterList;
		m_pResponseMonsterList = NULL;
	}

	if (m_pCreateItemList)
	{
		m_pCreateItemList->RemoveAll();
		delete m_pCreateItemList;
		m_pCreateItemList = NULL;
	}

	if (m_pActiveSectionList)
	{
		m_pActiveSectionList->RemoveAll();
		delete m_pActiveSectionList;
		m_pActiveSectionList = NULL;
	}

	if (m_pDeadMonsterList)
	{
		m_pDeadMonsterList->RemoveAll();
		delete m_pDeadMonsterList;
		m_pDeadMonsterList = NULL;
	}

	if (m_pMap)
	{
		delete m_pMap;
		m_pMap = NULL;
	}

	if (m_pLayerFormation)
	{
		delete m_pLayerFormation;
		m_pLayerFormation = NULL;
	}	
}

DWORD CDungeonLayer::GetUserCount()	
{	
	return m_pPcList->GetCount();	
}

void CDungeonLayer::InitMatch()
{
	m_bMatching					= FALSE;
	m_bStartMatch				= FALSE;
	m_dwMatchBeginTime			= 0;
	m_dwMatchTick				= MATCH_TIME;		// 5분 동안(300초)
	m_dwMatchEndTime			= 0;
	m_bReadyMatchNum			= 0;
	m_dwEndMatchJudgmentTick	= 0;
	m_dw10SecEndMatchLayerTick	= 0;
}

BOOL CDungeonLayer::SetSection(int nSectionNum, CSection* pSection)
{
	if(nSectionNum < 1 || !pSection)
	{
		__asm int 3
		return FALSE;
	}
	
	// 아이템 해쉬를 설정해준다.
	pSection->m_pItemHash = new ITEM_HASH;
	pSection->m_pItemHash->InitializeHashTable( 5000, 5000 );

	m_pSection[ nSectionNum-1 ] = pSection;
	m_pSection[ nSectionNum-1 ]->m_pActiveListPos	= NULL;
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// 던전의 한층(Layer)의 모든 유저에게 메세지를 Broadcast 함 
//-----------------------------------------------------------------------------
DWORD CDungeonLayer::BroadCastMsg( char* pMsg, DWORD wLength ) const
{
	DWORD dwCounter = 0;

	CUser* pUser=0;
	POSITION_ pos = m_pPcList->GetHeadPosition();
	
	while(pos)
	{
		 pUser = (CUser*)m_pPcList->GetNext(pos);
	     NetSendToUser(pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION);
		 ++dwCounter;
	}

	return dwCounter;
}

//-----------------------------------------------------------------------------
// 던전의 한층(Layer)의 Sender를 제외한 모든 유저에게 메세지를 Broadcast  
//-----------------------------------------------------------------------------
DWORD CDungeonLayer::BroadCastMsgExceptSender( char* pMsg, DWORD wLength, const CUser* pSender ) const
{
	DWORD dwCounter = 0;

	CUser* pUser=0;
	POSITION_ pos = m_pPcList->GetHeadPosition();
	
	while(pos)
	{
		 pUser = (CUser*)m_pPcList->GetNext(pos);
		 if(pUser == pSender) continue;
	     NetSendToUser(pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION);
		 ++dwCounter;
	}

	return dwCounter;
}

//-----------------------------------------------------------------------------
// 센더의 섹션과 그섹션의 링크 섹션에 속해있는 유저에게 메시지를 보낸다.  센더 제외
//-----------------------------------------------------------------------------
DWORD CDungeonLayer::BroadCastSectionMsg( char* pMsg, DWORD wLength, const CUser* pSender ) const
{
	DWORD dwCounter = 0;

	CSection*	pSection = GetSection( pSender->GetPrevSectionNum() );
	if( !pSection ) return 0;	

	// 센더의 섹션의 유저에게 메시지전달.
	CUser*		pUser	= NULL;
	POSITION_	pos		= pSection->m_pPcList->GetHeadPosition();

	while(pos)
	{
		pUser = (CUser*)pSection->m_pPcList->GetNext(pos);
		if(pUser == pSender ) continue;
		NetSendToUser( pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION );
		++dwCounter;
	}

	// 센더의 섹션의 링크된 섹션의 유저에게 메시지 전달.
	CSection* pLinkSection = NULL;

	for( int i=0; pSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetSection(pSection->m_wLink[i]);
		if( !pLinkSection ) continue;

		pos = pLinkSection->m_pPcList->GetHeadPosition();
		while(pos)
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			NetSendToUser( pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION );
			++dwCounter;
		}
	}
	
	return dwCounter;
}

//-----------------------------------------------------------------------------
// 섹션과 그섹션의 링크 섹션에 속해있는 유저에게 메시지를 보낸다.  
//-----------------------------------------------------------------------------
DWORD CDungeonLayer::BroadCastSectionMsg( char* pMsg, DWORD wLength, BYTE bSectionNum ) const
{
	DWORD dwCounter = 0;

	// 현재 송신자가 속해있는 섹션의 번호.
	CSection*	pSection = GetSection( bSectionNum );		
	if( !pSection ) return dwCounter;	

	// 센더의 섹션의 유저에게 메시지전달.
	CUser*		pUser	= NULL;
	POSITION_	pos = pSection->m_pPcList->GetHeadPosition();
	while(pos)
	{
		pUser = (CUser*)pSection->m_pPcList->GetNext(pos);
		NetSendToUser( pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION );
		++dwCounter;
	}

	// 센더의 섹션의 링크된 섹션의 유저에게 메시지 전달.
	CSection* pLinkSection = NULL;

	for( int i=0; pSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetSection(pSection->m_wLink[i]);
		if( !pLinkSection ) continue;

		pos = pLinkSection->m_pPcList->GetHeadPosition();
		while(pos)
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			NetSendToUser( pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION );
			++dwCounter;
		}
	}
	
	return dwCounter;
}

//-----------------------------------------------------------------------------
// 인자로 들어온 섹션에 메시지를 보낸다 링크 섹션에는 보내지 않는다. 링크 섹션을 포함하지 않고 메시지를 보낸다. 
//-----------------------------------------------------------------------------
DWORD CDungeonLayer::BroadCastSectionMsgExceptLink( char* pMsg, DWORD wLength, BYTE bSectionNum ) const
{
	DWORD dwCounter = 0;

	CSection*	pSection = GetSection( bSectionNum );
	if( !pSection ) return dwCounter;

	CUser*		pUser	= NULL;
	POSITION_ pos = pSection->m_pPcList->GetHeadPosition();
	while( pos )
	{
		pUser = (CUser*)pSection->m_pPcList->GetNext(pos);
		NetSendToUser( pUser->m_dwConnectionIndex, pMsg, wLength, FLAG_SEND_NOT_ENCRYPTION );
		++dwCounter;
	}

	return dwCounter;
}

DWORD CDungeonLayer::BroadCastSectionMsgExceptLink( char* pMsg, DWORD wLength, CUser* pSender ) const
{
	pMsg;wLength;pSender;

	// 아직 쓸일이 없을꺼 같아 코딩 안한다.. ㅋ ㅋ ㅋ 
	return 0;
}


//-----------------------------------------------------------------------------
// 차집합을 만들어 그 섹션들에 속하는 유저에게 메시지를 보낸다. 섹션의 차집합을 구해 리턴한다.
//-----------------------------------------------------------------------------
void CDungeonLayer::IntersectSection( BYTE bPrevSectionNum, BYTE bCurSectionNum, BYTE* pIntersectSection )
{
	if( !pIntersectSection ) return;

	CSection*	pPrevSection	= GetSection( bPrevSectionNum );
	CSection*	pCurSection		= GetSection( bCurSectionNum );	
	
	if( !pPrevSection ) return;
	if( !pCurSection ) return;

	memset( pIntersectSection, 0, sizeof(BYTE) * MAX_SECTION_LINK_NUM ); 

	int			i=0, j=0, cnt=0;
	BOOL		bInter=0;
	
	// 차집합 만들기.
	for( i=0; pCurSection->m_wLink[i] != 0; i++ )
	{
		if( pCurSection->m_wLink[i] == bPrevSectionNum ) continue;
		
		bInter = TRUE;

		for( j=0; pPrevSection->m_wLink[j] != 0; j++ )
		{
			if( pPrevSection->m_wLink[j] == pCurSection->m_wLink[i] )	// 여기에 걸렸다면 제외시킨다.
			{
				bInter = FALSE;
				break;
			}
		}

		if( bInter )		// 공통적인 섹션.
		{
			pIntersectSection[cnt] = (BYTE)pCurSection->m_wLink[i];
			++cnt;
		}
	}
}


// 유저가 섹션이 바뀌었을경우 이함수로 다른유저간에 정보를 교환한다.
void CDungeonLayer::BroadCastIntersectSectionInfo( BYTE bPrevSectionNum, BYTE bCurSectionNum, CUser* pOwnUser )
{
	if( !pOwnUser ) return;
	
	int		i=0;
	BYTE	pbIntersectSection[MAX_SECTION_LINK_NUM]={0,};		// 차집합 섹션을 저장할 배열.
	
	IntersectSection( bPrevSectionNum, bCurSectionNum, pbIntersectSection );

	for( i=0; pbIntersectSection[i] != 0; i++ )
	{
		SendSectionInfo( pbIntersectSection[i], pOwnUser );
	}
}

void CDungeonLayer::SendSectionInfo( BYTE bSectionNum, CUser* pOwnUser, BYTE bPortal )
{
	CDungeonLayer* pLayer = pOwnUser->GetCurDungeonLayer();
	if( !pLayer ) return;
	
	CSection* pSection = pLayer->GetSection( bSectionNum );
	if( !pSection ) return;

	// pOwnUser의 정보를 작성한다.
	DSTC_APPEAR	AppearMe;	
	MakeAppearUser(&AppearMe, pOwnUser, bPortal);

	DSTC_APPEAR			AppearUser;

	CUser*						pUser=0;
	ITEM_SERVER*				pItem=0;
	CMonster*					pMonster=0;	
	POSITION_					pos=0;
	ListNode<ITEM_SERVER>*		pItemNode=0;

	pos = pSection->m_pPcList->GetHeadPosition();

	POSITION_ del=0;
	POSITION_ posSkill = NULL;

	while( pos )
	{
		del = pos;
		pUser = (CUser*)pSection->m_pPcList->GetNext( pos );
		
		if (!pUser->GetID()  || !pUser->GetClass() || !pUser->GetHeadModelNo())
		{
			pSection->m_pPcList->RemoveAt(del);
			continue;
		}

		MakeAppearUser(&AppearUser, pUser, 0);
		
		// 기존에 있던놈들에게 나를 보내고, 나는 그놈들을 받아온다.
		if(	pUser->GetID() != pOwnUser->GetID() )
		{
			// 조인한넘을 받을 다른유저들 	
			if( !(AppearMe.dwGuildWarFlag & G_W_F_OBSERVER) )//본인이 옵저버가 아니면 보내준다.
			{
				NetSendToUser( pUser->m_dwConnectionIndex, (char*)&AppearMe, AppearMe.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );	
			}

			// 조인한넘 
			NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&AppearUser, AppearUser.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION  );	

			if (pUser->GetPlayerShopInfo()->m_bPlayerShopMode)
			{
				// 조인하는놈에게 기존에 있던 놈이 노점모드라면 노점모드라고 알려줘야지.
				DSTC_PLAYERSHOP_OPEN packet;
				memset(packet.szTitle, 0, sizeof(packet.szTitle));
				memcpy(packet.szTitle, pUser->GetPlayerShopInfo()->szTitle, sizeof(packet.szTitle));
				packet.dwUserIndex = pUser->GetID();
				NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );		
			}
			
			if(pUser->m_byRest==1)
			{
				DSTC_PLAYER_REST pSendPacket;
				pSendPacket.byType		= pUser->m_byRest;
				pSendPacket.dwUserIndex	= pUser->GetID();
				NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );		
			}
			
			if (pUser->m_sPKDescInfo.m_bBad)
			{
				DSTC_PLAYER_BAD pSendPacket;
				pSendPacket.byBad		= pUser->m_sPKDescInfo.m_bBad;
				pSendPacket.dwUserIndex	= pUser->GetID();
				NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			}			
			
			// 기존에 있떤 유저 상태 스킬을 다시 보내줘라.
			if (pUser->GetUsingStatusEffectList())
			{
				posSkill = pUser->GetUsingStatusEffectList()->GetHeadPosition();
				while(posSkill)
				{					
					EffectDesc* pEffectDesc = (EffectDesc*)pUser->GetUsingStatusEffectList()->GetNext(posSkill);
					g_pEffectLayer->SendSkillStatusUserUser(pOwnUser, pUser, pEffectDesc->pEffect->bID, pEffectDesc->bSkillLevel, pEffectDesc->dwDestTick-g_dwTickCount);
				}
			}
			
			pUser->SendToUserConsumableItemEffectListAll(pOwnUser);
		}
		
		posSkill = pOwnUser->GetUsingStatusEffectList()->GetHeadPosition();
		while(posSkill)
		{
			// 자기껄 남에게 			
			EffectDesc* pEffectDesc = (EffectDesc*)pOwnUser->GetUsingStatusEffectList()->GetNext(posSkill);
			g_pEffectLayer->SendSkillStatusUserUser(pUser, pOwnUser, pEffectDesc->pEffect->bID, pEffectDesc->bSkillLevel, pEffectDesc->dwDestTick-g_dwTickCount);	
		}		
	}

	// 몬스터 정보를 받아야한다.
	DSTC_APPEAR_MON		AppearMon;	
	DSTC_MOVE_MON		MoveMon;

	pos = pSection->m_pMonsterList->GetHeadPosition();	
	while( pos )
	{
		del = pos;
		pMonster = (CMonster*)pSection->m_pMonsterList->GetNext( pos );

		if (pMonster->GetUnitStatus() == UNIT_STATUS_DEAD)
		{			
			continue;
		}

		MakeAppearMonser( &AppearMon, pMonster );		

		NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&AppearMon, AppearMon.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		
		if( pMonster->GetUnitStatus() == UNIT_STATUS_WALKING )
		{
			MoveMon.dwMonsterIndex	= pMonster->GetID();
			MoveMon.v2MoveStartPos	= *pMonster->GetCurPosition();
			MoveMon.v2MoveStopPos	= *pMonster->GetDestPosition();
			MoveMon.v2MoveDirection	= *pMonster->GetCurDirection();
			MoveMon.bMoveType		= UNIT_STATUS_WALKING;
			MoveMon.fSpeed			= pMonster->GetMoveSpeed();
			NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&MoveMon, MoveMon.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		}

		// 가지고 있던 상태 스킬을 다시 보내줘라.
		// Modify By Minbobo.. 마을 입장할때 다운되서 수정했음.

		if(pMonster->GetUsingStatusEffectList())
		{
			posSkill = pMonster->GetUsingStatusEffectList()->GetHeadPosition();
			
			while(posSkill)
			{
				
				EffectDesc* pEffectDesc = (EffectDesc*)pMonster->GetUsingStatusEffectList()->GetNext(posSkill);
				g_pEffectLayer->SendSkillStatusUserMon(pOwnUser, pMonster, pEffectDesc->pEffect->bID, pEffectDesc->bSkillLevel, pEffectDesc->dwDestTick-g_dwTickCount);		
			}
		}		
	}
	
	// 땅에 떨어진 아이템의 정보를 받아야한다.
	DSTC_APPEAR_ITEM	AppearItem;
	pItemNode = pSection->m_pItemHash->GetHead();
	while( pItemNode )
	{
		pItem = pItemNode->pData;
		
		if(!pItem)
			return;

		AppearItem.bType		= 0;
		AppearItem.v2ItemPos	= pItem->v2ItemPos;
		memcpy( &AppearItem.Item, &pItem->Item, sizeof( CItem ) );
		NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&AppearItem, AppearItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

		pItemNode = pItemNode->pNext;
	}	
}

//-----------------------------------------------------------------------------
// 지정해준 섹션번호의 섹션 포인터를 리턴한다. 
//-----------------------------------------------------------------------------
CSection* CDungeonLayer::GetSection(int nSectionNum) const
{	
	if(nSectionNum <= 0 || nSectionNum > this->m_wTotalSectionNum )	
	{
		return NULL;	
	}

	return m_pSection[ nSectionNum - 1 ];
}


//-----------------------------------------------------------------------------
// 지정해준 섹션을 현재층의 활성화 되어있는 섹션 List에 추가한다. 현재층의 활성화 되어있는 섹션 List에 추가한다.
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::AddActiveSection(int nSectionNum)
{
	if(nSectionNum < 1 || nSectionNum > MAX_SECTION_NUM_PER_LAYER)
	{
		__asm int 3
		return FALSE;
	}
	
	CSection* pSection = GetSection( nSectionNum );
	if( !pSection )
	{
		__asm int 3
		return FALSE;
	}

	pSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pSection );

	if(!pSection->m_pActiveListPos)
	{
		__asm int 3
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// 지정해준 섹션을 현재층의 활성화 되어있는 섹션 List에 추가한다. 현재층의 활성화 되어있는 섹션 List에 추가한다.
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::AddActiveSection(CSection* pSection)
{
	if(!pSection)	return FALSE;

	pSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pSection );

	if(!pSection->m_pActiveListPos)
	{
		__asm int 3
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// 지정해준 섹션을 현재층의 활성화 되어있는 섹션 List에서 섹션 제거. 현재층의 활성화 되어있는 섹션 List에서 섹션 제거.
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::RemoveActiveSection(int nSectionNum)
{
	if(nSectionNum < 1 || nSectionNum > MAX_SECTION_NUM_PER_LAYER)
	{
		__asm int 3
		return FALSE;
	}
	
	CSection*	pSection = GetSection( nSectionNum );

	if( !pSection )
	{
		__asm int 3
		return FALSE;
	}

	m_pActiveSectionList->RemoveAt( pSection->m_pActiveListPos );
	pSection->m_pActiveListPos = NULL;
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// 지정해준 섹션을 현재층의 활성화 되어있는 섹션 List에서 섹션 제거. 현재층의 활성화 되어있는 섹션 List에서 섹션 제거.
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::RemoveActiveSection(CSection* pSection)
{
	if(!pSection)	return FALSE;

	m_pActiveSectionList->RemoveAt( pSection->m_pActiveListPos );
	pSection->m_pActiveListPos = NULL;

	return TRUE;
}


//-----------------------------------------------------------------------------
// 현재 유저의 섹션위치를 이동시킴. 현재 유저의 섹션위치를 이동시킴
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::MoveSection(CUser* pUser , int nFrom, int nTo)
{
	if(nFrom < 1 || nFrom > MAX_SECTION_NUM_PER_LAYER || nTo < 1 || nTo > MAX_SECTION_NUM_PER_LAYER)
	{
		__asm int 3
		return FALSE;
	}

	CSection* pSection = GetSection(nFrom);
	if( !pSection ) return FALSE;
	
	RemoveActiveSection( pSection );

	for(int k=0; k<MAX_SECTION_LINK_NUM; k++)	//Link 되어있는 쎅션들 모두 Disable 시키고 
	{
		if(pSection->m_wLink[k] && GetSection( pSection->m_wLink[k] )->m_pActiveListPos )
			RemoveActiveSection( pSection->m_wLink[ k ] );
	}

	pSection->m_pPcList->RemoveAt( pUser->m_pPrevSectionNode );	//Section에 있는 PCList에서 유저 삭제
	pUser->m_pPrevSectionNode = NULL;
		
	//새로운 섹션으로 Activate
	pSection = GetSection( nTo );
	if( !pSection ) return FALSE;

	AddActiveSection( pSection );
	
	for(k=0; k<MAX_SECTION_LINK_NUM; k++)	//링크된 맵도 모두 Activate
	{
		if(pSection->m_wLink[ k ])
			AddActiveSection( pSection->m_wLink[k] );
	}

	pUser->m_pPrevSectionNode = pSection->m_pPcList->AddTail( pUser );

	return TRUE;
}


//-----------------------------------------------------------------------------
// 현재 몬스터의 섹션위치를 이동시킴. 현재 몬스터의 섹션위치를 이동시킴
//-----------------------------------------------------------------------------
BOOL CDungeonLayer::MoveSection(CMonster* pMonster , int nFrom, int nTo)
{
	if(nFrom < 1 || nFrom > MAX_SECTION_NUM_PER_LAYER || nTo < 1 || nTo > MAX_SECTION_NUM_PER_LAYER)
	{
		__asm int 3
		return FALSE;
	}

	CSection* pSection = GetSection( nFrom );
	RemoveActiveSection( pSection );

	for(int k=0; k<MAX_SECTION_LINK_NUM; k++)	//Link 되어있는 쎅션들 모두 Disable 시키고 
	{
		if(pSection->m_wLink[ k ] && GetSection(pSection->m_wLink[ k ])->m_pActiveListPos)
			RemoveActiveSection( pSection->m_wLink[ k ]);
	}

	pMonster->RemoveCurSection();
		
	//새로운 섹션으로 Activate
	pSection = GetSection( nTo );
	AddActiveSection( nTo );

	for(k=0; k<MAX_SECTION_LINK_NUM; k++)	//링크된 맵도 모두 Activate
	{
		if(pSection->m_wLink[ k ])
			AddActiveSection(pSection->m_wLink[k]);
	}

	pMonster->SetCurSection(pSection);
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// 두벡터 사이의 거리를 구함 
//-----------------------------------------------------------------------------
double CDungeonLayer::GetDistance(VECTOR2* vFrom, VECTOR2* vTo)
{
	return sqrt( (vTo->x - vFrom->x) * (vTo->x - vFrom->x) + (vTo->y - vFrom->y) * (vTo->y - vFrom->y));
}


//-----------------------------------------------------------------------------
// 현재 던전 타입을 리턴함. 
//-----------------------------------------------------------------------------
BYTE CDungeonLayer::GetDungeonType()
{
	return m_pParent->GetDungeonKind();
}

BOOL CDungeonLayer::InsertUser( CUser* pUser, const VECTOR2* pv2Start )
{
	if( !pUser ) return FALSE;

	// 이제 레이어와 섹션에 유저를 추가하고 해당섹션과 링크된 섹션을 활성화 시킨다.
	pUser->SetLayerIndex(m_bLayer);
	pUser->m_pLayerUserListNode = m_pPcList->AddTail( pUser );	// 레이어에 유저 추가.
	pUser->SetCurPosition(pv2Start);
	
	// 만약 섹션에 등록이 실패 했다면..
	if( !InsertUserSection( pUser ) )						
	{
		m_pPcList->RemoveAt( pUser->m_pLayerUserListNode );
		pUser->m_pLayerUserListNode = NULL;

		if (pUser->GetCurMapTile())
		{
			m_pMap->SetTileOccupied( pUser->GetCurMapTile()->wIndex_X, pUser->GetCurMapTile()->wIndex_Z, TILE_EMPTY, NULL );
		}
		
		return FALSE;
	}	
	
	++m_wTotalUserCount;
	
	if(!pUser->GetHP())
	{
		pUser->SetHP(1);
	}

	Event_InsertDungeonLayerUser( pUser, this );	

	return TRUE;
}

	
// Layer에서 유저를 지운다.
BOOL CDungeonLayer::RemoveUser( CUser* pUser )
{
	Event_RemoveDungeonLayerUser( pUser, this );	
		
	// 모든 관계관 몬스터를 지운다.
	m_pParent->RemoveAllUserMonster( pUser );		

	if (pUser->GetCurMapTile())
	{
		m_pMap->SetTileOccupied( pUser->GetCurMapTile()->wIndex_X, pUser->GetCurMapTile()->wIndex_Z, TILE_EMPTY, pUser );
	}

	RemoveUserSection( pUser );

	try
	{
		m_pPcList->RemoveAt( pUser->m_pLayerUserListNode );
	}
	catch(...)
	{
		Log(LOG_IMPORTANT, "Exception from RemoveUser, RemoveAt");
	}

	pUser->m_pLayerUserListNode = NULL;
	pUser->SetLayerIndex(INVALID_DUNGEON_LAYER);

	--m_wTotalUserCount;
	
	if (m_bMatching)
	{
		if (m_bReadyMatchNum > 0)
		{
			m_bReadyMatchNum --;
		}
	}	

	return TRUE;
}


// 해당 섹션에 유저를 추가한다.
BOOL CDungeonLayer::InsertUserSection( CUser* pUser, WORD wSectionNum )
{
	if( !pUser ) return false;
	if( wSectionNum > m_wTotalSectionNum ) return false;
	
	CSection* pSection = GetSection( wSectionNum );
	if( !pSection ) return false;
	
	pUser->m_pPrevSectionNode = pSection->m_pPcList->AddTail( pUser );

	// 이게 제로베이스가 아닌것을 주의.
	pUser->SetPrevSectionNum(pSection->m_bSectionNum);	
	
	if( !pSection->m_pActiveListPos )	// 활성화가 아니면 활성화에 추가.
	{
		pSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pSection );
	}
	
	// 조금 어지러워 졌다.
	CSection* pActiveSection = 0;

	for( int i=0; (pSection->m_wLink[i] != 0); i++ )
	{
		pActiveSection = GetSection(pSection->m_wLink[i]);
		if( !pActiveSection ) continue;

		if( !pActiveSection->m_pActiveListPos ) // 링크된것이 활성화가 아니라면
		{
			pActiveSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pActiveSection );
		}
	}

	return true;
}


// 케릭터의 현재의 좌표를 가지고 적당한 섹션에 추가한다.
BOOL CDungeonLayer::InsertUserSection( CUser* pUser )
{
	if( !pUser ) return false;
	
	int		i=0;
	short	wSectionNum = 0;

	const MAP_TILE*	const pTile = pUser->GetCurMapTile();
	if( !pTile || pTile->wAttr.uSection == 0 ) return false;
	
	CSection* pActiveSection = 0;
	wSectionNum	= pTile->wAttr.uSection;

	CSection* pSection = GetSection( wSectionNum );
	if( !pSection ) return false;

	// 적당한 섹션을 찾았으면 섹션에 추가한다.
	pUser->m_pPrevSectionNode = pSection->m_pPcList->AddTail( pUser );

	// 이게 제로베이스가 아닌것을 주의.
	pUser->SetPrevSectionNum(pSection->m_bSectionNum);				
	
	// 활성화가 아니면 활성화에 추가.
	if( !pSection->m_pActiveListPos )							
	{
		pSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pSection );
	}
	
	// 조금 어지러워 졌다.
	for( i=0; (pSection->m_wLink[i] != 0); i++ )
	{
		pActiveSection = GetSection( pSection->m_wLink[i] );
		if( !pActiveSection ) continue;

		if( !pActiveSection->m_pActiveListPos ) // 링크된것이 활성화가 아니라면
		{
			pActiveSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pActiveSection );
		}
	}	
		
	return true;
}


// 해당 섹션에서 유저를 삭제한다.
BOOL CDungeonLayer::RemoveUserSection( CUser* pUser )
{
	if( !pUser ) return false;
	if( pUser->GetPrevSectionNum() == INVALID_DUNGEON_SECTION ) return false;
	
	CSection* pSection = GetSection( pUser->GetPrevSectionNum() );
	if( !pSection ) return false;
	
	// 일단 해당 세션에서 유저를 지우고..
	pSection->m_pPcList->RemoveAt( pUser->m_pPrevSectionNode );
	pUser->m_pPrevSectionNode = NULL;
	if( !pSection->m_pPcList->IsEmpty() ) return true;	// 다른 유저가 있다면 지울 필요없음..
	
	BOOL		bFlag = TRUE;
	CSection*	pLinkSection=0;
	CSection*	pLinkLink=0;
	int			i=0, j=0;

	for( i = 0; (pSection->m_wLink[i] != 0); i++ )
	{
		pLinkSection = GetSection( pSection->m_wLink[i] );	// 링크 섹션의 포인터.
		if( !pLinkSection ) continue;

		if( !pLinkSection->m_pPcList->IsEmpty() )
		{
			bFlag = FALSE;	// 링크된 섹션중에 한곳이라도 유저가 있다면 FALSE 세팅.
		}
		else	//비었다면..
		{
			m_pActiveSectionList->RemoveAt( pLinkSection->m_pActiveListPos );
			pLinkSection->m_pActiveListPos = NULL;	// 일단 비활성화 해놓고..
			for( j=0; (pLinkSection->m_wLink[j] != 0); j++ )
			{
				pLinkLink = GetSection(pLinkSection->m_wLink[j]);
				if( !pLinkLink ) continue;

				if( !pLinkLink->m_pPcList->IsEmpty() )	// 링크섹션의 링크섹션에 사람이 있다면.
				{
					pLinkSection->m_pActiveListPos = m_pActiveSectionList->AddTail( pLinkSection );
					break;
				}
			}
		}		
	}

	if( bFlag )	// 링크된 섹션들도 다 비었다면.
	{
		m_pActiveSectionList->RemoveAt( pSection->m_pActiveListPos );
		pSection->m_pActiveListPos = NULL;
	}
	
	pUser->SetPrevSectionNum(INVALID_DUNGEON_SECTION);

	return true;
}

BOOL CDungeonLayer::InsertMonster( CMonster* pMonster, const VECTOR2* v2Start )
{
	if( !pMonster ) return false;

	CMonster* pHashMonster = g_pMonsterTable->GetMonster(pMonster->GetID());

	if(!pHashMonster)
		Log(LOG_JUST_FILEWRITE, "g_MonsterTable = NULL");
	
	pMonster->SetCurDungeon(m_pParent);
	pMonster->SetLayerIndex(m_bLayer);
	pMonster->SetCurDungeonLayer(this);
	pMonster->SetCurPosition(v2Start);
	pMonster->SetPrevSectionNum(0);
	
	// 알맞은 섹션을 찾은후 섹션에도 삽입.
	DWORD dwCurX = (DWORD)(pMonster->GetCurPosition()->x / TILE_WIDTH);
	DWORD dwCurZ = (DWORD)(pMonster->GetCurPosition()->y / TILE_HEIGHT);
	
	for( int i=0; i<m_wTotalSectionNum; i++ )
	{
		if( (dwCurX >= m_pSection[i]->x1) && (dwCurX <= m_pSection[i]->x2 ) && (dwCurZ >= m_pSection[i]->y1) && (dwCurZ <= m_pSection[i]->y2 ) )
		{
			pMonster->SetPrevSectionNum(m_pSection[i]->m_bSectionNum);
			break;
		}
	}
			
	// 해당 섹션에 삽입.
	if( !InsertMonsterSection( pMonster, pMonster->GetPrevSectionNum() ) )
	{
		return false;
	}

	pMonster->CreateResource();

	return true;
}

BOOL CDungeonLayer::InsertMonsterSection( CMonster* pMonster, WORD wSectionNum )
{
	if( !pMonster ) return false;
	if( wSectionNum > m_wTotalSectionNum ) return false;
	
	CSection* pSection = GetSection( wSectionNum );
	if( !pSection ) return false;

	pMonster->SetCurSection(pSection);
	
	return true;
}

BOOL CDungeonLayer::RemoveMonsterSection( CMonster* pMonster )
{
	if( !pMonster ) 
		return false;

	pMonster->RemoveCurSection();
	
	return true;
}


BOOL CDungeonLayer::InsertItem( ITEM_SERVER* pItem )
{
	if( !pItem )
		return true;

	if( pItem->dwSectionNum == 0 )
		return true;
	
	if( m_pItemHash->Insert( pItem, pItem->Item.m_Serial.i64Serial ) == -1 )
		return true;

	if( m_pSection[pItem->dwSectionNum-1]->m_pItemHash->Insert( pItem, pItem->Item.m_Serial.i64Serial ) == -1)
		return true;
	
	return true;
}

BOOL CDungeonLayer::RemoveItem( ITEM_SERVER* pItem )
{
	if( !pItem ) return false;
	
	m_pItemHash->Delete( pItem, pItem->Item.m_Serial.i64Serial );
	m_pSection[pItem->dwSectionNum-1]->m_pItemHash->Delete( pItem, pItem->Item.m_Serial.i64Serial );
	
	LALFree( g_pItemPool, (void*)pItem );

	return true;
}

void CDungeonLayer::__ProcessDeadMonsterClear()
{
	POSITION_ Del = 0;
	POSITION_ MonsterPos = m_pDeadMonsterList->GetHeadPosition();

	while(MonsterPos)
	{
		try
		{
			Del = MonsterPos;
			CMonster* pMonster = (CMonster*)m_pDeadMonsterList->GetNext(MonsterPos);
			RemoveMonster(pMonster);
			m_pDeadMonsterList->RemoveAt(Del);
		}
		catch(...)
		{
			m_pDeadMonsterList->RemoveAt(Del);
			Log(LOG_IMPORTANT, "Exception from __ProcessDeadMonsterClear");
		}
	}
}

void CDungeonLayer::__ProcessUpdateMonster()
{
	POSITION_ SectionPos = m_pActiveSectionList->GetHeadPosition();
	while( SectionPos )
	{
		CSection* pSection = (CSection*)m_pActiveSectionList->GetNext( SectionPos );

		POSITION_ MonsterPos = pSection->m_pMonsterList->GetHeadPosition();
		while( MonsterPos )
		{
			CMonster* pMonster = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );			
			if( pMonster->GetUnitStatus() != UNIT_STATUS_DEAD )
			{
				try
				{
					pMonster->Update();
				}
				catch(...)
				{
					Log(LOG_IMPORTANT, "Exception from CMonster::Update");
					Log(LOG_IMPORTANT, "Monster will be dead!!");
					pMonster->SetStatus(UNIT_STATUS_DEAD);
					UpdateMonsterForAI(pMonster);
				}
			}
		}	
	}
}

void CDungeonLayer::__ProcessRespawnMonster()
{
	POSITION_ MonsterPos = m_pResponseMonsterList->GetHeadPosition();

	while( MonsterPos )
	{
		CMonster* pMonster = (CMonster*)m_pResponseMonsterList->GetNext( MonsterPos );
		ReviveMonster(pMonster);
	}
}

void CDungeonLayer::__ProcessCreateItem()
{
	POSITION_ ItemPos = m_pCreateItemList->GetHeadPosition();
	
	while(ItemPos)
	{
		ITEM_CREATE* pCreateItem = (ITEM_CREATE*)m_pCreateItemList->GetNext( ItemPos );
		if(!pCreateItem)
			continue;
		
		if( g_dwTickCount - pCreateItem->dwCreateTick <= 1000 )
			break;

		// 아이템을 생성해야한다.
		DSTC_APPEAR_ITEM AppearItem;
		AppearItem.bType	= 1;
		AppearItem.v2ItemPos= pCreateItem->v2ItemPos;

		memcpy( &AppearItem.Item, &pCreateItem->Item, sizeof(CItem) );
		
		ITEM_SERVER* pItem = (ITEM_SERVER*)LALAlloc( g_pItemPool );

		MakeItemStruct( pItem, &pCreateItem->Item, &pCreateItem->v2ItemPos, pCreateItem->dwSectionNum, pCreateItem->dwOwnerIndex, pCreateItem->dwPartyIndex, 0, 0 );

		m_pCreateItemList->RemoveAt( pCreateItem->DelPos );	// 생성했으면 지운다.
		LALFree( g_pCreateItemPool, (void*)pCreateItem );
		
		if( InsertItem( pItem ) == false ) 
		{
			Log(LOG_ALL, "incorrect item was drop : %d", pItem->Item.GetID());
			continue;//서버 다운 현상: hwoarang 050202 
		}

		BroadCastSectionMsg( (char*)&AppearItem, AppearItem.GetPacketSize(), (BYTE)pItem->dwSectionNum );
	}
}

void CDungeonLayer::__ProcessDeleteItem()
{
	// 10Sec
	if( g_dwTickCount - m_dwLastFrameSec > 10000 )	
	{
		//아이템 지우기
		m_dwLastFrameSec = g_dwTickCount;

		ITEM_SERVER*				pItem=0;
		ListNode<ITEM_SERVER>*		pItemNode=0;
		DSTC_DISAPPEAR_ITEM			DisAppearItem;
		pItemNode = m_pItemHash->GetHead();

		while( pItemNode )
		{
			pItem		= pItemNode->pData;
			pItemNode	= pItemNode->pNext;

			pItem->dwCurFramePerSec += 10;

			if(pItem->dwCurFramePerSec>=70)
			{
				// 만약 가디언 아이템이라면.
				if( ( pItem->Item.m_wItemID / ITEM_DISTRIBUTE ) == 19 )			
				{
					// 만약 생성되었던 가디언이라면 디비에서도 지운다.
					if( GUARDIAN_CREATE_STATUS_ALREADYCALL == pItem->Item.m_Item_Guardian.bCreate )	
					{
						char szSQL[255] = {0,};
						wsprintf( szSQL, "delete from GuardianInfo where ID = %d", pItem->Item.m_Item_Guardian.dwID );
						g_pDb->THExecuteSQL( szSQL, FALSE, 0, NULL, GAME_DB );
					}
				}

				RemoveItem( pItem );
				DisAppearItem.i64ItemID = pItem->Item.m_Serial.i64Serial;
				BroadCastSectionMsg( (char*)&DisAppearItem, DisAppearItem.GetPacketSize(), (BYTE)pItem->dwSectionNum );
			}
		}
	}
}

void CDungeonLayer::__ProcessUpdateItemOwner()
{
	// 마지막 아이템 소유자 체크 후 5초가 지났다면
	if( g_dwTickCount - m_dwLastOwnerFrameSec > 5000 )	
	{
		// 마지막 아이템 소유자 체크 시간을 갱신
		m_dwLastOwnerFrameSec = g_dwTickCount;

		ITEM_SERVER*				pItem=0;
		ListNode<ITEM_SERVER>*		pItemNode=0;
		pItemNode = m_pItemHash->GetHead();

		// 모든 아이템에 대해
		while( pItemNode )
		{
			pItem = pItemNode->pData;
			pItemNode = pItemNode->pNext;
			
			// 소유자 유지 시간이 아직 안끝났으면
			if( pItem->dwOwnerFramePerSec < MAX_ITEM_OWNER_FRAME )
			{
				// 소유자 유지 시간을 5초간 늘림
				pItem->dwOwnerFramePerSec += 5;
			}
			// 시간이 지났고 소유자 혹은 소유파티가 있는 아이템이면
			else if( pItem->dwOwnerIndex != 0 || pItem->dwPartyIndex != 0)
			{
				// 소유자와 소유파티를 없애준다
				pItem->dwOwnerIndex = 0;
				pItem->dwPartyIndex = 0;
			}
		}
	}
}

void CDungeonLayer::__ProcessSiege()
{
	if (m_pParent->GetDungeonDataEx()->m_bSiege)
	{
		// 공성중일때만 검사해야 한다.
		DSTC_DESTROY_CP packet;
		CVoidList* pCPList = m_pMap->m_pCPList;
		POSITION_ pos = pCPList->GetHeadPosition();

		while(pos)
		{
			// cp를 검사한다.
			CP_DESC* pCPDesc = (CP_DESC*)pCPList->GetNext(pos);

			if (!pCPDesc->bDestroy)
			{	
				if (pCPDesc->pTile->wAttr.uOccupied != TILE_OCCUPIED_PC)
				{
					//리셋해야 하므로.
					pCPDesc->dwStartTick = 0;
				}
				else
				{
					// cp에 user가 서있군.
					CUser* pCP_User = (CUser*)pCPDesc->pTile->pData;
					if (pCP_User->GetAttackMode() == ATTACK_MODE_DEFENCE)
						continue;// 방어자는 풀지 못한다.
					
					if (!pCPDesc->dwStartTick)	
					{
						pCPDesc->dwStartTick = g_dwTickCount;
						continue; // 인제 시작한 걸까?
					}
					
					if (g_dwTickCount - pCPDesc->dwStartTick 
						>= m_pParent->GetDungeonDataEx()->GetCPTime())
					{
						// cp 파괴해라.
						pCPDesc->dwStartTick = 0;
						pCPDesc->bDestroy = 1;
						GetMap()->m_bCPRemainCount--;
						
						// 마지막 층에 있을때 cp를 전부 해제 했다면.수호가디언을 소환해라.
						if (m_pParent->GetTotalLayer()-1 == pCP_User->GetCurLayerIndex()
							&& !GetMap()->m_bCPRemainCount)
						{
							if (m_pParent->GetDungeonDataEx()->m_byCallGuardian)
							{
								m_pParent->CreateDungeonGuardian();
							}
							else
							{
								m_pParent->SetDungeonOwner(pCP_User->GetID()
									, pCP_User->m_dwGuildId
									, pCP_User->m_dwPartyId
									, pCP_User->m_szId
									, pCP_User->GetName() );
							}
						}						
						
						packet.sCP_Info.bCPID		= pCPDesc->bID;
						packet.sCP_Info.bDestroy	= 1;
						packet.sCP_Info.wProperty	= pCPDesc->wProperty;
						
						BroadCastMsg( (char*)&packet, packet.GetPacketSize() );

						for(int i = 0; i < 5; ++i)
						{
							if (g_pCPTable[pCPDesc->wProperty].sCPTable_Value[i].wID 
									&& g_pCPTable[pCPDesc->wProperty].sCPTable_Value[i].wID < 10000)
							{
								// skill 적용해야한다.
								VECTOR2 vecStartPosition = {pCPDesc->pTile->x, pCPDesc->pTile->z};
								WORD wMinMax[2] = {0, 0};

								Effect* pEffect = g_pEffectLayer->GetEffectInfo((BYTE)g_pCPTable[pCPDesc->wProperty].sCPTable_Value[i].wID);

								if (pEffect->bFormula == 2	||
									pEffect->bFormula == 20 ||
									pEffect->bFormula == 21 ||
									pEffect->bFormula == 22)
								{
									wMinMax[0] = 100;
									wMinMax[1] = 300;
								}

								SystemSkillToUser(this
									, (BYTE)g_pCPTable[pCPDesc->wProperty].sCPTable_Value[i].wID
									, (BYTE)g_pCPTable[pCPDesc->wProperty].sCPTable_Value[i].wValue
									, g_pCPTable[pCPDesc->wProperty].wApply_Time*1000
									, &vecStartPosition
									, wMinMax
									, OBJECT_TYPE_SKILL
									, 0
									, pCP_User
								);
							}
						}
					}				
				}		
			}
		}
	}
}

void CDungeonLayer::__ProcessMatch()
{
	if (m_bMatching)
	{
		if (m_dwMatchBeginTime && g_dwTickCount >= (m_dwMatchBeginTime+MATCH_START_TERM_TICK) )
		{ 
			m_dwMatchBeginTime = 0;
			
			DSTC_START_MATCH_MAN2MAN packet;
			m_bStartMatch = TRUE;

			POSITION_ pos = m_pPcList->GetHeadPosition();

			while(pos)
			{
				CUser * pUserTemp = (CUser*) m_pPcList->GetNext(pos);
								
				if (pUserTemp && pUserTemp->m_sMatchDescInfo.m_bMatching == TRUE)
				{  
					// 전적 카운트
					MatchRecordCount(pUserTemp);

					CUser * pUser = g_pUserHash->GetData(pUserTemp->m_sMatchDescInfo.m_dwMatchUserIndex);

					packet.dwMatchUserIndex = (pUser) ? pUser->GetID() : 0;// 대전상대의 인덱스를 넣어둔다.

					// 대전 시간
					packet.dwMatchTimeTick = MATCH_TIME;

					NetSendToUser(pUserTemp->m_dwConnectionIndex, (char *)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);				
				}
			}

			m_dwMatchEndTime = 0;
			m_dwEndMatchJudgmentTick = g_dwTickCount + MATCH_TIME;
		}		
		else if (m_dwEndMatchJudgmentTick && g_dwTickCount >= m_dwEndMatchJudgmentTick /* m_dwMatchEndTime */ )
		{ 
			// Time Out
			m_dwEndMatchJudgmentTick = 0;

			DSTC_END_MATCH_MAN2MAN	packet;
			float	fRemainHPRate[2]	= {0.f, };	// 남아있는 HP 퍼센티지	
			CUser * pMatchUser[2]		= {NULL, };
			packet.bJudgmentCode		= 1;		// Time over

			int iMatchUser = 0;
			POSITION_ pos = m_pPcList->GetHeadPosition();

			// 판정하기위해서 에너지 율을 계산한다.
			while(pos)
			{
				CUser * pUserTemp = (CUser*) m_pPcList->GetNext(pos);

				if (pUserTemp->m_sMatchDescInfo.m_bMatching == TRUE)
				{
					pMatchUser[iMatchUser] = pUserTemp;
					fRemainHPRate[iMatchUser] = 
						(WORD)pMatchUser[iMatchUser]->GetHP() / (WORD)pMatchUser[iMatchUser]->GetMaxHP() * 100.f;	// 에너지 퍼센티지 

					iMatchUser++;
				}
			}

			if (!pMatchUser[0] || !pMatchUser[1])
			{
				// 둘중에 아무나 문제가 있으면 대전 정보를 날리지않는다.
				m_dwMatchEndTime = g_dwTickCount + MATCH_JUDGMENT_TERM_TICK;
			}
			else
			{ 
				memset(packet.szWinnerName, 0, MAX_CHARACTER_REAL_LENGTH);
				memset(packet.szLoserName, 0, MAX_CHARACTER_REAL_LENGTH); 

				// 판정
				if (fRemainHPRate[0] > fRemainHPRate[1])
				{ 
					__lstrcpyn(packet.szWinnerName, (LPSTR)pMatchUser[0]->GetName(), MAX_CHARACTER_REAL_LENGTH);
					__lstrcpyn(packet.szLoserName, (LPSTR)pMatchUser[1]->GetName(), MAX_CHARACTER_REAL_LENGTH); 

					// 승리 카운트 
					MatchWinCount(pMatchUser[0]);

					// 패배 카운트
					MatchLoseCount(pMatchUser[1]);
				}
				else if (fRemainHPRate[0] < fRemainHPRate[1]) 
				{ 
					__lstrcpyn(packet.szWinnerName, (LPSTR)pMatchUser[1]->GetName(), MAX_CHARACTER_REAL_LENGTH);
					__lstrcpyn(packet.szLoserName, (LPSTR)pMatchUser[0]->GetName(), MAX_CHARACTER_REAL_LENGTH); 

					// 승리 카운트 
					MatchWinCount(pMatchUser[1]);

					// 패배 카운트
					MatchLoseCount(pMatchUser[0]);
				}
				else
				{ // 무승부
					packet.bJudgmentCode	= 2;	// 무승부 
					__lstrcpyn(packet.szWinnerName, (LPSTR)pMatchUser[0]->GetName(), MAX_CHARACTER_REAL_LENGTH);
					__lstrcpyn(packet.szLoserName, (LPSTR)pMatchUser[1]->GetName(), MAX_CHARACTER_REAL_LENGTH); 
				}
				
				if (pMatchUser[0])
					pMatchUser[0]->GetCurDungeon()->GetDungeonLayer(0)->BroadCastMsg((char *)&packet, packet.GetPacketSize());
				else if (pMatchUser[1])
					pMatchUser[1]->GetCurDungeon()->GetDungeonLayer(0)->BroadCastMsg((char *)&packet, packet.GetPacketSize());			
				
				BroadCastMsg((char*)&packet, packet.GetPacketSize());
				
				m_dwMatchEndTime = g_dwTickCount + MATCH_JUDGMENT_TERM_TICK; // 5초 후 대전을 종료하라
			}
		}
		else if ( m_dwMatchEndTime && g_dwTickCount >= m_dwMatchEndTime )
		{
			m_dwMatchEndTime = 0;
			CUser * pMatchUser[2]	= {0,};
			int		iMatchUser		= 0;

			POSITION_ pos = m_pPcList->GetHeadPosition();

			while(pos)
			{
				CUser * pUserTemp = (CUser*) m_pPcList->GetNext(pos);
				if ( pUserTemp )
				{	
					if (pUserTemp->m_sMatchDescInfo.m_bMatching == TRUE)
					{
						// 대전중일때 죽으면 이리로 온다. 
						pMatchUser[iMatchUser] = pUserTemp;
						pMatchUser[iMatchUser]->m_sMatchDescInfo.m_bMatching = FALSE;
						pMatchUser[iMatchUser]->m_sMatchDescInfo.m_dwMatchUserIndex = 0;
						
						// 사용중인 마법을 모두 없앤다.
						// 사용중인 스킬을 치운다.
						pMatchUser[iMatchUser]->RemoveAllDetachSkill();

#if defined JAPAN_LOCALIZING
// 일본 서버에서는 사용자 되살아날때 전체 HP 의 50 % 회복
// ( 2004. 09. 12. minjin )
						if ( pMatchUser[iMatchUser]->GetHP() == 0 )
						{
							DSTC_USER_STATUS	UserStatus;
							UserStatus.bStatusMany	= 0;

							// 대전 후 HP 를 반땅 채워준다.
							pMatchUser[iMatchUser]->SetStatus(UNIT_STATUS_NORMAL, TRUE);
							pMatchUser[iMatchUser]->SetHP( pMatchUser[iMatchUser]->GetMaxHP() / 2 );

							// HP 반땅 채워준다.(클라이언트에 알린다.)
							UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
							UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pMatchUser[iMatchUser]->GetMaxHP() / 2;
							
							NetSendToUser(pMatchUser[iMatchUser]->m_dwConnectionIndex, (char*)&UserStatus,  UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
						}
#endif
						pMatchUser[iMatchUser]->SetStatus(UNIT_STATUS_NORMAL, TRUE);
						pMatchUser[iMatchUser]->GetCurDungeon()->ChangeLayerUser(pMatchUser[iMatchUser], pMatchUser[iMatchUser]->GetCurLayerIndex(), 0, 0);

						iMatchUser++;
					}
				}
			}
		}

		if (m_bStartMatch)
		{ 
			// 대전 시작하고 아무도 없다.
			if (m_pPcList->IsEmpty())
			{
				InitMatch();
			}
		}
	}
}

// 던젼 서버 업데이트...
void CDungeonLayer::Process()
{
	__ProcessDeadMonsterClear();	// 죽은 넘들은 죽인다.
	__ProcessUpdateMonster();		// 몬스터 업데이트
	__ProcessRespawnMonster();		// 몬스터 리스폰
	__ProcessCreateItem();			// 아이템 생성
	__ProcessDeleteItem();			// 아이템 제거(생성보다 제거 먼저하는 게 낫지 않나? 글쎄..)
	__ProcessUpdateItemOwner();		// 아이템 소유권 업데이트
	__ProcessSiege();				// 공성 처리
	__ProcessMatch();				// 1:1 대결 처리

	__ProcessTeamMatch();// 팀매치 : hwoarang 050202 
}


void CDungeonLayer::InitList()
{
	m_pPcList				= new CPcList;
	m_pMonsterList			= new CMonsterList;
	m_pResponseMonsterList	= new CMonsterList;
	m_pCreateItemList		= new CCreateItemList;
	m_pActiveSectionList	= new CActiveSectionList;
	m_pDeadMonsterList		= new CVoidList;

	m_pItemHash				= new ITEM_HASH;
	m_pItemHash->InitializeHashTable( 5000, 5000 );
}


// 이레이어가 가지고 있는 몬스터를 전부 만든다. 처음에 사용
void CDungeonLayer::CreateMonsterAll()
{
	int					i=0;
	CMonster*			pMonster=0;
	LPBASEMONSTER		pBaseMonster=0;
	VECTOR2				v2Start = {0.f, 0.f};

	for( i = 0; i < MAX_MONSTER_FORMATION && m_pLayerFormation->pMonsterFormation[i].dwMonsterID != 0; i++ )
	{		
		pBaseMonster = g_pBaseMonsterHash->GetData( m_pLayerFormation->pMonsterFormation[i].dwMonsterID );
		if( !pBaseMonster )
			continue;

		if (pBaseMonster->bRace == 0)
			pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_TRADER_MONSTER);
		else	
			pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER);
		
		pMonster->SetInfoIndex( WORD(i) );
		pMonster->Initialize(pBaseMonster, NULL, NULL, 0, pBaseMonster->wBaseLevel, pBaseMonster->dwExp);
				
		v2Start.x = TILE_X( m_pLayerFormation->pMonsterFormation[i].wX );
		v2Start.y = TILE_Z( m_pLayerFormation->pMonsterFormation[i].wY );

		pMonster->SetDestPosition(&v2Start);
		
		// Insert Monster.
		InsertMonster( pMonster, &v2Start );

		++m_wCurMonster;
		++m_wTotalMonster;						
	}
}


void CDungeonLayer::CreateMonster( CMonster* pMonster )
{	
	pMonster->RemoveResponList();

	if (pMonster->IsNormalMonster())
		++m_wCurMonster;

	VECTOR2 v2Start = 
	{
		TILE_X( m_pLayerFormation->pMonsterFormation[pMonster->GetInfoIndex()].wX ),
		TILE_Z( m_pLayerFormation->pMonsterFormation[pMonster->GetInfoIndex()].wY ) 
	};

	pMonster->SetCurPosition(&v2Start);
	
	pMonster->SetHP(pMonster->GetMaxHP());
	pMonster->SetTemp(NPC_TEMP_CREATETYPE, 0);	// 이넘은 죽으면 지운다.	
	pMonster->SetStatus(UNIT_STATUS_NORMAL, FALSE, TRUE);
	pMonster->SetTemp(NPC_TEMP_LASTATTACKTICK, g_dwTickCount);
	pMonster->SetTemp(NPC_TEMP_LASTSTATUSTICK, g_dwTickCount);	

	InsertMonster( pMonster, &v2Start );	
	
	int			iRandX=0, iRandZ=0, iSignX=0, iSignZ=0;
	int         iCnt=0;
	BOOL		bSpot= TRUE;
	MAP_TILE*	pTile = NULL;

	do
	{
		// X, Z 그리고 부호를 구한다.
		iRandX = rand()%8 + 1;
		iRandZ = rand()%8 + 1;
		iSignX = rand()%2;
		iSignZ = rand()%2;
		if( iSignX ) iRandX *= -1;
		if( iSignZ ) iRandZ *= -1;
		
		pTile = m_pMap->GetTile( pMonster->GetCurPosition()->x + ( (float)iRandX * TILE_WIDTH ), 
								pMonster->GetCurPosition()->y + ( (float)iRandZ * TILE_HEIGHT ) );
		
		if( iCnt >= 30 ) 
		{
			bSpot = FALSE;
			break;
		}

		iCnt++;

	} while( ( (pTile == NULL) || ( pTile->wAttr.uSection == 0 ) ) || ( pTile->wAttr.uOccupied == TILE_OCCUPIED_DONT_MOVE) );
	
	if( bSpot )
	{
		v2Start.x = TILE_X( pTile->wIndex_X );
		v2Start.y = TILE_Z( pTile->wIndex_Z );
		pMonster->SetCurPosition(&v2Start);
	}
	
	pMonster->SetDestPosition(pMonster->GetCurPosition());
	
	// 몬스터 생성을 알린다.
	DSTC_APPEAR_MON AppearMon;
	MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg( (char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );
	}
}


void CDungeonLayer::CallGuardian( CUser* pUser, DWORD dwZipCode, GUARDIAN_INFO* pGuardian )
{
	CMonster* pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER); // <-- 가디언이 될 포인터.

	if (!pMonster)
	{
		Log(LOG_IMPORTANT, "CDungeonLayer::CallGuardian MonsterPool(%d)", g_pMonsterTable->GetCount());
		return;
	}
		
	// 가디언의 데이타를 만든다.	
	LPBASEMONSTER pBaseMonster = g_pBaseMonsterHash->GetData( pGuardian->dwKind + (pGuardian->bOldGuardian?80:0) );
	if( !pBaseMonster ) __asm int 3;

	pMonster->SetDestPosition(pUser->GetCurPosition());
	InsertMonster( pMonster, pUser->GetCurPosition() );
	pGuardian->dwExp = min(pGuardian->dwExp, GetExpTableOfLevel(OBJECT_TYPE_MONSTER, 200));
	pMonster->Initialize(pBaseMonster, pGuardian, pUser, (BYTE)dwZipCode
		, GetGuardianItemLevel(pGuardian->dwExp), pGuardian->dwExp);
	
	if (!pUser->IsSummonGuardian((BYTE)dwZipCode))
	{
		pUser->AttachItem(&pUser->m_pInv_Guardian[dwZipCode]);
	}

	// 몬스터 생성을 알린다.
	DSTC_APPEAR_MON	AppearMon;
			MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg((char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );
	}

	pMonster->SendAllStatus();
}


CMonster* CDungeonLayer::CallDungeonGuardian( GUARDIAN_INFO* pGuardian )
{
	CMonster* pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER);
	if (!pMonster)
	{
		Log(LOG_IMPORTANT, "CDungeonLayer::CallDungeonGuardian MonsterPool(%d)", g_pMonsterTable->GetCount());
		return 0;
	}
		
	// 생성 위치를 정한다.	 
	if( !m_pMap->m_pGuardianTile ) return NULL;

		
	// 가디언의 데이타를 만든다.
	LPBASEMONSTER pBaseMonster = g_pBaseMonsterHash->GetData( pGuardian->dwKind );
	if( !pBaseMonster ) __asm int 3;

	// CP자리에 가디언을 출현 시킨다.
	VECTOR2 v2Start = 
	{
		TILE_X( m_pMap->m_pGuardianTile->wIndex_X ),
		TILE_Z( m_pMap->m_pGuardianTile->wIndex_Z ) 
	};
	pMonster->SetDestPosition(&v2Start);
	m_pParent->SetDungeonGuardian(pMonster);

	InsertMonster( pMonster, &v2Start );

	pMonster->Initialize(pBaseMonster, pGuardian, NULL, 0, pGuardian->dwLevel, 0);
	pMonster->SetMonsterAI( 5 );

	// 몬스터 생성을 알린다.
	DSTC_APPEAR_MON	AppearMon;
	MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg((char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );
	}
	
	return pMonster;
}


void CDungeonLayer::ReviveMonster(CMonster* pMonster, DWORD dwFlag /*값이 있다면 조건이 있다는 소리다.*/)
{
	// 이벤트(생존)던전 1차 이벤트 종료 되었을때 몬스터가 생성되는걸 막기 위해 
	if(	m_pParent &&
		m_pParent->GetEventDungeonType() == EVENT_DUNGEON_SURVIVAL &&
		m_pParent->GetEventFirstCloseTick() > 0 ||
		m_pParent->GetEventClearTick() > 0)
	{
		return;		
	}

	if(	dwFlag && (pMonster->IsNormalMonster() || pMonster->IsNPC()) &&
		pMonster->GetTemp(NPC_TEMP_KILLTICK) != 0xffffffff && ( g_dwTickCount - pMonster->GetTemp(NPC_TEMP_KILLTICK) ) 
		>= ( (DWORD)m_pLayerFormation->pMonsterFormation[pMonster->GetInfoIndex()].wResponse * 1000 + 10000) ) 
	{
		// 기본 5초
		CreateMonster( pMonster );
	}
	else if (!dwFlag)
	{
		// 무조건 태어나라.
		CreateMonster( pMonster );
	}
}


CMonster* CDungeonLayer::CallMonster( CUser* pUser, DWORD dwZipCode, DWORD dwBaseMonsterNum, const VECTOR2* const v2Start)
{
	LPBASEMONSTER pBaseMonster = g_pBaseMonsterHash->GetData( dwBaseMonsterNum );
	if( !pBaseMonster )	
	{
		Log( LOG_JUST_DISPLAY, "CallMonster, Invalid BaseMonster Index!" );
		return	NULL;
	}

	CMonster* pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER);	
	if (!pMonster)
	{
		Log(LOG_IMPORTANT, "CDungeonLayer::CallMonster MonsterPool(%d)", g_pMonsterTable->GetCount());
		return 0;
	}

	pMonster->SetDestPosition(v2Start);
	InsertMonster( pMonster, v2Start );	
		
	pMonster->Initialize(pBaseMonster, NULL, pUser, (BYTE)dwZipCode, pUser->GetLevel(), 0);
	
	((CUser*)pMonster->GetLord())->m_pMonster[ dwZipCode ]	= pMonster;
	
	// 소환몬스터 능력치를 증가시켜라.
	DWORD dwSummonPassiveSkill[] = {__SKILL_MIRACULOUSHEART__, __SKILL_FRENZY__, __SKILL_RAGE__};

	for(int i = 0; i < sizeof(dwSummonPassiveSkill)/4; ++i)
	{
		BYTE bSkillLevel = pUser->GetSkillLevel((BYTE)dwSummonPassiveSkill[i]);

		if (bSkillLevel)
			pMonster->SetStatusFromSkillStatusValue((BYTE)dwSummonPassiveSkill[i], BYTE(bSkillLevel-1), pUser->GetClass(), NULL);
	}	

	// 몬스터 생성을 알린다.
	DSTC_APPEAR_MON AppearMon;
	MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg( (char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );
	}

	return pMonster;
}

CMonster* CDungeonLayer::CallMonsterByGM( CUser* pUser, DWORD dwZipCode, DWORD dwBaseMonsterNum, DWORD dwItem )
{
	dwZipCode;

	LPBASEMONSTER pBaseMonster = g_pBaseMonsterHash->GetData( dwBaseMonsterNum );
	if(!pBaseMonster)	
	{
		Log( LOG_JUST_DISPLAY, "CallMonster, Invalid BaseMonster Index!" );
		return	NULL;
	}

	CMonster* pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER);
	if(!pMonster)
	{
		Log( LOG_JUST_DISPLAY, "CallMonster, No Free Pool Object!" );
		return NULL;
	}
	
	if (OBJECT_TYPE_GUARDIAN == pBaseMonster->bRace)
	{
		Log( LOG_JUST_DISPLAY, "CallMonster, Don't Call Guardian!" );
		return NULL;
	}

	if (6 == pBaseMonster->bRace)
	{
		// 엘레 멘털이다.
		Log( LOG_JUST_DISPLAY, "CallMonster, Don't Call Elemental!" );
		return NULL;
	}

	pMonster->SetDestPosition(pUser->GetCurPosition());
	InsertMonster( pMonster, pUser->GetCurPosition() );

	pMonster->SetTemp(NPC_TEMP_CREATETYPE, 1);
	pMonster->SetTemp(NPC_TEMP_ITEM, dwItem);

	pMonster->Initialize(pBaseMonster, NULL, NULL, 0, pUser->GetLevel(), 0);
	
	// 일단 주인공의 위치에 겹쳐서 만들자. 귀찮어서리..
	
	pMonster->SetInfoIndex( 0xffff );		// 몬
	
	// 소환몬스터 능력치를 증가시켜라.
	DWORD dwSummonPassiveSkill[] = {__SKILL_MIRACULOUSHEART__, __SKILL_ARCANUMSKIN__, __SKILL_FRENZY__};

	for(int i = 0; i < sizeof(dwSummonPassiveSkill)/4; ++i)
	{
		BYTE bSkillLevel = pUser->GetSkillLevel((BYTE)dwSummonPassiveSkill[i]);

		if (bSkillLevel)
			pMonster->SetStatusFromSkillStatusValue((BYTE)dwSummonPassiveSkill[i], BYTE(bSkillLevel-1)
			, pUser->GetClass(), NULL);
	}
		 
	// 몬스터 생성을 알린다.
	DSTC_APPEAR_MON AppearMon;
	MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg( (char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );
	}

	return pMonster;
}

void CDungeonLayer::InitMoveSpot( DWORD dwMoveSpotNum )
{
	if( dwMoveSpotNum <= 0 ) return;

	if( m_pMoveSpot )
	{
		delete [] m_pMoveSpot;
		m_pMoveSpot = NULL;
	}

	m_wMoveSpotNum = (WORD)dwMoveSpotNum;
	m_pMoveSpot = new Move_Spot[dwMoveSpotNum];
}


// 내 색션에 포함된 파티원들에게 매직 아템을 적용시켜랑.
void CDungeonLayer::AttachMagicArrayToParty(CUser* pUser, DWORD dwItemID)
{
	CSection* pSection = GetSection( pUser->GetPrevSectionNum() );

	if(!pSection)
	{
		return;	
	}

	// 센더의 섹션의 유저에게 메시지전달.
	POSITION_	pos			= NULL;
	CUser*		pTargetUser	= NULL;

	pos = pSection->m_pPcList->GetHeadPosition();

	while(pos)
	{
		pTargetUser = (CUser*)pSection->m_pPcList->GetNext(pos);
		
		if(pTargetUser->m_dwPartyId && pTargetUser->m_dwPartyId == pUser->m_dwPartyId )
		{
			// 같은 파티원이군
			pTargetUser->AttachMagicArray(dwItemID);			
		}			
	}
	
	// 센더의 섹션의 링크된 섹션의 유저에게 메시지 전달.
	CSection* pLinkSection = NULL;
	
	for(int i = 0; pSection->m_wLink[i] != 0; i++)
	{
		pLinkSection = GetSection(pSection->m_wLink[i]);

		if( !pLinkSection )
		{
			continue;
		}

		pos = pLinkSection->m_pPcList->GetHeadPosition();

		while(pos)
		{
			pTargetUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			pTargetUser->AttachMagicArray(dwItemID);			
		}
	}
}


// 스타트 타일주의에 비어있는 타일을 가져와라.
VECTOR2 CDungeonLayer::GetAroundEmptyTile(WORD wIndex_X, WORD wIndex_Z)
{
	VECTOR2 vecDest = {0.f, 0.f};
	int zz = -1;
	int xx = -1;

	for(;;)
	{		
		for(int z = zz; z < -zz; ++z)
		{
			for(int x = xx; x < -xx; ++x)
			{
				MAP_TILE* pTileTemp = m_pMap->GetMap(wIndex_X+x, wIndex_Z+z);

				if (pTileTemp)
				{
					if (pTileTemp->wAttr.uOccupied == TILE_EMPTY &&  !pTileTemp->pData)
					{
						// 나의 위치 재조정.
						vecDest.x = pTileTemp->x+TILE_SIZE/2;
						vecDest.y = pTileTemp->z+TILE_SIZE/2;
						
						return vecDest;
					}
				}				
			}
		}

		zz-=2;
		xx-=2;
	}
}


BOOL CDungeonLayer::GetStartTile(WORD wStartSpotID, VECTOR2* const pv2Start)
{
//	assert(wStartSpotID < m_wStartSpotNum);

	if( !m_wStartSpotNum )
	{
		pv2Start->x = 0;
		pv2Start->y = 0;
		return FALSE;
	}

	// 원하는 스팟 포인트가 없는 것 이면
	if( wStartSpotID >= m_wStartSpotNum)
	{
		// 기본 스팟을 대용하라	
		wStartSpotID = 0;
	}

	MAP_TILE* pTile=0;
	for(int x = m_pStartSpot[ wStartSpotID ].left; x <= m_pStartSpot[ wStartSpotID ].right; x++)
	{
		for(int z = m_pStartSpot[ wStartSpotID ].top; z <= m_pStartSpot[ wStartSpotID ].bottom; z++)
		{
			pTile = GetMap()->GetMap(x, z);
			
			if(!pTile) // 타일이 없으면..
			{
				Log(LOG_IMPORTANT, "Start Spot is invalid range! (x:%d, z:%d)", x, z);
				continue;
			}

			// 빈 타일이 아니면...
			if(pTile->wAttr.uOccupied != TILE_EMPTY || pTile->pData)
				continue;

			pv2Start->x = TILE_X(x);	
			pv2Start->y = TILE_Z(z);

			return TRUE;
		}
	}

	// 빈타일을 찾아서 뿌려라.
	VECTOR2 vec = GetAroundEmptyTile((WORD)m_pStartSpot[ wStartSpotID ].left,(WORD)m_pStartSpot[ wStartSpotID ].bottom);
	*pv2Start = vec;

	return TRUE;
}


void CDungeonLayer::ResetMonster(BOOL bCreateMonster)
{
	// 우선 기존의 몬스터들을 전부 제거한다.	
	POSITION_	MonsterPos	= NULL;	
	CMonster*	pMonster	= NULL;
	
	MonsterPos = m_pMonsterList->GetHeadPosition();

	while( MonsterPos )
	{
		pMonster = (CMonster*)m_pMonsterList->GetNext( MonsterPos );				

		if (pMonster->IsNormalMonster())
		{
			pMonster->SetStatus(UNIT_STATUS_DEAD);
			RemoveMonster(pMonster);
		}		
	}

	m_pDeadMonsterList->RemoveAll();

	// 그리고 다시 생성해 낸다.
	if(bCreateMonster)
	{
		MonsterPos = m_pResponseMonsterList->GetHeadPosition();
		while( MonsterPos )
		{
			pMonster = (CMonster*)m_pResponseMonsterList->GetNext( MonsterPos );	
			CreateMonster( pMonster );
		}
	}
	
	// 일단 짱나더라도 현재 층에 있는 몬스터의 숫를 알아 오라. ㅡ.ㅡ	
	m_wCurMonster = 0;
	POSITION_ pos = m_pMonsterList->GetHeadPosition();
	while(pos)
	{
		CMonster* pMonster = (CMonster*)m_pMonsterList->GetNext(pos);
		
		if (pMonster->IsNormalMonster() )
		{
			m_wCurMonster++;
		}
	}	
}


BOOL CDungeonLayer::RemoveMonster( CMonster* pMonster )
{
	if( !pMonster )
		return false;

	pMonster->RemoveResource();
	RemoveMonsterSection( pMonster );
	
	pMonster->RemoveCurDungeonLayer();
		
	if( pMonster->IsNormalMonster() || pMonster->IsNPC() )
	{	
		if( pMonster->IsMindControll() )
		{
			((CUser*)pMonster->GetLord())->m_pMonster[pMonster->GetTemp(NPC_TEMP_SLOT)] = NULL;
			pMonster->AddResponList();
		}
		else if(pMonster->GetLord())
		{
			// 부하가 죽으면 대장에게 "나 죽었소" 한다. 아메리타트용 : 최덕석 2005.1.26
			// 부하는 보스가 소환하므로 ResponList에 넣지 않음
            pMonster->GetLord()->SetStaff(NULL);
		}
		else {
			// 그 외의 몹들은 리스폰 되기를 대기 : 최덕석 2005.1.26
			pMonster->AddResponList();
		}
	}
	else
	{
		// 가디언과 서모닝 몬스터의 구분이다.
		switch(pMonster->IsGuardian())
		{
		case GUARDIAN_TYPE_NORMAL:	// 주인있는 가디언 가디언은 삭제될 때 Update한다.
			{
				UpdateGuardianDB( pMonster );
				CUser* pMasterUser = ((CUser*)pMonster->GetLord());

				if (pMasterUser)
				{
					pMasterUser->SetGuardian((BYTE)pMonster->GetTemp(NPC_TEMP_SLOT), NULL);
					pMasterUser->UnSummonGuardian((BYTE)pMonster->GetTemp(NPC_TEMP_SLOT));
				}
			}
			break;

		case GUARDIAN_TYPE_DUNGEON:
			m_pParent->SetDungeonGuardian(NULL);
			break;
		}

		if (pMonster->IsElemental())
		{
			// 서머닝 몬스터
			if (pMonster->GetTemp(NPC_TEMP_SLOT) < MAX_USER_GUARDIAN)
			{
				((CUser*)pMonster->GetLord())->m_pMonster[pMonster->GetTemp(NPC_TEMP_SLOT)] = NULL;
			}
		}

		DSTC_DISAPPEAR_MON	DisAppear;
		DisAppear.dwMonsterIndex = pMonster->GetID();
		CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
		if(pLayer)
		{
			pLayer->BroadCastSectionMsg((char*)&DisAppear, DisAppear.GetPacketSize(), pMonster->GetPrevSectionNum());
		}

		g_pMonsterTable->Remove( pMonster );
	}

	return true;
}


void CDungeonLayer::__ProcessTeamMatch()
{//시작 명령을 내리면 시간만 셋팅 된다
	if( m_pTeamMatch )
	{
		m_pTeamMatch->Process();//내부에서 OnEnd에서 월드에 보고하라.
	}
}

//팀매치 중인가(아래 TeamMatch 붙은 함수 전부 : hwoarang 050202
bool CDungeonLayer::IsTeamMatchRun() const 
{
	if( m_pTeamMatch ) 
	{
		return m_pTeamMatch->IsRun();
	}
	return false;
}

bool CDungeonLayer::IsTeamMatchEnd() const//팀매치 끝났냐?
{
//	if( m_dwTeamMatchEndTime <= g_dwTickCount)
//	{
//		return true;
//	}
	return false;
}

bool CDungeonLayer::OnDie(CUser* pCaster, CUser* pTarget)//사망시
{
	pTarget->OnDie();
	if(m_pTeamMatch)
	{
		return m_pTeamMatch->OnDie( pCaster, pTarget );//죽을때 false 로 리턴하면 죽이지 않는다.
	}
	return true;
}

bool CDungeonLayer::PrepareTeamMatch(const FLAG_TYPE match_type, const FLAG_TYPE match_level,  const COUNT_TYPE player_count, const COUNT_TYPE team_count, const GUILD_WAR_TIME match_time, const bool bIsAutoMatch)
{//Prepare오더를 받았으므로 대기시간을 주고 유저는 이동을 못하게 한다.
	if(m_pTeamMatch)
	{
		m_pTeamMatch->Prepare(match_type, match_level, player_count, team_count, match_time, bIsAutoMatch);
	}
	return false;
}

bool CDungeonLayer::IsTeamMatchPlayer(CUser* pUser) const
{

	/*
	if(m_pTeamMatch)
	{
<<<<<<< DungeonLayer.cpp
		return (bool)(G_W_F_PLAYER & m_pTeamMatch->GetTeamMatchFlag( pUser ) ) ;
=======
		return ((G_W_F_PLAYER & m_pTeamMatch->GetTeamMatchFlag( pUser )) == G_W_F_PLAYER) ? true : false;
>>>>>>> 1.18
	}
	*/

	
	// 재원씨 요청에 의한수정, 4월 6일, PM 6:52, 위의 주석구문은 컴파일오류 난 부분임.-------------------
	if(m_pTeamMatch) 
	{
		return ((G_W_F_PLAYER & m_pTeamMatch->GetTeamMatchFlag( pUser )) == G_W_F_PLAYER) ? true : false;
	}
	// --------------------------------------------------------------------------------------------------


	return false;//팀매치 맵 아니면 플레이어 일 수가 없다.
}

bool CDungeonLayer::CanMove()//이동시 가능 불가능
{
	if( m_pTeamMatch )
	{
		return m_pTeamMatch->CanMove();
	}
	return true;
}