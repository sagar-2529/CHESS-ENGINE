#include "Move.h"

Move::Move()
    : from(Position(-1, -1)),
      to(Position(-1, -1))
{
}

Move::Move(const Position& from, const Position& to)
    : from(from),
      to(to)
{
}

Position Move::getFrom() const
{
    return from;
}

Position Move::getTo() const
{
    return to;
}