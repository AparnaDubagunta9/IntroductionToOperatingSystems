/*
    File: my_allocator.h

    Original Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

 */

#ifndef _BuddyAllocator_h_                   // include file only once
#define _BuddyAllocator_h_
#include <iostream>
#include<vector>
using namespace std;
//typedef unsigned int uint;

/* declare types as you need *//*
    File: my_allocator.h

    Original Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

 */

#ifndef _BuddyAllocator_h_                   // include file only once
#define _BuddyAllocator_h_
#include <iostream>
#include<vector>
using namespace std;
//typedef unsigned int uint;

/* declare types as you need */

class BlockHeader{
	// decide what goes here
	// hint: obviously block size will go here
public:
	int block_size;
	bool is_used;
  bool is_left;
	BlockHeader* next;

	BlockHeader(int b_size)
	{
		block_size = b_size;
		is_used = false;
		next = NULL;
    is_left = true;
	}
	~BlockHeader();

};



class LinkedList{
	// this is a special linked list that is made out of BlockHeader s.
private:
	BlockHeader* head;		// you need a head of the list
public:
  LinkedList()
	{
		  head = NULL;
	}
	BlockHeader * get_head(){return head;}
	void insert (BlockHeader* b)
	{	// adds a block to the list
		if(b == NULL)
		{
			head = NULL;
		}

		else if(head == NULL)
		{
			head  =  b;
		}
		else
		{
			b->next = head;
			head = b;
		}

	}

	void remove (BlockHeader* b){
		if(b == head)
		{
			b = b->next;
			head = b;
			//head = head->next;
			b = NULL;
			free(b);
		}
		else
		{
			cout<<"not head"<<endl;
		}
	}
	// 	else
	// 	{
	// 		cout<<"not head"<<endl;
	// 		h = head;
	// 		prev = NULL;
	// 		while(h != NULL)
	// 		{
	// 			cout<<"hi"<<endl;
	// 			prev = h;
	// 			h = h->next;
	// 			if(h == b)
	// 			{
	// 				cout<<"hi"<<endl;
	// 				prev->next = h->next;
	// 				h = NULL;
	// 				//break;
	// 				// b = NULL;
	// 				// free(b);
	// 			}
	// 		}
	//
	// 	// 	cout<<"not head"<<endl;
	// 	// }
	// }

};


class BuddyAllocator
{
private:
	/* declare member variables as necessary */
	char * memory;
	int max_length,min_length;
	vector<LinkedList*> freelist;

public:
	BlockHeader * header;


private:
	/* private function you are required to implement
	 this will allow you and us to do unit test */
	 int get_list_num(int bsz)
	 {
		 int i=0 ;
		 int l = max_length;
		 while(i < freelist.size() && l != bsz )
			 {
				 l = l/2;
				 i++;
			 }
		 return i;

	 }

	BlockHeader* getbuddy (BlockHeader * addr)
	{
		char* address = (char*)addr;
	 int b_sz = addr->block_size;
	// cout<<"in getbuddy"<<endl;
	 //cout<<"addr->bs "<<b_sz<<endl;
	 if(b_sz == max_length )
	 {
		 return NULL;
	 }
	 else if(addr->is_left == true)
	 {
		 char *buddy_addr = (char*)(address + b_sz);
		 BlockHeader * buddy  = (BlockHeader*)(buddy_addr);
		 //cout<<"bud : "<<buddy->block_size<<endl;
		 //cout<<"getting right"<<endl;
		 return buddy;
	 }
	 else if(addr->is_left == false)
	 {
		 char *buddy_addr = (char*)(address - b_sz);
		 BlockHeader * buddy  = (BlockHeader*)(buddy_addr);
		// cout<<"getting left"<<endl;
		 return buddy;
	 }
	}
	// given a block address, this function returns the address of its buddy

	bool arebuddies (BlockHeader* block1, BlockHeader* block2)
	{
		BlockHeader * temp_bud  = getbuddy(block1);
    if(temp_bud == block2 && block1->block_size == block2->block_size)
    {
      return true;
    }
    return false;
	}
	// checks whether the two blocks are buddies are not

