#include "Menu.h"


Menu::Menu() {
    this->selectedMenu = 0;

    this->mainMenuTTF = TTF_OpenFont("ReggaeOne-Regular.ttf", 100);
    this->foreColor = {255, 140, 0};
    this->selectedItemColor = {255, 255, 255};
    this->backColor = {19, 19, 19, 100};

    MenuItem quickGame = {"Quick Game", 260, 70, true};
    MenuItem quit = {"Quit", 120, 70, false};
    menuItems.push_back(quickGame);
    menuItems.push_back(quit);

    MenuItem resume = {"Resume Game", 260, 70, true};
    MenuItem leave = {"Leave", 120, 70, false};
    gameMenuItems.push_back(resume);
    gameMenuItems.push_back(leave);
}

int Menu::getSelectedMenu() {
    return this->selectedMenu;
}

void Menu::setSelectedMenu(int id) {
    this->selectedMenu = id;
}

SDL_Texture *Menu::getTitleTexture(SDL_Renderer *renderer) {
    SDL_Surface *titleSurface = TTF_RenderText_Shaded(
            mainMenuTTF,
            "Battleship",
            foreColor,
            backColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);
    return texture;
}

SDL_Texture *Menu::getBonusTexture(SDL_Renderer *renderer, std::string title) {
    SDL_Surface *titleSurface = TTF_RenderText_Shaded(
            mainMenuTTF,
            title.c_str(),
            foreColor,
            backColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);
    return texture;
}

SDL_Texture *Menu::getActivatedBonusTexture(SDL_Renderer *renderer, std::string title) {
    SDL_Surface *titleSurface = TTF_RenderText_Shaded(
            mainMenuTTF,
            title.c_str(),
            selectedItemColor,
            backColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);
    return texture;
}

SDL_Texture *Menu::getVictoryTitleTexture(SDL_Renderer *renderer) {
    SDL_Surface *titleSurface = TTF_RenderText_Shaded(
            mainMenuTTF,
            "Victory!",
            foreColor,
            backColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);
    return texture;
}

SDL_Texture *Menu::getWinnerTexture(SDL_Renderer *renderer, const int winner) {
    std::string title;
    switch (winner) {
        case PLAYER_ID: {
            title = "Winner: Player!";
            break;
        }
        case COMPUTER_ID: {
            title = "Winner: Computer!";
            break;
        }
        default: {
            title = "Winner: Unknown!";
            break;
        }
    }

    SDL_Surface *titleSurface = TTF_RenderText_Shaded(
            mainMenuTTF,
            title.c_str(),
            foreColor,
            backColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);
    return texture;
}

SDL_Texture *Menu::getMenuItemTexture(SDL_Renderer *renderer, MenuItem menuItem) {
    SDL_Color color = menuItem.isSelected ? selectedItemColor : foreColor;

    SDL_Surface *menuItemSurface = TTF_RenderText_Shaded(
            mainMenuTTF,
            menuItem.text.c_str(),
            color,
            backColor
    );

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, menuItemSurface);
    SDL_FreeSurface(menuItemSurface);

    return texture;
}

void Menu::outputTitle(SDL_Renderer *renderer) {
    SDL_Rect rect = {0, 0, 400, 100};
    SDL_RenderCopy(renderer, getTitleTexture(renderer), NULL, &rect);
}

void Menu::outputVictoryTitle(SDL_Renderer *renderer) {
    SDL_Rect rect = {440, 0, 400, 100};
    SDL_RenderCopy(renderer, getVictoryTitleTexture(renderer), NULL, &rect);
}

void Menu::outputWinner(SDL_Renderer *renderer, const int winner) {
    SDL_Rect rect = {340, 300, 600, 100};
    SDL_RenderCopy(renderer, getWinnerTexture(renderer, winner), NULL, &rect);
}

void Menu::outputMenuItems(SDL_Renderer *renderer) {
    int x = 0,
            y = 300;
    SDL_Rect frame = {x, y};
    for (auto &menuItem : menuItems) {
        frame.w = menuItem.width;
        frame.h = menuItem.height;
        SDL_RenderCopy(renderer, getMenuItemTexture(renderer, menuItem), NULL, &frame);
        frame.y += menuItem.height;
    }
}

void Menu::outputGameMenuItems(SDL_Renderer *renderer) {
    int x = 500,
            y = 300;
    SDL_Rect frame = {x, y};
    for (auto &menuItem : gameMenuItems) {
        frame.w = menuItem.width;
        frame.h = menuItem.height;
        SDL_RenderCopy(renderer, getMenuItemTexture(renderer, menuItem), NULL, &frame);
        frame.y += menuItem.height;
    }
}

void Menu::outputMenuItemsCentered(SDL_Renderer *renderer) {
    int x = 500,
            y = 300;
    SDL_Rect frame = {x, y};
    for (auto &menuItem : menuItems) {
        frame.w = menuItem.width;
        frame.h = menuItem.height;
        SDL_RenderCopy(renderer, getMenuItemTexture(renderer, menuItem), NULL, &frame);
        frame.y += menuItem.height;
    }
}

void Menu::selectMenuItem(int menuItemId) {
    for (auto &menuItem : menuItems) {
        menuItem.isSelected = false;
    }

    menuItems[menuItemId].isSelected = true;
}

void Menu::selectGameMenuItem(int menuItemId) {
    for (auto &menuItem : gameMenuItems) {
        menuItem.isSelected = false;
    }

    gameMenuItems[menuItemId].isSelected = true;
}

