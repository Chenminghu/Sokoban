#include "Sokoban.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <conio.h>
using std::cout;
using std::endl;
Sokoban::Sokoban() {
	dir = -1;
	Succeed = Prove = 0;
	memset(GameMap, '.', sizeof(GameMap));
	Empty = '.';
	People = 'p';
	Box = '#';
	Block = '*';
	Target = 'T';

	dx[0] = -1; dx[1] = 0; dx[2] = 1; dx[3] = 0;
	dy[0] = 0; dy[1] = 1; dy[2] = 0; dy[3] = -1;

	srand(time(0));
}

//Initial Map
void Sokoban::Initial() {
	int count = 0, x, y;
	//Produce 25 block inside map
	while (count != 25) {
		x = rand() % H;
		y = rand() % L;
		if (GameMap[x][y] == Empty) {
			GameMap[x][y] = Block;
			count++;
		}
	}
	//Produce people at random poisiton
	while (true) {
		x = rand() % H;
		y = rand() % L;
		if (GameMap[x][y] == Empty) {
			GameMap[x][y] = People;
			Pex = x;
			Pey = y;
			break;
		}
	}
	//Produce the box
	while (true) {
		x = rand() % H;
		y = rand() % L;
		//Don't let box produce in the edge(Unplayable)
		if (GameMap[x][y] == Empty && x != 0 && y != 0
			&& x != H - 1 && y != L - 1) {
			GameMap[x][y] = Box;
			Boxx = x;
			Boxy = y;
			break;
		}
	}

	//use bfs to test the game is playable or not
	Sokoban::Box_Bfs(Boxx, Boxy, Pex, Pey);

	//if the game is not playable produce the map again
	if (!Prove) {
		memset(GameMap, '.', sizeof(GameMap));
		Sokoban::Initial();
	}
	else //otherwise count the map
		Sokoban::Show();
}