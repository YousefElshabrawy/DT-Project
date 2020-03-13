#pragma once
template <typename Type>
class Node
{
private:
	Type Data;
	int Piriorty;
	Node <Type>* NextNode;
public:
	//Constructors of the node :

	Node();
	Node(const Type &, int, Node<Type>*);
	Node(const Type&, int);


	//Getters

	Type GetData()const;
	int GetPiriorty()const;
	Node<Type>* GetNext()const;

	//Setters 

	void SetData(Type);
	void SetPiriorty(int);
	void SetNode(Node<Type>*);

};


///////////////////////////////////////////////////////////////////////////////////////////////

//Member functions Implementations :

//Constructors :

template <typename Type>
Node<Type>::Node()
{
	NextNode = nullptr;
}
template <typename Type>
Node<Type>::Node(const Type& D, int P, Node<Type>* Nxt)
{
	Data = D;
	Piriorty = P;
	NextNode = Nxt;
}
template <typename Type>
Node<Type>::Node(const Type& D, int P)
{
	Data = D;
	Piriorty = P;
	NextNode = nullptr;
}
///////////////////////////////////////////////////////////

//Getters :

template <typename Type>
Type Node<Type>::GetData()const
{
	return Data;
}
template <typename Type>
int Node<Type>::GetPiriorty()const
{
	return Piriorty;
}
template <typename Type>
Node<Type>* Node<Type>::GetNext()const
{
	return NextNode;
}


//Setters :

template <typename Type>
void Node<Type>::SetData(Type D)
{
	Data = D;
}
template <typename Type>
void Node<Type>::SetNode(Node<Type>* Nxt)
{
	NextNode = Nxt;
}
template <typename Type>
void Node<Type>::SetPiriorty(int P)
{
	Piriorty = P;
}
