#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "globals.h"
#include "world.h"

using namespace std;


// -------------------------------------------------
int main()
{
	char key;
	string player_input;
	vector<string> args;
	args.reserve(10);

	cout << "Welcome to KingKiller!\n";
	cout << "----------------\n";

	World my_world;

	args.push_back("look");

	while(1)
	{
		if(_kbhit() != 0)
		{
			key = _getch();
			if(key == '\b') // backspace
			{
				if(player_input.length() > 0)
				{
					player_input.pop_back();
					//cout << BACKSPACE;
					cout << '\b';
					cout << " ";
					cout << '\b';
				}
			}
			else if(key != '\r') // return
			{
				player_input += key;
				cout << key;
			}
			else
				Tokenize(player_input, args);
		}

		if(args.size() > 0 && Same(args[0], "quit"))
			break;

		if(my_world.Tick(args) == false)
			cout << "\nSorry, I do not understand your command.\n";

		if(args.size() > 0)
		{
			args.clear();
			player_input = "";
			cout << "> ";
		}
		// When the king is defeated, end the game.
		if (my_world.GameEnd() == 1) 
		{
			cout << "\nYou defeat the king! Congratulations for your achivement!\n You reach the end of the game";
			break;
		}
		// End the game when player dies
		else if (my_world.GameEnd() == 2)
		{
			cout << "\nYou lost the game.\nYou have been defeated and the king stays alive. His reign will last and he will be stronger than anyone";
			break;
		}

	}

	cout << "\nThanks for playing the game, hope you enjoy it!\n";
	cin.ignore();
	return 0;
}