#pragma once
template <typename Type>
class PirorityNode
{
private:
	Type Data;
	int Piriorty;
	PirorityNode <Type>* NextNode;
public:
	//Constructors of the node :

	PirorityNode();
	PirorityNode(const Type&, int, PirorityNode<Type>*);
	PirorityNode(const Type&, int);


	//Getters

	Type GetData()const;
	int GetPiriorty()const;
	PirorityNode<Type>* GetNext()const;

	//Setters 

	void SetData(Type);
	void SetPiriorty(int);
	void SetNode(PirorityNode<Type>*);

};


///////////////////////////////////////////////////////////////////////////////////////////////

//Member functions Implementations :

//Constructors :

template <typename Type>
PirorityNode<Type>::PirorityNode()
{
	NextNode = nullptr;
}
template <typename Type>
PirorityNode<Type>::PirorityNode(const Type& D, int P, PirorityNode<Type>* Nxt)
{
	Data = D;
	Piriorty = P;
	NextNode = Nxt;
}
template <typename Type>
PirorityNode<Type>::PirorityNode(const Type& D, int P)
{
	Data = D;
	Piriorty = P;
	NextNode = nullptr;
}
///////////////////////////////////////////////////////////

//Getters :

template <typename Type>
Type PirorityNode<Type>::GetData()const
{
	return Data;
}
template <typename Type>
int PirorityNode<Type>::GetPiriorty()const
{
	return Piriorty;
}
template <typename Type>
PirorityNode<Type>* PirorityNode<Type>::GetNext()const
{
	return NextNode;
}


//Setters :

template <typename Type>
void PirorityNode<Type>::SetData(Type D)
{
	Data = D;
}
template <typename Type>
void PirorityNode<Type>::SetNode(PirorityNode<Type>* Nxt)
{
	NextNode = Nxt;
}
template <typename Type>
void PirorityNode<Type>::SetPiriorty(int P)
{
	Piriorty = P;
}
