#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "../board/Board.h"
#include "../board/Move.h"
#include "../board/Position.h"
#include "../board/StandardBoardInitializer.h"
#include "../user/Player.h"
#include "../rules/MoveValidator.h"
#include "../enums/Color.h"
#include "../enums/GameStatus.h"

#include <string>
#include <vector>

class ChessGame
{
private:
    Board board;
    Player whitePlayer;
    Player blackPlayer;
    MoveValidator validator;
    Color currentTurn;
    GameStatus gameStatus;
    Move lastMove; 

private:
    void changeTurn();
    bool isGameOver() const;

    Move getMoveFromInput() const;
    Position parsePosition(const std::string& square) const;
    void handlePawnPromotion(const Move& move);
    void handleCastling(const Move& move);
    void handleEnPassant(const Move& move);

    // API helper: promote with an explicit piece choice string ("q","r","b","n")
    // Does NOT block on cin — used only by the API path.
    void promoteWithChoice(const Move& move, const std::string& choice);

public:
    // ── Original terminal entry point (unchanged) ──
    ChessGame();
    void start();

    // ── Thin public API surface for the Crow backend ──

    // Attempt a move from square "e2" to "e4".
    // promotion: one of "q","r","b","n" — required only when a pawn
    // reaches its last rank; ignored for all other moves.
    // Returns true if the move was legal and applied.
    bool makeMove(const std::string& from,
                  const std::string& to,
                  const std::string& promotion);

    // Returns true when the destination square of the given move string
    // is a pawn-promotion square AND the piece at `from` is a pawn of
    // the current turn's colour.  Used by the route to pre-validate
    // the promotion field before calling makeMove.
    bool isPromotionMove(const std::string& from, const std::string& to) const;

    Color      getTurn()   const;
    GameStatus getStatus() const;
    bool       isInCheck() const;

    // Re-initialise the game to its starting state.
    void reset();

    // Serialise the 8x8 board to a JSON-ready 2-D vector of strings.
    // Empty squares -> "."; pieces -> their getSymbol() char.
    std::vector<std::vector<std::string>> getBoardGrid() const;
};

#endif