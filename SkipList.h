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
	Node* sentinel;					  // First node
	int max_level;					  // Level of the  Skip list
	std::random_device rd;			  // random number to seed
	std::mt19937 rng;				  // generator to be seeded
	std::bernoulli_distribution flip; // probabilty of increasing level 
	int size;						  // number of nodes in the skip list

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
				// if key > node->key traverse 

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
			:sentinel{ create_Node(def,std::numeric_limits<int>::min(),0) }, max_level{level_Param}
			, rd{}, rng{ rd() }, flip{ 0.5 }, size{1}
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

		/* Deep copy constructor, copies memeber wise members of argument object to 
			calling object at initialization*/
		SkipList(const SkipList& lv_SList)
			:sentinel{ create_Node(lv_SList.sentinel->data, lv_SList.sentinel->key,lv_SList.sentinel->level)}
			, max_level{ lv_SList.max_level }, rd{ }, rng{ rd() }, flip{0.5}
		{

			// If not at end of list deep copy node of arg skip list
			// Insert nodes of arg list in LIFO order to avoid incorrect dup order
			std::vector<Node> vec;
			vec.reserve(25);
			Node* cursor = lv_SList.sentinel->vec_Levels[0];
			while (cursor != nullptr)
			{
				// Push onto vector
				vec.push_back(*cursor);
				// Traverse to next node
				cursor = cursor->vec_Levels[0];
			}

			// Now add nodes from vector to calling object 
			for (auto it = vec.rbegin(); it != vec.rend(); ++it)
			{
				// If not at end of list deep copy node of arg skip list
				// Deep copy node 
				cursor = create_Node((*it).data, (*it).key, (*it).level);
				// Now get a instantiate an update vector 
				std::vector<Node*> update_Vector = update_Vec(cursor->key);

				// Get level of cursor node
				int curr_Lvl = cursor->level - 1;
				for (int i = curr_Lvl; i >= 0; --i)
				{
					// Traverse down and update forward ptrs of new node
					cursor->vec_Levels[i] = update_Vector[i]->vec_Levels[i];
				}
				// level of update array
				curr_Lvl = cursor->level - 1;
				// Now assign address of new node to prev nodes
				for (int i = curr_Lvl; i >= 0; --i)
				{
					update_Vector[i]->vec_Levels[i] = cursor;
				}
			}
			
		}

		/* Deep copy assignment, copies data fields and nodes of the argument object
			returns self-reference to support assignment chaining*/
		SkipList& operator = (const SkipList& lv_SList) 
		{
			// Avoid self assignment
			if (this == &lv_SList) { return *this;  }
			// Free all nodes of calling obj 
			free_Nodes(); 
			delete this->sentinel;

			// Allocate sentenial of calling object
			this->sentinel = create_Node(lv_SList.sentinel->data, lv_SList.sentinel->key, lv_SList.sentinel->level);
			this->max_level = lv_SList.max_level;

			// If not at end of list deep copy node of arg skip list
			// Insert nodes of arg list in LIFO order to avoid incorrect dup order
			std::vector<Node> vec;
			vec.reserve(25);
			Node* cursor = lv_SList.sentinel->vec_Levels[0];
			while (cursor != nullptr)
			{
				// Push onto vector
				vec.push_back(*cursor);
				// Traverse to next node
				cursor = cursor->vec_Levels[0];
			}

			// Now add nodes from vector to calling object 
			for (auto it = vec.rbegin(); it != vec.rend(); ++it)
			{
				// If not at end of list deep copy node of arg skip list
				// Deep copy node 
				cursor = create_Node((*it).data, (*it).key, (*it).level);
				// Now get a instantiate an update vector 
				std::vector<Node*> update_Vector = update_Vec(cursor->key);

				// Get level of cursor node
				int curr_Lvl = cursor->level - 1;
				for (int i = curr_Lvl; i >= 0; --i)
				{
					// Traverse down and update forward ptrs of new node
					cursor->vec_Levels[i] = update_Vector[i]->vec_Levels[i];
				}
				// level of update array
				curr_Lvl = cursor->level - 1;
				// Now assign address of new node to prev nodes
				for (int i = curr_Lvl; i >= 0; --i)
				{
					update_Vector[i]->vec_Levels[i] = cursor;
				}
			}
			return *this; 
		}

		/*Skip move constructor, takes resoruces from temporary object and tranfers 
			it to calling constructor object */
		SkipList(SkipList&& rr_SList) 
		{
			// Take resources from RR value
			this->sentinel = rr_SList.sentinel;
			this->size = rr_SList.size; 
			this->max_level = rr_SList.max_level; 
			this->rd = rr_SList.rd;
			this->rng = rr_SList.rng; 
			this->flip = rr_SList.flip; 
			// Set RR value to nullptr
			rr_SList.sentinel = nullptr; 
		}

		/*Move assignment operator overloading, takes resources from temporary object and trasnfers 
			it to calling constructor object, returns self assignment to support chaining*/
		SkipList& operator = (SkipList&& rr_SList)
		{
			// Avoid self assignment
			if (this == &rr_SList) { return *this;  }
			// Free nodes of current list
			free_Nodes(); 
			delete this->sentinel;
			// Now take resources of temporary object
			this->sentinel = rr_SList.sentinel;
			this->size = rr_SList.size;
			this->max_level = rr_SList.max_level;
			this->rd = rr_SList.rd;
			this->rng = rr_SList.rng;
			this->flip = rr_SList.flip;

			return *this; 
		}

		// Destruct and deallocate all nodes in the skip list
		~SkipList()
		{
			printf("\nFree node called\n");
			free_Nodes(); 
			// Delete last node
			delete this->sentinel; 
			sentinel = nullptr; 
			this->size = 0; 
			printf("\n~Skip list destruct~\n");
		}

		/*Void function, passes data by const referenece and key by values, creates a node and 
			adds it to the skip list*/ 
		void add_Node( T data_Param,int key_Param)
		{
			int node_Level = 1; 
			// New node's level can't be higher than max level
			while (random_Level() != false && (node_Level != max_level) ) 
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
			// inc size
			size++; 
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
			// dec size
			size--; 
			return ret_Data; 
		}

		/*Bool returning function, binary searches skip list for node */
		bool binay_Search(int key_Param )
		{
			std::vector<Node*> update_Vector = update_Vec(key_Param);
			Node* cursor = update_Vector[0]->vec_Levels[0]; 
			// If node not found throw an execption
			if (cursor == nullptr) 
			{
				throw no_such_element_exception(); 
			}
			else if (cursor->key != key_Param) 
			{
				return false; 
			}
			else 
			{
				return true; 
			}
		}

		/*Void function frees the list of nodes that after sent node*/ 
		void free_Nodes()
		{
			// Get node after sentinel
			Node* cursor = sentinel->vec_Levels[0];
			Node* target = cursor; // node to be deleted
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
			// Reassign size back to 1
			size = 1; 
		}

		// Return amount of nodes in the list
		int get_Size() 
		{
			return this->size; 
		}
		
		/* Void memeber function traverses and prints data of the skip list */
		void traverse() const
		{
			// first node highest  
			int level = sentinel->level - 1;
			Node* cursor = nullptr; 
			// Start from higest node decrement to lowest node
			for (int i = level; i >= 0; --i) 
			{
				printf("[SEN]---");
				cursor = sentinel->vec_Levels[i]; 
				// Travese horzontally 
				while (cursor != nullptr) 
				{
					printf("[%d]---",cursor->key);	
					cursor = cursor->vec_Levels[i];
				}
				printf("[NULL]\n");
				// Dec a level
			}
		}
};