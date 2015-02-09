////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2005
// Packet Engineering, Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification is not permitted unless authorized in writing by a duly
// appointed officer of Packet Engineering, Inc. or its derivatives
//
// This type of IIL maintains a list of (string, docid) and is sorted
// based on the string value. It has a companion IIL that keeps the
// same set of Docids but sorted based on docids. 
//
// Modification History:
// 05/18/2012 Created by felicia
////////////////////////////////////////////////////////////////////////////
#include "Sort.h"

#include <math.h>
#include <string.h>


char* partition(char* first, char* last, const char* pivot, AosCompareFunPtr &comp)
{
	while (true)
	{
		while ((*comp)(first, pivot))
		{
			first += (*comp).size;
		}
		last -= (*comp).size;

		while ((*comp)(pivot, last))
		{
			last -= (*comp).size;
		}
		if (!(first < last))
		{
			return first;
		}
		change(first, last, comp);
		first += (*comp).size;
	}
}


void move_first(char* a, char* b, char* c, AosCompareFunPtr &comp)
{
	if ((*comp)(a, b))
	{
		// It is: a < b
		if ((*comp)(b, c))
		{
			// b < c
			change(a, b, comp);
		}
		else if ((*comp)(a, c))
		{
			// a < c
			change(a, c, comp);
		}
	}
	else if ((*comp)(a, c))
	{
		// a > b && a < c
		return;
	}
	else if ((*comp)(b, c))
	{
		change(a, c, comp);
	}
	else
	{
		change(a, b, comp);
	}
}


char* pivot(char* first, char* last, AosCompareFunPtr &comp)
{
	char* mid = first + (((last-first) / (*comp).size) >> 1) * (*comp).size;
	move_first(first, mid, (last - (*comp).size), comp);
    return partition(first + (*comp).size, last, first, comp);
}


void quick_sort(char* first, char* last, int deeplimit, AosCompareFunPtr &comp)
{
	while (last - first > MAXNUM)
	{
		if(deeplimit == 0)
		{
			heap_sort(first, last, last, comp);
			return;
		}
		
		deeplimit--;
		char* cut = pivot(first, last, comp);
		quick_sort(cut, last, deeplimit, comp);
		last = cut;
	}
}

void insert(char *last, AosCompareFunPtr &comp)
{
	char val[1000];
	memcpy(val, last, (*comp).size);
	char* next = last;
	next -= (*comp).size;
	while((*comp)(val, next))
	{
		memcpy(last, next, (*comp).size);
		last = next;
		next -= (*comp).size;
	}
	memcpy(last, val, (*comp).size);
}

void uninsertion_sort(char* first,char* last, AosCompareFunPtr &comp)
{
	for (char* i = first; i != last; i += (*comp).size)
	{
		insert(i, comp);
	}
}

void insertion_sort(char *first, char *last, AosCompareFunPtr &comp)
{
	if (first == last) return;

	for (char* i = first + (*comp).size; i != last; i += (*comp).size)
	{
		if ((*comp)(i, first))
		{
			char val[1000];
			memcpy(val, i, (*comp).size);
			memmove(first+(*comp).size, first, (i-first)*sizeof(char));
			memcpy(first, val, (*comp).size);
		}
		else
		{
			insert(i, comp);
		}
	}
}

void insert_sort(char* first, char* last, AosCompareFunPtr &comp)
{
    if (last - first > MAXNUM)
	{
	   insertion_sort(first, first + MAXNUM, comp);
	   uninsertion_sort(first + MAXNUM, last, comp);
	}
    else
	{
		insertion_sort(first, last, comp);
	}
}


void push_heap(char* first, int holeidx, int topidx, char* val, AosCompareFunPtr &comp)
{
	int parent = (holeidx - 1) / 2;
	while (holeidx > topidx && (*comp)(first + parent * (*comp).size, val))
	{
		memcpy(first + holeidx * (*comp).size, first + parent * (*comp).size, (*comp).size);
		holeidx = parent;
		parent = (holeidx - 1) / 2;
	}
	memcpy(first + holeidx * (*comp).size, val, (*comp).size);
}


void adjust_heap(char* first, int holeidx, int len, char* val, AosCompareFunPtr &comp)
{
	const int topidx = holeidx;
	int secondChild = holeidx;
	while (secondChild < (len - 1) / 2)
	{
		secondChild = 2 * (secondChild + 1);
		if ((*comp)(first + secondChild * (*comp).size, first + (secondChild - 1) * (*comp).size))
		{
			secondChild--;
		}
		memcpy(first + holeidx * (*comp).size, first + secondChild * (*comp).size, (*comp).size);
		holeidx = secondChild;
	}
	if ((len & 1) == 0 && secondChild == (len - 2) / 2)
	{
		secondChild = 2 * (secondChild + 1);
		memcpy(first + holeidx * (*comp).size, first + (secondChild - 1) * (*comp).size, (*comp).size);
		holeidx = secondChild - 1;
	}
	push_heap(first, holeidx, topidx, val, comp);
}


void make_heap(char* first, char* last, AosCompareFunPtr &comp)
{
	const int len = (last - first) / (*comp).size;
	if (len < 2) return;

	int holeidx = ((len - 2) / 2);
	while (true)
	{
		char val[1000];
		memcpy(val, first + holeidx * (*comp).size, (*comp).size);
		adjust_heap(first, holeidx, len, val, comp);
		
		if (holeidx == 0) return;
		holeidx--;
	}
}


void heap_select(char* first, char* mid, char* last, AosCompareFunPtr &comp)
{
	make_heap(first, mid, comp);
	for (char * i = mid; i != last; i += (*comp).size)
	{
		if ((*comp)(i, first))
		{
			pop_heap(first, mid, i, comp);
		}
	}
}


void pop_heap(char* first, char* last, char* result, AosCompareFunPtr &comp)
{
	char val[1000];
	memcpy(val, result, (*comp).size);
	memcpy(result, first, (*comp).size);
	int len = (last - first) / (*comp).size;
	adjust_heap(first, 0, len, val, comp);
}


void sort_heap(char* first, char* last, AosCompareFunPtr &comp)
{                                                                     
	while (last - first > (*comp).size)
	{
		last -= (*comp).size;
		pop_heap(first, last, last, comp);
	}
}


void heap_sort(char* first, char* mid, char* last, AosCompareFunPtr &comp)
{
	heap_select(first, mid, last, comp);
	sort_heap(first, mid, comp);
}


void AosSort(char* first, char* last, AosCompareFunPtr &comp)
{
	if (first != last)
	{
		quick_sort(first, last, (int)(log2((last-first) / comp->size)) * 2, comp);
		insert_sort(first, last, comp);
	}
}
