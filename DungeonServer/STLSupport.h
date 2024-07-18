#pragma once
#include <assert.h>
#include <cassert>

#pragma warning(disable:4786)
#include <list>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <bitset>
#include <fstream>
#include <iostream>
using namespace std;


#define SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);		(p)=NULL; } }
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p)=NULL; } }
#define SAFE_FCLOSE(p)			{ if(p) { ::fclose(p);		(p)=NULL; } }


//map ¿¤¸®¸àÆ® »èÁ¦ ÇÏ´Â Å¬·¡½º »ùÇÃ.
class DestroyElement
{
public:
	template<typename T1>
	void operator ()(T1 &t1)
	{
		SAFE_DELETE(t1.second);
	}
};

#define SRAND() \
		static bool bSrandFlag = true; \
		if(bSrandFlag)	\
		{	\
			::srand( (unsigned)time( NULL ) );	\
			bSrandFlag = false;	\
		}