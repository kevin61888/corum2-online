#include "SevenWork.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "message.h"
#include "CodeFun.h"
#include "CorumResource.h"
#include "DungeonMsg.h"
#include "EffectProcess.h"
#include "DungeonTable.h"

void DisplayMatchInfoOnDungeon()
{
	if (TRUE == g_MatchInfo.IsDisplayMatch() )
	{	
		char szTemp[0xff] = {0,}; 
		
		g_MatchInfo.InitMatchInfoString();

		// 대결 정보 설정 
		__lstrcpyn(szTemp, g_Message[ETC_MESSAGE730].szMessage, lstrlen(g_Message[ETC_MESSAGE730].szMessage));// "[대결 정보]"
		g_MatchInfo.SetInfoString(CMatchInfo::MATCH_INFO_STRING_TIME, szTemp);

		wsprintf(szTemp, g_Message[ETC_MESSAGE731].szMessage, g_MatchInfo.GetRemainSec()); // "대결 시간 : %ld Sec"	
		g_MatchInfo.SetInfoString(CMatchInfo::MATCH_INFO_STRING_RAMAINED_TIME, szTemp);

		// 대결 정보 표시 
		g_MatchInfo.RenderInfo();
	
		// 시간 계산
		g_MatchInfo.SetCurTick(g_dwCurTick);
	}	
}

void GXEmoticonProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	CUser*	pUser = (CUser*)pData->pInfo;

	VECTOR3	v3CameraAngleRad;
	// 카메라 각도 바껴도 정면만 봐라.
	g_pGeometry->GetCameraAngleRad( &v3CameraAngleRad, 0 );
	g_pExecutive->GXOSetDirection( handle, &g_Camera.v3AxsiY,  -v3CameraAngleRad.y);

	GXSetPosition( handle, &pUser->m_v3CurPos, FALSE );

	if (dwCurFrame >= 100)
	{
		// 100 frame 부터 살살 없어져라......(fade out~~
		g_pExecutive->SetAlphaFlag(handle, (unsigned int) (255 - (dwCurFrame-120) * 8) );
	}

	if(bFrameFlag==FRAME_FLAG_FINISHED || bFrameFlag == FRAME_FLAG_CHANGE_NEXT )	
		HideObject(handle);
}