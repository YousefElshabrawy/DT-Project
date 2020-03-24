#pragma once
#include "Events/Event.h"
class Restaurant; //Forward Delclearation :
class CancelEvent :
	public Event
{
	int oID;
	
public:
	CancelEvent(int eTime, int ordID);
	virtual void Execute(Restaurant* pRest);	//override execute function
};

