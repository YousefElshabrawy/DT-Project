#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include "..\PiriortyQueue.h"
#include "..\ListADT.h"
#include "Order.h"
#include "Cook.h"
#include <istream>
#include <ostream>
using namespace std;
// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	bool testing;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	ListADT<Cook*> Unavailable_Cooks;
	ListADT<Cook*> VIP_Cooks;
	ListADT<Cook*> Vegan_Cooks;
	ListADT<Cook*> Normal_Cooks;
	PiriorityQueue<Order*> VIP_Orders;
	Queue<Order*> Vegan_Orders;
	Queue<Order*> Urgent_Orders;
	//Queue<Order*> Normal_Orders;
	ListADT<Order*>Normal_Orders;
	ListADT<Order*>In_Service_List;
	ListADT<Order*>finished_List;
	/// ===================    Inputs from file   ================= 

	int Normal_C, Vegan_C, VIP_C; //For Number of cooks of each type
	int SN_min,SN_max, SG_min,SG_max, SV_min, SV_max; //Normal cook speed, Vegan cook speed and VIP cook speed
	int BO, BN_min, BN_max, BG_min,BG_max , BV_min ,BV_max;
	float InjProp;
	int RstPrd;
	/*
	BO: the number of orders a cook must prepare before taking a break
	BN: the break duration (in timesteps) for normal cooks
	BG: the break duration for vegan ones
	BV: the break duration for VIP cooks.
	*/
	int AutoP,VIP_WT; //that represent the number of timesteps after which an order is automatically promoted to VIP.
	int M; //Number of events 
	int ExMony; //the extra money the customer paid for promotion



	/// ===================    Restaurant outputs   =================
	int NumOfDeliveredVIPOrders;
	int NumOfDeliveredNORMALOrders;
	int NumOfDeliveredVEGANOrders;
	
	
	
	//
	// TODO: Add More Data Members As Needed
	//
	void swap(Order**& Ord1, Order**& Ord2) //For Sorting 
	{
		Order** Temp = Ord1;
		Ord1 = Ord2;
		Ord2 = Temp;
	}

public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();

	void ReadInputs();
	bool GetTesting()const;

	void FillDrawingList();

	//
	// TODO: Add More Member Functions As Needed
	//
	Order* GetNormalOrderByID(int ID);
	void DeleteNormalOrder(Order* order);
/// ===================       ================= 

	void AddtoVIPQueue(Order* po,int Pir);
	void AddtoNormalQueue(Order* po);
	void AddtoVeganQueue(Order* po);
	void AddtoInServiceList(Order* po);
	void ADDtoFinishedList(Order* po);
	void AddtoUnavailable_Cooks(Cook* CK);
	void AddtoUrgentQueue(Order* po);




/// ===================    Interactive mode-related functions.    ================= 

	void Interactive_mode();	
	void Simple_Simulator();
	void Step_By_Step_mode();
	void Silent_Mode();




};

#endif