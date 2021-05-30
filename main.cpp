#include <iostream>
#include "GameManager/GameManager.h"
#include "Menu/Menu.h"

bool isValid() {
    bool errored = false;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "\nSDL init error.\n";
        errored = true;
    }
    if (TTF_Init() < 0) {
        std::cout << "\nSDL ttf error.\n";
        errored = true;
    }
    if (Mix_Init(0) < 0) {
        std::cout << "\nSDL mixer error.\n";
        errored = true;
    }

    return !errored;
}

int main() {
    if (!isValid())
        return 0;


    auto *gm = new GameManager();
    gm->playMainMenuMusic();
    gm->loadStartPage();

    int selectedMenuItem = gm->getSelectedMenuItem();
    while (!gm->isFinished) {

        while (SDL_PollEvent(&gm->event)) {
            gm->isFinished = gm->event.type == SDL_QUIT;

            if (gm->event.type == SDL_KEYDOWN && gm->event.key.keysym.sym == SDLK_DOWN) {
                selectedMenuItem = (selectedMenuItem + 1) % 2;
                gm->selectMenuItem(selectedMenuItem);
            } else if (gm->event.type == SDL_KEYDOWN && gm->event.key.keysym.sym == SDLK_UP) {
                selectedMenuItem -= 1;
                selectedMenuItem = selectedMenuItem < 0 ? 0 : selectedMenuItem;
                gm->selectMenuItem(selectedMenuItem);
            } else if (gm->event.type == SDL_KEYDOWN && gm->event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                if (selectedMenuItem == 0) {
                    gm->Play();
                    gm->loadStartPage();
                }
                else
                    gm->isFinished = true;
            }
        }
        SDL_Delay(50);
    }


    return 0;
}
