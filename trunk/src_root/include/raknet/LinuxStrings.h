#ifndef _GCC_WIN_STRINGS
#define _GCC_WIN_STRINGS

#ifdef __GNUC__
int _stricmp(const char* s1, const char* s2);
int _strnicmp(const char* s1, const char* s2, size_t n);
//#ifndef __CYGWIN__
//int _unlink(const char* filename);
//#endif

#endif
#endif
