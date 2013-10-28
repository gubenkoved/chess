#include "helpers.h"


void Helpers::AppendIfValid(PositionList& list, const PositionList& positions)
{
    foreach(POSITION p, positions)
    {
        AppendIfValid(list, p);
    }
}

void Helpers::AppendIfValid(PositionList& list, const POSITION& position)
{
    if (!IsInvalid(position))
    {
        list.append(position);
    }
}

void Helpers::AppendIfValidAndNotExists(PositionList& list, const PositionList& positions)
{
    foreach(POSITION p, positions)
    {
        AppendIfValidAndNotExists(list, p);
    }
}

void Helpers::AppendIfValidAndNotExists(PositionList& list, const POSITION& position)
{
    if (!IsInvalid(position))
    {
        AppendIfNotExists(list, position);
    }
}
