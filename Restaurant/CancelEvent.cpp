#include "CancelEvent.h"
#include "Rest/Order.h"
#include "Rest/Restaurant.h"
CancelEvent::CancelEvent(int eTime, int ordID):Event(eTime,ordID)
{
	oID = ordID;
}

void CancelEvent::Execute(Restaurant* pRest)
{
	Order* ORD = pRest->GetNormalOrderByID(oID);
	delete ORD;
	ORD = nullptr;
}
