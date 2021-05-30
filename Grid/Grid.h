#ifndef BATTLESHIP_GRID_H
#define BATTLESHIP_GRID_H

//TITLE Definitions:
#define CELL_SIZE 50

#define FREE 0
#define RESERVED 1

#define UNKNOWN_DIRECTION -1
#define UP_DIRECTION 0
#define RIGHT_DIRECTION 1
#define DOWN_DIRECTION 2
#define LEFT_DIRECTION 3

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>

//TITLE Structs of Grid:

struct Cell {
    int x;
    int y;
    int status;
    bool isHovered;
    bool placeable;
    bool isShot;
};
struct Point {
    int x;
    int y;
};
struct Ship {
    Point start;
    Point end;

    int type;

    bool isSettled;
    bool isKilled;
};


class Grid {
private:
//TITLE Ships relations (variables, private):
    std::vector<Ship> ships;
    Ship *activeShip;
    int killedShipsCount;


//TITLE Ships relations (methods, private):
    Ship *getNewActiveShip();

    bool canBePlaced();

    bool canBeShot(const int x, const int y);

    void surroundShip(const Ship *ship);

    void surroundActiveShip();

    void surroundSettledShips();

    void unsurroundShip(const Ship *ship);

    void unsurroundActiveShip();

    void settleActiveShip();

    void unsettleActiveShip();

    void checkShips();

    void shootAround(const Ship *ship);


//TITLE Cell relations (variables, private):
    Cell cells[10][10];


//TITLE Cell relations (methods, private):
    SDL_Texture *getCellTexture(SDL_Renderer *renderer, const Cell cell);

    SDL_Texture *getWarCellTexture(SDL_Renderer *renderer, const Cell cell);

    SDL_Texture *getWarCellComputerTexture(SDL_Renderer *renderer, const Cell cell);

    void outputCell(SDL_Renderer *renderer, Cell *cell, const Point startPoint);

    void outputWarCell(SDL_Renderer *renderer, Cell *cell, const Point startPoint);

    void outputWarComputerCell(SDL_Renderer *renderer, Cell *cell, const Point startPoint);

    void clearCells();

    void surroundCell(const Point cellCoords);

    void shootAroundCell(const Point cellCoords);

    void unsurroundCell(const Point cellCoords);


public:
//TITLE Constructor & Destructor:
    Grid();

    ~Grid() = default;


//TITLE Ships relations (variables, public):
    bool isShipAimed;
    int shootingDirection;
    Cell lastShotCell;
    Cell startShotCell;


//TITLE Ships relations (methods, public):
    void lowerActiveShip();

    void upperActiveShip();

    void rightActiveShip();

    void leftActiveShip();

    void turnActiveShip();

    int placeActiveShip();

    void randomFill();

    bool isShipPressed(const int coordsX, const int coordsY, const Point startPoint);

    int shoot(const int coordsX, const int coordsY, const Point startPoint);

    int getShotRand(Cell *startShotCell, Cell *lastShotCell);

    int shootNear(Grid *grid);

    bool isGameOver();

//TITLE Bonuses relations (variables, public):
    bool isDestroyEnemyShip;
    bool isExtraShootPressed;
    bool hasShootTwice;

//TITLE Bonuses relations (variables, public):
    void destroyEnemyShip();

    bool extraShoot();

//TITLE Grid output:
    void outputGrid(SDL_Renderer *renderer, const Point startPoint);

    void outputWarGrid(SDL_Renderer *renderer, const Point startPoint);

    void outputWarComputerGrid(SDL_Renderer *renderer, const Point startPoint);
};

#endif //BATTLESHIP_GRID_H
