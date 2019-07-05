#include "tools.h"
#include "defines.h"

namespace mod::tools
{
	u64 randCustom = 1;

	bool indexOf(u8 array[], u32 size, u8 value)
	{
		bool b = false;
		for(u32 i = 0; i < size; i++)
		{
			if(array[i] == value)
			{
				b = true;
				break;
			}
		}
		return b;
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