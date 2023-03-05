/*
 * chunk_operations.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include <kern/trap/fault_handler.h>
#include <kern/disk/pagefile_manager.h>
#include "kheap.h"
#include "memory_manager.h"


/******************************/
/*[1] RAM CHUNKS MANIPULATION */
/******************************/

//===============================
// 1) CUT-PASTE PAGES IN RAM:
//===============================
//This function should cut-paste the given number of pages from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int cut_paste_pages(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 num_of_pages)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] cut_paste_pages
	// Write your code here, remove the panic and write your code
	//panic("cut_paste_pages() is not implemented yet...!!");
	ROUNDDOWN(dest_va,PAGE_SIZE);
	ROUNDDOWN(source_va,PAGE_SIZE);


	for(int i=0;i<num_of_pages;i++)
	{

		uint32 final_dest_va=dest_va+i*PAGE_SIZE;
		uint32 *ptr_page_table=NULL;
		struct FrameInfo *ptr_frame_info=get_frame_info(page_directory,final_dest_va,&ptr_page_table);
		if(ptr_frame_info!=NULL)
		{
			return -1;
		}

	}
	for(int i=0;i<num_of_pages;i++)
		{

		   uint32 final_dest_va=dest_va+i*PAGE_SIZE;
			uint32 *ptr_page_table=NULL;
			int ret =get_page_table(page_directory,final_dest_va,&ptr_page_table);
				if(ret ==TABLE_NOT_EXIST)
				{
					create_page_table(page_directory,final_dest_va);
				}

		}

	for(int i=0;i<num_of_pages;i++)
	{
			uint32 final_dest_va=dest_va+i*PAGE_SIZE;
			uint32 final_source_va=source_va+i*PAGE_SIZE;
			uint32 perm_source=pt_get_page_permissions(page_directory,final_source_va);
		    uint32 perm_dest=pt_get_page_permissions(page_directory,final_dest_va);
		    uint32 *ptr_page_table1=NULL;
		    uint32 *ptr_page_table2=NULL;
			int ret =get_page_table(page_directory,final_source_va,&ptr_page_table1);
			get_page_table(page_directory,final_dest_va,&ptr_page_table2);
			ptr_page_table2[PTX(final_dest_va)]=ptr_page_table1[PTX(final_source_va)];
			perm_dest=perm_source;
			ptr_page_table1[PTX(final_source_va)]=0;


	}

	return 0;

}


//===============================
// 2) COPY-PASTE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int copy_paste_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] copy_paste_chunk
	// Write your code here, remove the panic and write your code
	//panic("copy_paste_chunk() is not implemented yet...!!");

    uint32 source_va1=source_va;
    uint32 dest_va1=dest_va;
  	uint32 final_source_va=source_va+size;
  	uint32 final_dest_va=dest_va+size;


	for(int i=dest_va1;i<final_dest_va;i+=PAGE_SIZE)
	{


		uint32 *ptr_page_table=NULL;
		int perms=pt_get_page_permissions(page_directory,i);

		struct FrameInfo *ptr_frame_info=get_frame_info(page_directory,i,&ptr_page_table);
		if(ptr_frame_info!=NULL)
		{
			uint32 op1=perms & PERM_WRITEABLE;
			if(op1 !=PERM_WRITEABLE)
			{
				return -1;
			}

		}


	}
	for(int i=dest_va1;i<final_dest_va;i+=PAGE_SIZE)
		{


			uint32 *ptr_page_table=NULL;
			int ret =get_page_table(page_directory,i,&ptr_page_table);
				if(ret ==TABLE_NOT_EXIST)
				{
					create_page_table(page_directory,i);
				}

		}
	uint32 source1=source_va1;

	for(int i=dest_va1;i<final_dest_va;i+=PAGE_SIZE)
	{
		       int perms=pt_get_page_permissions(page_directory,i);
				int perms_source=pt_get_page_permissions(page_directory,source1);
				uint32 *ptr_page_table1=NULL;
				int ret1 =get_page_table(page_directory,i,&ptr_page_table1);
				struct FrameInfo *ptr_frame_info=get_frame_info(page_directory,i,&ptr_page_table1);
				if(ptr_frame_info==NULL)
				{
						   allocate_frame(&ptr_frame_info);
						 uint32 op2=perms_source & PERM_USER;

						 if(op2==PERM_USER)
						 {
							 perms=perms | PERM_USER;
						 }

						 else if(op2 != PERM_USER)

						 {
							 perms=perms & (~PERM_USER);
						 }

							perms=perms |PERM_WRITEABLE;

							map_frame(page_directory,ptr_frame_info,i,perms);

				}

				source1+=PAGE_SIZE;

	}
	uint8 * ptr_source=(uint8 *)source_va1;
	uint8 * ptr_dest=(uint8 *)dest_va1;
	for(int i=dest_va1;i<final_dest_va;i++)
	{
		*ptr_dest=*ptr_source;
		ptr_dest++;
		ptr_source++;
	}


     return 0;

}

