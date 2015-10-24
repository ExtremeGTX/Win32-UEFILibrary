#include "stdafx.h"
#include "dbg.h"
#include <time.h>

#define DBG_FILE "C:\\UEFIdbg.log"

FILE* dbgfile;

void DebugInit()
{
	if (!dbgfile)
	{
		fopen_s(&dbgfile, DBG_FILE, "w+");
		if (dbgfile)
		{ 
			DBG_INFO("===logging started===\n");
		}
	}
}