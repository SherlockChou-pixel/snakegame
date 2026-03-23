#ifndef _PLAYER_H_
#define _PLAYER_H_ 

#include "game/Room.h"
class Player {
public:
    Player(int id)
    void update()
    void setDirection(Direction dir)
    void setRoom(Room* room)
private:
    int id;
    Direction dir;
};
#endif