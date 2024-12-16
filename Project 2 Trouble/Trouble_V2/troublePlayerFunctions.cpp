#include "troublePlayerFunctions.h"
#include <cstdlib>
#include <iostream>

using namespace std;

short troublePlayerFunctions::numObj = 0;

troublePlayerFunctions::troublePlayerFunctions() {
    numObj++;
    color = new char;
    *color = 'N';
    p1pos = -1;
    p2pos = -1;
    p3pos = -1;
    p4pos = -1;
    startScore = 4;
    safeScore = 0;
    piecesInSafe[0] = 0;
    piecesInSafe[1] = 0;
    piecesInSafe[2] = 0;
    piecesInSafe[3] = 0;
}

troublePlayerFunctions::~troublePlayerFunctions()
{ delete [] color; 
  numObj--; }

short troublePlayerFunctions::getpos(int input) const
    { if(input == 1) return p1pos;
    else if(input == 2) return p2pos;
    else if(input == 3) return p3pos;
    else if(input == 4) return p4pos;
    else return -5;}

void troublePlayerFunctions::getpos(char input) const
{cout << p1pos << endl << p2pos << endl << p3pos << endl << p4pos << endl;}

void troublePlayerFunctions::setColor(char input)
{ *color = input; }

void troublePlayerFunctions::setp1pos(short input)
{ p1pos = input; }

void troublePlayerFunctions::setp2pos(short input)
{ p2pos = input; }

void troublePlayerFunctions::setp3pos(short input)
{ p3pos = input; }

void troublePlayerFunctions::setp4pos(short input)
{ p4pos = input; }

void troublePlayerFunctions::setstartScore(short input)
{ startScore = input; }

void troublePlayerFunctions::setsafeScore(short input)
{ safeScore = input; }

void troublePlayerFunctions::setsafeIndex(short input)
{ piecesInSafe[input] = 1; }

troublePlayerFunctions::troublePlayerFunctions(const troublePlayerFunctions& orig) {
}

troublePlayerFunctions troublePlayerFunctions::operator + (const troublePlayerFunctions &right)
{   troublePlayerFunctions temp;
    temp.safeScore = safeScore + 1;
    return temp;
}

troublePlayerFunctions troublePlayerFunctions::operator - (const troublePlayerFunctions &right)
{   troublePlayerFunctions temp;
    temp.safeScore = safeScore - 1;
    return temp;
}

troublePlayerFunctions troublePlayerFunctions::operator * (const troublePlayerFunctions &right)
{   troublePlayerFunctions temp;
    temp.safeScore = 0;
    return temp;
}