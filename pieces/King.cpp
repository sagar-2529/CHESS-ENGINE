#include "King.h"
#include "../board/Board.h"

King::King(const Position& position, Color color)
    : Piece(position, color)
{
}

std::vector<Move> King::getPossibleMoves(const Board& board) const
{
    std::vector<Move> possibleMoves;

    int row = position.getRow();
    int col = position.getCol();

    int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1,  0,  1,-1, 1,-1, 0, 1};

    for (int i = 0; i < 8; i++)
    {
        int newRow = row + dr[i];
        int newCol = col + dc[i];

        Position newPosition(newRow, newCol);

        if (!board.isInside(newPosition))
        {
            continue;
        }

        if (!board.isOccupied(newPosition))
        {
            possibleMoves.push_back(Move(position, newPosition));
        }
        else
        {
            Piece* targetPiece = board.getPiece(newPosition);

            if (targetPiece->getColor() != getColor())
            {
                possibleMoves.push_back(Move(position, newPosition));
            }
        }
    }
    if(!hasMoved())
{
    possibleMoves.push_back(
        Move(position,
             Position(row, col + 2))
    );

    possibleMoves.push_back(
        Move(position,
             Position(row, col - 2))
    );
}

    return possibleMoves;
}

char King::getSymbol() const
{
    return (getColor() == Color::WHITE) ? 'K' : 'k';
}

PieceType King:: getType() const{
    return PieceType::KING;
}