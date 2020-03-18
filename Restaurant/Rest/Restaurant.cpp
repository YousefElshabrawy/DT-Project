#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"

#include <istream>
#include <ostream>
#include <fstream>
Restaurant::Restaurant() 
{
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
		
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		break;
	case MODE_STEP:
		break;
	case MODE_SLNT:
		break;
	case MODE_DEMO:
		Just_A_Demo();

	};

}

void Restaurant::ReadInputs()
{
	ifstream InputFile;
	InputFile.open("Input.txt");
	

	int Normal_C, Vegan_C, VIP_C; //For Number of cooks of each type
	InputFile >> Normal_C >> Vegan_C >> VIP_C;

	int SN, SG, SV; //Normal cook speed, Vegan cook speed and VIP cook speed
	InputFile >> SN >> SG >> SV;

	int BO, BN, BG, BV; 
	/*
	BO: the number of orders a cook must prepare before taking a break
	BN: the break duration (in timesteps) for normal cooks
	BG: the break duration for vegan ones
	BV: the break duration for VIP cooks.
	*/

	InputFile >> BO >> BN >> BG >> BV;

	//Creating the cooks :
	int CookID = 0;


	//VIP Cooks :

	for (int i = 0; i < VIP_C; i++)
	{
		Cook* Cook_ptr = new Cook(CookID++, TYPE_VIP, SV, BV, BO);
		VIP_Cooks.enqueue(Cook_ptr);
	}

	//Normal Cooks

	for (int i = 0; i < Normal_C; i++)
	{
		Cook* Cook_ptr = new Cook(CookID++, TYPE_NRM, SV, BV, BO);
		VIP_Cooks.enqueue(Cook_ptr);
	}

	//VEGAN Cooks :

	for (int i = 0; i < Vegan_C; i++)
	{
		Cook* Cook_ptr = new Cook(CookID++, TYPE_VGAN, SV, BV, BO);
		VIP_Cooks.enqueue(Cook_ptr);
	}

	///////////////////////////////////////////////////////////////////////////////

	int AutoP; //that represent the number of timesteps after which an order is automatically promoted to VIP.
	InputFile >> AutoP;

	///////////////////////////////////////////////////////////////////////////////

	int M; //Number of events 

	// Start of loop of number of events
	for (int i = 0; i < M; i++)
	{
		char EVENT, TYP;
		int TS; //Type step
		int ID; //is a unique sequence number that identifies each order.
		int SIZE; //is the number of dishes of the order
		int MONY; //is the total order money.
		InputFile >> EVENT;
		switch (EVENT)
		{
		case'R' :
			////////////////////////////////////////////
				InputFile >> TYP >> TS >> ID >> SIZE >> MONY;
				float Equation = MONY + (1000 / TS) + SIZE;
				switch (TYP)
				{

				case 'N':

					Event* pE = new ArrivalEvent(TS, ID, TYPE_NRM, Equation);
					EventsQueue.enqueue(pE);

					break;


				case 'V':

					Event* pE = new ArrivalEvent(TS, ID, TYPE_VIP, Equation);
					EventsQueue.enqueue(pE);

					break;


				case 'G':

					Event* pE = new ArrivalEvent(TS, ID, TYPE_VGAN, Equation);
					EventsQueue.enqueue(pE);

					break;
	

				default:
					break;
				}
			

			break;

			///////////////////////////////////////////////////

		case 'X':
			//TODO
			//Cancelation Pointer will be created and reading It's parameters
			break;

			////////////////////////////////////////////////////////////////////

		case 'P':
			//TODO
			//Promotion poiter is to be created and reading it's parameters

			break;
			//////////////////////////////////////////////////////////////

		default:
			break;
		}


	} 
	//End of loop of number of events .....

	/////////////////////////////////////////////////////////////////////////////////
}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
		if (pGUI)
			delete pGUI;
}

void Restaurant::FillDrawingList()
{
	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);

}




//////////////////////////////////////////////////////////////////////////////////////////////
/// ==> 
///  DEMO-related functions. Should be removed in phases 1&2

//Begin of DEMO-related functions

//This is just a demo function for project introductory phase
//It should be removed starting phase 1
void Restaurant::Just_A_Demo()
{
	
	//
	// THIS IS JUST A DEMO FUNCTION
	// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2
	
	int EventCnt;	
	Order* pOrd;
	Event* pEv;
	srand(time(NULL));

	pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
	EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer

	pGUI->PrintMessage("Generating Events randomly... In next phases, Events should be loaded from a file...CLICK to continue");
	pGUI->waitForClick();
		
	//Just for sake of demo, generate some cooks and add them to the drawing list
	//In next phases, Cooks info should be loaded from input file
	int C_count = 12;	
	Cook *pC = new Cook[C_count];
	int cID = 1;

	for(int i=0; i<C_count; i++)
	{
		cID+= (rand()%15+1);	
		pC[i].setID(cID);
		pC[i].setType((ORD_TYPE)(rand()%TYPE_CNT));
	}	

		
	int EvTime = 0;

	int O_id = 1;
	
	//Create Random events and fill them into EventsQueue
	//All generated event will be "ArrivalEvents" for the demo
	for(int i=0; i<EventCnt; i++)
	{
		O_id += (rand()%4+1);		
		int OType = rand()%TYPE_CNT;	//Randomize order type		
		EvTime += (rand()%5+1);			//Randomize event time
		pEv = new ArrivalEvent(EvTime,O_id,(ORD_TYPE)OType);
		EventsQueue.enqueue(pEv);

	}	

	// --->   In next phases, no random generation is done
	// --->       EventsQueue should be filled from actual events loaded from input file

	
	
	
	
	//Now We have filled EventsQueue (randomly)
	int CurrentTimeStep = 1;
	

	//as long as events queue is not empty yet
	while(!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep,timestep,10);	
		pGUI->PrintMessage(timestep);


		//The next line may add new orders to the DEMO_Queue
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		

/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks
		
		//Let's add ALL randomly generated Cooks to GUI::DrawingList
		for(int i=0; i<C_count; i++)
			pGUI->AddToDrawingList(&pC[i]);
		
		//Let's add ALL randomly generated Ordes to GUI::DrawingList
		int size = 0;
		Order** Demo_Orders_Array = DEMO_Queue.toArray(size);
		
		for(int i=0; i<size; i++)
		{
			pOrd = Demo_Orders_Array[i];
			pGUI->AddToDrawingList(pOrd);
		}
/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	delete []pC;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

	
}
////////////////

void Restaurant::AddtoDemoQueue(Order *pOrd)
{
	DEMO_Queue.enqueue(pOrd);
}

void Restaurant::AddtoVIPQueue(Order* po, int Pir)
{
	VIP_Orders.enqueue(po, Pir);
}

void Restaurant::AddtoNormalQueue(Order* po, int Pir)
{
	Normal_Orders.enqueue(po, Pir);
}

void Restaurant::AddtoVeganQueue(Order* po, int Pir)
{
	Vegan_Orders.enqueue(po, Pir);
}

/// ==> end of DEMO-related function
//////////////////////////////////////////////////////////////////////////////////////////////


