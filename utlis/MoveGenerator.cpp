#include "MoveGenerator.h"

#include "../board/Board.h"
#include "../board/Move.h"
#include "../board/Position.h"
#include "../pieces/Piece.h"

void MoveGenerator::addMovesInDirection(
    const Board& board,
    const Piece& piece,
    int rowStep,
    int colStep,
    std::vector<Move>& moves)
{
    Position current = piece.getPosition();

    int row = current.getRow() + rowStep;
    int col = current.getCol() + colStep;

    while (true)
    {
        Position newPosition(row, col);

        if (!board.isInside(newPosition))
        {
            break;
        }

        if (!board.isOccupied(newPosition))
        {
            moves.push_back(Move(current, newPosition));
        }
        else
        {
            Piece* targetPiece = board.getPiece(newPosition);

            if (targetPiece->getColor() != piece.getColor())
            {
                moves.push_back(Move(current, newPosition));
            }

            break;
        }

        row += rowStep;
        col += colStep;
    }
}