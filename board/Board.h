#pragma once

#include "Position.h"
#include "Move.h"

class Piece;

class Board
{
private:
    Piece* board[8][8];

public:
    Board();

    Piece* getPiece(const Position& position) const;

    void setPiece(const Position& position, Piece* piece);

    bool isInside(const Position& position) const;

    bool isOccupied(const Position& position) const;

    void movePiece(const Move& move);

    void printBoard() const;
};