#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"
#include "NPC.h"

// ----------------------------------------------------
World::World()
{
	tick_timer = clock();

	// Rooms ----
	// Outside rooms
	Room* forest = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");
	Room* centrance = new Room("CastleEntrance", "You are in the entrance of the castle.");
	Room* cave = new Room("Cave", "You are inside a dark cave");
	// Inside rooms
	Room* chall = new Room("CastleHall", "You are in the main room of the castle, a huge and illuminated hall.");
	Room* ewing = new Room("CastleEastWing", "Small room used to wait for an audience");
	Room* wwing = new Room("CastleWestWing", "Room plenty of arworks. One statue seems suspicious");
	Room* throne = new Room("ThroneRoom", "You see the king sit in his throne, in a large and dark room");

	// "Secret" room is not created yet
	newRoom = false;

	// Exits
	Exit* ex1 = new Exit("north", "south", "Little path", centrance, forest);
	Exit* ex2 = new Exit("east", "north", "Passage", forest, cave,true);
	Exit* ex3 = new Exit("west", "east", "Cave entrance", cave, centrance);
	Exit* ex4 = new Exit("south", "north", "Castle bridge", centrance, chall);
	Exit* ex5 = new Exit("west", "east", "wooden door", chall, wwing);
	Exit* ex6 = new Exit("east", "west", "wooden door", chall, ewing);
	Exit* ex7 = new Exit("south", "north", "Double doors", chall, throne);
	ex1->locked = true;
	ex4->locked = true;
	ex7->locked = true;

	// Rooms and exits push
	entities.push_back(forest);
	entities.push_back(centrance);
	entities.push_back(chall);
	entities.push_back(cave);
	entities.push_back(ewing);
	entities.push_back(wwing);
	entities.push_back(throne);

	// Save forest to use it as exit of "secret" room (tree)
	SRoom = forest;

	entities.push_back(ex1);
	entities.push_back(ex2);
	entities.push_back(ex3);
	entities.push_back(ex4);
	entities.push_back(ex5);
	entities.push_back(ex6);
	entities.push_back(ex7);

	// Exit to open with statue
	open = ex7;

	// Creatures ----
	Creature* bat = new Creature("Bat", "An annoying bat", cave);
	bat->hit_points = 5;
	bat->min_damage = 1;
	bat->max_damage = 2;
	bat->gold = 3;

	Creature* wolf = new Creature("Wolf", "A ferocious wolf that seems hungry", forest);
	wolf->hit_points = 10;
	wolf->min_damage = 3;
	wolf->max_damage = 5;
	wolf->gold = 6;

	Creature* armor = new Creature("LivingArmor", "Living armor who protect the caste", ewing);
	armor->hit_points = 10;
	armor->gold = 10;

	Creature* king = new Creature("King", "The King, the ruler of the kingdom. He is a strong opponent", throne);
	king->hit_points = 15;

	// King as the enemy who must kill to succesfully end the game.
	end = king;

	// Creatures push
	entities.push_back(bat);
	entities.push_back(wolf);
	entities.push_back(armor);
	entities.push_back(king);

	//NPCs
	Npc* merchant = new Npc("Merchant", "A wandering merchant in search of fortune", MERCHANT, centrance);
	merchant->dialog = "Do you wanna buy something?";
	merchant->min_damage = 500;
	merchant->max_damage = 999;
	merchant->hit_points = 75;
	entities.push_back(merchant);
	

	Npc* man = new Npc("WiseMan", "An old man who knows some secrets about this place", CITIZEN, wwing);
	man->dialog = "You want to open the throne room? I hear something about an ornamental sword lost in the forest.\nMaybe if someone find it and put it in the statue in the other room...";
	man->hit_points = 100;
	man->min_damage = 10;
	man->max_damage = 15;
	entities.push_back(man);


	// Items -----
	// Items tools
	Item* statue = new Item("Statue", "A statue representing a legendary hero. Seems like something is missing in his hand", ewing, TOOL);

	// Items keys
	Item* forest_key = new Item("ForestKey", "Old iron key.", merchant, COMMON);
	ex1->key = forest_key;
	forest_key->cost = 3;

	Item* castle_key = new Item("CastleKey", "Rusty key. Seems to open a big door", wolf, COMMON);
	ex4->key = castle_key;

	// Items Equipment 
	Item* sword = new Item("Sword", "A small and old sword.", merchant, WEAPON);
	sword->min_value = 2;
	sword->max_value = 6;
	sword->cost = 4;

	Item* shield = new Item("SmallShield", "Weak shield, but still usefull", merchant, ARMOUR);
	shield->min_value = 1;
	shield->max_value = 2;
	shield->cost = 6;

	Item* longSword = new Item("LongSword", "Sword from the soldiers of the castle",armor, WEAPON);
	longSword->min_value = 3;
	longSword->max_value = 7;

	Item* ironShield = new Item("IronShield","Heavy shield used by the castle soldiers",armor, ARMOUR);
	ironShield->min_value = 2;
	ironShield->max_value = 3;

	Item* kingShield = new Item("King Shield", "", king,ARMOUR);
	kingShield->min_value = 3;
	kingShield->max_value = 4;
	
	Item* kingSword = new Item("King Sword","",king,WEAPON);
	kingSword->min_value = 5;
	kingSword->max_value = 8;

	// Equip enemies
	armor->AutoEquip();
	king->AutoEquip();

	// Usable items
	Item* shpotion = new Item("SmallHealPotion", "Potion that heals 9 hit points", bat, HEAL);
	shpotion->max_value = 9;

	Item* mpotion = new Item("MediumHealPotion", "Potion that heals 20 hit points", merchant, HEAL);
	mpotion->max_value = 20;
	mpotion->cost = 15;

	// Items push
	entities.push_back(statue);
	entities.push_back(forest_key);
	entities.push_back(castle_key);
	entities.push_back(sword);
	entities.push_back(shield);
	entities.push_back(longSword);
	entities.push_back(ironShield);
	entities.push_back(kingShield);
	entities.push_back(kingSword);
	entities.push_back(shpotion);
	entities.push_back(mpotion);

	// Player ----
	player = new Player("Hero", "An exsoldier of the king looking for revenge", centrance);
	player->hit_points = player ->max_hp = 40;
	entities.push_back(player);
}

