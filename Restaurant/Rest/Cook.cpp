#include "Cook.h"


Cook::Cook()
{
}

Cook::Cook(int ID, ORD_TYPE oT, int Speed, int BreakDuration, int NumberBeforeBreak)
{
	this->ID = ID;
	type = oT;
	speed = Speed;
	Break_Duration = BreakDuration;
	Number_Orders_BeforeBreak = NumberBeforeBreak;
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