	BlockHeader* merge (BlockHeader* block1, BlockHeader* block2)
	{
		int new_block_size = (block1->block_size) * 2;
		int i=0; //merge from
		i = get_list_num(block1->block_size);


		if(new_block_size == max_length)
			i = 0;
		cout<<"i: "<<i<<endl;

		char * left_address;
		if(block1->is_left )
		{
		  left_address  = (char*)block1;
		  freelist[i]->remove(block1);
		  freelist[i]->remove(block2);
		}
		else
		{
		  left_address  = (char*)block2;
		  freelist[i]->remove(block2);
		  freelist[i]->remove(block1);
		}

    cout<<"";
		if(arebuddies(block1,block2) ==true  && block1->is_used == false && block2->is_used == false)
		{
		  // BlockHeader * merged = new BlockHeader(new_block_size);
		  BlockHeader *merged = new BlockHeader(new_block_size);

		  merged = (BlockHeader*)(left_address);
		  merged->block_size = new_block_size;
		 // merged->is_left = true; //right block;
		 if(i ==0)
		 {
		 merged->next = NULL;
		 freelist[0]->insert(merged);
		 BlockHeader * prev = freelist[1]->get_head();
		 // if(prev != NULL)
		 // {
			//  cout<<"is there"<<endl;
		 // }
		 freelist[1]->remove(prev);
		 return NULL;
		  }
		 else
		  {
		 freelist[i-1]->insert(merged);
		  return merged;
		  }
		}
		else
		{
		  return NULL;
		}		}

	}
	// this function merges the two blocks returns the beginning address of the merged block
	// note that either block1 can be to the left of block2, or the other way around

	BlockHeader* split (BlockHeader* block)
	{
		cout<<"entered split"<<endl;
    int i = 0;

    while(i< freelist.size())
    {
			BlockHeader * list_head = freelist[i]->get_head();
			if(list_head == NULL )
      	i++;
			else if(list_head->block_size != block->block_size)
				i++;
			else
			 break;
    }
    cout<<i<<endl;
		//cout<<freelist.size()<<endl;

    char* address = (char*)(block);
    freelist[i]->remove(block);
		//cout<<"here"<<endl;
    int new_block_size = block->block_size /2;
    block->block_size = new_block_size;
    block->is_left = true;

    BlockHeader *BR = new BlockHeader(new_block_size);
		BR->block_size = new_block_size;
		//cout<<"here"<<endl;
		BR->is_left = false;
		char * new_address = (char*)(address + new_block_size);
    BR = (BlockHeader*)(new_address);
    //right block;

    block->next = BR;
    //update free_list

    freelist[i+1]->insert(BR);
		cout<<"done"<<endl;
		freelist[i+1]->insert(block);
		cout<<"done"<<endl;
    return block;
	}
	// splits the given block by putting a new header halfway through the block
	// also, the original header needs to be corrected


  vector<LinkedList*> get_list(){return freelist;}

public:
	BuddyAllocator (int _basic_block_size, int _total_memory_length);
	/* This initializes the memory allocator and makes a portion of
	   ’_total_memory_length’ bytes available. The allocator uses a ’_basic_block_size’ as
	   its minimal unit of allocation. The function returns the amount of
	   memory made available to the allocator. If an error occurred,
	   it returns 0.
	*/

	~BuddyAllocator();
	/* Destructor that returns any allocated memory back to the operating system.
	   There should not be any memory leakage (i.e., memory staying allocated).
	*/
	int pwr_ceil(int length)
	{
		int upper = 1;
		while(upper < length)
		{
			upper *= 2;
		}
		if(upper < min_length)
		{
			upper = min_length;
			return upper;
		}
		//cout<<"upper: "<<upper<<endl;
		return upper;
	}
	char* alloc(int _length);
	/* Allocate _length number of bytes of free memory and returns the
		address of the allocated portion. Returns 0 when out of memory. */

	int free(char* _a);
	/* Frees the section of physical memory previously allocated
	   using ’my_malloc’. Returns 0 if everything ok. */

	void debug ();
	/* Mainly used for debugging purposes and running short test cases */
	/* This function should print how many free blocks of each size belong to the allocator
	at that point. The output format should be the following (assuming basic block size = 128 bytes):

	128: 5
	256: 0
	512: 3
	1024: 0
	....
	....
	 which means that at point, the allocator has 5 128 byte blocks, 3 512 byte blocks and so on.*/
};

