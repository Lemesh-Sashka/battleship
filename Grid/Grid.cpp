#include "Grid.h"

//TITLE Constructor & Destructor:
Grid::Grid() {
    for (int col = 0; col < 10; ++col) {
        for (int row = 0; row < 10; ++row) {
            cells[row][col].x = col;
            cells[row][col].y = row;

            cells[row][col].status = FREE;

            cells[row][col].isShot = false;
            cells[row][col].isHovered = false;

            cells[row][col].placeable = true;
        }
    }

    Ship ship{};
    for (int i = 4; i > 0; --i) {
        ship.type = i;
        ship.isSettled = false;
        ship.isKilled = false;

        ship.start = {4, 4};
        ship.end = {4 + ship.type - 1, 4};

        for (int j = 0; j < 5 - i; ++j) {
            ships.push_back(ship);
        }
    }

    activeShip = &ships.front();

    killedShipsCount = 0;

    isDestroyEnemyShip = false;
    isExtraShootPressed = false;
    hasShootTwice = false;

    isShipAimed = false;
    shootingDirection = UNKNOWN_DIRECTION;
    lastShotCell = {
            -1,
            -1,
            -1,
            false,
            false,
            true
    };
    startShotCell = {
            -1,
            -1,
            -1,
            false,
            false,
            true
    };
}


//TITLE Grid output:
void Grid::outputGrid(SDL_Renderer *renderer, const Point startPoint) {
    clearCells();

    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            outputCell(renderer, &cells[row][col], startPoint);
        }
    }
}

void Grid::outputWarGrid(SDL_Renderer *renderer, const Point startPoint) {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            outputWarCell(renderer, &cells[row][col], startPoint);
        }
    }
}

void Grid::outputWarComputerGrid(SDL_Renderer *renderer, const Point startPoint) {
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            outputWarComputerCell(renderer, &cells[row][col], startPoint);
        }
    }
}


//TITLE Bonuses relations (methods, public):
void Grid::destroyEnemyShip() {
    if (!isDestroyEnemyShip) {
        isDestroyEnemyShip = !isDestroyEnemyShip;
        int idx;
        do {
            idx = rand() % 10;
        } while (ships[idx].isKilled);

        ships[idx].isKilled = true;

        if (ships[idx].start.y == ships[idx].end.y) {
            for (int i = 0; i < ships[idx].type; ++i) {
                cells[ships[idx].start.y][ships[idx].start.x + i].isShot = true;
            }
        } else {
            for (int i = 0; i < ships[idx].type; ++i) {
                cells[ships[idx].start.y + i][ships[idx].start.x].isShot = true;
            }
        }

        shootAround(&ships[idx]);
    }
}

bool Grid::extraShoot() {
    hasShootTwice = true;
    isExtraShootPressed = false;
}


//TITLE Ships relations (methods, private):

Ship *Grid::getNewActiveShip() {
    for (auto &ship : ships) {
        if (!ship.isSettled) {
            return &ship;
        }
    }

    return nullptr;
}

bool Grid::canBePlaced() {
    bool res = true;

    for (auto &row : cells) {
        for (auto &cell : row) {
            if (cell.isHovered && !cell.placeable)
                res = false;
        }
    }

    return res;
}

bool Grid::canBeShot(const int x, const int y) {
    return !cells[y][x].isShot;
}

void Grid::surroundShip(const Ship *ship) {
    bool isHor = ship->start.y == ship->end.y;
    Point lctn = ship->start;

    for (int i = 0; i < ship->type; ++i) {
        surroundCell(lctn);

        if (isHor) {
            lctn.x++;
        } else {
            lctn.y++;
        }
    }
}

void Grid::surroundActiveShip() {
    surroundShip(activeShip);
}

void Grid::surroundSettledShips() {
    for (auto &ship: ships) {
        if (ship.isSettled) {

            surroundShip(&ship);
        }
    }
}

void Grid::settleActiveShip() {
    bool isHor = activeShip->start.y == activeShip->end.y;

    for (int i = 0; i < activeShip->type; ++i) {
        if (isHor) {
            cells[activeShip->start.y][activeShip->start.x + i].status = RESERVED;
        } else {
            cells[activeShip->start.y + i][activeShip->start.x].status = RESERVED;
        }
    }
}

void Grid::unsettleActiveShip() {
    bool isHor = activeShip->start.y == activeShip->end.y;

    for (int i = 0; i < activeShip->type; ++i) {
        if (isHor) {
            cells[activeShip->start.y][activeShip->start.x + i].status = FREE;
        } else {
            cells[activeShip->start.y + i][activeShip->start.x].status = FREE;
        }
    }
}

