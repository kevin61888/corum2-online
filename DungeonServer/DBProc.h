#pragma once


void QueryTypePresentCountCheck(DBRECEIVEDATA* pResult);
void QueryTypeUserCharacter(DBRECEIVEDATA* pResult);
void QueryTypeBankUserInfo(DBRECEIVEDATA* pResult);
void QueryTypeUserPlayerShop(DBRECEIVEDATA* pResult);
void QueryTypeUUIDDungeonJoin(DBRECEIVEDATA* pResult);
void QueryTypeCreateGuardian(DBRECEIVEDATA* pResult);
void QueryTypeUUIDGuardianInfo(DBRECEIVEDATA* pResult);
void QueryTypeChangeDungeonOwner(DBRECEIVEDATA* pResult);
void QueryTypeUserPlayershop(DBRECEIVEDATA* pResult);
void QueryTypeUserUpdate(DBRECEIVEDATA* pResult);
void QueryTypeUserBankUpdate(DBRECEIVEDATA* pResult);
void QueryTypeTrade(DBRECEIVEDATA* pResult);


void UpdateUserDBThread(CUser* pUser);		
void UpdateUserDB(CUser* pUser);
void UpdateAllUserDB();							
void UpdateGuardianDB(CMonster* pMonster);
