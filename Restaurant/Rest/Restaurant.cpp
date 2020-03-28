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

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		Interactive_mode();
		break;
	case MODE_STEP:
		Step_By_Step_mode();
		break;
	case MODE_SLNT:
		break;
	case MODE_DEMO:
		Simple_Simulator();
		break;

	};

}

void Restaurant::ReadInputs()
{
	ifstream InputFile;
	InputFile.open("StepByStep.txt");



	InputFile >> Normal_C >> Vegan_C >> VIP_C;

	InputFile >> SN >> SG >> SV;



	InputFile >> BO >> BN >> BG >> BV;

	//Creating the cooks :
	int CookID = 0;


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

	InputFile >> AutoP;

	///////////////////////////////////////////////////////////////////////////////

	InputFile >> M;

	// Start of loop of number of events
	for (int i = 0; i < M; i++)
	{
		char EVENT, TYP;
		int TS; //Type step
		int ID; //is a unique sequence number that identifies each order.
		int SIZE; //is the number of dishes of the order
		int MONY; //is the total order money.
		InputFile >> EVENT;
		float Equation = 0;
		switch (EVENT)
		{
		case'R':
			////////////////////////////////////////////
			InputFile >> TYP >> TS >> ID >> SIZE >> MONY;
			Equation = MONY + (1000 / (float)TS) + SIZE;
			Event* pE;
			switch (TYP)
			{

			case 'N':

				pE = new ArrivalEvent(TS, ID, TYPE_NRM, Equation, TS, MONY, SIZE);
				EventsQueue.enqueue(pE);
				pE = NULL;
				break;


			case 'V':

				pE = new ArrivalEvent(TS, ID, TYPE_VIP, Equation, TS, MONY, SIZE);
				EventsQueue.enqueue(pE);
				pE = NULL;
				break;


			case 'G':

				pE = new ArrivalEvent(TS, ID, TYPE_VGAN, Equation, TS, MONY, SIZE);
				EventsQueue.enqueue(pE);
				pE = NULL;
				break;


			default:
				break;
			}

			break;



			///////////////////////////////////////////////////


		case 'X':
			//TODO (DONE)
				//Cancelation Pointer will be created and reading It's parameters
			InputFile >> TS >> ID;
			pE = new CancelEvent(TS, ID);
			EventsQueue.enqueue(pE);
			pE = NULL;

			break;

			////////////////////////////////////////////////////////////////////

		case 'P':
			//TODO (DONE)
			//Promotion poiter is to be created and reading it's parameters
			InputFile >> TS >> ID >> ExMony;
			pE = new PromotionEvent(TS, ID, ExMony);
			EventsQueue.enqueue(pE);
			pE = NULL;

			break;
			//////////////////////////////////////////////////////////////


		}


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
	//This function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);


	Order* pOrd;
	Cook** pCVIP_Array = VIP_Cooks.toArray(VIP_C);
	Cook** pCNormal_Array = Normal_Cooks.toArray(Normal_C);
	Cook** pCVegan_Array = Vegan_Cooks.toArray(Vegan_C);

	//add all Cooks to GUI::DrawingList
	for (int i = 0; i < VIP_C; i++)
		pGUI->AddToDrawingList(pCVIP_Array[i]);

	for (int i = 0; i < Normal_C; i++)
		pGUI->AddToDrawingList(pCNormal_Array[i]);

	for (int i = 0; i < Vegan_C; i++)
		pGUI->AddToDrawingList(pCVegan_Array[i]);

	//add all Ordes to GUI::DrawingList
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

	pGUI->UpdateInterface();
	Sleep(1000);
	pGUI->ResetDrawingList();

}

Order* Restaurant::GetNormalOrderByID(int ID)
{
	return Normal_Orders.SearchByID(ID);
}

void Restaurant::DeleteNormalOrder(Order* order)
{
	Normal_Orders.DeleteItem(order);
}

void Restaurant::AddtoVIPQueue(Order* po, int Pir)
{
	VIP_Orders.enqueue(po, Pir);
}
void Restaurant::AddtoNormalQueue(Order* po)
{
	//Normal_Orders.enqueue(po);
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





/// ==> end of DEMO-related function
//////////////////////////////////////////////////////////////////////////////////////////////
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

void Restaurant::Simple_Simulator()
{
	testing = true;
	ReadInputs();

	int CurrentTimeStep = 0;

	//as long as events queue or in service orders are not empty yet
	while (!EventsQueue.isEmpty() || !In_Service_List.IsEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);

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

		int nomOf_Normal_Orders = Normal_Orders.GetSize();
		Order** Normal_Orders_Array = Normal_Orders.toArray(nomOf_Normal_Orders);
		if (!Normal_Orders.IsEmpty())
		{
			if (Normal_Orders_Array[0]->GetArrTime() != CurrentTimeStep)
			{
				Normal_Orders_Array[0]->setStatus(SRV);
				AddtoInServiceList(Normal_Orders_Array[0]);
				Normal_Orders.pop(Normal_Orders_Array[0]);
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
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);

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

		int VEGAN_num = Vegan_Cooks.GetSize();
		Cook** VEGAN_Cooks_Array = Vegan_Cooks.toArray(VEGAN_num);
		for (int i = 0; i < VEGAN_num && VEGAN_Cooks_Array[i]->GetNumOfServedOrders() != 0 && VEGAN_Cooks_Array[i]->GetNumOfServedOrders() % VEGAN_Cooks_Array[i]->GetNumber_Of_Orders_Before_Break() == 0; i++)
		{
			VEGAN_Cooks_Array[i]->SetTimeBackWork(CurrentTimeStep + VEGAN_Cooks_Array[i]->GetBreakDuration());
			AddtoUnavailable_Cooks(VEGAN_Cooks_Array[i]);
			Vegan_Cooks.DeleteItem(VEGAN_Cooks_Array[i]);
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

