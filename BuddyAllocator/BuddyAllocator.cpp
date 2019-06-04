/*
    File: my_allocator.cpp
*/
#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length)
{
  memory = new char[_total_memory_length] ;//pool
  max_length = _total_memory_length;
  min_length = _basic_block_size;

  //make free list with no of lists needed
  int num_of_lists = 1;
  int bl_size = _total_memory_length;
  while(bl_size != _basic_block_size)
  {
    bl_size = bl_size/2;
    num_of_lists++;
  }


  for(int i=0;i<num_of_lists;i++)
  {
    LinkedList * new_free_list = new LinkedList();
    freelist.push_back(new_free_list);
  }

  //create memory_block
  header = (BlockHeader*)memory;
  header->next = NULL;
  header->block_size = _total_memory_length;

  freelist[0]->insert(header);

  }



BuddyAllocator::~BuddyAllocator ()
{
  delete [] memory;
  memory = NULL;
}


char* BuddyAllocator::alloc(int _length) {
  int required_space = pwr_ceil(_length + sizeof(BlockHeader)) ;
  if(required_space > max_length)
    required_space = max_length;
  //cout<<"reqd: "<<required_space<<endl;
  //traverse through freelist to find a block
  int list_num=0;
  while(list_num < freelist.size())
  {
    BlockHeader * list_head = freelist[list_num]->get_head();
    if(list_head == NULL)
      {
        //cout<<"null"<<endl;
        list_num ++;
      }
    else
    {

      if(list_head->block_size == required_space)
      {
        //cout<<"in list"<<list_num<<endl;
        if(list_head->is_used == false)
        {

          list_head->is_used = true;
          char * user_buff = (char*)(list_head + sizeof(BlockHeader));
          BlockHeader * h = list_head;
          if(list_head->next != NULL)
          {
            list_head = list_head->next;
            //list_head->is_left = true;
          }
          h->is_used = true;
          freelist[list_num]->remove(h);
          //cout<<"alloc done"<<endl;
          return user_buff;
        }
      }

      else
      {
        if(list_head->block_size > required_space)
        {
          int next = list_num+1;
          BlockHeader * next_list_head = freelist[next]->get_head();
          while(next_list_head != NULL && next_list_head->block_size > required_space)
          {
            list_head = next_list_head;
            next ++;
            next_list_head = freelist[next]->get_head();
          }
          if(next_list_head == NULL)
          {
            split(list_head);
            list_num++;
          }
          else if(next_list_head->block_size > required_space)
         {
          split(list_head);
          list_num++;
         }

         else if(next_list_head->block_size == required_space)
         {
           list_num++;
         }
        }

        else if(list_head->block_size < required_space)
        {
          return NULL;
        }

      }

    }

  }
  if(list_num >= freelist.size())
  {
   cout<<"sorry all memory used"<<endl;
   return NULL;
  }
}

int BuddyAllocator::free(char* _a)
{
  //cout<<"\n entered free"<<endl;
  char* header_address = (char*)((long long)(_a) - 256);
  BlockHeader * block = (BlockHeader*)(header_address);
  cout<<"block: "<<block->block_size<<endl;
  block->is_used = false;
  int list_num  = get_list_num(block->block_size);

  freelist[list_num]->insert(block);
  BlockHeader * b = block;
  BlockHeader * buddy = getbuddy(b);
  if(buddy == NULL) //1st block
  {
    //cout<<"1st"<<endl;
    //cout<<"done with free"<<endl;
    return 0;
  }

  while(buddy->block_size != max_length)
  {
    // cout<<"buddy is free"<<endl;
    // cout<<buddy->block_size<<endl;
    buddy = merge(b,buddy);
    if(buddy == NULL)
      break;
    b = getbuddy(buddy);
  }


  cout<<"done with free"<<endl;

  return 0;
}



void BuddyAllocator::debug (){
cout<<"debug"<<endl;
int list_num= 0;
int l  = max_length;
while(list_num < freelist.size() && l >= min_length)
{
  BlockHeader * list_head = freelist[list_num]->get_head();
  if(list_head == NULL)
  {
    //cout<<"came here"<<endl;
    cout<<l<<" : "<<"0"<<endl;
  }
  else
  {
    int count = 0;
    while(list_head != NULL)
    {
      count++;
      list_head = list_head->next;
    }

    cout<<l<<" : "<<count<<endl;
  }

  l = l/2;
  list_num++;
}
}
