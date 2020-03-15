#include "chestRando.h"
#include "defines.h"
#include "item.h"
#include "items.h"
#include "itemChecks.h"
#include "tools.h"
#include "array.h"

#include <tp/d_com_inf_game.h>
#include <tp/JFWSystem.h>
#include <cstdio>
#include <cstring>

namespace mod
{
	void ChestRandomizer::generate()
	{
		// Reset
		currentPlayerConditions = startConditions;
		currentSeed = tools::randomSeed;

		totalChecks = sizeof(item::checks)/sizeof(item::ItemCheck);
		empty = 0;
		layerCheckCount = 0;

		itemOrderIndex = 0;

		// Set up arrays
		itemOrder = new u8[totalChecks];

		item::ItemCheck* sourceCheck;
		item::ItemCheck* destCheck;

		// Reset randomization
		for(u16 i = 0; i < totalChecks; i++)
		{
			item::checks[i].destination = nullptr;
			item::checks[i].source = nullptr;
		}

		// Lock some checks
		for(u16 i = 0; i < totalChecks; i++)
		{
			if(isLocked(&item::checks[i]))
			{
				placeCheck(&item::checks[i], &item::checks[i]);
			}
		}

		// Place layer checks
		for(u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];

			if(!destCheck->source)
			{
				// Free slot
				if(destCheck->destLayer != 0xFF)
				{
					// Layer check	
					if (isPorgessiveEnabled == 0)
					{
						if(destCheck->itemID == items::Item::Ordon_Sword)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x1, destCheck);//to prevent woodensword from being overwritten before losing it			
						}
						else if(destCheck->itemID == items::Item::Clawshots)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x7, destCheck);//to prevent Clawshots from being overwritten by Clawshot
						}
						else if(destCheck->itemID == items::Item::Big_Quiver)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x4, destCheck);//to prevent bow from being overwritten
						}
						else if(destCheck->itemID == items::Item::Giant_Quiver)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x8, destCheck);//to prevent bow from being overwritten
						}
						else if(destCheck->itemID == items::Item::Giant_Wallet)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x7, destCheck);//to prevent overwriting giant wallet with big wallet
						}
						else
						{
							sourceCheck = findSource(destCheck->destLayer, 0x0, destCheck);
						}
					}
					else
					{
						sourceCheck = findSource(destCheck->destLayer, 0x0, destCheck);
					}
					placeCheck(sourceCheck, destCheck);
					layerCheckCount++;
				}
			}
		}

		// Place items that unlock other locations before caring about remaining items
		for(u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];

			if(!destCheck->source)
			{
				// Free slot
				if(item::getFlags(destCheck->itemID, 0) != 0)
				{
					// This would unlock new checks, so place it
					sourceCheck = findSource(0xFF, 0, destCheck);
					placeCheck(sourceCheck, destCheck);
				}
			}
		}

		// Place remaining
		for(u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];

			if(!destCheck->source)
			{
				sourceCheck = findSource(0xFF, 0, destCheck);
				placeCheck(sourceCheck, destCheck);
			}
		}

		// Count empty
		for(u16 i = 0; i < totalChecks; i++)
		{
			if(!item::checks[i].destination)
			{
				empty++;
			}
		}

		// Done
		checkSum = tools::fletcher16(itemOrder, sizeof(itemOrder));

		delete[] itemOrder;

		// Reset seed if the player wanted to lock it (otherwise it advances anyways)
		tools::randomSeed = currentSeed;
	}

	void ChestRandomizer::placeCheck(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck)
	{
		// Add source item to the itemOrder array
		itemOrder[itemOrderIndex] = sourceCheck->itemID;
		itemOrderIndex++;

		// Place without asking
		sourceCheck->destination = destCheck;
		destCheck->source = sourceCheck;

		// Update player conditions!
		currentPlayerConditions = item::getFlags(destCheck->itemID, currentPlayerConditions);
	}

	item::ItemCheck* ChestRandomizer::findSource(u8 maxLayer, u8 minLayer, item::ItemCheck* destCheck)
	{
		if(minLayer == 0xFF)
		{
			minLayer = 0x0;
		}

		item::ItemCheck* sourceCheck;
		do
		{
			u16 index = tools::getRandom(totalChecks);
			sourceCheck = &item::checks[index];
		} while(!checkCondition(sourceCheck, destCheck) || sourceCheck->destination || sourceCheck->sourceLayer > maxLayer || sourceCheck->sourceLayer < minLayer);

		return sourceCheck;
	}

	bool ChestRandomizer::checkCondition(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck)
	{
		if((sourceCheck->condition & item::Condition::AND) == item::Condition::AND)
		{
			if((sourceCheck->condition & currentPlayerConditions) == sourceCheck->condition)
			{
				return true;
			}
		}
		else
		{
			if((sourceCheck->condition & currentPlayerConditions) != 0)
			{
				return true;
			}
		}

		// If the destination item (which you'll receive) isn't required for this souce it can be placed though
		if((item::getFlags(destCheck->itemID, 0) & sourceCheck->condition) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ChestRandomizer::isLocked(item::ItemCheck* check)
	{
		bool result = false;

		switch(check->type)
		{
			case item::ItemType::Key:
				// Small + Big Keys
				result = true;
			break;

			case item::ItemType::Dungeon:
				// Map, compass
				result = true;
			break;

			case item::ItemType::Story:
				// Ilia quest
				result = true;
			break;
		}

		switch(check->itemID)
		{
			/*case items::Item::Iron_Boots:
				result = true;
			break;*/

			case items::Item::Fishing_Rod:
				result = true;
			break;
						
			case items::Item::Master_Sword:
				result = true;
			break;
			
			case items::Item::Ancient_Sky_Book_empty:
				if (isPorgessiveEnabled == 0)
				{
					result = true;
				}
			break;
			
			case items::Item::Ancient_Sky_Book_partly_filled:
				if (isPorgessiveEnabled == 0)
				{
					result = true;
				}
			break;
						
			case items::Item::Ancient_Sky_Book_completed:
				if (isPorgessiveEnabled == 0)
				{
					result = true;
				}
			break;
			
			
		}

		return result;
	}

	s32 ChestRandomizer::getItemReplacement(const float pos[3], s32 item)
	{
		item::ItemCheck* sourceCheck;
		snprintf(lastSourceInfo, 50, "%s %4.0f %4.0f %4.0f", gameInfo.currentStage, pos[0], pos[1], pos[2]);
		snprintf(lastDestInfo, 50, "No Replacement found for this source");

		for(u16 i = 0; i < totalChecks; i++)
		{
			sourceCheck = &item::checks[i];

			if(0 == strcmp(gameInfo.currentStage, sourceCheck->stage))
			{
				if (isPorgessiveEnabled == 1 && item == items::Item::Ancient_Sky_Book_completed)
				{
					item = items::Item::Ancient_Sky_Book_partly_filled;
				}
				// Correct stage
				if(sourceCheck->itemID == item)
				{
					bool isOk = false;
					
					if (sourceCheck->type == item::ItemType::Bug || sourceCheck->type == item::ItemType::Gear
					|| sourceCheck->type == item::ItemType::Equip || sourceCheck->type == item::ItemType::Dungeon || sourceCheck->type == item::ItemType::Story)
					{//bugs, gear, equipment, story items have unique itemids so position doesn't matter
					//dungeon items are unique in their dungeon
						isOk = true;
					}
					else 
					{							
						if (sourceCheck->type == item::ItemType::PoeSoul)
						{//poes can move a lot so give them more range
							//poe range= ~1400
							rangeX = 2800.0f;
							rangeY = 1400.0f;
							rangeZ = 2800.0f;
						}
						if (sourceCheck->type == item::ItemType::HeartPiece)
						{//some free standing PoH need to be gotten with boomerang or clawshot
							//give more range based on boomerang and clawshot range (clawshot = ~2000)
							rangeX = 2000.0f;
							rangeY = 2000.0f;
							rangeZ = 2000.0f;
						}
						if(tools::fCompare(sourceCheck->position[0], pos[0]) < rangeX)
						{
							if(tools::fCompare(sourceCheck->position[1], pos[1]) < rangeY)
							{
								if(tools::fCompare(sourceCheck->position[2], pos[2]) < rangeZ)
								{
									isOk = true;
								}
							}
						}
					}
					if (isOk)
					{
						snprintf(lastSourceInfo, 50, "%s->%d->%x", sourceCheck->stage, sourceCheck->room, sourceCheck->itemID);
						if(sourceCheck->destination)
						{
							snprintf(lastDestInfo, 50, "%s->%d->%x", sourceCheck->destination->stage, sourceCheck->destination->room, sourceCheck->destination->itemID);
							item = sourceCheck->destination->itemID;
							// Unset this check
							sourceCheck->destination = nullptr;
							//progressive checks (doesn't work if you already have items when generating seed)
							if (isPorgessiveEnabled == 1)
							{
								if(item == items::Item::Wooden_Sword)
								{
									if (swordState == 1)
									{
										item = items::Item::Ordon_Sword;
										swordState = 2;
									}	
									else 
									{
										swordState = 1;
									}
								}
								else if(item == items::Item::Ordon_Sword)
								{
									if (swordState == 0)
									{
										item = items::Item::Wooden_Sword;
										swordState = 1;
									}	
									else 
									{
										swordState = 2;
									}		
								}
								else if(item == items::Item::Clawshot)
								{
									if (clawshotState == 1)
									{
										item = items::Item::Clawshots;
										clawshotState = 2;
									}		
									else 
									{
										clawshotState = 1;
									}	
								}
								else if(item == items::Item::Clawshots)
								{
									if (clawshotState == 0)
									{
										item = items::Item::Clawshot;
										clawshotState = 1;
									}			
									else 
									{
										clawshotState = 2;
									}					
								}
								else if(item == items::Item::Heros_Bow)
								{
									if (bowState == 1)
									{
										item = items::Item::Big_Quiver;
										bowState = 2;
									}
									else if (bowState == 2)
									{
										item = items::Item::Giant_Quiver;
										bowState = 3;
									}									
									else 
									{
										bowState = 1;
									}						
								}
								else if(item == items::Item::Big_Quiver)
								{
									if (bowState == 0)
									{
										item = items::Item::Heros_Bow;
										bowState = 1;
									}
									else if (bowState == 2)
									{
										item = items::Item::Giant_Quiver;
										bowState = 3;
									}									
									else 
									{
										bowState = 2;
									}
								}
								else if(item == items::Item::Giant_Quiver)
								{
									if (bowState == 0)
									{
										item = items::Item::Heros_Bow;
										bowState = 1;
									}
									else if (bowState == 1)
									{
										item = items::Item::Big_Quiver;
										bowState = 2;
									}									
									else 
									{
										bowState = 3;
									}
								}
								else if(item == items::Item::Big_Wallet)
								{
									if (walletState == 1)
									{
										item = items::Item::Giant_Wallet;
										walletState = 2;
									}			
									else 
									{
										walletState = 1;
									}
								}
								else if(item == items::Item::Giant_Wallet)
								{
									if (walletState == 0)
									{
										item = items::Item::Big_Wallet;
										walletState = 1;
									}			
									else 
									{
										walletState = 2;
									}
								}
								else if(item == items::Item::Ancient_Sky_Book_empty)
								{
									if (bookState > 1 && bookState < 6)
									{
										item = items::Item::Ancient_Sky_Book_partly_filled;
										bookState++;
									}
									else if (bookState == 6)
									{
										item = items::Item::Ancient_Sky_Book_completed;
										bookState = 7;
									}									
									else 
									{
										bookState = 1;
									}
								}
								else if(item == items::Item::Ancient_Sky_Book_partly_filled)
								{
									if (bookState == 0)
									{
										item = items::Item::Ancient_Sky_Book_empty;
										bookState = 1;
									}
									else if (bookState == 6)
									{
										item = items::Item::Ancient_Sky_Book_completed;
										bookState = 7;
									}									
									else 
									{
										bookState++;
									}
								}
							}
							return item;
						}
						else
						{
							snprintf(lastDestInfo, 50, "No replacement here.");
							//no return in the case where 2 items are close enough to be considered the same (bug rewards for example)
						}
					}
				}
			}
		}
		return item;
	}
}