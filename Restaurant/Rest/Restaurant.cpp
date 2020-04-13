#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\PromotionEvent.h"
#include "..\CancelEvent.h"
#include <istream>
#include <ostream>
#include <fstream>
Restaurant::Restaurant()
{
	pGUI = NULL;
	testing = false;
	NumOfDeliveredVIPOrders = 0;
	NumOfDeliveredNORMALOrders = 0;
	NumOfDeliveredVEGANOrders = 0;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	//The mode of displaying outputs
	{


	case MODE_INTR: //Interactive mode

		Interactive_mode();

		break;


	case MODE_STEP: //Step by step mode 

		Step_By_Step_mode();

		break;


	case MODE_SLNT: //Silent Mode

		break;


	case MODE_DEMO: //Demo mode

		Simple_Simulator();

		break;

	};

}

//-----------------------------------------------------------------------------------------------------//
//--------------------------Reading the inputs from textfile-------------------------------------------//
void Restaurant::ReadInputs()
{
	
	ifstream InputFile;
	do {
		pGUI->PrintMessage("Enter the file name without .txt :");
		string FileName = pGUI->GetString();

		FileName = FileName + ".txt";

		InputFile.open(FileName); //Opening the file (Notice that the file name is step by step "May be changed in next phases")

		if (InputFile.is_open() == false)
		{
			pGUI->PrintMessage("The file name is not exsist , please try again, Please press anywhere to continue");
			pGUI->waitForClick();
			pGUI->PrintMessage(" ");
		}

		} while (InputFile.is_open() == false);

		pGUI->PrintMessage("File Is Opened Successfully! , Please press anywhere to continue");
		pGUI->waitForClick();
		pGUI->PrintMessage(" ");

	//------------------------------------> COOKS <--------------------------------------------//

	// i - Reading the cooks parameters (Inputs)

	InputFile >> Normal_C >> Vegan_C >> VIP_C; //Number of Normal Vegan and VIP Cooks

	InputFile >> SN >> SG >> SV;  //Speed of Normal , Vegan , and VIP Cooks

	InputFile >> BO >> BN >> BG >> BV;
	/*
	 BO: the number of orders a cook must prepare before taking a break .
	 BN: the break duration (in timesteps) for normal cooks .
	 BG: the break duration for vegan ones .
	 BV: the break duration for VIP cooks.
	*/


	// ii- Creating the cooks :

	int CookID = 0; //Intially the first cook ID


	//VIP Cooks :

	for (int i = 0; i < VIP_C; i++)
	{
		Cook* Cook_ptr = new Cook(CookID++, TYPE_VIP, SV, BV, BO);
		VIP_Cooks.pushEnd(Cook_ptr);
	}

	//Normal Cooks

	for (int i = 0; i < Normal_C; i++)
	{
		Cook* Cook_ptr = new Cook(CookID++, TYPE_NRM, SN, BN, BO);
		Normal_Cooks.pushEnd(Cook_ptr);
	}

	//VEGAN Cooks :

	for (int i = 0; i < Vegan_C; i++)
	{
		Cook* Cook_ptr = new Cook(CookID++, TYPE_VGAN, SG, BG, BO);
		Vegan_Cooks.pushEnd(Cook_ptr);
	}

	///////////////////////////////////////////////////////////////////////////////

	InputFile >> AutoP; //Represent the number of timesteps after which an order is automatically promoted to VIP.

	///////////////////////////////////////////////////////////////////////////////

	//----------------------------> EVENTS <----------------------------------//

	InputFile >> M; //Represents the number of events

	//The following lines of code Reading events parameners and create the event and push then into the Queue of events
	// Start of loop of number of events
	for (int i = 0; i < M; i++)
	{
		//Variables Decleration that will be used

		char EVENT, TYP;
		int TS; //Type step
		int ID; //is a unique sequence number that identifies each order.
		int SIZE; //is the number of dishes of the order
		int MONY; //is the total order money.


		InputFile >> EVENT; //Reading the Event Type (VIP,Normal,Vegan)

		float Equation = 0; //Priorty equation for VIP orders

		switch (EVENT) //To check the Type of Event
		{

		case'R': //Arraival Event 

			////////////////////////////////////////////

			/*

			i- TYP is the order type (N: normal, G: vegan, V: VIP).
			ii- TS is the event timestep. (order arrival time) .
			iii- ID is a unique sequence number that identifies each order.
			iv- SIZE is the number of dishes of the order .
			v-  MONY is the total order money.

			*/

			InputFile >> TYP >> TS >> ID >> SIZE >> MONY; //Reading the inputs


			Event* pE;
			switch (TYP)
			{

			case 'N':

				pE = new ArrivalEvent(TS, ID, TYPE_NRM, TS, MONY, SIZE);
				//pE = new ArrivalEvent(TS, ID, 0, TS, MONY, SIZE);
				EventsQueue.enqueue(pE);
				pE = NULL;
				break;


			case 'V':

				Equation = MONY + (1000 / (float)TS) + SIZE; //The priorty equation of Piriorty VIP Queue
				pE = new ArrivalEvent(TS, ID, TYPE_VIP, Equation, TS, MONY, SIZE);
				EventsQueue.enqueue(pE);
				pE = NULL;

				break;


			case 'G':

				pE = new ArrivalEvent(TS, ID, TYPE_VGAN, TS, MONY, SIZE);
				EventsQueue.enqueue(pE);
				pE = NULL;

				break;


			default:
				break;
			} //End of Type of Order in Arrival Event

			break;



			///////////////////////////////////////////////////


		case 'X': //Cancelation Event

			InputFile >> TS >> ID; //the event timestep and the id of the order to be canceled. This ID must be of a Normal order.(Validation!)
			pE = new CancelEvent(TS, ID);
			EventsQueue.enqueue(pE);
			pE = NULL;

			break;

			////////////////////////////////////////////////////////////////////

		case 'P': //Promotion Event 

			InputFile >> TS >> ID >> ExMony; //the event timestep and the id of the order to be promoted to VIP(MUST be Normal) , Extra money to be paid
			pE = new PromotionEvent(TS, ID, ExMony);
			EventsQueue.enqueue(pE);
			pE = NULL;

			break;
			//////////////////////////////////////////////////////////////

		default:

			break;

		}
		//End of Checking the Event : Arrival Promotion Cancelation

	}
	//End of loop of number of events .....
	/////////////////////////////////////////////////////////////////////////////////
}

