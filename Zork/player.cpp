#include <iostream>
#include <algorithm>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "NPC.h"
#include "player.h"

// ----------------------------------------------------
Player::Player(const char* title, const char* description, Room* room) :
Creature(title, description, room)
{
	type = PLAYER;
}

// ----------------------------------------------------
Player::~Player()
{
}

// ----------------------------------------------------
void Player::Look(const vector<string>& args) const
{
	if(args.size() > 1)
	{
		for(list<Entity*>::const_iterator it = parent->container.begin(); it != parent->container.cend(); ++it)
		{
			if(Same((*it)->name, args[1]) || ((*it)->type == EXIT && Same(args[1], ((Exit*)(*it))->GetNameFrom((Room*)parent))))
			{
				(*it)->Look();
				return;
			}
		}

		if(Same(args[1], "me"))
		{
			cout << "\n" << name << "\n";
			cout << description << "\n";
		}
	}
	else
	{
		parent->Look();
	}
}

// ----------------------------------------------------
bool Player::Go(const vector<string>& args)
{
	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->locked)
	{
		cout << "\nThat exit is locked.\n";
		return false;
	}

	cout << "\nYou take direction " << exit->GetNameFrom((Room*) parent) << "...\n";
	ChangeParentTo(exit->GetDestinationFrom((Room*) parent));
	parent->Look();

	return true;
}


// ----------------------------------------------------
bool Player::Take(const vector<string>& args)
{
	if(args.size() == 4)
	{
		Item* item = (Item*)parent->Find(args[3], ITEM);

		// we could pick something from a container in our inventory ...
		if(item == NULL)
			item = (Item*)Find(args[3], ITEM);

		if(item == NULL)
		{
			cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return false;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if(subitem == NULL)
		{
			cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return false;
		}

		cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(this);
	}
	else if(args.size() == 2)
	{
		Item* item = (Item*)parent->Find(args[1], ITEM);

		if(item == NULL)
		{
			cout << "\nThere is no item here with that name.\n";
			return false;
		}
		
		switch (item->item_type)
		{
			case TOOL:
			cout << "\nYou cant take this item\n";
			return false;

			default:
			break;
		}
		cout << "\nYou take " << item->name << ".\n";
		item->ChangeParentTo(this);
	}

	return false;
}

// ----------------------------------------------------
void Player::Inventory() const
{
	list<Entity*> items;
	FindAll(ITEM, items);

	if(items.size() == 0)
	{
		cout << "\nYou do not own any item.\n";
		return;
	}

	for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if(*it == weapon)
			cout << "\n" << (*it)->name << " (as weapon)";
		else if(*it == armour)
			cout << "\n" << (*it)->name << " (as armour)";
		else
			cout << "\n" << (*it)->name;
	}

	cout << "\n";
}

// ----------------------------------------------------
bool Player::Drop(const vector<string>& args)
{
	if(args.size() == 2)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if(item == NULL)
		{
			cout << "\nThere is no item on you with that name.\n";
			return false;
		}

		cout << "\nYou drop " << item->name << "...\n";
		item->ChangeParentTo(parent);

		return true;
	}
	else if(args.size() == 4)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if(item == NULL)
		{
			cout << "\nCan not find '" << args[1] << "' in your inventory.\n";
			return false;
		}

		Item* container = (Item*)parent->Find(args[3], ITEM);

		if(container == NULL)
		{
			container = (Item*)Find(args[3], ITEM);
			cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return false;
		}

		cout << "\nYou put " << item->name << " into " << container->name << ".\n";
		item->ChangeParentTo(container);

		return true;
	}
	
	return false;
}

// ----------------------------------------------------
bool Player::Equip(const vector<string>& args)
{
	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
	{
		cout << "\nCannot find '" << args[1] << "' is not in your inventory.\n";
		return false;
	}

	switch(item->item_type)
	{
		case WEAPON:
		weapon = item;
		break;

		case ARMOUR:
		armour = item;
		break;

		default:
		cout << "\n" << item->name << " cannot be equipped.\n";
		return false;
	}
		
	cout << "\nYou equip " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Player::UnEquip(const vector<string>& args)
{
	if(!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if(item == NULL)
	{
		cout << "\n" << item->name << " is not in your inventory.\n";
		return false;
	}

	if(item == weapon)
		weapon = NULL;
	else if(item == armour)
		armour = NULL;
	else
	{
		cout << "\n" << item->name << " is not equipped.\n";
		return false;
	}

	cout << "\nYou un-equip " << item->name << "...\n";

	return true;
}

// ----------------------------------------------------
bool Player::Examine(const vector<string>& args) const
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);
	// Npc could be examined
	Npc *npc = (Npc*)parent->Find(args[1], NPC);

	if (target == NULL && npc == NULL)
	{
		cout << "\n" << args[1] << " is not here.\n";
		return false;
	}
	else if (npc == NULL)
	{
		target->Inventory();
		target->Stats();
		return true;
	}
	npc->Stats();

	return true;
}

