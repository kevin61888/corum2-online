//
// File: CProcessManager.h
// Desc: ProcessManager class
//
// Auth: Standil (cpascal@nownuri.net)
// Adapted from Role Playing Games with DirectX book (Jim Adams)
//

#ifndef _LOWLIB_PROCESSMANAGER_H_
#define _LOWLIB_PROCESSMANAGER_H_

class CProcessManager
{
	// A structure that stores a function pointer and linked list
	typedef struct sProcess {
		void (*Function)( void );
		sProcess *pNext;
	} sProcess;

protected :
	sProcess *m_ProcessParent;	// The top state in the stack
								// the head of the stack
public:
	CProcessManager();
	~CProcessManager();

	void Push( void (*Function)() );
	BOOL Pop( void );
	void PopAll( void );
	void Process( void );
};

#endif /* _LOWLIB_CPROCESSMANAGER_H_ */

