#include "Stdafx.h"
#include "MonsterTable.h"
#include "Monster.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"


CMonsterTable::CMonsterTable(DWORD dwNum)
{
	memset(this, 0, sizeof(CMonsterTable));

	m_dwMaxBucketNum = dwNum;

	m_ppInfoTable = NULL;
	m_ppInfoTable = new CMonster*[ dwNum ];		
	memset(m_ppInfoTable, 0, sizeof(CMonster*)*dwNum);
}

BOOL CMonsterTable::IsCollisionID(DWORD dwID)
{
	if (!dwID)	return TRUE;

	DWORD dwIndex = dwID%m_dwMaxBucketNum;
	CMonster* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->GetID() == dwID)
			return TRUE;
		cur = cur->m_pNextData;
	}
	return FALSE;
}

CMonster* CMonsterTable::Add(UNIT_CLASS_ID eUnitClassID)
{
	assert(eUnitClassID == UNIT_CLASS_ID_MONSTER || eUnitClassID == UNIT_CLASS_ID_TRADER_MONSTER);

	CMonster* pMonster = (CMonster*)CreateUnitInstance(eUnitClassID);
	if (!pMonster)	return NULL;	
	
	// 충돌되는 몬스터번호를 위해.
	while(IsCollisionID(m_dwNum))
		m_dwNum++;
	
	DWORD dwIndex = m_dwNum % m_dwMaxBucketNum;	
	pMonster->SetID( m_dwNum );			// 아이디 넣어주고.
	m_dwNum++;							// 다음 아이디로 하나 플러스 해주고.

	if (!m_ppInfoTable[dwIndex])	//m_ppInfoTable[dwIndex] 에 내용이 없으면 괄호 안으로.
	{
		m_ppInfoTable[dwIndex] = pMonster;
		pMonster->m_pNextData = NULL;
		pMonster->m_pPrevData = NULL;
		return pMonster;
	}

	CMonster* cur = m_ppInfoTable[dwIndex];
	CMonster* prv = NULL;
	while (cur)
	{
		prv = cur;
		cur = cur->m_pNextData;
	}
	
	cur = prv->m_pNextData = pMonster;
	cur->m_pPrevData = prv;
	cur->m_pNextData = NULL;

	return pMonster;
}

CMonster* CMonsterTable::GetMonster(DWORD dwID)
{
	DWORD dwIndex = dwID%m_dwMaxBucketNum;
	CMonster* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->GetID() == dwID)
		{
			if( cur->GetUnitStatus() == UNIT_STATUS_DEAD )
				return NULL;
			
			return cur;
		}
		cur = cur->m_pNextData;
	}
	return NULL;
}

CMonster* CMonsterTable::GetMonsterABS(DWORD dwID)
{
	DWORD dwIndex = dwID%m_dwMaxBucketNum;
	CMonster* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->GetID() == dwID)
		{		
			return cur;
		}
		cur = cur->m_pNextData;
	}
	return NULL;
}

void CMonsterTable::Remove(CMonster* pMonster)
{
	DWORD dwIndex = pMonster->GetID()%m_dwMaxBucketNum;

	CMonster* cur = m_ppInfoTable[dwIndex];
	CMonster* next = NULL;
	CMonster* prv = NULL;
	
	while (cur)
	{
		prv = cur->m_pPrevData;
		next = cur->m_pNextData;
		if (cur->GetID() == pMonster->GetID())
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->m_pNextData = next;
			
			if (next)
				next->m_pPrevData = prv;

			if(cur)	
				delete cur;
										
			cur = NULL;
			break;
		}
		cur = cur->m_pNextData;
	}
}

void CMonsterTable::RemoveMonsterTable()
{
	CMonster* cur = NULL;
	CMonster* next = NULL;
	for (DWORD i=0; i<m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];
		while (cur)
		{
			next = cur->m_pNextData;
			delete cur;
							
			cur = next;
		}
		m_ppInfoTable[i] = NULL;
	}
}

CMonsterTable::~CMonsterTable()
{
	RemoveMonsterTable();

	if (m_ppInfoTable)
	{
		delete [] m_ppInfoTable;
		m_ppInfoTable = NULL;
	}
}