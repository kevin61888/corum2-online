#pragma once


class CDungeon;
class CUser;
struct ITEM_SERIAL;
class DUNGEON_DATA_EX;
class CVoidList;


//현재 서버정보 클래스 
class COwnServer
{
	BYTE				m_bServerType;								//Server Type
	DWORD				m_dwServerID;								//Server ID
	DWORD				m_dwServerSetNum;							//ServerSet Number	
	DWORD				m_dwDevServerSetNum;
	ITEM_SERIAL*		m_pSerial;	
	DWORD				m_dwAllowedMaxUserNum;						//이서버가 허용하는 유저 접속수 
	BOOL				m_bIsUserAcceptAllowed;						//유저 접속 허용 여부 
	DWORD				m_dwTotalDungeonNum;						//이 서버가 가지고 있는 총 던전 갯수 
	DUNGEON_DATA_EX*	m_pDungeonInfo;								//이서버에 해당하는 던전 ID들                        
	DWORD				m_dwConnectionIndexForWorldServer;			//WorldServer ConnectionIndex;
	char				m_szIpForUser[ MAX_IP_LENGTH ];				//User Listen용 IP
	char				m_szIpForServer[ MAX_IP_LENGTH ];			//Server Listen용 IP
	char				m_szIpForWorldServer[ MAX_IP_LENGTH ];		//WorldServer IP
	char				m_szIpForExtern[ MAX_IP_LENGTH ];			// 대만은 내부 아이피로 유저리슨을 열기때문에 외부 아이피정보를 따로 가지고 있어야한다. 
	char				m_szServerDataPath[ _MAX_PATH ];			//Dungeon Server Data Path
	WORD				m_wPortForUser;								//Port for User
	WORD				m_wPortForServer;							//Port for Server
	WORD				m_wPortForWorldServer;						//Port for WorldServer
	WORD				m_wLogColor;								//Log출력시 배경 색깔 
	CRITICAL_SECTION	m_csUserAccept;
	WORD				m_wMAX_USER_CONNECTION;
	WORD				m_wHASH_SIZE_USER;
	WORD				m_wPOOL_SIZE_MONSTER;
	WORD				m_wPOOL_SIZE_USER;
	WORD				m_wPOOL_SIZE_ITEMTRADE;
	WORD				m_wPOOL_SIZE_ITEMSTORE;
	WORD				m_wPOOL_SIZE_ITEMSHOP;
	WORD				m_wPOOL_SIZE_ITEMTILE;
	WORD				m_wPOOL_SIZE_GUILDLIST;
	WORD				m_wPOOL_SIZE_SHOPLOG;
	DWORD				m_dwPOOL_SIZE_NODE;
	WORD				m_wPOOL_SIZE_ITEM;
	WORD				m_wPOOL_SIZE_CREATEITEM;
	WORD				m_wPOOL_SIZE_PORTAL;
	WORD				m_wPOOL_SIZE_TRADER_MONSTER;
	WORD				m_wHASH_SIZE_SETITEMINFO;
	WORD				m_wHASH_SIZE_MAKEITEMINFO;
	WORD				m_wHASH_SIZE_BASEITEM;
	WORD				m_wHASH_SIZE_ITEMTRADE;
	WORD				m_wHASH_SIZE_ITEMSHOP;
	WORD				m_wHASH_SIZE_SHOPLOG;
	WORD				m_wHASH_SIZE_GUILDLIST;
	WORD				m_wHASH_SIZE_ITEMTILE;
	WORD				m_wHASH_SIZE_ITEMSTORE;
	WORD				m_wHASH_SIZE_BASEMONSTER;
	WORD				m_wHASH_SIZE_SETITEM;
	WORD				m_wHASH_SIZE_INVALIDLOG;
	WORD				m_wHASH_SIZE_STORENATIVE;
	DWORD				m_dwMainMsgQueMaxBufferSize;
	DWORD				m_dwMaxServerNum;
	DWORD				m_dwServerBufferSizePerConnection;
	DWORD				m_dwServerMaxTransferSize;
	DWORD				m_dwUserBufferSizePerConnection;
	DWORD				m_dwUserMaxTransferSize;
	BOOL				LoadConfigFile(char* pServerConfigInfoFile);

public:

	DWORD				m_dwGLOBAL_PARTY_BONUS_EXP_GAIN;			// 이벤트로 인한 파티 보너스 경험치 (절대 보너스 경험치만이다) 늘리는 변수
	DWORD				m_dwGLOBAL_EXP_GAIN;						// 이벤트로 인한 경험치 늘리는 변수.
	DWORD				m_dwGLOBAL_KARZ_GAIN;						// 이벤트로 인한 카르츠 늘리는 변수.
	DWORD				m_GLOBAL_MAGIC_FIND_PBT;					// 이벤트로 인한 마법 떨구는 확률
	BOOL				m_GLOBAL_EXP_GAIN_DOUBLE;					// 이벤트로 인한 경험치 2배를 하지 말까?
	BYTE				m_bAdultMode;								// 성인 서버
	HANDLE				m_hKeyEvent[10];							//KeyEvent;	
	CVoidList*			m_pPKUserCheckList;							// pk카운트가 있는놈들만 추가 되는 리스트다.

	COwnServer();
	~COwnServer();	

