#include "defines.h"
#include "mod.h"
#include "global.h"
#include "systemConsole.h"
#include "patch.h"
#include "controller.h"
#include "tools.h"
#include "array.h"
#include "eventListener.h"
#include "stage.h"
#include "game_patches.h"
#include "chestRando.h"
#include "itemChecks.h"
#include "HUDConsole.h"

#include <tp/f_op_scene_req.h>
#include <tp/m_Do_controller_pad.h>
#include <tp/d_map_path_dmap.h>
#include <tp/evt_control.h>
#include <tp/f_op_scene_req.h>
#include <tp/d_com_inf_game.h>
#include <tp/f_ap_game.h>
#include <tp/f_op_actor_mng.h>
#include <tp/d_a_alink.h>
#include <tp/d_save.h>
#include <tp/JFWSystem.h>
#include <cstdio>
#include <cstring>

namespace mod
{
	Mod* global::modPtr = nullptr;
	ChestRandomizer* global::chestRandoPtr = nullptr;
	event::EventListener* global::eventListenerPtr = nullptr;
	mod::HUDConsole* global::hudConsolePtr = nullptr;

	void main()
	{
		Mod* mod = new Mod();
		mod->init();
	}

	Mod::Mod()
	{
		eventListener = new event::EventListener();
		chestRandomizer = new ChestRandomizer();
		hudConsole = new HUDConsole("Settings", 0x80);
		global::modPtr = this;
		global::chestRandoPtr = this->chestRandomizer;
		global::eventListenerPtr = this->eventListener;
		global::hudConsolePtr = this->hudConsole;
	}

