#include "GameManager.h"

GameManager::GameManager() {
    menu = new Menu();

    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    this->menuMusic = Mix_LoadMUS("mono-bg.wav");
    this->victoryMusic = Mix_LoadMUS("victory.wav");

    window = SDL_CreateWindow(u8"Battleship", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface *bgSurf = IMG_Load("main-menu-bg.bmp");
    SDL_SetColorKey(bgSurf, SDL_TRUE,
                    SDL_MapRGB(bgSurf->format, 255, 255, 255));
    bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurf);
    SDL_FreeSurface(bgSurf);

    titleTexture = menu->getTitleTexture(renderer);

    isFinished = false;
    gameOver = false;
    toMainMenu = false;

    playerGrid = new Grid();
    setUpPoint = {400, 70};
    computerGrid = new Grid();


    warPlayerPoint = {120, 70};
    warComputerPoint = {670, 70};


    winner = UNKNOWN_ID;
}

void GameManager::Play() {
    isFinished = false;

    if (toMainMenu) {
        playerGrid = new Grid();
        warPlayerPoint = {120, 70};

        computerGrid = new Grid();
        warComputerPoint = {670, 70};

        toMainMenu = false;
    }

    setUpPage();

    if (isFinished || toMainMenu)
        return;

    warPage();

    if (isFinished || toMainMenu) {
        return;
    } else if (gameOver) {
        gameOver = !gameOver;
        playVictoryMusic();

        SDL_Delay(1000);

        hideGrids();

        victoryPage();
    }
}


//TITLE Bonuses relations:
void GameManager::destroyEnemyShip(Grid *grid) {
    grid->destroyEnemyShip();

    refreshWarPage();

    gameOver = grid->isGameOver();
}

void GameManager::extraShoot(Grid *grid) {
    if (!grid->hasShootTwice) {
        grid->isExtraShootPressed = true;
        refreshWarPage();
    }
}


//TITLE Pages:
void GameManager::menuPage() {
    int selectedMenuItemId = 0;
    menu->selectGameMenuItem(selectedMenuItemId);

    outputGameMenu();

    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isFinished = true;
                return;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                return;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                selectedMenuItemId = (selectedMenuItemId + 1) % 2;
                selectGameMenuItem(selectedMenuItemId);
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
                selectedMenuItemId -= 1;
                selectedMenuItemId = selectedMenuItemId < 0 ? 0 : selectedMenuItemId;
                selectGameMenuItem(selectedMenuItemId);
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                if (selectedMenuItemId == 1)
                    toMainMenu = true;
                return;
            }
        }
    }
}

void GameManager::setUpPage() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    playerGrid->outputGrid(renderer, setUpPoint);
    SDL_RenderPresent(renderer);

    while (!isFinished) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isFinished = true;
                return;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                menuPage();
                if (toMainMenu || isFinished)
                    return;
                else {
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

                    playerGrid->outputGrid(renderer, setUpPoint);

                    SDL_RenderPresent(renderer);
                }
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {
                lowerShip();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {
                upperShip();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_LEFT) {
                leftShip();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RIGHT) {
                rightShip();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                turnShip();
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                if (placeShip() == -1)
                    return;
            } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT) {
                removeShip();
            }
        }
        SDL_Delay(50);
    }
}

void GameManager::warPage() {
    computerGrid->randomFill();

    refreshWarPage();

    while (!isFinished) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isFinished = true;
                return;
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                menuPage();
                if (isFinished || toMainMenu)
                    return;
                else {
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

                    playerGrid->outputWarGrid(renderer, warPlayerPoint);
                    computerGrid->outputWarGrid(renderer, warComputerPoint);

                    outputBonuses();

                    SDL_RenderPresent(renderer);
                }
            } else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                shoot();
                if (gameOver)
                    return;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_1) {
                destroyEnemyShip(computerGrid);
                if (gameOver) {
                    winner = PLAYER_ID;
                    return;
                }
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_2) {
                extraShoot(playerGrid);
            }
        }
        SDL_Delay(50);
    }

    //TODO Make computer shoot intelligence

    //TODO Add sound effects.

    //TODO Add music handler tab in game menu

    //TODO Create Rules page
}

void GameManager::loadStartPage() {
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
    drawTitle();
    outputMenuItems();
    SDL_RenderPresent(renderer);
}

void GameManager::drawTitle() {
    menu->outputTitle(renderer);
}

void GameManager::victoryPage() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    menu->outputVictoryTitle(renderer);
    menu->outputWinner(renderer, winner);

    SDL_RenderPresent(renderer);

    SDL_Delay(10000);
}

void GameManager::hideGrids() {
    for (int i = warPlayerPoint.y; i < SCREEN_HEIGHT; i += 40) {
        warPlayerPoint.y = warComputerPoint.y = i;

        refreshWarPage();

        SDL_Delay(3);
    }
}


