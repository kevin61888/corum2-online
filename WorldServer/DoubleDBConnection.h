/*
 *	added by minjin. 2004. 11. 12.
 *	DB Connection 을 두개 유지할 수 있게 하자..
 *	
 *	사용법 : 두번째 커넥션으로 보내고자 하는 쿼리 전송 함수 자체의 위와 아래를
 *	
 *	SECOND_DB_OPERATION_START()
 *		????????????->THExecuteSQL(".............", ....... );
 *	SECOND_DB_OPERATION_END()
 *
 *	위와같이 사용한다.
 */
#ifndef __DOUBLE_DB_CONNECTION_H__
#define __DOUBLE_DB_CONNECTION_H__

/*
 *	GAME_DB1 과 GAME_DB2 는 main.cpp 에 정의되어 있고,
 *	stdafx.h 에 extern 선언되어 있다.
 */
#pragma once

	#ifdef DOUBLE_DB_CONNECTION

		#define	SECOND_DB_OPERATION_START()\
			GAME_DB = (BYTE)GAME_DB2;

		#define	SECOND_DB_OPERATION_END()\
			GAME_DB = (BYTE)GAME_DB1;

	#else

		#define SECOND_DB_OPERATION_START()

		#define SECOND_DB_OPERATION_END()

	#endif	//	DOUBLE_DB_CONNECTION


#endif	//	__DOUBLE_DB_CONNECTION_H__