// ToolMouseStatus.cpp: implementation of the CToolMouseStatus class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "4DyuchiGXMapEditor.h"
#include "MToolMouseStatus.h"
#include "../4dyuchigrx_common/typedef.h"
#include "../4DyuchiGXGFunc/global.h"

//#include "tool.h"

//extern CTool* g_pTool;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//	마우스 이벤트.
#define		MOUSE_NONE					0
#define		MOUSE_ROTATE				1
#define		MOUSE_MOVEHORIZON			2
#define		MOUSE_MOVEVERTICAL			3
#define		MOUSE_ROTATE_WITH_PIVOT		4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MToolMouseStatus::MToolMouseStatus()
{

}

MToolMouseStatus::~MToolMouseStatus()
{

}

BOOL MToolMouseStatus::Initialize( I4DyuchiGXGeometry* pGeometry)
{
	mfMoveHorizonStep		=	1.0f;
	mfMoveVerticalStep		=	10.0f;
//	fRotateStep		=	0.2f;	// Shift에 의해 상태가 달라지는 회전, 이동 속도값.
	mfRotateStep		=	0.2f * PI / 180.0f;	// Shift에 의해 상태가 달라지는 회전, 이동 속도값.
	mpGeometry = pGeometry;
	mbDoNotRotate		=	FALSE;

	miMouseX	=	-1;
	miMouseY	=	-1;
	miMouseZ	=	-1;

	miOldMouseX	=	-1;
	miOldMouseY	=	-1;

	mbLButtonDown	=	false;
	mbRButtonDown	=	false;
	mbMButtonDown	=	false;
	mbCtrlDown		=	false;
	mbShiftDown		=	false;		// 쉬프트 버튼은 고속 이동.

	mdwMouseMode	=	MOUSE_NONE;



	return TRUE;
}
bool MToolMouseStatus::MoveMouse(DWORD dwFlag, int iMouseX, int iMouseY)
{
	if( mpGeometry == 0)
	{
		__asm int 3
		// 랜더러가 아직 설정 안됬다.
	}
	if( miMouseX == -1 || miMouseY == -1)
	{
		miOldMouseX	=	iMouseX;
		miOldMouseY	=	iMouseY;
	}
	miMouseX	=	iMouseX;
	miMouseY	=	iMouseY;

	//	컨트롤키.
	mbCtrlDown	=	false;
	if( dwFlag & MK_CONTROL)
	{
		mbCtrlDown	=	true;
	}
	//	시프트키.
	mbShiftDown	=	false;
	if( dwFlag & MK_SHIFT)
	{
		mbShiftDown	=	true;
//		fMoveHorizonStep		=	5.0f;
//		fMoveVerticalStep		=	50.0f;
//		fRotateStep		=	0.5f;	// Shift에 의해 상태가 달라지는 회전, 이동 속도값.
	}
	else
	{
//		fMoveHorizonStep		=	1.0f;
//		fMoveVerticalStep		=	10.0f;
//		fRotateStep		=	0.2f;	// Shift에 의해 상태가 달라지는 회전, 이동 속도값.
	}
	//	마우스 왼쪽 버튼.
	mbLButtonDown	=	false;
	if( dwFlag & MK_LBUTTON)
	{
		mbLButtonDown	=	true;
	}
	//	마우스 중간 버튼.
	mbMButtonDown	=	false;
	if( dwFlag & MK_MBUTTON)
	{
		mbMButtonDown	=	true;
	}
	//	마우스 오른쪽 버튼.
	mbRButtonDown	=	false;
	if( dwFlag & MK_RBUTTON)
	{
		mbRButtonDown	=	true;
	}

	// 모드에 따른 스위치, 함수 콜.
	switch( mdwMouseMode)
	{
		case	MOUSE_NONE:
				miOldMouseX = iMouseX;
				miOldMouseY = iMouseY;
			// 마우스 오른쪽 버튼만 눌린 경우.
			// 회전 모드로.
			if( !mbLButtonDown && !mbMButtonDown && mbRButtonDown && !mbCtrlDown && !mbShiftDown)	
			{
				mdwMouseMode = MOUSE_ROTATE;	
			}
			// 마우스 오른쪽 버튼과 컨트롤이 눌려졌을 경우.
			//	수평 이동 모드로.
			else if( !mbLButtonDown && !mbMButtonDown && mbRButtonDown && mbCtrlDown && !mbShiftDown)	
			{
				mdwMouseMode	=	MOUSE_MOVEHORIZON;
			}
			// 마우스 오른쪽 버튼과 시프트가 눌려진 경우.
			//	수직(높이) 이동 모드로.
			else if( !mbLButtonDown && !mbMButtonDown && mbRButtonDown && !mbCtrlDown && mbShiftDown)	
			{
				mdwMouseMode	=	MOUSE_MOVEVERTICAL;
			}
			// 마우스 오른쪽 버튼과 시프트,컨트롤이 동시에 눌려진 경우.
			//	주어진 피봇을 중심으로 회전한다.
			else if( !mbLButtonDown && !mbMButtonDown && mbRButtonDown && mbCtrlDown && mbShiftDown)	
			{
				mdwMouseMode	=	MOUSE_ROTATE_WITH_PIVOT;
			}

			return this->MouseNone();
			break;
		case	MOUSE_ROTATE:
			if( mbLButtonDown || mbMButtonDown || !mbRButtonDown || mbCtrlDown || mbShiftDown)	// 어떻게든 상태가 달라지면 모드 해제.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseRotate();
			}

			break;
		case	MOUSE_MOVEHORIZON:
			if( mbLButtonDown || mbMButtonDown || !mbRButtonDown || !mbCtrlDown || mbShiftDown)	// 어떻게든 상태가 달라지면 모드 해제.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseMoveHorizon();
			}
			break;
		case	MOUSE_MOVEVERTICAL:
			if( mbLButtonDown || mbMButtonDown || !mbRButtonDown || mbCtrlDown || !mbShiftDown)	// 어떻게든 상태가 달라지면 모드 해제.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseMoveVertical();
			}
			break;
		case	MOUSE_ROTATE_WITH_PIVOT:
			if( mbLButtonDown || mbMButtonDown || !mbRButtonDown || !mbCtrlDown || !mbShiftDown)	// 어떻게든 상태가 달라지면 모드 해제.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseRotateWithPivot();
			}
			break;
		default:
			break;
	}

