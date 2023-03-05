/*
 * dyn_block_management.c
 *
 *  Created on: Sep 21, 2022
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"


//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//===========================
// PRINT MEM BLOCK LISTS:
//===========================

void print_mem_block_lists()
{
	cprintf("\n=========================================\n");
	struct MemBlock* blk ;
	struct MemBlock* lastBlk = NULL ;
	cprintf("\nFreeMemBlocksList:\n");
	uint8 sorted = 1 ;
	LIST_FOREACH(blk, &FreeMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nFreeMemBlocksList is NOT SORTED!!\n") ;

	lastBlk = NULL ;
	cprintf("\nAllocMemBlocksList:\n");
	sorted = 1 ;
	LIST_FOREACH(blk, &AllocMemBlocksList)
	{
		if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
			sorted = 0 ;
		cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
		lastBlk = blk;
	}
	if (!sorted)	cprintf("\nAllocMemBlocksList is NOT SORTED!!\n") ;
	cprintf("\n=========================================\n");

}

//********************************************************************************//
//********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList
	// Write your code here, remove the panic and write your code
	//panic("initialize_MemBlocksList() is not implemented yet...!!");

	LIST_INIT(&AvailableMemBlocksList);
	for(int i=0;i<numOfBlocks;i++)
	{
		LIST_INSERT_HEAD(&AvailableMemBlocksList,&MemBlockNodes[i]);
	}

}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock *find_block(struct MemBlock_List *blockList, uint32 va)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block
	// Write your code here, remove the panic and write your code
	//panic("find_block() is not implemented yet...!!");
	struct MemBlock* blk ;
		LIST_FOREACH(blk,blockList)
		{
			if(blk->sva==va)
				return (blk);
		}
				return (NULL);

}

//=========================================
// [3] INSERT BLOCK IN ALLOC LIST [SORTED]:
//=========================================
void insert_sorted_allocList(struct MemBlock *blockToInsert)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
	// Write your code here, remove the panic and write your code
	//panic("insert_sorted_allocList() is not implemented yet...!!");
	uint32 size = LIST_SIZE(&AllocMemBlocksList),ze=0;
			 if(size ==ze)
			{
				 LIST_INSERT_HEAD(&AllocMemBlocksList,blockToInsert);
			}
			else
			{
				struct MemBlock * lastElement = LIST_LAST(&AllocMemBlocksList);
				struct MemBlock * Firstelement =LIST_FIRST(&AllocMemBlocksList);
				if(blockToInsert->sva > lastElement->sva)
				{
					LIST_INSERT_TAIL(&AllocMemBlocksList , blockToInsert);
				}
				else if(blockToInsert->sva <Firstelement->sva)
				{
					LIST_INSERT_HEAD(&AllocMemBlocksList,blockToInsert);
				}
				else
				{

					struct MemBlock * blk;
					LIST_FOREACH(blk,&AllocMemBlocksList)
					{
						if(blockToInsert->sva<blk->sva)
						{
							struct MemBlock *before= blk->prev_next_info.le_prev;
							before->prev_next_info.le_next=blockToInsert;
							blockToInsert->prev_next_info.le_prev=before;
							blockToInsert->prev_next_info.le_next=blk;
							blk->prev_next_info.le_prev=blockToInsert;
							LIST_SIZE(&AllocMemBlocksList)++;
							//LIST_INSERT_BEFORE(&AllocMemBlocksList,blk,blockToInsert);
							break;
						}
					}
				 }

			}
}

//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
struct MemBlock *alloc_block_FF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_FF() is not implemented yet...!!");
	//struct MemBlock * block=LIST_LAST(&AvailableMemBlocksList);
	struct MemBlock *element;
		LIST_FOREACH(element, (&FreeMemBlocksList))
		{
		 if(element->size==size)
		 {
			LIST_REMOVE(&FreeMemBlocksList,element);
			return element;
		 }
		 else if(element->size>size)
		 {
			 struct MemBlock *element1= LIST_FIRST(&AvailableMemBlocksList);
			 LIST_REMOVE(&AvailableMemBlocksList,element1);
			 element1->size =size;
			 element1->sva=element->sva;
			 element->sva=size+element->sva;
			 element->size=element->size-size;
			 return element1;
		 }
		}

			return NULL;

}

//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
struct MemBlock *alloc_block_BF(uint32 size)
{
	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
	// Write your code here, remove the panic and write your code
	//panic("alloc_block_BF() is not implemented yet...!!");
	struct MemBlock*block;
		uint32 ma=1e9,c=-1,sol=-1,tempi;
		LIST_FOREACH (block, (&FreeMemBlocksList))
		{
			c++;
			 if(block->size==size)
			{
				LIST_REMOVE((&FreeMemBlocksList),block);
				return block;
			}
			else if(block->size>size)
			{
				tempi=(block->size)-size;
				if(tempi<ma)
				{
					ma=tempi;
					sol=c;
				}
			}
		}
		uint32 x=0;
		LIST_FOREACH (block, (&FreeMemBlocksList))
		{
			if(x==sol)
			{
				struct MemBlock *element1= LIST_FIRST(&AvailableMemBlocksList);
						 LIST_REMOVE(&AvailableMemBlocksList,element1);
						 element1->size =size;
						 element1->sva=block->sva;
						 block->sva=size+block->sva;
						 block->size=block->size-size;
						 return element1;
						 break;
			}
			x++;
		}
			return NULL;
}


