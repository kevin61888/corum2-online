#include "StdAfx.h"
#include "MonsterManager.h"

CMonsterManager::CMonsterManager()
{
	memset( this, 0, sizeof( CMonsterManager ) );

	//Modified by KBS 030107
	m_pBaseMonsterHash = new BASEMONSTER_HASH;
	m_pBaseMonsterHash->InitializeHashTable( 500, 500 );
	//--

}

CMonsterManager::~CMonsterManager()
{
	Destroy();
}

void CMonsterManager::Destroy()
{
	if( m_pBaseMonsterHash )
	{
		m_pBaseMonsterHash->Destroy( TRUE );
		delete m_pBaseMonsterHash;
		m_pBaseMonsterHash = NULL;
	}
}

LPBASEMONSTER CMonsterManager::GetMonster( DWORD dwItemID )
{

	return NULL;
}

/*BOOL CMonsterManager::Parse_BaseMonster( char* szFileName )
{
	int i;
	FILE*	fp;
	fp = fopen( szFileName, "rt" );
	if( !fp ) return FALSE;

	char			szLine[1000] = {0,}; 
	LPBASEMONSTER	pBaseMonster;
	char* tok;
	char chk[] = ",";

	m_pBaseMonsterHash->Destroy( TRUE );

	while(1)
	{
		memset( szLine, 0, 1000 );
		if( fgets(szLine, 1000, fp ) == NULL ) break;
		pBaseMonster = new BASEMONSTER;
		memset( pBaseMonster, 0, sizeof( BASEMONSTER ) );

		tok = strtok( szLine, chk );					// ID
		pBaseMonster->dwID = atoi( tok );
		
		tok = strtok( NULL, chk );
		__lstrcpyn( pBaseMonster->szMonsterName_Kor, tok, sizeof(tok) );	// Kor_Name
		
		tok = strtok( NULL, chk );
		__lstrcpyn( pBaseMonster->szMonsterName_Eng, tok, sizeof(tok) ); // Eng_Name

		tok = strtok( NULL, chk );						// Rank
		pBaseMonster->bRank = 0;
		for( i=0; i<MAX_MONSTER_RANK; i++ )
		{
			if( _stricmp( tok, szRank[i] ) == 0 )
			{
				pBaseMonster->bRank = i;
				break;
			}
		}
		
		tok = strtok( NULL, chk );						// type
		pBaseMonster->bType = 0;
		for( i=0; i<MAX_MONSTER_TYPE; i++ )
		{
			if( _stricmp( tok, szMonsterType[i] ) == 0 )
			{
				pBaseMonster->bType = i;
				break;
			}
		}
		
		tok = strtok( NULL, chk );						// size
		pBaseMonster->bSize = 0;
		for( i=0; i<MAX_MONSTER_SIZE; i++ )
		{
			if( _stricmp( tok, szMonsterSize[i] ) == 0 )
			{
				pBaseMonster->bSize = i;
				break;
			}
		}

		tok = strtok( NULL, chk );						// Level
		pBaseMonster->bLevel = atoi( tok );

		tok = strtok( NULL, chk );						// HP
		pBaseMonster->dwHP	= atoi( tok );
		
		tok = strtok( NULL, chk );
		pBaseMonster->dwDefense = atoi( tok );			// Defense

		tok = strtok( NULL, chk );
		pBaseMonster->bBlock = atoi( tok );				// Block
		
		tok = strtok( NULL, chk );
		pBaseMonster->dwExp = atoi( tok );				// Exp

		tok = strtok( NULL, chk );
		pBaseMonster->wA_Damage = atoi( tok );			// Attack Damage

		tok = strtok( NULL, chk );
		pBaseMonster->fA_Range = (float)atof( tok );			// Attack_Range

		tok = strtok( NULL, chk );
		pBaseMonster->fA_Speed = (float)atof( tok );			// Attack_Speed

		tok = strtok( NULL, chk );
		pBaseMonster->fMonsterSpeed = (float)atof( tok );		// Monster_Speed
	
		// resist
		tok = strtok( NULL, chk );
		pBaseMonster->bFire_resist = atoi( tok );		// Fire_resist
		
		tok = strtok( NULL, chk );
		pBaseMonster->bIce_resist = atoi( tok );		// Ice_resist
		
		tok = strtok( NULL, chk );
		pBaseMonster->bPhy_resist = atoi( tok );		// Phy_resist
		
		tok = strtok( NULL, chk );
		pBaseMonster->bLight_resit = atoi( tok );		// Light_resit
		
		tok = strtok( NULL, chk );
		pBaseMonster->bPoi_resist = atoi( tok );		// Poi_resist
		
		// Skills.
		for( i=0; i<3; i++ )
		{
			tok = strtok( NULL, chk );
			pBaseMonster->bSkill[i] = atoi( tok );
			
			tok = strtok( NULL, chk );
			pBaseMonster->bSkillLevel[i] = atoi( tok );
		}
		
		tok = strtok( NULL, chk );
		pBaseMonster->bTargetSize = atoi( tok );		// TargetSize

		tok = strtok( NULL, chk );
		pBaseMonster->bAI = 0;							// AI
		for( i=0; i<MAX_MONSTER_AI; i++ )
		{
			if( _stricmp( tok, szAI[i] ) == 0 )
			{
				pBaseMonster->bAI = i;
				break;
			}
		}

		tok = strtok( NULL, chk );
		pBaseMonster->fSigth = (float)atof( tok );				// Sight

		tok = strtok( NULL, chk );
		pBaseMonster->bSpaceKeeping = atoi( tok );		// Keeping

		// Insert.......
		if( m_pBaseMonsterHash->Insert( pBaseMonster, pBaseMonster->dwID ) == -1 )
		{
			delete pBaseMonster;
			pBaseMonster = NULL;
		}
	}
	
	m_dwMonsterNum = m_pBaseMonsterHash->GetNodeNum();
	fclose( fp );

	return TRUE;
}*/

