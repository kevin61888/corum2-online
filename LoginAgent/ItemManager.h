#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#pragma once

#include "./../CommonServer/AbyssHash64.h"


#define	__ITEM_WEAPON__				0
#define __ITEM_AROMR__				1
#define	__ITEM_SUPPLIES__			2
#define __ITEM_ZODIAC__				3
#define	__ITEM_RIDE__				4
#define	__ITEM_SPECIAL__			5
#define	__ITEM_GUARDIAN__			6
#define __ITEM_MAGICARRAY__			7
#define __ITEM_MATERIALS__			8
#define __ITEM_MIX_UPGRADE__		9
#define __ITEM_MAGICFIELD_ARRAY__	10
#define __ITEM_CONSUMABLE__			11
#define __ITEM_UPGRADE__			12
#define __ITEM_LIQUID__				13
#define __ITEM_EDITION__			14


#define ITEM_DISTRIBUTE				200


//#define	ITEM_LARGE					0
//#define	ITEM_SMALL					1
//#define	ITEM_NONE					2
#define	ITEM_MONEYS					3

#define	ITEM_GUILD_INDEX			12200
#define	ITEM_CLEN_INDEX				12201

#define	MAX_LARGE_ITEM_INDEX		20
#define	MAX_SMALL_ITEM_INDEX		70

// Item Index //
#define	ITEM_WEAPONST_INDEX			0
#define	ITEM_WEAPONEND_INDEX		7
#define	ITEM_ARMORST_INDEX			10
#define	ITEM_ARMOREND_INDEX			17
#define	ITEM_GUARDIAN_INDEX			19
#define	ITEM_ZODIAC_INDEX			40
#define	ITEM_SUPPLIES_INDEX			50
#define	ITEM_SPECIALST_INDEX		60
//#define	ITEM_SPECIALEND_INDEX		65
//#define	ITEM_MONEY_INDEX			100
#define	ITEM_RIDE_INDEX				20
#define	ITEM_MAGICARRAY_INDEX		80
#define ITEM_MATERIALST_INDEX		70
//#define ITEM_MATERIALEND_INDEX		76
#define ITEM_MIXUPGRADEST_INDEX		31
#define ITEM_MIXUPGRADEEND_INDEX	33
#define ITEM_UPGRADE_INDEX			34
#define ITEM_LIQUID_INDEX			35
#define ITEM_EDITION_INDEX			36
#define ITEM_MAGICFIELDARRAY_INDEX	90
#define ITEM_CONSUMABLEST_INDEX		101
#define ITEM_CONSUMABLEEND_INDEX	102


#define __ITEM_PORTAL_INDEX__		8002
#define __ITEM_PORTALUSED_INDEX__	8010
#define	__ITEM_PORTAL_VILL1__		8012
#define	__ITEM_PORTAL_VILL2__		8013
#define	__ITEM_PORTAL_VILL3__		8014
#define	__ITEM_PORTAL_VILL4__		8015
#define	__ITEM_PORTAL_VILL5__		8016
#define __ITEM_PARTY_PORTAL1__		16021
#define __ITEM_PARTY_PORTAL2__		16022
// 아이템 업그레이드에 필요한것들.
#define __ITEM_MIXUPGRADE_EXLIXIR__			32
#define __ITEM_MIXUPGRADE_JEWEL__			33
#define __ITEM_MIXUPGRADE_ZODIAC_ARIES__	8000
#define __ITEM_MIXUPGRADE_ZODIAC_VIRGO__	8005

#define __ITEM_STATUS_INT__				13001
#define __ITEM_SKILL_INT__				13002

#define __ITEM_STATUS_INTSTART__		13006
#define __ITEM_STATUS_INTEND__			13023

#define __ITEM_SKILL_INTSTART__			13024
#define __ITEM_SKILL_INTEND__			13038

// 파티 마법진 //
#define __ITEM_PARTY_PORTAL1__			16021
#define __ITEM_PARTY_PORTAL2__			16022
#define __ITEM_PARTY_LIFELINK1__		16001
#define __ITEM_PARTY_LIFELINK2__		16002
#define __ITEM_PARTY_LIFELINK3__		16003
#define __ITEM_PARTY_LIFELINK4__		16004
#define __ITEM_PARTY_LIFELINK5__		16005
#define __ITEM_PARTY_AMBUSH1__			16041
#define __ITEM_PARTY_AMBUSH2__			16042
#define __ITEM_PARTY_AMBUSH3__			16043
#define __ITEM_PARTY_AMBUSH4__			16044
#define __ITEM_PARTY_AMBUSH5__			16045
#define __ITEM_PARTY_ST1__				16061
#define __ITEM_PARTY_ST2__				16062
#define __ITEM_PARTY_ST3__				16063
#define __ITEM_PARTY_ST4__				16064
#define __ITEM_PARTY_ST5__				16065

// Item Sub ID
#define	ITEM_LIFE_LINK_SUBID		1
#define	ITEM_SYNERGISTIC_TRAINING	2
#define ITEM_AMBUSH_SUBID			3
// Item Type //
#define	ITEM_TYPE_MONEY		0
#define	ITEM_TYPE_LARGE		1
#define	ITEM_TYPE_SMALL		2

#define MAX_ITEM_ATTR	8		// 2Byte * 8 = 16Byte

#define MAX_ITEM_TYPE			10
#define MAX_ITEM_CLASS			17

// iti File Type..
#define ITI_TYPE_BASEITEM		1000

// Item Type.. 
#define ITEM_TYPE_WEAPON_ARM1	10		// 한손무기
#define ITEM_TYPE_WEAPON_ARM2	20		// 양손무기
#define ITEM_TYPE_SHIELD		30		// 방패
#define ITEM_TYPE_ARMOR			40		// 방어구
#define ITEM_TYPE_BELT			50		// 벨트
#define ITEM_TYPE_GLOVE			60		// 장갑		
#define ITEM_TYPE_BOOTS			70		// 신발
#define ITEM_TYPE_NECK			80		// 목에 걸리는것
#define ITEM_TYPE_RING			90		// 반지
#define ITEM_TYPE_ONCE			100		// 소모성

// Item Class..
#define	ITEM_NUM_PER_OPTION2	10010
#define ITEM_NUM_PER_CLASS		100
#define ITEM_NUM_PER_OPTION		18		// 옵션이 붙을수 있는 아이템의 수.
#define ITEM_CLASS_SWARD		0
#define ITEM_CLASS_MACE			1
#define ITEM_CLASS_KATAL		2
#define ITEM_CLASS_SWARD2		3
#define ITEM_CLASS_MACE2		4
#define ITEM_CLASS_SPEAR		5
#define ITEM_CLASS_WAND			6
#define ITEM_CLASS_STAFF		7
#define ITEM_CLASS_HELMET		10
#define ITEM_CLASS_ARMOR		11
#define ITEM_CLASS_SHIELD		12
#define ITEM_CLASS_BELT			13
#define ITEM_CLASS_GLOVE		14
#define ITEM_CLASS_BOOTS		15
#define ITEM_CLASS_RING			16
#define ITEM_CLASS_AMULET		17
//-> 기타등등...

