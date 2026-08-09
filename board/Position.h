#pragma once

#include <string>
using namespace std;

class Position
{
private:
    int row;
    int col;

public:
    Position();
    Position(int row, int col);

    int getRow() const;
    int getCol() const;

    bool isValid() const;

    bool operator==(const Position& other) const;
    bool operator<(const Position& other) const;

    string toChessNotation() const;
};