BOOL CMonsterManager::Save_BaseMonster( char* szFileName )
{
	FILE *fp;
	DWORD dwType = ITI_TYPE_BASEMONSTER;

	fp = fopen( szFileName, "wb" );
	if( !fp ) return FALSE;

	fwrite( &dwType, sizeof( DWORD ), 1, fp );
	fwrite( &m_dwMonsterNum, sizeof( DWORD ), 1, fp );

	ListNode<BASEMONSTER>* pNode = m_pBaseMonsterHash->GetHead();	//Modified by KBS 030107
	while( pNode )
	{
		fwrite( pNode->pData, sizeof( BASEMONSTER ), 1, fp );
		pNode = pNode->pNext;
	}
	
	fclose( fp );
	return TRUE;
}

BOOL CMonsterManager::Load_BaseMonster( char* szFileName )
{
	FILE*			fp;
	DWORD			i, dwType;
	LPBASEMONSTER	pBaseMonster;

	fp = fopen( szFileName, "rb" );
	if( !fp ) return FALSE;
	
	fread( &dwType, sizeof( DWORD ), 1, fp );
	if( dwType != ITI_TYPE_BASEMONSTER )
	{
		fclose( fp );
		return FALSE;
	}

	m_pBaseMonsterHash->Destroy( TRUE );
	fread( &m_dwMonsterNum, sizeof( DWORD ), 1, fp );
	
	for( i=0; i<m_dwMonsterNum; i++ )
	{
		pBaseMonster = new BASEMONSTER;
		fread( pBaseMonster, sizeof( BASEMONSTER ), 1, fp );
		if( !m_pBaseMonsterHash->Insert( pBaseMonster, pBaseMonster->dwID ) )
		{
			delete pBaseMonster;
			pBaseMonster = NULL;
		}
	}
	
	fclose( fp );
	return TRUE;
}