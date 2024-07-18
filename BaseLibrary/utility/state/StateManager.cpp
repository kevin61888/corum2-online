//
// File: CStateManager.cpp
// Desc: StateManager class
//
// Auth: Standil (cpascal@nownuri.net)
// Adapted from Role Playing Games with DirectX book (Jim Adams)
//

#include "../../StdAFX.h"
#include "StateManager.h"

CStateManager::CStateManager() : m_StateParent( NULL )
{
}

CStateManager::~CStateManager()
{
	PopAll();
}

void CStateManager::PopAll( void )
{
	while( Pop() == TRUE );
}

void CStateManager::Push( void (*Function)() )
{
	// Don't push a NULL value
	if( Function != NULL ) {
		// Allocate a new state and push it on stack
		sState *StatePtr = new sState;
		StatePtr->Function = Function;
		StatePtr->pNext = m_StateParent;
		m_StateParent = StatePtr;
	}
}

BOOL CStateManager::Pop( void )
{
	sState *StatePtr = m_StateParent;

	// Remove the head of stack (if any)
	if( StatePtr != NULL ) {
		m_StateParent = StatePtr->pNext;
		delete StatePtr;
	}

	// return TRUE if more states exist, FALSE otherwise
	if( m_StateParent == NULL )
		return FALSE;
	return TRUE;
}

BOOL CStateManager::Process( void )
{
	// return an error if no more states
	if( m_StateParent == NULL )
		return FALSE;
	// Process the top-most state (if any)
	m_StateParent->Function();
	return TRUE;
}


/*

e.g.

CStateManager SM;

// Macro to ease the use of MessageBox function
#define MB(s) MessageBox( NULL, s, s, MB_OK );

void Func1() { MB("1"); SM.Pop(); }
void Func2() { MB("2"); SM.Pop(); }
void Func3() { MB("3"); SM.Pop(); }

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow )
{
	SM.Push( Func1 );
	SM.Push( Func2 );
	SM.Push( Func3 );
	while( SM.Process() == TRUE );
}

*/
