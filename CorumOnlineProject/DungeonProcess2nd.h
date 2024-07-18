#pragma once


struct OBJECT_DESC;


void CheckAndProcForGroupWnd();
void PickingObjectTypeMonster(LPObjectDesc pObjDesc);
void PickingObjectTypePlayer(LPObjectDesc pObjDesc);
void PickingObjectTypeTrader_Monster(LPObjectDesc pObjDesc);
void PickingObjectTypeItem(LPObjectDesc pObjDesc);
void DungeonMove();