#endif


class BlockHeader{
	// decide what goes here
	// hint: obviously block size will go here
public:
	int block_size;
	bool is_used;
  bool is_left;
	BlockHeader* next;

	BlockHeader(int b_size)
	{
		block_size = b_size;
		is_used = false;
		next = NULL;
    is_left = true;
	}
	~BlockHeader();

};



class LinkedList{
	// this is a special linked list that is made out of BlockHeader s.
private:
	BlockHeader* head;		// you need a head of the list
public:
  LinkedList()
	{
		  head = NULL;
	}
	BlockHeader * get_head(){return head;}
	void insert (BlockHeader* b)
	{	// adds a block to the list
		if(b == NULL)
		{
			head = NULL;
		}

		else if(head == NULL)
		{
			head  =  b;
		}
		else
		{
			b->next = head;
			head = b;
		}

	}

	void remove (BlockHeader* b){  // removes a block from the list
		if(b == head)
		{
			b = b->next;
			head = b;
			//head = head->next;
			b = NULL;
			free(b);
		}
		else
		{
			cout<<"not head"<<endl;
		}
	}
};


class BuddyAllocator
{
private:
	/* declare member variables as necessary */
	char * memory;
	int max_length,min_length;
	vector<LinkedList*> freelist;

public:
	BlockHeader * header;


private:
	/* private function you are required to implement
	 this will allow you and us to do unit test */
	 int get_list_num(int bsz)
	 {
		 int i=0 ;
		 int l = max_length;
		 while(i < freelist.size() && l != bsz )
			 {
				 l = l/2;
				 i++;
			 }
		 return i;

	 }

	BlockHeader* getbuddy (BlockHeader * addr)
	{
		char* address = (char*)addr;
	 int b_sz = addr->block_size;
	// cout<<"in getbuddy"<<endl;
	 //cout<<"addr->bs "<<b_sz<<endl;
	 if(b_sz == max_length )
	 {
		 return NULL;
	 }
	 else if(addr->is_left == true)
	 {
		 char *buddy_addr = (char*)(address + b_sz);
		 BlockHeader * buddy  = (BlockHeader*)(buddy_addr);
		 //cout<<"bud : "<<buddy->block_size<<endl;
		 //cout<<"getting right"<<endl;
		 return buddy;
	 }
	 else if(addr->is_left == false)
	 {
		 char *buddy_addr = (char*)(address - b_sz);
		 BlockHeader * buddy  = (BlockHeader*)(buddy_addr);
		// cout<<"getting left"<<endl;
		 return buddy;
	 }
	}
	// given a block address, this function returns the address of its buddy

	bool arebuddies (BlockHeader* block1, BlockHeader* block2)
	{
		BlockHeader * temp_bud  = getbuddy(block1);
    if(temp_bud == block2 && block1->block_size == block2->block_size)
    {
      return true;
    }
    return false;
	}
	// checks whether the two blocks are buddies are not