//===============================
// 3) SHARE RANGE IN RAM:
//===============================
//This function should share the given size from dest_va with the source_va
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int share_chunk(uint32* page_directory, uint32 source_va,uint32 dest_va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] share_chunk
	// Write your code here, remove the panic and write your code
	//panic("share_chunk() is not implemented yet...!!");

	     uint32 source_va1=ROUNDDOWN(source_va,PAGE_SIZE);
	     uint32 dest_va1=ROUNDDOWN(dest_va,PAGE_SIZE);
	   	uint32 final_source_va=ROUNDUP(source_va+size,PAGE_SIZE);
	   	uint32 final_dest_va=ROUNDUP(dest_va+size,PAGE_SIZE);


		for(int i=dest_va1;i<final_dest_va;i+=PAGE_SIZE)
		{


			uint32 *ptr_page_table=NULL;
			struct FrameInfo *ptr_frame_info=get_frame_info(page_directory,i,&ptr_page_table);
			if(ptr_frame_info!=NULL)
			{
				return -1;
			}

		}
		for(int i=dest_va1;i<final_dest_va;i+=PAGE_SIZE)
			{


				uint32 *ptr_page_table=NULL;
				int ret =get_page_table(page_directory,i,&ptr_page_table);
					if(ret ==TABLE_NOT_EXIST)
					{
						create_page_table(page_directory,i);
					}

			}
		uint32 source=source_va1;
		for(int i=dest_va1;i<final_dest_va;i+=PAGE_SIZE)
		{
			uint32 *ptr_page_table1=NULL;
			int ret1 =get_page_table(page_directory,final_source_va,&ptr_page_table1);
			struct FrameInfo *ptr_frame_info=get_frame_info(page_directory,source,&ptr_page_table1);
			map_frame(page_directory,ptr_frame_info,i,perms);
			source+=PAGE_SIZE;

		}




		return 0;

}

//===============================
// 4) ALLOCATE CHUNK IN RAM:
//===============================
//This function should allocate in RAM the given range [va, va+size)
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int allocate_chunk(uint32* page_directory, uint32 va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] allocate_chunk
	// Write your code here, remove the panic and write your code
	//panic("allocate_chunk() is not implemented yet...!!");
	uint32 va1=ROUNDDOWN(va,PAGE_SIZE);
	uint32 final_va=ROUNDUP(va+size,PAGE_SIZE);
	for(int i=va1;i<final_va;i+=PAGE_SIZE)
	{
		       uint32 *ptr_page_table=NULL;
				struct FrameInfo *ptr_frame_info=get_frame_info(page_directory,i,&ptr_page_table);
				if(ptr_frame_info!=NULL)
				{
					return -1;
				}

	}
	for(int i=va1;i<final_va;i+=PAGE_SIZE)
	{
		 uint32 *ptr_page_table=NULL;
		 int ret =get_page_table(page_directory,i,&ptr_page_table);
			if(ret ==TABLE_NOT_EXIST)
			{
				create_page_table(page_directory,i);
			}

	}
	for(int i=va1;i<final_va;i+=PAGE_SIZE)
	{
		uint32 *ptr_page_table=NULL;
		get_page_table(page_directory,i,&ptr_page_table);
		struct FrameInfo *ptr_frame_info=get_frame_info(page_directory,i,&ptr_page_table);
		int ret =allocate_frame(&ptr_frame_info);
		if(ret==0)
		 map_frame(page_directory,ptr_frame_info,i,perms);\
		 ptr_frame_info->va=i;

	}

 return 0;

}

