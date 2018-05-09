#ifndef __World__
#define __World__

#include <string>
#include <list>
#include <time.h>
#include <algorithm>

#define TICK_FREQUENCY 0.5f

using namespace std;

class Entity;
class Player;
class Room;
class Exit;

class World
{
public :

	World();
	~World();

	bool Tick(vector<string>& args);
	bool ParseCommand(vector<string>& args);
	void GameLoop();
	void CreateNewRoom();

private:

	clock_t tick_timer;
	list<Entity*> entities;
	Player* player;
	Room* SRoom;
	Exit* open;
};

#endif //__World__