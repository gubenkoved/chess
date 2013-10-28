#ifndef LIGHTCOLLECTION_H
#define LIGHTCOLLECTION_H

#include "typedefs.h"

#define DEFAULT_POSITION_LIST_CAPACITY 10

template <class T> class LightCollection
{

public:

    INT32 Capacity;
    INT32 Num;
    T* Items;

    LightCollection(INT32 capacity = DEFAULT_POSITION_LIST_CAPACITY);

    void Add(T item);
    void AddIfNotExists(T item);
};

typedef LightCollection<POSITION> LightPositionCollection;
//typedef LightCollection<MOVE> LightMoveCollection;

#endif // LIGHTCOLLECTION_H
