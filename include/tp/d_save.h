#pragma once

#include "defines.h"

namespace mod::d_save
{
	extern "C"
	{
		u8 d_save_getItem(void* inventory, s32 item, bool unk3);
	}
}