#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"
#include "..\PiriortyQueue.h"


//class for the arrival event
class ArrivalEvent: public Event
{
	//info about the order ralted to arrival event
	int OrdDistance;	//order distance
	ORD_TYPE OrdType;		//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
	float Piriorty_Of_Order;
	int ordarrivaltime;
	int ordersize;
public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType);
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int arrivaltime, int money, int size);
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType, float Piriorty, int arrivaltime, int money, int size); //Added to Set The piriorty of the order
	//Add more constructors if needed
	
	virtual void Execute(Restaurant *pRest);	//override execute function

};

#endif