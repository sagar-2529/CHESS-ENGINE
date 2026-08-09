#pragma once

#include "../board/Position.h"
#include "../enums/Color.h"
#include <vector>
#include"../enums/PieceType.h"

class Move;
class Board;

class Piece
{
protected:
    Position position;
    Color color;
    bool captured;
    bool hasMovedFlag;

public:
    Piece(const Position& position, Color color);

    virtual ~Piece() = default;

    Position getPosition() const;
    Color getColor() const;
    bool hasMoved() const;
    void setHasMoved(bool moved); 

    void setPosition(const Position& position);

    bool isCaptured() const;
    void setCaptured(bool captured);

    virtual std::vector<Move> getPossibleMoves(const Board& board) const = 0;

    virtual char getSymbol() const = 0;
    
    virtual std::vector<Position> getAttackedPositions(
    const Board& board) const;

    virtual PieceType getType() const = 0;
    
    
};