#pragma once

#include "Generic_DS/Node.h"
#include "Order.h"
#include <iostream>
using namespace std;
template <class Type>
class ListADT
{
	Node<Type>* Head;
	Node<Type>* End;
	int size;
public:

	//Constructor : This Constructor is to intialize the Head and End pointers of the List
	ListADT();


	//Member Functions :
	//Insertion Member Fuctions :

	bool push(const Type& NewEntry);
	bool pushEnd(const Type& NEwEntry);

	//Pop And Remove Functions :

	bool pop(Type& Entry);
	bool DeleteItem(const Type& Entry);
	//bool SearchAndExtract(Type);
	Type SearchByID(int ID);
	bool IsEmpty()const;

	Type* toArray(int& count);	//returns array of T (array if items)

	int GetSize();

	//Destructor to deallocate the List
	~ListADT();





};



template<class Type>
inline ListADT<Type>::ListADT()
{
	Head = nullptr;
	End = nullptr;
	size = 0;
}

template<class Type>
inline bool ListADT<Type>::push(const Type& NewEntry)
{
	Node<Type>* NewNode = new Node<Type>(NewEntry);
	//If The Linked List is empty
	if (!Head)
	{
		Head = NewNode;
		size++;
	}
	else
	{
		NewNode->setNext(Head);
		Head = NewNode;
		size++;
	}
	return true;
}

template<class Type>
inline bool ListADT<Type>::pushEnd(const Type& NewEntry)
{
	Node<Type>* NewNode = new Node<Type>(NewEntry);
	if (!Head)
	{
		Head = NewNode;
		End = NewNode;
	}
	else
		End->setNext(NewNode);
	End = NewNode;
	size++;
	return true;
}

template<class Type>
inline bool ListADT<Type>::pop(Type& Entry)
{
	if (IsEmpty())
		return false;

	Node<Type>* Temp = Head;
	Entry = Head->getItem();
	Head = Head->getNext();
	delete Temp;
	Temp = nullptr;
	size--;
	return true;

}

template<class Type>
inline Type ListADT<Type>::SearchByID(int ID)
{
	if (IsEmpty())
		return nullptr;

	Node<Type>* CurrentNode;
	if (Head->getItem()->GetID() == ID) //Indication that it is the first Node
	{
		Type T = Head->getItem();
		Node<Type>* Temp = Head;
		Head = Head->getNext();
		delete Temp;
		Temp = nullptr;
		size--;
		return T;
	}
	//If It Isnot the Head
	Node<Type>* PrevNode = Head;
	CurrentNode = Head->getNext();
	Type Value = NULL; //to be checked when is needed to be used
	while (CurrentNode)
	{
		if (CurrentNode->getItem()->GetID() == ID)
		{
			PrevNode->setNext(CurrentNode->getNext());
			Value = CurrentNode->getItem();
			delete CurrentNode;
			CurrentNode = nullptr;
			size--;
			return Value;
		}
		PrevNode = CurrentNode;
		CurrentNode = CurrentNode->getNext();
	}
	return Value;
}

template<class Type>
inline bool ListADT<Type>::IsEmpty() const
{
	return !Head;
}

template<class Type>
inline Type* ListADT<Type>::toArray(int& count)
{
	count = 0;

	if (!Head)
		return nullptr;
	//counting the no. of items in the Queue
	Node<Type>* p = Head;
	while (p)
	{
		count++;
		p = p->getNext();
	}


	Type* Arr = new Type[count];
	p = Head;
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}

template<class Type>
inline int ListADT<Type>::GetSize()
{
	return size;
}

template<class Type>
inline bool ListADT<Type>::DeleteItem(const Type& Entry)
{
	Node<Type>* Temp = Head;
	//If the node is head node : 
	if (Head->getItem() == Entry)
	{
		Temp = Head;
		Head = Head->getNext();
		delete Temp;
		Temp = nullptr;
		size--;
		return true;
	}
	//If Not the Head 
	Node<Type>* prev = Head;
	Temp = Head->getNext();
	while (Temp)
	{
		if (Temp->getItem() == Entry)
		{
			if (Temp != End)
			{
				prev->setNext(Temp->getNext());
				Node<Type>* tptr = Temp;
				Temp = Temp->getNext();
				delete tptr;
				tptr = nullptr;
				size--;
				return true;
			}
			else
			{
				prev->setNext(Temp->getNext());
				Node<Type>* tptr = Temp;
				Temp = Temp->getNext();
				delete tptr;
				tptr = nullptr;
				size--;
				End = prev;
				return true;
			}

		}
		else
		{
			prev = Temp;
			Temp = Temp->getNext();
		}
	}
	return false;
}

template<class Type>
inline ListADT<Type>::~ListADT()
{
	delete Head;
	size = 0;
}






