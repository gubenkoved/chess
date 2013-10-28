#ifndef HELPERS_H
#define HELPERS_H

#include <QList>
#include "lightfigureposition.h"

class Helpers
{
public:
    template<class T> static void AppendIfNotNull(QList<T>& list, const T& value);
    template<class T> static void AppendIfNotExists(QList<T>& list, const T& value);
    template<class T> static void AppendIfNotExists(QList<T>& list, const QList<T>& values);
    static void AppendIfValid(PositionList& list, const POSITION& position);
    static void AppendIfValid(PositionList& list, const PositionList& positions);
    static void AppendIfValidAndNotExists(PositionList& list, const POSITION& position);
    static void AppendIfValidAndNotExists(PositionList& list, const PositionList& positions);
};

template<class T> void Helpers::AppendIfNotNull(QList<T>& list, const T& value)
{
    if (value != NULL)
    {
        list.append(value);
    }
}

template<class T> void Helpers::AppendIfNotExists(QList<T>& list, const T& value)
{
    if (!list.contains(value))
    {
        list.append(value);
    }
}

template<class T> void Helpers::AppendIfNotExists(QList<T>& list, const QList<T>& values)
{
    foreach(T value, values)
    {
        AppendIfNotExists(list, value);
    }
}


#endif // HELPERS_H