	void Mod::init()
	{
		// Perform any necessary assembly overwrites
		game_patch::assemblyOverwrites();
		game_patch::increaseWalletSize();
		game_patch::increaseClimbSpeed();
		/*
		 * Causes issues right now (argarok cannot be beaten)
		 * game_patch::removeIBLimit();
		 */

		// Init rando
		tools::randomSeed = 0x9e3779b97f4a7c15;
		randoEnabled = 1;
		truePause = 1;
		inputBuffering = 0;

		// Print controls
		strcpy(sysConsolePtr->consoleLine[20].line, "D-Pad Nav: Up/Down  Left/Right");
		strcpy(sysConsolePtr->consoleLine[21].line, "+/- Value: A/B/X/Y  Console  :  R + Z");
		strcpy(sysConsolePtr->consoleLine[22].line, " Generate: R + Start (auto on new file)");
		strcpy(sysConsolePtr->consoleLine[23].line, "Bring up the console to use commands");
		strcpy(sysConsolePtr->consoleLine[24].line, "https://rando.tpspeed.run | Twitter: @theAECX");

		u8 page = 0;

		// General settings
		hudConsole->addOption(page, "Red:", &reinterpret_cast<u8*>(&sysConsolePtr->consoleColor)[0], 0xFF);
		hudConsole->addOption(page, "Green:", &reinterpret_cast<u8*>(&sysConsolePtr->consoleColor)[1], 0xFF);
		hudConsole->addOption(page, "Blue:", &reinterpret_cast<u8*>(&sysConsolePtr->consoleColor)[2], 0xFF);
		hudConsole->addOption(page, "Alpha:", &reinterpret_cast<u8*>(&sysConsolePtr->consoleColor)[3], 0xFF);
		hudConsole->addOption(page, "True Pause:", &truePause, 0x1);
		hudConsole->addOption(page, "Input Buffer:", &inputBuffering, 0x1);

		// Seed settings
		page = hudConsole->addPage("Seed");

		hudConsole->addOption(page, "Rando enabled?", &randoEnabled, 0x1);
		hudConsole->addOption(page, "Custom Seed?", &customSeed, 0x1);
		hudConsole->addOption(page, "Seed 1:", &reinterpret_cast<u8*>(&tools::randomSeed)[0], 0xFF);
		hudConsole->addOption(page, "Seed 2:", &reinterpret_cast<u8*>(&tools::randomSeed)[1], 0xFF);
		hudConsole->addOption(page, "Seed 3:", &reinterpret_cast<u8*>(&tools::randomSeed)[2], 0xFF);
		hudConsole->addOption(page, "Seed 4:", &reinterpret_cast<u8*>(&tools::randomSeed)[3], 0xFF);

		hudConsole->addOption(page, "Seed 5:", &reinterpret_cast<u8*>(&tools::randomSeed)[4], 0xFF);
		hudConsole->addOption(page, "Seed 6:", &reinterpret_cast<u8*>(&tools::randomSeed)[5], 0xFF);
		hudConsole->addOption(page, "Seed 7:", &reinterpret_cast<u8*>(&tools::randomSeed)[6], 0xFF);
		hudConsole->addOption(page, "Seed 8:", &reinterpret_cast<u8*>(&tools::randomSeed)[7], 0xFF);

		hudConsole->addWatch(page, "Resulting Seed", &tools::randomSeed, 'x', WatchInterpretation::_u64);

		// Debug
		page = hudConsole->addPage("Debug Info");
		
		hudConsole->addOption(page, "Progressive Items?", &chestRandomizer->isProgressiveEnabled, 0x1);	
		hudConsole->addOption(page, "Bugsanity?", &chestRandomizer->isBugsanityEnabled, 0x1);	
		hudConsole->addOption(page, "Poesanity?", &chestRandomizer->isPoesanityEnabled, 0x1);	
		
		hudConsole->addWatch(page, "Function:", &lastItemFunc, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "  Source:", &chestRandomizer->lastSourceInfo, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "    Dest:", &chestRandomizer->lastDestInfo, 's', WatchInterpretation::_str);

		hudConsole->addWatch(page, "Total Checks:", &chestRandomizer->totalChecks, 'u', WatchInterpretation::_u16);
		hudConsole->addWatch(page, "Layer Checks:", &chestRandomizer->layerCheckCount, 'u', WatchInterpretation::_u16);
		hudConsole->addWatch(page, "Empty:", &chestRandomizer->empty, 'u', WatchInterpretation::_u16);

		hudConsole->addWatch(page, "Active Seed:", &chestRandomizer->currentSeed, 'x', WatchInterpretation::_u64);
		hudConsole->addWatch(page, "   Checksum:", &chestRandomizer->checkSum, 'x', WatchInterpretation::_u16);


		// Item search
		page = hudConsole->addPage("Item Search");

		hudConsole->addOption(page, "Search ID:", &itemSearchID, 0xFF);
		hudConsole->addOption(page, "Reverse ID:", &itemReverseSearchID, 0xFF);

		hudConsole->addWatch(page, "1. Result:", &itemSearchResults, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "1. Reverse:", &itemReverseSearchResults, 's', WatchInterpretation::_str);

		hudConsole->addWatch(page, "MetadataID:", &lastItemDataID, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "MetadataX:", &lastItemDataX, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "MetadataY:", &lastItemDataY, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "MetadataZ:", &lastItemDataZ, 's', WatchInterpretation::_str);
		
		// Game info
		page = hudConsole->addPage("Game Info");
		
		/*hudConsole->addOption(page, "Item half milk", &chestRandomizer->itemThatReplacesHalfMilk, 0xFF);
		hudConsole->addOption(page, "Item slingshot", &chestRandomizer->itemThatReplacesSlingShot, 0xFF);	*/	
		
		hudConsole->addWatch(page, "CurrentStage:", &gameInfo.currentStage, 's', WatchInterpretation::_str);
		
		hudConsole->addWatch(page, "CurrentPosX:", &currentPosX, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "CurrentPosY:", &currentPosY, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "CurrentPosZ:", &currentPosZ, 's', WatchInterpretation::_str);		
				
		hudConsole->addWatch(page, "CurrentEventID:", &gameInfo.eventSystem.currentEventID, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextStage:", &gameInfo.nextStageVars.nextStage, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "NextRoom:", &gameInfo.nextStageVars.nextRoom, 'd', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextSpawnPoint:", &gameInfo.nextStageVars.nextSpawnPoint, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextState:", &gameInfo.nextStageVars.nextState, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextEventID:", &gameInfo.eventSystem.nextEventID, 'x', WatchInterpretation::_u8);

		// Print
		hudConsole->draw();
		system_console::setState(true, 0);

		//   =================
		//  | Custom events   |
		//   =================

		// Generate random data when a new game starts
		eventListener->addLoadEvent(stage::allStages[Stage_Faron_Woods], 0x1, 0x15, 0xFF, 0xFF, tools::triggerRandomGenerator, event::LoadEventAccuracy::Stage_Room_Spawn);
		
		// Skip sewers when the load happens
		eventListener->addLoadEvent(stage::allStages[Stage_Hyrule_Castle_Sewers], 0x0, 0x18, 0xFF, 0xFF, game_patch::skipSewers, event::LoadEventAccuracy::Stage_Room_Spawn);

		// Fix BiTE
		eventListener->addLoadEvent(stage::allStages[Stage_Faron_Woods], 0x0, 0x17, 0xFF, 0xFF, game_patch::giveEpona, event::LoadEventAccuracy::Stage_Room_Spawn);

		// Kill spider at Link's house
		eventListener->addLoadEvent(stage::allStages[Stage_Ordon_Village], 0x1, 0xFF, 0xFF, 0xFF, game_patch::killLinkHouseSpider, event::LoadEventAccuracy::Stage_Room);

		//   =================
		//  | Function Hooks  |
		//   =================

		fapGm_Execute_trampoline = patch::hookFunction(tp::f_ap_game::fapGm_Execute,
			[]()
			{
				global::modPtr->procNewFrame();
			}
		);

		checkTreasureRupeeReturn_trampoline = patch::hookFunction(tp::d_a_alink::checkTreasureRupeeReturn,
			[](void* unk1, s32 item)
			{
				return global::modPtr->procCheckTreasureRupeeReturn(unk1, item);
			}
		);

		createItemForPresentDemo_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForPresentDemo,
			[](const float pos[3], s32 item, u8 unk3, s32 unk4, s32 unk5, const float unk6[3], const float unk7[3])
			{
				// Call replacement function
				item = global::modPtr->procItemCreateFunc(pos, item, "createItemForPresentDemo");

				return global::modPtr->createItemForPresentDemo_trampoline(pos, item, unk3, unk4, unk5, unk6, unk7);
			}
		);
			

