#include <stdlib.h>
#include <stdio.h>
#define MAX_MEM_BYTES 8192
#define MAX_WASTED_SIZE 128
#define _UDEBUG

static char mem[MAX_MEM_BYTES];

struct MemoryBlock{
	size_t size;
	int allocated;
	void* ptr;
	struct MemoryBlock* next;
};

struct MemoryBlock* _blocks_head = NULL;
struct MemoryBlock* _blocks_tail = NULL;

size_t _uenddata = 0;

void* _ubreak(int dist);

void* umalloc(size_t size);

void ufree(void* ptr);

void* umerge(size_t target);
