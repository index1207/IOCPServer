#include "pch.h"
#include <iostream>

using TL = TypeList<class Player, class Knight, class Archer>;

class Player
{
public:
	CASTABLE_CLASS;
public:
	Player() { INIT_ID(Player); }
};

class Knight : public Player
{
public:
	Knight() { INIT_ID(Knight); }
};

class Archer : public Player
{
public:
	Archer() { INIT_ID(Archer); }
};

int main()
{
	Player* p = new Knight();
	Knight* k = checked_cast<Knight*>(p);
}