#ifndef __UNIQUE_PROCESS__H__
#define __UNIQUE_PROCESS__H__


#pragma  once


#include "CommonServer.h"


#define GKO_SUCCESS		1
#define GKO_ERROR		-1
#define GKO_EXIST		0


int		DECLSPECIFIER CreateGlobalKernelObject(DWORD dwServerID); // FALSE: 이미 실행중인 서버임!
void	DECLSPECIFIER DeleteGlobalKernelObject();


#endif 