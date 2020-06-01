

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
	NumOfDeliveredUrgentOrders = 0;
	NOofInjuredCooks = 0;
	SharedBTVIPandURG = 0;
	TotalWaitTime = 0;
	TotalServTime = 0;
	NoOfPromotedOrders = 0;
	NoOfPromotedOrders_VIP = 0;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	//The mode of displaying outputs
	{


	case MODE_INTR: //Interactive mode

		simulation('I');
		//Interactive_mode();

		break;


	case MODE_STEP: //Step by step mode 

		simulation('S');
		//Step_By_Step_mode();

		break;


	case MODE_SLNT: //Silent Mode

		simulation('N');
		//Silent_Mode();

		break;


	case MODE_DEMO: //Demo mode

		//Simple_Simulator();

		break;

	};

}

//-----------------------------------------------------------------------------------------------------//
//--------------------------Reading the inputs from textfile-------------------------------------------//
void Restaurant::ReadInputs()
{

	ifstream InputFile;
	do {
		pGUI->DrawImage("TextfileImage");
		pGUI->PrintMessage("Enter the file name without .txt :");
		string FileName = pGUI->GetString();

		FileName = FileName + ".txt";

		InputFile.open(FileName); //Opening the file (Notice that the file name is step by step "May be changed in next phases")

		if (InputFile.is_open() == false)
		{
			//pGUI->PrintMessage("The file name is not exsist , please try again, Please press anywhere to continue");
			pGUI->DrawImage("FileNotFound");
			pGUI->PrintMessage("Please press anywhere to continue");
			pGUI->waitForClick();
			pGUI->PrintMessage(" ");
		}

	} while (InputFile.is_open() == false);

	//pGUI->PrintMessage("File Is Opened Successfully! , Please press anywhere to continue");
	pGUI->PrintMessage("Please press anywhere to continue");
	pGUI->DrawImage("FileFound");
	pGUI->waitForClick();
	pGUI->PrintMessage(" ");

	//------------------------------------> COOKS <--------------------------------------------//

	// i - Reading the cooks parameters (Inputs)

	InputFile >> Normal_C >> Vegan_C >> VIP_C; //Number of Normal Vegan and VIP Cooks

	InputFile >> SN_min >> SN_max >> SG_min >> SG_max >> SV_min >> SV_max;  //Speed of Normal , Vegan , and VIP Cooks

	InputFile >> BO >> BN_min >> BN_max >> BG_min >> BG_max >> BV_min >> BV_max;
	/*
	 BO: the number of orders a cook must prepare before taking a break .
	 BN: the break duration (in timesteps) for normal cooks .
	 BG: the break duration for vegan ones .
	 BV: the break duration for VIP cooks.
	*/

	InputFile >> InjProp >> RstPrd;


	// ii- Creating the cooks :

	int CookID = 0; //Intially the first cook ID


	//VIP Cooks :

	for (int i = 0; i < VIP_C; i++)
	{
		Cook* Cook_ptr = new Cook(CookID++, TYPE_VIP, SV_max, SV_min, BV_max, BV_min, BO);
		VIP_Cooks.pushEnd(Cook_ptr);
	}

	//Normal Cooks

	for (int i = 0; i < Normal_C; i++)
	{
		Cook* Cook_ptr = new Cook(CookID++, TYPE_NRM, SN_max, SN_min, BN_max, BN_min, BO);
		Normal_Cooks.pushEnd(Cook_ptr);
	}

	//VEGAN Cooks :

	for (int i = 0; i < Vegan_C; i++)
	{
		Cook* Cook_ptr = new Cook(CookID++, TYPE_VGAN, SG_max, SG_min, BG_max, BG_min, BO);
		Vegan_Cooks.pushEnd(Cook_ptr);
	}

	///////////////////////////////////////////////////////////////////////////////

	InputFile >> AutoP >> VIP_WT; //Represent the number of timesteps after which an order is automatically promoted to VIP.

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
		float MONY; //is the total order money.


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
	int nomOf_Urgent_Orders = UrgentOrders.GetSize();
	int nomOf_IN_Service_Orders = In_Service_List.GetSize();
	int nomOf_Finished_Orders = finished_List.GetSize();
	int nomOf_Waiting_Orders = nomOf_Urgent_Orders + nomOf_VIP_Orders + nomOf_Normal_Orders + nomOf_Vegan_Orders - SharedBTVIPandURG;
	//make array of orders to add them to GUI
	Order** VIP_Orders_Array = VIP_Orders.ToArray(nomOf_VIP_Orders);
	Order** Normal_Orders_Array = Normal_Orders.toArray(nomOf_Normal_Orders);
	Order** Vegan_Orders_Array = Vegan_Orders.toArray(nomOf_Vegan_Orders);
	Order** Urgent_Orders_Array = UrgentOrders.toArray(nomOf_Urgent_Orders);
	Order** Waiting_List = new Order * [nomOf_Waiting_Orders];
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
		//swap(&VIP_Orders_Array[min_index], &VIP_Orders_Array[i]);

		Order* Temp = VIP_Orders_Array[min_index];
		VIP_Orders_Array[min_index] = VIP_Orders_Array[i];
		VIP_Orders_Array[i] = Temp;

	}

	//End Sorting
	//////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////
	//Sorting all orders

	//sort num of orders for each type min1 is the minimum of them, min3 is the largest of them
	//int min1, min2, min3;
	//if (nomOf_Normal_Orders < nomOf_VIP_Orders)
	//{
	//	min1 = nomOf_Normal_Orders;
	//	min2 = nomOf_VIP_Orders;
	//}
	//else
	//{
	//	min2 = nomOf_Normal_Orders;
	//	min1 = nomOf_VIP_Orders;
	//}
	//if (nomOf_Vegan_Orders < min1)
	//{
	//	min3 = min2;
	//	min2 = min1;
	//	min1 = nomOf_Vegan_Orders;
	//}
	//else
	//{
	//	if (nomOf_Vegan_Orders < min2)
	//	{
	//		min3 = min2;
	//		min2 = nomOf_Vegan_Orders;
	//	}
	//	else
	//	{
	//		min3 = nomOf_Vegan_Orders;
	//	}
	//}

	int VIPindex = 0, Normalindex = 0, Veganindex = 0, waitingindex = 0, Urgentindex = 0;
	bool continuelooping = true;
	for (int i = 0; continuelooping; i++)
	{
		bool condition1 = (VIPindex != nomOf_VIP_Orders);
		bool condition2 = (Normalindex != nomOf_Normal_Orders);
		bool condition3 = (Veganindex != nomOf_Vegan_Orders);
		bool condition4 = (Urgentindex != nomOf_Urgent_Orders);


		if (condition4)
		{
			//Order*** PPPUO = new Order**;
			//Order* PUO;
			//PUO = *Urgent_Orders_Array[Urgentindex];
			while (Urgentindex != nomOf_Urgent_Orders && Urgent_Orders_Array[Urgentindex]->GetArrTime() == i)
			{
				Waiting_List[waitingindex] = Urgent_Orders_Array[Urgentindex];
				Urgentindex += 1;
				waitingindex += 1;
			}
		}

		if (condition1)
		{
			while (VIPindex != nomOf_VIP_Orders && VIP_Orders_Array[VIPindex]->GetArrTime() == i)
			{
				if (!VIP_Orders_Array[VIPindex]->GetIsUrgent())
				{
					Waiting_List[waitingindex] = VIP_Orders_Array[VIPindex];
					waitingindex += 1;
				}
				VIPindex += 1;
			}
		}

		if (condition2)
		{
			while (Normalindex != nomOf_Normal_Orders && Normal_Orders_Array[Normalindex]->GetArrTime() == i)
			{
				Waiting_List[waitingindex] = Normal_Orders_Array[Normalindex];
				Normalindex += 1;
				waitingindex += 1;
			}
		}

		if (condition3)
		{
			while (Veganindex != nomOf_Vegan_Orders && Vegan_Orders_Array[Veganindex]->GetArrTime() == i)
			{
				Waiting_List[waitingindex] = Vegan_Orders_Array[Veganindex];
				Veganindex += 1;
				waitingindex += 1;
			}
		}


		if (!condition1 && !condition2 && !condition3 && !condition4)
		{
			continuelooping = false;
		}
	}

	//End Sorting
	//////////////////////////////////////////////////////////////////////////////////



	////add all Orders to GUI::DrawingList
	for (int i = 0; i < nomOf_Waiting_Orders; i++)
	{

		pOrd = Waiting_List[i];
		pGUI->AddToDrawingList(pOrd);
	}

	//for (int i = 0; i < nomOf_VIP_Orders; i++)
	//{
	//	pOrd = VIP_Orders_Array[i];
	//	pGUI->AddToDrawingList(pOrd);
	//}

	//for (int i = 0; i < nomOf_Normal_Orders; i++)
	//{
	//	pOrd = Normal_Orders_Array[i];
	//	pGUI->AddToDrawingList(pOrd);
	//}

	//for (int i = 0; i < nomOf_Vegan_Orders; i++)
	//{
	//	pOrd = Vegan_Orders_Array[i];
	//	pGUI->AddToDrawingList(pOrd);
	//}

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

