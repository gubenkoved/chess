#include "lightcollection.h"
#include "exception.h"
#include <vector>

template <class T> LightCollection<T>::LightCollection(INT32 capacity)
{
    this->Capacity = capacity;
    this->Num = 0;
    this->items = new T[capacity];
}

template <class T> void LightCollection<T>::Add(T item)
{
    if (this->Num == this->Capacity - 1)
    {
        throw new Exception("Positions list is full.");
    }

    *(this->Items + this->Num) = item;
}

template <class T> void LightCollection<T>::AddIfNotExists(T item)
{
    if (this->Num == this->Capacity - 1)
    {
        throw new Exception("Positions list is full.");
    }

    *(this->Items + this->Num) = item;
}
