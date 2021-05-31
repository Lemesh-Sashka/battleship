#ifndef BATTLESHIP_GAMEMANAGER_H
#define BATTLESHIP_GAMEMANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800


#include "../Menu/Menu.h"
#include "../Grid/Grid.h"
#include "../WinnersManager/WinnerManager.h"
#include "../Help/Help.h"

class GameManager {
private:
//TITLE SDL_Music relations:
    Mix_Music *menuMusic;
    Mix_Music *victoryMusic;


//TITLE SDL output relations:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *bgTexture;
    SDL_Texture *titleTexture;

//TITLE RefreshWarPage:
    void refreshWarPage();

//TITLE Menu relations:
    Menu *menu;

//TITLE Bonuses relations:
    void destroyEnemyShip(Grid *grid);

    void extraShoot(Grid *grid);

//TITLE Grid relations:
    Grid *playerGrid;
    Point setUpPoint;
    Point warPlayerPoint;

    Grid *computerGrid;
    Point warComputerPoint;

    void hideGrids();


//TITLE WinnersManager relations:
    WinnerManager *winnerManager;
    int winnerId;


//TITLE HelpManager relations:
    HelpManager *helpManager;


//TITLE Pages(private):
    void drawTitle();


//TITLE Pages flags(triggers):
    bool toMainMenu;
    bool gameOver;


//TITLE MenuActions(private):
    void outputMenuItems();

    void outputGameMenu();

    void outputBonuses();


//TITLE Ship actions:
    void lowerShip();

    void upperShip();

    void rightShip();

    void leftShip();

    void turnShip();

    int placeShip();

    void removeShip();

    void shootingProcess();

    bool playerShootingProcess();

    void computerShootingProcess();


//TITLE Pages:
    void menuPage();

    void setUpPage();

    void warPage();

    void victoryPage();


public:

//TITLE Constructor & Destructor
    GameManager();

//TITLE Game complicity flag:
    bool isFinished;


//TITLE Mouse coordinates:
    int coordsX;
    int coordsY;


//TITLE SDL_Event:
    SDL_Event event;


//TITLE Play function:
    void Play();


//TITLE Pages(public):
    void loadStartPage();


//TITLE Music:
    void playMainMenuMusic();

    void playVictoryMusic();


//TITLE Menu actions(public):
    int getSelectedMenuItem();

    void selectMenuItem(int menuItemId);

    void selectGameMenuItem(int menuItemId);
};

#endif //BATTLESHIP_GAMEMANAGER_H
