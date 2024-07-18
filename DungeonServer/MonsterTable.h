#pragma once


class CMonster;


class CMonsterTable
{
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwNum;					//Global Monster Count
	CMonster**				m_ppInfoTable;

	void					RemoveMonsterTable();
	BOOL					IsCollisionID(DWORD dwID);

public:	

	CMonster*				GetMonster(DWORD dwId);
	CMonster*				GetMonsterABS(DWORD dwID);
	CMonster*				Add(enum UNIT_CLASS_ID eUnitClassID);
	void					Remove(CMonster* pMonster);
	DWORD					GetCount()	{	return m_dwNum;	}
	void					SetStaticMonsterCount(DWORD dwStaticMonsterCount);

	CMonsterTable(DWORD dwNum);
	~CMonsterTable();
};


