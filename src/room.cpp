#include "room.h"

Room::Room(unsigned int input_x1, unsigned int input_y1, unsigned int input_x2, unsigned int input_y2) {

    x1 = input_x1;
    x2 = input_x2;
    y1 = input_y1;
    y2 = input_y2;
}

unsigned int Room::getHeight() {
    return y2 - y1;
}

unsigned int Room::getLength() {
    return x2 - x1;
}