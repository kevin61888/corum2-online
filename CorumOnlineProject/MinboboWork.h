#pragma once

#include "Struct.h"

struct	DSTC_PORTAL_JOIN_USER;
class	CUser;

void	CreateMainPlayerByChangeLayer();
void	ReleaseMainPlayerByChangeLayer();
void	ReleaseDungeonEffect();
void	PortalEffectFuncMainPlayer(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void	PortalEffectFuncInsideMainPlayer(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void	ChangeLayerForPortal(DSTC_PORTAL_JOIN_USER* pPacket);
void	PortalAppearFuncUser(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void	PortalEffectFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void	ToggleUserShadow(CUser* pUser, BYTE bEnable);