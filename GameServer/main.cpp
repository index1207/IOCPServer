#include "pch.h"
#include "CorePch.h"
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"
#include "TypeCast.h"

class Animal
{

};

class Dog : public Animal
{

};

class Cat : public Animal
{

};

int main()
{
	Dog* d = checked_cast<Dog*>(new Animal());

	return 0;
}