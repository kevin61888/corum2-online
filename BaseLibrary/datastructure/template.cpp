#include "../StdAfx.h"
//#include "TSTL.h"

/* Open drives timer cache */
// typedef TimerCache <const word*, od> dtc, *pdtc;

/* Lincages timer cache - Get Real Device Name by symbolic link */
// typedef TimerCache <const word*, word> ltc, *pltc;

/* Reverse lincages cache - Get Drive symbolic link by Real Device Name */
// typedef TimerCache <const word*, byte> rltc, *prltc;

/* Audit IN Path Storage */
// typedef PathStorage <3 * 4096, byte> apths, *papths;

/* File object elements storage */
// typedef ThreadSafeMap <PFILE_OBJECT, fhe> fom, *pfom;

/* File attributs elements storage */
// typedef ThreadSafeMap <const word*, pfae> fnm, *pfnm;

#include "SafeMap.hpp"