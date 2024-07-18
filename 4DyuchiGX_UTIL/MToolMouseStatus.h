// MToolMouseStatus.h: interface for the CToolMouseStatus class.
//
//////////////////////////////////////////////////////////////////////
/*
	독립적으로 돌아갈 수 있는 클래스는 아니다.
*/
#if !defined(AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_)
#define AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGRX_common/IGeometry.h"



class MToolMouseStatus  
{
protected:		// 변수군.
	int		miOldMouseX;
	int		miOldMouseY;
	int		miMouseX;
	int		miMouseY;
	int		miMouseZ;		// 휠.

	float	mfMoveHorizonStep;
	float	mfMoveVerticalStep;
	float	mfRotateStep;	// Shift에 의해 상태가 달라지는 회전, 이동 속도값.

	bool	mbLButtonDown;
	bool	mbRButtonDown;
	bool	mbMButtonDown;
	bool	mbCtrlDown;
	bool	mbShiftDown;		// 쉬프트 버튼은 고속 이동.

	DWORD	mdwMouseMode;

	I4DyuchiGXGeometry*			mpGeometry;

	BOOL	mbDoNotRotate;
public:

	// 초기화 할때 꼭 넣어줘야한다.
	BOOL	Initialize(I4DyuchiGXGeometry* pGeometry);

	// 로테이트 모드가 WithPivot일때 기준축.
	void SetRotationPivot( VECTOR3* pPivot);
	// WM_MOUSEMOVE 메시지를 받았을때 호출할 함수. dwFlag은 버튼 상태.
	bool MoveMouse( DWORD dwFlag, int iMouseX, int iMouseY);

	void	DoNotRotate(void);
	void	AllowRotate(void);

	void	PushCameraAngleAndPos(DWORD	dwStackNum);
	void	PopCameraAngleAndPos(DWORD	dwStackNum);
	DWORD	GetMouseMode() {return mdwMouseMode;}
	MToolMouseStatus();
	virtual ~MToolMouseStatus();



	VECTOR3		mpCameraStackPos[10];
	float		mpfCameraStackFromTo[10];
	VECTOR3		mpCameraStackAngle[10];

	VECTOR3		mRotationPivot;

protected:		// 함수군.
	bool MouseNone(void);
	bool MouseRotate(void);
	bool MouseMoveVertical(void);
	bool MouseMoveHorizon(void);
	bool MouseRotateWithPivot(void);


};

#endif // !defined(AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_)
