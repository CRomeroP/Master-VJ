#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"
#include "NPC.h"



Npc::Npc(const char* title, const char* description,NPCType clase, Room* room) :
	Creature(title, description, room)
{
	type = NPC;
	clas = clase;
}


Npc::~Npc()
{
}

// Show npc Inventory
void Npc::Inventory() const
{

	list<Entity*> items;
	FindAll(ITEM, items);

	if (items.size() == 0)
	{
		cout << "\nThis merchant dont have more items to sell.\n";
		return;
	}

	cout << "\nItems for sell: ";
	for (list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
			cout << "\n" << (*it)->name << " " << (*it)->cost << " gold";
	}
	cout << "\n";
}

//Show npc stats
void Npc::Stats() const
{
	cout << "\nStats of this NPC are unknown.\n";
}
