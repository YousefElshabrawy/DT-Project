#pragma once

#include "..\Defs.h"

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int Break_Duration;
	int Number_Orders_BeforeBreak;
public:
	Cook();
	Cook(int ID, ORD_TYPE oT, int Speed, int BreakDuration, int NumberBeforeBreak);
	virtual ~Cook();
	
	int GetID() const;
	ORD_TYPE GetType() const;
	int GetSpeed()const;
	int GetBreakDuration()const;
	int GetNumber_Of_Orders_Before_Break()const;

	void setID(int);
	void setType(ORD_TYPE) ;
	void setSpeed(int s);
	void SetBreak_Duration(int BD);
	void SetNumberBeforeBreak(int N);


};
