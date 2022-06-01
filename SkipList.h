#pragma once
#include <limits>
#include <vector>

struct no_such_level_exception {};

template<typename T>
class SkipList 
{

		struct Node
		{
			// Node data fields 
			T data;
			int key;
			std::vector<Node*> ptr_Table; // Contain pointers of nodes 
			explicit Node(T data_Param,int key) :data{data_Param}, next{nullptr} {}
			~Node() {}

		};

	private:
	// Skip list data fields
	Node* sentinel;
	int level;
	// Private function returns address of location of node
	Node* search(T data, int key) {}

	Node* createNode(T data,T key) 
	{
		return new Node(data,key); 
	}
	public:

		// Default call type and min key limit for sentinel  
		SkipList( int level_Param,T def = T())
			:sentinel{ createNode(def,std::numeric_limits<int>::min())}, level{level_Param}
		{
			// If level arg is below 0 throw an exception
			if (level_Param < 0) 
			{
				throw no_such_level_exception(); 
			}
			// Resize level of the pointer table
			sentinel.ptr_Table.resize(level); 
		}

		// Destruct and deallocate all nodes in the skip list
		~SkipList()
		{
		}


		Node* search(T data,int key) 
		void add_Node(T data,int key) {}
		T remove_Node(T data,int key) {}
		bool binay_Search(T data, int key ) {}
		void freeNodes() {}

};