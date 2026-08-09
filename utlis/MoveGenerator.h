#pragma once

#include <vector>

class Board;
class Piece;
class Move;

class MoveGenerator
{
public:
    static void addMovesInDirection(
        const Board& board,
        const Piece& piece,
        int rowStep,
        int colStep,
        std::vector<Move>& moves
    );
};