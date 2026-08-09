# pragma once

#include <string>
# include "Position.h"

using namespace std;

class Move{

    private :
    Position from;
    Position to;

    public: 
    Move();
    Move(const Position& from, const Position& to);

    Position getFrom() const;
    Position getTo() const;

    string toString() const;
};
