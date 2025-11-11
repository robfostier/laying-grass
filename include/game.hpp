#pragma once

#include "board.hpp"
#include "player.hpp"
#include "tile_queue.hpp"
#include <vector>

class Game {
  private:
    Board board;
    TileQueue tileQueue;
    size_t nbPlayers;
    std::vector<Player> players;
    size_t currentRound = 0;
    static const size_t maxRounds = 9;

  public:
    Game(size_t nbPlayers);

    void setup();

    void display(Player &player, Tile &tile, bool bDisplayQueue) const;
    void placingTile(Player &player, Tile &tile, bool bStealable, bool bDisplayQueue);

    void play();
    void playTurn(Player &player);

    void applyStoneBonus(Player &player);
    void applyRobberyBonus(Player &player);

    void exchangeRemainingCoupons(Player &player);
    const Player& determineWinner() const;
};