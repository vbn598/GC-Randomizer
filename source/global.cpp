#include "global.h"
#include "defines.h"

#include <gc/OSCache.h>
#include <tp/m_Do_controller_pad.h>
#include <tp/JFWSystem.h>

bool checkForButtonInput(u32 buttonCombo)
{
	return tp::m_Do_controller_pad::cpadInfo.wButtonInput & buttonCombo;
}

void setConsoleColor(u32 rgba)
{
	*reinterpret_cast<u32*>(tp::JFWSystem::systemConsole->ConsoleColor) = rgba;
}

void setConsole(bool activeFlag, u32 totalLines)
{
	tp::JFWSystem::SystemConsole* Console = tp::JFWSystem::systemConsole;
	Console->consoleEnabled = activeFlag;
	
	for (u32 i = 0; i < totalLines; i++)
	{
		Console->consoleLine[i].showLine = activeFlag;
	}
}

void clearConsole(u8 lines)
{
	for(u8 line = 0; line < lines; line++)
	{
		tp::JFWSystem::systemConsole->consoleLine[line].line[0] = '\0';
	}
}

void clear_DC_IC_Cache(void* ptr, u32 size)
{
	gc::OSCache::DCFlushRange(ptr, size);
	gc::OSCache::ICInvalidateRange(ptr, size);
}