# pragma once

#include "Piece.h"


class Pawn : public Piece {
public:
    Pawn(const Position& position, Color color);
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    char getSymbol() const override;
    std::vector<Position> getAttackedPositions(
        const Board& board) const override;
        PieceType getType() const override;
};