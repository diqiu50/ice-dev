////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2005
// Packet Engineering, Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification is not permitted unless authorized in writing by a duly
// appointed officer of Packet Engineering, Inc. or its derivatives
//
// Modification History:
// 12/13/2012 Created by Ice
//	struct T_Data_Gen {
//		T_Data getData(T_Id id);
//		T_Data getData(T_Id id, T_Data old_data);
//	};
//
////////////////////////////////////////////////////////////////////////////
#ifndef LRUCache_h
#define LRUCache_h

#include <list>
#include <map>
using namespace std;

template<typename T_Id, typename T_Data, int T_Size=30>
class LRUCache
{
	typedef typename list<pair<T_Id, T_Data> >::iterator 		T_MapValueType;
	typedef typename map<T_Id, T_MapValueType>::iterator 		T_MapItrType;

	//OmnMutexPtr 				mLock;
	list<pair<T_Id, T_Data> >	mCache;
	map<T_Id, T_MapValueType> 	mMap;
	int 						mCached;
	int 						mTotal;

public:
	LRUCache():mCached(0), mTotal(0){}

	template <typename T_DataGen>
	T_Data& get(T_Id id, T_DataGen &dataGen)
	{
		T_Data data;
		//mLock->lock();

		T_MapItrType mapitr = mMap.find(id);
		bool gen_data = false;
		if (mapitr == mMap.end())
		{
			if (mCache.size() < T_Size)
			{
				data = dataGen.getData(id);
				gen_data = true;
			}
			mapitr = (mMap.insert(make_pair<const T_Id&, T_MapValueType>(id, mCache.end()))).first;
		}
		else
		{
			mCached++;
			data = (*(mapitr->second)).second;
			mCache.erase(mapitr->second);
		}

		if (mCache.size() == T_Size)
		{
			pair<T_Id, T_Data> oldentry = mCache.back();
			T_MapItrType mapitrold = mMap.find(oldentry.first);
			mCache.erase(mapitrold->second);
			mMap.erase(mapitrold);
			if (!gen_data)
			{
				data = dataGen.getData(id, oldentry.first, oldentry.second);
			}
		}
		mCache.push_front(make_pair(id, data));
		mapitr->second = mCache.begin();

		mTotal++;
		//mLock->unlock();
		return mCache.front().second;
	}

	void remove(T_Id id) {
		//mLock->lock();
		T_MapItrType mapitr = mMap.find(id);
		if (mapitr == mMap.end()) return;
		mCache.erase(mapitr->second);
		mMap.erase(mapitr);
		//mLock->unlock();
	}

	T_Data& insert(T_Id id, T_Data &data)
	{
		//mLock->lock();
		T_Data tmpdata;
		T_MapItrType mapitr = mMap.find(id);
		if (mapitr != mMap.end())
		{
			mCache.erase(mapitr->second);
			mMap.erase(mapitr);
		}

		if (mCache.size() == T_Size)
		{
			pair<T_Id, T_Data> oldentry = mCache.back();
			tmpdata = oldentry.second;
			T_MapItrType mapitrold = mMap.find(oldentry.first);
			mCache.erase(mapitrold->second);
			mMap.erase(mapitrold);
		}
		mCache.push_front(make_pair(id, data));
		mMap.insert(make_pair<const T_Id&, T_MapValueType>(id, mCache.begin()));
		mTotal++;
		//mLock->unlock();
		return tmpdata;
	}

	double hitRate() {return (double)mCached/(mTotal+0.0001);}

	list<pair<T_Id, T_Data> > copy(){
		//mLock->lock();
		list<pair<T_Id, T_Data> > v = mCache;
		//mLock->unlock();
		return v;
	}
};
#endif

