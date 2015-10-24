#ifndef __DBG_H_
#define __DBG_H_


extern void DebugInit();
extern void WriteDebugInfo(const char* format, ...);
extern FILE* dbgfile;

#ifdef _DEBUG_INFO
#define DBG_INIT() DebugInit()
#define DBG_INFO(fmt, ...) fwprintf(dbgfile,TEXT(fmt) , __VA_ARGS__)
#else
#define DBG_INIT()
#define DBG_INFO(x, ...)
#endif

#endif