/*BONUS*/
//=====================================
// 5) CALCULATE ALLOCATED SPACE IN RAM:
//=====================================
void calculate_allocated_space(uint32* page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	uint32 rsva = ROUNDDOWN(sva, PAGE_SIZE);
	uint32 reva = ROUNDUP(eva, PAGE_SIZE);
	uint32 npages = 0, ntables = 0;
	uint32 *ptr_page_table1 = NULL;
	bool check = 0;
	for(uint32 i = rsva;i < reva ;i += PAGE_SIZE)
	{
		uint32 *ptr_page_table2=NULL;
		int ret = get_page_table(page_directory, i, &ptr_page_table2);
		if(ret == TABLE_IN_MEMORY)
		{

			check = 1; //to make sure table in mem
			uint32 entry = ptr_page_table2[PTX(i)];
			if((entry & PERM_PRESENT) != 0)
			{
				npages += 1;
				if(ptr_page_table1 == NULL)
				{
					ptr_page_table1 = ptr_page_table2;
					ntables += 1;
				}

				else
					if(!(ptr_page_table2 == ptr_page_table1))
						ntables += 1;
			}
		}
	}

	if(check == 1 && ntables == 0)
		ntables += 1;

	*num_pages = npages;//return p# to function
	*num_tables = ntables;//return pt# to function
}
/*BONUS*/
//=====================================
// 6) CALCULATE REQUIRED FRAMES IN RAM:
//=====================================
// calculate_required_frames:
// calculates the new allocation size required for given address+size,
// we are not interested in knowing if pages or tables actually exist in memory or the page file,
// we are interested in knowing whether they are allocated or not.
uint32 calculate_required_frames(uint32* page_directory, uint32 sva, uint32 size)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_required_frames
	// Write your code here, remove the panic and write your code
	//panic("calculate_required_frames() is not implemented yet...!!");
	uint32 final_va =ROUNDUP(sva+size, PAGE_SIZE);
		uint32 table_size = PAGE_SIZE*1024;
		uint32 a_final_va = ROUNDUP(sva+size, table_size);
		uint32 a_sva = ROUNDDOWN(sva, table_size);
		sva = ROUNDDOWN(sva, PAGE_SIZE);
		int frames = 0;


		for(int i= a_sva; i<a_final_va; i+=table_size)
		{
			uint32 *ptr_page_table =NULL;
			int ret = get_page_table(page_directory,i, &ptr_page_table);
			if(ret == TABLE_NOT_EXIST)
			{
				//create_page_table(ptr_page_directory,i);
				frames+=1;
			}
		}

		for(int i= sva; i<final_va; i+=PAGE_SIZE)
		{
			uint32 *ptr_page_table =NULL;
			int ret = get_page_table(page_directory,i, &ptr_page_table);
			if(ret == TABLE_NOT_EXIST)
			{
				frames+=1;
			}
			else{
				struct FrameInfo *ptr_frame_info = get_frame_info(page_directory,i,&ptr_page_table);
				if(ptr_frame_info == NULL)
				{
					frames+=1;
				}
			}
		}

		//frames = num_of_pages + frames;

		return frames;

}

//=================================================================================//
//===========================END RAM CHUNKS MANIPULATION ==========================//
//=================================================================================//

/*******************************/
/*[2] USER CHUNKS MANIPULATION */
/*******************************/

//======================================================
/// functions used for USER HEAP (malloc, free, ...)
//======================================================

//=====================================
// 1) ALLOCATE USER MEMORY:
//=====================================
void allocate_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	// Write your code here, remove the panic and write your code
	panic("allocate_user_mem() is not implemented yet...!!");
}

//=====================================
// 2) FREE USER MEMORY:
//=====================================
void free_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3] [USER HEAP - KERNEL SIDE] free_user_mem
	// Write your code here, remove the panic and write your code
	uint32 s=size/PAGE_SIZE;
	uint32 vir=virtual_address;
	for(int i=0; i<s;i++)
	{
		//1. Free ALL pages of the given range from the Page File
		pf_remove_env_page(e,vir);
		vir =vir+PAGE_SIZE;
	}

	uint32* ptr_page_table;
	vir=virtual_address;
	for(int i=0; i<e->page_WS_max_size; i++)
	{
		//2. Free ONLY pages that are resident in the working set from the memory
		uint32 ws_va=env_page_ws_get_virtual_address(e,i);
		uint32 newsize=ROUNDUP(size+vir,PAGE_SIZE);
		if((ws_va>=vir) && (ws_va<newsize))
		{
			unmap_frame(e->env_page_directory,ws_va);
			env_page_ws_clear_entry(e,i);

		}
	}

	vir= virtual_address;
	for(int i=0; i<s; i++)
	{
		//3. Removes ONLY the empty page tables
		int check=1;
		ptr_page_table=NULL;
		get_page_table(e->env_page_directory,vir,&ptr_page_table);
		if(ptr_page_table !=NULL)
		{
			for(int j=0; j<1024; j++)
			{
				if(ptr_page_table[j]!=0)
				{
					check=0;
					break;
				}
			}
			if(check==1)
			{
				kfree((void*)ptr_page_table);
				pd_clear_page_dir_entry(e->env_page_directory,(uint32)vir);
			}
		}
		vir=vir+PAGE_SIZE;
	}
}

//=====================================
// 2) FREE USER MEMORY (BUFFERING):
//=====================================
void __free_user_mem_with_buffering(struct Env* e, uint32 virtual_address, uint32 size)
{
	// your code is here, remove the panic and write your code
	panic("__free_user_mem_with_buffering() is not implemented yet...!!");

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Free any BUFFERED pages in the given range
	//4. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 3) MOVE USER MEMORY:
//=====================================
void move_user_mem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3 - BONUS] [USER HEAP - KERNEL SIDE] move_user_mem
	//your code is here, remove the panic and write your code
	panic("move_user_mem() is not implemented yet...!!");

	// This function should move all pages from "src_virtual_address" to "dst_virtual_address"
	// with the given size
	// After finished, the src_virtual_address must no longer be accessed/exist in either page file
	// or main memory

	/**/
}

//=================================================================================//
//========================== END USER CHUNKS MANIPULATION =========================//
//=================================================================================//

