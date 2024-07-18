// Info =======================================================
// Built by eflmake (ver:EF010) at 2004/07/16-01:29:25
//
// Contained ERL files --------------------
// DefResource.erl
// ----------------------------------------
//
// ============================================================

// Types ======================================================
#define	RESTYPE_HEAD_MALE               	1           
#define	RESTYPE_HEAD_FEMALE             	2           
#define	RESTYPE_BASE_BODY               	3           
#define	RESTYPE_MAP_WORLD               	8           
#define	RESTYPE_TTB_WORLD               	9           
#define	RESTYPE_MAP                     	10          
#define	RESTYPE_TTB                     	11          
#define	RESTYPE_NPC_CDB                 	12          
#define	RESTYPE_NPC                     	13          
#define	RESTYPE_SKILL_ICON1             	14          
#define	RESTYPE_SKILL_ICON2             	15          
#define	RESTYPE_SKILL_ICON3             	16          
#define	RESTYPE_ITEMICON                	17          
#define	RESTYPE_ITEMMOD                 	18          
#define	RESTYPE_IV1                     	19          
#define	RESTYPE_IV2                     	20          
#define	RESTYPE_IV3                     	21          
#define	RESTYPE_IV4                     	22          
#define	RESTYPE_IV5                     	23          
#define	RESTYPE_IV6                     	24          
#define	RESTYPE_IV7                     	25          
#define	RESTYPE_IV8                     	26          
#define	RESTYPE_IV9                     	27          
#define	RESTYPE_IV10                    	28          
#define	RESTYPE_IV11                    	29          
#define	RESTYPE_IV12                    	30          
#define	RESTYPE_IV13                    	31          
#define	RESTYPE_IV14                    	32          
#define	RESTYPE_IV15                    	33          
#define	RESTYPE_IV16                    	34          
#define	RESTYPE_IV17                    	35          
#define	RESTYPE_IV18                    	36          
#define	RESTYPE_IV19                    	37          
#define	RESTYPE_IV20                    	38          
#define	RESTYPE_IV21                    	39          
#define	RESTYPE_IV22                    	51          
#define	RESTYPE_IV23                    	52          
#define	RESTYPE_IV24                    	53          
#define	RESTYPE_IV25                    	54          
#define	RESTYPE_IV26                    	55          
#define	RESTYPE_IV27                    	56          
#define	RESTYPE_IV28                    	57          
#define	RESTYPE_IV29                    	58          
#define	RESTYPE_IV30                    	59          
#define	RESTYPE_IV31                    	60          
#define	RESTYPE_IV32                    	61          
#define	RESTYPE_IV33                    	62          
#define	RESTYPE_IV34                    	63          
#define	RESTYPE_IV35                    	64          
#define	RESTYPE_IV36                    	65          
#define	RESTYPE_IV37                    	66          
#define	RESTYPE_IV38                    	67          
#define	RESTYPE_IV39                    	68          
#define	RESTYPE_IV40                    	69          
#define	RESTYPE_IV41                    	70          
#define	RESTYPE_IV42                    	71          
#define	RESTYPE_IVBELT1                 	72          
#define	RESTYPE_IVBELT2                 	73          
#define	RESTYPE_IVBELT3                 	74          
#define	RESTYPE_IVBELT4                 	75          
#define	RESTYPE_IVBELT5                 	76          
#define	RESTYPE_IVBELT6                 	77          
#define	RESTYPE_IVBELT7                 	78          
#define	RESTYPE_IVBELT8                 	79          
#define	RESTYPE_SKILL_MAGIC             	100         
#define	RESTYPE_SKILL_AURA              	101         
#define	RESTYPE_SKILL_DIVINE            	102         
#define	RESTYPE_SUMMON                  	103         
#define	RESTYPE_SKILL_CHAKRA            	104         
#define	RESTYPE_SKILL_GENERATION        	105         
#define	RESTYPE_SKILLSTATUS_MAGIC       	110         
#define	RESTYPE_SKILLSTATUS_AURA        	111         
#define	RESTYPE_SKILLSTATUS_DIVINE      	112         
#define	RESTYPE_SKILLSTATUS_SUMMON      	113         
#define	RESTYPE_SKILLSTATUS_CHAKRA      	114         
#define	RESTYPE_SKILLSTATUS_GENERAL     	115         
#define	RESTYPE_BANK_ICON1              	116         
#define	RESTYPE_BANK_ICON2              	117         
#define	RESTYPE_BANK_ICON3              	118         
#define	RESTYPE_BANK_ICON4              	119         
#define	RESTYPE_BANK_ICON5              	120         
#define	RESTYPE_BANK_ICON6              	121         
#define	RESTYPE_BANK_ICON7              	122         
#define	RESTYPE_BANK_ICON8              	123         
#define	RESTYPE_BANK_ICON9              	124         
#define	RESTYPE_BANK_ICON10             	125         
#define	RESTYPE_BANK_ICON11             	126         
#define	RESTYPE_BANK_ICON12             	127         
#define	RESTYPE_BANK_ICON13             	128         
#define	RESTYPE_BANK_ICON14             	129         
#define	RESTYPE_BANK_ICON15             	130         
#define	RESTYPE_BANK_ICON16             	131         
#define	RESTYPE_BANK_ICON17             	132         
#define	RESTYPE_BANK_ICON18             	133         
#define	RESTYPE_BANK_ICON19             	134         
#define	RESTYPE_BANK_ICON20             	135         
#define	RESTYPE_BANK_ICON21             	136         
#define	RESTYPE_BANK_ICON22             	137         
#define	RESTYPE_BANK_ICON23             	138         
#define	RESTYPE_BANK_ICON24             	139         
#define	RESTYPE_BANK_ICON25             	140         
#define	RESTYPE_BANK_ICON26             	141         
#define	RESTYPE_BANK_ICON27             	142         
#define	RESTYPE_BANK_ICON28             	143         
#define	RESTYPE_BANK_ICON29             	144         
#define	RESTYPE_BANK_ICON30             	145         
#define	RESTYPE_BANK_ICON31             	146         
#define	RESTYPE_BANK_ICON32             	147         
#define	RESTYPE_BANK_ICON33             	148         
#define	RESTYPE_BANK_ICON34             	149         
#define	RESTYPE_BANK_ICON35             	150         
#define	RESTYPE_BANK_ICON36             	151         
#define	RESTYPE_BANK_ICON37             	152         
#define	RESTYPE_BANK_ICON38             	153         
#define	RESTYPE_BANK_ICON39             	154         
#define	RESTYPE_BANK_ICON40             	155         
#define	RESTYPE_BANK_ICON41             	156         
#define	RESTYPE_BANK_ICON42             	157         
#define	RESTYPE_BANK_ICON43             	158         
#define	RESTYPE_BANK_ICON44             	159         
#define	RESTYPE_BANK_ICON45             	160         
#define	RESTYPE_BANK_ICON46             	161         
#define	RESTYPE_BANK_ICON47             	162         
#define	RESTYPE_BANK_ICON48             	163         
#define	RESTYPE_BANK_ICON49             	164         
#define	RESTYPE_BANK_ICON50             	165         
#define	RESTYPE_BANK_ICON51             	166         
#define	RESTYPE_BANK_ICON52             	167         
#define	RESTYPE_BANK_ICON53             	168         
#define	RESTYPE_BANK_ICON54             	169         
#define	RESTYPE_BANK_ICON55             	170         
#define	RESTYPE_BANK_ICON56             	171         
#define	RESTYPE_BANK_ICON57             	172         
#define	RESTYPE_BANK_ICON58             	173         
#define	RESTYPE_BANK_ICON59             	174         
#define	RESTYPE_BANK_ICON60             	175         
#define	RESTYPE_BANK_ICON61             	176         
#define	RESTYPE_BANK_ICON62             	177         
#define	RESTYPE_BANK_ICON63             	178         
#define	RESTYPE_BANK_ICON64             	179         
#define	RESTYPE_BANK_ICON65             	180         
#define	RESTYPE_BANK_ICON66             	181         
#define	RESTYPE_BANK_ICON67             	182         
#define	RESTYPE_BANK_ICON68             	183         
#define	RESTYPE_BANK_ICON69             	184         
#define	RESTYPE_BANK_ICON70             	185         
#define	DUNGEON_MANAGEMENT              	186         
#define	RESTYPE_DUNGEON_MAGICARRAY      	187         
#define	RESTYPE_GUARDIAN                	188         
#define	RESTYPE_UIGUARDIAN_ITEM         	189         
#define	RESTYPE_ITEMTRADE1              	190         
#define	RESTYPE_ITEMTRADE2              	191         
#define	RESTYPE_ITEMTRADE3              	192         
#define	RESTYPE_ITEMTRADE4              	193         
#define	RESTYPE_ITEMTRADE5              	194         
#define	RESTYPE_ITEMTRADE6              	195         
#define	RESTYPE_ITEMTRADE7              	196         
#define	RESTYPE_ITEMTRADE8              	197         
#define	RESTYPE_ITEMTRADE9              	198         
#define	RESTYPE_ITEMTRADE10             	199         
#define	RESTYPE_ITEMTRADE11             	200         
#define	RESTYPE_ITEMTRADE12             	201         
#define	RESTYPE_ITEMTRADE13             	202         
#define	RESTYPE_ITEMTRADE14             	203         
#define	RESTYPE_ITEMTRADE15             	204         
#define	RESTYPE_ITEMTRADE16             	205         
#define	RESTYPE_ITEMTRADE17             	206         
#define	RESTYPE_ITEMTRADE18             	207         
#define	RESTYPE_ITEMTRADE19             	208         
#define	RESTYPE_ITEMTRADE20             	209         
#define	RESTYPE_ITEMTRADE21             	210         
#define	RESTYPE_ITEMTRADE22             	211         
#define	RESTYPE_ITEMTRADE23             	212         
#define	RESTYPE_ITEMTRADE24             	213         
#define	RESTYPE_ITEMTRADE25             	214         
#define	RESTYPE_ITEMTRADE26             	215         
#define	RESTYPE_ITEMTRADE27             	216         
#define	RESTYPE_ITEMTRADE28             	217         
#define	RESTYPE_PLAYERSHOP1             	218         
#define	RESTYPE_PLAYERSHOP2             	219         
#define	RESTYPE_PLAYERSHOP3             	220         
#define	RESTYPE_PLAYERSHOP4             	221         
#define	RESTYPE_PLAYERSHOP5             	222         
#define	RESTYPE_PLAYERSHOP6             	223         
#define	RESTYPE_PLAYERSHOP7             	224         
#define	RESTYPE_PLAYERSHOP8             	225         
#define	RESTYPE_PLAYERSHOP9             	226         
#define	RESTYPE_PLAYERSHOP10            	238         
#define	RESTYPE_PLAYERSHOP11            	239         
#define	RESTYPE_PLAYERSHOP12            	240         
#define	RESTYPE_PLAYERSHOP13            	241         
#define	RESTYPE_PLAYERSHOP14            	242         
#define	RESTYPE_PLAYERSHOP15            	243         
#define	RESTYPE_PLAYERSHOP16            	244         
#define	RESTYPE_PLAYERSHOP17            	245         
#define	RESTYPE_PLAYERSHOP18            	246         
#define	RESTYPE_PLAYERSHOP19            	247         
#define	RESTYPE_PLAYERSHOP20            	248         
// ============================================================

