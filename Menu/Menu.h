#ifndef BATTLESHIP_MENU_H
#define BATTLESHIP_MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

#define UNKNOWN_ID -1
#define PLAYER_ID 0
#define COMPUTER_ID 1

struct MenuItem {
    std::string text;
    int width;
    int height;
    bool isSelected;
};

class Menu {
private:
    std::vector<MenuItem> menuItems;
    std::vector<MenuItem> gameMenuItems;
    int selectedMenu;
    TTF_Font *mainMenuTTF;
    SDL_Color foreColor{};
    SDL_Color selectedItemColor{};
    SDL_Color backColor{};

public:
    Menu();

    int getSelectedMenu();

    void setSelectedMenu(int id);

    SDL_Texture *getTitleTexture(SDL_Renderer *renderer);
    SDL_Texture *getVictoryTitleTexture(SDL_Renderer *renderer);
    SDL_Texture *getWinnerTexture(SDL_Renderer *renderer, const int winner);

    SDL_Texture *getMenuItemTexture(SDL_Renderer *renderer, MenuItem menuItem);

    SDL_Texture *getBonusTexture(SDL_Renderer *renderer, std::string title);
    SDL_Texture *getActivatedBonusTexture(SDL_Renderer *renderer, std::string title);

    void outputTitle(SDL_Renderer *renderer);

    void outputVictoryTitle(SDL_Renderer *renderer);

    void outputWinner(SDL_Renderer *renderer, const int winner);

    void outputMenuItems(SDL_Renderer *renderer);

    void outputGameMenuItems(SDL_Renderer *renderer);

    void outputMenuItemsCentered(SDL_Renderer *renderer);

    void selectMenuItem(int menuItemId);

    void selectGameMenuItem(int menuItemId);

    void replaceShip();
};


#endif //BATTLESHIP_MENU_H