void Grid::unsurroundShip(const Ship *ship) {
    bool isHor = ship->start.y == ship->end.y;
    Point lctn = ship->start;

    for (int i = 0; i < ship->type; ++i) {
        unsurroundCell(lctn);

        if (isHor) {
            lctn.x++;
        } else {
            lctn.y++;
        }
    }
}

void Grid::unsurroundActiveShip() {
    unsurroundShip(activeShip);
}

void Grid::checkShips() {
    bool isKilled = true;
    int counter = 0;

    for (auto &ship : ships) {
        if (ship.start.y == ship.end.y) {
            for (int i = 0; i < ship.type; ++i) {
                if (!cells[ship.start.y][ship.start.x + i].isShot) {
                    isKilled = false;
                    break;
                }
            }
        } else {
            for (int i = 0; i < ship.type; ++i) {
                if (!cells[ship.start.y + i][ship.start.x].isShot) {
                    isKilled = false;
                    break;
                }
            }
        }

        ship.isKilled = isKilled;
        isKilled = true;

        if (ship.isKilled) {
            shootAround(&ship);
            ++counter;
        }
    }

    if (counter != killedShipsCount) {
        killedShipsCount = counter;
        isShipAimed = false;
    }
}

void Grid::shootAround(const Ship *ship) {
    bool isHor = ship->start.y == ship->end.y;
    Point lctn = ship->start;

    for (int i = 0; i < ship->type; ++i) {
        shootAroundCell(lctn);

        if (isHor) {
            lctn.x++;
        } else {
            lctn.y++;
        }
    }

}

//TITLE Ships relations (methods, public):
void Grid::lowerActiveShip() {

    if (activeShip->end.y + 1 <= 9) {
        activeShip->start.y++;
        activeShip->end.y++;
    }
}

void Grid::upperActiveShip() {
    if (activeShip->start.y - 1 >= 0) {
        activeShip->start.y--;
        activeShip->end.y--;
    }
}

void Grid::rightActiveShip() {
    if (activeShip->end.x + 1 <= 9) {
        activeShip->start.x++;
        activeShip->end.x++;
    }
}

void Grid::leftActiveShip() {
    if (activeShip->start.x - 1 >= 0) {
        activeShip->start.x--;
        activeShip->end.x--;
    }
}

void Grid::turnActiveShip() {
    if (activeShip->start.x == activeShip->end.x) {
        activeShip->end.y = activeShip->start.y;
        activeShip->end.x += activeShip->type - 1;

        if (activeShip->end.x > 9) {
            int diff = activeShip->end.x - 9;

            activeShip->start.x -= diff;
            activeShip->end.x -= diff;
        }

        return;
    }

    activeShip->end.x = activeShip->start.x;
    activeShip->end.y += activeShip->type - 1;

    if (activeShip->end.y > 9) {
        int diff = activeShip->end.y - 9;

        activeShip->start.y -= diff;
        activeShip->end.y -= diff;
    }
}

int Grid::placeActiveShip() {
    if (canBePlaced()) {
        activeShip->isSettled = true;

        surroundActiveShip();

        settleActiveShip();

        activeShip = getNewActiveShip();
        return activeShip == NULL ? -1 : 1;
    }

    return 0;
}

void Grid::randomFill() {
    bool isFilled = false;
    int angle = 0;
    int stepStatus = 0;

    while (!isFilled) {
        angle = rand() % 2;

        do {
            activeShip->start.x = rand() % 10;
            activeShip->start.y = rand() % 10;

            if (angle == 0) {
                if (activeShip->start.x + activeShip->type - 1 <= 9) {
                    activeShip->end.y = activeShip->start.y;
                    activeShip->end.x = activeShip->start.x + activeShip->type - 1;
                } else {
                    continue;
                }
            } else {
                if (activeShip->start.y + activeShip->type - 1 <= 9) {
                    activeShip->end.x = activeShip->start.x;
                    activeShip->end.y = activeShip->start.y + activeShip->type - 1;
                } else {
                    continue;
                }
            }

            clearCells();

            for (auto &row : cells) {
                for (auto &cell : row) {
                    if (activeShip->start.x <= cell.x && cell.x <= activeShip->end.x) {
                        if (activeShip->start.y <= cell.y && cell.y <= activeShip->end.y) {
                            cell.isHovered = true;
                        }
                    }
                }
            }

            stepStatus = placeActiveShip();
        } while (stepStatus == 0);

        isFilled = stepStatus == -1;
    }
}