// ==== =================================== =============== ===============================
// IDX  Name                                UID             FileName
// ---- ----------------------------------- --------------- -------------------------------
// UnNamed Resource:	16778217    					// .\Data\Character\ph101001.MOD
// UnNamed Resource:	16778218    					// .\Data\Character\ph101002.MOD
// UnNamed Resource:	16778219    					// .\Data\Character\ph101003.MOD
// UnNamed Resource:	16778220    					// .\Data\Character\ph101004.MOD
// UnNamed Resource:	16778221    					// .\Data\Character\ph101005.MOD
// UnNamed Resource:	16778222    					// .\Data\Character\ph101006.MOD
// UnNamed Resource:	16778223    					// .\Data\Character\ph101007.MOD
// UnNamed Resource:	16778224    					// .\Data\Character\ph101008.MOD
// UnNamed Resource:	16778225    					// .\Data\Character\ph101009.MOD
// UnNamed Resource:	16778226    					// .\Data\Character\ph101010.MOD
// UnNamed Resource:	16778227    					// .\Data\Character\ph101011.MOD
// UnNamed Resource:	16778228    					// .\Data\Character\ph101012.MOD
// UnNamed Resource:	16778229    					// .\Data\Character\ph101013.MOD
// UnNamed Resource:	16778230    					// .\Data\Character\ph101014.MOD
// UnNamed Resource:	16778231    					// .\Data\Character\ph101015.MOD
// UnNamed Resource:	16778232    					// .\Data\Character\ph101016.MOD
// UnNamed Resource:	16778233    					// .\Data\Character\ph101017.MOD
// UnNamed Resource:	16778234    					// .\Data\Character\ph101018.MOD
// UnNamed Resource:	16778235    					// .\Data\Character\ph101019.MOD
// UnNamed Resource:	16778236    					// .\Data\Character\ph101020.MOD
// UnNamed Resource:	16778237    					// .\Data\Character\ph101021.MOD
// UnNamed Resource:	16778238    					// .\Data\Character\ph101022.MOD
// UnNamed Resource:	33555433    					// .\Data\Character\ph201001.MOD
// UnNamed Resource:	33555434    					// .\Data\Character\ph201002.MOD
// UnNamed Resource:	33555435    					// .\Data\Character\ph201003.MOD
// UnNamed Resource:	33555436    					// .\Data\Character\ph201004.MOD
// UnNamed Resource:	33555437    					// .\Data\Character\ph201005.MOD
// UnNamed Resource:	33555438    					// .\Data\Character\ph201006.MOD
// UnNamed Resource:	33555439    					// .\Data\Character\ph201007.MOD
// UnNamed Resource:	33555440    					// .\Data\Character\ph201008.MOD
// UnNamed Resource:	33555441    					// .\Data\Character\ph201009.MOD
// UnNamed Resource:	33555442    					// .\Data\Character\ph201010.MOD
// UnNamed Resource:	33555443    					// .\Data\Character\ph201011.MOD
// UnNamed Resource:	33555444    					// .\Data\Character\ph201012.MOD
// UnNamed Resource:	33555445    					// .\Data\Character\ph201013.MOD
// UnNamed Resource:	33555446    					// .\Data\Character\ph201014.MOD
// UnNamed Resource:	33555447    					// .\Data\Character\ph201015.MOD
// UnNamed Resource:	33555448    					// .\Data\Character\ph201016.MOD
// UnNamed Resource:	33555449    					// .\Data\Character\ph201017.MOD
// UnNamed Resource:	33555450    					// .\Data\Character\ph201018.MOD
// UnNamed Resource:	33555451    					// .\Data\Character\ph201019.MOD
// UnNamed Resource:	33555452    					// .\Data\Character\ph201020.MOD
// UnNamed Resource:	33555453    					// .\Data\Character\ph201021.MOD
// UnNamed Resource:	33555454    					// .\Data\Character\ph201022.MOD
// UnNamed Resource:	50331649    					// .\Data\Character\pm01000.chr
// UnNamed Resource:	50331650    					// .\Data\Character\pm02000.chr
// UnNamed Resource:	50331651    					// .\Data\Character\pm03000.chr
// UnNamed Resource:	50331652    					// .\Data\Character\pm04000.chr
// UnNamed Resource:	50331653    					// .\Data\Character\pm05000.chr
// UnNamed Resource:	134217729   					// .\Data\Map\worldmap.map
// UnNamed Resource:	150994945   					// .\Data\Map\WorldMap.ttb
// UnNamed Resource:	167772161   					// .\Data\Map\KS-Ma01.map
// UnNamed Resource:	167772162   					// .\Data\Map\KS-Ma01.map
// UnNamed Resource:	167772170   					// .\Data\Map\kaien_home.map
// UnNamed Resource:	167772260   					// .\Data\Map\JY_D1_000.map
// UnNamed Resource:	167772261   					// .\Data\Map\JY_D1_001.map
// UnNamed Resource:	167772262   					// .\Data\Map\JY_D1_002.map
// UnNamed Resource:	167772263   					// .\Data\Map\JY_D1_003.map
// UnNamed Resource:	167772264   					// .\Data\Map\JY_D1_004.map
// UnNamed Resource:	167772265   					// .\Data\Map\JY_D1_005.map
// UnNamed Resource:	167772266   					// .\Data\Map\JY_D1_006.map
// UnNamed Resource:	167772267   					// .\Data\Map\JY_D1_007.map
// UnNamed Resource:	167772268   					// .\Data\Map\JY_D1_008.map
// UnNamed Resource:	167772269   					// .\Data\Map\JY_D1_009.map
// UnNamed Resource:	167772270   					// .\Data\Map\JY_D1_010.map
// UnNamed Resource:	167772271   					// .\Data\Map\JY_D1_011.map
// UnNamed Resource:	167772272   					// .\Data\Map\JY_D1_012.map
// UnNamed Resource:	167772273   					// .\Data\Map\JY_D1_013.map
// UnNamed Resource:	167772274   					// .\Data\Map\JY_D1_014.map
// UnNamed Resource:	167772275   					// .\Data\Map\JY_D1_015.map
// UnNamed Resource:	167772276   					// .\Data\Map\JY_D1_016.map
// UnNamed Resource:	167772360   					// .\Data\Map\JY_D2_000.map
// UnNamed Resource:	167772361   					// .\Data\Map\JY_D2_001.map
// UnNamed Resource:	167772362   					// .\Data\Map\JY_D2_002.map
// UnNamed Resource:	167772363   					// .\Data\Map\JY_D2_003.map
// UnNamed Resource:	167772364   					// .\Data\Map\JY_D2_004.map
// UnNamed Resource:	167772365   					// .\Data\Map\JY_D2_005.map
// UnNamed Resource:	167772366   					// .\Data\Map\JY_D2_006.map
// UnNamed Resource:	167772367   					// .\Data\Map\JY_D2_007.map
// UnNamed Resource:	167772368   					// .\Data\Map\JY_D2_008.map
// UnNamed Resource:	167772369   					// .\Data\Map\JY_D2_009.map
// UnNamed Resource:	167772370   					// .\Data\Map\JY_D2_010.map
// UnNamed Resource:	167772371   					// .\Data\Map\JY_D2_011.map
// UnNamed Resource:	167772372   					// .\Data\Map\JY_D2_012.map
// UnNamed Resource:	167772373   					// .\Data\Map\JY_D2_013.map
// UnNamed Resource:	167772374   					// .\Data\Map\JY_D2_014.map
// UnNamed Resource:	167772375   					// .\Data\Map\JY_D2_015.map
// UnNamed Resource:	167772376   					// .\Data\Map\JY_D2_016.map
// UnNamed Resource:	167772460   					// .\Data\Map\KS-DUN03-00.map
// UnNamed Resource:	167772461   					// .\Data\Map\KS-DUN03-01.map
// UnNamed Resource:	167772462   					// .\Data\Map\KS-DUN03-02.map
// UnNamed Resource:	167772463   					// .\Data\Map\KS-DUN03-03.map
// UnNamed Resource:	167772464   					// .\Data\Map\KS-DUN03-04.map
// UnNamed Resource:	167772465   					// .\Data\Map\KS-DUN03-05.map
// UnNamed Resource:	167772466   					// .\Data\Map\KS-DUN03-06.map
// UnNamed Resource:	167772467   					// .\Data\Map\KS-DUN03-07.map
// UnNamed Resource:	167772468   					// .\Data\Map\KS-DUN03-08.map
// UnNamed Resource:	167772469   					// .\Data\Map\KS-DUN03-09.map
// UnNamed Resource:	167772470   					// .\Data\Map\KS-DUN03-10.map
// UnNamed Resource:	167772471   					// .\Data\Map\KS-DUN03-11.map
// UnNamed Resource:	167772472   					// .\Data\Map\KS-DUN03-12.map
// UnNamed Resource:	167772473   					// .\Data\Map\KS-DUN03-13.map
// UnNamed Resource:	167772474   					// .\Data\Map\KS-DUN03-14.map
// UnNamed Resource:	167772475   					// .\Data\Map\KS-DUN03-15.map
// UnNamed Resource:	167772561   					// .\Data\Map\NA_D4-01.map
// UnNamed Resource:	167772562   					// .\Data\Map\NA_D4-02.map
// UnNamed Resource:	167772563   					// .\Data\Map\NA_D4-03.map
// UnNamed Resource:	167772564   					// .\Data\Map\NA_D4-04.map
// UnNamed Resource:	167772660   					// .\Data\Map\ICE_D5-00.map
// UnNamed Resource:	167772661   					// .\Data\Map\ICE_D5-01.map
// UnNamed Resource:	167772662   					// .\Data\Map\ICE_D5-02.map
// UnNamed Resource:	167772663   					// .\Data\Map\ICE_D5-03.map
// UnNamed Resource:	167772664   					// .\Data\Map\ICE_D5-04.map
// UnNamed Resource:	167772665   					// .\Data\Map\ICE_D5-05.map
// UnNamed Resource:	167772666   					// .\Data\Map\ICE_D5-06.map
// UnNamed Resource:	167772667   					// .\Data\Map\ICE_D5-07.map
// UnNamed Resource:	167772669   					// .\Data\Map\ICE_D5-09.map
// UnNamed Resource:	167772670   					// .\Data\Map\ICE_D5-10.map
// UnNamed Resource:	167772671   					// .\Data\Map\ICE_D5-11.map
// UnNamed Resource:	167772672   					// .\Data\Map\ICE_D5-12.map
// UnNamed Resource:	167772674   					// .\Data\Map\ICE_D5-14.map
// UnNamed Resource:	167772675   					// .\Data\Map\ICE_D5-15.map
// UnNamed Resource:	167772676   					// .\Data\Map\ICE_D5-16.map
// UnNamed Resource:	167772677   					// .\Data\Map\ICE_D5-17.map
// UnNamed Resource:	167772678   					// .\Data\Map\ICE_D5-18.map
// UnNamed Resource:	167772679   					// .\Data\Map\ICE_D5-19.map
// UnNamed Resource:	167772680   					// .\Data\Map\ICE_D5-20.map
// UnNamed Resource:	167772681   					// .\Data\Map\ICE_D5-21.map
// UnNamed Resource:	167782161   					// .\Data\Map\worldmap1.cdb
// UnNamed Resource:	201326593   					// .\Data\Manager\NPC1.cdb
// UnNamed Resource:	201326595   					// .\Data\Manager\NPC3.cdb
// UnNamed Resource:	201326596   					// .\Data\Manager\NPC4.cdb
// UnNamed Resource:	201326597   					// .\Data\Manager\NPC5.cdb
// UnNamed Resource:	201326598   					// .\Data\Manager\NPC6.cdb
// UnNamed Resource:	201326602   					// .\Data\Manager\NPC10.cdb
// UnNamed Resource:	201329648   					// .\Data\Manager\NPC3056.cdb
// UnNamed Resource:	201329649   					// .\Data\Manager\NPC3057.cdb
// UnNamed Resource:	201331597   					// .\Data\Manager\NPC5005.cdb
// UnNamed Resource:	201331598   					// .\Data\Manager\NPC5006.cdb
// UnNamed Resource:	218103809   					// .\Data\Npc\npc003.chr
// UnNamed Resource:	218103810   					// .\Data\Npc\npc001.chr
// UnNamed Resource:	218103811   					// .\Data\Npc\npc012.chr
// UnNamed Resource:	218103812   					// .\Data\Npc\npc004.chr
// UnNamed Resource:	218103813   					// .\Data\Npc\npc005.chr
// UnNamed Resource:	218103815   					// .\Data\Npc\npc_kaien.chr
// UnNamed Resource:	218103816   					// .\Data\Npc\npc007.chr
// UnNamed Resource:	218103817   					// .\Data\Npc\npc008.chr
// UnNamed Resource:	218103818   					// .\Data\Npc\npc009.chr
// UnNamed Resource:	218103819   					// .\Data\Npc\npc010.chr
// UnNamed Resource:	218103820   					// .\Data\Npc\npc011.chr
// UnNamed Resource:	218103821   					// .\Data\Npc\npc008.chr
// UnNamed Resource:	218103822   					// .\Data\Npc\npc013.chr
// UnNamed Resource:	218103823   					// .\Data\Npc\npc014.chr
// UnNamed Resource:	218103824   					// .\Data\Npc\npc006.chr
// UnNamed Resource:	218103825   					// .\Data\Npc\npc007.chr
// UnNamed Resource:	218103826   					// .\Data\Npc\npc002.chr
// UnNamed Resource:	218103827   					// .\Data\Npc\npc009.chr
// UnNamed Resource:	218103828   					// .\Data\Npc\npc005.chr
// UnNamed Resource:	218103829   					// .\Data\Npc\npc006.chr
// UnNamed Resource:	218103830   					// .\Data\Npc\npc011.chr
// UnNamed Resource:	218103831   					// .\Data\Npc\npc012.chr
// UnNamed Resource:	218103832   					// .\Data\Npc\npc013.chr
// UnNamed Resource:	218103833   					// .\Data\Npc\npc014.chr
// UnNamed Resource:	218103834   					// .\Data\Npc\npc015.chr
// UnNamed Resource:	218103835   					// .\Data\Npc\npc016.chr
// UnNamed Resource:	218103836   					// .\Data\Npc\npc017.chr
// UnNamed Resource:	218103837   					// .\Data\Npc\npc018.chr
// UnNamed Resource:	218103838   					// .\Data\Npc\npc019.chr
// UnNamed Resource:	218103839   					// .\Data\Npc\npc020.chr
// UnNamed Resource:	218103840   					// .\Data\Npc\npc015.chr
// UnNamed Resource:	218103841   					// .\Data\Npc\npc006.chr
// UnNamed Resource:	218103842   					// .\Data\Npc\npc006.chr
// UnNamed Resource:	218103843   					// .\Data\Npc\npc006.chr
// UnNamed Resource:	218103844   					// .\Data\Npc\npc006.chr
// UnNamed Resource:	218103845   					// .\Data\Npc\npc006.chr
// UnNamed Resource:	218103846   					// .\Data\Npc\npc011.chr
// UnNamed Resource:	218103847   					// .\Data\Npc\npc015.chr
// UnNamed Resource:	218103848   					// .\Data\Npc\npc015.chr
// UnNamed Resource:	218103849   					// .\Data\Npc\npc015.chr
// UnNamed Resource:	218103850   					// .\Data\Npc\npc015.chr
// UnNamed Resource:	218103851   					// .\Data\Npc\npc015.chr
// ==== =================================== =============== ===============================
