#pragma once


class CMonsterList;
class CPcList;
class CActiveSectionList;
class CDungeonList;
class CCreateItemList;
class CDungeon;
class CDungeonList;


struct FunctionTableSymbol
{
	CMonsterList*			pMonsterList;
	CPcList*				pPcList;	
	CActiveSectionList*		pActiveSectionList;
	CDungeonList*			pDungeonList;
	CCreateItemList*		pCreateItemList;
};

class CDungeonTable
{	
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwNum;				//Global User Count
	CDungeon**				m_ppInfoTable;

	void					RemoveDungeonTable();

	FunctionTableSymbol		m_vfptr;
			
public:	

	CDungeonList*			m_pDungeonList;			//총 던전 리스트(일반) 
	
	FunctionTableSymbol*	GetVfptr()	{	return &m_vfptr;	}
	
	CDungeon*				CreateNewDungeon(WORD wID);
	CDungeon*				GetDungeonInfo(WORD wId);

	BOOL					Add(CDungeon* pDungeon);
	void					Remove(CDungeon* pDungeon);
	void					BroadCast(char* pPacket, DWORD dwLength);
	void					ShowAllDungeonStatus();
	DWORD					GetCount()	{	return m_dwNum;	}	

	CDungeonTable(DWORD dwNum);
	~CDungeonTable();
};


