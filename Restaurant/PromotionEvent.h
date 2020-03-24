#pragma once
#include "Events\Event.h"
class Restaurant; //Forward Declearation
class PromotionEvent :
	public Event
{
	int oID; //Referes to Order ID 
	double ExMONY;	//Paid money for promotion:

public:

	PromotionEvent(int oTime, int oID);
	PromotionEvent(int oTime, int oID,double Money);

	virtual void Execute(Restaurant* pRest);	//override execute function

};

