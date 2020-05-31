#pragma once

#include "..\Defs.h"
#include "Order.h"
#include <cstdlib>
#include <stdlib.h> 
#include<time.h> 

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int Break_Duration;
	int Number_Orders_BeforeBreak;
	bool cooking;  //identicate if the cook is available or not
	Order** OrderServing;  //identicate the order that cook is cooking
	int TimeTODeliver;
	int numofServedorders;
	int TimeBackWork;
	int nomoftakenbreaks;
	bool IsInjured;
	int subspeed;
	
	//Utility function for randomization 

	int random(int min, int max);

public:
	Cook();
	Cook(int ID, ORD_TYPE oT, int Max_Speed, int Min_Speed, int B_max, int B_min, int NumberBeforeBreak);
	virtual ~Cook();
	
	int GetID() const;
	ORD_TYPE GetType() const;
	int GetSpeed()const;
	int GetBreakDuration()const;
	int GetNumber_Of_Orders_Before_Break()const;
	bool GetCooking()const;
	Order* GetServingOrder()const;
	int GetTimeTODeliver()const;
	int GetTimeBackWork()const;
	int GetNumOfServedOrders()const;
	int Getnomoftakenbreaks()const;
	bool GetIsInjured()const;

	void setID(int);
	void setType(ORD_TYPE) ;
	void setSpeed(int s);
	void SetBreak_Duration(int BD);
	void SetNumberBeforeBreak(int N);
	void SetCooking(bool assign);
	void SetOrderForServing(Order* ord);
	void SetTimeTODeliver(int time);
	void SetTimeBackWork(int time);

	void ServedOrder();
	void ResetNumOfServedOrders();
	void takebreak();
	void WorkInjury();
	void Recovery();
};
