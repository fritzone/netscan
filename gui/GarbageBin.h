#pragma once

#include <vector>

using namespace std;

#include "leaker.h"

/**
 * The class GarbageBin is acting as a real life Garbage bin. When you allocate
 * some objects that you might need all along the life cycle of the program, you 
 * can throw them in their respective type GarbageBin, and when your program exists
 * you simply call the empty method on the GarbageBin. This way you don't have to worry about 
 * memory leaks, when by mistake you forget a delete... 
 * This class is actually a template, so you can have different type of GarbageBins...
 * This class is implemented as a Singleton.
 * @see emptyGarbageBins in utils.cpp which must be called before your program leaves
 */
template <class T>
class CGarbageBin
{
private:

	/**
	 * Constructor. Creates a new Garbaga Bin object.
	 */
	CGarbageBin<T>(void)
	{
		items.clear();
	}

public:

	// the instance
	static CGarbageBin<T>* instance;

	/**
	 * Returns the instance of this type of garbage bin...
	 */
	static CGarbageBin<T>* getInstance()
	{
		if(instance == NULL)
		{
			instance = new CGarbageBin<T>();
		}

		return instance;
	}

	/**
	 * Deletes the instance
	 */
	static void deleteInstance()
	{
		if (instance)
		{
			delete instance;
		}
	}

	/**
	 * Destructor
	 */
	~CGarbageBin<T>(void)
	{
	}

	/**
	 * Empties the garbage bin...
	 */
	void empty(void)
	{
		for(size_t i=0; i<items.size(); i++)
		{
			delete items[i];
		}
	}

	/**
	 * Adds an item to the garbage bin
	 */
	void throwIn(T* item)
	{
		items.push_back(item);
	}

private:

	// this is the vector which actually contains the pointers that will be deleted at the end
	vector<T*> items;
};

// this is here to create the instance of the garbage bin.
template <class T> CGarbageBin<T>*  CGarbageBin<T>::instance = NULL;

