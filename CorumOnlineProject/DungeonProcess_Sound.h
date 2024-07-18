#ifndef  __DUNGEONPROCESS_SOUND_H__
#define	 __DUNGEONPROCESS_SOUND_H__

#include "SoundLib.h"

int GetDungeonLayerProperty(DWORD dwLayerID);

ISoundEffect * 
_PlaySound( DWORD dwSoundIndex, 
		    DWORD dwType, 
			DWORD dwKind, 
			VECTOR3 & v3Pos, 
			bool bLoop);

void PlayItemSound(WORD	wItemId);

void SelectBGM(WORD wID, 
			   BYTE byteDungeonLayer, 
			   DWORD dwLayerID);


#endif
// __DUNGEONPROCESS_SOUND_H__