bool Restaurant::GetTesting() const
{
	return testing;
}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
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

	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);


	Order* pOrd;

	//-------------------------> COOKS <--------------------------------------

	//Convert to Arrays (Easy to Iterate on them)

	Cook** pCVIP_Array = VIP_Cooks.toArray(VIP_C); //Array of Pointers to VIP Cooks
	Cook** pCNormal_Array = Normal_Cooks.toArray(Normal_C); //Array of Pointers to Normal Cooks
	Cook** pCVegan_Array = Vegan_Cooks.toArray(Vegan_C); ////Array of Pointers to Vegan Cooks

	//add all Cooks to GUI::DrawingList
	for (int i = 0; i < VIP_C; i++)
		pGUI->AddToDrawingList(pCVIP_Array[i]);

	for (int i = 0; i < Normal_C; i++)
		pGUI->AddToDrawingList(pCNormal_Array[i]);

	for (int i = 0; i < Vegan_C; i++)
		pGUI->AddToDrawingList(pCVegan_Array[i]);

	//---------------------------------> Orders <------------------------------------
	//add all Ordes to GUI::DrawingList

	//Getting Numbers of All Orders
	int nomOf_VIP_Orders = VIP_Orders.GetSize();
	int nomOf_Normal_Orders = Normal_Orders.GetSize();
	int nomOf_Vegan_Orders = Vegan_Orders.GetSize();
	int nomOf_IN_Service_Orders = In_Service_List.GetSize();
	int nomOf_Finished_Orders = finished_List.GetSize();

	//make array of orders to add them to GUI
	Order** VIP_Orders_Array = VIP_Orders.ToArray(nomOf_VIP_Orders);
	Order** Normal_Orders_Array = Normal_Orders.toArray(nomOf_Normal_Orders);
	Order** Vegan_Orders_Array = Vegan_Orders.toArray(nomOf_Vegan_Orders);
	Order** IN_Service_Array = In_Service_List.toArray(nomOf_IN_Service_Orders);
	Order** Finished_Array = finished_List.toArray(nomOf_Finished_Orders);

	////////////////////////////////////////////////////////////////////////////////
	//Sorting VIP Orders :  This is by using Selection Sorting 

	int i, j, min_index;

	for (i = 0; i < nomOf_VIP_Orders - 1; i++)
	{
		// Find the minimum element in unsorted array 
		min_index = i;
		for (j = i + 1; j < nomOf_VIP_Orders; j++)
			if (VIP_Orders_Array[j]->GetArrTime() < VIP_Orders_Array[min_index]->GetArrTime())
				min_index = j;

		//Then Swapping
		swap(&VIP_Orders_Array[min_index], &VIP_Orders_Array[i]);

	}

	//End Sorting
	//////////////////////////////////////////////////////////////////////////////////





	////add all Orders to GUI::DrawingList
	for (int i = 0; i < nomOf_VIP_Orders; i++)
	{
		pOrd = VIP_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}

	for (int i = 0; i < nomOf_Normal_Orders; i++)
	{
		pOrd = Normal_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}

	for (int i = 0; i < nomOf_Vegan_Orders; i++)
	{
		pOrd = Vegan_Orders_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}

	for (int i = 0; i < nomOf_IN_Service_Orders; i++)
	{
		pOrd = IN_Service_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}

	for (int i = 0; i < nomOf_Finished_Orders; i++)
	{
		pOrd = Finished_Array[i];
		pGUI->AddToDrawingList(pOrd);
	}
	////////////////////////////////////////////////////////////////////////////////////////////

	//Updating the Interface to draw
	pGUI->UpdateInterface();
	Sleep(1000);
	pGUI->ResetDrawingList();

}


