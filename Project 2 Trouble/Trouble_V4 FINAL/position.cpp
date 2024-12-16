#include "position.h"
#include <cstdlib>
#include <iostream>

using namespace std;

position::position() {
    p1pos = -1;
    p2pos = -1;
    p3pos = -1;
    p4pos = -1;
}

void position::setp1pos(short input)
{ p1pos = input; }

void position::setp2pos(short input)
{ p2pos = input; }

void position::setp3pos(short input)
{ p3pos = input; }

void position::setp4pos(short input)
{ p4pos = input; }

short position::getpos(int input) const
    { if(input == 1) return p1pos;
    else if(input == 2) return p2pos;
    else if(input == 3) return p3pos;
    else if(input == 4) return p4pos;
    else return -5;}

void position::getpos(char input) const
{cout << p1pos << endl << p2pos << endl << p3pos << endl << p4pos << endl;}

position::position(const position& orig) {
}

position::~position() {
}