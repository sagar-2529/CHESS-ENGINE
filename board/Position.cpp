#include "Position.h"
using namespace std;

Position::Position()
{
    row = 0;
    col = 0;
}

Position::Position(int row, int col)
{
    this->row = row;
    this->col = col;
}

int Position::getRow() const
{
    return row;
}

int Position::getCol() const
{
    return col;
}

bool Position::isValid() const
{
    return row >= 0 &&
           row < 8 &&
           col >= 0 &&
           col < 8;
}

string Position::toChessNotation() const{
    return string(1, 'a' + col) + to_string(8 - row);
}

bool Position::operator==(const Position& other) const
{
    return row == other.row && col == other.col;
}

bool Position::operator<(const Position& other) const
{
    if (row != other.row)
        return row < other.row;

    return col < other.col;
}