bool Grid::isShipPressed(const int coordsX, const int coordsY, const Point startPoint) {
    int x, y;

    if (startPoint.x <= coordsX && coordsX <= startPoint.x + (CELL_SIZE + 1) * 10) {
        if (startPoint.y <= coordsY && coordsY <= startPoint.y + (CELL_SIZE + 1) * 10) {
            x = (coordsX - startPoint.x) / (CELL_SIZE + 1);
            y = (coordsY - startPoint.y) / (CELL_SIZE + 1);

            for (auto &ship : ships) {
                if (ship.isSettled) {
                    if (ship.start.x <= x && x <= ship.end.x && ship.start.y <= y && y <= ship.end.y) {
                        ship.isSettled = false;
                        activeShip = &ship;

                        unsurroundActiveShip();
                        surroundSettledShips();
                        unsettleActiveShip();

                        return true;
                    }
                }
            }
        }
    }

    return false;
}

int Grid::shoot(const int coordsX, const int coordsY, const Point startPoint) {
    int x, y;

    if (startPoint.x <= coordsX && coordsX <= startPoint.x + (CELL_SIZE + 1) * 10) {
        if (startPoint.y <= coordsY && coordsY <= startPoint.y + (CELL_SIZE + 1) * 10) {
            x = (coordsX - startPoint.x) / (CELL_SIZE + 1);
            y = (coordsY - startPoint.y) / (CELL_SIZE + 1);


            if (canBeShot(x, y)) {
                cells[y][x].isShot = true;

                checkShips();

                if (isGameOver())
                    return -1;
                else
                    return cells[y][x].status == RESERVED ? 2 : 1;
            } else {
                return 0;
            }
        }
    }

    return 0;
}

int Grid::getShotRand(Cell *startShotCell, Cell *lastShotCell) {
    int x;
    int y;

    do {
        x = rand() % 10;
        y = rand() % 10;
    } while (!canBeShot(x, y));

    cells[y][x].isShot = true;
    *startShotCell = *lastShotCell = cells[y][x];

    checkShips();

    if (isGameOver())
        return -1;
    else {
        return cells[y][x].status == RESERVED ? 2 : 1;
    }
}

int Grid::shootNear(Grid *grid) {
//    int x = lastShotCell.x;
//    int y = lastShotCell.y;

    bool hasShot = false,
            upTried = false,
            rightTried = false,
            downTried = false,
            leftTried = false;


    while (!hasShot) {
        switch (shootingDirection) {
            case UP_DIRECTION: {
                if (lastShotCell.y - 1 >= 0 && !grid->cells[lastShotCell.y - 1][lastShotCell.x].isShot) {
//                    --lastShotCell.y;
                    hasShot = true;

                    grid->cells[lastShotCell.y - 1][lastShotCell.x].isShot = true;
                    lastShotCell = grid->cells[lastShotCell.y - 1][lastShotCell.x];

                    if (lastShotCell.status != RESERVED) {
                        lastShotCell = startShotCell;
                        upTried = true;
                        shootingDirection += downTried ? 1 : 0;
                        shootingDirection = (shootingDirection + 2) % 4;
                    }
                } else {
                    upTried = true;
                    shootingDirection += downTried ? 1 : 0;
                    shootingDirection = (shootingDirection + 2) % 4;
                }
                break;
            }
            case RIGHT_DIRECTION: {
                if (lastShotCell.x + 1 <= 9 && !grid->cells[lastShotCell.y][lastShotCell.x + 1].isShot) {
//                    ++lastShotCell.x;
                    hasShot = true;

                    grid->cells[lastShotCell.y][lastShotCell.x].isShot = true;
                    lastShotCell = grid->cells[lastShotCell.y][lastShotCell.x + 1];

                    if (lastShotCell.status != RESERVED) {
                        lastShotCell = startShotCell;
                        rightTried = true;
                        shootingDirection += leftTried ? 1 : 0;
                        shootingDirection = (shootingDirection + 2) % 4;
                    }
                } else {
                    rightTried = true;
                    shootingDirection += leftTried ? 1 : 0;
                    shootingDirection = (shootingDirection + 2) % 4;
                }
                break;
            }
            case DOWN_DIRECTION: {
                if (lastShotCell.y + 1 <= 9 && !grid->cells[lastShotCell.y + 1][lastShotCell.x].isShot) {
//                    ++lastShotCell.y;
                    hasShot = true;

                    grid->cells[lastShotCell.y][lastShotCell.x].isShot = true;
                    lastShotCell = grid->cells[lastShotCell.y + 1][lastShotCell.x];

                    if (lastShotCell.status != RESERVED) {
                        lastShotCell = startShotCell;
                        downTried = true;
                        shootingDirection += upTried ? 1 : 0;
                        shootingDirection = (shootingDirection + 2) % 4;
                    }
                } else {
                    downTried = true;
                    shootingDirection += upTried ? 1 : 0;
                    shootingDirection = (shootingDirection + 2) % 4;
                }
                break;
            }
            case LEFT_DIRECTION: {
                if (lastShotCell.x - 1 >= 0 && !grid->cells[lastShotCell.y][lastShotCell.x - 1].isShot) {
//                    --lastShotCell.x;
                    hasShot = true;

                    grid->cells[lastShotCell.y][lastShotCell.x].isShot = true;
                    lastShotCell = grid->cells[lastShotCell.y][lastShotCell.x - 1];

                    if (lastShotCell.status != RESERVED) {
                        lastShotCell = startShotCell;
                        leftTried = true;
                        shootingDirection += rightTried ? 1 : 0;
                        shootingDirection = (shootingDirection + 2) % 4;
                    }
                } else {
                    leftTried = true;
                    shootingDirection += rightTried ? 1 : 0;
                    shootingDirection = (shootingDirection + 2) % 4;
                }
                break;
            }
            case UNKNOWN_DIRECTION:
            default: {
                shootingDirection = rand() % 4;
                break;
            }
        }
    }

    checkShips();

    if (isGameOver())
        return -1;
    else
        return cells[lastShotCell.y][lastShotCell.x].status == RESERVED ? 2 : 1;
}


