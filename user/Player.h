#pragma once

#include<string>

#include "../enums/Color.h"

class Player
{
    private:
    Color color;
    std::string name;

    public:
    Player(Color color, const std::string& name);

    std::string getName() const;
    Color getColor() const;
    
};