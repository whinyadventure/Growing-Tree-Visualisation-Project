#include "leaf.h"
#include "branch.h"

Leaf::Leaf(){
    setDirection();
    setDistance();
}

Leaf::~Leaf() {}

void Leaf::setDirection(){
    direction = (rand() % 360)*(PI/180); // range <0:360>
}

void Leaf::setDistance(){
    distance_from_basis = rand() % (int)MAX_TRUNK;
}

float Leaf::getAngle(){
    return angle;
}

float Leaf::getDirection(){
    return direction;
}

float Leaf::getDistance(){
    return distance_from_basis;
}
