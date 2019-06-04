//#include "Ackerman.h"
#include "BuddyAllocator.h"

int main(int argc, char ** argv) {

  int basic_block_size = 128*1024, memory_length = 1024* 1024;

  // create memory manager
  BuddyAllocator * allocator = new BuddyAllocator(basic_block_size, memory_length);
  //char * buf1 = allocator->alloc(128*1024);
  char* buf2 = allocator->alloc(128*1024);
   allocator->free(buf2);
 //allocator->free(buf1);
//   //char* buf3 = allocator->alloc(128*1024);
// //  char* buf4 = allocator->alloc(10*1024);
//
//   allocator->free(buf1);
//   allocator->free(buf2);
//  char * buf3 = allocator->alloc(128*1024);
//   if(buf1 == buf3)
//   {
//     cout<<"worked"<<endl;
//   }
//  allocator->free(buf3);
   allocator->debug();
  //char * buf4 = allocator->alloc(1024*1024);
  // char *buf1 = allocator->alloc(12*1024);
  // char *buf2 = allocator->alloc(1024*1024);
  // char *buf3 = allocator->alloc(12*1024);
  // allocator->free(buf1);
   //allocator->free(buf4);
  // buf2 = allocator->alloc(1024*1024);

//  //vector<LinkedList*> free_list = allocator->get_list();
// // cout<<free_list[2]->get_head()->block_size<<endl;
//   allocator->debug();
  //allocator->free(buf4);
//allocator->debug();
  //allocator->free(buf1);
  //buf1 = allocator->alloc(1024);
  //allocator->free(buf1);
  //cout<<"not freed"<<endl;

  // test memory manager
  //
  // Ackerman* am = new Ackerman ();
  // am->test(allocator); // this is the full-fledged test.

  // destroy memory manager
  delete allocator;
//  cout<<"done"<<endl;
}
