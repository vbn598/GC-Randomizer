#include "item.h"
#include "items.h"
#include "defines.h"

namespace mod::item
{
	u32 getFlags(u8 item, u32 currentPlayerConditions)
	{
		u32 flags = currentPlayerConditions;
		switch(item)
		{
			case items::Item::Lantern:
				flags |= item::Condition::Lantern;
			break;

			case items::Item::Iron_Boots:
				flags |= item::Condition::Iron_Boots;
			break;

			case items::Item::Boomerang:
				flags |= item::Condition::Boomerang;
			break;

			case items::Item::Slingshot:
				flags |= item::Condition::Slingshot;
			break;

			case items::Item::Heros_Bow:
				flags |= item::Condition::Bow;
			break;

			case items::Item::Goron_Bomb_Bag:
				flags |= item::Condition::Bombs;
				flags |= item::Condition::Water_Bombs;
			break;

			case items::Item::Bomb_Bag_Regular_Bombs:
				flags |= item::Condition::Bombs;
				flags |= item::Condition::Water_Bombs;
			break;

			case items::Item::Ball_and_Chain:
				flags |= item::Condition::Ball_And_Chain;
			break;

			case items::Item::Clawshot:
				flags |= item::Condition::Clawshot;
			break;

			case items::Item::Clawshots:
				flags |= item::Condition::Double_Clawshot;
			break;

			case items::Item::Spinner:
				flags |= item::Condition::Spinner;
			break;

			case items::Item::Dominion_Rod:
				flags |= item::Condition::Dominion_Rod;
			break;

			case items::Item::Zora_Armor:
				flags |= item::Condition::Zora_Armor;
			break;
			
			case items::Item::Coral_Earring:
				flags |= item::Condition::Coral_Earring;
			break;
			
			case items::Item::Wooden_Sword:
				flags |= item::Condition::Wooden_Sword;
			break;
			
			case items::Item::Ordon_Sword:
				flags |= item::Condition::Ordon_Sword;
			break;
						
			case items::Item::Master_Sword:
				flags |= item::Condition::Master_Sword;
			break;
						
			case items::Item::Ordon_Shield:
				flags |= item::Condition::Shield;
			break;
						
			case items::Item::Wooden_Shield:
				flags |= item::Condition::Shield;
			break;
						
			case items::Item::Hylian_Shield:
				flags |= item::Condition::Shield;
			break;
			
		}

		if ((flags & item::Condition::Bow) && ((flags & item::Condition::Bombs) || (flags & item::Condition::Water_Bombs)))
		{
			// We have bow && (bombs || waterbombs) = bombarrows
			flags |= item::Condition::Bomb_Arrows;
		}

		return flags;
	}
}