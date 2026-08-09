
#include "Knight.h"
#include "../board/Board.h"

Knight:: Knight(const Position& position, Color color) : Piece(position, color) {}

std::vector<Move> Knight::getPossibleMoves(const Board& board) const
{
     std::vector<Move> possibleMoves;
     int r = position.getRow();
     int c = position.getCol();
     int dr[8] = {-1,-1,2,2,-2,-2,1,1};
     int dc[8] = {2,-2,1,-1,1,-1,-2,2};

     for(int i=0; i<8; i++){
        Position newPos(r+dr[i], c+dc[i]);
        if(board.isInside(newPos)){
            if(!board.isOccupied(newPos) || board.getPiece(newPos)->getColor() != getColor()){
                possibleMoves.push_back(Move(position, newPos));
           }
        }
     }
     return possibleMoves;
}

char Knight::getSymbol() const
{
    return (getColor() == Color::WHITE) ? 'N' : 'n';
}

PieceType Knight:: getType() const{
    return PieceType::KNIGHT;
}