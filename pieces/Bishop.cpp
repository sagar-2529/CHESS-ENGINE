

#include "Bishop.h"
#include "../board/Board.h"
#include "../utlis/MoveGenerator.h"

Bishop :: Bishop(const Position& position, Color color) : Piece(position, color) {}


std:: vector<Move> Bishop::getPossibleMoves(const Board& board) const
{
   std:: vector<Move> possibleMoves;
    MoveGenerator::addMovesInDirection(board, *this, 1, 1, possibleMoves);
    MoveGenerator::addMovesInDirection(board, *this, 1, -1, possibleMoves);
    MoveGenerator::addMovesInDirection(board, *this, -1, 1, possibleMoves);
    MoveGenerator::addMovesInDirection(board, *this, -1, -1, possibleMoves);
    return possibleMoves;
}


char Bishop::getSymbol() const
{
    return (getColor() == Color::WHITE) ? 'B' : 'b';
}


PieceType Bishop:: getType() const{
    return PieceType::BISHOP;
}


