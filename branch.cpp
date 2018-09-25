#include "branch.h"

Branch::Branch(Branch *parent_ptr){
    if(parent_ptr == nullptr){ // trunk
        parent = NULL;
        max_length = MAX_TRUNK;
        level = 0;
        randChildren();
        is_trunk = true;
    }
    else { // branches lvl 1-3
        parent = parent_ptr;
        max_length = parent->max_length*LENGTH_COEFF;
        setAngle();
        setDirection();
        setDistance();
        level = parent->level + 1;
        if(level < 3) // if not level 3, then generate children
            randChildren();
        else
            children = 0;
    }
}

Branch::~Branch() {}

void Branch::setAngle(){
    while(angle == 0.0)
        angle = ((rand() % 160) - 80)*(PI/180); // range <-80:80> / {0}
}

void Branch::setDirection(){
    direction = (rand() % 360)*(PI/180); // range <0:360>
}

void Branch::setDistance(){
    distance_from_basis = (parent->max_length - (rand() % (int)(parent->max_length/2))); // upper half of parent branch
}

void Branch::randChildren(){
    children = ((rand() % 3) + 3); // range <3:5>
}

float Branch::getMaxLength(){
    return max_length;
}

float Branch::getAngle(){
    return angle;
}

float Branch::getDirection(){
    return direction;
}

float Branch::getDistance(){
    return distance_from_basis;
}

int Branch::getLvl(){
    return level;
}

int Branch::getChildren(){
    return children;
}

bool Branch::isTrunk(){
    return is_trunk;
}
