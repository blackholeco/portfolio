/*
 *	LinkedList.h by Chris Allen

 *	This file is provided "as-is", for the sole purpose of a demonstration of my
	work.  It is not intended to be copied or used in an external or third-party
	project, and no support will be given for that use.

 *	You may not use or copy this file, in whole or in part, to use for your own
	projects.  All rights reserved over this file.
 */

#pragma once

#include <memory>

using namespace std;

template <class T>
class LinkedList;

template <class T>
class LinkedListNode
{
public:
	LinkedListNode()
	{
		prevnode = nextnode = nullptr;
		nodeitem = make_shared<T>();
	}

	LinkedListNode(T& item, LinkedListNode<T>* prev)
	{
		nodeitem.reset(&item);

		prevnode = prev;

		if(prevnode != nullptr)
			prevnode->nextnode = this;

		nextnode = nullptr;
	}
	
	LinkedListNode(shared_ptr<T> item, LinkedListNode* prev)
	{
		nodeitem = item;

		prevnode = prev;

		if(prevnode)
			prevnode->nextnode = this;

		nextnode = nullptr;
	}

	~LinkedListNode()
	{
		nodeitem.reset();
	}

	void operator =(LinkedListNode<T>& node)
	{
		prevnode = node.prevnode;
		nextnode = node.nextnode;

		nodeitem.reset(node.nodeitem);
	}

	LinkedListNode<T>* nextNode()
	{
		return nextnode;
	}

	LinkedListNode<T>* prevNode()
	{
		return prevnode;
	}

	shared_ptr<T> getNodeItem()
	{
		return nodeitem;
	}

protected:
	friend class LinkedList<T>;

	LinkedListNode<T>* prevnode;
	LinkedListNode<T>* nextnode;
	shared_ptr<T> nodeitem;
};

template <class T>
class LinkedListIter
{
public:
	LinkedListIter()
	{
		curnode = nullptr;
	}

	LinkedListIter(LinkedListNode<T>*);

	void operator=(const LinkedListIter<T>& orig)
	{
		curnode = orig.curnode;
	}

	bool operator!=(const LinkedListNode<T>* orig)
	{
		return orig != curnode;
	}

	shared_ptr<T> operator *()
	{
		if(curnode != nullptr)
			return curnode->getNodeItem();

		return make_shared<T>();
	}

	void operator++()
	{
		if(curnode)
			curnode = curnode->nextNode();
	}

	void operator++(int)
	{
		if(curnode)
			curnode = curnode->nextNode();
	}

	void operator--()
	{
		if(curnode)
			curnode = curnode->prevNode();
	}

	T* nextNodeItem() const;
	T* prevNodeItem() const;

protected:
	friend class LinkedList<T>;
	LinkedListNode<T>* curnode;
};

template <class T>
class LinkedList
{
public:
	LinkedList()
	{
		listlength = 0;
		maxitems = 5;

		first = last = nullptr;
	};

	~LinkedList()
	{
		removeAllEntries();
	}
	
	void setMaxItems(int max)
	{
		maxitems = max;
	}

	int getMaxItems() const
	{
		return maxitems;
	}

	int findEntry(int itemid, shared_ptr<T> ret = make_shared<T>())
	{
		LinkedListIter<T> iterator = start();

		for(int i = 0; 
			iterator != nullptr; 
			iterator++, i++)
		{
			if(*((*iterator)) == itemid)
			{
				if(ret.get())
					ret = (*iterator);

				return i;
			}
		}

		return -1;
	}

	/**
	 *	Needs work
	 */
	bool removeEntry(int itmnum)
	{
		LinkedListNode<T>* curnode = nullptr;

		if(listlength == 0)
			return false;

		if(itmnum < listlength)
		{
			for(int i = 0; i <= itmnum; i++)
			{
				if(i == 0)
					curnode = first;
				else
					curnode = curnode->nextNode();
			}

			assert(curnode);

			if(curnode == first)
				first = curnode->nextNode();
			else if(curnode == last)
				last = curnode->prevNode();
			
			if(curnode->nextnode != nullptr)
				curnode->nextnode->prevnode = curnode->prevnode;

			if(curnode->prevnode != nullptr)
				curnode->prevnode->nextnode = curnode->nextnode;

			delete curnode;

			curnode = nullptr;

			listlength--;

			return true;
		}

		return false;
	}

	void removeAllEntries()
	{
		if(listlength == 0)
			return;

		LinkedListNode<T>* tmp = last;
		LinkedListNode<T>* nextone = last->prevnode;

		while(tmp != nullptr)
		{
			delete tmp;

			tmp = nextone;
		}

		listlength = 0;

		last = first = nullptr;
	}

	shared_ptr<T> itemAt(unsigned int ref)
	{
		LinkedListNode<T>* curnode = nullptr;

		if(ref < listlength)
		{
			curnode = first;

			for(int i = 1; i <= ref; i++)
				curnode = curnode->nextNode();

			return curnode->getNodeItem();
		}

		return nullptr;
	}

	int addEntry()
	{
		return addEntry(make_shared<T>());
	}

	int addEntry(T& newentry)
	{
		LinkedListNode<T>* newnode = nullptr;

		if(maxitems != 0 || listlength < maxitems)
		{
			if(first == nullptr)
			{
				newnode = new LinkedListNode<T>(newentry, nullptr);
				assert(newnode);
				
				first = newnode;
			}
			else
				newnode = new LinkedListNode<T>(newentry, last);

			assert(newnode);

			last = newnode;
			listlength++;

			return listlength -1;
		}
		else
			return -1;
	}

	int addEntry(shared_ptr<T> newentry)
	{
		LinkedListNode<T>* newnode = nullptr;

		if(maxitems != 0 || listlength < maxitems)
		{
			if(first == nullptr)
			{
				newnode = new LinkedListNode<T>(newentry, nullptr);
				assert(newnode);
				
				first = newnode;
			}
			else
				newnode = new LinkedListNode<T>(newentry, last);

			assert(newnode);

			last = newnode;
			listlength++;

			return listlength -1;
		}
		else
			return -1;
	}

	int length() const
	{
		return listlength;
	}

	LinkedListIter<T> start()
	{
		LinkedListIter<T> ret;

		ret.curnode = first;

		return ret;
	}
	
protected:
	friend class LinkedListIter<T>;
	int listlength;
	int maxitems;

	LinkedListNode<T>* first;
	LinkedListNode<T>* last;
};

