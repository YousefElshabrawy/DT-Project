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
	Queue<Cook*> VIP_Cooks;
	Queue<Cook*> Vegan_Cooks;
	Queue<Cook*> Normal_Cooks;
	PiriorityQueue<Order*> VIP_Orders;
	Queue<Order*> Vegan_Orders;
	//Queue<Order*> Normal_Orders;
	ListADT<Order*>Normal_Orders;
	ListADT<Order*>In_Service_List;
	ListADT<Order*>finished_List;
	/// ===================    Inputs from file   ================= 

	int Normal_C, Vegan_C, VIP_C; //For Number of cooks of each type
	int SN, SG, SV; //Normal cook speed, Vegan cook speed and VIP cook speed
	int BO, BN, BG, BV; 
	/*
	BO: the number of orders a cook must prepare before taking a break
	BN: the break duration (in timesteps) for normal cooks
	BG: the break duration for vegan ones
	BV: the break duration for VIP cooks.
	*/
	int AutoP; //that represent the number of timesteps after which an order is automatically promoted to VIP.
	int M; //Number of events 
	int ExMony; //the extra money the customer paid for promotion



	/// ============================================================ 



	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	Queue<Order*> DEMO_Queue;	//Important: This is just for demo

	/// ==>
	
	
	
	//
	// TODO: Add More Data Members As Needed
	//

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
/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 

	void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue
	void AddtoVIPQueue(Order* po,int Pir);
	void AddtoNormalQueue(Order* po);
	void AddtoVeganQueue(Order* po);
	void AddtoInServiceList(Order* po);
	void ADDtoFinishedList(Order* po);
	void AddtoUnavailable_Cooks(Cook* CK);


/// ===================    Interactive mode-related functions.    ================= 

	void Interactive_mode();	
	void Simple_Simulator();
	void Step_By_Step_mode();




};

#endif