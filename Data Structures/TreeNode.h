#pragma once

//This Is The Node of the tree Data Structure :
/*

						Root
						/  \
                       /    \
					 Left  Right
*/

template <typename Type>
class TreeNode
{
	//Data Objects
	Type Data; 
	TreeNode<Type>* LeftNode;
	TreeNode<Type>* RightNode;

public :
//Member Functions :

	//Different Constructors

	TreeNode();
	TreeNode(const Type& D);
	TreeNode(const Type& D, TreeNode<Type>* L, TreeNode<Type>* R);


	//Setters :

	void SetData(const Type& D);
	void SetLeftNode(TreeNode<Type>* L);
	void SetRightNode(TreeNode<Type>* R);


	//Getters :

	Type GetData()const;
	TreeNode<Type>* GetLeftNode()const;
	TreeNode<Type>* GetRightNode()const;



	//Destructor 

	~TreeNode();



};

/*------------------------------------------------------------------------------------------------------*/

//Member functions implementations :

//Constructors :
//---------------

template <typename Type>
TreeNode<Type>::TreeNode() //Default constructor if the user dont enter any parameters ONLY
{
	LeftNode = nullptr;
	RightNode = nullptr;
}

template <typename Type>
TreeNode<Type>::TreeNode(const Type& D) //Constructor to add Data only to the node without assignment of the Left and right nodes
{
	Data = D;
	LeftNode = nullptr;
	RightNode = nullptr;
}

template <typename Type>
TreeNode<Type>::TreeNode(const Type& D, TreeNode <Type>* L, TreeNode <Type>* R) //Constructor to assign nodes and data of the node
{
	Data = D;
	LeftNode = L;
	RightNode = R;
}



//Setters Implementation :


template <typename Type>
void TreeNode<Type>::SetData(const Type& D)
{
	Data = D;
}

template <typename Type>
void TreeNode<Type>::SetLeftNode(TreeNode<Type>* L)
{
	LeftNode = L;
}

template <typename Type>
void TreeNode<Type>::SetRightNode(TreeNode<Type>* R)
{
	RightNode = R;
}



//Getters Implementations 


template <typename Type>
Type TreeNode<Type>::GetData()const
{
	return Data;
}

template <typename Type>
TreeNode<Type>* TreeNode<Type>::GetLeftNode()const
{
	return LeftNode;
}

template <typename Type>
TreeNode<Type>* TreeNode<Type>::GetRightNode()const
{
	return RightNode;
}



//Destructor :

template <typename Type>
TreeNode<Type>::~TreeNode()
{

}

//End of tree node class