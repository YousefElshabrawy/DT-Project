#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int Distance;	//The distance (in meters) between the order location and the resturant 
	int size;
	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times
	bool IsUrgent;  //only for VIP Orders
	bool WasNormal;  ////only for Normal Orders

	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	Order(int ID, ORD_TYPE r_Type, int AT, double cost, int sizeoforder);
	virtual ~Order();

	int GetID();
	int GetArrTime();
	int GetFinishTime();
	int GetServTime();
	int GetSize();
	int GetWaitTime();
	bool GetIsUrgent()const;
	void setIsUrgent(bool val);
	bool GetWasNormal();
	void SetWasNormal(bool val);

	void SetFinishTime(int time);
	void SetServTime(int time);

	void SetType(ORD_TYPE newtype);
	ORD_TYPE GetType() const;

	void SetDistance(int d);
	int GetDistance() const;

	void SettotalMoney(double mon);
	double GettotalMoney() const;

	void AddExtraMoney(double mon);

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	
	//
	// TODO: Add More Member Functions As Needed
	//


	//Operator Overloading for The ==
	bool operator ==(int Value);

};

#endif