/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    Joel Jossie, Gergo Medveczky
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h"

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

/*************************************************
 * P QUEUE
 * Create a priority queue.
 *************************************************/
template<class T>
class priority_queue
{
   friend class ::TestPQueue; // give the unit test class access to the privates
   template <class TT>
   friend void swap(priority_queue<TT>& lhs, priority_queue<TT>& rhs);
public:

   //
   // construct
   //
   priority_queue() = default;
   priority_queue(const priority_queue &  rhs) : container(rhs.container)            {}
   priority_queue(priority_queue && rhs)       : container(std::move(rhs.container)) {}
   template <class Iterator>
   priority_queue(Iterator first, Iterator last) 
   {
      container.reserve(last - first);
      auto it = first;
      while(it != last)
      {
         container.push_back(*it);
         it++;
      }
   }
   explicit priority_queue (custom::vector<T> && rhs) : container(rhs) {}
   explicit priority_queue (custom::vector<T> &  rhs) : container(rhs) {}
  ~priority_queue()                                                    {}

   //
   // Access
   //
   const T & top() const;

   //
   // Insert
   //
   void  push(const T& t);
   void  push(T&& t);     

   //
   // Remove
   //
   void  pop(); 

   //
   // Status
   //
   size_t size() const { return container.size();  }
   bool empty()  const { return container.empty(); }
   
private:

   bool percolateDown(size_t indexHeap);      // fix heap from index down. This is a heap index!

   custom::vector<T> container;

   T & containerAt(size_t indexQueue) {
      return container[indexQueue - 1];
   }

};

/************************************************
 * P QUEUE :: TOP
 * Get the maximum item from the heap: the top item.
 ***********************************************/
template <class T>
const T & priority_queue <T> :: top() const
{
   if(container.empty())
      throw "std:out_of_range";
   return container.front();
}

/**********************************************
 * P QUEUE :: POP
 * Delete the top item from the heap.
 **********************************************/
template <class T>
void priority_queue <T> :: pop()
{
   if(size() > 0)
   {
      std::swap(containerAt(1), containerAt(size()));
      container.pop_back();
      percolateDown(1);
   }
}

/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/
template <class T>
void priority_queue <T> :: push(const T & t)
{
   container.push_back(t);
   auto parentIndex = container.size() / 2;
   while(parentIndex && percolateDown(parentIndex))
      parentIndex /= 2;
}

template <class T>
void priority_queue <T> :: push(T && t)
{
   container.push_back(std::move(t));
   auto parentIndex = container.size() / 2;
   while(parentIndex && percolateDown(parentIndex))
      parentIndex /= 2;
}

/************************************************
 * P QUEUE :: PERCOLATE DOWN
 * The item at the passed index may be out of heap
 * order. Take care of that little detail!
 * Return TRUE if anything changed.
 ************************************************/
template <class T>
bool priority_queue <T> :: percolateDown(size_t indexHeap)
{
   auto indexLeft = indexHeap * 2;
   auto indexRight = indexLeft + 1;
   size_t indexBigger;

   if (indexRight <= size() && containerAt(indexLeft) < containerAt(indexRight))
      indexBigger = indexRight;
   else
      indexBigger = indexLeft;

   if(indexBigger <= size() && containerAt(indexHeap) < containerAt(indexBigger))
   {
      std::swap(containerAt(indexHeap), containerAt(indexBigger));
      percolateDown(indexBigger);
      return true;
   }
   return false;
}


/************************************************
 * SWAP
 * Swap the contents of two priority queues
 ************************************************/
template <class T>
inline void swap(custom::priority_queue <T>& lhs,
                 custom::priority_queue <T>& rhs) { std::swap(lhs.container, rhs.container); }

};