//This Function to return an Order from the List to Use it in the Events
Order* Restaurant::GetNormalOrderByID(int ID)
{
	return Normal_Orders.SearchByID(ID);
}

//To delete Normal Order from the List 
void Restaurant::DeleteNormalOrder(Order* order)
{
	Normal_Orders.DeleteItem(order);
}

//////////////////////////////////////////////////////////////////////////

//Adding to Data Structures
void Restaurant::AddtoVIPQueue(Order* po, int Pir)
{
	VIP_Orders.enqueue(po, Pir);
}
void Restaurant::AddtoNormalQueue(Order* po)
{
	Normal_Orders.pushEnd(po);
}

void Restaurant::AddtoVeganQueue(Order* po)
{
	Vegan_Orders.enqueue(po);
}

void Restaurant::AddtoInServiceList(Order* po)
{
	In_Service_List.pushEnd(po);
}

void Restaurant::ADDtoFinishedList(Order* po)
{
	finished_List.pushEnd(po);
}

void Restaurant::AddtoUnavailable_Cooks(Cook* CK)
{
	Unavailable_Cooks.pushEnd(CK);
}





//////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------> Projects Modes <--------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////


/*
Interactive mode:
allows user to monitor the restaurant operation. At each time step, program should provide output similar to that in the figure.
In this mode, program pauses for a user mouse click to display the output of the next timestep.
*/
void Restaurant::Interactive_mode()
{

	ReadInputs();

	int CurrentTimeStep = 0;

	//as long as events queue is not empty yet
	while (!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);

		//execute all events at current time step
		ExecuteEvents(CurrentTimeStep);


		//add all current ordes & cooks to GUI
		FillDrawingList();



		/////////////////////////////////////////////////////////////////////////////////////////

		Sleep(1000);
		CurrentTimeStep++;	//advance timestep
	}



	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

}


