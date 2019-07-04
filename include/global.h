#pragma once

#include "defines.h"

extern "C"
{
	void clearConsole(u32 lines);
	bool indexOf(u8 array[], u32 size, u8 value);
	bool checkForButtonInput(u32 buttonCombo);
	void setConsoleColor(u32 rgba);
	void setConsole(bool activeFlag, u32 totalLines);
	u64 randGetNext();
	void clear_DC_IC_Cache(void* ptr, u32 size);
	
	extern u64 RandCustom;
}
