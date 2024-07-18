//
//	Server Message
//
//		2003/10/27
//		Trust Pak



#ifndef __SERVER_MESSAGE__
#define __SERVER_MESSAGE__



/// Constants ////////////////////////////////////////////////////////////////

const int MAX_SERVER_MESSAGE_LENGTH		= 1024;
const int MAX_SERVER_MESSAGE_COUNT		= 256;


/// Types ////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
//----------------------------------------------------------------------------
struct SERVER_MESSAGE
{
	long int			iId;
	char				szMessage[MAX_SERVER_MESSAGE_LENGTH];
};
//----------------------------------------------------------------------------
#pragma pack(pop)


/// Function proto types /////////////////////////////////////////////////////

char* GetServerMessage(int iCount);


/// Global variables /////////////////////////////////////////////////////////

extern SERVER_MESSAGE*		g_pServerMessage;
extern DWORD				g_dwServerMessageCount;



#endif // __SERVER_MESSAGE__