// Item Attr Code
#define	MAX_ITEM_KIND1_CODE		32		// 옵션1종류의 끝번호
#define MAX_ITEM_KIND2_CODE		306		// 옵션2종류의 끝번호
#define ITEM_ATTR_NONE															0
#define ITEM_ATTR_BLOCK															1		// 방어력.
#define ITEM_ATTR_AVOID_POINT													2		// 아이템 재료.
#define ITEM_ATTR_AVOID_PERSENT													3		// 공격력.
#define ITEM_ATTR_ATTACK_SPEED													4		// 공격 속도.
#define ITEM_ATTR_MOVE_SPEED													5		// 이동 속도.
#define ITEM_ATTR_EGO															6		// EGO 상승.
#define ITEM_ATTR_STR															7		// STR 상승.
#define ITEM_ATTR_DEX															8		// DEX 상승.
#define ITEM_ATTR_INT															9		// INT 상승.
#define ITEM_ATTR_VIT															10		// VIT 상승.
#define ITEM_ATTR_HP_POINT														11		// HP 상승 포인트.
#define ITEM_ATTR_HP_PERCENT													12		// HP 상승 퍼센트.
#define ITEM_ATTR_SP_POINT														13		// SP 상승 포인트.
#define ITEM_ATTR_SP_PERCENT													14		// SP 상승 퍼센트.
#define ITEM_ATTR_AA_POINT														15		// 공격 정확도 포인트.
#define ITEM_ATTR_AA_PERCENT													16		// 공격 정확도 페센트.
#define ITEM_ATTR_FIRERESIST													17		// 화염 저항력.
#define ITEM_ATTR_LIGHTRESIST													18		// 전기 저항력.
#define ITEM_ATTR_ICERESIST														19		// 얼음 저항력.
#define ITEM_ATTR_POIRESIST														20		// 독 저항력.
#define ITEM_ATTR_ALLRESIST														21		// 모든 저항력.
#define ITEM_ATTR_PHYRESIST														22		// 물리 저항력.
#define ITEM_ATTR_ALLSKILL														23		// 올 스킬
#define ITEM_ATTR_MANASKILL														24
#define ITEM_ATTR_AURASKILL														25
#define ITEM_ATTR_DEVINESKILL													26
#define ITEM_ATTR_CHAKRASKILL													27
#define ITEM_ATTR_SUMMONSKILL													28
#define ITEM_ATTR_HEALSPEEDHP													29
#define ITEM_ATTR_HEALSPEEDMP													30
#define ITEM_ATTR_ALLSTATUS														31
#define	ITEM_ATTR_LEVEL_PER_MAXHP												32
#define	ITEM_ATTR_LEVEL_PER_MAXMP												33
#define	ITEM_ATTR_LEVEL_PER_AA													34
#define	ITEM_ATTR_LEVEL_PER_AVOID												35
#define	ITEM_ATTR_BLOCKRATE														36
#define ITEM_ATTR_MAGIC_FIND_PBT												37
#define	ITEM_ATTR_KARZ_GAIN														38			// 카르츠 떨구는 양을 늘려주는 속성.
#define	ITEM_ATTR_CRITICAL														39
#define	ITEM_ATTR_SUPER_CRITICAL												40
#define	ITEM_ATTR_ATTACK_MIN													41
#define	ITEM_ATTR_ATTACK_MAX													42
#define	ITEM_ATTR_ATTACK_MAGIC													43
#define	ITEM_ATTR_ATTACK_FIRE													44
#define	ITEM_ATTR_ATTACK_ICE													45
#define	ITEM_ATTR_ATTACK_LIGHT													46
#define	ITEM_ATTR_ATTACK_POI													47
#define	ITEM_ATTR_SKILL_DMG_MAGIC												48	
#define	ITEM_ATTR_SKILL_DMG_FIRE												49
#define	ITEM_ATTR_SKILL_DMG_ICE													50
#define	ITEM_ATTR_SKILL_DMG_LIGHT												51
#define	ITEM_ATTR_SKILL_DMG_POI													52
#define	ITEM_ATTR_TARGET_ALL_RESIST												53
#define	ITEM_ATTR_TARGET_FIRE_RESIST											54
#define	ITEM_ATTR_TARGET_ICE_RESIST												55
#define	ITEM_ATTR_TARGET_LIGHT_RESIST											56
#define	ITEM_ATTR_TARGET_POI_RESIST												57
#define	ITEM_ATTR_TARGET_PHY_RESIST												58
#define	ITEM_ATTR_TARGET_AVOID													59
#define	ITEM_ATTR_STUN_TIME														60
#define	ITEM_ATTR_POI_TIME														61
#define	ITEM_ATTR_FROZEN_TIME													62
#define	ITEM_ATTR_FIRE_RESIST_POINT												63
#define	ITEM_ATTR_ICE_RESIST_POINT												64
#define	ITEM_ATTR_LIGHT_RESIST_POINT											65
#define	ITEM_ATTR_POI_RESIST_POINT												66
#define	ITEM_ATTR_PHY_RESIST_POINT												67
#define	ITEM_ATTR_ALL_RESIST_POINT												68
#define	ITEM_ATTR_FIRE_ABSORB_POINT												69
#define	ITEM_ATTR_FIRE_ABSORB													70
#define	ITEM_ATTR_ICE_ABSORB_POINT												71
#define	ITEM_ATTR_ICE_ABSORB													72
#define	ITEM_ATTR_LIGHT_ABSORB_POINT											73
#define	ITEM_ATTR_LIGHT_ABSORB													74
#define	ITEM_ATTR_POI_ABSORB_POINT												75
#define	ITEM_ATTR_POI_ABSORB													76
#define	ITEM_ATTR_ALL_ABSORB_POINT												77
#define	ITEM_ATTR_ALL_ABSORB													78
#define	ITEM_ATTR_PHY_DMG_TO_SP													79
#define	ITEM_ATTR_MOP_KILL_TO_SP												80
#define	ITEM_ATTR_MOP_KILL_TO_HP												81
#define	ITEM_ATTR_ATTACK_DMG_TO_HP												82
#define	ITEM_ATTR_ATTACK_DMG_TO_SP												83
#define	ITEM_ATTR_DMG_REFLECT_POINT												84
#define	ITEM_ATTR_DMG_REFLECT													85
#define	ITEM_ATTR_SKILL_LEV_AURA_ALL											86
#define	ITEM_ATTR_SKILL_LEV_AURA_ACTIVE											87
#define	ITEM_ATTR_SKILL_LEV_AURA_PACIVE											88
#define	ITEM_ATTR_SKILL_LEV_AURA_OVERDRIVE										89
#define	ITEM_ATTR_SKILL_LEV_DIVINE_ALL											90
#define	ITEM_ATTR_SKILL_LEV_DIVINE_ACTIVE										91
#define	ITEM_ATTR_SKILL_LEV_DIVINE_PACIVE										92
#define	ITEM_ATTR_SKILL_LEV_DIVINE_OVERDRIVE									93
#define	ITEM_ATTR_SKILL_LEV_SUMMON_ALL											94
#define	ITEM_ATTR_SKILL_LEV_SUMMON_ACTIVE										95
#define	ITEM_ATTR_SKILL_LEV_SUMMON_PACIVE										96
#define	ITEM_ATTR_SKILL_LEV_SUMMON_OVERDRIVE									97
#define	ITEM_ATTR_SKILL_LEV_CHAKRA_ALL											98
#define	ITEM_ATTR_SKILL_LEV_CHAKRA_ACTIVE										99
#define	ITEM_ATTR_SKILL_LEV_CHAKRA_PACIVE										100
#define	ITEM_ATTR_SKILL_LEV_CHAKRA_OVERDRIVE									101
#define	ITEM_ATTR_SKILL_LEV_MANA_ALL											102
#define	ITEM_ATTR_SKILL_LEV_MANA_ACTIVE											103
#define	ITEM_ATTR_SKILL_LEV_MANA_ACTIVE_ICE										104
#define	ITEM_ATTR_SKILL_LEV_MANA_ACTIVE_FIRE									105
#define	ITEM_ATTR_SKILL_LEV_MANA_ACTIVE_LIGHT									106
#define	ITEM_ATTR_SKILL_LEV_MANA_ACTIVE_EARTH									107
#define	ITEM_ATTR_SKILL_LEV_MANA_PACIVE											108
#define	ITEM_ATTR_SKILL_LEV_MANA_OVERDRIVE										109
#define	ITEM_ATTR_SIZE_SCALE													110
#define	ITEM_ATTR_ATTACK_NOR													111
#define	ITEM_ATTR_EXP_GAIN														112
#define	ITEM_ATTR_LEVEL_PER_ALL_MAGIC_RESIST									113
#define	ITEM_ATTR_LEVEL_PER_FIRE_RESIST											114
#define	ITEM_ATTR_LEVEL_PER_ICE_RESIST											115
#define	ITEM_ATTR_LEVEL_PER_LIGHT_RESIST										116
#define	ITEM_ATTR_LEVEL_PER_POI_RESIST											117
#define	ITEM_ATTR_LEVEL_PER_PHY_RESIST											118
#define	ITEM_ATTR_LEVEL_PER_DMG_MIN												119
#define	ITEM_ATTR_LEVEL_PER_DMG_MAX												120
#define	ITEM_ATTR_CASTINGTIME													121
#define	ITEM_ATTR_COOLTIME														122
#define	ITEM_ATTR_EXP_LOSS														123
#define	ITEM_ATTR_LEVEL_PER_DMG													124
#define	ITEM_ATTR_SIZE_SCALE_DECREASE											125
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_MEDITETION									126
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_FIREMISSILE									127
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_FIREMISSILE					128
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_FIREMISSILE					129
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_FIREBLAZE									130
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_FIREBLAZE						131
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_FIREBLAZE					132
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_MAGMAWALL									133
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_MAGMAWALL						134
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_MAGMAWALL					135
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_ICEMISSILE									136
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_ICEMISSILE					137
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_ICEMISSILE					138
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_ICEWAVE										139
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_ICEWAVE						140
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_ICEWAVE						141
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_ICEWALL										142
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_ICEWALL						143
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_ICEWALL						144
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_LIGHTNING									145
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_LIGHTNING						146
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_LIGHTNING					147
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_AIREALCOAT									148
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_AIREALCOAT					149
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_AIREALCOAT					150
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_THUNDERSTORM								151
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_THUNDERSTORM					152
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_THUNDERSTORM				153
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_LANDINGSUPPORT								154
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_LANDINGSUPPORT				155
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_LANDINGSUPPORT				156
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_EARTHQUAKE									157
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_EARTHQUAKE					158
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_EARTHQUAKE					159
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_METEOR										160
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_METEOR						161
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_METEOR						162
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_MANAEXPAND									163
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_MANAEXPAND					164
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_MANAEXPAND					165
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_SPEEDCASTING								166
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_SPEEDCASTING					167
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_SPEEDCASTING				168
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_EXPLOSION									169
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_EXPLOSION						170
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_EXPLOSION					171
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_CONCENTRATION								172
//#define	ITEM_ATTR_SKILL_LEVEL_SKILL(20)											173
//#define	ITEM_ATTR_SKILL_LEVEL(20)												174
//#define	ITEM_ATTR_SKILL_LEVEL(20)												175
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_REGENERATION								176
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_WEAPONMASTERY								177
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_DEFENSEMASTERY								178
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_AURARECHARGE								179
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_AURARECHARGE					180
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_AURARECHARGE				181
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_ZEAL										182
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_ZEAL							183
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_ZEAL						184
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_BASH										185
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_BASH							186
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_BASH						187
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_LIGHTNINGBREAK								188
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_LIGHTNINGBREAK				189
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_LIGHTNINGBREAK				190
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_SILENTBRANDYSHIP							191
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_SILENTBRANDYSHIP				192
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_SILENTBRANDYSHIP			193
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_ANTAGONIZE									194
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_ANTAGONIZE					195
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_ANTAGONIZE					196
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_AURABATTLER									197
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_AURABATTLER					198
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_AURABATTLER					199
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_LIFEFORCE									200
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_LIFEFORCE						201
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_LIFEFORCE					202
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_PRAY										203
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_PRAY							204
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_PRAY						205
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_BLESSING									206
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_BLESSING						207
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_BLESSING					208
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_WINDTALES									209
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_WINDTALES						210
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_WINDTALES					211
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_HOLDSHIELD									212
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_HOLDSHIELD					213
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_HOLDSHIELD					214
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_WINDFORCE									215
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_WINDFORCE						216
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_WINDFORCE					217
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_AMFLEAFIRE									218
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_AMFLEAFIRE					219
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_AMFLEAFIRE					220
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_HOLYPRESSURE								221
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_HOLYPRESSURE					222
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_HOLYPRESSURE				223
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_APSOLUTMAGICBARRIER							224
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_APSOLUTMAGICBARRIER			225
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_APSOLUTMAGICBARRIER			226
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_SPELLBINDING								227
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_SPELLBINDING					228
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_SPELLBINDING				229
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_DOMENATION									230
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_MIRACULOUSHEART								231
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_ARCANUMSKIN									232
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_FRENZY										233
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_CALMDOWN									234
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_CALMDOWN						235
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_CALMDOWN					236
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_SLEEP										237
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_SLEEP							238
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_SLEEP						239
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_CONFUSE										240
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_CONFUSE						241
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_CONFUSE						242
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_SOULBLASTER									243
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_SOULBLASTER					244
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_SOULBLASTER					245
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_MINDCONTROL									246
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_MINDCONTROL					247
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_MINDCONTROL					248
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_RAGE										249
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_RAGE							250
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_RAGE						251
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_REDELEMENTAL								252
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_REDELEMENTAL					253
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_REDELEMENTAL				254
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_BLUEELEMENTAL								255
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_BLUEELEMENTAL					256
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_BLUEELEMENTAL				257
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_GREENELEMENTAL								258
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_GREENELEMENTAL				259
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_GREENELEMENTAL				260
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_SPEEDOVER									261
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_DODGE										262
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_POISONING									263
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_POISONING						264
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_POISONING					265
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_CHAINATTACKER								266
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_CHAINATTACKER					267
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_CHAINATTACKER				268
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_BLASTQUAKE									269
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_BLASTQUAKE					270
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_BLASTQUAKE					271
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_IRONSKIN									272
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_IRONSKIN						273
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_IRONSKIN					274
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_HIDING										275
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_HIDING						276
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_HIDING						277
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_SPELLTRAP									278
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_SPELLTRAP						279
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_SPELLTRAP					280
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_PRESSURE									281
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_PRESSURE						282
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_PRESSURE					283
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_LIFEEXPLOSION								284
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_LIFEEXPLOSION					285
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_LIFEEXPLOSION				286
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_SYLPHID										287
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_SYLPHID						288
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_SYLPHID						289
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_SHAUT										290
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_SHAUT							291
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_SHAUT						292
#define	ITEM_ATTR_SKILL_LEVEL_SKILL_BERSERK										293
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACK_SKILL_BERSERK						294
#define	ITEM_ATTR_SKILL_CASTING_WHEN_ATTACKED_SKILL_BERSERK						295
#define	ITEM_ATTR_SKILL_CASTING_WHEN_DEAD_SKILL_TREEAGION						296
#define	ITEM_ATTR_OPTION_INCREASE_WHEN_CHAR_BIRTHDAY							297
#define	ITEM_ATTR_OPTION_INCREASE_WHEN_MASTER_BIRTHDAY							298
#define	ITEM_ATTR_ONLY_EFFECT_XMAS												299
#define	ITEM_ATTR_ONLY_EFFECT_SPECIAL_DAY										300
#define	ITEM_ATTR_OPTION_INCREASE_WHEN_NIGHTTIME								301
#define	ITEM_ATTR_OPTION_INCREASE_WHEN_DAYTIME									302
#define	ITEM_ATTR_ONLY_EFFECT_WHEN_NIGHTTIME									303
#define	ITEM_ATTR_ONLY_EFFECT_WHEN_DAYTIME										304
#define	ITEM_ATTR_DESTROYED_WHEN_DUEDATE										305
	
