#ifndef BATTLESHIP_BONUS_H
#define BATTLESHIP_BONUS_H

#include <iostream>

class Bonus {
    std::string title;
    bool wasActivated;
    bool isUsed;
};

class DestroyEnemyShip : Bonus {

};

#endif //BATTLESHIP_BONUS_H
