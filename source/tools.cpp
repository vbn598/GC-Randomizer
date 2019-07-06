#include "tools.h"
#include "defines.h"

#include <tp/d_com_inf_game.h>
#include <tp/d_save.h>

namespace mod::tools
{
	u64 randCustom = 1;

	bool indexOf(u8 array[], u32 size, u8 value)
	{
		for(u32 i = 0; i < size; i++)
		{
			if(array[i] == value)
			{
				return true;
			}
		}
		return false;
	}

	bool checkIfHaveItem(u8 item)
	{
		tp::d_com_inf_game::ItemWheel* ItemWheel = &tp::d_com_inf_game::dComIfG_gameInfo.itemWheel;
		u32 Size = sizeof(tp::d_com_inf_game::ItemWheel);

		for (u32 i = 0; i < Size; i++)
		{
			u8 itemInSlot = d_save::d_save_getItem(ItemWheel, static_cast<s32>(i), false);
			if (itemInSlot == item)
			{
				return true;
			}
		}
		return false;
	}

	void advanceRand()
	{
		randCustom += 0x9e3779b97f4a7c15;
	}

	u64 randGetNext()
	{
		u64 z = (randCustom += 0x9e3779b97f4a7c15);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
		return z ^ (z >> 31);
	}
}