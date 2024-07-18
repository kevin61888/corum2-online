#ifndef __ACOUNT_H__
#define __ACOUNT_H__


#if defined		(DEV_MODE)
#	define DEFAULT_GAME_DB					"Corum"
#	define DEFAULT_GAME_DB_ACCOUNT			"potato"
#	define DEFAULT_GAME_DB_PASSWORD			"5378abc"
#	define DEFAULT_TOTAL_DB					"Corum_Total_DB"
#	define DEFAULT_TOTAL_DB_ACCOUNT			"potato"
#	define DEFAULT_TOTAL_DB_PASSWORD		"5378abc"
#	define DEFAULT_MEMBER_DB				"Member"
#	define DEFAULT_MEMBER_DB_ACCOUNT		"potato"
#	define DEFAULT_MEMBER_DB_PASSWORD		"5378abc"
#	define DEFAULT_LOG_DB_ACCOUNT			"potato"
#	define DEFAULT_LOG_DB_PASSWORD			"5378abc"

#elif defined	(JAPAN_LOCALIZING)
#	define DEFAULT_GAME_DB					"Corum"
#	define DEFAULT_GAME_DB_ACCOUNT			"potato"
#	define DEFAULT_GAME_DB_PASSWORD			"ajtwlssja"
#	define DEFAULT_TOTAL_DB					"Total_DB"
#	define DEFAULT_TOTAL_DB_ACCOUNT			"potato"
#	define DEFAULT_TOTAL_DB_PASSWORD		"ajtwlssja"
#	define DEFAULT_MEMBER_DB				"CORUM"
#	define DEFAULT_MEMBER_DB_ACCOUNT		"potato"
#	define DEFAULT_MEMBER_DB_PASSWORD		"ajtwlssja"
#	define DEFAULT_LOG_DB_ACCOUNT			"potato"
#	define DEFAULT_LOG_DB_PASSWORD			"ajtwlssja"

#elif defined	(CHINA_LOCALIZING)
#	define DEFAULT_GAME_DB					"Corum"
#	define DEFAULT_GAME_DB_ACCOUNT			"potato"
#	define DEFAULT_GAME_DB_PASSWORD			"ajtwlssja"
#	define DEFAULT_TOTAL_DB					"Total_DB"
#	define DEFAULT_TOTAL_DB_ACCOUNT			"potato"
#	define DEFAULT_TOTAL_DB_PASSWORD		"ajtwlssja"
#	define DEFAULT_MEMBER_DB				"Member"
#	define DEFAULT_MEMBER_DB_ACCOUNT		"potato"
#	define DEFAULT_MEMBER_DB_PASSWORD		"ajtwlssja"
#	define DEFAULT_LOG_DB					"Log_Corum_"
#	define DEFAULT_LOG_DB_ACCOUNT			"potato"
#	define DEFAULT_LOG_DB_PASSWORD			"ajtwlssja"



#elif defined	(TAIWAN_LOCALIZING)
#	if defined (TAIWAN_INTERNAL_DEVELOP_VERSION)	// append by seven [12/6/2004]
#	define DEFAULT_GAME_DB					"Corum"
#	define DEFAULT_GAME_DB_ACCOUNT			"potato"
#	define DEFAULT_GAME_DB_PASSWORD			"5378abc"
#	define DEFAULT_TOTAL_DB					"Corum_Total_DB"
#	define DEFAULT_TOTAL_DB_ACCOUNT			"potato"
#	define DEFAULT_TOTAL_DB_PASSWORD		"5378abc"
#	define DEFAULT_MEMBER_DB				"Member_tw"
#	define DEFAULT_MEMBER_DB_ACCOUNT		"potato"
#	define DEFAULT_MEMBER_DB_PASSWORD		"5378abc"
#	define DEFAULT_LOG_DB					"Log_Corum_"
#	define DEFAULT_LOG_DB_ACCOUNT			"y2jinc"
#	define DEFAULT_LOG_DB_PASSWORD			"dream7"

#	else
// !TAIWAN_INTERNAL_DEVELOP_VERSION
#	define DEFAULT_GAME_DB					"Corum"
#	define DEFAULT_GAME_DB_ACCOUNT			"potato"
#	define DEFAULT_GAME_DB_PASSWORD			"8x0ozs1xos"
#	define DEFAULT_TOTAL_DB					"Total_DB"
#	define DEFAULT_TOTAL_DB_ACCOUNT			"potato"
#	define DEFAULT_TOTAL_DB_PASSWORD		"8x0ozs1xos"
#	define DEFAULT_MEMBER_DB				"Member"
#	define DEFAULT_MEMBER_DB_ACCOUNT		"potato"
#	define DEFAULT_MEMBER_DB_PASSWORD		"8x0ozs1xos"
#	define DEFAULT_LOG_DB					"Log_Corum_"
#	define DEFAULT_LOG_DB_ACCOUNT			"potato"
#	define DEFAULT_LOG_DB_PASSWORD			"8x0ozs1xos"
#	endif

#else
#	define DEFAULT_GAME_DB					"Corum"
#	define DEFAULT_GAME_DB_ACCOUNT			"GameSrv"
#	define DEFAULT_GAME_DB_PASSWORD			"vhrwnQkdqhdxn!"
#	define DEFAULT_TOTAL_DB					"Total_DB"
#	define DEFAULT_TOTAL_DB_ACCOUNT			"GameSrv"
#	define DEFAULT_TOTAL_DB_PASSWORD		"vhrwnQkdqhdxn!"
#	define DEFAULT_MEMBER_DB				"Member"
#	define DEFAULT_MEMBER_DB_ACCOUNT		"GameSrv"
#	define DEFAULT_MEMBER_DB_PASSWORD		"vhrwnQkdqhdxn!"
#	define DEFAULT_LOG_DB					"Total_DB"
#	define DEFAULT_LOG_DB_ACCOUNT			"GameSrv"
#	define DEFAULT_LOG_DB_PASSWORD			"vhrwnQkdqhdxn!"

#endif


#endif