#pragma pack(push, 1 )


//==================================================================================
//struct SET_ITEM_OPTION_1
//{
//	WORD	wKind;
//	WORD	wMin;
//	WORD	wMax;
//};

//struct SET_ITEM_OPTION_2
//{
//	WORD	wKind;
//	WORD	wValue;
//};


//typedef struct SET_ITEM_INFO
//{
//	WORD				wSetItemID;
//	char				szItemName_Kor[50];
//	char				szItemName_Eng[50];
//	BYTE				bFullSet;
//	WORD				pwSetID[7];
//	
//	SET_ITEM_OPTION_2	pBonusOption[5][2];		// 총 5개까지 각각 2개씩 붙을수 있다.
//	SET_ITEM_OPTION_2	pFullOption[8];			// Set를 보두 장착했을시에 얻을수 있는 풀옵 보너스.		
//} *LPSET_ITEM_INFO;

// Weapon
//typedef struct BASEITEM_WEAPON
//{	
////#ifdef INNER_TEST_SERVER_CONNECT
//	BYTE				bItemGrade;
////#endif
//	BYTE				bClassGrade;
//	WORD				wSetID;
//	BYTE				byDropBox;
//	BYTE				bRaceID;
//	BYTE				bClassID;
//	
//	WORD				wWeight;
//	
//	WORD				wMin_Damage;
//	WORD                wMax_Damage;
////#ifdef INNER_TEST_SERVER_CONNECT
//	WORD				wMana_Damage;
////#endif
//	WORD				wMin_Str;
//	WORD				wMin_Dex;
//	WORD				wMin_Level;
//	WORD				wMin_Slot;
//	WORD				wMax_Slot;
//	WORD				wBase_Speed;
//	WORD				wMax_Speed;
//	WORD				wRange;
//	
//	// SetItem을 위한 정보.
//	SET_ITEM_OPTION_1	pBaseSetItemOption[6];
//	SET_ITEM_OPTION_2	pPartOption[4];
//	// 아이템 생성시 필요한 재료의 퀄리티및 개수	
//	WORD				wM_QTT;
//	WORD				wG_QTT;
//	WORD				wW_QTT;
//	WORD				wL_QTT;
//	BYTE				bMaterial;
//	
//	DWORD				dwItemPrice;
//	DWORD				dwSellPrice;
//	
//	BYTE				bIVIconCount;
//	BYTE				bIVIndex;	
//	BYTE				bModCount;
//	
//} *LPBASEITEM_WEAPON;

