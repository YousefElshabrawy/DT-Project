#include "Cook.h"
#include <iostream>
using namespace std;

int Cook::random(int min, int max)
{
	static bool Seeding = true;
	if (Seeding)
	{
		srand(time(NULL));
		Seeding = false;
	}
	return min+rand()%((max+1)-min);
}

Cook::Cook()
{
}

Cook::Cook(int ID, ORD_TYPE oT, int Max_Speed, int Min_Speed, int B_max, int B_min, int NumberBeforeBreak)
{
	this->ID = ID;
	type = oT;

	speed = random(Min_Speed, Max_Speed);
	//srand((unsigned int)time(NULL));
	//int randomSpeed;
	//int ranom = rand() % Max_Speed;
	//randomSpeed = (ranom) + Min_Speed;
	//if (randomSpeed> Max_Speed)
	//{
	//	speed = randomSpeed - Min_Speed;
	//}
	//else
	//{
	//	speed = randomSpeed;
	//}
	subspeed = speed;

	Break_Duration = random(B_min, B_max);
	//srand((unsigned int)time(NULL));
	/*int randomBreak;
	ranom = rand() % B_max;
	randomBreak = (ranom) + B_min;
	if (randomBreak > B_max)
	{
		Break_Duration = randomBreak - B_min;
	}
	else
	{
		Break_Duration = randomBreak;
	}*/

	Number_Orders_BeforeBreak = NumberBeforeBreak;
	cooking = false;
	TimeTODeliver = 0;
	OrderServing = nullptr;
	numofServedorders = 0;
	IsInjured = false;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}

int Cook::GetSpeed() const
{
	return speed;
}

int Cook::GetBreakDuration() const
{
	return Break_Duration;
}

int Cook::GetNumber_Of_Orders_Before_Break() const
{
	return Number_Orders_BeforeBreak;
}

bool Cook::GetCooking() const
{
	return cooking;
}

Order* Cook::GetServingOrder() const
{
	return *OrderServing;
}

int Cook::GetTimeTODeliver() const
{
	return TimeTODeliver;
}

int Cook::GetTimeBackWork() const
{
	return TimeBackWork;
}

int Cook::GetNumOfServedOrders() const
{
	return numofServedorders;
}

int Cook::Getnomoftakenbreaks() const
{
	return nomoftakenbreaks;
}

bool Cook::GetIsInjured() const
{
	return IsInjured;
}

void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setSpeed(int s)
{
	speed = s;
}

void Cook::SetBreak_Duration(int BD)
{
	Break_Duration = BD;
}

void Cook::SetNumberBeforeBreak(int N)
{
	Number_Orders_BeforeBreak = N;
}

void Cook::SetCooking(bool assign)
{
	cooking = assign;
}

void Cook::SetOrderForServing(Order* ord)
{
	OrderServing = new Order*;
	*OrderServing = ord;
}

void Cook::SetTimeTODeliver(int time)
{
	TimeTODeliver = time;
}

void Cook::SetTimeBackWork(int time)
{
	TimeBackWork = time;
}

void Cook::ServedOrder()
{
	numofServedorders++;
}

void Cook::ResetNumOfServedOrders()
{
	numofServedorders = 0;
}

void Cook::takebreak()
{
	nomoftakenbreaks++;
}

void Cook::WorkInjury()
{
	IsInjured = true;
	speed /= 2;
	if (speed<1)
	{
		speed = 1;
	}
}

void Cook::Recovery()
{
	IsInjured = false;
	speed = subspeed;
}



