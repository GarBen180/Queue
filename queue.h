/***********************************************************************
* Header:
*    Queue
* Summary:
*    This class contains the notion of a Queue: a bucket to hold
*    data for the user, which accesses the front item, adds to the back.
*
*    This will contain the class definition of:
*        Queue         : A class that holds stuff
* Author
*    Garrett Bennett
************************************************************************/

#ifndef Queue_H
#define Queue_H

#include <cassert>


/************************************************
 * Queue
 * A class that holds stuff
 ***********************************************/
template <class T>
class Queue
{
public:
   // default constructor : empty and kinda useless
	Queue() : numItems(0), cap(0), frontItem(0), backItem(0), data(NULL) {}

   // copy constructor : copy it
   Queue(const Queue & rhs) throw (const char *);
   
   // non-default constructor : pre-allocate
   Queue(int capacity) throw (const char *);
   
   // destructor : free everything
   ~Queue() { if (cap) delete [] data; }
   
   // is the Queue currently empty
   bool empty() const { return (frontItem == 0 && backItem == 0); }

   // remove all the items from the Queue
	void clear() { frontItem = 0; backItem = 0; }

   // how many items are currently in the Queue?
   int size() const { return numItems; }

	// gets the capacity of the Queue
	int capacity() { return cap; }

	// the push function!!!
	void push(T item);

	// the pop function
	void pop() throw(const char *);

	// the front function
	T front() throw(const char *);

	// the back function
	T back() throw(const char *);

	// the = operator overload
	Queue<T>& operator=(const Queue<T> & rhs);
   

   
private:
   T * data;            // dynamically allocated array of T
   int numItems;        // how many items are currently in the Queue?
   int cap;             // how many items can I put on the Queue before full?
	int frontItem;       // position of the front of the queue
	int backItem;        // position of the back of the queue
};



/*******************************************
 * Queue :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
Queue <T> :: Queue(const Queue <T> & rhs) throw (const char *)
{
   assert(rhs.cap >= 0);
      
   // do nothing if there is nothing to do
   if (rhs.cap == 0)
   {
      cap = 0;
		numItems = 0;
		frontItem = 0;
		backItem = 0;
      data = NULL;
      return;
   }

   // attempt to allocate
   try
   {
      data = new T[rhs.cap];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }
   
   // copy over the capacity and numItems
   assert(rhs.numItems >= 0 && rhs.numItems <= rhs.cap);
   cap = rhs.cap;
   numItems = rhs.numItems;
	
   // copy the items over one at a time using the assignment operator
   for (int i = 0; i < numItems; i++)
      data[i] = rhs.data[i];

	frontItem = rhs.frontItem;
	backItem = rhs.backItem;

   // the rest needs to be filled with the default value for T
   for (int i = numItems; i < cap; i++)
      data[i] = T();
}

/**********************************************
 * Queue : NON-DEFAULT CONSTRUCTOR
 * Preallocate the Queue to "capacity"
 **********************************************/
template <class T>
Queue <T> :: Queue(int capacity) throw (const char *)
{
   assert(capacity >= 0);
   
   // do nothing if there is nothing to do
   if (capacity == 0)
   {
      this->cap = this->numItems = 0;
		this->frontItem = this->backItem = 0;
      this->data = NULL;
      return;
   }

   // attempt to allocate
   try
   {
      data = new T[capacity];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }

      
   // copy over the stuff
   this->cap = capacity;
   this->numItems = 0;
	this->frontItem = 0;
	this->backItem = 0;

   // initialize the Queue by calling the default constructor
   for (int i = 0; i < capacity; i++)
      data[i] = T();
}


/***************************************************
* Queue :: push
* The push operation of a queue
**************************************************/
template <class T>
void Queue <T> :: push(T item)
{
	if (numItems < cap && ((backItem+1)%cap)>0) {
		data[backItem] = item;
		backItem = (backItem + 1) % cap;
		numItems++;
	}
	else {
		if (cap == 0)
		{
			cap = 1;
			data = new T[cap];
		}
		else {
			T* holder = new T[cap * 2];

			int j = frontItem;
			for (int i = 0; i < numItems; i++, j = (j + 1) % cap)
			{
				holder[i] = data[j];
			}

			delete[] data;
			data = holder;
			frontItem = 0;
			backItem = numItems;
			cap *= 2;
		}
		push(item);
	}
	return;	
}

/***************************************************
* Queue :: pop
* Removes the first item placed in the Queue
**************************************************/
template<class T>
void Queue<T>::pop() throw (const char *)
{
	if (!empty()) {
		frontItem = (frontItem + 1) % cap;
		numItems--;
	}
	else
		throw "ERROR: Unable to pop from an empty Queue"; 
}

/***************************************************
* Queue :: numItems
* Gets the front of the queue
**************************************************/
template<class T>
T Queue<T>::front() throw (const char *)
{
	if (!empty())
		return data[frontItem];
	else {
		throw "ERROR: Unable to reference the element from an empty Queue";
	}		
}

/***************************************************
* Queue :: back
* Gets the back of the queue
**************************************************/
template<class T>
inline T Queue<T>::back() throw(const char *)
{
	if(!empty())
		return data[backItem];
	else {
		throw "ERROR: Unable to reference the element from an empty Queue";
	}
}

/***************************************************
* Queue :: = operator overload
* Make stuff equal.
**************************************************/
template<class T>
Queue<T>& Queue<T>::operator=(const Queue<T>& rhs)
{
	if (this == &rhs)
		return *this;

	if (rhs.cap > cap) {
		cap = rhs.cap;
		delete[] data;
		data = new T[cap];			
	}

	int j = rhs.frontItem;
	for (int i = 0; i < rhs.numItems; i++, j = (j + 1) % cap)
	{
		data[i] = rhs.data[j];
	}

	frontItem = 0;
	numItems = rhs.numItems;
	cap = rhs.cap;
	backItem = numItems;

	return *this;
}



#endif // Queue_H

