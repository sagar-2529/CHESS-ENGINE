#pragma once
class Board;

class BoardInitializer
{
    public:
       ~BoardInitializer() = default;
       virtual void initialize(Board& board) = 0;

};