// ----------------------------------------------------
bool Player::Attack(const vector<string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);
	Npc *target2 = (Npc*)parent->Find(args[1], NPC);

	if(target == NULL && target2 == NULL)
	{
		cout << "\n" << args[1] << " is not here.";
		return false;
	}
	if (target2 == NULL)
	{
		combat_target = target;
		cout << "\nYou jump to attack " << target->name << "!\n";
		return true;
	}
	else
	{
		combat_target = target2;
		cout << "\nYou jump to attack " << target2->name << "!\n";
		return true;
	}
}

// ----------------------------------------------------
bool Player::Loot(const vector<string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if(target == NULL)
	{
		cout << "\n" << args[1] << " is not here.\n";
		return false;
	}

	if(target->IsAlive() == true)
	{
		cout << "\n" << target->name << " cannot be looted until it is killed.\n";
		return false;
	}

	list<Entity*> items;
	target->FindAll(ITEM, items);

	if(items.size() > 0)
	{
		cout << "\nYou loot " << target->name << "'s corpse:\n";

		for(list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
		{
			Item* i = (Item*)(*it);
			cout << "You find: " << i->name << "\n";
			i->ChangeParentTo(this);
		}
	}
	else
		cout << "\nYou loot " << target->name << "'s corpse, but find nothing there.\n";
	if (target->gold > 0)
	{
		gold = gold + target->gold;
		cout << "\nYou earn " << target->gold << " gold.\n";
		target->gold = 0;
	}

	return true;
}

// ----------------------------------------------------
bool Player::Lock(const vector<string>& args)
{
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->locked == true)
	{
		cout << "\nThat exit is already locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL)
	{
		cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if(exit->key != item)
	{
		cout << "\nItem '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	cout << "\nYou lock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = true;

	return true;
}

// ----------------------------------------------------
bool Player::UnLock(const vector<string>& args)
{
	if(!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if(exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if(exit->locked == false)
	{
		cout << "\nThat exit is not locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if(item == NULL)
	{
		cout << "\nKey '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if(exit->key != item)
	{
		cout << "\nKey '" << item->name << "' is not the key for " << exit->GetNameFrom((Room*)parent) << ".\n";
		return false;
	}

	cout << "\nYou unlock " << exit->GetNameFrom((Room*)parent) << "...\n";

	exit->locked = false;

	return true;
}

// Function to use objects
bool Player::Use(const vector<string>& args)
{
	Item* item = (Item*)Find(args[1], ITEM);

	if (item == NULL)
	{
		cout << "\nItem not found at your inventory.\n";
		return false;
	}

	switch (item->item_type)
	{
		//Use heal objects
		case HEAL:
		if (hit_points == max_hp)
		{
			cout << "\nYou have max hit points! Cant use this item.\n";
			return false;
		}
		else
		{
			int actual_hp = hit_points;
			// Just can heal to max_hp
			hit_points = min(hit_points + item->max_value, max_hp);

			item->ChangeParentTo(NULL);
			
			cout << "\nHit_points healed from " << actual_hp << " to " << hit_points << ".\n";
			break;
		}
	}
	cout << "\nYou consume item " << item->name << ".\n";
	return true;
}

// function to talk with npcs
bool Player::Talk(const vector<string>& args)
{
	Npc *target = (Npc*)parent->Find(args[1], NPC);

	if (target == NULL)
	{
		cout << "\n" << args[1] <<" not exist or is not a npc.\n";
		return false;
	}
	list<Entity*> items;
	target->FindAll(ITEM, items);
	switch(target->clas)
	{
		// If npc is merchant, should give you a sword in first time talk and show inventory
		case MERCHANT:
		if (items.size() > 0)
		{
			for (list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
			{
				Item* i = (Item*)(*it);
				if (Same("sword", i->name))
				{
					cout << "\nI see you are lost here, take this";
					i->ChangeParentTo(this);
					cout << "\n(Sword is added to your inventory!)\n";
				}
			}
		}
		cout << "\n" << target->dialog << "\n";
		target->Inventory();
		break;

		// Si el npc no es merchant, solo se muestra el dialogo
		default:
		cout << "\n" << target->dialog << "\n";
		return true;
	}
	return true;
}

// Function to buy from NPCs
bool Player::Buy(const vector<string>& args)
{
	Npc *target = (Npc*)parent->Find(args[1], NPC);

	if (target == NULL)
	{
		cout << "\nNPC " << target->name << " is not in this room\n";
		return false;
	}

	list<Entity*> items;
	target->FindAll(ITEM, items);

	switch(target->clas)
	{
		// NPC should be a merchant
		case MERCHANT:
		if (items.size() > 0)
		{
			for (list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
			{
				Item* i = (Item*)(*it);
				if (Same(args[2], i->name))
				{
					if (gold >= i->cost)
					{
						i->ChangeParentTo(this);
						gold -= i->cost;
						cout << "\nYou buy " << i->name << " from " << target->name << " for " << i->cost << " gold!\n";
					}
					else
						cout << "\nYou don't have enough gold for buy " << i->name << "!\n";
				}
			}
		}
		break;

		// If he is not merchant, you cant buy
		default:
			cout << "\nNPC " << target->name << " is not a merchant";
			return false;
	}
	return true;
}