//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
struct MemBlock *temp;
bool hh=0;
struct MemBlock *alloc_block_NF(uint32 size)
{
	//TODO: [PROJECT MS1 - BONUS] [DYNAMIC ALLOCATOR] alloc_block_NF
	// Write your code here, remove the panic and write your code
	 //panic("alloc_block_NF() is not implemented yet...!!");
	if(hh==0)
	{
		temp= LIST_FIRST(&FreeMemBlocksList);
		hh=1;
	}
	uint32 c=1;
	struct MemBlock *element=temp;
	while(c!=LIST_SIZE(&FreeMemBlocksList))
	{
		 if(element->size==size)
			 {
			 	if(element->prev_next_info.le_next==NULL)
			 	{
			 		temp= LIST_FIRST(&FreeMemBlocksList);
			 	}
			 	else
			 		{
			 		temp= element->prev_next_info.le_next;
			 		}
				 LIST_REMOVE(&FreeMemBlocksList,element);
				return element;
			 }
			 else if(element->size>size)
			 {
				 struct MemBlock *element1= LIST_FIRST(&AvailableMemBlocksList);
				 LIST_REMOVE(&AvailableMemBlocksList,element1);
				 element1->size =size;
				 element1->sva=element->sva;
				 element->sva=size+element->sva;
				 element->size=element->size-size;
				 temp=element;
				 return element1;
			 }
		 c++;
		 if(element->prev_next_info.le_next==NULL)
		 {
		  element= LIST_FIRST(&FreeMemBlocksList);
		 }
		 else
		 {
			 element= element->prev_next_info.le_next;
		 }
	}
	return NULL;
}





//===================================================
// [8] INSERT BLOCK (SORTED WITH MERGE) IN FREE LIST:
//===================================================
void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert)
{
	//cprintf("BEFORE INSERT with MERGE: insert [%x, %x)\n=====================\n", blockToInsert->sva, blockToInsert->sva + blockToInsert->size);
	//print_mem_block_lists() ;

	//TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_with_merge_freeList
	// Write your code here, remove the panic and write your code
	//panic("insert_sorted_with_merge_freeList() is not implemented yet...!!");
	//int size=;
	//int size_v=LIST_SIZE(&(AvailableMemBlocksList));

	if(LIST_SIZE(&FreeMemBlocksList)==0)
			{
			LIST_INSERT_HEAD(&FreeMemBlocksList,blockToInsert);
			}
		else
		{
			struct MemBlock * lastElement = LIST_LAST(&FreeMemBlocksList);
						struct MemBlock * Firstelement =LIST_FIRST(&FreeMemBlocksList);
						if(blockToInsert->sva > lastElement->sva)
						{
							if(lastElement->size+lastElement->sva==blockToInsert->sva)
							{
								lastElement->size+= blockToInsert->size;
								blockToInsert->size=0;
								blockToInsert->sva=0;
								LIST_INSERT_HEAD(&AvailableMemBlocksList,blockToInsert);
							}
							else
							{
								LIST_INSERT_TAIL(&FreeMemBlocksList, blockToInsert);
							}
						}
						else if(blockToInsert->sva <Firstelement->sva)
						{
							if(blockToInsert->size+blockToInsert->sva==Firstelement->sva)
							{
								Firstelement->sva=blockToInsert->sva;
								Firstelement->size+=blockToInsert->size;
								blockToInsert->size=0;
								blockToInsert->sva=0;
								LIST_INSERT_HEAD(&AvailableMemBlocksList,blockToInsert);
							}
							else
							{
								LIST_INSERT_HEAD(&FreeMemBlocksList,blockToInsert);
							}
						}
						else
						{
							struct MemBlock * blk;
							struct MemBlock *prev;
							LIST_FOREACH(blk,&FreeMemBlocksList)
							{
								if(blockToInsert->sva<blk->sva)
								{
								prev=blk->prev_next_info.le_prev;
								/*
								 prev sva+ size = blk_ins_sva >> merge prev
								 blk_ins_sva+size= blk_sva>> merge
								 */
								uint32 wiprev= prev->sva+prev->size
								, winew= blockToInsert->size+blockToInsert->sva;
								if(wiprev!=blockToInsert->sva&&winew!=blk->sva)
								{
									// no merge
									LIST_INSERT_BEFORE(&FreeMemBlocksList,blk,blockToInsert);
								}
								else if(wiprev==blockToInsert->sva&&winew==blk->sva)
								{
									// both merge
									prev->size+= (blockToInsert->size)+(blk->size);
									blockToInsert->size=0;
									blockToInsert->sva=0;
									blk->size=0;
									blk->sva=0;
									LIST_INSERT_HEAD(&AvailableMemBlocksList,blockToInsert);
									LIST_REMOVE(&FreeMemBlocksList,blk);
									LIST_INSERT_HEAD(&AvailableMemBlocksList,blk);

								}
								else if(wiprev==blockToInsert->sva&&winew!=blk->sva)
								{
									// prev only
									prev->size+=blockToInsert->size;
									blockToInsert->size=0;
									blockToInsert->sva=0;
									LIST_INSERT_HEAD(&AvailableMemBlocksList,blockToInsert);
								}
								else if(wiprev!=blockToInsert->sva&&winew==blk->sva)
								{
									// merge with next
									blk->sva= blockToInsert->sva;
									blk->size+= blockToInsert->size;
									blockToInsert->size=0;
									blockToInsert->sva=0;
									LIST_INSERT_HEAD(&AvailableMemBlocksList,blockToInsert);
								}
								break;
							}

						}
		          }

		}

	//cprintf("\nAFTER INSERT with MERGE:\n=====================\n");
		//print_mem_block_lists();

}

