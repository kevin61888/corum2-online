#pragma once

// #include <BaseLibrary/Utility/Macro/DebugMacros.h>
// #todo ("COMMAND_SWITCHER_DATA에 순서를 맞출필요 없도록 수정")
// #todo ("리턴값 처리기 추가")

#define DECLARE_COMMAND_SWITCHER( cmName, argType ) \
extern bool Run##cmName( int nID, ##cmName *pClass, argType lpData ); \
extern bool Check##cmName( void );

#define RUN_COMMAND_SWITCHER( cmName, ID, cmData, argData ) \
Run##cmName( ID, cmData, argData )

#define CHECK_COMMAND_SWITCHER( cmName ) \
Check##cmName()

#define BEGIN_COMMAND_SWITCHER( csMax, cmName, argType ) \
const int MAX_##cmName = csMax; \
typedef struct str_##cmName { \
	int iID; \
	int (##cmName::*Function)( argType lpData ); \
} type##cmName; \
\
extern type##cmName data##cmName[MAX_##cmName]; \
\
bool Check##cmName( void ) \
{ \
	for( int i=0; i < MAX_##cmName; i++ ) { \
		if( data##cmName[i].Function == NULL ) \
			continue; \
		if( data##cmName[i].iID != 0 ) { \
			if( data##cmName[i].iID != i ) \
				return false; \
		} \
	} \
	return true; \
} \
\
bool Run##cmName( int nID, ##cmName *pClass, argType lpData ) \
{ \
	if( nID < 0 || nID >= MAX_##cmName ) \
		return false; \
	if( data##cmName[nID].Function == NULL ) \
		return false; \
	(pClass->*(data##cmName[nID].Function))( lpData ); \
	return true; \
} \
\
type##cmName data##cmName[MAX_##cmName] = {

#define COMMAND_SWITCHER_DATA( cmName, ID, Function ) \
	{ ID, ##cmName::Function },

#define END_COMMAND_SWITCHER()		};