		createItemForTrBoxDemo_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForTrBoxDemo,
			[](const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
			{
				// Call replacement function
				item = global::modPtr->procItemCreateFunc(pos, item, "createItemForTrBoxDemo");

				return global::modPtr->createItemForTrBoxDemo_trampoline(pos, item, unk3, unk4, unk5, unk6);
			}
		);
		//this function is called when the heart spawns, not when link gets it		
		//createItemForTrBoxDemo is called when heart container is gotten
		createItemForBoss_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForBoss,
			[](const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7, s32 unk8)
			{
				// Call replacement function
				item = global::modPtr->procItemCreateFunc(pos, item, "createItemForBoss");

				return global::modPtr->createItemForBoss_trampoline(pos, item, unk3, unk4, unk5, unk6, unk7, unk8);
			}
		);

		createItemForMidBoss_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForMidBoss,
			[](const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], s32 unk6, s32 unk7)
			{
				// Call replacement function
				item = global::modPtr->procItemCreateFunc(pos, item, "createItemForMidBoss");

				return global::modPtr->createItemForMidBoss_trampoline(pos, item, unk3, unk4, unk5, unk6, unk7);
			}
		);

		createItemForDirectGet_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForDirectGet,
			[](const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7)
			{
				// Call replacement function
				item = global::modPtr->procItemCreateFunc(pos, item, "createItemForDirectGet");

				return global::modPtr->createItemForDirectGet_trampoline(pos, item, unk3, unk4, unk5, unk6, unk7);
			}
		);

		evt_control_Skipper_trampoline = patch::hookFunction(tp::evt_control::skipper,
			[](void* evtPtr)
			{
				return global::modPtr->procEvtSkipper(evtPtr);
			}
		);
	}

	void Mod::procNewFrame()
	{
		float linkPos[3];
		getPlayerPos(linkPos);

		snprintf(currentPosX, 30, "%f", linkPos[0]);
		snprintf(currentPosY, 30, "%f", linkPos[1]);
		snprintf(currentPosZ, 30, "%f", linkPos[2]);
		// Increment seed
		if(!customSeed)
		{
			tools::getRandom(0);
		}
		
		// If loading has started check for LoadEvents
		if(isLoading)
		{
			eventListener->checkLoadEvents();
		}

		if(controller::checkForButtonInputSingleFrame((controller::PadInputs::Button_R | controller::PadInputs::Button_Z)))
		{
			// Toggle console			
			system_console::setState(!sysConsolePtr->consoleEnabled, 0);
		}

		if(sysConsolePtr->consoleEnabled)
		{
			if(controller::checkForButtonInputSingleFrame((controller::PadInputs::Button_R | controller::PadInputs::Button_Start)))
			{
				chestRandomizer->generate();
			}

			// Parse inputs of this frame
			switch(tp::m_do_controller_pad::cpadInfo.buttonInputTrg)
			{
				case controller::PadInputs::Button_A:
					hudConsole->performAction(ConsoleActions::Option_Increase);
				break;

				case controller::PadInputs::Button_X:
					hudConsole->performAction(ConsoleActions::Option_Increase, 10);
				break;

				case controller::PadInputs::Button_B:
					hudConsole->performAction(ConsoleActions::Option_Decrease);
				break;

				case controller::PadInputs::Button_Y:
					hudConsole->performAction(ConsoleActions::Option_Decrease, 10);
				break;

				case controller::PadInputs::Button_DPad_Up:
					hudConsole->performAction(ConsoleActions::Move_Up);
				break;

				case controller::PadInputs::Button_DPad_Down:
					hudConsole->performAction(ConsoleActions::Move_Down);
				break;

				case controller::PadInputs::Button_DPad_Left:
					hudConsole->performAction(ConsoleActions::Move_Left);
				break;

				case controller::PadInputs::Button_DPad_Right:
					hudConsole->performAction(ConsoleActions::Move_Right);
				break;
			}
			hudConsole->draw();
		}

		if(truePause && sysConsolePtr->consoleEnabled)
		{
			// Inputs handled, don't pass onto the game
			tp::f_op_scene_req::freezeActors = 1;
			tp::m_do_controller_pad::cpadInfo.buttonInputTrg = 0;
			tp::m_do_controller_pad::cpadInfo.buttonInput = 0;
		}
		else
		{
			tp::f_op_scene_req::freezeActors = 0;
		}

		if(itemSearchID != lastItemSearchID)
		{
			lastItemSearchID = itemSearchID;

			strcpy(itemSearchResults, "404");

			for(u16 i = 0; i < chestRandomizer->totalChecks; i++)
			{
				item::ItemCheck* check = &item::checks[i];
				if (check->destination)
				{
					if(check->destination->itemID == itemSearchID)
					{
						// Found the source
						snprintf(itemSearchResults, 40, "ID: %x Stage: %s Room: %d", check->itemID, check->stage, check->room);
					}
				}
			}
		}
		else if(itemReverseSearchID != lastItemReverseSearchID)
		{
			lastItemReverseSearchID = itemReverseSearchID;

			strcpy(itemReverseSearchResults, "404");

			for(u16 i = 0; i < chestRandomizer->totalChecks; i++)
			{
				item::ItemCheck* check = &item::checks[i];
				if (check->source)
				{
					if(check->source->itemID == itemReverseSearchID)
					{
						// Found the source
						snprintf(itemReverseSearchResults, 40, "ID: %x Stage: %s Room: %d", check->itemID, check->stage, check->room);
					}
				}
			}
		}

		if(inputBuffering)
		{
			tp::m_do_controller_pad::cpadInfo.buttonInputTrg = tp::m_do_controller_pad::cpadInfo.buttonInput;
		}

		// Call original function
		fapGm_Execute_trampoline();
	}

	s32 Mod::procItemCreateFunc(const float pos[3], s32 item, const char funcIdentifier[32])
	{
		strcpy(lastItemFunc, funcIdentifier);
		snprintf(lastItemDataID, 5, "0x%02x", item);
		snprintf(lastItemDataX, 30, "%f", pos[0]);
		snprintf(lastItemDataY, 30, "%f", pos[1]);
		snprintf(lastItemDataZ, 30, "%f", pos[2]);
		// Runs once when Link picks up an item with text and is holding it towards the camera
		if(randoEnabled && strcmp(funcIdentifier, "createItemForDirectGet") != 0 && strcmp(funcIdentifier, "createItemForBoss") != 0 && strcmp(funcIdentifier, "createItemForMidBoss") != 0)
		{
			item = chestRandomizer->getItemReplacement(pos, item);
		}

		return item;
	}
	
	bool Mod::procCheckTreasureRupeeReturn(void* unk1, s32 item)
	{
		// Allow to open
		return false;
	}

	s32 Mod::procEvtSkipper(void* evtPtr)
	{
		// Runs when the user tries to skip a Cutscene		
		if(0 == strcmp(gameInfo.currentStage, stage::allStages[Stage_Sacred_Grove]))
		{
			// We're at sacred grove
			if(0x2 == gameInfo.eventSystem.currentEventID)
			{
				// Master Sword cutscene
				tools::setCutscene(true, false, cutscene_skip::onMasterSwordSkip);
			}
		}
		// Call original function
		return evt_control_Skipper_trampoline(evtPtr);
	}
}