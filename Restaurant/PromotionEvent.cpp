#include "PromotionEvent.h"
#include "Rest/Order.h"
#include "Rest/Restaurant.h"


PromotionEvent::PromotionEvent(int oTime, int oID) :Event(oTime, oID)
{
	this->oID = oID;

}

PromotionEvent::PromotionEvent(int oTime, int oID, double Money) :Event(oTime, oID)
{
	this->oID = oID;
	ExMONY = Money;
}

void PromotionEvent::Execute(Restaurant* pRest)
{
	Order* ORD = pRest->GetNormalOrderByID(oID);
	if (ORD)
	{
		ORD->AddExtraMoney(ExMONY);
		ORD->SetType(TYPE_VIP);
		int Equation = ORD->GettotalMoney() + (1000 / (float)ORD->GetArrTime()) + ORD->GetSize();
		pRest->AddtoVIPQueue(ORD, -1); // -1 Is an indication that it is added to the end of the queue as the priorties are all postitive	
	}
}
