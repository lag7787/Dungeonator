#ifndef CELL
#define CELL
#include "room.h"
#include "generator.h"

class Generator;

class Cell {

    enum orientation {
        VERTICAL = 0,
        HORIZONTAL = 1
    };

    public:
        Cell* r1;
        Cell* r2;
        Room* room;
        Cell(unsigned int input_width, unsigned int input_height, unsigned int input_x, unsigned int input_y);
        void split(unsigned int min_width, unsigned int min_height); // populated r1 and r2
        void buildRooms(Generator* gen);
        void buildConnections();
        void prettyPrint();
        int getHeight(Cell* rm);

    private:
        unsigned int width;
        unsigned int height;
        unsigned int x;
        unsigned int y;

};
#endif