void Restaurant::GivesBreaks(int Time)
{
	int CurrentTimeStep = Time;
	//check for all cooks one after one if they serviced enough orders to have a break 
	int VIP_num = VIP_Cooks.GetSize();
	Cook** VIP_Cooks_Array = VIP_Cooks.toArray(VIP_num);
	for (int i = 0; i < VIP_num; i++)
	{
		if (VIP_Cooks_Array[i]->GetNumOfServedOrders() != 0 && VIP_Cooks_Array[i]->GetNumOfServedOrders() % VIP_Cooks_Array[i]->GetNumber_Of_Orders_Before_Break() == 0)
		{
			VIP_Cooks_Array[i]->SetTimeBackWork(CurrentTimeStep + VIP_Cooks_Array[i]->GetBreakDuration());
			AddtoUnavailable_Cooks(VIP_Cooks_Array[i]);
			VIP_Cooks.DeleteItem(VIP_Cooks_Array[i]);
		}
	}

	int NORMAL_num = Normal_Cooks.GetSize();
	Cook** NORMAL_Cooks_Array = Normal_Cooks.toArray(NORMAL_num);
	for (int i = 0; i < NORMAL_num; i++)
	{
		if (NORMAL_Cooks_Array[i]->GetNumOfServedOrders() != 0 && NORMAL_Cooks_Array[i]->GetNumOfServedOrders() % NORMAL_Cooks_Array[i]->GetNumber_Of_Orders_Before_Break() == 0)
		{
			NORMAL_Cooks_Array[i]->SetTimeBackWork(CurrentTimeStep + NORMAL_Cooks_Array[i]->GetBreakDuration());
			AddtoUnavailable_Cooks(NORMAL_Cooks_Array[i]);
			Normal_Cooks.DeleteItem(NORMAL_Cooks_Array[i]);
		}
	}

	int VEGAN_num = Vegan_Cooks.GetSize();
	Cook** VEGAN_Cooks_Array = Vegan_Cooks.toArray(VEGAN_num);
	for (int i = 0; i < VEGAN_num; i++)
	{
		if (VEGAN_Cooks_Array[i]->GetNumOfServedOrders() != 0 && VEGAN_Cooks_Array[i]->GetNumOfServedOrders() % VEGAN_Cooks_Array[i]->GetNumber_Of_Orders_Before_Break() == 0)
		{
			VEGAN_Cooks_Array[i]->SetTimeBackWork(CurrentTimeStep + VEGAN_Cooks_Array[i]->GetBreakDuration());
			AddtoUnavailable_Cooks(VEGAN_Cooks_Array[i]);
			Vegan_Cooks.DeleteItem(VEGAN_Cooks_Array[i]);
		}
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
				Unavailable_Cooks_Array[i]->SetTimeBackWork(0);
				VIP_Cooks.pushEnd(Unavailable_Cooks_Array[i]);
			}
			else if (Unavailable_Cooks_Array[i]->GetType() == TYPE_NRM)
			{
				Unavailable_Cooks_Array[i]->ResetNumOfServedOrders();
				Unavailable_Cooks_Array[i]->SetTimeBackWork(0);
				Normal_Cooks.pushEnd(Unavailable_Cooks_Array[i]);
			}
			else
			{
				Unavailable_Cooks_Array[i]->ResetNumOfServedOrders();
				Unavailable_Cooks_Array[i]->SetTimeBackWork(0);
				Vegan_Cooks.pushEnd(Unavailable_Cooks_Array[i]);
			}
			Unavailable_Cooks.DeleteItem(Unavailable_Cooks_Array[i]);
		}
	}

	int numofinjuredcooks = Injured_Cooks.GetSize();
	Cook** Injured_Cooks_Array = Injured_Cooks.toArray(numofinjuredcooks);
	for (int i = 0; i < numofinjuredcooks; i++)
	{
		if (Injured_Cooks_Array[i]->GetTimeBackWork() == CurrentTimeStep)
		{
			if (Injured_Cooks_Array[i]->GetType() == TYPE_VIP)
			{
				Injured_Cooks_Array[i]->ResetNumOfServedOrders();
				Injured_Cooks_Array[i]->SetTimeBackWork(0);
				Injured_Cooks_Array[i]->Recovery();
				VIP_Cooks.pushEnd(Injured_Cooks_Array[i]);
			}
			else if (Injured_Cooks_Array[i]->GetType() == TYPE_NRM)
			{
				Injured_Cooks_Array[i]->ResetNumOfServedOrders();
				Injured_Cooks_Array[i]->SetTimeBackWork(0);
				Injured_Cooks_Array[i]->Recovery();
				Normal_Cooks.pushEnd(Injured_Cooks_Array[i]);
			}
			else
			{
				Injured_Cooks_Array[i]->ResetNumOfServedOrders();
				Injured_Cooks_Array[i]->SetTimeBackWork(0);
				Injured_Cooks_Array[i]->Recovery();
				Vegan_Cooks.pushEnd(Injured_Cooks_Array[i]);
			}
			Injured_Cooks.DeleteItem(Injured_Cooks_Array[i]);
		}
	}

}

