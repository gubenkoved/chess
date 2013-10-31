#ifndef HELPERS_H
#define HELPERS_H

#include <QList>

#include "lightfigureposition.h"

class Helpers
{
public:
    template<typename Container, typename T> static inline void AppendIfNotNull(Container& container, const T& value);
    template<typename Container, typename T> static inline void AppendIfNotExists(Container& container, const T& value);
    template<typename T, template<typename T> class Container > static inline void AppendIfNotExists(Container<T>& container, const Container<T>& values);

    static void AppendIfValid(PositionList& list, const POSITION& position);
    static void AppendIfValid(PositionList& list, const PositionList& positions);
    static void AppendIfValidAndNotExists(PositionList& list, const POSITION& position);
    static void AppendIfValidAndNotExists(PositionList& list, const PositionList& positions);
};

template<typename Container, typename T> void Helpers::AppendIfNotNull(Container& container, const T& value)
{
    if (value != NULL)
    {
        container.append(value);
    }
}

template<typename Container, typename T> void Helpers::AppendIfNotExists(Container& container, const T& value)
{
    if (!container.contains(value))
    {
        container.append(value);
    }
}

template<typename T, template<typename T> class Container> void Helpers::AppendIfNotExists(Container<T>& container, const Container<T>& values)
{
    foreach(T value, values)
    {
        AppendIfNotExists(container, value);
    }
}

#endif // HELPERS_H
