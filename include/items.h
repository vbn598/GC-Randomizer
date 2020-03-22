#pragma once

#include "defines.h"

namespace mod::items
{
	enum Item : u8
	{
		Recovery_Heart	=	0x00,
		Green_Rupee	=	0x01,
		Blue_Rupee	=	0x02,
		Yellow_Rupee	=	0x03,
		Red_Rupee	=	0x04,
		Purple_Rupee	=	0x05,
		Orange_Rupee	=	0x06,
		Silver_Rupee	=	0x07,
		/*Borrow_Bomb_Bag?	=	0x08,*/
		/*Borrow_Bomb_Bag?	=	0x09,*/
		Bombs_5	=	0x0A,
		Bombs_10	=	0x0B,
		Bombs_20	=	0x0C,
		Bombs_30	=	0x0D,
		Arrows_10	=	0x0E,
		Arrows_20	=	0x0F,
		Arrows_30	=	0x10,
		Arrows_1	=	0x11,
		Seeds_50	=	0x12,
		/*?	=	0x13,*/
		/*?	=	0x14,*/
		/*?	=	0x15,*/
		Water_Bombs_5	=	0x16,
		Water_Bombs_10	=	0x17,
		Water_Bombs_15	=	0x18,
		/*unused*/Water_Bombs_3	=	0x19,
		Bomblings_5	=	0x1A,
		Bomblings_10	=	0x1B,
		/*unused*/Bomblings_3	=	0x1C,
		/*unused*/Bombling_X	=	0x1D,
		Fairy	=	0x1E,
		Recovery_Heart_x3	=	0x1F,
		Small_Key	=	0x20,
		Piece_of_Heart	=	0x21,
		Heart_Container	=	0x22,
		Dungeon_Map	=	0x23,
		Compass	=	0x24,
		Ooccoo	=	0x25,
		Big_Key	=	0x26,
		Ooccoo_Jr	=	0x27,
		Ordon_Sword	=	0x28,
		Master_Sword	=	0x29,
		Ordon_Shield	=	0x2A,
		Wooden_Shield	=	0x2B,
		Hylian_Shield	=	0x2C,
		Asheis_Sketch_Ooccoos_Note	=	0x2D,
		Ordon_Clothing	=	0x2E,
		Heros_Clothes	=	0x2F,
		Magic_Armor	=	0x30,
		Zora_Armor	=	0x31,
		/*does notting*/Shadow_Crystal	=	0x32,
		Ooccoo_Dungeon	=	0x33,
		/*unused*/Small_Wallet	=	0x34,
		Big_Wallet	=	0x35,
		Giant_Wallet	=	0x36,
		/*Piece_of_Heart_2?	=	0x37,*/
		/*Piece_of_Heart_3?	=	0x38,*/
		/*Piece_of_Heart_4?	=	0x39,*/
		/*Piece_of_Heart_5?	=	0x3A,*/
		/*sword?	=	0x3B,*/
		/*?	=	0x3C,*/
		Coral_Earring	=	0x3D,
		Hawkeye	=	0x3E,
		Wooden_Sword	=	0x3F,
		Boomerang	=	0x40,
		Spinner	=	0x41,
		Ball_and_Chain	=	0x42,
		Heros_Bow	=	0x43,
		Clawshot	=	0x44,
		Iron_Boots	=	0x45,
		Dominion_Rod	=	0x46,
		Clawshots	=	0x47,
		Lantern	=	0x48,
		Master_Sword_Light	=	0x49,
		Fishing_Rod	=	0x4A,
		Slingshot	=	0x4B,
		Dominion_Rod_2	=	0x4C,
		/*?	=	0x4D,*/
		/*?	=	0x4E,*/
		Giant_Bomb_Bag	=	0x4F,
		Empty_Bomb_Bag	=	0x50,
		Goron_Bomb_Bag	=	0x51,
		/*Giant_Bomb_Bag?	=	0x52,*/
		/*?	=	0x53,*/
		/*unused*/Small_Quiver	=	0x54,
		Big_Quiver	=	0x55,
		Giant_Quiver	=	0x56,
		/*?	=	0x57,*/
		Fishing_Rod_Lure	=	0x58,
		Bow_Bombs	=	0x59,
		Bow_Hawkeye	=	0x5A,
		Fishing_Rod_Bee_Larva	=	0x5B,
		Fishing_Rod_Coral_Earring	=	0x5C,
		Fishing_Rod_Worm	=	0x5D,
		Fishing_Rod_Earring_Bee_Larva	=	0x5E,
		Fishing_Rod_Earring_Worm	=	0x5F,
		Empty_Bottle	=	0x60,
		Red_Potion_Shop	=	0x61,
		Green_Potion	=	0x62,
		Blue_Potion	=	0x63,
		Milk	=	0x64,
		Sera_Bottle	=	0x65,
		Lantern_oil_Shop	=	0x66,
		Water	=	0x67,
		Lantern_Oil_Scooped	=	0x68,
		Red_Potion_Scooped	=	0x69,
		Nasty_soup	=	0x6A,
		Hot_spring_water_Scooped	=	0x6B,
		Fairy_Bottle	=	0x6C,
		Hot_Spring_Water_Shop	=	0x6D,
		Lantern_Refill_Scooped	=	0x6E,
		Lantern_Refill_Shop	=	0x6F,
		Bomb_Bag_Regular_Bombs	=	0x70,
		Bomb_Bag_Water_Bombs	=	0x71,
		Bomb_Bag_Bombslings	=	0x72,
		Fairy_Tears	=	0x73,
		Worm	=	0x74,
		Jovani_Bottle	=	0x75,
		Bee_Larva_Scooped	=	0x76,
		Rare_Chu_Jelly	=	0x77,
		Red_Chu_Jelly	=	0x78,
		Blue_Chu_Jelly	=	0x79,
		Green_Chu_Jelly	=	0x7A,
		Yellow_Chu_Jelly	=	0x7B,
		Purple_Chu_Jelly	=	0x7C,
		Simple_Soup	=	0x7D,
		Good_Soup	=	0x7E,
		Superb_Soup	=	0x7F,
		Renardos_Letter	=	0x80,
		Invoice	=	0x81,
		Wooden_Statue	=	0x82,
		Ilias_Charm	=	0x83,
		Horse_Call	=	0x84,
		/*?	=	0x85,*/
		/*?	=	0x86,*/
		/*?	=	0x87,*/
		/*?	=	0x88,*/
		/*?	=	0x89,*/
		/*?	=	0x8A,*/
		/*?	=	0x8B,*/
		/*?	=	0x8C,*/
		/*?	=	0x8D,*/
		/*?	=	0x8E,*/
		/*?	=	0x8F,*/
		Aurus_Memo	=	0x90,
		Asheis_Sketch	=	0x91,
		/*?	=	0x92,*/
		/*?	=	0x93,*/
		/*?	=	0x94,*/
		/*?	=	0x95,*/
		/*?	=	0x96,*/
		/*?	=	0x97,*/
		/*?	=	0x98,*/
		/*?	=	0x99,*/
		/*?	=	0x9A,*/
		/*?	=	0x9B,*/
		Lantern_Yellow_Chu_Chu	=	0x9C,
		Coro_Bottle	=	0x9D,
		Bee_Larva_Shop	=	0x9E,
		Black_Chu_Jelly	=	0x9F,
		/*unused*/Tear_Of_Light	=	0xA0,
		Vessel_Of_Light_Faron	=	0xA1,
		Vessel_Of_Light_Eldin	=	0xA2,
		Vessel_Of_Light_Lanayru	=	0xA3,
		/*unused*/Vessel_Of_Light_Full	=	0xA4,
		/*?	=	0xA5,*/
		/*?	=	0xA6,*/
		/*?	=	0xA7,*/
		/*?	=	0xA8,*/
		/*?	=	0xA9,*/
		/*?	=	0xAA,*/
		/*?	=	0xAB,*/
		/*?	=	0xAC,*/
		/*?	=	0xAD,*/
		/*?	=	0xAE,*/
		/*?	=	0xAF,*/
		Ilias_Scent	=	0xB0,
		/*Unused_Scent?	=	0xB1,*/
		Poe_Scent	=	0xB2,
		Reekfish_Scent	=	0xB3,
		Youths_Scent	=	0xB4,
		Medicine_Scent	=	0xB5,
		/*?	=	0xB6,*/
		/*?	=	0xB7,*/
		/*?	=	0xB8,*/
		/*?	=	0xB9,*/
		/*?	=	0xBA,*/
		/*?	=	0xBB,*/
		/*?	=	0xBC,*/
		/*?	=	0xBD,*/
		/*?	=	0xBE,*/
		/*Bottle_Insides?	=	0xBF,*/
		Male_Beetle	=	0xC0,
		Female_Beetle	=	0xC1,
		Male_Butterfly	=	0xC2,
		Female_Butterfly	=	0xC3,
		Male_Stag_Beetle	=	0xC4,
		Female_Stag_Beetle	=	0xC5,
		Male_Grasshopper	=	0xC6,
		Female_Grasshopper	=	0xC7,
		Male_Phasmid	=	0xC8,
		Female_Phasmid	=	0xC9,
		Male_Pill_Bug	=	0xCA,
		Female_Pill_Bug	=	0xCB,
		Male_Mantis	=	0xCC,
		Female_Mantis	=	0xCD,
		Male_Ladybug	=	0xCE,
		Female_Ladybug	=	0xCF,
		Male_Snail	=	0xD0,
		Female_Snail	=	0xD1,
		Male_Dragonfly	=	0xD2,
		Female_Dragonfly	=	0xD3,
		Male_Ant	=	0xD4,
		Female_Ant	=	0xD5,
		Male_Dayfly	=	0xD6,
		Female_Dayfly	=	0xD7,
		/*?	=	0xD8,*/
		/*?	=	0xD9,*/
		/*?	=	0xDA,*/
		/*?	=	0xDB,*/
		/*?	=	0xDC,*/
		/*?	=	0xDD,*/
		/*?	=	0xDE,*/
		/*?	=	0xDF,*/
		Poe_Soul	=	0xE0,
		/*?	=	0xE1,*/
		/*?	=	0xE2,*/
		/*?	=	0xE3,*/
		/*?	=	0xE4,*/
		/*?	=	0xE5,*/
		/*?	=	0xE6,*/
		/*?	=	0xE7,*/
		/*?	=	0xE8,*/
		Ancient_Sky_Book_empty	=	0xE9,
		Ancient_Sky_Book_partly_filled	=	0xEA,
		Ancient_Sky_Book_completed	=	0xEB,
		/*unused*/Ooccoo_Beta/*CitS?*/	=	0xEC,
		Purple_Rupee_Links_house	=	0xED,
		Small_Key_N_Faron_Gate	=	0xEE,
		/*Blue_Fire?	=	0xEF,*/
		/*Blue_Fire?	=	0xF0,*/
		/*Blue_Fire?	=	0xF1,*/
		/*Blue_Fire?	=	0xF2,*/
		Gate_Keys	=	0xF3,
		Ordon_Pumpkin	=	0xF4,
		Ordon_Goat_Cheese	=	0xF5,
		Bed_Key	=	0xF6,
		/*Shield?	=	0xF7,*/
		Got_Lantern_Back	=	0xF8,
		Key_Shard_1	=	0xF9,
		Key_Shard_2	=	0xFA,
		Key_Shard_3	=	0xFB,
		/*Key?	=	0xFC,*/
		Big_Key_Goron_Mines	=	0xFD,
		Coro_Key	=	0xFE,
		/*Gives_Vanilla*/NullItem	=	0xFF
	};
}