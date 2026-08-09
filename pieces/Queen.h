# pragma once

#include "Piece.h"


class Queen : public Piece {
public:
    Queen(const Position& position, Color color);
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    char getSymbol() const override;
    PieceType getType() const override;
};