#pragma once

#include "Generic_DS/Node.h"
#include <iostream>
using namespace std;
template <class Type>
class LinkedList
{
	Node<Type>* Head;
	Node<Type>* End;
public:
	LinkedList();
	bool push(const Type& NewEntry);
	bool pushEnd(const Type& NEwEntry);
	bool pop(Type& Entry);
	bool IsEmpty()const;
	bool DeleteItem(const Type& Entry);
	~LinkedList();





};



template<class Type>
inline LinkedList<Type>::LinkedList()
{
	Head = nullptr;
	End = nullptr;
}

template<class Type>
inline bool LinkedList<Type>::push(const Type& NewEntry)
{
	Node<Type>* NewNode = new Node<Type>(NewEntry);
	//If The Linked List is empty
	if (!Head)
	{
		Head = NewNode;
	}
	else
	{
		NewNode->setNext(Head);
		Head = NewNode;
	}
	return true;
}

template<class Type>
inline bool LinkedList<Type>::pushEnd(const Type& NewEntry)
{
	Node<Type>* NewNode = new Node<Type>(NewEntry);
	if (!Head)
	{
		Head = NewNode;
		End = NewNode;
	}
	else
		End->SetNextNode(NewNode);
	End = NewNode;
	return true;
}

template<class Type>
inline bool LinkedList<Type>::pop(Type& Entry)
{
	if (IsEmpty())
		return false;

	Node<Type>* Temp = Head;
	Entry = Head->getItem();
	Head = Head->getNext();
	delete Temp;
	Temp = nullptr;
	return true;

}

template<class Type>
inline bool LinkedList<Type>::IsEmpty() const
{
	return !Head;
}

template<class Type>
inline bool LinkedList<Type>::DeleteItem(const Type& Entry)
{
	Node<Type>* Temp = Head;
	//If the node is head node : 
	if (HeadgetItem() == Entry)
	{
		Temp = Head;
		Head = Head->getNext();
		delete Temp;
		Temp = nullptr;
		return true;
	}
	//If Not the Head 
	Node<Type>* prev = Head;
	Temp = Head->getNext();
	while (Temp)
	{
		if (Temp->getItem()==Entry)
		{
			prev->setNext(Temp->getNext());
			Node<Type>* tptr = Temp;
			Temp = Temp->getNext();
			delete tptr;
			tptr = nullptr;
			return true;
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
inline LinkedList<Type>::~LinkedList()
{
	delete Head;
}






