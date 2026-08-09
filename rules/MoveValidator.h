#pragma once

#include "../enums/Color.h"

class Board;
class Move;
class Position;

class MoveValidator
{
public:
    bool isValidMove(
    Board& board,
    const Move& move,
    const Move& lastMove) const;

    bool isMovePossible(const Board& board, const Move& move) const;

    bool leavesKingInCheck(
        Board& board,
        const Move& move) const;

    bool isKingInCheck(
        const Board& board,
        Color color) const;

    bool isSquareUnderAttack(
        const Board& board,
        const Position& position,
        Color color) const;

    bool hasAnyLegalMove(
        Board& board,
        Color color, const Move& lastMove) const;

    bool isValidCastling(
    const Board& board,
    const Move& move) const;

    bool isValidEnPassant(
    const Board& board,
    const Move& move,
    const Move& lastMove) const;
};