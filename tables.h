#ifndef TABLES_H_
#define TABLES_H_

struct HeapChain
{
	struct HeapChain* next;
	void* allocation;
	size_t size;
	int tag;
};

struct Symtable
{
 	struct Symtable* next;
	char* symbol;
	char* value;
};






#endif
