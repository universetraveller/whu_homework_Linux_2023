# Document
## Basic Principle
Use a list of structure MemoryBlock to save each block allocated for users. And use a variable to indicate the high end of the memory to simplify the free function. Once user requests a size of memory, umalloc return a MemoryBlock representing the memory. So, memory is managed via blocks (both allocate, merge and free).  

## Data structure
MemoryBlock which contains the size of itself, the flag indicating if it is allocated, pointer of the memory begin of the block, and a pointer to the next block.  
Then I keep head and tail pointers to find the blocks quickly.  
Another data structure is the memory pool mem[MAX_MEM_BYTES] and the variable _uenddata representing the high end of it.  

## Algorithms
### _ubreak
Pushing the mark of memory high end, accepts an integer, checking the range and adding it to _uenddata and then returns pointer to the high end.  

### umalloc
Using first fit algorithm, find the first block with size larger than the request. If the block exists, call _umalloc_block to process it.  
If the suitable block does not exist, check the range of the memory. If add size to the high end and the memory does not overflow, create a new block for the request and return the pointer of the memory begin of the block.  
If the memory out of range when adding parameter size, try to merge the blocks which is not allocated now. Call umerge to process that situation.  

### ufree
First, check if the parameter ptr exists in the block list. If so, unset the allocated flag. Then, if the block at the end of the block list, free this memory via reset the high end of the memory (that's time actually releasing memory). Before return from free, check if the tail of the list is not allocated. If so, call free recursively to free the blocks which is not allocated continuously at the end of block list.  

### _umalloc_block
Check if the block is larger than the requested size adding a pre-defined parameter MAX_WASTED_SIZE. If so, split this block to be block with size is requested size and the remaining part (another block). Or, if the block is not very large (compared with size), directly return this block (actually the pointer to memory it represents) for user allocation request.  

### umerge
Use first fit algorithm, find the first area (continuous blocks) satisfy the request, then merge the blocks into one block and call _umalloc_block to process it. If the area does not exist, which indicates the memory is not enough, the function returns NULL.  