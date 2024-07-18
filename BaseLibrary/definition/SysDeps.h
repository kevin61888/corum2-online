#ifndef __SYSDEPS_H__
#define __SYSDEPS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef _WIN32
  #define snprintf  _snprintf
  #define vsnprintf _vsnprintf
#endif

#endif /* __SYSDEPS_H__ */
