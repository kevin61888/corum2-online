#include "stdafx.h"
#include "ChatBackground.h"
#include "CorumResource.h"
#include "InitGame.h"


void CreateChatEditBackground()
{
	g_pSprManager->CreateSprite(SPR_EDIT_BKGR, 60, 100, FALSE, 100);
	SPR(SPR_EDIT_BKGR)->SetAlpha(128);
}

void ReleaseChatEditBackground()
{
	RELEASE_SPR(SPR_EDIT_BKGR);
}

// by deepdark.
void ShowChatEditBackground(BOOL bShow, int zOrder)
{
	if(bShow)
	{
		SPR(SPR_EDIT_BKGR)->vPos.x		= 4;
		SPR(SPR_EDIT_BKGR)->vPos.y		= 630;

#if IS_JAPAN_LOCALIZING()
		SPR(SPR_EDIT_BKGR)->v2Scaling.x = 170.0f;//178.0f;
#else
		SPR(SPR_EDIT_BKGR)->v2Scaling.x = 178.0f;//210.0f;
#endif

		SPR(SPR_EDIT_BKGR)->v2Scaling.y = 10.0f;
		SPR(SPR_EDIT_BKGR)->nZOrder		= zOrder;
	}

	SPR(SPR_EDIT_BKGR)->ShowSprite(bShow);
}

BOOL IsChatEditBackgroundShowed()
{
	return BOOL(SPR(SPR_EDIT_BKGR)->bRenderFlag);
}