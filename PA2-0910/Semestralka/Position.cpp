/* 
 * File:   Position.cpp
 * Author: Enzy
 * 
 * Created on 12. červen 2010, 17:19
 */

#include "Position.h"

tPosition::tPosition() {
    x = y = 0;
}

tPosition::tPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

tPosition::tPosition(const tPosition& orig) {
    x = orig.x;
    y = orig.y;
}

