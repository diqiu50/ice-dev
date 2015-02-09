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
// based on the string value. 
//
// Modification History:
// 05/18/2012 Created by felicia
////////////////////////////////////////////////////////////////////////////
#ifndef AOS_Util_Sort_h
#define AOS_Util_Sort_h


class AosCompareFun
{
	public:
	int size;
	AosCompareFun(int siz):size(siz)
	{
	}
	bool operator()(const char *lhs,const  char* rhs)
	{
		return *(int*)lhs < *(int*)rhs;
	}
};

typedef AosCompareFun* AosCompareFunPtr;

#define MAXNUM  16*(*comp).size

#define change(first, last, comp) \
{\
	{\
		char temp[1000];\
		memcpy(temp, first, (*comp).size);\
		memcpy(first, last, (*comp).size);\
		memcpy(last, temp, (*comp).size);\
	}\
}
void AosSort(char* first, char* last, AosCompareFunPtr &comp);

// Thease all the internal function used by AosSort
inline char* partition(char* first,char* last,const char* pivot,AosCompareFunPtr &comp);

inline void move_first(char* a, char* b, char* c,AosCompareFunPtr &comp);

inline char* pivot(char* first,char* last, AosCompareFunPtr &comp);

inline void quick_sort(char* first, char* last, int deeplimit, AosCompareFunPtr &comp);

inline void insert(char *last, AosCompareFunPtr &comp);

inline void uninsertion_sort(char* first, char* last, AosCompareFunPtr &comp);

inline void insertion_sort(char* first, char* last, AosCompareFunPtr &comp);

inline void insert_sort(char* first, char* last, AosCompareFunPtr &comp);

inline void heap_sort(char* first, char* mid, char* last, AosCompareFunPtr &comp);

inline void sort_heap(char* first, char* last, AosCompareFunPtr &comp);

inline void pop_heap(char* first, char* last, char* result, AosCompareFunPtr &comp);

inline void heap_select(char* first, char* mid, char* last, AosCompareFunPtr &comp);

inline void make_heap(char* first, char* last, AosCompareFunPtr &comp);

inline void adjust_heap(char* first, int holeidx, int len, char* val, AosCompareFunPtr &comp);

inline void push_heap(char* first, int holeidx, int topidx, char* val, AosCompareFunPtr &comp);

#endif
