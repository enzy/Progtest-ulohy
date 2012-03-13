/* 
 * File:   Position.h
 * Author: Enzy
 *
 * Created on 12. červen 2010, 17:19
 */

#ifndef POSITION_H
#define	POSITION_H

#include <iostream>
using namespace std;

class tPosition {
public:
    /**
     * Zero position [0, 0]
     */
    tPosition();

    /**
     * Custom position [x, y]
     * @param x
     * @param y
     */
    tPosition(int x, int y);

    /**
     * Copy constructor
     * @param orig
     */
    tPosition(const tPosition& orig);

    /**
     * output in format: x, y
     */
    friend std::ostream & operator<<(std::ostream& os, const tPosition& pos) {
        int x, y;
        x = pos.x;
        y = pos.y;
        os << x << ", " << y;
        return os;
    };

    bool operator==(tPosition pos) const {
        return (x == pos.x && y == pos.y);
    }
    bool operator!=(tPosition pos) const{
        return !(x == pos.x && y == pos.y);
    }

    int x;
    int y;
private:

};
typedef tPosition Position;

#endif	/* POSITION_H */

