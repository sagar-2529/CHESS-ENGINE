#include "StandardBoardInitializer.h"

#include "../board/Board.h"

#include "../pieces/King.h"
#include "../pieces/Queen.h"
#include "../pieces/Rook.h"
#include "../pieces/Bishop.h"
#include "../pieces/Knight.h"
#include "../pieces/Pawn.h"

#include "../enums/Color.h"

void StandardBoardInitializer::initialize(Board& board)
{
    // White Pieces

    board.setPiece(Position(7, 0), new Rook(Position(7, 0), Color::WHITE));
    board.setPiece(Position(7, 1), new Knight(Position(7, 1), Color::WHITE));
    board.setPiece(Position(7, 2), new Bishop(Position(7, 2), Color::WHITE));
    board.setPiece(Position(7, 3), new Queen(Position(7, 3), Color::WHITE));
    board.setPiece(Position(7, 4), new King(Position(7, 4), Color::WHITE));
    board.setPiece(Position(7, 5), new Bishop(Position(7, 5), Color::WHITE));
    board.setPiece(Position(7, 6), new Knight(Position(7, 6), Color::WHITE));
    board.setPiece(Position(7, 7), new Rook(Position(7, 7), Color::WHITE));

    for (int col = 0; col < 8; col++)
    {
        board.setPiece(Position(6, col),
                       new Pawn(Position(6, col), Color::WHITE));
    }

    // Black Pieces

    board.setPiece(Position(0, 0), new Rook(Position(0, 0), Color::BLACK));
    board.setPiece(Position(0, 1), new Knight(Position(0, 1), Color::BLACK));
    board.setPiece(Position(0, 2), new Bishop(Position(0, 2), Color::BLACK));
    board.setPiece(Position(0, 3), new Queen(Position(0, 3), Color::BLACK));
    board.setPiece(Position(0, 4), new King(Position(0, 4), Color::BLACK));
    board.setPiece(Position(0, 5), new Bishop(Position(0, 5), Color::BLACK));
    board.setPiece(Position(0, 6), new Knight(Position(0, 6), Color::BLACK));
    board.setPiece(Position(0, 7), new Rook(Position(0, 7), Color::BLACK));

    for (int col = 0; col < 8; col++)
    {
        board.setPiece(Position(1, col),
                       new Pawn(Position(1, col), Color::BLACK));
    }
}