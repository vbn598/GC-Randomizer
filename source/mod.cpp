#include "mod.h"
#include "patch.h"
#include "defines.h"
#include "global.h"
#include "items.h"

#include <gc/buttons.h>

#include <tp/f_ap_game.h>
#include <tp/f_op_actor_mng.h>
#include <tp/d_a_alink.h>
#include <tp/JFWSystem.h>

#include <math.h>

#include <cstdio>
#include <string.h>

namespace mod
{
	Mod* gMod = nullptr;

	void main()
	{
		Mod* mod = new Mod();
		mod->init();
	}

	Mod::Mod()
	{
		gMod = this;
	}

	void Mod::init()
	{
		// Set the initial console color
		setConsoleColor(0xA000A050);
		setConsole(true, 20);

		resetConsoleAtSeconds = 10;

		itemsFound = 0;

		strcpy(tp::JFWSystem::systemConsole->consoleLine[0].line, "TP Randomizer 0.1b by AECX");
		strcpy(tp::JFWSystem::systemConsole->consoleLine[1].line, "gl hf taka lol");

		unplacedItems = allItems;
		// Set other values
		frameCount = 0;
		secondsSinceStart = 0;


		fapGm_Execute_trampoline = patch::hookFunction(tp::f_ap_game::fapGm_Execute,
			[]()
			{
				gMod->run();
			}
		);

		checkTreasureRupeeReturn_trampoline = patch::hookFunction(tp::d_a_alink::checkTreasureRupeeReturn,
			[](void* unused, s32 amount)
			{
				// Disable this function as this check happens before the replacement
				// Might render an item unobtainable if you're having rupees
				return false;
			}
		);

		createItemForTrBox_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForTrBoxDemo,
			[](const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
			{
				gMod->procCreateItemForTrBoxDemo(pos, item, unk3, unk4, unk5, unk6);
			}
		);
	}
	
	void Mod::run()
	{
		// Runs once each frame
		frameCount++;
		if(frameCount >= 30)
		{
			secondsSinceStart += 1;
			frameCount = 0;
		}

		if(secondsSinceStart == resetConsoleAtSeconds)
		{
			setConsole(false, 20);
		}

		// Call original function
		fapGm_Execute_trampoline();
	}

	void Mod::procCreateItemForTrBoxDemo(const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
	{
		// Runs once when Link picks up an item with text and is holding it towards the camera
		resetConsoleAtSeconds = secondsSinceStart + 10;

		setConsole(true, 20);

		// Get the console pointer
		tp::JFWSystem::SystemConsole* console = tp::JFWSystem::systemConsole;

		size_t lineLength = sizeof(tp::JFWSystem::ConsoleLine::line);

		strcpy(console->consoleLine[0].line, "TP Randomizer 0.1b by AECX");
		strcpy(console->consoleLine[1].line, "Player: TakaRikka");

		snprintf(console->consoleLine[13].line, lineLength, "Original item: %02d", item);

		size_t numItems = sizeof(allItems);
		if(itemsFound < numItems)
		{
			u16 randomItemIndex = 0;

			u32 RNG1 = 1 + *reinterpret_cast<u32*>(0x80451168);
			u32 RNG2 = 1 + *reinterpret_cast<u32*>(0x8045116C);
			u32 RNG3 = 1 + *reinterpret_cast<u32*>(0x80451170);

			u32 RNG = 1;

			if(RNG1 % 2)
			{
				RNG = RNG2;
			}
			else
			{
				RNG = RNG3;
			}

			// RandomItemIndex is > 0 and < numItems
			randomItemIndex = RNG % numItems;

			u8 randomItem = unplacedItems[randomItemIndex];

			while(randomItem == 0)
			{
				// if the item is 0 that means it has been placed already
				randomItemIndex++;
				if(randomItemIndex >= numItems)
				{
					randomItemIndex = 0;
				}
				randomItem = unplacedItems[randomItemIndex];
			}
			itemsFound += 1;

			item = randomItem;

			snprintf(console->consoleLine[14].line, lineLength, "New      item: %02d", item);
		}
		else
		{
			strcpy(console->consoleLine[14].line, "All items that were to be randomized found");
		}
		snprintf(console->consoleLine[15].line, lineLength, "Items found: %02d/%02d", itemsFound, numItems);
		// Call original function
		createItemForTrBox_trampoline(pos, item, unk3, unk4, unk5, unk6);
	}
}