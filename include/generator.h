#ifndef GENERATOR
#define GENERATOR

#include <vector>
#include "room.h"
#include "cell.h"

class Cell;

class Generator {

    public:
        Generator(unsigned int input_width, unsigned int input_height);
        Cell generate();
        std::vector<Room*> rooms; // may lead to memory leaks... 
        void printRooms();
        float* getVerticies(Room* room);
    private:
        unsigned int width;
        unsigned int height;

};


#endif
