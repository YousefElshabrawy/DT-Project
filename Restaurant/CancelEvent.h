#pragma once
#include "Events/Event.h"
class CancelEvent :
	public Event
{
	
public:
	CancelEvent(int eTime, int ordID);
	virtual void Execute(Restaurant* pRest);	//override execute function
};

