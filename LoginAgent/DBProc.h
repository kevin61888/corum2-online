#ifndef __DBPROC_H__
#define __DBPROC_H__

#pragma once

#include "GlobalDefine.h"
#include "../CommonServer/CommonHeader.h"
#include "User.h"

#pragma pack(push,1)

/*
<-----     Login Procedure Return Value For Corum     ------>
SELECT  0, 0, @propid, 	1, @eday , 0, 0 ,@id_regdate    --로그인 성공	
SELECT  1, 0, 0,		0, @eeday, 0, 0 ,@id_regdate 	--SQL 디비 에러,로그인실패
SELECT  2, 0, 0, 		0, @eeday, 0, 0 ,@id_regdate 	--없는 아이디 입니다.
SELECT  3, 0, 0, 		0, @eeday, 0, 0 ,@id_regdate 	--비밀번호가 틀립니다.
SELECT  4, 0, @propid, 	0, @eeday, 0, 0 ,@id_regdate 	--이미 게임에 접속 중입니다. 
SELECT  5, 0, @propid, 	0, @eeday, 0, 0 ,@id_regdate 	--게임 블럭 유저
SELECT  6, 0, @propid,  0, @eeday, 0, 0 ,@id_regdate 	--14세미만 임시가입자로 부모 동의 처리 후 게임가능합니다.
*/

/*
<-----     Login Procedure Return Value For NetMarble     ------>
select 0,0,@idx,1,getdate() ,0,0, @id_regdate	-- 성공
select 3,0,0,	0,getdate() ,0,0, @id_regdate	-- 비밀번호 틀림 	
select 4,0,@idx,0,getdate() ,0,0, @id_regdate	-- 이미 로그인되어있음
select 5,0,@idx,0,getdate() ,0,0, @id_regdate	-- 불량블럭유저
select 6,0,@idx,0,getdate() ,0,0, @id_regdate	-- SQL 오류.
*/

struct LOGIN_RESULT
{
	DWORD dwResult;				//로그인/(0:무조건 통과, 1:로그인ok, 2:없는 아이디, 3:틀린 비번, 4:로그인중인 아이디, 5:제재유저, 6:테스터 아니다, 7:이미 PC방 아이피 1개 사용중)
	DWORD dwPrevUserServerSet;	//중복로그인시 이전 유저가 로그인 되어있던 서버셋 번호 
	DWORD dwPropID;				//propid
	DWORD dwRestrictEx;			//결재관련(0:돈을내시오, 10:무조건무료, 9:1시간무료, 2:개인 날짜가남아서 통과, 1:pc방 정액통과, 3:pc방 종량통과, 4: 유저 종량 통과 
	DBTIMESTAMP eday;			//eday	
	DWORD dwRemainTime;			//종량제 일때 남은시간 
	DWORD dwCoupon;				//처음 회원가입한 유저 14일 체험판 사용가능 여부 (1:사용가능, 2:전투레벨로 사용불가, 3:경제레벨로 사용불가)
	DBTIMESTAMP id_regdate ;   //계정 생성일 ( id_regdate ) 재원 050323
};

struct LOGIN_RESULT_WITH_PROPID
{
	DWORD dwResult;				//로그인/(0:무조건 통과, 1:로그인ok, 2:없는 아이디, 3:틀린 비번, 4:로그인중인 아이디, 5:제재유저, 6:테스터 아니다, 7:이미 PC방 아이피 1개 사용중)
	DWORD dwPrevUserServerSet;	//중복로그인시 이전 유저가 로그인 되어있던 서버셋 번호 
	char  szPropID[15];			//propid
	DWORD dwRestrictEx;			//결재관련(1:ip eday, 2:유저의 eday, 3:ip time, 4:유저의 time, 5:결재요망)
	DBTIMESTAMP eday;			//eday	
	DWORD dwRemainTime;			//종량제 일때 남은시간 
	DWORD dwCoupon;				//처음 회원가입한 유저 14일 체험판 사용가능 여부 (1:사용가능, 2:전투레벨로 사용불가, 3:경제레벨로 사용불가)
	DBTIMESTAMP id_regdate ;   //계정 생성일 ( id_regdate ) 재원 050323
};

struct JUMIN_CHECK_RESULT
{
	DWORD dwResult;
	char  szCharacterName[20];
};

struct BLOCK_TITLE
{
	DWORD dwType;
	char  szReason[0xff];
};

struct INIT_BINARY_VALUE
{
	CItem			m_pEquip[ MAX_EQUIP ];
	CItem			m_pInv_Large[ MAX_INV_LARGE ];
	CItem			m_pInv_Small[ MAX_INV_SMALL ];
	CItem			m_pBelt[ MAX_BELT ];
	BYTE			m_pwSkillLevel[ MAX_SKILL ];
	DWORD			dwMoney;
};

struct CREATE_CHARACTER_RESULT
{
	DWORD dwResult;
	DWORD dwChar_Index;
	DWORD dwCharCount; //재원 050323 
};

struct DELETE_CHARACTER_RESULT
{
	DWORD	dwResult;
	char	szDeletedCharacterName[20];
};

struct BILLING_CHECK_RESULT
{
	DWORD dwPropID;
	DWORD nBillingType;
	DBTIMESTAMP eday;
	DWORD	dwRemainTime;
	DWORD	dwContinue;		//0이면 이 유저는 결제해야 된다.. 짤라라..

};

#pragma pack(pop)

struct DBRECEIVEDATA;

void QueryTypeLogin(DBRECEIVEDATA* pResult);
void QueryTypeLoginFree(DBRECEIVEDATA* pResult);
void QueryTypeChrSelectInfo(DBRECEIVEDATA* pResult);
void QueryTypeCheckMonitoring(DBRECEIVEDATA* pResult);
void QueryTypeIsAliveUser(DBRECEIVEDATA* pResult);
void QueryTypeBillingCheck(DBRECEIVEDATA* pResult);
void QueryTypeBankItemCheck(DBRECEIVEDATA* pResult);
void QueryTypeCreateNewChar(DBRECEIVEDATA* pResult);
void QueryTypeDeleteCharacter(DBRECEIVEDATA* pResult);
void QueryTypeJuminCheck(DBRECEIVEDATA* pResult);
void QueryTypeCreateCharInitBinary(DBRECEIVEDATA* pResult);
void QueryTypeGMCheck(DBRECEIVEDATA* pResult);
void QueryTypeBlockTitle(DBRECEIVEDATA* pResult);
void QueryTypeWarningTitle(DBRECEIVEDATA* pResult);


#endif // __DBPROC_H__