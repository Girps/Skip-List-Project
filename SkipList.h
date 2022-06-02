#pragma once
#include <limits>
#include <vector>
#include <random>

struct no_such_level_exception {};
struct no_such_element_exception {};


template<typename T>
class SkipList 
{

		struct Node
		{
			// Node data fields 
			T data;
			int key;
			int level;
			std::vector<Node*> vec_Levels; // Contain pointers of nodes 

			explicit Node(T data_Param,int key_Param,int level_Param)
				:data{ data_Param }, key{ key_Param }, level{level_Param}
			{
				vec_Levels.resize(level_Param);
				// Initialize update vector to nullptr
				for (auto it = vec_Levels.begin(); it != vec_Levels.end(); it++) { *it = nullptr;  }
			}
			~Node() { printf("~[key: %d]~",key); }
		};

	private:
	// Skip list data fields
	Node* sentinel;			// First node
	const int MAX_LEVEL;	// Level of the  Skip list
	std::random_device rd;	// random number to seed
	std::mt19937 rng;		// generator to be seeded
	std::bernoulli_distribution flip; // probabilty of increasing level 
	
	// Private function returns address of location of node
	Node* search(T data, int key) {}
	
	// Returns pointer to new node allocated on the heap
	Node* create_Node(T data,int key,int level) 
	{
		Node* temp = new Node(data, key, level);
			return temp; 
	}

	// Int returning function returns value 50% being 1 or 0
	int random_Level() 
	{
		return flip(rng); 
	}


	/* Vector returning memeber function, builds an update array that holds addresses
		of node to be assigned to node holding key_Param*/
	std::vector<Node*> update_Vec(int key_Param)
	{
		/*
			Begin from the highest node and decrement down as follows
			Assign pointers to node with largest key that is less
			Than or equal to temp->key if no key exists and null
			assign to address of current node
		*/
		std::vector<Node* > update_Vector;
		update_Vector.resize(sentinel->level);
		int curr_Lvl = sentinel->level - 1;
		Node* cursor = nullptr; // Node traverse vector  
		for (auto it = update_Vector.rbegin(); it != update_Vector.rend(); ++it)
		{
			cursor = sentinel; // Get sent node
			// Traverse the skip list
			while (cursor->vec_Levels[curr_Lvl] != nullptr && key_Param > cursor->vec_Levels[curr_Lvl]->key)
			{
				// if key >= node->key traverse 

				cursor = cursor->vec_Levels[curr_Lvl];

				// else we get current node 
			}
			// Update vector 
			*it = cursor;
			// Dec a level 
			curr_Lvl--;
		}
		// Returing update vector
		return update_Vector;
	}

	public:

		// Default call type and min key limit for sentinel  
		SkipList( int level_Param,T def = T())
			:sentinel{ create_Node(def,std::numeric_limits<int>::min(),0) }, MAX_LEVEL{level_Param}
			, rd{}, rng{ rd() }, flip{0.5}
		{
			// If level arg is below 0 throw an exception
			// Not exception safe fix this later
			if (level_Param <= 0) 
			{
				throw no_such_level_exception(); 
			}
			// Resize level of the pointer table
			sentinel->vec_Levels.resize(1); 
		}

		// Destruct and deallocate all nodes in the skip list
		~SkipList()
		{
			printf("\nFree node called\n");
			free_Nodes(); 
			printf("\n~Skip list destruct~\n");
		}

		/*Void function, passes data by const referenece and key by values, creates a node and 
			adds it to the skip list*/ 
		void add_Node( T data_Param,int key_Param)
		{
			int node_Level = 1; 
			// New node's level can't be higher than max level
			while (random_Level() != false && (node_Level != MAX_LEVEL) ) 
			{
				node_Level++; // inc new node level
			}

			// Create new node 
			Node* temp = create_Node(data_Param,key_Param,node_Level);

			// Incremenet level of sentinel node if sentinel.level < temp->level 
			if (node_Level > sentinel->level) 
			{
				sentinel->vec_Levels.resize(node_Level); 
				sentinel->level = node_Level; 
			}

			// Now get a instantiate an update vector 
			std::vector<Node*> update_Vector = update_Vec(temp->key); 

			// Get level of temp node
			int curr_Lvl = temp->level - 1;
			for (int i = curr_Lvl; i >=0; --i) 
			{
				// Traverse down and update forward ptrs of new node
				temp->vec_Levels[i] = update_Vector[i]->vec_Levels[i]; 
			}
			// level of update array
			curr_Lvl = temp->level - 1; 
			// Now assign address of new node to prev nodes
			for (int i = curr_Lvl;i >=0;--i ) 
			{
				update_Vector[i]->vec_Levels[i] = temp; 
			}
		}

		/*T returning member function binary searches a node removes and retrieves its data */
		T remove_Node(int key_Param)
		{
			// Get location of the node
			std::vector<Node*> update_Vector = update_Vec(key_Param);
			Node* target = nullptr;
			auto it = update_Vector.rbegin();
			// Update vector contains address of targeted node
			target = update_Vector[0]->vec_Levels[0];

			// Throw execption if not found
			if (target == nullptr ||  target->key != key_Param) { throw no_such_element_exception(); }

			// Reassign pior nodes to addresses of nodes after the target
			for (int i = target->level-1;i >=0; --i  ) 
			{
				update_Vector[i]->vec_Levels[i] = target->vec_Levels[i]; 
			}

			// Now remove node and return its data
			T ret_Data = target->data; 
			delete target; 
			return ret_Data; 
		}
		bool binay_Search(int key )
		{
			
		}

		/*Void function frees the list of nodes that after sent node*/ 
		void free_Nodes()
		{
			Node* cursor = sentinel->vec_Levels[0];
			Node* target = cursor; 
			while (cursor != nullptr) 
			{
				// trav next node
				cursor = cursor->vec_Levels[0];
				delete target; 
				target = cursor; 
			}
			// Null out all dangling pointers
			for (auto it = sentinel->vec_Levels.begin(); it != sentinel->vec_Levels.end(); ++it) 
			{
				*it = nullptr; 
			}
		}

};