void Restaurant::AutoPromotion(int time)
{
	int CurrentTimeStep = time;
	// auto promote the normal orders to VIP ones 
	int NoOfNormalOrders = Normal_Orders.GetSize();
	Order** NORMAL_Orders_Array = Normal_Orders.toArray(NoOfNormalOrders);
	for (int i = 0; i < NoOfNormalOrders; i++)
	{
		if (CurrentTimeStep - NORMAL_Orders_Array[i]->GetArrTime() >= AutoP)
		{
			NoOfPromotedOrders++;
			NORMAL_Orders_Array[i]->SetType(TYPE_VIP);
			int Equation = NORMAL_Orders_Array[i]->GettotalMoney() + (1000 / (float)NORMAL_Orders_Array[i]->GetArrTime()) + NORMAL_Orders_Array[i]->GetSize();
			AddtoVIPQueue(NORMAL_Orders_Array[i], -1); // -1 Is an indication that it is added to the end of the queue as the priorties are all postitive	
			Normal_Orders.DeleteItem(NORMAL_Orders_Array[i]);
		}
	}

	int NoOfVIPOrders = VIP_Orders.GetSize();
	Order** VIP_Orders_Array = VIP_Orders.ToArray(NoOfVIPOrders);
	for (int i = 0; i < NoOfVIPOrders; i++)
	{
		if (!VIP_Orders_Array[i]->GetIsUrgent())
		{
			if (VIP_Orders_Array[i]->GetWasNormal())
			{
				if (CurrentTimeStep - VIP_Orders_Array[i]->GetArrTime() >= VIP_WT + AutoP)
				{
					NoOfPromotedOrders_VIP++;
					VIP_Orders_Array[i]->setIsUrgent(true);
					AddtoUrgentQueue(VIP_Orders_Array[i]);
					SharedBTVIPandURG++;
				}
			}
			else
			{
				if (CurrentTimeStep - VIP_Orders_Array[i]->GetArrTime() >= VIP_WT)
				{
					NoOfPromotedOrders_VIP++;
					VIP_Orders_Array[i]->setIsUrgent(true);
					AddtoUrgentQueue(VIP_Orders_Array[i]);
					SharedBTVIPandURG++;
				}
			}
		}
	}

	bool cont = true;
	while (cont)
	{
		if (VIP_Orders.peekFront(ORD) && ORD->GetIsUrgent())
		{
			VIP_Orders.dequeue(ORD);
			VIP_Orders.peekFront(ORD);
			SharedBTVIPandURG--;
			ORD = NULL;
		}
		else
		{
			cont = false;
		}
	}
}