	BOOL				Initialize(char* pServerInfoFile, char* pServerConfigInfoFile);
	BOOL				InitializeDecodeCFG(char* pServerInfoFile);
	BOOL				GetLocalAddress(char* OUT szIp);	
	BOOL				ToggleUserAccept();
	BOOL				IsUserAcceptAllowed()				{	return m_bIsUserAcceptAllowed;	}	
	BYTE				GetServerType()	const				{	return m_bServerType;		}
	DWORD				GetServerID() const					{	return m_dwServerID;		}
	DWORD				GetServerSetNum() const				{	return m_dwServerSetNum;	}
	DWORD				GetDevModeServerSetNum() const		{   return m_dwDevServerSetNum; }
	DWORD				GetTotalDungeonNum() const			{	return m_dwTotalDungeonNum;	}
	DWORD				GetWSConnectionIndex() const		{	return m_dwConnectionIndexForWorldServer; }		
	WORD				GetPortForUser() const				{	return m_wPortForUser;		}
	WORD				GetPortForServer() const			{	return m_wPortForServer;	}
	WORD				GetPortForWorldServer() const		{	return m_wPortForWorldServer;	}
	const char*			GetIPForUser()						{	return m_szIpForUser;		}
	const char*			GetIPForServer()					{	return m_szIpForServer;		}
	const char*			GetIPForWorldServer()				{	return m_szIpForWorldServer;}
	const char*			GetIPForExtern()					{	return m_szIpForExtern;		}
	const char*			GetServerDataPath()					{	return m_szServerDataPath;	}
	WORD 				GetLogColor()	const				{	return m_wLogColor;			}
	void				SetLogColor(WORD wColor)			{	m_wLogColor = wColor;		}	
	BOOL				RequestDungeonInfo();
	BOOL				PreLoadDungeon();
	BOOL				ResetItemSerialNumber();	
	ITEM_SERIAL*		GetNewSerial();
	void				SetWSConnectionIndex(DWORD dwIndex)	{	m_dwConnectionIndexForWorldServer = dwIndex;	}
	WORD				GetMaxUserConnection() const		{	return m_wMAX_USER_CONNECTION;	}
	WORD				GetHashSizeUser() const				{	return m_wHASH_SIZE_USER;		}
	WORD				GetPoolSizeMonster() const			{	return m_wPOOL_SIZE_MONSTER;	}
	WORD				GetPoolSizeTraderMonster() const	{	return m_wPOOL_SIZE_TRADER_MONSTER;		}
	WORD				GetPoolSizeUser() const				{	return m_wPOOL_SIZE_USER;		}
	WORD				GetPoolSizeItemTrade() const		{	return m_wPOOL_SIZE_ITEMTRADE;	}
	WORD				GetPoolSizeItemStore() const		{	return m_wPOOL_SIZE_ITEMSTORE;	}
	WORD				GetPoolSizeItemShop() const			{	return m_wPOOL_SIZE_ITEMSHOP;	}
	WORD				GetPoolSizeItemTitle() const		{	return m_wPOOL_SIZE_ITEMTILE;	}
	WORD				GetPoolSizeGuildList() const		{	return m_wPOOL_SIZE_GUILDLIST;	}
	WORD				GetPoolSizeShopLog() const			{	return m_wPOOL_SIZE_SHOPLOG;	}
	DWORD				GetPoolSizeNode() const				{	return m_dwPOOL_SIZE_NODE;		}
	WORD				GetPoolSizeItem() const				{	return m_wPOOL_SIZE_ITEM;		}
	WORD				GetPoolSizeCreateItem() const		{	return m_wPOOL_SIZE_CREATEITEM;	}
	WORD				GetPoolSizePortal() const			{	return m_wPOOL_SIZE_PORTAL;		}
	WORD				GetHashSizeSetItemInfo() const		{	return m_wHASH_SIZE_SETITEMINFO;}
	WORD				GetHashSizeMakeItemInfo() const		{	return m_wHASH_SIZE_MAKEITEMINFO;	}
	WORD				GetHashSizeBaseItem() const			{	return m_wHASH_SIZE_BASEITEM;	}
	WORD				GetHashSizeItemTrade() const		{	return m_wHASH_SIZE_ITEMTRADE;	}
	WORD				GetHashSizeItemShop() const			{	return m_wHASH_SIZE_ITEMSHOP;	}
	WORD				GetHashSizeShopLog() const			{	return m_wHASH_SIZE_SHOPLOG;	}
	WORD				GetHashSizeGuildList() const		{	return m_wHASH_SIZE_GUILDLIST;	}
	WORD				GetHashSizeItemTitle() const		{	return m_wHASH_SIZE_ITEMTILE;	}
	WORD				GetHashSizeItemStore() const		{	return m_wHASH_SIZE_ITEMSTORE;	}
	WORD				GetHashSizeBaseMonster() const		{	return m_wHASH_SIZE_BASEMONSTER;}
	WORD				GetHashSizeSetItem() const			{	return m_wHASH_SIZE_SETITEM;	}
	WORD				GetHashSizeInvalidLog() const		{	return m_wHASH_SIZE_INVALIDLOG;	}
	WORD				GetHashSizeStoreNative() const		{	return m_wHASH_SIZE_STORENATIVE;}
	DWORD				GetMainMsgQueMaxBufferSize() const	{	return m_dwMainMsgQueMaxBufferSize;	}
	DWORD				GetMaxServerNum() const				{	return m_dwMaxServerNum;		}
	DWORD				GetServerBufferSizePerConnection() const {	return m_dwServerBufferSizePerConnection;	}
	DWORD				GetServerMaxTransferSize() const	{	return m_dwServerMaxTransferSize;	}
	DWORD				GetUserBufferSizePerConnection() const		{	return m_dwUserBufferSizePerConnection;	}
	DWORD				GetUserMaxTransferSize() const		{	return m_dwUserMaxTransferSize;	}
};


extern MACHINENAME g_MachineName;