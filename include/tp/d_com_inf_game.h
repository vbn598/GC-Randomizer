#pragma once

#include "defines.h"

namespace tp::d_com_inf_game
{
	struct ItemWheel
	{
		u8 galeBoomerang;
		u8 lantern;
		u8 spinner;
		u8 ironBoots;
		u8 herosBow;
		u8 hawkEye;
		u8 ballAndChain;
		u8 unkSlot_7;
		u8 dominionRod;
		u8 clawShot;
		u8 doubleClawShot;
		u8 bottle1;
		u8 bottle2;
		u8 bottle3;
		u8 bottle4;
		u8 bombBag1;
		u8 bombBag2;
		u8 bombBag3;
		u8 ooccoo;
		u8 memoOrSketch;
		u8 fishingRod;
		u8 horseCall;
		u8 ancientSkyBook;
		u8 slingShot;
	} __attribute__((__packed__));

	// Should try to fill in the missing variables at some point
	// Need to also get the exact size of this struct
	struct LinkMapVars
	{
		u8 unk_0[0x4D0];
		float position[3];
	} __attribute__((__packed__));

	// Should try to fill in the missing variables at some point
	struct GameInfo
	{
		u8 unk_0[0x9C];
		ItemWheel itemWheel;
		u8 unk_b4[0x4D4C];
		char currentStage[8];
		u8 unk_4e04[0xFA4];
		LinkMapVars* linkMapPtr;
		u8 unk_5dac[0x18060];
	} __attribute__((__packed__));

	static_assert(sizeof(ItemWheel) == 0x18);
	static_assert(sizeof(GameInfo) == 0x1DE10);

	extern "C"
	{
		extern GameInfo dComIfG_gameInfo;
	}
}