#pragma once

#include "defines.h"

namespace mod::tools
{
	extern "C"
	{
		bool indexOf(u8 array[], u32 size, u8 value);
		bool checkIfHaveItem(u8 item);
		void advanceRand();
		u64 randGetNext();
	}
}