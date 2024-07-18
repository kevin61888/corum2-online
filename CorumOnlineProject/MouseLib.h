

#ifndef	_MouseLib_h_
#define	_MouseLib_h_

#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGRX_common/IGeometry.h"
#else
#include "./../4DyuchiGRX_common/IGeometry.h"
#endif

// 스크린상의 x,y좌표를 실제 좌표의 x,0,z로 바꾼다. y는 언제나 0
//	주의할점은, 카메라 from의 y좌표가 0 이상일때 수평선보다 위를 보고 찍으면, 시점보다 뒤쪽에서 만나는 좌표가 나온다.
VECTOR3	GetXYZFromScreenXY( I4DyuchiGXGeometry* pGeometry, DWORD dwX, DWORD dwY, DWORD dwScreenX, DWORD dwScreenY);

// 공간상의 VECTOR3가 화면상의 어디인지 알아내는 함수.
// 단순히 pWhere을 트랜스폼을 거친 후 결과를 pOutScreen에 출력한다.
// 나오는 pOutScreen.x와 .y는 왼쪽, 위가 각각 0.0f, 오른쪽 아래가 각각 1.0f 이므로, 화면 해상도를 곱해야 화면상의 좌표를 구할 수
// 있다. 범위가 0.0f와 1.0f사이를 벗어나는 경우 화면에 안나온다고 보면 된다.
void	GetScreenXYFromXYZ( I4DyuchiGXGeometry* pGeometry, DWORD dwViewportIndex, VECTOR3* pInWhere, VECTOR3* pOutScreen);

#endif