//TITLE Cell relations (methods, private):
SDL_Texture *Grid::getCellTexture(SDL_Renderer *renderer, const Cell cell) {
    std::string path;
    if (cell.isHovered) {
        path = !cell.placeable ? "red-zone.bmp" : "free-zone.bmp";
    } else {
        switch (cell.status) {
            case FREE: {
                path = "cell.bmp";
                break;
            }
            case RESERVED: {
                path = "reserved.bmp";
                break;
            }
            default:
                break;
        }
    }


    SDL_Surface *cellSurf = IMG_Load(path.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, cellSurf);

    return texture;
}

SDL_Texture *Grid::getWarCellTexture(SDL_Renderer *renderer, const Cell cell) {
    std::string path;
    if (cell.isShot) {
        switch (cell.status) {
            case FREE: {
                path = "cell-shot.bmp";
                break;
            }
            case RESERVED: {
                path = "reserved-shot.bmp";
                break;
            }
            default:
                break;
        }
    } else {
        switch (cell.status) {
            case FREE: {
                path = "cell.bmp";
                break;
            }
            case RESERVED: {
                path = "reserved.bmp";
                break;
            }
            default:
                break;
        }
    }


    SDL_Surface *cellSurface = IMG_Load(path.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, cellSurface);

    return texture;
}

SDL_Texture *Grid::getWarCellComputerTexture(SDL_Renderer *renderer, const Cell cell) {
    std::string path;
    if (cell.isShot) {
        switch (cell.status) {
            case FREE: {
                path = "cell-shot.bmp";
                break;
            }
            case RESERVED: {
                path = "reserved-shot.bmp";
                break;
            }
            default:
                break;
        }
    } else {
        path = "cell.bmp";
    }


    SDL_Surface *cellSurface = IMG_Load(path.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, cellSurface);

    return texture;
}

void Grid::clearCells() {
    for (auto &row : cells) {
        for (auto &cell : row) {
            cell.isHovered = false;
        }
    }
}

void Grid::outputCell(SDL_Renderer *renderer, Cell *cell, const Point startPoint) {
    int x = startPoint.x + (CELL_SIZE + 1) * cell->x;
    int y = startPoint.y + (CELL_SIZE + 1) * cell->y;

    SDL_Rect cellLayout = {x, y, CELL_SIZE, CELL_SIZE};

    if (activeShip->start.x <= cell->x && cell->x <= activeShip->end.x) {
        if (activeShip->start.y <= cell->y && cell->y <= activeShip->end.y) {
            cell->isHovered = true;
        }
    }

    SDL_RenderCopy(renderer, getCellTexture(renderer, *cell), NULL, &cellLayout);
}