//TITLE Ship actions:
void GameManager::lowerShip() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    playerGrid->lowerActiveShip();

    playerGrid->outputGrid(renderer, setUpPoint);

    SDL_RenderPresent(renderer);
}

void GameManager::upperShip() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    playerGrid->upperActiveShip();

    playerGrid->outputGrid(renderer, setUpPoint);

    SDL_RenderPresent(renderer);
}

void GameManager::rightShip() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    playerGrid->rightActiveShip();

    playerGrid->outputGrid(renderer, setUpPoint);

    SDL_RenderPresent(renderer);
}

void GameManager::leftShip() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    playerGrid->leftActiveShip();

    playerGrid->outputGrid(renderer, setUpPoint);

    SDL_RenderPresent(renderer);
}

void GameManager::turnShip() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    playerGrid->turnActiveShip();
    playerGrid->outputGrid(renderer, setUpPoint);

    SDL_RenderPresent(renderer);
}

int GameManager::placeShip() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    int res = playerGrid->placeActiveShip();

    if (res < 0)
        return res;

    playerGrid->outputGrid(renderer, setUpPoint);
    SDL_RenderPresent(renderer);
    return res;
}

void GameManager::removeShip() {
    SDL_GetMouseState(&coordsX, &coordsY);
    if (playerGrid->isShipPressed(coordsX, coordsY, setUpPoint)) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

        playerGrid->outputGrid(renderer, setUpPoint);
        SDL_RenderPresent(renderer);
    }
}

void GameManager::shoot() {
    SDL_GetMouseState(&coordsX, &coordsY);

    switch (computerGrid->shoot(coordsX, coordsY, warComputerPoint)) {
        case -1: {
            gameOver = true;
            winner = PLAYER_ID;
            return;
        }
        case 0:
            break;
        case 1: {
            if (playerGrid->isExtraShootPressed && !playerGrid->hasShootTwice) {
                playerGrid->hasShootTwice = true;
            } else {
                int res;

                do {
                    res = playerGrid->shootRand();

                    refreshWarPage();

                    SDL_Delay(400);
                } while (res == 2);
                switch (res) {
                    case -1: {
                        gameOver = true;
                        winner = COMPUTER_ID;
                        return;
                    }
                    case 1:
                    default:
                        refreshWarPage();

                        break;
                }
            }
        }
        case 2:
        default:
            refreshWarPage();
            break;
    }
}


//TITLE Music:
void GameManager::playMainMenuMusic() {
    Mix_PlayMusic(menuMusic, -1);
}


void callbackFunc() {
    Mix_PlayMusic(Mix_LoadMUS("mono-bg.wav"), -1);
}

void GameManager::playVictoryMusic() {
    Mix_PlayMusic(victoryMusic, 1);
    Mix_HookMusicFinished(callbackFunc);
}


//TITLE Menu actions:
int GameManager::getSelectedMenuItem() {
    return menu->getSelectedMenu();
}

void GameManager::selectMenuItem(int menuItemId) {
    menu->selectMenuItem(menuItemId);

    SDL_RenderClear(renderer);
    loadStartPage();
}

void GameManager::selectGameMenuItem(int menuItemId) {
    menu->selectGameMenuItem(menuItemId);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
    outputGameMenu();
}

void GameManager::outputMenuItems() {
    menu->outputMenuItems(renderer);
}

void GameManager::outputGameMenu() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    menu->outputGameMenuItems(renderer);
    SDL_RenderPresent(renderer);
}

void GameManager::outputBonuses() {
    SDL_Rect frame{};
    std::string title;
    int len;
    frame.h = 50;
    int ratio = 15;

    //bonus was not activated -> display, otherwise do not display
    if (!computerGrid->isDestroyEnemyShip) {
        title = "1 - Destroy enemy ship.";
        len = title.length();
        frame.x = (SCREEN_WIDTH / 2 - len * ratio) / 2;
        frame.y = SCREEN_HEIGHT - 200;
        frame.w = len * ratio;
        SDL_RenderCopy(renderer, menu->getBonusTexture(renderer, title), NULL, &frame);
    }

    title = "2 - Extra chance.";
    len = title.length();
    frame.x = (SCREEN_WIDTH + SCREEN_WIDTH / 2 - len * ratio) / 2;
    frame.y = SCREEN_HEIGHT - 200;
    frame.w = len * ratio;

    if (!playerGrid->isExtraShootPressed) {
        SDL_RenderCopy(renderer, menu->getBonusTexture(renderer, title), NULL, &frame);
    } else {
        if (!playerGrid->hasShootTwice) {
            SDL_RenderCopy(renderer, menu->getActivatedBonusTexture(renderer, title), NULL, &frame);
        }
    }
}


void GameManager::refreshWarPage() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

    playerGrid->outputWarGrid(renderer, warPlayerPoint);
    computerGrid->outputWarComputerGrid(renderer, warComputerPoint);

    outputBonuses();

    SDL_RenderPresent(renderer);
}