	BlockHeader* merge (BlockHeader* block1, BlockHeader* block2)
	{

		int new_block_size = (block1->block_size) * 2;
	  //cout<<endl;
		//cout<<"new: "<<new_block_size<<endl;
		int i; //merge from
		i = get_list_num(block1->block_size);

		if(new_block_size == max_length)
			i = 0;
		//cout<<"i: "<<i<<endl;

		char * left_address;
		if(block1->is_left)
		{
		  //cout<<"entered left loop"<<endl;
		  left_address  = (char*)block1;
		  freelist[i]->remove(block1);
		  freelist[i]->remove(block2);
			// if(freelist[i]->get_head() == NULL)
			// 	cout<<""<<i<<endl;
		}
		else
		{
		 // cout<<"entered right loop"<<endl;
		  left_address  = (char*)block2;
		  freelist[i]->remove(block2);
		  freelist[i]->remove(block1);
			// if(freelist[i]->get_head() == NULL)
			// 	cout<<"all removed"<<endl;
		}

	//	cout<<"done with lists"<<endl;

		if(arebuddies(block1,block2) ==true  && block1->is_used == false && block2->is_used == false)
		{
		 //cout<<"\nentered"<<endl;
		  BlockHeader *merged = new BlockHeader(new_block_size);

		  merged = (BlockHeader*)(left_address);
		  merged->block_size = new_block_size;
		 if(i ==0)
		 {
		 merged->next = NULL;
		 cout<<"initial"<<endl;
		 freelist[0]->insert(merged);
		 return NULL;
		  }
		 else
		  {
		 cout<<"not first"<<endl;
		 freelist[i-1]->insert(merged);
		  return merged;
		  }
		}
		else
		{
		 //cout<<"cant merge"<<endl;
		  return NULL;
		}

	}
	// this function merges the two blocks returns the beginning address of the merged block
	// note that either block1 can be to the left of block2, or the other way around

	BlockHeader* split (BlockHeader* block)
	{
		//cout<<"entered split"<<endl;
    int i = 0;

    while(i< freelist.size())
    {
			BlockHeader * list_head = freelist[i]->get_head();
			if(list_head == NULL )
      	i++;
			else if(list_head->block_size != block->block_size)
				i++;
			else
			 break;
    }

    char* address = (char*)(block);
    freelist[i]->remove(block);
    int new_block_size = block->block_size /2;
    block->block_size = new_block_size;
    block->is_left = true;
    BlockHeader *BR = new BlockHeader(new_block_size);
		char * new_address = (char*)(address + new_block_size);
    BR = (BlockHeader*)(new_address);
    BR->block_size = new_block_size;
    BR->is_left = false; //right block;

    block->next = BR;
    //update free_list

    freelist[i+1]->insert(BR);
		freelist[i+1]->insert(block);
    return block;
	}
	// splits the given block by putting a new header halfway through the block
	// also, the original header needs to be corrected


  vector<LinkedList*> get_list(){return freelist;}

public:
	BuddyAllocator (int _basic_block_size, int _total_memory_length);
	/* This initializes the memory allocator and makes a portion of
	   ’_total_memory_length’ bytes available. The allocator uses a ’_basic_block_size’ as
	   its minimal unit of allocation. The function returns the amount of
	   memory made available to the allocator. If an error occurred,
	   it returns 0.
	*/

	~BuddyAllocator();
	/* Destructor that returns any allocated memory back to the operating system.
	   There should not be any memory leakage (i.e., memory staying allocated).
	*/
	int pwr_ceil(int length)
	{
		int upper = 1;
		while(upper < length)
		{
			upper *= 2;
		}
		if(upper < min_length)
		{
			upper = min_length;
			return upper;
		}
		//cout<<"upper: "<<upper<<endl;
		return upper;
	}
	char* alloc(int _length);
	/* Allocate _length number of bytes of free memory and returns the
		address of the allocated portion. Returns 0 when out of memory. */

	int free(char* _a);
	/* Frees the section of physical memory previously allocated
	   using ’my_malloc’. Returns 0 if everything ok. */

	void debug ();
	/* Mainly used for debugging purposes and running short test cases */
	/* This function should print how many free blocks of each size belong to the allocator
	at that point. The output format should be the following (assuming basic block size = 128 bytes):

	128: 5
	256: 0
	512: 3
	1024: 0
	....
	....
	 which means that at point, the allocator has 5 128 byte blocks, 3 512 byte blocks and so on.*/
};

#endif
