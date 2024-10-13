#include "generator.h"
#include "room.h"
#include <iostream>
#include <algorithm>

bool comparator (Room* i, Room* j) {
    return i->x1 < j->x1;
}

Generator::Generator(unsigned int input_width, unsigned int input_height) {
    width = input_width;
    height = input_height;
}

Cell Generator::generate() {
    return Cell(width, height, 0, 0);
}

void Generator::printRooms() {
       // for (int i = 0; i < rooms.size(); i++) {
       //     Room tmpRoom = *(rooms[i]);
       //     std::cout << "room" << i << "-> x1: " <<  tmpRoom.x1 << " y1: " << tmpRoom.y1 << " x2: " << tmpRoom.x2 << " y2: " << tmpRoom.y2 << std::endl;
       // }
}

float* Generator::getVerticies(Room* room) {

    float half_width = width / 2.0f;
    float half_height = height / 2.0f;

    float* verticies = new float[18]();
    // t1
    verticies[0] = static_cast<float>((room->x2 - (half_width)) / half_width); // top right x value
    verticies[1] = static_cast<float>((room->y2 - (half_height)) / half_height);
    verticies[2] = 0.0f;
    verticies[3] = static_cast<float>((room->x2 - (half_width)) / half_width); //bottom right x value
    verticies[4] = static_cast<float>((room->y1 - (half_height)) / half_height); // bottom right y value
    verticies[5] = 0.0f;
    verticies[6] = static_cast<float>((room->x1 - (half_width)) / half_width); // top left x value
    verticies[7] = static_cast<float>((room->y2 - (half_height)) / half_height); // top left y value
    verticies[8] = 0.0f;
    // t2
    verticies[9] = static_cast<float>((room->x2 - (half_width)) / half_width); //bottom right x value
    verticies[10] = static_cast<float>((room->y1 - (half_height)) / half_height); // bottom right y value
    verticies[11] = 0.0f;
    verticies[12] = static_cast<float>((room->x1 - (half_width)) / half_width);
    verticies[13] = static_cast<float>((room->y2 - (half_width)) / half_width);
    verticies[14] = 0.0f;
    verticies[15] = static_cast<float>((room->x1 - (half_width)) / half_width);
    verticies[16] = static_cast<float>((room->y2 - (half_height)) / half_height); // top left y value
    verticies[17] = 0.0f;

    return verticies;
}

void Generator::buildConnectors() {
    std::sort(rooms.begin(),rooms.end(),comparator);
    Room* room;
    Room* nextRoom;
    for (unsigned int i = 0; i < rooms.size()-1; i++) {
        room = rooms[i];
        nextRoom = rooms[i+1];
        unsigned int x1 = room->x1 + (rand() % (room->x2 - room->x1 - 1));
        unsigned int y1 = room->y1 + (rand() % (room->y2 - room->y1 - 1));
        unsigned int x2 = nextRoom->x1 + (rand() % (nextRoom->x2 - nextRoom->x1 - 1));
        unsigned int y2 = nextRoom->y1 + (rand() % (nextRoom->y2 - nextRoom->y1 - 1));
        connections.push_back(new Room(x1, y1, x2, y2));
    }
}


