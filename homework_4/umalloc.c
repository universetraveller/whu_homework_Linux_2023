#include "umalloc.h"


void* _ubreak(int dist){
	if(_uenddata + dist < 0){
		perror("Address out of range: lower than 0\n");
		return NULL;
	}
	if(_uenddata + dist < MAX_MEM_BYTES){
		_uenddata += dist;
		return mem + _uenddata;
	}
	perror("Address out of range: higher than MAX_MEM_BYTES\n");
	printf("MAX_MEM_BYTES: %d\n", MAX_MEM_BYTES);
	return NULL;
}

struct MemoryBlock* _create_block(size_t size, int allocated, void* ptr){
	struct MemoryBlock* block = (struct MemoryBlock*)malloc(sizeof(struct MemoryBlock));
	block -> size = size;
	block -> allocated = allocated;
	block -> ptr = ptr;
	block -> next = NULL;
	return block;
}

struct MemoryBlock* _find_first_fit(size_t size){
	struct MemoryBlock* p = _blocks_head;
	while(p != NULL){
		if(p -> allocated || p -> size < size){
			p = p -> next;
			continue;
		}
		break;
	}
	return p;
}

void* _umalloc_block(struct MemoryBlock* block, size_t size){
	if(block -> size > MAX_WASTED_SIZE + size){
		// split the block when it is too large
		struct MemoryBlock* next_of_block = block -> next;
		struct MemoryBlock* new_block = _create_block(block -> size - size, 0, block -> ptr + size);
		new_block -> next = next_of_block;
		block -> next = new_block;
		block -> size = size;
	}
	block -> allocated = 1;
	return block -> ptr;
}

struct MemoryBlock* _next_allocation(struct MemoryBlock* now, int allocation){
	struct MemoryBlock* p = now;
	while(p != NULL){
		if(p -> allocated == allocation)
			break;
		p = p -> next;
	}
	return p;
}

void* umerge(size_t target){
	// still use first fit blocks algorithm
	struct MemoryBlock* begin = _next_allocation(_blocks_head, 0);
	struct MemoryBlock* p = begin == NULL ? NULL : begin -> next;
	size_t now_size = begin == NULL ? 0 : begin -> size;
	while(p != NULL){
		if(p -> allocated){
			begin = _next_allocation(p, 0);
			if(begin == NULL)
				break;
			p = begin -> next;
			now_size = begin -> size;
			continue;
		}
		now_size += p -> size;
		if(now_size < target){
			p = p -> next;
			continue;
		}
		break;
	}
	if(p == NULL || begin == NULL){
		perror("Memory is not enough\n");
		return NULL;
	}
	struct MemoryBlock* merged = _create_block(now_size, 0, begin -> ptr);
	merged -> next = p -> next;
	if(begin == _blocks_head){
		_blocks_head = merged;
	}else{
		struct MemoryBlock* pre = _blocks_head;
		while(pre -> next != begin)
			pre = pre -> next;
		pre -> next = merged;
	}
	return _umalloc_block(merged, target);
}

void* umalloc(size_t size){
	// find from existing free blocks
	struct MemoryBlock* block;
	block = _find_first_fit(size);
	if(block != NULL)
		return _umalloc_block(block, size);
	// push the mark of high memory
	if(_uenddata + size < MAX_MEM_BYTES){
		block = _create_block(size, 1, _ubreak(0));
		_ubreak(size);
		// low efficiency to judge if list head is null
		if(_blocks_head == NULL)
			_blocks_head = block;
		else
			_blocks_tail -> next = block;
		_blocks_tail = block;
		return block -> ptr;
	}
	// merge the fragments
	return umerge(size);
}

void ufree(void* ptr){
	struct MemoryBlock* p = _blocks_head;
	while(p != NULL){
		if(p -> ptr == ptr)
			break;
		p = p -> next;
	}
	if(p == NULL){
		perror("No memory block match the pointer\n");
		return;
	}
	p -> allocated = 0;
	if(p == _blocks_tail){
		if(p == _blocks_head){
			_blocks_head = NULL;
			_blocks_tail = NULL;
		}else{
			// find the previous block for actually release memory
			// Complexity if O(n) but we do not need to use pre pointer in the struct MemoryBlock
			// Besides, this loop is executed only when p is at the end of the block list
			struct MemoryBlock* pre = _blocks_head;
			while(pre -> next != p)
				pre = pre -> next;
			_blocks_tail = pre;
			pre -> next = NULL;
		}
		_ubreak(-(p -> size));
	}
	// patch for test case 6
	// when free last block, check if its previous block can be freed
	if(_blocks_tail != NULL && !(_blocks_tail -> allocated))
		ufree(_blocks_tail -> ptr);
}
