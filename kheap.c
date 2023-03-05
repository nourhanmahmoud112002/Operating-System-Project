#include "kheap.h"

#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include "memory_manager.h"

//==================================================================//
//==================================================================//
//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)//
//==================================================================//
//==================================================================//

void initialize_dyn_block_system()
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] initialize_dyn_block_system
	// your code is here, remove the panic and write your code
	    LIST_INIT(&FreeMemBlocksList);
	    LIST_INIT(&AllocMemBlocksList);
	    MAX_MEM_BLOCK_CNT=NUM_OF_KHEAP_PAGES;
		MemBlockNodes= (struct MemBlock*)KERNEL_HEAP_START;
		 uint32 NodeSize= ROUNDUP(sizeof(*MemBlockNodes)*MAX_MEM_BLOCK_CNT,PAGE_SIZE);
	     uint32 ret =allocate_chunk(ptr_page_directory,KERNEL_HEAP_START,NodeSize, PERM_WRITEABLE);
		initialize_MemBlocksList(MAX_MEM_BLOCK_CNT);
		//	kpanic_into_prompt("initialize_dyn_block_system() is not implemented yet...!!");
		struct MemBlock*element=LIST_FIRST(&AvailableMemBlocksList);
		LIST_REMOVE(&AvailableMemBlocksList,element);

	//	uint32 ptr_page_directory=PDX(KERNEL_HEAP_START);

		element->size=(KERNEL_HEAP_MAX-KERNEL_HEAP_START)-NodeSize;
		element->sva=KERNEL_HEAP_START+NodeSize;
	//	LIST_INSERT_HEAD(&FreeMemBlocksList,element);
		insert_sorted_with_merge_freeList(element);

		//LIST_INIT(&AvailableMemBlocksList);
	//[1] Initialize two lists (AllocMemBlocksList & FreeMemBlocksList) [Hint: use LIST_INIT()]
#if STATIC_MEMBLOCK_ALLOC
	//DO NOTHING
#else
	/*[2] Dynamically allocate the array of MemBlockNodes
	 * 	remember to:
	 * 		1. set MAX_MEM_BLOCK_CNT with the chosen size of the array
	 * 		2. allocation should be aligned on PAGE boundary
	 * 	HINT: can use alloc_chunk(...) function
	 */
#endif
	//[3] Initialize AvailableMemBlocksList by filling it with the MemBlockNodes
	//[4] Insert a new MemBlock with the remaining heap size into the FreeMemBlocksList
}

void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kmalloc
	// your code is here, remove the panic and write your code
	uint32 Target_size= ROUNDUP(size,PAGE_SIZE);
	if(isKHeapPlacementStrategyFIRSTFIT())
	{
		struct MemBlock* blk=NULL ;
		blk=alloc_block_FF(Target_size);
		if(blk!=NULL)
		{
			/*
			 blk >> sva

			 */
			// uint32 hh = ROUNDDOWN(blk->sva,PAGE_SIZE);
			 int ret= allocate_chunk(ptr_page_directory,blk->sva
					 ,Target_size, PERM_WRITEABLE);
			 if(ret==-1)
				return NULL;
			 else
			   {
				 insert_sorted_allocList(blk);
				 return (void *)blk->sva;
			   }

		}
		else
			{
			//kpanic_into_prompt("Not found ya negm");
			return NULL;
			}
	}
	else if(isKHeapPlacementStrategyBESTFIT())
	{
		//kpanic_into_prompt("8ir stratgey ya bro");
		struct MemBlock* blk=NULL ;
		blk=alloc_block_BF(Target_size);
		if(blk!=NULL)
				{
					/*
					 blk >> sva

					 */
					 uint32 NodeSize= ROUNDUP(sizeof(*MemBlockNodes)*MAX_MEM_BLOCK_CNT,PAGE_SIZE);
					// uint32 hh = ROUNDDOWN(blk->sva,PAGE_SIZE);
					 int ret= allocate_chunk(ptr_page_directory,blk->sva
							 ,Target_size, PERM_WRITEABLE);
					 if(ret==-1)
						return NULL;
					 else
					   {
						 insert_sorted_allocList(blk);
						 return (void *)blk->sva;
					   }
				}
				else
					{
					//kpanic_into_prompt("Not found ya negm");
					return NULL;
					}
	}

	return NULL;

}
void kfree(void* virtual_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kfree
	// Write your code here, remove the panic and write your code
	struct MemBlock* blk ;
	blk=find_block(&AllocMemBlocksList,(uint32 )virtual_address);
	if(blk==NULL)
	{
		return ;
	}
	else
	{
		LIST_REMOVE(&AllocMemBlocksList,blk);
	//unmap_frame(ptr_page_directory,(uint32 )blk->sva);
		int i=blk->sva;
	//	uint32 Lazy_Nagib= blk->size/PAGE_SIZE;
	for(i;i<(blk->size)+(blk->sva);i+=PAGE_SIZE)
	{
		unmap_frame(ptr_page_directory,i);
	}
	insert_sorted_with_merge_freeList(blk);
	}
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_virtual_address
	// Write your code here, remove the panic and write your code
	//panic("kheap_virtual_address() is not implemented yet...!!");
	struct FrameInfo *ptr_frame_info=to_frame_info(physical_address);


	return ptr_frame_info->va;

	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details
	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT MS2] [KERNEL HEAP] kheap_physical_address
	// Write your code here, remove the panic and write your code

		uint32 *ptr_page_table=NULL;
		uint32 hh;
	    get_page_table(ptr_page_directory,virtual_address,&ptr_page_table);
		hh=ptr_page_table[PTX(virtual_address)]>>12;

		return (hh*PAGE_SIZE)+(virtual_address&0x00000FFF);

		//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details
}



void kfreeall()
{
	panic("Not implemented!");

}

void kshrink(uint32 newSize)
{
	panic("Not implemented!");
}

void kexpand(uint32 newSize)
{
	panic("Not implemented!");
}




//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT MS2 - BONUS] [KERNEL HEAP] krealloc
	// Write your code here, remove the panic and write your code
	panic("krealloc() is not implemented yet...!!");
}