//Armor..
//typedef struct BASEITEM_ARMOR
//{
////#ifdef INNER_TEST_SERVER_CONNECT
//	BYTE				bItemGrade;
////#endif
//	BYTE				bClassGrade;
//	WORD				wSetID;
//	BYTE				byDropBox;
//	BYTE				bRaceID;
//	BYTE				bClassID;
//	
//	WORD				wWeight;
//	
//	WORD				wMin_Damage;
//	WORD                wMax_Damage;
//	WORD				wMin_Str;
//	WORD				wMin_Level;
//	WORD				wMin_Slot;
//	WORD				wMax_Slot;
//	
//	// SetItem을 위한 정보.
//	SET_ITEM_OPTION_1	pBaseSetItemOption[6];
//	SET_ITEM_OPTION_2	pPartOption[4];
//	
//	WORD				wM_QTT;
//	WORD				wG_QTT;
//	WORD				wW_QTT;
//	WORD				wL_QTT;
//	BYTE				bMaterial;
//	
//	DWORD				dwItemPrice;
//	DWORD				dwSellPrice;
//	
//	BYTE				bIVIconCount;
//	BYTE				bIVIndex;	
//	BYTE				bModCount;
//	
//} *LPBASEITEM_ARMOR;

// Special
//typedef struct BASEITEM_SPECIAL
//{
//	BYTE				bClassGrade;	
//	BYTE				bWeight;	
//
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.	
//	BYTE				bModCount;				// Mod 카운드.
//	BYTE				bQuantityCnt;			// 한 슬롯에 갯수.
//
//} *LPBASEITEM_SPECIAL;
#define MAX_BASEITEM_CONSUMABLE_ITEMATTR	5
//struct BASEITEM_CONSUMABLE_ITEMATTR
//{
//	WORD				wAttr_Code;
//	WORD				wAttr_Value;
//};
//typedef struct	BASEITEM_CONSUMABLE
//{
//	BASEITEM_CONSUMABLE_ITEMATTR	sItem_Attr[MAX_BASEITEM_CONSUMABLE_ITEMATTR];
//	WORD				wLast_Time;
//	WORD				wMin_Lev;
//	WORD				wMax_Lev;
//
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;	
//
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.
//	BYTE				bModCount;				// Mod 카운드.
//	BYTE				bQuantityCnt;			// 한 슬롯에 갯수.
//
//} *LPBASEITEM_CONSUMABLE;

//Supplies
//typedef struct BASEITEM_SUPPLIES
//{
//	BYTE				bType;					// 어떤 용도의 아이템이냐.
//	WORD				wMin;					// 최소 최대 수치 퍼센트이다.
//	WORD				wMax;					
//	BYTE				bWeight;	
//	
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//		
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.	
//	BYTE				bModCount;				// Mod 갯수.
//	BYTE				bQuantityCnt;			/// 한 슬롯에 갯수.
//	
//} *LPBASEITEM_SUPPLIES;

//typedef struct BASEITEM_ZODIAC
//{
//	BYTE				bGrade;
//	WORD				wMin_Lev;
//	WORD				wDifficulty;
//	DWORD				dwRarity;
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.	
//	BYTE				bModCount;				// Mod 갯수.
//	BYTE				bQuantityCnt;			// 한 슬롯에 갯수.
//	
//} *LPBASEITEM_ZODIAC;

//typedef struct BASEITEM_RIDE
//{
//	BYTE				bGrade;
//	BYTE				bWeight;
//	
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.
//	BYTE				bModCount;				// Mod 카운드.
//	
//} *LPBASEITEM_RIDE;