string Restaurant::AssignOrders(int time)
{
	string assignedorders = "\n";
	int CurrentTimeStep = time;
	//assign orders to in-service

	while (UrgentOrders.peekFront(ORD) && VIP_Cooks.peekFront(COK))
	{
		//ORD = *UrgentOrder;

		ORD->setStatus(SRV);
		ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		AddtoInServiceList(ORD);
		UrgentOrders.dequeue(ORD);

		COK->SetCooking(true);
		COK->SetOrderForServing(ORD);
		COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		COK->SetTimeBackWork(0);
		AddtoCooknig_Cooks(COK);
		VIP_Cooks.DeleteItem(COK);
		assignedorders += "V" + to_string(COK->GetID()) + "(U" + to_string(ORD->GetID()) + "),  ";
		ORD = NULL;
		COK = NULL;
		UrgentOrder = NULL;
	}

	while (UrgentOrders.peekFront(ORD) && Normal_Cooks.peekFront(COK))
	{
		//ORD = *UrgentOrder;

		ORD->setStatus(SRV);
		ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		AddtoInServiceList(ORD);
		UrgentOrders.dequeue(ORD);

		COK->SetCooking(true);
		COK->SetOrderForServing(ORD);
		COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		COK->SetTimeBackWork(0);
		AddtoCooknig_Cooks(COK);
		Normal_Cooks.DeleteItem(COK);
		assignedorders += "N" + to_string(COK->GetID()) + "(U" + to_string(ORD->GetID()) + "),  ";
		ORD = NULL;
		COK = NULL;
		UrgentOrder = NULL;
	}

	while (UrgentOrders.peekFront(ORD) && Vegan_Cooks.peekFront(COK))
	{
		//ORD = *UrgentOrder;

		ORD->setStatus(SRV);
		ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		AddtoInServiceList(ORD);
		UrgentOrders.dequeue(ORD);

		COK->SetCooking(true);
		COK->SetOrderForServing(ORD);
		COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		COK->SetTimeBackWork(0);
		AddtoCooknig_Cooks(COK);
		Vegan_Cooks.DeleteItem(COK);
		assignedorders += "G" + to_string(COK->GetID()) + "(U" + to_string(ORD->GetID()) + "),  ";
		ORD = NULL;
		COK = NULL;
		UrgentOrder = NULL;
	}

	while (UrgentOrders.peekFront(ORD) && Unavailable_Cooks.peekFront(COK))
	{
		//ORD = *UrgentOrder;

		ORD->setStatus(SRV);
		ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		AddtoInServiceList(ORD);
		UrgentOrders.dequeue(ORD);

		COK->SetCooking(true);
		COK->SetOrderForServing(ORD);
		COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		COK->SetTimeBackWork(0);
		AddtoCooknig_Cooks(COK);
		Unavailable_Cooks.DeleteItem(COK);
		switch (COK->GetType())
		{
		case TYPE_VIP:
			assignedorders += "V" + to_string(COK->GetID()) + "(U" + to_string(ORD->GetID()) + "),  ";
			break;
		case TYPE_NRM:
			assignedorders += "N" + to_string(COK->GetID()) + "(U" + to_string(ORD->GetID()) + "),  ";
			break;
		case TYPE_VGAN:
			assignedorders += "G" + to_string(COK->GetID()) + "(U" + to_string(ORD->GetID()) + "),  ";
			break;
		default:
			break;
		}
		ORD = NULL;
		COK = NULL;
		UrgentOrder = NULL;
	}

	while (UrgentOrders.peekFront(ORD) && Injured_Cooks.peekFront(COK))
	{
		//ORD = *UrgentOrder;

		ORD->setStatus(SRV);
		ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		AddtoInServiceList(ORD);
		UrgentOrders.dequeue(ORD);

		COK->SetCooking(true);
		COK->SetOrderForServing(ORD);
		COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		AddtoCooknig_Cooks(COK);
		Injured_Cooks.DeleteItem(COK);
		switch (COK->GetType())
		{
		case TYPE_VIP:
			assignedorders += "V" + to_string(COK->GetID()) + "(U" + to_string(ORD->GetID()) + "),  ";
			break;
		case TYPE_NRM:
			assignedorders += "N" + to_string(COK->GetID()) + "(U" + to_string(ORD->GetID()) + "),  ";
			break;
		case TYPE_VGAN:
			assignedorders += "G" + to_string(COK->GetID()) + "(U" + to_string(ORD->GetID()) + "),  ";
			break;
		default:
			break;
		}
		ORD = NULL;
		COK = NULL;
		UrgentOrder = NULL;
	}

	while (VIP_Orders.peekFront(ORD) && VIP_Cooks.peekFront(COK))
	{
		if (ORD->GetIsUrgent())
		{
			VIP_Orders.dequeue(ORD);
			VIP_Orders.peekFront(ORD);
		}

		if (VIP_Orders.GetSize())
		{
			ORD->setStatus(SRV);
			ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
			AddtoInServiceList(ORD);
			VIP_Orders.dequeue(ORD);

			COK->SetCooking(true);
			COK->SetOrderForServing(ORD);
			COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
			COK->SetTimeBackWork(0);
			AddtoCooknig_Cooks(COK);
			VIP_Cooks.DeleteItem(COK);
			assignedorders += "V" + to_string(COK->GetID()) + "(V" + to_string(ORD->GetID()) + "),  ";
			ORD = NULL;
			COK = NULL;
		}
	}

	while (VIP_Orders.peekFront(ORD) && Normal_Cooks.peekFront(COK))
	{
		if (ORD->GetIsUrgent())
		{
			VIP_Orders.dequeue(ORD);
			VIP_Orders.peekFront(ORD);
		}

		if (VIP_Orders.GetSize())
		{
			ORD->setStatus(SRV);
			ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
			AddtoInServiceList(ORD);
			VIP_Orders.dequeue(ORD);

			COK->SetCooking(true);
			COK->SetOrderForServing(ORD);
			COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
			COK->SetTimeBackWork(0);
			AddtoCooknig_Cooks(COK);
			Normal_Cooks.DeleteItem(COK);
			assignedorders += "N" + to_string(COK->GetID()) + "(V" + to_string(ORD->GetID()) + "),  ";
			ORD = NULL;
			COK = NULL;
		}
	}

	while (VIP_Orders.peekFront(ORD) && Vegan_Cooks.peekFront(COK))
	{
		if (ORD->GetIsUrgent())
		{
			VIP_Orders.dequeue(ORD);
			VIP_Orders.peekFront(ORD);
		}

		if (VIP_Orders.GetSize())
		{
			ORD->setStatus(SRV);
			ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
			AddtoInServiceList(ORD);
			VIP_Orders.dequeue(ORD);

			COK->SetCooking(true);
			COK->SetOrderForServing(ORD);
			COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
			COK->SetTimeBackWork(0);
			AddtoCooknig_Cooks(COK);
			Vegan_Cooks.DeleteItem(COK);
			assignedorders += "G" + to_string(COK->GetID()) + "(V" + to_string(ORD->GetID()) + "),  ";
			ORD = NULL;
			COK = NULL;
		}
	}

	while (Vegan_Orders.peekFront(ORD) && Vegan_Cooks.peekFront(COK))
	{
		ORD->setStatus(SRV);
		ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		AddtoInServiceList(ORD);
		Vegan_Orders.dequeue(ORD);

		COK->SetCooking(true);
		COK->SetOrderForServing(ORD);
		COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		COK->SetTimeBackWork(0);
		AddtoCooknig_Cooks(COK);
		Vegan_Cooks.DeleteItem(COK);
		assignedorders += "G" + to_string(COK->GetID()) + "(G" + to_string(ORD->GetID()) + "),  ";
		ORD = NULL;
		COK = NULL;
	}

	while (Normal_Orders.peekFront(ORD) && Normal_Cooks.peekFront(COK))
	{
		ORD->setStatus(SRV);
		ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		AddtoInServiceList(ORD);
		Normal_Orders.DeleteItem(ORD);

		COK->SetCooking(true);
		COK->SetOrderForServing(ORD);
		COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		COK->SetTimeBackWork(0);
		AddtoCooknig_Cooks(COK);
		Normal_Cooks.DeleteItem(COK);
		assignedorders += "N" + to_string(COK->GetID()) + "(N" + to_string(ORD->GetID()) + "),  ";
		ORD = NULL;
		COK = NULL;
	}

	while (Normal_Orders.peekFront(ORD) && VIP_Cooks.peekFront(COK))
	{
		ORD->setStatus(SRV);
		ORD->SetServTime(ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		AddtoInServiceList(ORD);
		Normal_Orders.DeleteItem(ORD);

		COK->SetCooking(true);
		COK->SetOrderForServing(ORD);
		COK->SetTimeTODeliver(CurrentTimeStep + ceil((float)(ORD->GetSize()) / COK->GetSpeed()));
		COK->SetTimeBackWork(0);
		AddtoCooknig_Cooks(COK);
		VIP_Cooks.DeleteItem(COK);
		assignedorders += "V" + to_string(COK->GetID()) + "(N" + to_string(ORD->GetID()) + "),  ";
		ORD = NULL;
		COK = NULL;
	}
	return assignedorders;
}

void Restaurant::InjuryDuringDuty(int Time)
{
	//Gnerating R
	//srand((unsigned int)time(NULL));
	float R = ((float)rand() / (RAND_MAX));
	while (R > 1)
	{
		R--;
	}

	int CurrentTimeStep = Time;

	int numofcookingcooks = Cooknig_Cooks.GetSize();
	Cook** Cooknig_Cooks_Array = Cooknig_Cooks.toArray(numofcookingcooks);
	if (R <= InjProp)
	{
		bool done = false;
		for (int i = 0; i < numofcookingcooks && !done; i++)
		{
			if (!Cooknig_Cooks_Array[i]->GetIsInjured())
			{

				NOofInjuredCooks++;
				int CookSpeed = Cooknig_Cooks_Array[i]->GetSpeed();
				float OrderSize = Cooknig_Cooks_Array[i]->GetServingOrder()->GetSize();
				float OrderSerTime = Cooknig_Cooks_Array[i]->GetServingOrder()->GetServTime();
				int PrevTimeToDeliver = Cooknig_Cooks_Array[i]->GetTimeTODeliver();
				int newspeed = CookSpeed / 2;
				if (newspeed<1)
				{
					newspeed = 1;
				}
				float RestOfSize = OrderSize- (OrderSerTime - (PrevTimeToDeliver - CurrentTimeStep)) * CookSpeed; //Clean Code please
				if (RestOfSize<0)
				{
					RestOfSize = 0;
				}
				int timetodelieverorder = CurrentTimeStep + ceil(RestOfSize / newspeed);
				//int timetodelieverorder = CurrentTimeStep + (PrevTimeToDeliver - CurrentTimeStep) * 2;
				int servicetime = timetodelieverorder - (PrevTimeToDeliver - OrderSerTime);
				
				//int servicetime = ceil((float)(Cooknig_Cooks_Array[i]->GetServingOrder()->GetSize() - ((Cooknig_Cooks_Array[i]->GetServingOrder()->GetServTime() - (Cooknig_Cooks_Array[i]->GetTimeTODeliver() - CurrentTimeStep)) * Cooknig_Cooks_Array[i]->GetSpeed())) / (Cooknig_Cooks_Array[i]->GetSpeed() / 2));
				Cooknig_Cooks_Array[i]->SetTimeTODeliver(timetodelieverorder);
				Cooknig_Cooks_Array[i]->GetServingOrder()->SetServTime(servicetime);
				Cooknig_Cooks_Array[i]->WorkInjury();
				done = true;

			}
		}
	}
}

void Restaurant::DeliverOrders(int time)
{
	int CurrentTimeStep = time;
	int numofcookingcooks = Cooknig_Cooks.GetSize();
	Cook** Cooknig_Cooks_Array = Cooknig_Cooks.toArray(numofcookingcooks);

	for (int i = 0; i < numofcookingcooks; i++)
	{
		if (Cooknig_Cooks_Array[i]->GetCooking())
		{
			if (Cooknig_Cooks_Array[i]->GetTimeTODeliver() == CurrentTimeStep)
			{
				ORD = Cooknig_Cooks_Array[i]->GetServingOrder();

				Cooknig_Cooks_Array[i]->GetServingOrder()->setStatus(DONE);
				Cooknig_Cooks_Array[i]->GetServingOrder()->SetFinishTime(CurrentTimeStep);
				ADDtoFinishedList(Cooknig_Cooks_Array[i]->GetServingOrder());
				In_Service_List.DeleteItem(Cooknig_Cooks_Array[i]->GetServingOrder());

				Cooknig_Cooks_Array[i]->ServedOrder();
				Cooknig_Cooks_Array[i]->SetCooking(false);
				Cooknig_Cooks_Array[i]->SetOrderForServing(nullptr);
				Cooknig_Cooks_Array[i]->SetTimeTODeliver(0);



				if (Cooknig_Cooks_Array[i]->GetIsInjured())
				{
					if (Cooknig_Cooks_Array[i]->GetTimeBackWork())
					{
						Cooknig_Cooks_Array[i]->ResetNumOfServedOrders();
						Cooknig_Cooks_Array[i]->SetTimeBackWork(0);
						Cooknig_Cooks_Array[i]->Recovery();
						switch (Cooknig_Cooks_Array[i]->GetType())
						{
						case TYPE_VIP:

							VIP_Cooks.pushEnd(Cooknig_Cooks_Array[i]);
							break;
						case TYPE_NRM:

							Normal_Cooks.pushEnd(Cooknig_Cooks_Array[i]);
							break;
						case TYPE_VGAN:

							Vegan_Cooks.pushEnd(Cooknig_Cooks_Array[i]);
							break;
						}
					}
					else
					{
						Cooknig_Cooks_Array[i]->SetTimeBackWork(CurrentTimeStep + RstPrd);
						Injured_Cooks.pushEnd(Cooknig_Cooks_Array[i]);
					}
				}
				else
				{
					switch (Cooknig_Cooks_Array[i]->GetType())
					{
					case TYPE_VIP:

						VIP_Cooks.pushEnd(Cooknig_Cooks_Array[i]);
						break;
					case TYPE_NRM:

						Normal_Cooks.pushEnd(Cooknig_Cooks_Array[i]);
						break;
					case TYPE_VGAN:

						Vegan_Cooks.pushEnd(Cooknig_Cooks_Array[i]);
						break;
					}
				}


				switch (ORD->GetType())
				{
				case TYPE_VIP:
					if (ORD->GetIsUrgent())
					{
						NumOfDeliveredUrgentOrders++;
					}
					else
					{
						NumOfDeliveredVIPOrders++;
					}
					break;
				case TYPE_NRM:
					NumOfDeliveredNORMALOrders++;
					break;
				case TYPE_VGAN:
					NumOfDeliveredVEGANOrders++;
					break;
				default:
					break;
				}

				Cooknig_Cooks.DeleteItem(Cooknig_Cooks_Array[i]);
				ORD = NULL;
			}
		}
	}
}

void Restaurant::OPFile()
{
	int NumOfTotalOrders = NumOfDeliveredVIPOrders + NumOfDeliveredNORMALOrders + NumOfDeliveredVEGANOrders + NumOfDeliveredUrgentOrders;
	ofstream Outputfile;
	Outputfile.open("Output.txt");
	Order* ORD;
	Outputfile << "FT\tID\tAT\tWT\tST\n";
	while (finished_List.peekFront(ORD))
	{
		TotalWaitTime += ORD->GetWaitTime();
		TotalServTime += ORD->GetServTime();
		Outputfile << ORD->GetFinishTime() << "\t" << ORD->GetID() << "\t" << ORD->GetArrTime() << "\t" << ORD->GetWaitTime() << "\t" << ORD->GetServTime() << "\n";
		finished_List.DeleteItem(ORD);
	}
	Outputfile << "Orders: " << NumOfTotalOrders << " [Norm: " << NumOfDeliveredNORMALOrders << ", Veg: " << NumOfDeliveredVEGANOrders << ", VIP: " << NumOfDeliveredVIPOrders << "]\n";
	Outputfile << "Cooks: " << Normal_C + Vegan_C + VIP_C << " [Norm: " << Normal_C << ", Veg: " << Vegan_C << ", VIP: " << VIP_C << ", Injured: " << NOofInjuredCooks << "]\n";
	Outputfile << "Avg Wait = " << TotalWaitTime / NumOfTotalOrders << ", Avg Serv = " << TotalServTime / NumOfTotalOrders << "\n";
	Outputfile << "Urgent Orders: " << NoOfPromotedOrders_VIP << ", Auto-promoted: " << ((float)NoOfPromotedOrders / (NumOfDeliveredNORMALOrders + NoOfPromotedOrders)) * 100 << "%\n";
	Outputfile.close();
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

void Restaurant::AddtoCooknig_Cooks(Cook* CK)
{
	Cooknig_Cooks.pushEnd(CK);
}

void Restaurant::AddtoUrgentQueue(Order* po)
{
	//Order** UrgentOrder = new Order*;
	//Order* Ord = po;
	//*UrgentOrder = po;
	//UrgentOrders.enqueue(UrgentOrder);
	UrgentOrders.enqueue(po);
}





//////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------> Projects Modes <--------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////


/*
Interactive mode:
allows user to monitor the restaurant operation. At each time step, program should provide output similar to that in the figure.
In this mode, program pauses for a user mouse click to display the output of the next timestep.
*/
//void Restaurant::Interactive_mode()
//{
//
//	ReadInputs();
//	pGUI->PrintBackGrounds();
//	int CurrentTimeStep = 0;
//
//	//as long as events queue or in service orders are not empty yet
//	while (!EventsQueue.isEmpty() || !In_Service_List.IsEmpty())
//	{
//		//execute all events at current time step
//		ExecuteEvents(CurrentTimeStep);
//
//		GivesBreaks(CurrentTimeStep);
//
//		AutoPromotion(CurrentTimeStep);
//
//		AssignOrders(CurrentTimeStep);
//
//		DeliverOrders(CurrentTimeStep);
//
//		//print current timestep
//
//		string Message1 = "TS :" + to_string(CurrentTimeStep);
//		string AvailableCooks = "Avaiable Cooks ---> Normal = " + to_string(Normal_Cooks.GetSize()) + " , VIP = " + to_string(VIP_Cooks.GetSize()) + " , Vegan = " + to_string(Vegan_Cooks.GetSize());
//		string WaitingOrders = "Waiting Orders ---> Normal = " + to_string(Normal_Orders.GetSize()) + " , VIP = " + to_string(VIP_Orders.GetSize() - SharedBTVIPandURG) + " , Vegan = " + to_string(Vegan_Orders.GetSize());
//		pGUI->PrintMessage(Message1, AvailableCooks, WaitingOrders);
//
//		//add all current ordes & cooks to GUI
//		FillDrawingList();
//
//		CurrentTimeStep++;	//advance timestep
//	}
//
//	int NumOfTotalOrders = NumOfDeliveredVIPOrders + NumOfDeliveredNORMALOrders + NumOfDeliveredVEGANOrders;
//	ofstream Outputfile;
//	Outputfile.open("Output.txt");
//	Order* ORD;
//	Outputfile << "FT\tID\tAT\tWT\tST\n";
//	while (finished_List.peekFront(ORD))
//	{
//		TotalWaitTime += ORD->GetWaitTime();
//		TotalServTime += ORD->GetServTime();
//		Outputfile << ORD->GetFinishTime() << "\t" << ORD->GetID() << "\t" << ORD->GetArrTime() << "\t" << ORD->GetWaitTime() << "\t" << ORD->GetServTime() << "\n";
//		finished_List.DeleteItem(ORD);
//	}
//	Outputfile << "Orders: " << NumOfDeliveredVIPOrders + NumOfDeliveredNORMALOrders + NumOfDeliveredVEGANOrders << " [Norm: " << NumOfDeliveredNORMALOrders << ", Veg: " << NumOfDeliveredVEGANOrders << ", VIP: " << NumOfDeliveredVIPOrders << "]\n";
//	Outputfile << "Cooks: " << Normal_C + Vegan_C + VIP_C << " [Norm: " << Normal_C << ", Veg: " << Vegan_C << ", VIP: " << VIP_C << "]\n";
//	Outputfile << "Avg Wait = " << TotalWaitTime / NumOfTotalOrders << ", Avg Serv = " << TotalServTime / NumOfTotalOrders << "\n";
//	Outputfile << "Auto-promoted: " << ((float)NoOfPromotedOrders / (NumOfDeliveredNORMALOrders + NoOfPromotedOrders)) * 100;
//	Outputfile.close();
//
//	pGUI->DrawImage("ExitImage");
//	pGUI->PrintMessage("generation done, click to END program");
//	pGUI->waitForClick();
//}


/*
Simple Simulator Is ONLY To Test phase 1 Code
*/
//void Restaurant::Simple_Simulator()
//{
//	testing = true; //To check that it is only testing 
//
//	ReadInputs(); //Reading inputs from file
//
//	int CurrentTimeStep = 0;
//
//	//as long as events queue or in service orders are not empty yet
//	while (!EventsQueue.isEmpty() || !In_Service_List.IsEmpty())
//	{
//		//execute all events at current time step
//		//should ignore promotion events 
//		ExecuteEvents(CurrentTimeStep);
//
//		//Pick one order from each order type and move it to In-service list(s)
//		Order* ORD;
//		if (VIP_Orders.peekFront(ORD))
//		{
//			if (ORD->GetArrTime() != CurrentTimeStep)
//			{
//				ORD->setStatus(SRV);
//				AddtoInServiceList(ORD);
//				VIP_Orders.dequeue(ORD);
//				ORD = NULL;
//			}
//		}
//
//		if (Normal_Orders.peekFront(ORD))
//		{
//			if (ORD->GetArrTime() != CurrentTimeStep)
//			{
//				ORD->setStatus(SRV);
//				AddtoInServiceList(ORD);
//				Normal_Orders.DeleteItem(ORD);
//				ORD = NULL;
//			}
//		}
//
//		if (Vegan_Orders.peekFront(ORD))
//		{
//			if (ORD->GetArrTime() != CurrentTimeStep)
//			{
//				ORD->setStatus(SRV);
//				AddtoInServiceList(ORD);
//				Vegan_Orders.dequeue(ORD);
//				ORD = NULL;
//			}
//		}
//
//		//Each 5 timesteps, move an order of each type from In-service list(s) to finished list(s)
//		if (CurrentTimeStep % 5 == 0)
//		{
//			bool finishedNormal = false, finishedVIP = false, finishedVEGAN = false;
//
//			int nomOf_IN_SERVICE_ORDERS = In_Service_List.GetSize();
//			Order** In_Service_Array = In_Service_List.toArray(nomOf_IN_SERVICE_ORDERS);
//
//			for (int i = 0; i < nomOf_IN_SERVICE_ORDERS; i++)
//			{
//				if (!finishedVIP && In_Service_Array[i]->GetType() == TYPE_VIP)
//				{
//					In_Service_Array[i]->setStatus(DONE);
//					ADDtoFinishedList(In_Service_Array[i]);
//					In_Service_List.DeleteItem(In_Service_Array[i]);
//					finishedVIP = true;
//				}
//				else if (!finishedNormal && In_Service_Array[i]->GetType() == TYPE_NRM)
//				{
//					In_Service_Array[i]->setStatus(DONE);
//					ADDtoFinishedList(In_Service_Array[i]);
//					In_Service_List.DeleteItem(In_Service_Array[i]);
//					finishedNormal = true;
//				}
//				else if (!finishedVEGAN && In_Service_Array[i]->GetType() == TYPE_VGAN)
//				{
//					In_Service_Array[i]->setStatus(DONE);
//					ADDtoFinishedList(In_Service_Array[i]);
//					In_Service_List.DeleteItem(In_Service_Array[i]);
//					finishedVEGAN = true;
//				}
//			}
//		}
//
//		//print current timestep
//
//		string Message1 = "TS :" + to_string(CurrentTimeStep);
//		string AvailableCooks = "Avaiable Cooks ---> Normal = " + to_string(Normal_C) + " , VIP = " + to_string(VIP_C) + " , Vegan = " + to_string(Vegan_C);
//		string WaitingOrders = "Waiting Orders ---> Normal = " + to_string(Normal_Orders.GetSize()) + " , VIP = " + to_string(VIP_Orders.GetSize() - SharedBTVIPandURG) + " , Vegan = " + to_string(Vegan_Orders.GetSize());
//		pGUI->PrintMessage(Message1, AvailableCooks, WaitingOrders);
//
//		//add all current ordes & cooks to GUI
//		FillDrawingList();
//
//		pGUI->waitForClick();
//		CurrentTimeStep++;	//advance timestep
//	}
//
//
//
//	pGUI->DrawImage("ExitImage");
//	pGUI->PrintMessage("generation done, click to END program");
//	pGUI->waitForClick();
//}

//void Restaurant::Step_By_Step_mode()
//{
//	ReadInputs();
//	pGUI->PrintBackGrounds();
//	int CurrentTimeStep = 0;
//
//	//as long as events queue or in service orders are not empty yet
//	while (!EventsQueue.isEmpty() || !In_Service_List.IsEmpty() || !Vegan_Orders.isEmpty() || !VIP_Orders.IsEmpty() || !UrgentOrders.isEmpty() || !Normal_Orders.IsEmpty())
//	{
//		//execute all events at current time step
//		ExecuteEvents(CurrentTimeStep);
//
//		GivesBreaks(CurrentTimeStep);
//
//		AutoPromotion(CurrentTimeStep);
//
//		AssignOrders(CurrentTimeStep);
//
//		DeliverOrders(CurrentTimeStep);
//
//		//print current timestep
//
//		string Message1 = "TS :" + to_string(CurrentTimeStep);
//		string AvailableCooks = "Avaiable Cooks ---> Normal = " + to_string(Normal_Cooks.GetSize()) + " , VIP = " + to_string(VIP_Cooks.GetSize()) + " , Vegan = " + to_string(Vegan_Cooks.GetSize());
//		string WaitingOrders = "Waiting Orders ---> Normal = " + to_string(Normal_Orders.GetSize()) + " , VIP = " + to_string(VIP_Orders.GetSize() - SharedBTVIPandURG) + " , Vegan = " + to_string(Vegan_Orders.GetSize());
//		pGUI->PrintMessage(Message1, AvailableCooks, WaitingOrders);
//
//		//add all current ordes & cooks to GUI
//		FillDrawingList();
//
//		pGUI->waitForClick();
//		CurrentTimeStep++;	//advance timestep
//	}
//
//	int NumOfTotalOrders = NumOfDeliveredVIPOrders + NumOfDeliveredNORMALOrders + NumOfDeliveredVEGANOrders;
//	ofstream Outputfile;
//	Outputfile.open("Output.txt");
//	Order* ORD;
//	Outputfile << "FT\tID\tAT\tWT\tST\n";
//	while (finished_List.peekFront(ORD))
//	{
//		TotalWaitTime += ORD->GetWaitTime();
//		TotalServTime += ORD->GetServTime();
//		Outputfile << ORD->GetFinishTime() << "\t" << ORD->GetID() << "\t" << ORD->GetArrTime() << "\t" << ORD->GetWaitTime() << "\t" << ORD->GetServTime() << "\n";
//		finished_List.DeleteItem(ORD);
//	}
//	Outputfile << "Orders: " << NumOfDeliveredVIPOrders + NumOfDeliveredNORMALOrders + NumOfDeliveredVEGANOrders << " [Norm: " << NumOfDeliveredNORMALOrders << ", Veg: " << NumOfDeliveredVEGANOrders << ", VIP: " << NumOfDeliveredVIPOrders << "]\n";
//	Outputfile << "Cooks: " << Normal_C + Vegan_C + VIP_C << " [Norm: " << Normal_C << ", Veg: " << Vegan_C << ", VIP: " << VIP_C << "]\n";
//	Outputfile << "Avg Wait = " << TotalWaitTime / NumOfTotalOrders << ", Avg Serv = " << TotalServTime / NumOfTotalOrders << "\n";
//	Outputfile << "Auto-promoted: " << ((float)NoOfPromotedOrders / (NumOfDeliveredNORMALOrders + NoOfPromotedOrders)) * 100;
//	Outputfile.close();
//
//	pGUI->DrawImage("ExitImage");
//	pGUI->PrintMessage("generation done, click to END program");
//	pGUI->waitForClick();
//}

//void Restaurant::Silent_Mode()
//{
//	ReadInputs();
//	int CurrentTimeStep = 0;
//
//	//as long as events queue or in service orders are not empty yet
//	while (!EventsQueue.isEmpty() || !In_Service_List.IsEmpty())
//	{
//		//execute all events at current time step
//		ExecuteEvents(CurrentTimeStep);
//
//		GivesBreaks(CurrentTimeStep);
//
//		AutoPromotion(CurrentTimeStep);
//
//		AssignOrders(CurrentTimeStep);
//
//		DeliverOrders(CurrentTimeStep);
//
//
//		CurrentTimeStep++;	//advance timestep
//	}
//
//	int NumOfTotalOrders = NumOfDeliveredVIPOrders + NumOfDeliveredNORMALOrders + NumOfDeliveredVEGANOrders;
//	ofstream Outputfile;
//	Outputfile.open("Output.txt");
//	Order* ORD;
//	Outputfile << "FT\tID\tAT\tWT\tST\n";
//	while (finished_List.peekFront(ORD))
//	{
//		TotalWaitTime += ORD->GetWaitTime();
//		TotalServTime += ORD->GetServTime();
//		Outputfile << ORD->GetFinishTime() << "\t" << ORD->GetID() << "\t" << ORD->GetArrTime() << "\t" << ORD->GetWaitTime() << "\t" << ORD->GetServTime() << "\n";
//		finished_List.DeleteItem(ORD);
//	}
//	Outputfile << "Orders: " << NumOfDeliveredVIPOrders + NumOfDeliveredNORMALOrders + NumOfDeliveredVEGANOrders << " [Norm: " << NumOfDeliveredNORMALOrders << ", Veg: " << NumOfDeliveredVEGANOrders << ", VIP: " << NumOfDeliveredVIPOrders << "]\n";
//	Outputfile << "Cooks: " << Normal_C + Vegan_C + VIP_C << " [Norm: " << Normal_C << ", Veg: " << Vegan_C << ", VIP: " << VIP_C << "]\n";
//	Outputfile << "Avg Wait = " << TotalWaitTime / NumOfTotalOrders << ", Avg Serv = " << TotalServTime / NumOfTotalOrders << "\n";
//	Outputfile << "Auto-promoted: " << ((float)NoOfPromotedOrders / (NumOfDeliveredNORMALOrders + NoOfPromotedOrders)) * 100;
//	Outputfile.close();
//
//	pGUI->DrawImage("ExitImage");
//	pGUI->PrintMessage("generation done, click to END program");
//	pGUI->waitForClick();
//}


void Restaurant::simulation(char Mode)
{
	/////////////////////////////////////////////////////////
	//
	//		Node(I):for Interactive Mode
	//		Node(S):for StepbyStep Mode
	//		Node(N):for Silent Mode
	//
	/////////////////////////////////////////////////////////

	ReadInputs();
	if (Mode != 'N')
	{
		pGUI->PrintBackGrounds();
	}
	int CurrentTimeStep = 0;

	//as long as events queue or in service orders are not empty yet
	while (!EventsQueue.isEmpty() || !In_Service_List.IsEmpty() || !Vegan_Orders.isEmpty() || !VIP_Orders.IsEmpty() || !UrgentOrders.isEmpty() || !Normal_Orders.IsEmpty() || !Unavailable_Cooks.IsEmpty() || !Injured_Cooks.IsEmpty())
	{
		//execute all events at current time step
		ExecuteEvents(CurrentTimeStep);

		GivesBreaks(CurrentTimeStep);

		AutoPromotion(CurrentTimeStep);

		string AssignedOrders = AssignOrders(CurrentTimeStep);

		InjuryDuringDuty(CurrentTimeStep);

		DeliverOrders(CurrentTimeStep);

		if (Mode != 'N')
		{
			//print current timestep
			string Message1 = "TS :" + to_string(CurrentTimeStep);
			string AvailableCooks = "Avaiable Cooks ---> Normal = " + to_string(Normal_Cooks.GetSize()) + " , VIP = " + to_string(VIP_Cooks.GetSize()) + " , Vegan = " + to_string(Vegan_Cooks.GetSize());
			string WaitingOrders = "Waiting Orders ---> Normal = " + to_string(Normal_Orders.GetSize()) + " , VIP = " + to_string(VIP_Orders.GetSize() - SharedBTVIPandURG) + " , Vegan = " + to_string(Vegan_Orders.GetSize()) + " , Urgent = " + to_string(UrgentOrders.GetSize());
			string ServedOrders = "Served Orders ---> Normal = " + to_string(NumOfDeliveredNORMALOrders) + " , VIP = " + to_string(NumOfDeliveredVIPOrders) + " , Vegan = " + to_string(NumOfDeliveredVEGANOrders) + " , Urgent = " + to_string(NumOfDeliveredUrgentOrders);
			pGUI->PrintMessage(Message1, AvailableCooks, WaitingOrders, AssignedOrders, ServedOrders);

			//add all current ordes & cooks to GUI
			FillDrawingList();

			switch (Mode)
			{
			case 'I':
				pGUI->waitForClick();
				break;
			case 'S':
				Sleep(1000);
				break;
			}
		}

		CurrentTimeStep++;	//advance timestep
	}

	OPFile();

	pGUI->DrawImage("ExitImage");
	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();
}