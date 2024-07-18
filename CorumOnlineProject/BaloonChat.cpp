#include "BaloonChat.h"

//------------------------------------------------------------------------
// Date			: 2004. 10. 05. ( Written by minjin )
// Description	: 말풍선 위치 보정용 값들
//------------------------------------------------------------------------
SHORT g_usBaloonChatWndResInfo[10][5] = 
{
	//														   Adjustments
	//	Sprite Index,							x		y		x		y
	{	SPR_INTERFACE_BALOON_CHAT_WND_LTOP,		0,		0,		-8,		-8	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_CTOP,		8,		0,		0,		-8	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_RTOP,		9,		0,		0,		-8	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_LMID,		0,		8,		-8,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_CMID,		8,		8,		0,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_RMID,		9,		8,		0,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_LBOT,		0,		9,		-8,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_CBOT,		8,		9,		0,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_RBOT,		9,		9,		0,		0	},
	{	SPR_INTERFACE_BALOON_CHAT_WND_TAIL,		17,		0,		70,		8	}		
};

//------------------------------------------------------------------------
// Function		: __inline GetRegisteredSprite
// Date			: 2004. 10. 05. ( Written by minjin )
// Description	: 인터페이스 스프라이트 해쉬에서
//				  키값으로 스프라이트 오브젝트 포인터 얻어오기
//------------------------------------------------------------------------
__inline IDISpriteObject* GetRegisteredSprite( DWORD dwKey )
{
	return g_pInterfaceSprHash->GetData(dwKey)->pSpr;
}

//------------------------------------------------------------------------
// Function		: RenderBaloonChatBox
// Date			: 2004. 10. 05. ( Written by minjin )
// Description	: 말 쓰면, 말풍선 모양으로 화면에 표시
//------------------------------------------------------------------------
void RenderBaloonChatBox( VECTOR2* pV2Scailing, VECTOR2* pV2Position, int nIndex)
{
	VECTOR2 vScale;
	VECTOR2 vPosition;

	// 배열에 들어있는 스프라이트 정보 가지고 찍는다
	for ( int i = 0; i < 10; i++ )
	{
		// 처음에 위치 보정부터 하고.. (기준은 가운데 박스 좌측 상단지점..)
		vPosition.x = pV2Position->x + g_usBaloonChatWndResInfo[i][RESOURCE_X_ADJ];
		vPosition.y = pV2Position->y + g_usBaloonChatWndResInfo[i][RESOURCE_Y_ADJ];

		// 스케일링 필요한 스프라이트 스케일링하기

		vScale.x = 1.0f;		// 기본 스케일링 먼저 하고,
		vScale.y = 1.0f;

		switch ( g_usBaloonChatWndResInfo[i][RESOURCE_INDEX] )
		{
		case SPR_INTERFACE_BALOON_CHAT_WND_CMID:	// 정가운데 있는거 (가로세로 다 늘인다)
			vScale.x = pV2Scailing->x;
			vScale.y = pV2Scailing->y;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_CTOP:	// 윗쪽 가운데.. (가로로 늘인다)
			vScale.x = pV2Scailing->x;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_CBOT:	// 아래쪽 가운데.. (가로로 늘인다)
			vScale.x = pV2Scailing->x;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_LMID:	// 왼쪽 중간.. (세로로 늘인다)
			vScale.y = pV2Scailing->y;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_RMID:	// 오른쪽 중간.. (세로로 늘인다)
			vScale.y = pV2Scailing->y;
			break;

		default:
			break;
		}

		// 스케일링 끝나는대로, 위치 조절
		switch ( g_usBaloonChatWndResInfo[i][RESOURCE_INDEX] )
		{
		case SPR_INTERFACE_BALOON_CHAT_WND_RTOP:
			vPosition.x += pV2Scailing->x;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_RMID:
			vPosition.x += pV2Scailing->x;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_LBOT:
			vPosition.y += pV2Scailing->y;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_CBOT:
			vPosition.y += pV2Scailing->y;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_RBOT:
			vPosition.x += pV2Scailing->x;
			vPosition.y += pV2Scailing->y;
			break;

		case SPR_INTERFACE_BALOON_CHAT_WND_TAIL:
			vPosition.y += pV2Scailing->y;
			break;

		default:
			break;
		}

		g_pRenderer->RenderSprite( GetRegisteredSprite( g_usBaloonChatWndResInfo[i][RESOURCE_INDEX] )
			, &vScale, 0.0, &vPosition, NULL, 0xb0ffffff, nIndex, 0);
	}
}
