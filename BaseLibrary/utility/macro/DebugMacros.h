#ifndef _CUSTOM_DEBUG_MACROS_
#define _CUSTOM_DEBUG_MACROS_

#include <assert.h>


#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "
#define NOTE(x)  message(x)
#define FILE_LINE  message(__FILE__LINE__)
#define TODO(x)  message(__FILE__LINE__"\n"\
						" -------------------------------------------------\n"\
						"|  TODO :   " x "\n"\
						" -------------------------------------------------\n")
#define FIXME(x)  message(__FILE__LINE__"\n"\
						" -------------------------------------------------\n"\
						"|  FIXME :  " x "\n"\
						" -------------------------------------------------\n")
#define todo(x)  message(__FILE__LINE__" TODO : " x "\n")
#define fixme(x)  message(__FILE__LINE__" FIXME : " x "\n")
#define note(x)  message(__FILE__LINE__" NOTE : " x "\n")

#ifndef _verify
#ifdef _DEBUG
#define _verify(x)	(assert( x ))
#else
#define _verify(x)	(void( x ))
#endif
#endif

#endif