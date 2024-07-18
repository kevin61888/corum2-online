//==================================================//
// Code by Jang.									//
//==================================================//
#pragma once

// 인터페이스 관련 스프라이트를 데이터를 관리 한다.
#include	"Menu.h"

#pragma pack(push, 1)

// 리소스의 정보를 담고 있는 구조체
// 리소스 추가할때는 InterfaceResourceInfo table에 채워 넣고 cdb로 뽑는다.
#define MAX_INTERFACE_RESOURCE_INFO	700
struct INTERFACE_RESOURCE_INFO
{
	WORD	wID;
	char	szFileName[50];
	BYTE	byType;
	WORD	wLeft;
	WORD	wTop;
	WORD	wRight;
	WORD	wBottom;
};

#define MAX_INTERFACE_SPRITE_MANAGER	700
struct INTERFACE_SPRITE_MANAGER
{
	WORD	wID;
	WORD	wResourceID;	// INTERFACE_RESOURCE_INFO 구조체를 참조할 인덱스
};
#define MAX_INTERFACE_COMPONENT_INFO	1800
enum INTERFACE_COMPONENT_INFO_RESOUCETYPE
{
	INTERFACE_COMPONENT_INFO_RESOUCETYPE_COLLITION_BOX = 1,
	INTERFACE_COMPONENT_INFO_RESOUCETYPE_MESSAGE = 2,
	INTERFACE_COMPONENT_INFO_RESOUCETYPE_SPRITE = 3,
};
struct INTERFACE_COMPONENT_INFO
{
	WORD	wInterfaceID;
	BYTE	byCallBackID;
	BYTE	byResourceType;
	WORD	wResourceID;
	BYTE	wOrder;
	WORD	wLeft;
	WORD	wTop;
	WORD	wRight;
	WORD	wBottom;
	float	fScaleX;
	float	fScaleY;
	BYTE	byValue;
	BYTE	byPos;
};
#define MAX_INTERFACE_FRAME_INFO	100
struct INTERFACE_FRAME_INFO
{
	WORD	wFrameID;
	char	szFrameName[30];
	WORD	wWidth;
	WORD	wHeight;
	BYTE	byType;
	WORD	wIndex;
	BYTE	byActive;
	BYTE	byScroll;
};
#pragma pack(pop)


typedef struct sSprite_Data
{
	DWORD	dwId;
	
	IDISpriteObject* pSpr;

}SSPRITE_DATA, *LP_SPRITE_DATA;

class CInterfaceSprHash : public CAbyssHash<SSPRITE_DATA>	
{
public:

	void DeleteData( void* pData );
};

extern CInterfaceSprHash*	g_pInterfaceSprHash;
extern STMPOOL_HANDLE		g_pInterfaceSprPool;

//==================================================//
// End.												//
//==================================================//