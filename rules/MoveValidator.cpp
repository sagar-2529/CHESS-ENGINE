#include "MoveValidator.h"
#include "../board/Board.h"
#include "../board/Move.h"
#include "../pieces/Piece.h"
#include "../enums/Color.h"
#include <iostream>

bool MoveValidator::isValidMove(Board& board, const Move& move, const Move& lastMove) const
{
    Piece* piece = board.getPiece(move.getFrom());

  if(piece != nullptr &&
   piece->getType() == PieceType::KING)
{
    int colDiff =
        abs(move.getFrom().getCol() - move.getTo().getCol());

    if(colDiff == 2)
    {
        return isValidCastling(board, move);
    }
}
    if(piece->getType() == PieceType::PAWN)
{
    if(isValidEnPassant(board, move, lastMove))
        return true;
}

    return isMovePossible(board, move) &&
           !leavesKingInCheck(board, move);
}

bool MoveValidator::isMovePossible(
    const Board& board,
    const Move& move) const
{
    Piece* piece = board.getPiece(move.getFrom());

    if(piece == nullptr)
        return false;

    std::vector<Move> possibleMoves = piece->getPossibleMoves(board);

    for(const Move& m : possibleMoves)
    {
        if(m.getFrom() == move.getFrom() &&
           m.getTo() == move.getTo())
        {
            return true;
        }
    }

    return false;
}

bool MoveValidator::leavesKingInCheck(
    Board& board,
    const Move& move) const
{
    Position from = move.getFrom();
    Position to = move.getTo();

    Piece* movingPiece = board.getPiece(from);
    Piece* capturedPiece = board.getPiece(to);

    Color movingColor = movingPiece->getColor();

    // Simulate move
    board.setPiece(from, nullptr);
    board.setPiece(to, movingPiece);
    movingPiece->setPosition(to);

    bool kingInCheck = isKingInCheck(board, movingColor);

    // Undo move
    board.setPiece(from, movingPiece);
    board.setPiece(to, capturedPiece);
    movingPiece->setPosition(from);

    return kingInCheck;
}

bool MoveValidator::isKingInCheck(
    const Board& board,
    Color color) const
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Position pos(i, j);

            Piece* piece = board.getPiece(pos);

            if(piece != nullptr &&
               piece->getColor() == color &&
               piece->getType() == PieceType::KING)
            {
                return isSquareUnderAttack(board, pos, color);
            }
        }
    }

    return false;
}

bool MoveValidator::isSquareUnderAttack(
    const Board& board,
    const Position& position,
    Color color) const
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Position pos(i, j);

            Piece* piece = board.getPiece(pos);

            if(piece == nullptr ||
               piece->getColor() == color)
            {
                continue;
            }

            std::vector<Position> attackedSquares =
                piece->getAttackedPositions(board);

            for(const Position& square : attackedSquares)
            {
                if(square == position)
                    return true;
            }
        }
    }

    return false;
}

bool MoveValidator::hasAnyLegalMove(
    Board& board,
    Color color, const Move& lastMove) const
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            Piece* piece = board.getPiece(Position(i, j));

            if(piece == nullptr)
                continue;

            if(piece->getColor() != color)
                continue;

            std::vector<Move> possibleMoves =
                piece->getPossibleMoves(board);

            for(const Move& move : possibleMoves)
            {
                if(isValidMove(board, move, lastMove))
                    return true;
            }
        }
    }

    return false;
}

// castling 
bool MoveValidator::isValidCastling(
    const Board& board,
    const Move& move) const
{
    const Position& from = move.getFrom();
    const Position& to = move.getTo();

    Piece* king = board.getPiece(from);

    if(king == nullptr)
        return false;

    if(king->getType() != PieceType::KING)
        return false;

    if(king->hasMoved())
        return false;

    int row = from.getRow();
    int fromCol = from.getCol();
    int toCol = to.getCol();

    bool kingSide = (toCol > fromCol);

    int direction = kingSide ? 1 : -1;
    int rookFromCol = kingSide ? 7 : 0;

    Position rookPosition(row, rookFromCol);

    Piece* rook = board.getPiece(rookPosition);

    if(rook == nullptr)
        return false;

    if(rook->getType() != PieceType::ROOK)
        return false;

    if(rook->getColor() != king->getColor())
        return false;

    if(rook->hasMoved())
        return false;

    // Squares between king and rook must be empty
    for(int col = fromCol + direction;
        col != rookFromCol;
        col += direction)
    {
        if(board.getPiece(Position(row, col)) != nullptr)
            return false;
    }

    // King must not currently be in check
    if(isSquareUnderAttack(board, from, king->getColor()))
        return false;

    // King cannot pass through check
    Position middle(row, fromCol + direction);

    if(isSquareUnderAttack(board, middle, king->getColor()))
        return false;

    // King cannot end in check
    if(isSquareUnderAttack(board, to, king->getColor()))
        return false;

    return true;
}

bool MoveValidator::isValidEnPassant(
    const Board& board,
    const Move& move,
    const Move& lastMove) const
{
    Piece* pawn = board.getPiece(move.getFrom());

    if(pawn == nullptr)
        return false;

    if(pawn->getType() != PieceType::PAWN)
        return false;


    int fromRow = move.getFrom().getRow();
    int fromCol = move.getFrom().getCol();
    int toRow = move.getTo().getRow();
    int toCol = move.getTo().getCol();

    int direction =
        (pawn->getColor() == Color::WHITE) ? -1 : 1;

    if(toRow != fromRow + direction)
        return false;


    if(abs(toCol - fromCol) != 1)
        return false;

    if(board.getPiece(move.getTo()) != nullptr)
        return false;

    if(lastMove.getFrom().getRow() == -1)
        return false;


    Piece* lastMovedPiece = board.getPiece(lastMove.getTo());

    if(lastMovedPiece == nullptr)
        return false;

    if(lastMovedPiece->getType() != PieceType::PAWN)
        return false;

    if(lastMovedPiece->getColor() == pawn->getColor())
        return false;

    int lastMoveDistance =
        abs(lastMove.getFrom().getRow() -
            lastMove.getTo().getRow());

    if(lastMoveDistance != 2)
        return false;


    if(lastMove.getTo().getRow() != fromRow)
        return false;


    if(lastMove.getTo().getCol() != toCol)
        return false;


    return true;
}