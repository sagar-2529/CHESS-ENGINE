#include "Piece.h"
#include"../board/Move.h"

Piece::Piece(const Position& position, Color color)
    : position(position),
      color(color),
      captured(false),
      hasMovedFlag(false)
{
}

Position Piece:: getPosition() const{
    return position;
}

Color Piece:: getColor() const{
    return color;
}

void Piece:: setPosition(const Position& position){
    this -> position = position;
}

bool Piece::hasMoved() const {
    return hasMovedFlag;
}

void Piece::setHasMoved(bool moved) {
    hasMovedFlag = moved;
}

bool Piece:: isCaptured() const{
    return captured;
}

void Piece:: setCaptured(bool captured){
    this -> captured = captured;
}

std::vector<Position> Piece::getAttackedPositions(
    const Board& board) const
{
    std::vector<Position> attackedPositions;

    std::vector<Move> moves = getPossibleMoves(board);

    for (const Move& move : moves)
    {
        attackedPositions.push_back(move.getTo());
    }

    return attackedPositions;
}












