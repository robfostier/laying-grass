#pragma once

#include <string>

enum PlayerColor { PURPLE, RED, GREEN, YELLOW , BLUE, MAGENTA, CYAN, TURQUOISE, ORANGE };

class Player {
  private:
    const std::string name;
    const PlayerColor color;

    size_t coupons;
    size_t stoneBonus;
    size_t robberyBonus;

  public:
    Player(const std::string &name, PlayerColor color);

    const std::string& getName() const { return name; } // Returns a constant reference to the player's name
    PlayerColor getColor() const { return color; } // Returns the player's color by value
    size_t getCoupons() const { return coupons; }
    size_t getStoneBonus() const { return stoneBonus; }
    size_t getRobberyBonus() const { return robberyBonus; }

    void addCoupon() { ++coupons; }
    void addStoneBonus() { ++stoneBonus; }
    void addRobberyBonus() { ++robberyBonus; }

    void useCoupon() { --coupons; }
    void useStoneBonus() { --stoneBonus; }
    void useRobberyBonus() { --robberyBonus; }
};