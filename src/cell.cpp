#include "cell.h"
#include <glm/gtc/random.hpp>
#include <iostream>
#include <queue>
#include <iomanip> 
#include <cstdlib>


// going to need other data to specify the positions of these rooms i think
// or just need to specify boundaries better ... make a decicion on wshat width and height mean
// or some sort of relative anchor point (perhpas top left of its boundary)
// bottom left seems best 

Cell::Cell(unsigned int input_width, unsigned int input_height, unsigned int input_x, unsigned int input_y) : r1(nullptr), r2(nullptr), room(nullptr) {
    width = input_width;
    height = input_height;
    x = input_x; // bottom_left x
    y = input_y; // bottom_left y 
}

// recursive split funciton
void Cell::split(unsigned int min_width, unsigned int min_height, orientation choice) {
    
    if (width <= min_width || height <= min_height) {
        return;
    } 

    //std::cout << "width: " << width << " height: " <<  height << std::endl;

    if (choice == HORIZONTAL){
        //std::cout << "horizontal choice!" << std::endl;
        unsigned int half_height = height / 2;
        r1 = new Cell(width, half_height, this->x, this->y);
        r2 = new Cell(width, height - half_height, this->x, this->y + half_height);
        choice = VERTICAL;
    } else {
        //std::cout << "vertical choice!" << std::endl;
        unsigned int half_width = width / 2;
        r1 = new Cell(half_width, height, this->x, this->y);
        r2 = new Cell(width - half_width, height, this->x + half_width, this->y);
        choice = HORIZONTAL;
    }

    r1->split(min_width, min_height, choice);
    r2->split(min_width, min_height, choice);
}

void Cell::buildRooms(Generator* gen) {

// figure out why this breaks when i seed 

    if (!this->r1 && !this->r2) {
        // leaf node

        unsigned int x1 = rand() % width;
        unsigned int y1 = rand() % height;

        unsigned int x2 = x1 + (rand() % (width - x1));
        unsigned int y2 = y1 + (rand() % (height - y1));

        while (x2 - x1 < width/4) {
            x1 = rand() % width;
            x2 = x1 + (rand() % (width - x1));
        }
        
        // i think this could cause an infite look, unlikely, but i think it oculd happen
        while (y2 - y1 < height/4) {
            y1 = rand() % height;
            y2 = y1 + (rand() % (height - y1));
        }

        room = new Room(x + x1, y + y1, x + x2, y + y2);
        // implment better fix
        gen->rooms.push_back(room);
        // we could do the verticy conerstion right here. or maybe just populate within the constructor itself... depends on how were handling abstracitons 
    } else {
        r1->buildRooms(gen);
        r2->buildRooms(gen);
    }
}

void Cell::buildConnections() {

    if (!this->r1 && !this->r2) {
        // no connections to build
        return;
    } else {
        // build connection between two both children (could be groups or other wise ..... how should we handle this )
        // if we building connections between nodes rooms we simply join the rooms, but if were connecting between groups of rooms,
        // we connect to either a connector in that group or room in that group... 

        // could auto populate a connection point? 
        // connection essentialy has to traverse split point defined earlier (e.g. horizontal or vertical a)
        // shoot out a connector until it collides with something from that group? seems expensive      4w  t
        // at most two turns to reach other group 
        // always connect rooms? ->? use the x,y starting point?
        // would hav to find closest leaf?
        // do we even want to generate the coordidors now? shoudl we just start drawing? 
        // should be draw procedurally
    }

}


void Cell::prettyPrint() {

    int height = getHeight(this);
    int maxNodesInLevel = 1 << (height - 1);

    std::queue<Cell*> q;
    q.push(this);

    int level = 0;
    while(!q.empty()) {
        int levelNodes = q.size();
        int spaceBetweenNodes = maxNodesInLevel / (1 << level);

        for (int i = 0; i < spaceBetweenNodes / 2; ++i) {
            std::cout << "  ";

        }

        for (int i = 0; i < levelNodes; ++i) {
            Cell* current = q.front();
            q.pop();

            if (current) {
                std::cout << std::setw(4) << "Cell";
                q.push(current->r1);
                q.push(current->r2);
            } else {
                std::cout << std::setw(4) << " ";
                q.push(nullptr);
                q.push(nullptr);
            }

            for (int j = 0; j < spaceBetweenNodes; ++j) {
                std::cout << "  ";
            }
        }

        std::cout << std::endl;
        std::cout << std::endl;
        level++;
        if (level == height) break;

    }
}

int Cell::getHeight(Cell* rm) {
    if (!rm) {
        return 0;
    }
    return std::max(getHeight(rm->r1), getHeight(rm->r2)) + 1;
}