return false;
}

bool MToolMouseStatus::MouseNone()
{
	// 아무것도 안한다. -_-;
	return false;
}

bool MToolMouseStatus::MouseRotate()
{
	if( mbDoNotRotate == TRUE)	return true;		// 마우스 로테이트 금지 모드일경우...

	float	x = float(miMouseX - miOldMouseX) * mfRotateStep;
	float	y = float(miMouseY - miOldMouseY) * mfRotateStep;
	VECTOR3	Rotate;
	Rotate.x = -y;
	Rotate.y = x;
	Rotate.z = 0;


	mpGeometry->RotateCameraRad(&Rotate,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}

bool MToolMouseStatus::MouseRotateWithPivot()
{
	if( mbDoNotRotate == TRUE)	return true;		// 마우스 로테이트 금지 모드일경우...
//	여기

	mRotationPivot.x	=	50.0f;
	mRotationPivot.y	=	0.0f;
	mRotationPivot.z	=	0.0f;


	float	fRotateYaw		=	float(miMouseX - miOldMouseX) * mfRotateStep;
	float	fRotatePitch	=	-1.0f * float(miMouseY - miOldMouseY) * mfRotateStep;
	VECTOR3	Rotate;
	VECTOR3		NewPos, CurrentPos;
	CAMERA_DESC Desc;
	mpGeometry->GetCameraDesc( &Desc, 0);
	CurrentPos	=	Desc.v3From;

	VECTOR3		CameraAngle;
	mpGeometry->GetCameraAngleRad( &CameraAngle, 0);
/*
	if( CameraAngle.x >= (PI-0.01)/2.0f)	// 카메라가 아래를 보고 있을 경우.
	{
		_asm int 3;
	}
	else if( CameraAngle.x <= -1.0f * (PI-0.01)/2.0f)	// 카메라가 아래를 보고 있을 경우.
	{
		_asm int 3;
	}
*/
	// 피봇을 기준으로 위치를 옮기고,
	RotatePositionWithPivot( &NewPos, &mRotationPivot, &CurrentPos, &CameraAngle, fRotateYaw, fRotatePitch);
	mpGeometry->SetCameraPos( &NewPos, 0);

	// 마우스를 로테이트.	(위치 옮기는것과의 순서는 관계 없다.)
	Rotate.x = fRotatePitch;
	Rotate.y = fRotateYaw;
	Rotate.z = 0;
	mpGeometry->RotateCameraRad(&Rotate,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}

bool MToolMouseStatus::MouseMoveHorizon()
{
	float	x	=	float(miMouseX - miOldMouseX) * mfMoveHorizonStep * -1.0f;
	float	y	=	float(miMouseY - miOldMouseY) * mfMoveHorizonStep * -1.0f;
	
	VECTOR3	Rot;
	mpGeometry->GetCameraAngleRad(&Rot,0);

	VECTOR3	To;
	To.x	=	(-1 * x * (float)cosf(Rot.y) *10.0f + y * (float)sinf(Rot.y)*10.0f);
	To.y	=	0.0f;
	To.z	=	x * (float)sinf(Rot.y) *10.0f + y * (float)cosf(Rot.y)*10.0f;

	mpGeometry->MoveCamera( &To,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;
						
	return true;
}

bool MToolMouseStatus::MouseMoveVertical()
{
	VECTOR3		To;
	To.x	=	0;
	To.y	=	float(miMouseY - miOldMouseY) * mfMoveVerticalStep * -1;
	To.z	=	0;
	mpGeometry->MoveCamera(&To,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}


void	MToolMouseStatus::DoNotRotate(void)
{
	mbDoNotRotate	=	TRUE;
}


void	MToolMouseStatus::AllowRotate(void)
{
	mbDoNotRotate	=	FALSE;
}

void	MToolMouseStatus::PushCameraAngleAndPos(DWORD dwStackNum)
{
	CAMERA_DESC		Desc;
	mpGeometry->GetCameraDesc(&Desc,0);
	mpCameraStackPos[dwStackNum]		=	Desc.v3From;
	mpfCameraStackFromTo[dwStackNum]	=	CalcDistance( &(Desc.v3From), &(Desc.v3To));
	mpGeometry->GetCameraAngleRad(&mpCameraStackAngle[dwStackNum],0);
}

void	MToolMouseStatus::PopCameraAngleAndPos(DWORD dwStackNum)
{
	VECTOR3	To	=	mpCameraStackPos[dwStackNum];
	To.z	=	To.z + mpfCameraStackFromTo[dwStackNum];
	VECTOR3 Up;
	Up.x = Up.z = 0;
	Up.y = 1;
		
	float fNear = DEFAULT_NEAR;
	float fFar = DEFAULT_FAR;
	

	mpGeometry->ResetCamera( &(mpCameraStackPos[dwStackNum]),fNear,fFar,DEFAULT_FOV,0);
//	mpGeometry->ResetCamera( &(mpCameraStackPos[dwStackNum]),fNear,5000.0f,DEFAULT_FOV,0);
	mpGeometry->SetCameraAngleRad( &(mpCameraStackAngle[dwStackNum]),0 );
}

void MToolMouseStatus::SetRotationPivot(VECTOR3 *pPivot)
{
	mRotationPivot	=	*pPivot;
}
