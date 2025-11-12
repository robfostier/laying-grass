# Laying Grass - pCCPP2

*Robinson Fostier*
https://github.com/robfostier/laying-grass.git

---
## User documentation

### About The Project

Laying Grass is a CLI version of a board game from The Devil's Plan (데블스 플랜) Netflix reality game show.
In Laying Grass, players must place grass tiles on a grid-based territory board to expand their territory, with the goal of creating the largest square territory.
This project is educational. This was done as a student project for Supinfo.

---
### Getting started

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
### Game rules

---
## Technical documentation

### Project architecture

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
### Data structures

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

It also contains 12 public methods :
- 4 getters, 1 for `size`, 1 for `placedTiles` and 2 for the cells at specific coordinates.
- `void setCell(std::pair<size_t, size_t> coords, CellType type, Player *owner)` : A setter for cells at specific coordinates.
- `void setup(size_t nbPlayers)` : A method used by the class constructor that sets up the board for a game, taking the number of players as an argument. It allocates `grid` and places bonuses on the board.
- `void placeBonus(CellType bonusType)` : A method that places a bonus on the board, following the game rules.
- `bool canPlaceTile(std::pair<size_t, size_t> coords, const Tile &tile, const Player &player, bool bIsStartingTile) const` : A constant method that returns a bool indicating whether a player can place a specific tile at specific coordinates.
- `bool canPlaceTileAnywhere(const Tile &tile, const Player &player) const` : A constant method that returns a bool indicating whether a player can place a specific tile anywhere on the board.
- `void placeTile(std::pair<size_t, size_t> coords, const Tile &tile, Player *player, bool bStealable)` : A method that places a tile on the board.
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
### Program overview

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
                    if (x == 0 || y == 0)
                        table[x][y] = 1;
                    else
                        table[x][y] = 1 + std::min({table[x - 1][y], table[x][y - 1], table[x - 1][y - 1]});

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
                if (cell.type == GRASS && cell.owner == &player)
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
- After computing `largestSquare`, it uses `bestX`and `bestY` to go back at these coordinates and look in a `largestSquare` size. For each cell in this area, it increments `grassCount` if that cell's type is `GRASS`.
- It then compares `largestSquare` and `grassCount` to `toBeat`. If `largestSquare` beats `toBeat.first` or if it equals it and `grassCount` beats `toBeat.second`, that player has set a new best Score. It updates `toBeat` and store that player as the provisional winner.

Finally, after computing the scores for each player, the method has determined the winner and returns it.
