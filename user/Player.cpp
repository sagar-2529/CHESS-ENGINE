
#include "Player.h"


Player::Player(Color color,const std::string & name){
    this -> color = color;
    this -> name = name;
}
std:: string Player::getName() const{
    return name;
}

Color Player::getColor() const{
    return color;
}