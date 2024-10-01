#ifndef ROOM
#define ROOM

class Room {

    public:
        Room(unsigned int input_x1, unsigned int input_y1, unsigned int input_x2, unsigned int input_y2);
        unsigned int x1;
        unsigned int y1;
        unsigned int x2;
        unsigned int y2;
        unsigned int getHeight();
        unsigned int getLength();
    private:

};

#endif