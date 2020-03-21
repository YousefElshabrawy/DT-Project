#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


//Constructors 
ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType):Event(eTime, oID)
{
	OrdType = oType;
}
ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, float Piriorty) : Event(eTime, oID)
{
	OrdType = oType;
	this->Piriorty_Of_Order = Piriorty;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1

	
	
	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phases 1&2
	//pRest->AddtoDemoQueue(pOrd);

	Order* pOrd = new Order(OrderID,OrdType);
	
	switch (OrdType)
	{
	case TYPE_NRM:
		pRest->AddtoNormalQueue(pOrd);
		break;
	case TYPE_VGAN:
		pRest->AddtoVeganQueue(pOrd);
		break;
	case TYPE_VIP:
		pRest->AddtoVIPQueue(pOrd, Piriorty_Of_Order);
		break;
	default:
		break;
	}
}