/*
Simple Simulator Is ONLY To Test phase 1 Code
*/
void Restaurant::Simple_Simulator()
{
	testing = true; //To check that it is only testing 

	ReadInputs(); //Reading inputs from file

	int CurrentTimeStep = 0;

	//as long as events queue or in service orders are not empty yet
	while (!EventsQueue.isEmpty() || !In_Service_List.IsEmpty())
	{
		//execute all events at current time step
		//should ignore promotion events 
		ExecuteEvents(CurrentTimeStep);

		//Pick one order from each order type and move it to In-service list(s)
		Order* ORD;
		if (VIP_Orders.peekFront(ORD))
		{
			if (ORD->GetArrTime() != CurrentTimeStep)
			{
				ORD->setStatus(SRV);
				AddtoInServiceList(ORD);
				VIP_Orders.dequeue(ORD);
				ORD = NULL;
			}
		}

		if (Normal_Orders.peekFront(ORD))
		{
			if (ORD->GetArrTime() != CurrentTimeStep)
			{
				ORD->setStatus(SRV);
				AddtoInServiceList(ORD);
				Normal_Orders.DeleteItem(ORD);
				ORD = NULL;
			}
		}

		if (Vegan_Orders.peekFront(ORD))
		{
			if (ORD->GetArrTime() != CurrentTimeStep)
			{
				ORD->setStatus(SRV);
				AddtoInServiceList(ORD);
				Vegan_Orders.dequeue(ORD);
				ORD = NULL;
			}
		}

		//Each 5 timesteps, move an order of each type from In-service list(s) to finished list(s)
		if (CurrentTimeStep % 5 == 0)
		{
			bool finishedNormal = false, finishedVIP = false, finishedVEGAN = false;

			int nomOf_IN_SERVICE_ORDERS = In_Service_List.GetSize();
			Order** In_Service_Array = In_Service_List.toArray(nomOf_IN_SERVICE_ORDERS);

			for (int i = 0; i < nomOf_IN_SERVICE_ORDERS; i++)
			{
				if (!finishedVIP && In_Service_Array[i]->GetType() == TYPE_VIP)
				{
					In_Service_Array[i]->setStatus(DONE);
					ADDtoFinishedList(In_Service_Array[i]);
					In_Service_List.DeleteItem(In_Service_Array[i]);
					finishedVIP = true;
				}
				else if (!finishedNormal && In_Service_Array[i]->GetType() == TYPE_NRM)
				{
					In_Service_Array[i]->setStatus(DONE);
					ADDtoFinishedList(In_Service_Array[i]);
					In_Service_List.DeleteItem(In_Service_Array[i]);
					finishedNormal = true;
				}
				else if (!finishedVEGAN && In_Service_Array[i]->GetType() == TYPE_VGAN)
				{
					In_Service_Array[i]->setStatus(DONE);
					ADDtoFinishedList(In_Service_Array[i]);
					In_Service_List.DeleteItem(In_Service_Array[i]);
					finishedVEGAN = true;
				}
			}
		}

		//print current timestep

		string Message1 = "TS :" + to_string(CurrentTimeStep);
		string AvailableCooks = "Avaiable Cooks ---> Normal = " + to_string(Normal_C) + " , VIP = " + to_string(VIP_C) + " , Vegan = " + to_string(Vegan_C);
		string WaitingOrders = "Waiting Orders ---> Normal = " + to_string(Normal_Orders.GetSize()) + " , VIP = " + to_string(VIP_Orders.GetSize()) + " , Vegan = " + to_string(Vegan_Orders.GetSize());
		pGUI->PrintMessage(Message1, AvailableCooks, WaitingOrders);

		//add all current ordes & cooks to GUI
		FillDrawingList();

		pGUI->waitForClick();
		CurrentTimeStep++;	//advance timestep
	}



	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}

