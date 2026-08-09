
#include "Rook.h"
#include "../board/Board.h"
#include "../utlis/MoveGenerator.h"

Rook::Rook(const Position& position, Color color): Piece(position, color) {}


std::vector<Move> Rook::getPossibleMoves(const Board& board) const
{
      std::vector<Move> PossibleMoves;

      MoveGenerator::addMovesInDirection(board, *this, 1, 0, PossibleMoves);
      MoveGenerator::addMovesInDirection(board, *this, -1, 0, PossibleMoves);
      MoveGenerator::addMovesInDirection(board, *this, 0, 1, PossibleMoves);
      MoveGenerator::addMovesInDirection(board, *this, 0, -1, PossibleMoves);

       
      return PossibleMoves;
}

char Rook::getSymbol() const
{
    return (getColor() == Color::WHITE) ? 'R' : 'r';
}

PieceType Rook:: getType() const{
    return PieceType::ROOK;
}