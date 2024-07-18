#include "StdAfx.h"
#include "MonsterManager.h"

CMonsterManager::CMonsterManager()
{
	memset( this, 0, sizeof( CMonsterManager ) );

	m_pBaseMonsterHash = new BASEMONSTER_HASH;
	m_pBaseMonsterHash->InitializeHashTable( 2000, 1000 );
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
	return m_pBaseMonsterHash->GetData( dwItemID );
}

BOOL CMonsterManager::Save_BaseMonster( char* szFileName )
{
	FILE *fp=0;
	DWORD dwType = ITI_TYPE_BASEMONSTER;

	fp = fopen( szFileName, "wb" );
	if( !fp ) return FALSE;

	fwrite( &dwType, sizeof( DWORD ), 1, fp );
	fwrite( &m_dwMonsterNum, sizeof( DWORD ), 1, fp );

	ListNode<BASEMONSTER>* pNode = m_pBaseMonsterHash->GetHead();
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
	FILE*			fp=0;
	DWORD			i=0, dwType=0;
	LPBASEMONSTER	pBaseMonster=0;

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