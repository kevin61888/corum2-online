#pragma once

#pragma warning(disable:4786)
#include <windows.h>
#include <string>
#include <map>
using namespace std;

namespace LOCAL_MGR
{
	typedef enum eVersionCode
	{
		VER_NOT_SET		= 5200,
		VER_KOREA		= 5200,
		VER_TAIWAN		= 3200,
		VER_HONGKONG	= 4200,
		VER_CHINA		= 5200,
		VER_THAI		= 6200,
		VER_USA			= 7200,	
		VER_JAPAN		= 8200,
	}VERSION_CODE;

	typedef enum eNationCode
	{
		NC_NOT_SET	= 0x00000000,//NOTSET일 경우는 무조건 에러처리합니다.
		NC_KOREA	= 0x00000001,
		NC_CHINA	= 0x00000002,
		NC_TAIWAN	= 0x00000004,
		NC_THAI		= 0x00000008,
		NC_HONGKONG	= 0x00000010,//16
		NC_USA		= 0x00000020,//32
		NC_JAPAN	= 0x00000040,//64
	}NATION_CODE;

	typedef enum eServiceType
	{
		ST_NOT_SET			= 0x00000000,
		ST_DEVELOP			= 0x00000001,//		//개발자 모드

		ST_BILLING			= 0x00000002,//		//시간제 과금함
		ST_OTHER_1			= 0x00000004,//		//--x 
		ST_CHINA_BILLING	= 0x00000008,//		//중국 과금

		ST_NETMARBLE		= 0x00000010,//16	//NET_MARBLE_LOGIN
		ST_FREE				= 0x00000020,//32	//과금 안함
		ST_NO_LOG			= 0x00000040,//32	//로그 안남김
	}SERVICE_TYPE;

	class CLocal  
	{
	public:
		CLocal();
		virtual ~CLocal();
	public://SetNationCode 류의 함수는 만들지 않습니다. 게임 서비스 중에 쓸일이 없기 때문입니다.
		NATION_CODE	NationCode()const {return m_eNationCode;}
		VERSION_CODE VersionCode()const {return m_eVersionCode;}
		DWORD ServiceType()const {return m_dwServiceTypeFlag;}
		const char* NationName() const {return m_strNationName.c_str();}

		bool LoadFromINI(const char* szINIFileName);

		void Clear();
		bool Init(const string& strNationName, const DWORD dwServiceTypeFlag);
		bool Init(const NATION_CODE eNC, const DWORD dwServiceTypeFlag);

		bool IsAbleVersion(const VERSION_CODE eVC) const;//불가능한 마이코드라면 0을 리턴 가능하면 1을 리턴
		bool IsAbleNation(const DWORD dwFlag) const;//불가능한 국가라면 0을 리턴 가능하면 1을 리턴
		bool IsAbleServiceType(const DWORD dwFlag) const;
/*	
		void example()//IsAbleNation()의 샘플 코드
		{
			if(IsAbleNation( KOREA | TAIWAN )
			{//한국이거나 대만이면 실행 됩니다.
			}
			else
			{//나머지 국가에서 실행 됩니다.
			}
			
			if(IsAbleServiceType( ST_DEVELOP | ST_COMMERCIAL )//개발용 이거나 상용 일때
			{

			}
		}
*/
	private:
		void NationCode(const NATION_CODE eNC){m_eNationCode = eNC;}
		void VersionCode(const VERSION_CODE eVC){m_eVersionCode = eVC;}
		void ServiceType(const DWORD dwST){m_dwServiceTypeFlag = dwST;}
		void NationName(const string& strNationName) {m_strNationName = strNationName;}

	private:
		NATION_CODE		ConvertNameToCode(const string& strNationName);
		NATION_CODE		m_eNationCode;
		VERSION_CODE	m_eVersionCode;
		DWORD			m_dwServiceTypeFlag;
		string			m_strNationName;

	private:
		CLocal operator=(const CLocal &old);//대입연산자 사용 불가.
		CLocal(const CLocal &old);//복사생성자 사용 불가.

	};
};

namespace DBINFO_MGR
{
	typedef enum eDatabaseType //DB타입
	{
		DT_NOT_SET		= 0,
		DT_TOTAL_DB		= 1,
		DT_GAME_DB		= 2,	
		DT_LOG_DB		= 3,
		DT_MEMBER_DB	= 4,
		DT_OTHER_DB2	= 5,
		DT_OTHER_DB3	= 6,
	}DATABASE_TYPE;

	typedef enum eInfoType
	{
		DBIT_SOURCE		= 0,
		DBIT_CATALOG	= 1,
		DBIT_ID			= 2,
		DBIT_PW			= 3,
		DBIT_MAX		= 4,
	}INFO_TYPE;

	class CDBInfoMgr
	{
	public:
		CDBInfoMgr();
		virtual ~CDBInfoMgr();

	private:
		typedef map< INFO_TYPE, string > INFO_HASH;
		typedef INFO_HASH::iterator INFO_ITOR;

		typedef map< DATABASE_TYPE, INFO_HASH* > DB_HASH;
		typedef DB_HASH::iterator DB_ITOR;

	private:
		DB_HASH m_mapInfo;

	public:
		const char* Get(const DATABASE_TYPE eDT, const INFO_TYPE eIT);
		void Set(const DATABASE_TYPE eDT, const INFO_TYPE eIT, const string &strValue);
		void Clear();
	};

};
using namespace LOCAL_MGR;
using namespace DBINFO_MGR;
