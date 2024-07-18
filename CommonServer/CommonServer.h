#ifdef COMMONSERVER_EXPORTS
#	define DECLSPECIFIER __declspec(dllexport)//제네릭 한것으로 define명을 변경 하였습니다. DECLSPECIFIER --> DECLSPECIFIER : 050102 Hwoarang
#	define EXPIMP_TEMPLATE
#else
#	define DECLSPECIFIER __declspec(dllimport)
#	define EXPIMP_TEMPLATE extern
#endif

#include "CommonDungeon.h"