void Restaurant::Step_By_Step_mode()
{
	ReadInputs();

	int CurrentTimeStep = 0;

	//as long as events queue or in service orders are not empty yet
	while (!EventsQueue.isEmpty() || !In_Service_List.IsEmpty())
	{
		//execute all events at current time step
		ExecuteEvents(CurrentTimeStep);

		Order* ORD;
		Cook* COK;

		//check for all cooks one after one if they serviced enough orders to have a break 
		int VIP_num = VIP_Cooks.GetSize();
		Cook** VIP_Cooks_Array = VIP_Cooks.toArray(VIP_num);
		for (int i = 0; i < VIP_num && VIP_Cooks_Array[i]->GetNumOfServedOrders() != 0 && VIP_Cooks_Array[i]->GetNumOfServedOrders() % VIP_Cooks_Array[i]->GetNumber_Of_Orders_Before_Break() == 0; i++)
		{
			VIP_Cooks_Array[i]->SetTimeBackWork(CurrentTimeStep + VIP_Cooks_Array[i]->GetBreakDuration());
			AddtoUnavailable_Cooks(VIP_Cooks_Array[i]);
			VIP_Cooks.DeleteItem(VIP_Cooks_Array[i]);
		}

		int NORMAL_num = Normal_Cooks.GetSize();
		Cook** NORMAL_Cooks_Array = Normal_Cooks.toArray(NORMAL_num);
		for (int i = 0; i < NORMAL_num && NORMAL_Cooks_Array[i]->GetNumOfServedOrders() != 0 && NORMAL_Cooks_Array[i]->GetNumOfServedOrders() % NORMAL_Cooks_Array[i]->GetNumber_Of_Orders_Before_Break() == 0; i++)
		{
			NORMAL_Cooks_Array[i]->SetTimeBackWork(CurrentTimeStep + NORMAL_Cooks_Array[i]->GetBreakDuration());
			AddtoUnavailable_Cooks(NORMAL_Cooks_Array[i]);
			Normal_Cooks.DeleteItem(NORMAL_Cooks_Array[i]);
		}

		int VEGAN_num = Vegan_Cooks.GetSize();
		Cook** VEGAN_Cooks_Array = Vegan_Cooks.toArray(VEGAN_num);
		for (int i = 0; i < VEGAN_num && VEGAN_Cooks_Array[i]->GetNumOfServedOrders() != 0 && VEGAN_Cooks_Array[i]->GetNumOfServedOrders() % VEGAN_Cooks_Array[i]->GetNumber_Of_Orders_Before_Break() == 0; i++)
		{
			VEGAN_Cooks_Array[i]->SetTimeBackWork(CurrentTimeStep + VEGAN_Cooks_Array[i]->GetBreakDuration());
			AddtoUnavailable_Cooks(VEGAN_Cooks_Array[i]);
			Vegan_Cooks.DeleteItem(VEGAN_Cooks_Array[i]);
		}

		int numofunavailblecooks = Unavailable_Cooks.GetSize();
		Cook** Unavailable_Cooks_Array = Unavailable_Cooks.toArray(numofunavailblecooks);
		for (int i = 0; i < numofunavailblecooks; i++)
		{
			if (Unavailable_Cooks_Array[i]->GetTimeBackWork() == CurrentTimeStep)
			{
				if (Unavailable_Cooks_Array[i]->GetType() == TYPE_VIP)
				{
					Unavailable_Cooks_Array[i]->ResetNumOfServedOrders();
					VIP_Cooks.pushEnd(Unavailable_Cooks_Array[i]);
				}
				else if (Unavailable_Cooks_Array[i]->GetType() == TYPE_NRM)
				{
					Unavailable_Cooks_Array[i]->ResetNumOfServedOrders();
					Normal_Cooks.pushEnd(Unavailable_Cooks_Array[i]);
				}
				else
				{
					Unavailable_Cooks_Array[i]->ResetNumOfServedOrders();
					Vegan_Cooks.pushEnd(Unavailable_Cooks_Array[i]);
				}
				Unavailable_Cooks.DeleteItem(Unavailable_Cooks_Array[i]);
			}
		}

		//assign orders to in-service


		while (VIP_Orders.peekFront(ORD) && VIP_Cooks.peekFront(COK))
		{
			ORD->setStatus(SRV);
			AddtoInServiceList(ORD);
			VIP_Orders.dequeue(ORD);

			COK->SetCooking(true);
			COK->SetOrderForServing(ORD);
			COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
			AddtoUnavailable_Cooks(COK);
			VIP_Cooks.DeleteItem(COK);
			ORD = NULL;
			COK = NULL;
		}

		while (Normal_Orders.peekFront(ORD) && Normal_Cooks.peekFront(COK))
		{
			ORD->setStatus(SRV);
			AddtoInServiceList(ORD);
			Normal_Orders.DeleteItem(ORD);

			COK->SetCooking(true);
			COK->SetOrderForServing(ORD);
			COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
			AddtoUnavailable_Cooks(COK);
			Normal_Cooks.DeleteItem(COK);
			ORD = NULL;
			COK = NULL;
		}

		while (Vegan_Orders.peekFront(ORD) && Vegan_Cooks.peekFront(COK))
		{
			ORD->setStatus(SRV);
			AddtoInServiceList(ORD);
			Vegan_Orders.dequeue(ORD);

			COK->SetCooking(true);
			COK->SetOrderForServing(ORD);
			COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
			AddtoUnavailable_Cooks(COK);
			Vegan_Cooks.DeleteItem(COK);
			ORD = NULL;
			COK = NULL;
		}

		//move finished orders from In-service list to finished list
		numofunavailblecooks = Unavailable_Cooks.GetSize();
		Unavailable_Cooks_Array = Unavailable_Cooks.toArray(numofunavailblecooks);
		for (int i = 0; i < numofunavailblecooks; i++)
		{
			if (Unavailable_Cooks_Array[i]->GetCooking())
			{
				if (Unavailable_Cooks_Array[i]->GetTimeTODeliver() == CurrentTimeStep)
				{
					Unavailable_Cooks_Array[i]->GetServingOrder()->setStatus(DONE);
					ADDtoFinishedList(Unavailable_Cooks_Array[i]->GetServingOrder());
					In_Service_List.DeleteItem(Unavailable_Cooks_Array[i]->GetServingOrder());

					Unavailable_Cooks_Array[i]->ServedOrder();
					Unavailable_Cooks_Array[i]->SetCooking(false);
					Unavailable_Cooks_Array[i]->SetOrderForServing(nullptr);
					if (Unavailable_Cooks_Array[i]->GetType() == TYPE_VIP)
					{
						VIP_Cooks.pushEnd(Unavailable_Cooks_Array[i]);
						NumOfDeliveredVIPOrders++;
					}
					else if (Unavailable_Cooks_Array[i]->GetType() == TYPE_NRM)
					{
						Normal_Cooks.pushEnd(Unavailable_Cooks_Array[i]);
						NumOfDeliveredNORMALOrders++;
					}
					else
					{
						Vegan_Cooks.pushEnd(Unavailable_Cooks_Array[i]);
						NumOfDeliveredVEGANOrders++;
					}
					Unavailable_Cooks.DeleteItem(Unavailable_Cooks_Array[i]);
				}
			}
		}

		//print current timestep

		string Message1 = "TS :" + to_string(CurrentTimeStep);
		string AvailableCooks = "Avaiable Cooks ---> Normal = " + to_string(Normal_Cooks.GetSize()) + " , VIP = " + to_string(VIP_Cooks.GetSize()) + " , Vegan = " + to_string(Vegan_Cooks.GetSize());
		string WaitingOrders = "Waiting Orders ---> Normal = " + to_string(Normal_Orders.GetSize()) + " , VIP = " + to_string(VIP_Orders.GetSize()) + " , Vegan = " + to_string(Vegan_Orders.GetSize());
		pGUI->PrintMessage(Message1, AvailableCooks, WaitingOrders);

		//add all current ordes & cooks to GUI
		FillDrawingList();

		pGUI->waitForClick();
		CurrentTimeStep++;	//advance timestep
	}

	Order* ORD;
	cout << "ID\tAT\n";
	while (finished_List.peekFront(ORD))
	{
		cout << ORD->GetID() << "\t" << ORD->GetArrTime() << "\n";
		finished_List.DeleteItem(ORD);
	}
	cout << "Orders: " << NumOfDeliveredVIPOrders + NumOfDeliveredNORMALOrders + NumOfDeliveredVEGANOrders << " [Norm: " << NumOfDeliveredNORMALOrders << ", Veg: " << NumOfDeliveredVEGANOrders << ", VIP: " << NumOfDeliveredVIPOrders << "]\n";
	cout << "Cooks: " << Normal_C + Vegan_C + VIP_C << " [Norm: " << Normal_C << ", Veg: " << Vegan_C << ", VIP: " << VIP_C << "]\n";

	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}

