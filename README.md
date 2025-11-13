# 🟩 Laying Grass - pCCPP2

_Author : Robinson Fostier_

_Repo : [github.com/robfostier/laying-grass](https://github.com/robfostier/laying-grass)_

---
## 📄 User documentation

### 🎯 About The Project

Laying Grass is a CLI version of a board game from The Devil's Plan (데블스 플랜) Netflix reality game show.
In Laying Grass, players must place grass tiles on a grid-based territory board to expand their territory, with the goal of creating the largest square territory.
This project is educational. This was done as a student project for Supinfo.

---
### ⚙️ Getting started

#### Prerequisites

Make sure you have the following installed :
- `g++`
- `make`

To check if you have them installed :

```bash
g++ --version
make --version
```

You will also need Linux, MacOS or Windows.

#### Build instructions

1. Clone the repository

```bash
git clone https://github.com/robfostier/laying-grass.git
cd laying-grass
```

2. Build the project

```bash
make
```

3. Run the executable
    - on Windows : `./bin/layingrass.exe`
    - on Linux : `./bin/layingrass`

---
### 🕹️ Game rules

#### Overview

Laying Grass is a turn-based strategy game, played by 2 to 9 players. Each player must expand their territory on a shared grid by placing grass tiles of various shapes.
The goal is to form the largest possible square territory (e.g., 7x7). In case of a tie, the player with the most grass tiles wins.

#### Setup

- For 2 to 4 players, the grid is 20x20 squares.
- For 5 to 9 players, the grid is 30x30 squares. 
- Special bonus squares are randomly placed on the board (not on edges or next to each other).
- Each player enters a name and color, and receives:
    - One 1x1 starting tile
    - One tile exchange coupon

Players begin by placing their starting tile anywhere on the grid.

#### Tile distribution

- There are 96 possible tile shapes.
- Only a limited number of tiles will be used depending on the number of players: 32/3 (10.6..67) tiles per player (e.g., 43 for 4 players), rounded.
- Tiles are drawn in a predetermined random order, forming a shared tile queue.

Each turn, the current player receives the first tile from the queue.

If the player wants to exchange it, they can use a tile exchange coupon to pick one of the next five tiles instead.
The chosen tile is removed from the queue, and the tile immediately following it becomes the next one for the next player.
The skipped tiles (before the chosen one) are placed back at the back of the queue.

#### Tile placement rules

- Tiles must always be adjacent to the player’s territory (one of their existing tiles).
- Tiles cannot overlap or touch other players’ territories.
- Tiles can be rotated or flipped before placement.
- Once placed, tiles cannot be moved.
- If a player cannot place their tile, it is discarded, and they skip their turn.

#### End of game

The game lasts nine rounds, meaning each player will place up to nine tiles.
At the very end, players may spend any remaining tile exchange coupon to buy and place 1x1 grass tiles wherever they wish.

#### Bonus type

When a player surrounds a bonus square on its four sides (N, S, E, W), they claim it and gain a reward.
The square then becomes part of their territory.

1. Exchange Square :
+1 tile exchange coupon. 1.5 per player (rounded up).

2. Stone Square	:
Place a stone tile anywhere. Other tiles can’t be placed there. Must be used immediately. Removing it costs 1 coupon.	0.5 per player (rounded up).

3. Robbery Square:
Steal one tile from another player’s territory and add it to yours. Must be used immediately. 1 per player.

#### Win conditions

At the end of the game :
1. The player with the largest square-shaped territory wins (e.g., 7x7).
2. If there’s a tie, the player with the most grass tiles overall wins.

---
## 📄 Technical documentation

### 🧩 Project architecture

#### Directory structure

This project is written in C++. It follows a modular architecture that separates source code files, headers, compiled objects and executable into dedicated directories for maintainability.

```bash
laying-grass/
├── bin/
│   └── layingrass
├── src/
│   └── sources.cpp
├── include/
│   └── headers.hpp
├── obj/
│   └── objects.o
├── Makefile
└── README.md
```

- `bin/` contains compiled binaries.
- `src/` contains source code files.
- `include/` contains header files.
- `obj/` contains compiled objects.
- `Makefile` is a custom made file that automates compilation on Linux and Windows.
- `README.md` contains User Doc and Technical Doc. It is the file you are currently looking at.

#### Makefile Targets

Three commands are defined within Makefile :
- `make` compiles the project and produces `bin/layingrass`
- `make run` builds and runs the program
- `make clean` cleans the build

#### Build Process

1. Compile Sources
    Each `.cpp` file in `src/` is compiled into an `.o` file in `obj/`, using `.hpp` files in `include/`
2. Link Objects
    `.o` files are linked together to produce an executable in `bin/`
3. Run
    Execute the program from `bin/layingrass.exe`
4. Clean
    Use `make clean` to clean the build by removing all files in `obj/` and `bin/`

---
### 🧱 Data structures

#### Tile

```c++
class Tile {
  private:
    Shape baseShape;
    int rotation = 0;
    bool flipped = false;

    static Shape rotateShape(const Shape &shape);

  public:
    Tile(const Shape &shape) : baseShape(shape), rotation(0), flipped(false) {}

    Shape getShape() const;
    void rotateClockwise() { rotation = (rotation + 90) % 360; }
    void flipHorizontal() { flipped = !flipped; }
    void print() const;
};
```

`class Tile` represents a Tile object in the game.
It contains 3 private attributes :
- `Shape baseShape` : `Shape` is an alias for `std::vector<std::vector<uint8_t>>`, defined in `tile_shapes.hpp`. Each tile contains one unsigned 8-bit integers 2D vector to store its shape. Zeroes represent negative space and ones represent positive space.
- `int rotation` : An integer used to store rotation values. It can take four values : 0, 90, 180 and 270. It takes a default value of 0.
- `bool flipped` : A boolean used to store horizontal flipping information. It takes a default value of false.

It also contains 1 static private method :
- `static Shape rotateShape(const Shape &shape)` : This class method is used to apply a 90 degree rotation to a `Shape`, and return the result.

It also contains a basic constructor and 4 public methods :
- `Shape getShape() const` : A constant method that returns a copy of `baseShape` with rotation and flipping applied.
- `void rotateClockwise()` : A simple method that adds 90 to `rotation` and keeps it between 0 and 270.
- `void flipHorizontal()` : A simple method that inverts `flipped` value.
- `void print() const` : A constant method that prints the Tile in the terminal.

#### Tile Queue

```c++
class TileQueue {
  private:
    std::deque<Tile> tiles;

  public:
    TileQueue(size_t nbPlayers);

    Tile drawTile();
    void pushBack(const Tile &tile) { tiles.push_back(tile); }
    Tile exchangeTile(int index);
    std::vector<std::reference_wrapper<const Tile>> nextTiles(size_t n = 5) const;
    void printExchangeQueue(const std::vector<std::reference_wrapper<const Tile>> &tiles) const;
};
```

`class TileQueue` represents the queue from which tiles are drawn.
It contains 1 private attribute :
- `std::deque<Tile> tiles` : A `deque` is used instead of a `queue` to access tiles by index. This is useful to access the content of the structure without having to draw tiles.

It also contains a constructor and 5 public methods :
- `TileQueue(size_t nbPlayers)` : A `TileQueue` is constructed using the number of players as an argument. It constructs `10.67 * nbPlayers` tiles, randomly drawing shapes from `tile_shapes.hpp`.
- `Tile drawTile()` : A method that draws the next tile from the queue and returns it.
- `void pushBack(const Tile &tile)` : A simple method that puts a `Tile` at the back of the queue.
- `Tile exchangeTile(int index)` : A method that draws a tile at a specific index and places every tile before that at the back of the queue.
- `std::vector<std::reference_wrapper<const Tile>> nextTiles(size_t n) const` : A constant method that returns a vector of constant references to the next n tiles in the queue, without removing them from the queue.
- `void printExchangeQueue(const std::vector<std::reference_wrapper<const Tile>> &tiles) const` : A constant method that prints the next n tiles in the exchange queue.

#### Player

##### enum PlayerColor

```c++
enum PlayerColor { PURPLE, RED, GREEN, YELLOW , BLUE, MAGENTA, CYAN, TURQUOISE, ORANGE }
```

`enum PlayerColor` defines 9 colors.

##### class Player


```c++
class Player {
  private:
    const std::string name;
    const PlayerColor color;
    size_t coupons;
    size_t stoneBonus;
    size_t robberyBonus;

  public:
    Player(const std::string &name, PlayerColor color)
        : name(name), color(color), coupons(1), stoneBonus(0), robberyBonus(0) {}

    const std::string& getName() const { return name; }
    PlayerColor getColor() const { return color; }
    size_t getCoupons() const { return coupons; }
    size_t getStoneBonus() const { return stoneBonus; }
    size_t getRobberyBonus() const { return robberyBonus; }
    void addCoupon() { ++coupons; }
    void addStoneBonus() { ++stoneBonus; }
    void addRobberyBonus() { ++robberyBonus; }
    void useCoupon() { if (coupons > 0) --coupons; }
    void useStoneBonus() { if (stoneBonus > 0) --stoneBonus; }
    void useRobberyBonus() { if (robberyBonus > 0) --robberyBonus; }
};
```

`class Player` represents a player in the game.
It contains 5 private attributes :
- `const std::string name` : A constant string that stores the player's name.
- `const PlayerColor color` : A constant `PlayerColor` that stores the player's color.
- `size_t coupons` : An unsigned integer that stores the number of exchange coupons owned by the player.
- `size_t stoneBonus` : An unsigned integer that stores the number of stone bonuses owned by the player.
- `size_t robberyBonus` : An unsigned integer that stores the number of robbery bonuses owned by the player.

It also contains a simple constructor and 11 public methods :
- 5 getters, 1 for each private attribute.
- 3 simple methods to increment `coupons`, `stoneBonus` and `robberyBonus`, respectively.
- 3 simple methods to decrement `coupons`, `stoneBonus` and `robberyBonus`, respectively.

#### Board

##### enum CellType

```c++
enum CellType { EMPTY, GRASS, STONE, BONUS_EXCHANGE, BONUS_STONE, BONUS_ROBBERY };
```

`enum CellType` defines 6 cell types.

##### struct Cell

```c++
struct Cell {
    CellType type;
    Player *owner;
    std::string printSymbol = "";
};
```

`struct Cell` represents a cell in the board.
It contains 3 attributes:
- `CellType type` : A `CellType` that stores the type/content of the cell.
- `Player *owner` : A pointer to the player who owns the cell. It takes `nullptr` if unowned.
- `std::string printSymbol` : A string used to print the content of the cell in the terminal. It is used to distinguish bordering tiles owned by the same player.

##### struct PlacedTile

```c++
struct PlacedTile {
  Tile tile;
  std::pair<size_t, size_t> coords;
  Player *owner;
  bool bStealable;
};
```

`struct PlacedTile` represents a tile placed on the board.
It contains 4 attributes :
- `Tile tile` : A `Tile`.
- `std::pair<size_t, size_t> coords` : A pair of unsigned integers that stores the coordinates of the top-left square of the placed tile on the board.
- `Player *owner` : A pointer to the player who owns the cell.
- `bool bStealable` : A boolean used to protect starting tiles from being robbed.

##### class Board

```c++
class Board {
  private:
    const size_t size;
    Cell **grid; // Dynamic 2D array of Cells
    std::vector<PlacedTile> placedTiles;

  public:
    Board(size_t nbPlayers) : size(nbPlayers < 5 ? 20 : 30), grid(nullptr) { setup(nbPlayers); }
    ~Board();

    size_t getSize() const { return size; };
    const std::vector<PlacedTile>& getPlacedTiles() const { return placedTiles; }
    Cell& getCell(std::pair<size_t, size_t> coords) { return grid[coords.first][coords.second]; }
    const Cell& getCell(std::pair<size_t, size_t> coords) const { return grid[coords.first][coords.second]; }
    void setCell(std::pair<size_t, size_t> coords, CellType type, Player *owner);

    void setup(size_t nbPlayers);
    void placeBonus(CellType bonusType);
    bool canPlaceTile(std::pair<size_t, size_t> coords, const Tile &tile, const Player &player, bool bIsStartingTile) const;
    bool canPlaceTileAnywhere(const Tile &tile, const Player &player) const;
    void placeTile(std::pair<size_t, size_t> coords, const Tile &tile, Player *player, bool bStealable);
    void checkBonusCapture(Player *player);
    std::optional<Tile> stealTile(std::pair<size_t,size_t> target, Player* newOwner);
    void display() const;
};
```

`class Board` represents a Board object in the game.
It contains 3 private attributes :
- `const size_t size` : A constant unsigned integer that stores the size of the board.
- `Cell **grid` : A dynamic 2D array of cells.
- `std::vector<PlacedTile> placedTiles` : A vector of `PlacedStiles` that stores all tiles placed on the board.

It also contains a constructor and a destructor :
- `Board(size_t nbPlayers)` : A `Board` is constructed using the number of players as an argument. It initializes `size` and calls `setup()`.
- `~Board()` : A `Board` is destroyed by manually freeing the contents of `grid`.

It also contains 13 public methods :
- 4 getters, 1 for `size`, 1 for `placedTiles` and 2 for the cells at specific coordinates.
- `void setCell(std::pair<size_t, size_t> coords, CellType type, Player *owner)` : A setter for cells at specific coordinates.
- `void setup(size_t nbPlayers)` : A method used by the class constructor that sets up the board for a game, taking the number of players as an argument. It allocates `grid` and places bonuses on the board.
- `void placeBonus(CellType bonusType)` : A method that places a bonus on the board, following the game rules.
- `bool canPlaceTile(std::pair<size_t, size_t> coords, const Tile &tile, const Player &player, bool bIsStartingTile) const` : A constant method that returns a bool indicating whether a player can place a specific tile at specific coordinates.
- `bool canPlaceTileAnywhere(const Tile &tile, const Player &player) const` : A constant method that returns a bool indicating whether a player can place a specific tile anywhere on the board.
- `void placeTile(std::pair<size_t, size_t> coords, const Tile &tile, Player *player, bool bStealable)` : A method that places a tile on the board.
- `void checkBonusCapture(Player *player)` : A method that updates bonus cells when captured.
- `std::optional<Tile> stealTile(std::pair<size_t,size_t> target, Player* newOwner)` : A method that removes a tile from `placedTiles` and returns it. It is used for robbery bonuses logic. It may return `nullopt` on failure.
- `void display() const` : A method that prints the board in the terminal.

#### Game

```c++
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
```

`class Game` represents a game of Laying Grass.
It contains 6 private attributes :
- `Board board` : A `Board`.
- `TileQueue tileQueue` : A `TileQueue`.
- `size_t nbPlayers` : An unsigned integer that stores the number of players.
- `std::vector<Player> players` : A vector that stores every `Player` in the game.
- `size_t currentRound` : An unsigned integer that stores the current round number.
- `static const size_t maxRounds` : A static constant unsigned integer that indicates the maximum number of rounds. It takes 9, as per the game rules.

It also contains a constructor and 9 public methods :
- `Game(size_t nbPlayers)` : A `Game` is constructed using the number of players as an argument. It initializes `tileQueue`, `nbPlayers`, `currentRound` and calls `setup()`.
- `void setup()` : A method that initializes `players`. It asks each physical player for a name and a color using standard input.
- `void display(Player &player, Tile &tile, bool bDisplayQueue) const` : A constant method that displays the state of the game in the terminal. It may or may not display the tile queue.
- `void placingTile(Player &player, Tile &tile, bool bStealable, bool bDisplayQueue)` : A method that defines the player interface and game logic regarding tile placement.
- `void play()` : A method that defines the game loop. It is called in `main()`.
- `void playTurn(Player &player)` : A method that defines single turn logic.
- `void applyStoneBonus(Player &player)` : A method that defines stone bonuses logic.
- `void applyRobberyBonus(Player &player)` : A method that defines robbery bonuses logic.
- `void exchangeRemainingCoupons(Player &player)` : A method called at the end of the game to exchange any remaining coupon for 1x1 tiles.
- `const Player& determineWinner() const` : A constant method called at the end of the game that determines the winner of the game, as per the game rules. 

---
### 🔄 Program overview

#### Entry point

```c++
int main(void) {
    clearTerminal();

    std::cout << "Welcome to The Devil's Plan!" << std::endl;

    int nbPlayers = getIntegerInputInRange("Please enter number of players (2 - 9): ", 2, 9);
    Game game(static_cast<size_t>(nbPlayers));

    game.play();

    return 0;
}
```

`main()` is defined in `main.cpp`. It acts as the entry point to our program. It gets `nbPlayers` using standard input, then constructs a `Game` object and calls its `play()` method.

#### Game::play()

```c++
void Game::play() {
    for (currentRound = 0; currentRound <= maxRounds; ++currentRound)
        for (auto &player : players)
            playTurn(player);

    for (auto &player : players)
        exchangeRemainingCoupons(player);

    const Player &winner = determineWinner();

    clearTerminal();
    board.display();

    std::cout << colorize(winner.getColor()) << winner.getName() << resetColor << " wins the game." << std::endl;
    std::cout << "Thank you for playing !"  << std::endl;
}
```

`Game::play()` defines the main game loop :
- It loops `maxRounds` times, calling `Game::playTurn()` for each player in `Game::players` each round.
- It then calls `Game::exchangeRemainingCoupons` for each player in `Game::players` to let them exchange any remaining coupons for 1x1 tiles. 
- It then determines the winner by calling `Game::determineWinner()` and prints the result to the terminal.

#### Game::playTurn()

```c++
// Simplified to bare-bone logic
void Game::playTurn(Player &player) {
    if (currentRound == 0) {
        Tile startingTile = Tile(STARTING_TILE);

        std::pair<size_t, size_t> coords = {0, 0};
        while (true) {
            coords = getCoordinatesInput(board.getSize());
            bool canPlace = board.canPlaceTile(coords, startingTile, player, true);
            if (canPlace) break;
        }

        board.placeTile(coords, startingTile, &player, false);
        return;
    }

    Tile currentTile = tileQueue.drawTile();
    size_t exchangeCoupons = player.getCoupons();
    bool bDone = false;
    while (exchangeCoupons > 0 && !bDone) {
        int choiceAction = getIntegerInputInRange("> ", 1, 2);
        switch (choiceAction) {
        case 1:
            bDone = true;
            break;
        case 2: {
            int choiceExchange = getIntegerInputInRange("> ", 1, 2);
            switch (choiceExchange) {
            case 1: {
                size_t maxChoices = std::min(static_cast<size_t>(5), tileQueue.nextTiles().size());
                if (maxChoices > 0) {
                    int choiceTileIndex = getIntegerInputInRange("> ", 0, static_cast<int>(maxChoices));
                    if (choiceTileIndex != 0) {
                        int tileIndex = choiceTileIndex - 1;
                        tileQueue.pushBack(currentTile);
                        currentTile = tileQueue.exchangeTile(tileIndex);
                        player.useCoupon();
                        bDone = true;
                    }
                }
                break;
            }
            case 2: {
                bool bExists = false;
                for (size_t x = 0; x < board.getSize(); ++x) {
                    for (size_t y = 0; y < board.getSize(); ++y) {
                        if (board.getCell({x, y}).type == STONE) {
                            bExists = true;
                            break;
                        }
                    }
                }

                if (bExists) {
                    std::pair<size_t, size_t> coords = getCoordinatesInput(board.getSize());
                    const Cell &cell = board.getCell(coords);

                    if (cell.type == STONE) {
                        board.setCell(coords, EMPTY, nullptr);
                        player.useCoupon();
                        bDone = true;
                        break;
                    }
                }
                break;
            }
            default: break;
            }
            break;
        }
        default: break;
        }

        exchangeCoupons = player.getCoupons();
    }

    if (board.canPlaceTileAnywhere(currentTile, player))
        placingTile(player, currentTile, true, true);
}
```

Here is defined a single turn game logic.
That method has a special behaviour if `Game::currentRound` is equal to 0 :
- It gets valid coordinates from the user. The validity is mostly computed by calling `Board::canPlaceTile()`. Previous input management checks are done in `getCoordinatesInput()`, defined in `utils.cpp`.
- It places a 1x1 tile at that location. The 1x1 tile shape is defined in `tile_shapes.hpp` as `STARTING_TILE`.

For every turn after the initial turn 0, a more complex logic is applied :
- It draws a tile from `Game::tileQueue` using `TileQueue::drawTile()`.
- It then gets the player's number of exchange coupons using its getter. If the player has any, it offers the choice to use one and only one.
    - It offers the choice to exchange the currently drawn tile for any tile in the next 5 slots in `Game::tileQueue` or to remove a `STONE` from the board.
    - If the player wants to exchange the currently drawn tile, it puts `currentTile` at the back of the queue using `TileQueue::pushBack()` and draws the wanted tile from the queue using `TileQueue::exchangeTile()`, which returns that tile and pushes all the ones before back in the queue. It also decrements `Player::coupons` using `Player::useCoupon()`.
    - If the player wants to remove a `STONE` and there is one on the board, it asks for valid coordinates and removes it using `Board::setCell()`. It also decrements `Player::coupons` using `Player::useCoupon()`.
- It then calls `Board::canPlaceTileAnywhere()` to make sure there is a possible placement. If not, the turn ends here and the tile is discarded.
- It finally calls `Game::placingTile()`, to which it delegates the tile placement logic (rotation, horizontal flipping, placement).

#### Game::placingTile()

```c++
// Simplified to bare-bone logic
void Game::placingTile(Player &player, Tile &tile, bool bStealable, bool bDisplayQueue) {
    bool ready = false;
    while(!ready) {
        int choiceBuffer = getIntegerInputInRange("> ", 1, 3);
        switch (choiceBuffer) {
            case 1:
                ready = true;
                break;
            case 2:
                tile.rotateClockwise();
                break;
            case 3:
                tile.flipHorizontal();
                break;
        }
    }

    std::pair<size_t, size_t> coords = {0, 0};
    while (true) {
        coords = getCoordinatesInput(board.getSize());
        bool canPlace = board.canPlaceTile(coords, tile, player, false);
        if (canPlace)
            break;
    }

    board.placeTile(coords, tile, &player, bStealable);

    size_t pendingStone = player.getStoneBonus();
    while (pendingStone > 0) {
        applyStoneBonus(player);
        pendingStone = player.getStoneBonus();
    }

    // Exact same logic for applyRobberyBonus()
}
```

This method defines the game rules associated with tile placement, and implements the user interface for it :
- It first offers the choice to place, rotate or flip the tile. Rotation is applied using `Tile::rotateClockwise()` and horizontal flipping is applied using `Tile::flipHorizontal()`.
- It then asks the user for valid coordinates for tile placement. The validity is mostly computed by calling `Board::canPlaceTile()`. Previous input management checks are done in `getCoordinatesInput()`, defined in `utils.cpp`.
- It then calls `Board::placeTile()` to place the tile on the board.
- Finally, it checks for `Player::stoneBonus` and `Player::robberyBonus`. While the player owns any, they are consumed using `Player::applyStoneBonus()` and `Player::applyRobberyBonus()`.

#### Game::applyStoneBonus()

```c++
// Simplified to bare-bone logic
void Game::applyStoneBonus(Player &player) {
    std::pair<size_t, size_t> coords = {0, 0};

    while (true) {
        coords = getCoordinatesInput(board.getSize());
        if (board.getCell(coords).type == EMPTY) break;
    }

    board.setCell(coords, STONE, nullptr);
    player.useStoneBonus();
}
```

This method defines stone bonuses logic :
- It first asks the user for valid coordinates. The validity is done by checking for the cell's type at the specified position. Stone bonuses can only be used on `EMPTY` cells.
- It then sets the chosen cell as `STONE` using `Board::setCell()`.
- Finally, it decrements `Player::stoneBonus` using `Player::useStoneBonus()`.

#### Game::applyRobberyBonus()

```c++
// Simplified to bare-bone logic
void Game::applyRobberyBonus(Player &player) {
    std::pair<size_t, size_t> coords = {0, 0};
    std::optional<Tile> stolenTile;

    player.useRobberyBonus();

    bool bExists = false;
    for (const PlacedTile &placedTile : board.getPlacedTiles()) {
        if (placedTile.owner != &player && placedTile.bStealable) {
            bExists = true;
            break;
        }
    }

    if (!bExists) return;

    while (true) {
        coords = getCoordinatesInput(board.getSize());
        stolenTile = board.stealTile(coords, &player);
        if (stolenTile.has_value()) break;
    }
    
    placingTile(player, stolenTile.value(), true, false);
}
```

This method defines robbery bonuses logic :
- It first decrements `Player::robberyBonus` using `Player::useRobberyBonus()`.
- It then needs to make sure there is a placed tile to steal before entering the loop. If none exist, player can't use bonus but still loses coupon. It does so by looking at all the tiles in `Board::placedTiles` using its getter. It it is unable to find a stealable placed tile belonging to another player, it returns immediately. 
- It then asks the user for valid coordinates. It checks for validity mostly by calling `Board::stealTile()`, that removes a placed tile from `Board::placedTiles` and returns it.
- It finally calls `Game::placingTile()`, to which it delegates the tile placement logic (rotation, horizontal flipping, placement).

#### Game::exchangeRemainingCoupons()

```c++
// Simplified to bare-bone logic
void Game::exchangeRemainingCoupons(Player &player) {
    size_t coupons = player.getCoupons();
    while (coupons > 0) {
        std::pair<size_t, size_t> coords = getCoordinatesInput(board.getSize());
        board.placeTile(coords, Tile(STARTING_TILE), &player, false);
        player.useCoupon();
        coupons = player.getCoupons();
    }
}
```

While the player has any coupons remaining, it asks him for coordinates to place a 1x1 tile on the board and decrements the coupons count using `Player::useCoupon()`.
The 1x1 tile shape is defined in `tile_shapes.hpp` as `STARTING_TILE`.

#### Game::determineWinner()

```c++
// Simplified to bare-bone logic
const Player& Game::determineWinner() const {
    const Player *winner = &players[0];
    std::pair<size_t, size_t> toBeat = {0, 0};

    for (const Player &player : players) {
        size_t boardSize = board.getSize();
        std::vector<std::vector<size_t>> table(boardSize, std::vector<size_t>(boardSize, 0));

        size_t largestSquare, bestX, bestY;
        for (size_t x = 0; x < boardSize; ++x) {
            for (size_t y = 0; y < boardSize; ++y) {
                const Cell &cell = board.getCell({x, y});
                if (cell.owner == &player) {
                    if (x == 0 || y == 0) table[x][y] = 1;
                    else table[x][y] = 1 + std::min({table[x - 1][y], table[x][y - 1], table[x - 1][y - 1]});

                    if (table[x][y] > largestSquare){
                        largestSquare = table[x][y];
                        bestX = x;
                        bestY = y;
                    }
                }
            }
        }

        size_t grassCount = 0;
        for (size_t x = bestX - largestSquare + 1; x < bestX; ++x) {
            for (size_t y = bestY - largestSquare + 1; y < bestY; ++y) {
                const Cell &cell = board.getCell({x, y});
                if (cell.type == GRASS
                    && cell.printSymbol != "Ｅ"
                    && cell.printSymbol != "Ｓ"
                    && cell.printSymbol != "Ｒ"
                    && cell.owner == &player)
                    grassCount++;
            }
        }

        if (largestSquare > toBeat.first || (largestSquare == toBeat.first && grassCount > toBeat.second)) {
            toBeat = {largestSquare, grassCount};
            winner = &player;
        }
    }

    return *winner;
}
```

The winner is determined by computing two values for each player, and comparing them against each other :
- `size_t largestSquare` : A player-specific unsigned integer that stores the size of the largest square covered by tiles owned by this player on the board.
- `size_t grassCount` : A player-specific unsigned integer that stores the number of `GRASS` tiles in the largest square covered by the tiles owned by this player on the board.
- `std::pair<size_t, size_t> toBeat` : stores the best overall result. It is used for comparison and updated if beaten.

The method computes these scores for each player using the following logic :
- It creates a 2D vector of unsigned integers `table` initially filled with 0s. `table[x][y]` stores the size of the largest valid square whose bottom right corner is at {x, y}.
- It then looks at every cell in the board, starting at {0, 0} :
    - If it finds a cell owned by that player, it stores 1 + the minimum value stored in {x - 1, y}, {x, y - 1} and {x - 1, y - 1}. Using that logic, we are able to construct `table`.
    - It a cell beats that player `largestSquare`, it updates it with its value and stores its coordinates in `bestX` and `bestY`.
- After computing `largestSquare`, it uses `bestX`and `bestY` to go back at these coordinates and look in a `largestSquare` size. For each cell in this area, it increments `grassCount` if that cell's type is `GRASS`, is not a former bonus, and is owned by the player.
- It then compares `largestSquare` and `grassCount` to `toBeat`. If `largestSquare` beats `toBeat.first` or if it equals it and `grassCount` beats `toBeat.second`, that player has set a new best Score. It updates `toBeat` and store that player as the provisional winner.

Finally, after computing the scores for each player, the method has determined the winner and returns it.

#### Board::canPlaceTile()

```c++
// Simplified to bare-bone logic
bool Board::canPlaceTile(std::pair<size_t, size_t> coords, const Tile &tile, const Player &player, bool bIsStartingTile) const {
    Shape shape = tile.getShape();
    const std::array<std::pair<int,int>,4> directions = {{{-1,0}, {1,0}, {0,-1}, {0,1}}};
    bool bTouchesOwnCell = false;

    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[0].size(); ++j) {
            size_t x = coords.first + i;
            size_t y = coords.second + j;

            if (x >= size || y >= size) return false;
            if (grid[x][y].type == GRASS || grid[x][y].type == STONE) return false;

            for (const auto &dir : directions) {
                int newX = static_cast<int>(x) + dir.first;
                int newY = static_cast<int>(y) + dir.second;
                if (newX < 0 || newY < 0 || newX >= static_cast<int>(size) || newY >= static_cast<int>(size))
                    continue;

                const Player *owner = grid[newX][newY].owner;
                if (owner) {
                    if (owner != &player) return false;
                    if (!bIsStartingTile) bTouchesOwnCell = true;
                }
            }
        }
    }

    return bIsStartingTile || bTouchesOwnCell;
}
```

This method is used to determine player input validation when trying to place tiles on the board. It returns a boolean, indicating if a player can place a tile at specified coordinates :
- First, it defines an array of directions. In this program, coordinates are represented by pairs of unsigned integers. These 4 coordinates mark directions. They will be used to check orthogonal adjacency.
- It then loops over each cell in the tile's shape :
    - To find the "global" position of each tile cell in the board, it adds the coordinates input by the user to the "local" position of the cell in its tile.
    - It then checks for two conditions : if the cell's global coordinates are outside board bounds or if they point to a location already covered by `GRASS` or `STONE`, the method returns false. This placement is invalid.
    - It then checks for orthogonal neighbours, by looping over the directions array :
        - If it finds a cell that already has an owner and that owner is not the same player as the one trying to place the tile, it returns false. This placement is invalid.
        - If it finds a cell that already has an owner and that owner is the same player as the one trying to place the tile, it marks the tile as touching own territory using `bool bTouchesOwnCell`.
- It can then make the final validation check : if the player is trying to place his `STARTING_TILE` (indicated by a boolean passed in function parameter) or if `bTouchesOwnCell` was marked true by the last procedure, it returns true. This placement is valid. Otherwise, it returns false.

#### Board::placeTile()

```c++
// Simplified to bare-bone logic
void Board::placeTile(std::pair<size_t, size_t> coords, const Tile &tile, Player *player, bool bStealable) {
    Shape shape = tile.getShape();
    const std::array<std::string, 12> symbols = {"██", "██", "▒▒", "░░", ...};
    std::set<std::string> neighbourSymbols;
    const std::array<std::pair<int,int>,4> directions = {{{-1,0}, {1,0}, {0,-1}, {0,1}}};

    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[i].size(); ++j) {
            if (!shape[i][j]) continue;

            size_t x = coords.first + i;
            size_t y = coords.second + j;

            for (const auto &dir : directions) {
                int newX = static_cast<int>(x) + dir.first;
                int newY = static_cast<int>(y) + dir.second;

                if (newX < 0 || newY < 0 || newX >= static_cast<int>(size) || newY >= static_cast<int>(size))
                    continue;

                Cell &neighbourCell = grid[newX][newY];

                if (neighbourCell.type == GRASS && !neighbourCell.printSymbol.empty())
                    neighbourSymbols.insert(neighbourCell.printSymbol);
            }
        }
    }

    std::string availableSymbol = "██";
    for (std::string s : symbols) {
        if (neighbourSymbols.find(s) == neighbourSymbols.end()) {
            availableSymbol = s;
            break;
        }
    }

    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[i].size(); ++j) {
            if (!shape[i][j]) continue;

            Cell &cell = grid[coords.first + i][coords.second + j];
            cell.type = GRASS;
            cell.owner = player;
            cell.printSymbol = availableSymbol;
        }
    }

    checkBonusCapture(player);
    placedTiles.push_back({tile, coords, player, bStealable});
}
```

This method is responsible for placing a tile on the board. It is not responsible for any validation check : 
- Using the same logic as `Board::canPlaceTile()`, it make a first pass on orthogonal neighbours. It looks for any previously placed `GRASS` cell. If it finds one, it adds its `Cell::printSymbol` to the set `neighboursSymbols`. It keeps track of this information so that no two adjacent tiles use the same printSymbol. That allows the players to distinguish previously placed tiles, for robbery bonuses.
- It then choses the first symbol in the array `symbols` not in the set `neighboursSymbols` as an availableSymbol.
- It then places the tile on the board. Logic here is straightforward.
- It then calls `Board::checkBonusCapture()`, that updates any cell containing a bonus if that cell is now surrounded by 4 `GRASS` tiles belonging to a single player.
- Finally, it pushes the tile into `Board::placedTiles` to keep a record of it, for robbery bonuses logic.

#### Board::checkBonusCapture()

```c++
void Board::checkBonusCapture(Player *player) {
    const std::array<std::pair<int,int>,4> directions = {{{-1,0}, {1,0}, {0,-1}, {0,1}}};

    for (int x = 0; x < static_cast<int>(size); ++x) {
        for (int y = 0; y < static_cast<int>(size); ++y) {
            Cell &cell = grid[x][y];

            if (cell.type != BONUS_EXCHANGE && cell.type != BONUS_STONE && cell.type != BONUS_ROBBERY)
                continue;

            bool surrounded = true;
            for (const auto &dir : directions) {
                int newX = x + dir.first;
                int newY = y + dir.second;

                Cell &neighbour = grid[newX][newY];
                if (neighbour.owner != player || neighbour.type != GRASS) {
                    surrounded = false;
                    break;
                }
            }

            if (surrounded) {
                switch (cell.type) {
                    case BONUS_EXCHANGE:
                        player->addCoupon();
                        cell.printSymbol = "Ｅ";
                        break;
                    case BONUS_STONE:
                        player->addStoneBonus();
                        cell.printSymbol = "Ｓ";
                        break;
                    case BONUS_ROBBERY:
                        player->addRobberyBonus();
                        cell.printSymbol = "Ｒ";
                        break;
                    default:
                        break;
                }

                cell.type = GRASS;
                cell.owner = player;
            }
        }
    }
}
```

This method loops over every cell in the board, looking for any `BONUS_...` cell surrounded by 4 `GRASS` tiles owned by the same player :
- If the cell is not a bonus cell, it does not look at it further.
- It then loops over all 4 orthogonal directions. If it finds a cell that is not `GRASS` or that is not owned by `player`, it marks the current bonus cell as not surrounded.
- If the bonus cell is still marked as surrounded, it found a newly surrounded bonus cell. It then adds the corresponding coupon to the player, and updates the cell's content to replace the bonus by a `GRASS` tile.

#### Board::stealTile()

```c++
// Simplified to bare-bone logic
std::optional<Tile> Board::stealTile(std::pair<size_t, size_t> target, Player *newOwner) {
    for (auto it = placedTiles.begin(); it != placedTiles.end(); ++it) {
        PlacedTile &placedTile = *it;
        if (!placedTile.bStealable || placedTile.owner == newOwner) continue;

        Shape shape = placedTile.tile.getShape();
        size_t posX = placedTile.coords.first;
        size_t posY = placedTile.coords.second;

        for (size_t i = 0; i < shape.size(); ++i) {
            for (size_t j = 0; j < shape[i].size(); ++j) {
                if (!shape[i][j]) continue;

                if (posX + i == target.first && posY + j == target.second) {
                    for (size_t i2 = 0; i2 < shape.size(); ++i2) {
                        for (size_t j2 = 0; j2 < shape[i2].size(); ++j2) {
                            if (!shape[i2][j2]) continue;

                            Cell &cell = grid[posX + i2][posY + j2];
                            cell.type = EMPTY;
                            cell.owner = nullptr;
                            cell.printSymbol = "";
                        }
                    }
                    Tile stolenTile = placedTile.tile;
                    placedTiles.erase(it);
                    return stolenTile;
                }
            }
        }
    }
    return std::nullopt;
}
```

This method is responsible for removing a tile from `Board::placedTiles` at specified coordinates, and returning it. If it is unable to find such a tile, or if this tile can not be stolen, it returns `nullopt`. It acts as the main logic operator for robbery bonuses, looping over every tile in `Board::placedTiles` using iterators :
- If that placed tile is not stealable (meaning it is another player's starting 1x1 tile), or if it belongs to the player trying to steal, it skips that tile.
- It then goes through every cell covered by that tile. If a cell's global position matches the target coordinates specified as parameter, it found the target placed tile.
It removes the tile from the board by emptying each cell's content and erasing the tile from `Board::placedTiles` and it then returns it.
- If none of the previous checks matched, it returns `nullopt`.

---
### 🖥️ Displaying methods

#### Tile::print()

```c++
void Tile::print() const {
    for (const auto &row : getShape()) {
        for (uint8_t cell : row)
            std::cout << (cell ? "██" : "  ");
        std::cout << std::endl;
    }
}
```

This method is used to display a drawn tile, outside the board. It simply prints `██` for positive spaces and ` ` for negative spaces.

#### TileQueue::printExchangeQueue()

```c++
// Simplified to bare-bone logic
void TileQueue::printExchangeQueue(const std::vector<std::reference_wrapper<const Tile>> &tiles) const {
    size_t maxHeight = 0;
    for (const auto &tile : tiles) {
        const Shape &shape = tile.get().getShape();
        if (shape.size() > maxHeight) maxHeight = shape.size();
    }

    for (size_t row = 0; row < maxHeight; ++row) {
        for (const auto &tile : tiles) {
            const Shape &shape = tile.get().getShape();
            size_t tileHeight = shape.size();
            size_t offset = (maxHeight - tileHeight) / 2;

            if (row < offset || row >= offset + tileHeight) {
                for (size_t col = 0; col < shape[0].size(); ++col)
                    std::cout << "  ";
            } else {
                const auto &tileRow = shape[row - offset];
                for (uint8_t cell : tileRow)
                    std::cout << (cell ? "██" : "  ");
            }

            std::cout << "  ";
        }
    }
}
```

This methods prints the next n tiles in the queue. It prints multiple tiles on same lines, and aligns each tile vertically :
- It first determines the maximum height of the tiles to align.
- It then prints each row of the tiles, looping over the tiles to print out :
    - For each tile, it computes an unsigned integer `offset`, which will determine the number of blank rows after and before the tile, to align all the tiles vertically
    - It then either prints empty spaces, for rows outside the tile's height, or it prints the corresponding row of the tile.
    - Finally, it prints blank spaces before moving on to the next tile in the row.

#### Board::display()

```c++
void Board::display() const {
    std::vector<std::string> labels;

    for (size_t i = 0; i < size; ++i) {
        char32_t ch;
        if (i < 26) ch = 0xFF21 + i; // "Ａ" to "Ｚ"
        else ch = 0xFF41 + (i - 26); // "ａ" to "ｚ"

        std::string utf8;
        if (ch <= 0x7F) utf8.push_back(ch);
        else if (ch <= 0x7FF) {
            utf8.push_back(0xC0 | ((ch >> 6) & 0x1F));
            utf8.push_back(0x80 | (ch & 0x3F));
        } else if (ch <= 0xFFFF) {
            utf8.push_back(0xE0 | ((ch >> 12) & 0x0F));
            utf8.push_back(0x80 | ((ch >> 6) & 0x3F));
            utf8.push_back(0x80 | (ch & 0x3F));
        } else {
            utf8.push_back(0xF0 | ((ch >> 18) & 0x07));
            utf8.push_back(0x80 | ((ch >> 12) & 0x3F));
            utf8.push_back(0x80 | ((ch >> 6) & 0x3F));
            utf8.push_back(0x80 | (ch & 0x3F));
        }
        labels.push_back(utf8);
    }

    std::cout << "     ";
    for (const auto &label : labels) std::cout << label;

    std::cout << std::endl << "   +-";
    for (size_t i = 0; i < size; ++i) std::cout << "--";
    std::cout << "-+" << std::endl;

    for (size_t x = 0; x < size; ++x) {
        std::cout << labels[x] << " | ";
        for (size_t y = 0; y < size; ++y) {
            const Cell &cell = grid[x][y];

            switch (cell.type) {
            case EMPTY:
                std::cout << "・";
                break;
            case GRASS:
                std::cout << colorize(cell.owner->getColor()) << cell.printSymbol << resetColor;
                break;
            case STONE:
                std::cout << "\033[37m" << "██" << resetColor;
                break;
            case BONUS_EXCHANGE:
                std::cout << "Ｅ";
                break;
            case BONUS_STONE:
                std::cout << "Ｓ";
                break;
            case BONUS_ROBBERY:
                std::cout << "Ｒ";
                break;
            default:
                break;
            }
        }
        std::cout << " | " << labels[x] << std::endl;
    }

    std::cout << "   +-";
    for (size_t i = 0; i < size; ++i) std::cout << "--";
    std::cout << "-+" << std::endl;

    std::cout << "     ";
    for (const auto &label : labels) std::cout << label;
    std::cout << std::endl << std::endl;
}
```

This method is responsible for printing the board, and everything on it, in the terminal :
- First, it creates a vector of string `labels`, where the row and column index labels will be stored. This method uses 2-spaces wide characters to properly align the labels with the centers of there associated rows/columns. To construct this vector, it gets the character in UTF-32 and converts it to UTF-8.
- It then prints the first row, which is mostly the labels side-by-side.
- It then prints a separator row between the labels and the board content.
- For all the successive rows, it will print the label, a column separator, and then each cell in that row. The cell content is printed out depending on its type and its optional printing symbol. Then, it prints another column separator, and prints again the label, so that it exists on both sides of each row.
- It then prints the bottom separator row.
- Finally, it prints the last row which is mostly the labels side-by-side.

#### Game::display()

```c++
void Game::display(Player &player, Tile &tile, bool bDisplayQueue) const {
    clearTerminal();
    std::cout << colorize(player.getColor()) << player.getName() << resetColor << " - Round " << currentRound << ":" << std::endl << std::endl;

    std::cout << "Current Tile:" << std::endl;
    tile.print();
    std::cout << std::endl;

    if (bDisplayQueue) {
        std::cout << "Next Tiles:" << std::endl;
        auto nextTiles = tileQueue.nextTiles();
        tileQueue.printExchangeQueue(nextTiles);
        std::cout << std::endl;
    }

    board.display();
}
```

This method combines all 3 previously discussed printing methods to print the whole state of the game in one swoop :
- It first clears the terminal and prints the name of the current player, colorized, as well as the current round.
- It then calls `Tile:print()`.
- If asked to print the tile queue, through a boolean passed as parameter, it calls `TileQueue::printExchangeQueue()`.
- Finally, it calls `Board:display()`.

---
### 🔧 I/O Utilities

Utility functions are declared in `utils.hpp`, and used throughout the program. They are mostly responsible for Input/Output management, such as setting the terminal to UTF-8, managing colorized output, managing player input, etc.
These functions are all quite straightforward.