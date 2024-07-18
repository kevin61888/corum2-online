#include "GXDefault.h"
#include "InitGame.h"
#include "User.h"
#include "GameControl.h"
#include <crtdbg.h>
#include "GlobalSoundHandles.h"
extern UPDATE_GAME_STATUS	g_bGameStatus;// sung-han

static DWORD g_dwFrame = 0;

// 플레이어의 애니메이션 프레임 관리 프로시져.
DWORD __stdcall GXPlayerPROC(I4DyuchiGXExecutive* pExecutive, GXOBJECT_HANDLE gxh, DWORD msg, int arg1, int arg2, void* pData)
{	
	LPObjectDesc pObjDesc = (LPObjectDesc)pData;
	DWORD	dwMotionIndex = pExecutive->GXOGetCurrentMotionIndex(gxh);
	
	MOTION_DESC		MotionDesc;
	BOOL			bMotion	= pExecutive->GXOGetMotionDesc(gxh, &MotionDesc, dwMotionIndex,0);
	DWORD			dwSync=0;
	DWORD			dwFrame=0;	
	
	if( pObjDesc->bSkip	)			// 프레임 스킵을 시행한다.
	{
		if( DWORD( pObjDesc->nCurFrame + arg1 ) >= MotionDesc.dwLastFrame )
		{
			if( DWORD(pObjDesc->nCurFrame) >= MotionDesc.dwLastFrame ) 
			{
				pObjDesc->nCurFrame = MotionDesc.dwLastFrame;
				dwSync = 1;
			}
			else
			{
				dwSync = ( MotionDesc.dwLastFrame - pObjDesc->nCurFrame ) + 1;
			}
		}
		else 
		{
			dwSync = arg1;
		}
	}
	else
	{
		dwSync = 1;					// 스킵하지 않는다.
	}

	for(int f = 0; (DWORD)f < dwSync ; f++)
	{
		dwFrame			= pObjDesc->nCurFrame;	// 현재 찍어야 할 프레임 넘버.	
		
		// 충돌처리.----------
		if( msg == GXSCHEDULE_PROC_MSG_COLLISION )
		{
			GXSCHEDULE_PROC_MSG_COLLISION_ARG *pArg = (GXSCHEDULE_PROC_MSG_COLLISION_ARG*)arg1;	
			LPObjectDesc pDesc = (LPObjectDesc)pData;

			if(pDesc->CrashFunc)
			{
				(*pDesc->CrashFunc)(gxh, pDesc, pArg);
				return 0;	
			}
		
			return	GXSCHEDULE_PROC_MSG_COLLISION_RETURN_STOP;
		}
		//-------------------

	//	float	frad = PI/4;

		BYTE bFrameFlag = FRAME_FLAG_PLAYING;		

		if(bMotion)
		{
			switch((int)pObjDesc->bActionFlag)
			{
			case ACTION_ONCE:
				{
					if(dwFrame < MotionDesc.dwLastFrame)
					{
						pExecutive->GXOSetCurrentFrame(gxh, dwFrame);
						pObjDesc->nCurFrame++;
					}
					else
					{
						bFrameFlag = FRAME_FLAG_FINISHED;
					}
				}
				break;

			case ACTION_ONCE_HIDE:
				{
					if(dwFrame < MotionDesc.dwLastFrame)
					{
						pExecutive->GXOSetCurrentFrame(gxh, dwFrame);
						pObjDesc->nCurFrame++;
					}
					else
					{
						bFrameFlag = FRAME_FLAG_FINISHED_HIDE;
					}
				}
				break;

			case ACTION_LOOP:
				{
					if(dwFrame < MotionDesc.dwLastFrame)
					{
						pExecutive->GXOSetCurrentFrame(gxh,dwFrame);	
						pObjDesc->nCurFrame++;
					}
					else
					{
						bFrameFlag = FRAME_FLAG_FINISHED;
						pExecutive->GXOSetCurrentFrame(gxh,0);
						pObjDesc->nCurFrame = 0;
						dwFrame = 0; 
					}
				}
				break;

			case ACTION_PAUSE:
				{
					
				}
				break;

			case ACTION_NEXT:
				{
					if(dwFrame < MotionDesc.dwLastFrame)
					{
						pExecutive->GXOSetCurrentFrame(gxh, dwFrame);
						pObjDesc->nCurFrame++;
					}
					else
					{
						pObjDesc->bActionFlag = pObjDesc->bNextFlag;
						
						pExecutive->GXOSetCurrentMotionIndex( gxh, pObjDesc->wNextAct );

						if(pObjDesc->nNextFrame >= 0)
						{
							pExecutive->GXOSetCurrentFrame( gxh, pObjDesc->nNextFrame );
							pObjDesc->nCurFrame		= pObjDesc->nNextFrame;
							pObjDesc->nNextFrame	= 0;
						}
						else
						{
							pExecutive->GXOSetCurrentFrame( gxh, 0 );
							pObjDesc->nCurFrame		= 0;
							pObjDesc->nNextFrame	= 0;
							dwFrame = 0;
						}

						bFrameFlag = FRAME_FLAG_CHANGE_NEXT;
					}
				}
				break;
			}
	
			if(pObjDesc->ObjectFunc)
				(*pObjDesc->ObjectFunc)(gxh, pObjDesc, dwFrame, bFrameFlag);

			g_SoundEffectBasketCase.UpdateToClean(); 
			
			// 막 프레임이면 스킵을 다시 활성화 해준다.
			if( bFrameFlag == FRAME_FLAG_CHANGE_NEXT || bFrameFlag == FRAME_FLAG_FINISHED )
				pObjDesc->bSkip = TRUE;

			ObjectSoundPlay(pObjDesc, dwMotionIndex, (BYTE)dwFrame);
		}
	}	

	// sung-han 2005-01-24 로그인화면에서 카메라 워킹 작업
	//---------------------------------------------------------------------
	if( g_bGameStatus == UPDATE_GAME_LOGIN )
	{
		VECTOR3 vecQuake;vecQuake.x = vecQuake.y = vecQuake.z = 0;
		CAMERA_DESC	CameraDesc;
		g_pGeometry->GetCameraDesc( &CameraDesc, 0 );
		DWORD dwTick = GetTickCount();
		WORD wTemp = dwTick % 20000;

		if( wTemp >= 0 && wTemp < 1000 )
		{
			vecQuake.x = (float)-0.05;
			vecQuake.z = (float)-0.05;
		}
		else if( wTemp >= 1000 && wTemp < 2000 )
		{
			vecQuake.x = (float)-0.1;
			vecQuake.z = (float)-0.1;
		}
		else if( wTemp >= 2000 && wTemp < 9000 )
		{
			vecQuake.x = (float)-0.2;
			vecQuake.z = (float)-0.2;
		}
		else if( wTemp >= 9000 && wTemp < 9500 )
		{
			vecQuake.x = (float)-0.1;
			vecQuake.z = (float)-0.1;
		}
		else if( wTemp >= 9500 && wTemp < 10000 )
		{
		}
		else if( wTemp >= 10000 && wTemp < 11000 )
		{
			vecQuake.x = (float)0.05;
			vecQuake.z = (float)0.05;
		}
		else if( wTemp >= 11000 && wTemp < 12000 )
		{
			vecQuake.x = (float)0.1;
			vecQuake.z = (float)0.1;
		}
		else if( wTemp >= 12000 && wTemp < 19000 )
		{
			vecQuake.x = (float)0.2;
			vecQuake.z = (float)0.2;
		}
		else if( wTemp >= 19000 && wTemp < 19500 )
		{
			vecQuake.x = (float)0.1;
			vecQuake.z = (float)0.1;
		}
		else if( wTemp >= 19500 && wTemp < 20000 )
		{
		}

		if( CameraDesc.v3From.z > 200.34f-60.0f && CameraDesc.v3From.z < 200.34f+60.0f)
			g_pGeometry->MoveCamera(&vecQuake, 0 );
		else if( CameraDesc.v3From.z <= 200.34f-60.0f && vecQuake.z > 0 )
			g_pGeometry->MoveCamera(&vecQuake, 0 );
		else if( CameraDesc.v3From.z >= 200.34f+60.0f && vecQuake.z < 0 )
			g_pGeometry->MoveCamera(&vecQuake, 0 );
	}
	//---------------------------------------------------------------------

	return 0;
}