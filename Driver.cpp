#include<iostream>
#include"SkipList.h"
/*
	Author: Jesse Jimenez
	Date: 5/31/2022
	Purpose: Implement a Skip list data structure with C++ and run a series of
	Tests on the data structures functions. 
*/

int main()
{
	// Allocate skip list on the stack with a max level of 3 
	SkipList<int> list(3);
	//	(data, key)
	list.add_Node(69, 10);
	list.add_Node(50, 25); 
	list.add_Node(80, 5); 
	list.add_Node(90, 5); 

	list.traverse(); 


	return 0; 
}