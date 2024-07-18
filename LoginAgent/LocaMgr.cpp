// LocalizingMgr.cpp: implementation of the CLocal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LocalMgr.h"

#define SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);		(p)=NULL; } }

//using namespace LOCAL_MGR;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
const char szKorea[]	= "KOREA";
const char szChina[]	= "CHINA";
const char szTaiwan[]	= "TAIWAN";
const char szThai[]		= "THAI";
const char szHongkong[] = "HONGKONG";
const char szUsa[]		= "USA";
const char szJapan[]	= "JAPAN";

//INI info
const char szRoot[] = "LOCAL_INFO";
const char szElement_NN[] = "NATION_NAME";
const char szElement_ST[] = "SERVICE_TYPE";

CLocal::CLocal()
{
	Clear();
}

CLocal::~CLocal()
{
	Clear();
}

void CLocal::Clear()
{
	NationCode(NC_NOT_SET);
	VersionCode(VER_NOT_SET);
	ServiceType(ST_NOT_SET);
	NationName("");
}

bool CLocal::LoadFromINI(const char* szINIFileName)
{
	char szBuf[MAX_PATH] ={0,};
	::GetPrivateProfileString(szRoot, szElement_NN, "", szBuf, sizeof(szBuf), szINIFileName);	
	DWORD dwST = ::GetPrivateProfileInt(szRoot, szElement_ST, ST_NOT_SET, szINIFileName);
//	서비스 타입은 DWORD로 처리하는게 낫겠다.
	return Init(szBuf, dwST);
}

bool CLocal::Init(const string& strNationName, const DWORD dwServiceTypeFlag)
{
	return Init(ConvertNameToCode(strNationName), dwServiceTypeFlag);
}

bool CLocal::Init(const NATION_CODE eNC, const DWORD dwServiceTypeFlag)
{
	switch(eNC)
	{
	case NC_KOREA:
		{
			NationName(szKorea);
			VersionCode(VER_KOREA);
		}break;
	case NC_CHINA:
		{	
			NationName(szChina);
			VersionCode(VER_CHINA);
		}break;
	case NC_TAIWAN:
		{
			NationName(szTaiwan);
			VersionCode(VER_TAIWAN);
		}break;
	case NC_THAI:
		{
			NationName(szThai);
			VersionCode(VER_THAI);
		}break;
	case NC_HONGKONG:
		{
			NationName(szHongkong);
			VersionCode(VER_HONGKONG);
		}break;
	case NC_USA:
		{
			NationName(szUsa);
			VersionCode(VER_USA);
		}break;
	case NC_JAPAN:
		{
			NationName(szJapan);
			VersionCode(VER_JAPAN);
		}break;
	case NC_NOT_SET:
	default:
		{
			Clear();
			return false;
		}break;
	}
	NationCode(eNC);
	ServiceType(dwServiceTypeFlag);

	return 1;
}

eNationCode CLocal::ConvertNameToCode(const string& strNationName)
{
	if(!::stricmp(strNationName.c_str(), szKorea)){return NC_KOREA;}
	if(!::stricmp(strNationName.c_str(), szChina)){return NC_CHINA;}
	if(!::stricmp(strNationName.c_str(), szTaiwan)){return NC_TAIWAN;}
	if(!::stricmp(strNationName.c_str(), szThai)){return NC_THAI;}
	if(!::stricmp(strNationName.c_str(), szHongkong)){return NC_HONGKONG;}
	if(!::stricmp(strNationName.c_str(), szUsa)){return NC_USA;}
	if(!::stricmp(strNationName.c_str(), szJapan)){return NC_JAPAN;}
	
	return NC_NOT_SET;	
}

bool CLocal::IsAbleNation(const DWORD dwFlag) const//불가능한 국가라면 0을 리턴 가능하면 1을 리턴
{
	return (( NationCode() & dwFlag )?true:false);
}

bool CLocal::IsAbleVersion(const VERSION_CODE eVC) const//불가능한 마이코드라면 0을 리턴 가능하면 1을 리턴
{
	return (( VersionCode() == eVC )?true:false);
}

bool CLocal::IsAbleServiceType(const DWORD dwFlag) const
{
	return (( ServiceType() & dwFlag )?true:false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CDBInfoMgr::CDBInfoMgr()
{
	Clear();
}

CDBInfoMgr::~CDBInfoMgr()
{
	Clear();
}

const char* CDBInfoMgr::Get(const DATABASE_TYPE eDT, const INFO_TYPE eIT)
{
	DB_ITOR itor = m_mapInfo.find(eDT);
	if( m_mapInfo.end() != itor)
	{
		INFO_HASH &rInfo = *((*itor).second);

		INFO_ITOR in_itor = rInfo.find(eIT);
		if(rInfo.end() != in_itor)
		{
			return ((*in_itor).second).c_str();
		}
	}

	return NULL;
}

void CDBInfoMgr::Set(const DATABASE_TYPE eDT, const INFO_TYPE eIT, const string &strValue)
{
	DB_ITOR itor = m_mapInfo.find(eDT);
	INFO_HASH *pInfo = NULL;
	if(m_mapInfo.end() != itor)
	{
		pInfo = (*itor).second;
	}
	else
	{
		pInfo = new INFO_HASH;
		m_mapInfo.insert( make_pair(eDT, pInfo) );
	}

	INFO_ITOR info_itor = pInfo->find(eIT);

	if(pInfo->end() != info_itor)
	{
		string &rstrValue =  (*info_itor).second;
		rstrValue = strValue;
	}
	else
	{
		pInfo->insert( make_pair(eIT, strValue) );
	}
}

void CDBInfoMgr::Clear()
{
	DB_ITOR itor = m_mapInfo.begin();
	while(m_mapInfo.end() != itor)
	{
		INFO_HASH *pInfo = (*itor).second;
		SAFE_DELETE(pInfo);
		++itor;
	}
}