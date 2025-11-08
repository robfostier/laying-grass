#include "player.hpp"

Player::Player(const std::string &name, PlayerColor color)
    : name(name), color(color), coupons(1), stoneBonus(0), robberyBonus(0) {}

