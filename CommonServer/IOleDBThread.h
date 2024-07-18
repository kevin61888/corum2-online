//---------------------------------------------------------------------------------------------------------
//						OLEDBThread Interface Header File by Byungsoo Koo											
//																
//																Since 2001. 7. 24															
//																
//																Modified : Minbobo
//---------------------------------------------------------------------------------------------------------
#pragma once
#include <initguid.h>
#include "dbstruct.h"


// {896940DB-7E35-4e4f-8D90-AF5E78A10B3B}
DEFINE_GUID(CLSID_OLEDBTHREAD, 
0x896940db, 0x7e35, 0x4e4f, 0x8d, 0x90, 0xaf, 0x5e, 0x78, 0xa1, 0xb, 0x3b);

// {E8BDB6F8-679D-40bf-83A3-B647E797FB1B}
DEFINE_GUID(IID_OLEDBTHREAD, 
0xe8bdb6f8, 0x679d, 0x40bf, 0x83, 0xa3, 0xb6, 0x47, 0xe7, 0x97, 0xfb, 0x1b);


interface IOleDBThread : IUnknown
{			
	virtual BOOL	__stdcall InitDBModule(DB_INITIALIZE_DESC* desc) = 0;

	virtual	int		__stdcall CreateDBConnection() = 0;
	virtual	int		__stdcall Connect(const char* szDataSource, const char* szDefaultDb, const char* szUserId, const char* szPassword, int nConnectTimeout = 20, int nQuerytimeout = 10, BOOL bCommandTimeOut = FALSE, BYTE bConnectionIndex = 0) = 0;
	virtual	int		__stdcall Connect(const char* szDataSource, const char* szDefaultDb, const char* szUserId, const char* szPassword, int nConnectTimeout = 20, BYTE bConnectionIndex = 0) = 0;
	virtual	BOOL	__stdcall ReleaseDBConnectionForSingleThread(BYTE bConnectionIndex) = 0;	


	virtual	int			__stdcall ExecuteSQL(char* szQuerySQL, BYTE bConnectionIndex = 0) = 0;
	virtual	int			__stdcall ExecuteSQLByParam(char* szQuerySQL, DBBINDING* pBinding, void* pParamValue, BYTE bParamNum, BYTE bConnectionIndex = 0) = 0;
	virtual	DBBINDING*  __stdcall CreateParamInfo(WORD wParamNum, BYTE bConnectionIndex = 0) = 0;
	virtual	BOOL		__stdcall ReleaseParamInfo(DBBINDING* pBinding, BYTE bConnectionIndex = 0) = 0;
		
		
	virtual	int				__stdcall OpenRecord(char* szQuerySQL, void* pRecordSet, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, BYTE bConnectionIndex = 0) = 0;
	virtual	DBRECEIVEDATA*  __stdcall OpenRecordEx(char* szQuerySQL, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, DWORD dwRowPerRead = DEFAULT_ROWS_PER_READ, BYTE bConnectionIndex = 0) = 0;
	virtual	BOOL			__stdcall GetData(void* pReceiveData, DBRECEIVEDATA* pResultData, DWORD dwRowNum, WORD wColumnNum, BYTE bConnectionIndex = 0) = 0;	//dwRowNum와 wColumnNum 은 zero based Index이다..
	virtual	BOOL			__stdcall ReleaseRecordset(DBRECEIVEDATA* pResultData, BYTE bConnectionIndex = 0) = 0;

		
	virtual	int		__stdcall GetBLOBData(char* szTableName, char* szColumnName, char* szFilter, void* pReadBuffer, DWORD dwReadSize, BYTE bConnectionIndex = 0) = 0;
	virtual	int		__stdcall SetBLOBData(char* szTableName, char* szColumnName, char* szFilter, void* pWriteData,  DWORD dwWriteSize, BYTE bConnectionIndex = 0) = 0;
	//by File
	virtual int __stdcall GetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
								char* szFilePath, DWORD dwReadSize, BYTE bConnectionIndex = 0) = 0;
	virtual int __stdcall SetBLOBData(char* szTableName, char* szColumnName, char* szFilter, 
								char* szFilePath, BYTE bConnectionIndex = 0) = 0;


	virtual	BOOL	__stdcall ChangeDB(char* szDbName, BYTE bConnectionIndex) = 0;
	virtual	BOOL	__stdcall SetDefaultDBIndex(BYTE bConnectionIndex) = 0;
	virtual	BYTE	__stdcall GetDefaultDBIndex() = 0;

	//Multibyte 한글 -> 유니코드 한글 형으로 변환
	virtual BOOL	__stdcall KSCToUnicode(const char *pKsc, WCHAR *pUni) = 0;
	//유니코드형 한글 -> Multibyte한글로 변환 
	virtual BOOL	__stdcall UnicodeToKSC(WCHAR *pUni, char *pKsc, int nKscSize) = 0;

	//Added at 020109
	//SQLServer의 모든 Database 명반환 
	virtual int		__stdcall QueryDBCatalog(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows, BYTE bConnectionIndex = 0) = 0;
	//Added at 020110
	//현재 Database의 모든 UserTable 명 반환 
	virtual int		__stdcall QueryDBTable(DBSCHEMA* pSchemaBuf, DWORD dwMaxNumRows, BYTE bConnectionIndex = 0) = 0;

	virtual DBCOLUMNINFO*	__stdcall GetColumnInfo(char* szQuerySQL, DWORD* pColnum/*out*/, BYTE bConnectionIndex = 0) = 0;

	
	//Methods For Thread 
	virtual void	__stdcall THOpenRecord(char* szQuerySQL,  DWORD dwQueryUID,  void* pData = NULL, DWORD dwMaxNumRows = DEFAULT_RETURNED_MAX_ROWS, BYTE bConnectionIndex = 0) = 0;
	virtual void    __stdcall THExecuteSQL(char* szQuerySQL,  BYTE bReturnResult = FALSE, DWORD dwQueryUID = 0,  void* pData = NULL, BYTE bConnectionIndex = 0) = 0;
	virtual void	__stdcall THExecuteSQLByParam(char* szQuerySQL, DBBINDING* pBinding, void* pParamValue, DWORD dwParamValueSize, BYTE bParamNum,
										BYTE bReturnResult = FALSE, DWORD dwQueryUID = 0,  void* pData= NULL, BYTE bConnectionIndex = 0) = 0;
	virtual DBBINDING* __stdcall THCreateParamInfo(WORD wParamNum) = 0;
	virtual BOOL	__stdcall THChangeDB(char* szDbName, BYTE bReturnResult = FALSE, DWORD dwQueryUID = 0, BYTE bConnectionIndex = 0) = 0;
	virtual BOOL	__stdcall THReleaseRecordset(DBRECEIVEDATA* pResultData) = 0;
	virtual void    __stdcall FreeResultMessage(DBRECEIVEDATA* pMsg) = 0;
	virtual void	__stdcall SetDBResultEvent(HANDLE val) = 0;
	virtual void    __stdcall GetDBResult() = 0;

	virtual	void	__stdcall THIgnoreQueryInput() = 0;
	virtual void	__stdcall THAcceptQueryInput() = 0;

	virtual void	__stdcall SetPerformanceReport(BOOL bEnable) = 0;

	virtual DWORD	__stdcall GetOLEDBSize() = 0;
};