// Guardian
//typedef struct BASEITEM_GUARDIAN
//{
//	BYTE				bClassGrade;	
//	WORD				wWeight;
//	
//	BYTE				bType;					// 초란 : 1
//	WORD				wCompleteTime;			// 분으로 계산됨.
//	DWORD				dwDueDayTime;			// 분으로 계산됨. 만기일
//	WORD				wCreatureID;			// 부활했을때 변경될 아이디
//	WORD				wDieItemID;				// 죽엇을때 변경될 아이디
//	WORD				wBrokenItemID;			// 깨졋을때 변경될 아이디
//	WORD				wBaseGuardianID;
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;	
//
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.	
//	BYTE				bModCount;				// Mod 카운드.
//
//} *LPBASEITEM_GUARDIAN;
//typedef struct BASEITEM_MAGICARRAY
//{
//	BYTE				bSubID;					// 사용중이라고 기억시킬 배열 인덱스
//	BYTE				bGrade;
//	BYTE				bWeight;
//	BYTE				bClassId;
//	BYTE				bRaceId;
//
//	WORD				wMinLev;
//	WORD				wMinRank;
//	WORD				wMagicId1;
//	WORD				wPoint1;
//	DWORD				dwDur1;
//	WORD				wMagicId2;
//	WORD				wPoint2;
//	DWORD				dwDur2;
//	WORD				wMagicId3;
//	WORD				wPoint3;
//	DWORD				dwDur3;	
//	
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//		
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.	
//	BYTE				bModCount;				// Mod 카운드.
//	BYTE				bQuantityCnt;			// 한 슬롯에 갯수.
//	
//} *LPBASEITEM_MAGICARRAY;

//typedef struct BASEITEM_MATERIALS
//{
//	BYTE				bGrade;
//	BYTE				byOrder;
//	WORD				wPower;
//	WORD				wBias;
//	BYTE				bSmin;
//	BYTE				bSmax;
//	BYTE				bWeight;
//	BYTE				bLevADD;
//	BYTE				bDurability;
//	BYTE				bDifficulty;
//	
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//	
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.	
//	BYTE				bModCount;				// Mod 카운드.
//	BYTE				bQuantityCnt;			// 한 슬롯에 갯수.
//	
//} *LPBASEITEM_MATERIALS;

//typedef struct BASEITEM_MIX_UPGRADE
//{
//	BYTE				bGrade;
//	WORD				wWeight;
//	WORD				wMin_Lev;
//	WORD				wDurability;
//	WORD				wDifficulty;
//	
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.	
//	BYTE				bModCount;				// Mod 카운드.
//	BYTE				bQuantityCnt;			// 한 슬롯에 갯수.
//	
//} *LPBASEITEM_MIX_UPGRADE;
//struct VALUE_BASEITEM_MAGICFIELD_ARRAY
//{
//	WORD	wID;
//	BYTE	bWho;
//	short	nValue;
//	BYTE	bFormula;
//};
//typedef struct BASEITEM_MAGICFIELD_ARRAY
//{
//	WORD				wMin_Lv;
//	VALUE_BASEITEM_MAGICFIELD_ARRAY sValue[5];
//
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.
//	BYTE				bModCount;				// Mod 카운드.
//	BYTE				bQuantityCnt;			// 한 슬롯에 갯수.
//
//} *LPBASEITEM_MAGICFIELD_ARRAY;


//typedef struct BASEITEM_UPGRADE
//{
//	WORD				wWeight;
//	
//	WORD				wProbability_minus;
//	
//	WORD				wR1_plus;
//	WORD				wR2_plus;
//	
//	WORD				wW_Grade;
//	
//	WORD				wFormula[18];
//	
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//	
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.
//	BYTE				bModCount;				// Mod 카운드.
//	BYTE				bQuantityCnt;			// 한 슬롯에 갯수.
//} *LPBASEITEM_UPGRADE;


//typedef struct BASEITEM_LIQUID 
//{
//	WORD				wWeight;
//	
//	BYTE				bclassfication;
//	
//	WORD				w1_slot;
//	WORD				wfluent_a;
//	
//	WORD				w2_slot;
//	WORD				wfluent_b;
//	
//	WORD				wfluent_c;
//	
//	WORD				wW_Grade;
//	
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//	
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.
//	BYTE				bModCount;				// Mod 카운드.
//	BYTE				bQuantityCnt;			// 한 슬롯에 갯수.
//} *LPBASEITEM_LIQUID;


//typedef struct BASEITEM_EDITION 
//{
//	WORD				wWeight;
//	
//	WORD				wOption;
//	WORD				wProbability_plus;
//	
//	WORD				wLiquid_a_min;
//	WORD				wLiquid_a_max;
//	
//	WORD				wLiquid_b_min;
//	WORD				wLiquid_b_max;
//	
//	WORD				wFormula[18];
//	
//	DWORD				dwPrice;
//	DWORD				dwSellPrice;
//	
//	BYTE				bIVIconCount;			// 동시에 나올수 있는 Icon 갯수.
//	BYTE				bIVIndex;				// 한손인지 두손인지.
//	BYTE				bModCount;				// Mod 카운드.
//	BYTE				bQuantityCnt;			// 한 슬롯에 갯수.	
//} *LPBASEITEM_EDITION;


