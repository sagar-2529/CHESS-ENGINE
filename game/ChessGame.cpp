#include "ChessGame.h"
#include"../pieces/Piece.h"
#include"../pieces/Queen.h"
#include"../pieces/Bishop.h"
#include"../pieces/Rook.h"
#include"../pieces/Knight.h"


#include <iostream>

ChessGame::ChessGame()
    : whitePlayer(Color::WHITE, "White"),
      blackPlayer(Color::BLACK, "Black"),
      currentTurn(Color::WHITE),
      gameStatus(GameStatus::IN_PROGRESS)
{
    StandardBoardInitializer initializer;
    initializer.initialize(board);
}

void ChessGame::start()
{
    while (!isGameOver())
    {
        board.printBoard();

        Player& currentPlayer =
            (currentTurn == Color::WHITE)
                ? whitePlayer
                : blackPlayer;

        std::cout << "\n====================================\n";
        std::cout << currentPlayer.getName() << "'s Turn\n";
        std::cout << "====================================\n";

        Move move = getMoveFromInput();

        if (!validator.isValidMove(board, move, lastMove))
        {
            std::cout << "\nInvalid Move! Please try again.\n\n";
            continue;
        }

        board.movePiece(move);

        handleCastling(move);
        handleEnPassant(move);

        lastMove = move;

        // Switch turn to opponent
        changeTurn();

        // Is the opponent's king in check?
        if (validator.isKingInCheck(board, currentTurn))
        {
            // Checkmate
            if (!validator.hasAnyLegalMove(board, currentTurn, lastMove))
            {
                gameStatus = GameStatus::CHECKMATE;

                std::cout << "\n====================================\n";
                std::cout << "CHECKMATE!\n";
                std::cout << currentPlayer.getName() << " Wins!\n";
                std::cout << "====================================\n";
            }
            // Just check
            else
            {
                gameStatus = GameStatus::CHECK;

                std::cout << "\n====================================\n";
                std::cout << "CHECK!\n";
                std::cout << "====================================\n";

                // Continue the game
                gameStatus = GameStatus::IN_PROGRESS;
            }
        }
        // Stalemate
        else if (!validator.hasAnyLegalMove(board, currentTurn, lastMove))
        {
            gameStatus = GameStatus::STALEMATE;

            std::cout << "\n====================================\n";
            std::cout << "STALEMATE!\n";
            std::cout << "Game Drawn!\n";
            std::cout << "====================================\n";
        }
    }

    board.printBoard();

    std::cout << "\nGame Over!\n";
}

Move ChessGame::getMoveFromInput() const
{
    std::string from, to;

    std::cout << "Enter Move (Example: e2 e4): ";
    std::cin >> from >> to;

    Position source = parsePosition(from);
    Position destination = parsePosition(to);

    return Move(source, destination);
}

Position ChessGame::parsePosition(const std::string& square) const
{
    int row = 8 - (square[1] - '0');
    int col = square[0] - 'a';

    return Position(row, col);
}

void ChessGame::changeTurn()
{
    currentTurn =
        (currentTurn == Color::WHITE)
            ? Color::BLACK
            : Color::WHITE;
}

bool ChessGame::isGameOver() const
{
    return gameStatus == GameStatus::CHECKMATE ||
           gameStatus == GameStatus::STALEMATE ||
           gameStatus == GameStatus::DRAW;
}

void ChessGame::handlePawnPromotion(const Move& move)
{
    Piece* piece = board.getPiece(move.getTo());

    if (piece == nullptr)
        return;

    if (piece->getType() != PieceType::PAWN)
        return;

    int row = move.getTo().getRow();

    if (piece->getColor() == Color::WHITE && row != 0)
        return;

    if (piece->getColor() == Color::BLACK && row != 7)
        return;

    cout << "\n=== Pawn Promotion ===\n";
    cout << "1. Queen\n";
    cout << "2. Rook\n";
    cout << "3. Bishop\n";
    cout << "4. Knight\n";
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;

    const Position& pos = move.getTo();
    Color color = piece->getColor();

    Piece* promotedPiece = nullptr;

    switch (choice)
    {
        case 1:
            promotedPiece = new Queen(pos, color);
            break;

        case 2:
            promotedPiece = new Rook(pos, color);
            break;

        case 3:
            promotedPiece = new Bishop(pos, color);
            break;

        case 4:
            promotedPiece = new Knight(pos, color);
            break;

        default:
            cout << "Invalid choice! Promoting to Queen.\n";
            promotedPiece = new Queen(pos, color);
    }

    delete piece;

    board.setPiece(pos, promotedPiece);

    cout << "Pawn promoted successfully!\n";
}

void ChessGame::handleCastling(const Move& move)
{
    Piece* king = board.getPiece(move.getTo());

    if(king == nullptr)
        return;

    if(king->getType() != PieceType::KING)
        return;

    int diff =
        abs(move.getTo().getCol() -
            move.getFrom().getCol());

    if(diff != 2)
        return;

    int row = move.getTo().getRow();

    bool kingSide =
        move.getTo().getCol() >
        move.getFrom().getCol();

    int direction =
        kingSide ? 1 : -1;

    int rookFromCol =
        kingSide ? 7 : 0;

    int rookToCol =
        move.getTo().getCol() - direction;

    Position rookFrom(row, rookFromCol);
    Position rookTo(row, rookToCol);

    board.movePiece(Move(rookFrom, rookTo));
}

void ChessGame::handleEnPassant(const Move& move)
{
    Piece* piece = board.getPiece(move.getTo());

    if(piece == nullptr)
        return;

    if(piece->getType() != PieceType::PAWN)
        return;

    int fromCol = move.getFrom().getCol();
    int toCol = move.getTo().getCol();

    // En Passant is always a diagonal move
    if(abs(toCol - fromCol) != 1)
        return;

    // Destination was empty before the move
    if(board.getPiece(lastMove.getTo()) == nullptr)
        return;

    Piece* lastMovedPiece = board.getPiece(lastMove.getTo());

    if(lastMovedPiece == nullptr)
        return;

    if(lastMovedPiece->getType() != PieceType::PAWN)
        return;

    int lastMoveDistance =
        abs(lastMove.getFrom().getRow() -
            lastMove.getTo().getRow());

    if(lastMoveDistance != 2)
        return;

    Position capturedPawn(
        move.getFrom().getRow(),
        move.getTo().getCol());

    if(!(capturedPawn == lastMove.getTo()))
    return;
    
     Piece* captured = board.getPiece(capturedPawn);

    delete captured;

    board.setPiece(capturedPawn, nullptr);
}