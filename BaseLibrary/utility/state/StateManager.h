//
// File: CStateManager.h
// Desc: StateManager class
//
// Auth: Standil (cpascal@nownuri.net)
// Adapted from Role Playing Games with DirectX book (Jim Adams)
//

#ifndef _LOWLIB_STATEMANAGER_H_
#define _LOWLIB_STATEMANAGER_H_

class CStateManager
{
	// A structure that stores a function pointer and linked list
	typedef struct sState {
		void (*Function)( void );
		sState *pNext;
	} sState;

protected :
	sState *m_StateParent;	// The top state in the stack
							// the head of the stack
public:
	CStateManager();
	~CStateManager();

	void Push( void (*Function)() );
	BOOL Pop( void );
	void PopAll( void );
	BOOL Process( void );
};

#endif /* _LOWLIB_CSTATEMANAGER_H_ */

