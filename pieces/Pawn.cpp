#include "Pawn.h"
#include "../board/Board.h"

Pawn::Pawn(const Position& position, Color color)
    : Piece(position, color)
{
}

std::vector<Move> Pawn::getPossibleMoves(const Board& board) const
{
    std::vector<Move> possibleMoves;

    int row = getPosition().getRow();
    int col = getPosition().getCol();

    int direction = (getColor() == Color::WHITE) ? -1 : 1;
    int startRow = (getColor() == Color::WHITE) ? 6 : 1;

    Position oneStep(row + direction, col);

    if (board.isInside(oneStep) &&
        !board.isOccupied(oneStep))
    {
        possibleMoves.push_back(Move(getPosition(), oneStep));

        Position twoStep(row + 2 * direction, col);

        if (row == startRow &&
            board.isInside(twoStep) &&
            !board.isOccupied(twoStep))
        {
            possibleMoves.push_back(Move(getPosition(), twoStep));
        }
    }

    Position leftCapture(row + direction, col - 1);
    Position rightCapture(row + direction, col + 1);

    if (board.isInside(leftCapture) &&
        board.isOccupied(leftCapture) &&
        board.getPiece(leftCapture)->getColor() != getColor())
    {
        possibleMoves.push_back(Move(getPosition(), leftCapture));
    }

    if (board.isInside(rightCapture) &&
        board.isOccupied(rightCapture) &&
        board.getPiece(rightCapture)->getColor() != getColor())
    {
        possibleMoves.push_back(Move(getPosition(), rightCapture));
    }

    return possibleMoves;
}

std::vector<Position> Pawn::getAttackedPositions(
    const Board& board) const
{
    std::vector<Position> attackedPositions;

    int row = getPosition().getRow();
    int col = getPosition().getCol();

    int direction = (getColor() == Color::WHITE) ? -1 : 1;

    Position leftAttack(row + direction, col - 1);
    Position rightAttack(row + direction, col + 1);

    if (board.isInside(leftAttack))
    {
        attackedPositions.push_back(leftAttack);
    }

    if (board.isInside(rightAttack))
    {
        attackedPositions.push_back(rightAttack);
    }

    return attackedPositions;
}

char Pawn::getSymbol() const
{
    return (getColor() == Color::WHITE) ? 'P' : 'p';
}

PieceType Pawn:: getType() const{
    return PieceType::PAWN;
}