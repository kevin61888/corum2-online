#pragma once

#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IRenderer.h"
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IGeometry.h"
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IExecutive.h"
#include "./../SS3D_0719/4dyuchigx/4dyuchiGRX_common/IAudio.h"
#else
#include "./../4dyuchiGRX_common/IRenderer.h"
#include "./../4dyuchiGRX_common/IGeometry.h"
#include "./../4dyuchiGRX_common/IExecutive.h"
#include "./../4dyuchiGRX_common/IAudio.h"
#endif
DWORD __stdcall GXPlayerPROC(I4DyuchiGXExecutive* pExecutive,GXOBJECT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
