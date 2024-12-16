#include "troublePlayerFunctions.h"
#include <cstdlib>
#include <iostream>

using namespace std;

short troublePlayerFunctions::numObj = 0;

troublePlayerFunctions::troublePlayerFunctions() {
    numObj++;
    color = new char;
    *color = 'N';
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

void troublePlayerFunctions::setColor(char input)
{ *color = input; }

void troublePlayerFunctions::setstartScore(short input)
{ startScore = input; }

void troublePlayerFunctions::setsafeScore(short input)
{ safeScore = input; }

void troublePlayerFunctions::setsafeIndex(short input)
{ piecesInSafe[input] = 1; }

troublePlayerFunctions::troublePlayerFunctions(const troublePlayerFunctions& orig) {
}

void troublePlayerFunctions::incpos1(short a)
{
    short temp;
    
    temp = a + position.getp1pos();
    position.setp1pos(temp);
}

void troublePlayerFunctions::incpos2(short a)
{
    short temp;
    
    temp = a + position.getp2pos();
    position.setp2pos(temp);
}

void troublePlayerFunctions::incpos3(short a)
{
    short temp;
    
    temp = a + position.getp3pos();
    position.setp3pos(temp);
}

void troublePlayerFunctions::incpos4(short a)
{
    short temp;
    
    temp = a + position.getp4pos();
    position.setp4pos(temp);
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