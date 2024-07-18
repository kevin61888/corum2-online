//
// File: CStateManagerEx.cpp
// Desc: StateManager class
//
// Auth: Standil (cpascal@nownuri.net)
// Adapted from Role Playing Games with DirectX book (Jim Adams)
//

#include "../../StdAFX.h"
#include "StateManagerEx.h"

CStateManagerEx::CStateManagerEx( void ) : m_StateParent( NULL )
{
}

CStateManagerEx::~CStateManagerEx( void )
{
	PopAll();
}

void CStateManagerEx::Push( void (*Function)( void *pPointer, LONG dwPurpose ), void *pDataPointer )
{
	// Don't push a NULL value
	if( Function != NULL ) {
		// Allocate a new state and push it on stack
		sState *StatePtr = new sState;
		StatePtr->Function = Function;
		StatePtr->pNext = m_StateParent;
		m_StateParent = StatePtr;
		StatePtr->Function( pDataPointer, PURPOSE_INITIALIZE );
	}
}

BOOL CStateManagerEx::Pop( void *pDataPointer )
{
	sState *StatePtr = m_StateParent;

	// Remove the head of stack (if any)
	if( StatePtr != NULL ) {
		StatePtr->Function( pDataPointer, PURPOSE_SHUTDOWN );
		m_StateParent = StatePtr->pNext;
		delete StatePtr;
	}

	// return TRUE if more states exist, FALSE otherwise
	if( m_StateParent == NULL )
		return FALSE;
	return TRUE;
}

void CStateManagerEx::PopAll( void *pDataPointer )
{
	while( Pop( pDataPointer ) == TRUE );
}

BOOL CStateManagerEx::Process( void *pDataPointer, LONG lPurpose )
{
	// return an error if no more states
	if( m_StateParent == NULL )
		return FALSE;
	// Process the top-most state (if any)
	m_StateParent->Function( pDataPointer, lPurpose );
	return TRUE;
}

PSExFunctionType CStateManagerEx::GetState( void )
{
	if( m_StateParent == NULL )
		return NULL;
	return m_StateParent->Function;
}

void CStateManagerEx::Set( void (*Function)( void *pPointer, LONG dwPurpose ), void *pDataPointer )
{
	Pop();
	Push( Function, pDataPointer );
}


/*

e.g.

PSEXCMD( Func1 );
PSEXCMD( Func2 );

CStateManagerEx SM;

// Macro to ease the use of MessageBox function
#define MB(s) MessageBox( NULL, s, s, MB_OK );

DWORD dwFrame1, dwFrame2;

// PSEXCMD macro = void Func1( void *pDataPointer, LONG lPurpose )
PSEXCMD( Func1 )
{
	switch( lPurpose ) {
	case PURPOSE_INITIALIZE :	MB( "Func1::Initialized!" );		dwFrame1 = 0;			break;
	case PURPOSE_SHUTDOWN	:	MB( "Func1::Uninitialized!" );		dwFrame1 = 0xFFFFFFFF;	break;
	case PURPOSE_FRAME		:
		MB( "Func1::Frame %u\n", dwFrame1 );
		dwFrame1++;
		if( dwFrame1 > 10 ) {
			SM.Pop();			// or SM.PopAll()
			SM.Push( Func2 );
		}			
		break;
	case PURPOSE_USER		:	MB( "Func1::UserPurpose!" );								break;
	}
}

// PSEXCMD macro = void Func2( void *pDataPointer, LONG lPurpose )
PSEXCMD( Func2 )
{
	switch( lPurpose ) {
	case PURPOSE_INITIALIZE :	MB( "Func2::Initialized!" );		dwFrame2 = 0;			break;
	case PURPOSE_SHUTDOWN	:	MB( "Func2::Uninitialized!" );		dwFrame2 = 0xFFFFFFFF;	break;
	case PURPOSE_FRAME		:
		MB( "Func2::Frame %u\n", dwFrame2 );
		dwFrame2++;
		if( dwFrame2 > 10 )
			SM.Pop();
		break;
	case PURPOSE_USER		:	MB( "Func2::UserPurpose!" );								break;
	case PURPOSE_USER+1		:	MB( "Func2::UserPurpose+1!" );								break;
	}
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow )
{
	SM.Push( Func1 );
	while( SM.Process() == TRUE );

// TO call UserPurpose
	SM.Process( NULL, PURPOSE_USER+1 );
	SM.Process( NULL, PURPOSE_USER+1 );
}

*/
