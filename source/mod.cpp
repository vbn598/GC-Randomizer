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
#include <tp/c_math.h>

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

		resetConsoleAtSeconds = 15;

		itemsFound = 0;

		strcpy(tp::JFWSystem::systemConsole->consoleLine[0].line, "TP Randomizer 0.1b by AECX");
		strcpy(tp::JFWSystem::systemConsole->consoleLine[1].line, "! Reset your console to reset the randomization!");
		strcpy(tp::JFWSystem::systemConsole->consoleLine[2].line, "! Keep in mind this is a pre-release");

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

		clearConsole(20);
		setConsole(true, 20);

		// Get the console pointer
		tp::JFWSystem::SystemConsole* console = tp::JFWSystem::systemConsole;

		size_t lineLength = sizeof(tp::JFWSystem::ConsoleLine::line);

		strcpy(console->consoleLine[0].line, "TP Randomizer 0.1b by AECX");

		snprintf(console->consoleLine[13].line, lineLength, "Original item: %02d", item);

		if(item != 0x42 && item != 0x40 && item != 0xEE)
		{
			size_t numItems = sizeof(allItems);

			if(itemsFound < numItems)
			{
				u16 randomItemIndex = 0;

				u32 RNG1 = 1 + tp::c_math::rng1;
				u32 RNG2 = 1 + tp::c_math::rng2;
				u32 RNG3 = 1 + tp::c_math::rng3;

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

				while(randomItem == 0 )
				{
					// if the item is 0 that means it has been placed already
					randomItemIndex++;
					if(randomItemIndex >= numItems)
					{
						randomItemIndex = 0;
					}
					randomItem = unplacedItems[randomItemIndex];
				}

				// Set this to be found
				unplacedItems[randomItemIndex] = 0;

				itemsFound += 1;

				item = randomItem;

				snprintf(console->consoleLine[14].line, lineLength, "New      item: %02d", item);
			}
			else
			{
				strcpy(console->consoleLine[14].line, "All items that were to be randomized found");
			}
			snprintf(console->consoleLine[15].line, lineLength, "Items found: %02d/%02d", itemsFound, numItems);
		}
		else
		{
			strcpy(console->consoleLine[0].line, "Item rando skipped for reasons");
		}
		// Call original function
		createItemForTrBox_trampoline(pos, item, unk3, unk4, unk5, unk6);
	}
}