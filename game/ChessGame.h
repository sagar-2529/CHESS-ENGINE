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
    

public:
    ChessGame();
    void start();
};

#endif