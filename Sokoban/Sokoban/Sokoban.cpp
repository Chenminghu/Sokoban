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

void Sokoban::Box_Bfs(int bx, int by, int px, int py)
{
	queue<node>Box;
	int visit[H][L][H][L];
	//means from point[H][L] to point [H][L]
	//FOR Example visit[a][b][c][d] from point(a,b) to point(c,d)

	memset(visit, 0, sizeof(visit));
	s.bx = bx;
	s.by = by;
	s.px = px;
	s.py = py;
	Box.push(s);
	int pe_x, pe_y;
	while (!Box.empty()) {
		s = Box.front();
		Box.pop();

		if (GameMap[s.bx][s.by] == Target)  //arrive the destination
		{
			Prove = 1;
			break;
		}
		for (int i = 0; i < 4; i++)
		{
			e.bx = s.bx + dx[i];  e.by = s.by + dy[i];

			switch (i) //position people push box
			{
			case 0:  pe_x = s.bx + dx[2]; pe_y = s.by + dy[2]; break;
			case 1:  pe_x = s.bx + dx[3]; pe_y = s.by + dy[3]; break;
			case 2:  pe_x = s.bx + dx[0]; pe_y = s.by + dy[0]; break;
			case 3:  pe_x = s.bx + dx[1]; pe_y = s.by + dy[1]; break;
			}

			//make sure box and people don't overlap with block
			if (!Check(e.bx, e.by) || !Check(pe_x, pe_y)
				|| GameMap[e.bx][e.by] == Block || GameMap[pe_x][pe_y] == Block
				|| visit[s.bx][s.by][e.bx][e.by])
				continue;

			
			if (Sokoban::People_Bfs(pe_x, pe_y))
			{
				//storethe position
				e.px = pe_x;  e.py = pe_y;
				Box.push(e);
				visit[s.bx][s.by][e.bx][e.by] = 1; //箱子路劲的标记
			}
		}
	}
}

//use bfs test if people can push the box
bool Sokoban::People_Bfs(int ex, int ey)
{
	queue<node>People;
	node t, end;
	//assume 0 means didn't pass 1 means passed
	int visit[H][L];
	
	memset(visit, 0, sizeof(visit));
	t.px = s.px;  t.py = s.py;  
	People.push(t);
	visit[t.px][t.py] = 1;

	while (!People.empty()) 
	{
		t = People.front();
		People.pop();
		if (t.px == ex && t.py == ey)  //People can arrive box position
			return 1;
		for (int i = 0; i < 4; i++)
		{
			end.px = t.px + dx[i];  end.py = t.py + dy[i];
			
			if (!Check(end.px, end.py) || GameMap[end.px][end.py] == Block
				|| GameMap[end.px][end.py] == Box || visit[end.px][end.py])
				continue;
			
			People.push(end);
			visit[end.px][end.py] = 1;
		}
	}
	return 0;
}

//Refresh Map
void Sokoban::Show()
{
	int i, j;
	while (true)
	{
		//refresh every half second
		clock_t  s = clock();
		while (clock() - s < CLOCKS_PER_SEC / 2)
			;
		//determine the move
		Sokoban::Button();
		Sokoban::Move();
		system("cls");
		for (i = 0; i < H; i++)
		{
			for (j = 0; j < L; j++)
				cout << GameMap[i][j];
			cout << endl;
		}
		cout << endl;

	
		if (Succeed)
		{
			cout << "Congraducation playmore?" << endl;
			getchar();
			break;
		}
	}
}


void Sokoban::Button()
{
	int key;
	if (_kbhit() != 0) //record input otherwise -
	{
		while (_kbhit() != 0)  //record all input and take last one
			key = _getch(); //store input
		switch (key)
		{
			//up
		case 72:  dir = 0;
			break;
			//right
		case 77:  dir = 1;
			break;
			//down
		case 80:  dir = 2;
			break;
			//left
		case 75:  dir = 3;
			break;
		}
	}
}


void Sokoban::Move()
{
	int x, y;
	//has input
	if (dir != -1)
	{
		//change location
		x = Pex + dx[dir];  y = Pey + dy[dir];
		
		if (Check(x, y) && GameMap[x][y] == '.')
		{
			GameMap[Pex][Pey] = '.';  //people position change
			GameMap[x][y] = 'P';
			Pex = x;  Pey = y;
			dir = -1;  //按键记录为无即-1
		}
		else //if people on the position of box, box go ahead one position with same direction
			if (Check(x, y) && GameMap[x][y] == '#'
				&& Check(x + dx[dir], y + dy[dir])
				&& GameMap[x + dx[dir]][y + dy[dir]] == '.')
			{
				GameMap[Boxx][Boxy] = '.';  //box position change
				GameMap[x + dx[dir]][y + dy[dir]] = '#';
				Boxx = x + dx[dir];  Boxy = y + dy[dir];

				GameMap[Pex][Pey] = '.';  //people position change
				GameMap[x][y] = 'P';
				Pex = x;  Pey = y;
				dir = -1;
			}
			else  
				if (Check(x, y) && GameMap[x][y] == '#'
					&& Check(x + dx[dir], y + dy[dir])
					&& GameMap[x + dx[dir]][y + dy[dir]] == 'T')
				{
					GameMap[Boxx][Boxy] = '.';  //box position changes
					GameMap[x + dx[dir]][y + dy[dir]] = '#';
					Boxx = x + dx[dir];  Boxy = y + dy[dir];

					GameMap[Pex][Pey] = '.';  //People position changes
					GameMap[x][y] = 'P';
					Pex = x;  Pey = y;
					dir = -1;
					Succeed = 1;  //Successfully arrive
				}
	}
}

//Check if it is out of map
bool Sokoban::Check(int x, int y)
{
	if (x < 0 || x >= H || y < 0 || y >= L)
		return 0;
	else
		return 1;
}