//class CBaseItem
//{
//public:
//	WORD				wID;
//
//	char				szItemName_Kor[50];		// Item Name
//	char				szItemName_Eng[35];
//
//	union
//	{
//		BASEITEM_WEAPON				BaseItem_Weapon;
//		BASEITEM_ARMOR				BaseItem_Armor;
//		BASEITEM_SUPPLIES			BaseItem_Supplies;		
//		BASEITEM_RIDE				BaseItem_Ride;
//		BASEITEM_SPECIAL			BaseItem_Special;		
//		BASEITEM_ZODIAC				BaseItem_Zodiac;
//		BASEITEM_GUARDIAN			BaseITem_Guardian;
//		BASEITEM_MAGICARRAY			BaseItem_MagicArray;
//		BASEITEM_MATERIALS			BaseItem_Materials;
//		BASEITEM_MIX_UPGRADE		BaseItem_Mix_Upgrade;
//		BASEITEM_MAGICFIELD_ARRAY	BaseItem_MagicField_Array;
//		BASEITEM_CONSUMABLE			BaseItem_Consumable;
//		BASEITEM_UPGRADE			BaseItem_UpGreade;
//		BASEITEM_LIQUID				BaseItem_LiQuid;
//		BASEITEM_EDITION			BaseItem_Edition;
//	};
//
//	BYTE GetClass()
//	{
//		int nValue = wID/ITEM_DISTRIBUTE;
//
//		if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)
//			return BaseItem_Weapon.bClassID;
//		else if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
//			return BaseItem_Armor.bClassID;
//		else if(nValue==ITEM_RIDE_INDEX)
//			return 0;
//	}
//
//	DWORD GetSellPrice()
//	{
//		int nValue = wID/ITEM_DISTRIBUTE;
//
//		if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)
//			return BaseItem_Weapon.dwSellPrice;
//		else if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
//			return BaseItem_Armor.dwSellPrice;
//		else if(nValue==ITEM_GUARDIAN_INDEX)
//			return BaseITem_Guardian.dwSellPrice;
//		else if(nValue==ITEM_ZODIAC_INDEX)
//			return BaseItem_Zodiac.dwSellPrice;
//		else if(nValue==ITEM_RIDE_INDEX)
//			return BaseItem_Ride.dwSellPrice;
//		else if(nValue>=ITEM_SPECIALST_INDEX && nValue<=ITEM_SPECIALEND_INDEX)
//			return BaseItem_Special.dwSellPrice;
//		else if(nValue==ITEM_MAGICARRAY_INDEX)
//			return BaseItem_MagicArray.dwSellPrice;
//		else if(nValue>=ITEM_MATERIALST_INDEX && nValue<=ITEM_MATERIALEND_INDEX)
//			return BaseItem_Materials.dwSellPrice;
//		else if(nValue>=ITEM_MIXUPGRADEST_INDEX && nValue<=ITEM_MIXUPGRADEEND_INDEX)
//			return BaseItem_Mix_Upgrade.dwSellPrice;
//		else if(nValue==ITEM_MAGICFIELDARRAY_INDEX)
//			return BaseItem_MagicField_Array.dwSellPrice;
//		else if(nValue==ITEM_SUPPLIES_INDEX)
//			return BaseItem_Supplies.dwSellPrice;
//		else if(nValue>=ITEM_CONSUMABLEST_INDEX && nValue<=ITEM_CONSUMABLEEND_INDEX)
//			return BaseItem_Consumable.dwSellPrice;	
//		else if(nValue==ITEM_UPGRADE_INDEX)
//			return BaseItem_UpGreade.dwSellPrice;
//		else if(nValue==ITEM_LIQUID_INDEX)
//			return BaseItem_LiQuid.dwSellPrice;
//		else if(nValue==ITEM_EDITION_INDEX)
//			return BaseItem_Edition.dwSellPrice;
//	}
//
//
//	DWORD GetPrice()
//	{
//		int nValue = wID/ITEM_DISTRIBUTE;
//		
//		if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)
//			return BaseItem_Weapon.dwItemPrice;
//		else if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
//			return BaseItem_Armor.dwItemPrice;
//		else if(nValue==ITEM_GUARDIAN_INDEX)
//			return BaseITem_Guardian.dwPrice;
//		else if(nValue==ITEM_ZODIAC_INDEX)
//			return BaseItem_Zodiac.dwPrice;
//		else if(nValue==ITEM_RIDE_INDEX)
//			return BaseItem_Ride.dwPrice;
//		else if(nValue>=ITEM_SPECIALST_INDEX && nValue<=ITEM_SPECIALEND_INDEX)
//			return BaseItem_Special.dwPrice;
//		else if(nValue==ITEM_MAGICARRAY_INDEX)
//			return BaseItem_MagicArray.dwPrice;
//		else if(nValue>=ITEM_MATERIALST_INDEX && nValue<=ITEM_MATERIALEND_INDEX)
//			return BaseItem_Materials.dwPrice;
//		else if(nValue>=ITEM_MIXUPGRADEST_INDEX && nValue<=ITEM_MIXUPGRADEEND_INDEX)
//			return BaseItem_Mix_Upgrade.dwPrice;
//		else if(nValue==ITEM_MAGICFIELDARRAY_INDEX)
//			return BaseItem_MagicField_Array.dwPrice;
//		else if(nValue==ITEM_SUPPLIES_INDEX)
//			return BaseItem_Supplies.dwPrice;
//		else if(nValue>=ITEM_CONSUMABLEST_INDEX && nValue<=ITEM_CONSUMABLEEND_INDEX)
//			return BaseItem_Consumable.dwPrice;
//		else if(nValue==ITEM_UPGRADE_INDEX)
//			return BaseItem_UpGreade.dwPrice;
//		else if(nValue==ITEM_LIQUID_INDEX)
//			return BaseItem_LiQuid.dwPrice;
//		else if(nValue==ITEM_EDITION_INDEX)
//			return BaseItem_Edition.dwPrice;
//	}
//	
//	BYTE GetMaxQuantity()
//	{	
//		int nValue = wID/ITEM_DISTRIBUTE;
//		
//		switch(nValue)
//		{								
//		case ITEM_SUPPLIES_INDEX:
//			return this->BaseItem_Supplies.bQuantityCnt;
//		case ITEM_ZODIAC_INDEX:
//			return this->BaseItem_Zodiac.bQuantityCnt;
//		case ITEM_MAGICARRAY_INDEX:
//			return this->BaseItem_MagicArray.bQuantityCnt;		
//		case ITEM_MAGICFIELDARRAY_INDEX:
//			return this->BaseItem_MagicField_Array.bQuantityCnt;
//		case ITEM_CONSUMABLEST_INDEX:
//		case ITEM_CONSUMABLEEND_INDEX:
//			return this->BaseItem_Consumable.bQuantityCnt;
//		case ITEM_UPGRADE_INDEX:
//			return this->BaseItem_UpGreade.bQuantityCnt;
//		case ITEM_LIQUID_INDEX:
//			return this->BaseItem_LiQuid.bQuantityCnt;
//		case ITEM_EDITION_INDEX:
//			return this->BaseItem_Edition.bQuantityCnt;
//
//		default:
//			{
//				if(nValue>=ITEM_SPECIALST_INDEX && nValue<=ITEM_SPECIALEND_INDEX)
//					return this->BaseItem_Special.bQuantityCnt;
//				else if(nValue>=ITEM_MATERIALST_INDEX && nValue<=ITEM_MATERIALEND_INDEX)
//					return this->BaseItem_Materials.bQuantityCnt;
//				else
//					return this->BaseItem_Mix_Upgrade.bQuantityCnt;
//			}			
//		}	
//	}
//
//	
//	BYTE	GetGrade();
//	
//	WORD	GetSetID()
//	{
//		int nValue = wID/ITEM_DISTRIBUTE;
//
//		if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)
//			return BaseItem_Weapon.wSetID;
//		else if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
//			return BaseItem_Armor.wSetID;
//
//		return 0;
//	}
//};
//===================================================================================

//===================================================================================
// Item and Item class...
//struct ITEM_SERIAL
//{
//	union
//	{
//		struct 	
//		{
//			BYTE	bServerSet;
//			BYTE	bServerSerialID;
//			WORD	wRestartCount;
//			DWORD	dwSerialNum;
//		};
//
//		__int64 i64Serial;
//	};
//};

// 아이템에 붙는 확장속성----------------
//struct ITEM_WEAPON
//{
//	BYTE	bClassGrade;
//	WORD	wMin_Damage;
//	WORD    wMax_Damage;
//	WORD	wMin_Str;
//	BYTE	bMin_Level;
//	BYTE	bItem_Material;
//
//	WORD	wMana_Damage;
//	WORD	wTemp;
//};

//struct ITEM_ARMOR
//{
//	BYTE	bClassGrade;
//	WORD	wDefense;
//	WORD	wMin_Str;
//	BYTE	bMin_Level;
//	BYTE	bItem_Material;
//};

//struct ITEM_ZODIAC
//{
//	BYTE	bQuantity;		// 수량.
//
//	BYTE	bType;
//
//	WORD	wMapId;
//	BYTE	bLayer;
//	
//	WORD	wPosX;	
//	WORD	wPosZ;
//};

//struct ITEM_SPECIAL
//{
//	BYTE	bQuantity;
//};
//
//struct ITEM_CONSUMABLE
//{
//	BYTE	bQuantity;
//	BYTE	bInvIndex;		// 사용했을때의 인벤위치 : 사용한 후에는 이동하지 못한다. base : 1부터
//	DWORD	dwEndDate;		// 아이템 끝나는 시간을 기록한다.
//};
//
//struct ITEM_SUPPLIES
//{
//	BYTE	bQuantity;		// 수량.
//};
//
//struct ITEM_MAGICARRAY
//{
//	BYTE	bQuantity;		// 수량.
//};
//
//
//struct ITEM_MONEY
//{
//	DWORD	dwMoney;		// 얼마?
//};