void Grid::outputWarCell(SDL_Renderer *renderer, Cell *cell, const Point startPoint) {
    int x = startPoint.x + (CELL_SIZE + 1) * cell->x;
    int y = startPoint.y + (CELL_SIZE + 1) * cell->y;

    SDL_Rect cellLayout = {x, y, CELL_SIZE, CELL_SIZE};

    SDL_RenderCopy(renderer, getWarCellTexture(renderer, *cell), NULL, &cellLayout);
}

void Grid::outputWarComputerCell(SDL_Renderer *renderer, Cell *cell, const Point startPoint) {
    int x = startPoint.x + (CELL_SIZE + 1) * cell->x;
    int y = startPoint.y + (CELL_SIZE + 1) * cell->y;

    SDL_Rect cellLayout = {x, y, CELL_SIZE, CELL_SIZE};

    SDL_RenderCopy(renderer, getWarCellComputerTexture(renderer, *cell), NULL, &cellLayout);

}

void Grid::surroundCell(const Point cellCoords) {
    bool left = cellCoords.x > 0;
    bool right = cellCoords.x < 9;
    bool up = cellCoords.y > 0;
    bool down = cellCoords.y < 9;

    if (left)
        cells[cellCoords.y][cellCoords.x - 1].placeable = false;
    if (right)
        cells[cellCoords.y][cellCoords.x + 1].placeable = false;

    if (up)
        cells[cellCoords.y - 1][cellCoords.x].placeable = false;
    if (down)
        cells[cellCoords.y + 1][cellCoords.x].placeable = false;

    if (left && up) {
        cells[cellCoords.y - 1][cellCoords.x - 1].placeable = false;
    }
    if (left && down) {
        cells[cellCoords.y + 1][cellCoords.x - 1].placeable = false;
    }
    if (right && up) {
        cells[cellCoords.y - 1][cellCoords.x + 1].placeable = false;
    }
    if (right && down) {
        cells[cellCoords.y + 1][cellCoords.x + 1].placeable = false;
    }

    cells[cellCoords.y][cellCoords.x].placeable = false;
}

void Grid::shootAroundCell(const Point cellCoords) {
    bool left = cellCoords.x > 0;
    bool right = cellCoords.x < 9;
    bool up = cellCoords.y > 0;
    bool down = cellCoords.y < 9;

    if (left)
        cells[cellCoords.y][cellCoords.x - 1].isShot = true;
    if (right)
        cells[cellCoords.y][cellCoords.x + 1].isShot = true;

    if (up)
        cells[cellCoords.y - 1][cellCoords.x].isShot = true;
    if (down)
        cells[cellCoords.y + 1][cellCoords.x].isShot = true;

    if (left && up) {
        cells[cellCoords.y - 1][cellCoords.x - 1].isShot = true;
    }
    if (left && down) {
        cells[cellCoords.y + 1][cellCoords.x - 1].isShot = true;
    }
    if (right && up) {
        cells[cellCoords.y - 1][cellCoords.x + 1].isShot = true;
    }
    if (right && down) {
        cells[cellCoords.y + 1][cellCoords.x + 1].isShot = true;
    }

    cells[cellCoords.y][cellCoords.x].isShot = true;
}

void Grid::unsurroundCell(const Point cellCoords) {
    bool left = cellCoords.x > 0;
    bool right = cellCoords.x < 9;
    bool up = cellCoords.y > 0;
    bool down = cellCoords.y < 9;

    if (left)
        cells[cellCoords.y][cellCoords.x - 1].placeable = true;
    if (right)
        cells[cellCoords.y][cellCoords.x + 1].placeable = true;

    if (up)
        cells[cellCoords.y - 1][cellCoords.x].placeable = true;
    if (down)
        cells[cellCoords.y + 1][cellCoords.x].placeable = true;

    if (left && up) {
        cells[cellCoords.y - 1][cellCoords.x - 1].placeable = true;
    }
    if (left && down) {
        cells[cellCoords.y + 1][cellCoords.x - 1].placeable = true;
    }
    if (right && up) {
        cells[cellCoords.y - 1][cellCoords.x + 1].placeable = true;
    }
    if (right && down) {
        cells[cellCoords.y + 1][cellCoords.x + 1].placeable = true;
    }

    cells[cellCoords.y][cellCoords.x].placeable = true;
}

bool Grid::isGameOver() {
    for (auto &row : cells) {
        for (auto &cell : row) {
            if (!cell.isShot && cell.status == RESERVED) {
                return false;
            }
        }
    }

    return true;
}





