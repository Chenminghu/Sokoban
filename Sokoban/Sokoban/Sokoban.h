#pragma once
#ifndef SOKOBAN_H_
#define SOKOBAN_H_
#include <queue>
using std::queue;

struct node
{
	int bx, by; //position of box
	int px, py; //position of people
};

class Sokoban {
private:
	enum { L = 15, H = 7};
	char GameMap[H][L];//Generate Map
	int Pex, Pey; //Position of people
	int Boxx, Boxy; //Position of box
	int Succeed, Prove; //Succeed test whether the box arrive destination/Prove test whether the map is playable
	int dx[4], dy[4];
protected:
	char Empty;
	char People;
	char Box;
	char Block;
	char Target;
	int dir;//store the direction box is going to move
	node s, e;
public:
	Sokoban();
	~Sokoban(){}

	void Initial();   //initiate the map
	void Box_Bfs(int bx, int by, int px, int py); //test map is playable by bfs
	bool People_Bfs(int ex, int ey);// test people can arrive destination
	void Show();
	void Button();
	void Move();
	bool Check(int x, int y);
};
#endif