//struct ITEM_RIDEOBJECT
//{
//	float	fWorldPos_x;		//월드맵에서의 현제 위치해있는 좌표 
//	float	fWorldPos_z;
//};
//
//struct ITEM_MATERIALS
//{
//	BYTE	bQuantity;
//};
//
//struct ITEM_MIX_UPGRADE
//{
//	BYTE	bQuantity;
//};
//
//struct ITEM_GUARDIAN
//{
//	BYTE	bCreate;			// 가디언이 디비에 생성됐는가. 0이라면 아직 알인 상태인것이다.
//	WORD	wID;				// 디비에 생성된 가디언의 아이디.
//	DWORD	dwEndLife;
//	DWORD	dwCompleteTime;
//};
//struct ITEM_MAGICFIELD_ARRAY
//{
//	BYTE	bQuantity;
//};
//struct ITEM_UPGREADE
//{
//	BYTE	bQuantity;
//};
//struct ITEM_LIQUID
//{
//	BYTE	bQuantity;
//};
//struct ITEM_EDITION 
//{
//	BYTE	bQuantity;
//};
////---------------------------------------
//
//struct ITEM_SLOT
//{
//	BYTE	uUsedSlot	:4;
//	BYTE	uMaxSlot	:4;
//};
//
//// 아이템에 붙는 추가속성이다.
//struct ITEM_ATTR
//{
//	WORD	uCode	:6;			// 속성 코드
//	WORD	uValue	:10;		// 증가치.
//};


//class CItem
//{
//public:
//
//	WORD			m_wItemID;
//	ITEM_SERIAL		m_Serial;
//
//	union						// 기본적으로 아이템에서 바뀔수 있는 속성의 구조체
//	{
//		ITEM_WEAPON				m_ItemWeapon;
//		ITEM_ARMOR				m_Item_Armor;
//		ITEM_SUPPLIES			m_Item_Supplies;
//		ITEM_MONEY				m_Item_Money;		
//		ITEM_RIDEOBJECT			m_Item_RideObject;		
//		ITEM_SPECIAL			m_Item_Special;
//		ITEM_ZODIAC				m_Item_Zodiac;
//		ITEM_GUARDIAN			m_Item_Guardian;
//		ITEM_MAGICARRAY			m_Item_MagicArray;
//		ITEM_MATERIALS			m_Item_Materials;
//		ITEM_MIX_UPGRADE		m_Item_Mix_Upgrade;
//		ITEM_MAGICFIELD_ARRAY	m_Item_MagicField_Array;
//		ITEM_CONSUMABLE			m_ItemConsumable;
//		ITEM_UPGREADE			m_Item_UpGreade;
//		ITEM_LIQUID				m_Item_LiQuid;
//		ITEM_EDITION			m_Item_Edition;
//	};
//	
//	// 무게는 안쓰니까 빼버린다.
////	BYTE						m_bWeight;	// 무게.
//	ITEM_SLOT					m_bSlot;	// 사용슬롯과 최대슬롯.
//
//	union	// 아이템에 붙는 추가 속성.
//	{
//		ITEM_ATTR		wAttr[MAX_ITEM_ATTR];						//<-- 16Byte
//	};
//		
//	void			SetSerialNum(ITEM_SERIAL serial)	{	m_Serial = serial;	}
//	WORD			GetID()								{	return m_wItemID;	}	
//	ITEM_SERIAL*	GetSerial()							{	return &m_Serial;	}
//	
//	BYTE GetQuantity()
//	{	
//		switch(this->m_wItemID/ITEM_DISTRIBUTE)
//		{								
//		case ITEM_SUPPLIES_INDEX:
//			return this->m_Item_Supplies.bQuantity;
//		case ITEM_ZODIAC_INDEX:
//			return this->m_Item_Zodiac.bQuantity;
//		case ITEM_MAGICARRAY_INDEX:
//			return this->m_Item_MagicArray.bQuantity;
//		case ITEM_MAGICFIELDARRAY_INDEX:
//			return this->m_Item_MagicField_Array.bQuantity;
//		case ITEM_CONSUMABLEST_INDEX:
//		case ITEM_CONSUMABLEEND_INDEX:
//			return this->m_ItemConsumable.bQuantity;
//		case ITEM_UPGRADE_INDEX:
//			return this->m_Item_UpGreade.bQuantity;
//		case ITEM_LIQUID_INDEX:
//			return this->m_Item_LiQuid.bQuantity;
//		case ITEM_EDITION_INDEX:
//			return this->m_Item_Edition.bQuantity;	
//		default:						
//			{
//				if((this->m_wItemID/ITEM_DISTRIBUTE)>=ITEM_SPECIALST_INDEX && (this->m_wItemID/ITEM_DISTRIBUTE)<=ITEM_SPECIALEND_INDEX)
//					return this->m_Item_Special.bQuantity;
//				else if((this->m_wItemID/ITEM_DISTRIBUTE)>=ITEM_MATERIALST_INDEX && (this->m_wItemID/ITEM_DISTRIBUTE)<=ITEM_MATERIALEND_INDEX)
//					return this->m_Item_Materials.bQuantity;
//				else if((this->m_wItemID/ITEM_DISTRIBUTE)>=ITEM_MIXUPGRADEST_INDEX && (this->m_wItemID/ITEM_DISTRIBUTE)<=ITEM_MIXUPGRADEEND_INDEX)
//					return this->m_Item_Mix_Upgrade.bQuantity;
//				else if( this->m_wItemID == 25000 )
//					return BYTE(this->m_Item_Money.dwMoney);
//				else
//					 return 1;
//
//			}			
//		}	
//	}
//
//
//	void SetQuantity(BYTE bQuantity)
//	{
//		switch(this->m_wItemID/ITEM_DISTRIBUTE)
//		{								
//		case ITEM_SUPPLIES_INDEX:
//			this->m_Item_Supplies.bQuantity = bQuantity;
//			break;
//		case ITEM_ZODIAC_INDEX:
//			this->m_Item_Zodiac.bQuantity = bQuantity;
//			break;
//		case ITEM_MAGICARRAY_INDEX:
//			this->m_Item_MagicArray.bQuantity = bQuantity;
//			break;
//		case ITEM_MAGICFIELDARRAY_INDEX:
//			this->m_Item_MagicField_Array.bQuantity = bQuantity;
//			break;
//		case ITEM_CONSUMABLEST_INDEX:
//		case ITEM_CONSUMABLEEND_INDEX:
//			this->m_ItemConsumable.bQuantity = bQuantity;
//			break;
//		case ITEM_UPGRADE_INDEX:
//			this->m_Item_UpGreade.bQuantity = bQuantity;
//			break;
//		case ITEM_LIQUID_INDEX:
//			this->m_Item_LiQuid.bQuantity = bQuantity;
//			break;
//		case ITEM_EDITION_INDEX:
//			this->m_Item_Edition.bQuantity = bQuantity;
//			break;
//		default:
//			{
//				if((this->m_wItemID/ITEM_DISTRIBUTE)>=ITEM_SPECIALST_INDEX && (this->m_wItemID/ITEM_DISTRIBUTE)<=ITEM_SPECIALEND_INDEX)
//					this->m_Item_Special.bQuantity = bQuantity;
//				else if((this->m_wItemID/ITEM_DISTRIBUTE)>=ITEM_MATERIALST_INDEX && (this->m_wItemID/ITEM_DISTRIBUTE)<=ITEM_MATERIALEND_INDEX)
//					this->m_Item_Materials.bQuantity = bQuantity;
//				else
//					this->m_Item_Mix_Upgrade.bQuantity = bQuantity;
//			}			
//			break;
//		}
//	}	
//
//	int GetType()
//	{
//		int nValue = this->m_wItemID/ITEM_DISTRIBUTE;
//
//		if((nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX) || (nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX) || nValue==ITEM_GUARDIAN_INDEX || nValue==ITEM_RIDE_INDEX)
//			return ITEM_LARGE;
//		else if((nValue>=ITEM_MATERIALST_INDEX && nValue<=ITEM_MATERIALEND_INDEX ) 
//			|| (nValue>=ITEM_SPECIALST_INDEX && nValue<=ITEM_SPECIALEND_INDEX) 
//			|| (nValue>=ITEM_MIXUPGRADEST_INDEX && nValue<=ITEM_MIXUPGRADEEND_INDEX) 
//			|| nValue==ITEM_SUPPLIES_INDEX || nValue==ITEM_ZODIAC_INDEX 
//			|| nValue==ITEM_MAGICARRAY_INDEX || nValue==ITEM_MAGICFIELDARRAY_INDEX 
//			|| (nValue>=ITEM_CONSUMABLEST_INDEX && nValue<=ITEM_CONSUMABLEEND_INDEX)
//			|| nValue==ITEM_UPGRADE_INDEX 
//			|| nValue==ITEM_LIQUID_INDEX || nValue==ITEM_EDITION_INDEX)
//			return ITEM_SMALL;
//		else if(nValue==ITEM_MONEY_INDEX)
//			return ITEM_MONEYS;
//		else
//			return ITEM_NONE;
//	}
//	
//	int	GetOptionCount()
//	{
//		int iOption = 0;
//		while(wAttr[iOption].uCode != 0 && iOption<MAX_ITEM_ATTR)
//		{
//			iOption++;
//		}
//		return iOption;
//	}
//};
//
//// 옵션 아이템을 위한 구조체.
//struct ITEM_OPTION
//{
//	DWORD	dwID;
//	char	szItemName[50];
//	BYTE	pbOption[ MAX_ITEM_KIND1_CODE ];
//};
struct ITEM_ATTR_DEFINE
{
	DWORD	dwID;
	WORD	wFormula;
	WORD	wSuccessFormula;
	BYTE	bSkillID;
	BYTE	bValueType;
};
struct ITEM_ATTR_VALUE_LIST
{
	DWORD	dwID;
	BYTE	bValueType;
	short	wMin;
	short	wMax;
	BYTE	bPBT;
};
#define	MAX_ATTR_DEFINE		500
#define	MAX_ATTR_VALUE_LIST	500
class	CItemAttrLayer
{
public:
	ITEM_ATTR_DEFINE	m_ItemAttrDefine[MAX_ATTR_DEFINE];
	ITEM_ATTR_VALUE_LIST	m_ItemAttrValueList[MAX_ATTR_VALUE_LIST];

