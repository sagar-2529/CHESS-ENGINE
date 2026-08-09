
#include "Position.h"
#include "Move.h"
#include "../pieces/Piece.h"
#include "Board.h"
#include<iostream>
Board::Board()
{
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            board[i][j] = nullptr;
        }
    }
}

Piece* Board::getPiece(const Position& position) const
{
    if(isInside(position)){
        return board[position.getRow()][position.getCol()];
    }
    return nullptr;
}

void Board::setPiece(const Position& position, Piece* piece){
    if(isInside(position)){
        board[position.getRow()][position.getCol()] = piece;
    }
}

bool Board::isInside(const Position& position) const
{

    return position.isValid();
}

bool Board::isOccupied(const Position& position) const
{
    if(isInside(position) ){
        return board[position.getRow()][position.getCol()] != nullptr;
    }

    return false;
}

void Board::movePiece(const Move& move)
{
    const Position& from = move.getFrom();
    const Position& to = move.getTo();

    Piece* movingPiece = getPiece(from);
    Piece* capturedPiece = getPiece(to);

    // Delete captured piece (if any)
    if (capturedPiece != nullptr)
    {
        delete capturedPiece;
    }

    // Move the piece
    setPiece(from, nullptr);
    setPiece(to, movingPiece);

    // Update piece information
    movingPiece->setPosition(to);
    movingPiece->setHasMoved(true);
}

void Board::printBoard() const
{
    std::cout << "\n";

    for (int row = 0; row < 8; row++)
    {
        std::cout << 8 - row << " ";

        for (int col = 0; col < 8; col++)
        {
            Piece* piece = board[row][col];

            if (piece == nullptr)
            {
                std::cout << ". ";
            }
            else
            {
                std::cout << piece->getSymbol() << " ";
            }
        }

        std::cout << '\n';
    }

    std::cout << "  a b c d e f g h\n\n";
}













