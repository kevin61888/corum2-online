//
//		Notice Message object
//
//			2003/10/17
//			Trust Pak
//



#ifndef __NOTICE_MESSAGE_OBJECT__
#define __NOTICE_MESSAGE_OBJECT__


/// Constants ////////////////////////////////////////////////////////////////

const int MAX_NOTICE_MESSAGE_LENGTH		= 1024;
const int MAX_NOTICE_COUNT				= 256;


/// Types ////////////////////////////////////////////////////////////////////


#pragma pack(push,1)
//----------------------------------------------------------------------------
struct NOTICE_MESSAGE
{
	long int			iNo;
	unsigned char		byImportant;
	unsigned char		byType;	
	char				szMessage[MAX_NOTICE_MESSAGE_LENGTH];
	unsigned char		byShow;
};

//----------------------------------------------------------------------------
#pragma pack(pop)



/// Global variables /////////////////////////////////////////////////////////

extern NOTICE_MESSAGE*		g_pNoticeMessage;
extern DWORD				g_dwNoticeMessageCount;

#endif // __NOTICE_MESSAGE_OBJECT__