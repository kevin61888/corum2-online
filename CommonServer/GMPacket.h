#pragma once


#pragma pack(push,1)


struct Protocol_GM
{
	enum 
	{
		CMD_QUICK_MOVE				= 200,
	};
};


//-----------------------------------------------------------------------------
// GM¿ë Quick Move Packet
//-----------------------------------------------------------------------------
struct CTDS_GM_QUICK_MOVE
{
	BYTE	bGameStatus;
	BYTE	bHeader;
	VECTOR2	v2MoveStartPos;		// Move start position..
	VECTOR2	v2MoveDirection;	// Move direction..
	WORD	wDestX;
	WORD	wDestZ;

	CTDS_GM_QUICK_MOVE()	{	bGameStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_GM::CMD_QUICK_MOVE;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_GM_QUICK_MOVE);	}
};


#pragma pack(pop)