	void	QueryItemAttr();
	DWORD	GetVirtualCode(WORD	wRealOptionCode, WORD wRealOptionValue);
	DWORD	GetVirtualValue(WORD wRealOptionValue);
	char*	GetAttrDescription(WORD	wRealOptionCode, WORD wRealOptionValue);
	DWORD	GetValueListID(WORD	wVirtualOptionCode, WORD wVirtualOptionValue);
	short	GetValue(WORD	wRealOptionCode, WORD	wRealOptionValue);
};
// 아이템 업그레이드시 필요한 테이블.
//struct ITEM_UPGRADE
//{
//	DWORD	dwID;
//	BYTE	bUpgradeLevel;
//	BYTE	bCode;
//	char	szUpgradeDesc[50];
//	DWORD	dwMin;
//	DWORD	dwMax;
//	WORD	wLevel;
//	WORD	wMinLevel;
//	WORD	wMinGrade;
//	WORD	wAddLevel;
//	WORD	pwItemMax[ ITEM_NUM_PER_OPTION ];
//};

// 아이템 로그를 위한 구조체.
//typedef struct ITEM_LOG
//{
//	ITEM_SERIAL			Serial;
//	BYTE				bLogCode;
//	BYTE				bServerSet;
//	DWORD				dwDungeonID;
//	CItem				Item;
//	char				szOwnerName[ MAX_CHARACTER_NAME_LENGTH ];		// 현재의 소유자.
//	char				szSellerName[ MAX_CHARACTER_NAME_LENGTH ];		// 거래라면 판사람.
//} *LPITEM_LOG;
//===================================================================================
// Item ===============================


#pragma pack(pop)
//struct ITEM_CREATE
//{
//	void*				DelPos;
//	VECTOR3				v3ItemPos;
//	DWORD				dwCreateTick;		// 아이템이 생성될 당시의 틱.
//	DWORD				dwSectionNum;
//	DWORD				dwOwnerIndex;
//	CItem				Item;
//	BYTE				bTemp[3];
//	BYTE				bCreateByType;		
////	DSTC_APPEAR_ITEM	AppearItem;
//};

struct ITEM
{
	VECTOR3				v3ItemPos;			// 아이템의 위치.
	DWORD				dwCurFramePerSec;	// 아이템의 사라지는 시간.
	DWORD				dwOwnerFramePerSec;	// 아이템을 생성시간 유저의 소유 시간.
	DWORD				dwOwnerIndex;		// 주인의 인덱스번호.
	DWORD				dwSectionNum;		// 현재 아이템이 있는 섹션.
	CItem				Item;				
};

// Func
/*int	GetItemType( WORD wBaseItemID );*/

// typedef...
typedef CAbyssHash<SET_ITEM_INFO> SET_ITEM_INFO_HASH, *LPSET_ITEM_INFO_HASH;
typedef CAbyssHash<BASEITEM_WEAPON> BASEITEM_WEAPON_HASH, * LPBASEITEM_WEAPON_HASH;
typedef CAbyssHash<BASEITEM_ARMOR>	BASEITEM_ARMOR_HASH, * LPBASEITEM_ARMOR_HASH;
typedef CAbyssHash<BASEITEM_SUPPLIES> BASEITEM_SUPPLIES_HASH, * LPBASEITEM_SUPPLIES_HASH;
typedef CAbyssHash<BASEITEM_ZODIAC> BASEITEM_ZODIAC_HASH, * LPBASEITEM_ZODIAC_HASH;
typedef CAbyssHash<BASEITEM_SPECIAL> BASEITEM_SPECIAL_HASH, * LPBASEITEM_SPECIAL_HASH;
typedef CAbyssHash<BASEITEM_GUARDIAN> BASEITEM_GUARDIAN_HASH, * LPBASEITEM_GUARDIAN_HASH;

typedef CAbyssHash<CBaseItem> BASEITEM_HASH, * LPBASEITEM_HASH;

typedef CAbyssHash64<ITEM>	ITEM_HASH, * LPITEM_HASH;


#endif // __ITEM_MANAGER_H__