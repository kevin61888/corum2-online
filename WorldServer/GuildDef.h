#ifndef __GUILD_DEF_H__
#define __GUILD_DEF_H__

#pragma once

#define		MAX_GUILD					200
#define		MAX_GUILD_USER				100
#define		MAX_GUILD_FILENAME_LENGTH	50

#ifdef JAPAN_LOCALIZING
#	define	GUILDWAR_TIME				24
#	define	MAX_GUILD_WAR				5
#else
#	define	GUILDWAR_TIME				72
#	define	MAX_GUILD_WAR				8
#endif

#define		MAX_GUILD_TEXT				10

#endif // __GUILD_DEF_H__