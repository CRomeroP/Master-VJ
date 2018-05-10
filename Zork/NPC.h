#ifndef __Npc__
#define __Npc__

#include "creature.h"

enum NPCType
{
	MERCHANT,
	CITIZEN
};

class Npc : public Creature
{
public:
	Npc(const char* name, const char* description,NPCType clase, Room* room);
	~Npc();

	void Inventory() const;
	void Stats() const;

public:
	NPCType clas;
};

#endif

