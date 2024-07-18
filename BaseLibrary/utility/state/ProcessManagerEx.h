//
// File: CProcessManager.h
// Desc: ProcessManager class
//
// Auth: Standil (cpascal@nownuri.net)
// Adapted from Role Playing Games with DirectX book (Jim Adams)
//

#ifndef _LOWLIB_PROCESSMANAGEREX_H_
#define _LOWLIB_PROCESSMANAGEREX_H_

#include "StateManagerEx.h"

class CProcessManagerEx
{
	// A structure that stores a function pointer and linked list
	typedef struct sProcess {
		PSEXCMD_RETTYPE (*Function)( PSEXCMD_ARGUMENT );
		sProcess *pNext;
	} sProcessEx;

protected :
	sProcessEx *m_ProcessParent;	// The top state in the stack
								// the head of the stack
public:
	CProcessManagerEx();
	~CProcessManagerEx();

	void Push( void (*Function)( void *pPointer, LONG dwPurpose ), void *pDataPointer = NULL );
	BOOL Pop( void *pDataPointer = NULL );
	void PushBack( void (*Function)( void *pPointer, LONG dwPurpose ), void *pDataPointer = NULL );
//	void PopBack( void *pDataPointer = NULL );
	void PopAll( void *pDataPointer = NULL );
	void Process( void *pDataPointer = NULL, LONG lPurpose = PURPOSE_FRAME );
};

#endif /* _LOWLIB_PROCESSMANAGER_H_ */

