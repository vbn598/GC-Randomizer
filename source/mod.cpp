#include "mod.h"
#include "defines.h"
#include "systemConsole.h"
#include "patch.h"
#include "items.h"
#include "controller.h"
#include "tools.h"

#include <tp/f_ap_game.h>
#include <tp/f_op_actor_mng.h>
#include <tp/d_a_alink.h>
#include <tp/JFWSystem.h>

#include <cstdio>
#include <cstring>

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
		// Perform any necessary assembly overwrites
		assemblyOverwrites();

		// Set the initial console color
		system_console::setBackgroundColor(0xA000A050);
		system_console::setState(true, 20);

		resetConsoleAtSeconds = 15;

		itemsFound = 0;

		// Get the console pointer
		tp::jfw_system::SystemConsole* console = sysConsolePtr;

		strcpy(console->consoleLine[0].line, "TP Randomizer 0.1b by AECX");
		strcpy(console->consoleLine[1].line, "! Reset your console to reset the randomization!");
		strcpy(console->consoleLine[2].line, "! Keep in mind this is a pre-release");
		strcpy(console->consoleLine[3].line, "! Hold [Z] to show the console");

		memcpy(items, allItems, sizeof(allItems));
		
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
			system_console::setState(false, 0);
		}

		if(controller::checkForButtonInput(controller::Button_Z))
		{
			system_console::setState(true, 0);
		}
		else if(secondsSinceStart > resetConsoleAtSeconds)
		{
			system_console::setState(false, 0);
		}

		tools::advanceRand();

		// Call original function
		fapGm_Execute_trampoline();
	}

	void Mod::procCreateItemForTrBoxDemo(const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
	{
		// Runs once when Link picks up an item with text and is holding it towards the camera
		resetConsoleAtSeconds = secondsSinceStart + 10;

		system_console::clearLines(20);
		system_console::setState(true, 20);

		// Get the console pointer
		tp::jfw_system::SystemConsole* console = sysConsolePtr;

		size_t maxLineLength = sizeof(tp::jfw_system::ConsoleLine::line);
		size_t numItems = sizeof(allItems);

		strcpy(console->consoleLine[0].line, "TP Randomizer 0.1b by AECX");

		snprintf(console->consoleLine[13].line, maxLineLength, "Original item: %02d", item);

		if(tools::indexOf(items, numItems, item))
		{
			if(itemsFound < numItems)
			{
				u16 randomItemIndex = 0;

				u64 RNG = tools::randGetNext();

				// RandomItemIndex is > 0 and < numItems
				randomItemIndex = RNG % numItems;

				u8 randomItem = items[randomItemIndex];

				while(randomItem == 0)
				{
					// if the item is 0 that means it has been placed already
					randomItemIndex++;
					if(randomItemIndex >= numItems)
					{
						randomItemIndex = 0;
					}
					randomItem = items[randomItemIndex];
				}

				// Set this to be found
				items[randomItemIndex] = 0;

				itemsFound += 1;

				item = randomItem;
				lastItem = item;
				snprintf(console->consoleLine[14].line, maxLineLength, "New      item: %02d", item);
			}
			else
			{
				strcpy(console->consoleLine[14].line, "All items that were to be randomized found");
			}
			snprintf(console->consoleLine[15].line, maxLineLength, "Items found: %02d/%02d", itemsFound, numItems);
		}
		else
		{
			strcpy(console->consoleLine[0].line, "Item skipped (not in pool)");
		}
		// Call original function
		createItemForTrBox_trampoline(pos, item, unk3, unk4, unk5, unk6);
	}
	
	void Mod::assemblyOverwrites()
	{
		// Get the addresses to overwrite
		#ifdef TP_US
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);
		#elif defined TP_EU
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B878);
		#elif defined TP_JP
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);
		#endif

		// Perform the overwrites

		/* If the address is loaded into the cache before the overwrite is made, 
		then the cache will need to be cleared after the overwrite */

		// Enable the crash screen
		*enableCrashScreen = 0x48000014; // b 0x14
	}
}