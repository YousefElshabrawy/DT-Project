#pragma once
#include "TreeNode.h"

/*
							[Root]
							 / \
							/   \
						   /     \
						[left]  [right]
						 / \      / \
						/   \    /   \
					  [l]  [r] [l]   [r]
*/

template <typename Type>
class BinaryTree
{
	TreeNode<Type>* Root;


public:


	//Constructors :

	BinaryTree(); //Default Constructor
	BinaryTree(TreeNode<Type>* TN);

	//Methods
	bool IsEmpty()const;
	bool Insert(const Type& newEntry);

};

/////////////////////////////////////////////////////////////////////////////////

//Member functions implementations :


//For constructors :

// i-Default constructor Only make the root node points to null
template <typename Type>
BinaryTree<Type>::BinaryTree()
{
	Root = nullptr;
}

// ii-Constructor to make the root points to the function parameter :
template <typename Type>
BinaryTree<Type>::BinaryTree(TreeNode<Type>* R)
{
	Root = R;
}

template<typename Type>
inline bool BinaryTree<Type>::IsEmpty() const
{
	return !Root;
}

template<typename Type>
bool BinaryTree<Type>::Insert(const Type& newEntry)
{
	TreeNode<Type> *Temp = new TreeNode<Type>(newEntry);
	TreeNode<Type>* Parent;

	Temp->SetLeftNode(nullptr);
	Temp->SetRightNode(nullptr);

	Parent = nullptr;


	if (IsEmpty())
		Root = Temp;

	else
	{
		TreeNode<Type>* Ptr;
		Ptr = Root;


		while (Ptr!=nullptr)
		{
			Parent = Ptr;
			if (newEntry > Ptr->GetData())
				Ptr = Ptr->GetRightNode();
			else Ptr = Ptr->GetLeftNode();
		}


		if (newEntry < Parent->GetData())
			Parent->SetLeftNode(Temp);

		else
			Parent->SetRightNode(Temp);

	}


	return true;
}
