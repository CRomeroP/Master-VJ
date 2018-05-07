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
	Room* forest = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");
	Room* centrance = new Room("Castle_entrance", "You are inside a beautiful but small white house.");
	Room* chall = new Room("Basement", "The basement features old furniture and dim light.");
	Room* cave = new Room("Cave", "You are inside a dark cave");
	Room* lwing = new Room("Castle left wing", "left");
	Room* rwing = new Room("Castle right wing", "right");
	Room* throne = new Room("Throne room", "Throne");

	Exit* ex1 = new Exit("north", "south", "Little path", centrance, forest, true);
	Exit* ex2 = new Exit("east", "north", "Passage", forest, cave);
	Exit* ex3 = new Exit("west", "east", "Cave entrance", cave, centrance);
	Exit* ex4 = new Exit("south", "north", "Castle bridge", centrance, chall);
	Exit* ex5 = new Exit("west", "east", "wooden door", chall, lwing);
	Exit* ex6 = new Exit("east", "west", "wooden door", chall, rwing);
	Exit* ex7 = new Exit("south", "north", "Double doors", chall, throne);
	ex4->locked = true;
	//ex5->locked = true;

	entities.push_back(forest);
	entities.push_back(centrance);
	entities.push_back(chall);
	entities.push_back(cave);
	entities.push_back(lwing);
	entities.push_back(rwing);
	entities.push_back(throne);

	entities.push_back(ex1);
	entities.push_back(ex2);
	entities.push_back(ex3);
	entities.push_back(ex4);
	entities.push_back(ex5);
	entities.push_back(ex6);
	entities.push_back(ex7);

	// Creatures ----
	//Creature* butler = new Creature("Butler", "It's James, the house Butler.", house);
	//butler->hit_points = 10;
	Creature* bat = new Creature("Bat", "An annoying bat", cave);
	bat->hit_points = 5;
	bat->min_damage = 1;
	bat->max_damage = 2;
	bat->gold = 3;

	//entities.push_back(butler);
	entities.push_back(bat);

	//NPCs
	Npc* merchant = new Npc("Merchant", "A wandering merchant in search of fortune", MERCHANT, centrance);
	merchant->dialog = "Do you wanna buy something?";
	entities.push_back(merchant);


	// Items -----
	//Item* mailbox = new Item("Mailbox", "Looks like it might contain something.", house);
	Item* key = new Item("Key", "Old iron key.", merchant);
	ex4->key = key;
	key->cost = 3;

	Item* sword = new Item("Sword", "A simple old and rusty sword.", merchant, WEAPON);
	sword->min_value = 2;
	sword->max_value = 6;
	sword->cost = 4;

	Item* sword2(sword);
	//sword2->parent = butler;

	//Item* shield = new Item("Shield", "An old wooden shield.", butler, ARMOUR);
	//shield->min_value = 1;
	//shield->max_value = 3;
	//butler->AutoEquip();

	Item* shpotion = new Item("Small_heal_potion", "Potion that heals 2 hit points", bat, HEAL);
	shpotion->max_value = 2;
	shpotion->parent = bat;

	//entities.push_back(mailbox);
	entities.push_back(sword);
	//entities.push_back(shield);
	entities.push_back(key);
	entities.push_back(shpotion);

	// Player ----
	player = new Player("Hero", "You are an awesome adventurer!", centrance);
	player->hit_points = 25;
	player->gold = 4;
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
				player->Talk(args);
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
				player->Drop(args);
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