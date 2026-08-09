#include "Queen.h"
#include "../utlis/MoveGenerator.h"
#include "../board/Board.h"

Queen::Queen(const Position& position, Color color)
    : Piece(position, color)
{
}

std::vector<Move> Queen::getPossibleMoves(const Board& board) const
{
    std::vector<Move> possibleMoves;

    // Rook directions
    MoveGenerator::addMovesInDirection(board, *this, 1, 0, possibleMoves);
    MoveGenerator::addMovesInDirection(board, *this, -1, 0, possibleMoves);
    MoveGenerator::addMovesInDirection(board, *this, 0, 1, possibleMoves);
    MoveGenerator::addMovesInDirection(board, *this, 0, -1, possibleMoves);

    // Bishop directions
    MoveGenerator::addMovesInDirection(board, *this, 1, 1, possibleMoves);
    MoveGenerator::addMovesInDirection(board, *this, 1, -1, possibleMoves);
    MoveGenerator::addMovesInDirection(board, *this, -1, 1, possibleMoves);
    MoveGenerator::addMovesInDirection(board, *this, -1, -1, possibleMoves);

    return possibleMoves;
}

char Queen::getSymbol() const
{
    return (getColor() == Color::WHITE) ? 'Q' : 'q';
}

PieceType Queen:: getType() const{
    return PieceType::QUEEN;
}