// ----------------------------------------------------
World::~World()
{
	for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;

	entities.clear();
}

// ----------------------------------------------------
bool World::Tick(vector<string>& args)
{
	bool ret = true;

	if(args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args);

	GameLoop();

	return ret;
}

// ----------------------------------------------------
void World::GameLoop()
{
	clock_t now = clock();

	if((now - tick_timer) / CLOCKS_PER_SEC > TICK_FREQUENCY)
	{
		for(list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Tick();

		tick_timer = now;
	}
}

// ----------------------------------------------------
bool World::ParseCommand(vector<string>& args)
{
	bool ret = true;

	switch(args.size())
	{
		case 1: // commands with no arguments ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "north") || Same(args[0], "n"))
			{
				(args.size() == 1) ? args.push_back("north") : args[1] = "north";
				player->Go(args);
			}
			else if(Same(args[0], "south") || Same(args[0], "s"))
			{
				(args.size() == 1) ? args.push_back("south") : args[1] = "south";
				player->Go(args);
			}
			else if(Same(args[0], "east") || Same(args[0], "e"))
			{
				(args.size() == 1) ? args.push_back("east") : args[1] = "east";
				player->Go(args);
			}
			else if(Same(args[0], "west") || Same(args[0], "w"))
			{
				(args.size() == 1) ? args.push_back("west") : args[1] = "west";
				player->Go(args);
			}
			else if(Same(args[0], "up") || Same(args[0], "u"))
			{
				(args.size() == 1) ? args.push_back("up") : args[1] = "up";
				player->Go(args);
			}
			else if(Same(args[0], "down") || Same(args[0], "d"))
			{
				(args.size() == 1) ? args.push_back("down") : args[1] = "down";
				player->Go(args);
			}
			else if(Same(args[0], "stats") || Same(args[0], "st"))
			{
				player->Stats();
			}
			else if(Same(args[0], "inventory") || Same(args[0], "i"))
			{
				player->Inventory();
			}
			else
				ret = false;
			break;
		}
		case 2: // commands with one argument ------------------------------
		{
			if(Same(args[0], "look") || Same(args[0], "l"))
			{
				player->Look(args);
			}
			else if(Same(args[0], "go"))
			{
				player->Go(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				player->Drop(args);
			}
			else if(Same(args[0], "equip") || Same(args[0], "eq"))
			{
				player->Equip(args);
			}
			else if(Same(args[0], "unequip") || Same(args[0], "uneq"))
			{
				player->UnEquip(args);
			}
			else if(Same(args[0], "examine") || Same(args[0], "ex"))
			{
				player->Examine(args);
			}
			else if(Same(args[0], "attack") || Same(args[0], "at"))
			{
				player->Attack(args);
			}
			else if(Same(args[0], "loot") || Same(args[0], "lt"))
			{
				player->Loot(args);
			}
			else if (Same(args[0], "use") || Same(args[0], "u"))
			{
				player->Use(args);
			}
			else if (Same(args[0], "talk") || Same(args[0], "t"))
			{
				aux = player->Talk(args);
				// Create room after talking for first time with Wise Man npc
				if (Same(args[1], "WiseMan") && !newRoom && aux) CreateNewRoom();
			}
			else
				ret = false;
			break;
		}
		case 3: // commands with two arguments ------------------------------
		{
			if (Same(args[0], "buy") || Same(args[0], "b"))
			{
			player->Buy(args);
			}
			break;
		}
		case 4: // commands with three arguments ------------------------------
		{
			if(Same(args[0], "unlock") || Same(args[0], "unlk"))
			{
				player->UnLock(args);
			}
			else if(Same(args[0], "lock") || Same(args[0], "lk"))
			{
				player->Lock(args);
			}
			else if(Same(args[0], "take") || Same(args[0], "pick"))
			{
				player->Take(args);
			}
			else if(Same(args[0], "drop") || Same(args[0], "put"))
			{
				aux = player->Drop(args);
				//Open trhone room when use ornamental sword in statue
				if (Same(args[1], "OrnamentalSword") && Same(args[3], "statue") && aux)
				{
					open->locked = false;
					cout << "\nThe exit to throne room is now open!\n";
				}
			}
			else
				ret = false;
			break;
		}
		default:
		ret =  false;
	}

	return ret;
}

// Function to create a room after an event occurs
void World::CreateNewRoom()
{
	// Room
	Room* tree = new Room("Tree", "Tree");

	entities.push_back(tree);

	// Exit
	Exit* ex8 = new Exit("up", "down", "Climb tree", SRoom, tree);

	entities.push_back(ex8);

	// Creature
	Creature* bird = new Creature("Bird", "a bird", tree);
	bird->hit_points = 13;
	bird->min_damage = 3;
	bird->max_damage = 5;
	bird->gold = 12;

	entities.push_back(bird);

	// Items
	Item* chest = new Item("Chest", "Looks like it might contain something.", tree, TOOL);
	Item* ornamental = new Item("OrnamentalSword", "Ornamental", chest);

	entities.push_back(chest);
	entities.push_back(ornamental);

	// Mark room as created to avoid duplications
	newRoom = true;

}

// Returns when king dies to end the game
int World::GameEnd()
{
	if (!end->IsAlive()) return 1;
	if (!player->IsAlive()) return 2;
	return 0;
}