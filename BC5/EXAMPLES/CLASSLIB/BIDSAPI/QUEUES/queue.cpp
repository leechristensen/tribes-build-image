// ---------------------------------------------------------------------------
// Copyright (C) 1994 Borland International
// queue.cpp
//    Must link with myclass.cpp.
// ---------------------------------------------------------------------------

#include <classlib/queues.h>
#include <iostream.h>
#include <strstrea.h>
#include "../myclass.h" 

typedef TQueueAsVector<MyClass> ContainerType;
typedef TQueueAsVectorIterator<MyClass> IteratorType;

const int MaxItems=6;

void ForEachCallBack(MyClass& mc, void* s)
{
    cout << (char*)s << mc << endl;
}

void AddItems(ContainerType& container, int numItems)
{
    for( int i=numItems; i>0; i-- )
        {
        char buf[80];
        ostrstream str( buf, sizeof(buf) );
        str << i << " hello" << ends;
        container.Put( MyClass(buf) );
        }
}

void UseForwardIterator(ContainerType& container)
{
    IteratorType iterator(container);
    while( iterator )
        {
        cout << iterator.Current() << endl;
        iterator++;
        }
}

int main()
{
    ContainerType container;
    AddItems(container, MaxItems);
  
    cout << "--- Starting ForEach" << endl;
    container.ForEach(ForEachCallBack, (void*)"FE ");
  
    cout << "--- Starting Iterator" << endl;
    UseForwardIterator(container);
  
    return 0;
}
