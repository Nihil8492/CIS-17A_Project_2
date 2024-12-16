#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <cstring>
#include <string>

#include "troublePlayerFunctions.h"
#include "position.h"
#include <algorithm>

using namespace std;

struct player
{
    char color;
    short p1pos = { -1 };
    short p2pos = { -1 };
    short p3pos = { -1 };
    short p4pos = { -1 };
    short startScore = { 4 };
    short safeScore = { 0 };
    short piecesInSafe[4] = {0,0,0,0};
};

struct winnerLine
{
    string winnerName;
    short winnerScore;
};

struct loserLine
{
    short loserScore1;
    short loserScore2;
    short loserScore3;
};

void gameStart(short *);
void mainGameLoop(short *, char *,char, string, player *, bool *);
void prntBoard(short *, player *);
void turn(short *, bool, short, player *);
bool pop(bool);
short roll(bool);
void stompLog(short *, player *, short);
char* scoreSave(short *);
void fOutOpen(fstream *, string);
void fClose(fstream *);
player playerStatus(player *);

void winnerBoard();
void loserBoard();
void nameBoard();
void winnerScoreStore();
void loserScoreStore();

void rules();

int main(int argc, char** argv) {
    char choice;    
    int currentTime = time(0);
    srand(currentTime);    
    short* rcrdPtr = new short[5];
    char* resultCharPtr;
    
    cout << "Welcome to Trouble!" << endl << endl;
    do
    {
        cout << "Please select an option: " << endl;
        cout << "1. Play game." << endl;
        cout << "2. View winner board." << endl;
        cout << "3. View loser board." << endl;
        cout << "4. View the names of those who have played." << endl;
        cout << "5. View the rules of the game." << endl;
        cout << "0. Exit program." << endl;
        
        cin >> choice;
        
        switch(choice)
        {
                case '1': gameStart(rcrdPtr); break;
                case '2': winnerBoard(); break;
                case '3': loserBoard(); break;
                case '4': nameBoard(); break;
                case '5': rules(); break;
                case '0': break;
                default: cout << "Please enter a valid menu option." << endl;
        }
    }while(choice != '0');
    cout << "Exiting game." << endl;
    
    resultCharPtr = scoreSave(rcrdPtr);
    
    delete[] rcrdPtr;
    return 0;
}

void gameStart(short* rcrdPtr)
{
    string name;
    char colorSel;
    bool goodOrder;    
    short* orderPtr = new short[4];
    player* playerList = new player[4];
    troublePlayerFunctions* playerObjList[4];
    char* colorOrd = new char[4];
    bool* playerTurn = new bool[4];
    short tempShort;
    char tempChar;
    bool tempBool;
    player tempPlayer;
    
    *(colorOrd) = 'R';
    *(colorOrd + 1) = 'B';
    *(colorOrd + 2) = 'Y';
    *(colorOrd + 3) = 'G';
    
    *(playerTurn) = false;
    *(playerTurn + 1) = false;
    *(playerTurn + 2) = false;
    *(playerTurn + 3) = false;
    
    cout << "Please enter your name: " << endl;
    cin >> name;
    
    do
    {
        cout << "Please select one of the following colors: " << endl;
        cout << "1. Red" << endl;
        cout << "2. Blue" << endl;
        cout << "3. Yellow" << endl;
        cout << "4. Green" << endl;
        
        cin >> colorSel;
    }while(int(colorSel) < 49|| int(colorSel) > 52);
    
    if(colorSel = '1')
        *(playerTurn) = true;
    else if(colorSel = '2')
        *(playerTurn + 1) = true;
    else if(colorSel = '3')
        *(playerTurn + 2) = true;
    else if(colorSel = '4')
        *(playerTurn + 3) = true;
    
    cout << "Now determining order of players..." << endl;
    
    for(short count = 0; count < 4; count++)
    {
        goodOrder = false;
        while(!(goodOrder))
        {
            goodOrder = true;
            *(orderPtr + count) = ((rand()%6)+1);
            for(short count2 = 0; count2 < 4; count2++)
            {
                if((*(orderPtr + count)==*(orderPtr + count2))&&(count != count2))
                    goodOrder = false;
            }
        }
        
    }
    
    (*(playerList)).color = 'R';
    (*(playerList + 1)).color = 'B';
    (*(playerList + 2)).color = 'Y';
    (*(playerList + 3)).color = 'G';  
    
    for(short maxEl = 3; maxEl > 0; maxEl--)
    {
        for(short index = 0; index < maxEl; index++)
        {
            if(*(orderPtr + index) > *(orderPtr + index + 1))
            {
                tempShort = *(orderPtr + index);
                tempChar = *(colorOrd + index);
                tempBool = *(playerTurn + index);
                tempPlayer = *(playerList + index);
                
                *(orderPtr + index) = *(orderPtr + index + 1);
                *(colorOrd + index) = *(colorOrd + index + 1);
                *(playerTurn + index) = *(playerTurn + index + 1);
                *(playerList + index) = *(playerList + index + 1);
                
                *(orderPtr + index + 1) = tempShort;
                *(colorOrd + index + 1) = tempChar;
                *(playerTurn + index + 1) = tempBool;
                *(playerList + index + 1) = tempPlayer;
            }
        }
    }
    
    cout << "The turn order will be as follows: " << endl;
    for(short count = 0; count < 4; count++)
    {
        cout << "Turn " << count + 1 << " goes to: " << *(colorOrd + count) << endl;
    }
    cout << endl << endl;
    
    mainGameLoop(rcrdPtr, colorOrd, colorSel, name, playerList, playerTurn);
    
    delete[] colorOrd;
    delete[] orderPtr;
    delete[] playerList;
    
    cout << "Game over!" << endl << endl;
}

void mainGameLoop(short *rcrdPtr, char *colorOrd, char colorSel, string name, player *playerList, bool *playerTurn)
{
    player debugPlayer;
    bool debug = false;
    bool win = false;
    bool playerTurnNow = false;
    short whoseTurn;
    short winner;
    //actual board size not counting safe and start zones is 28 spaces
    short* board = new short[28];
    for(int count = 0; count < 2; count++)
    {
        board[count] = 0;
    }
    
    prntBoard(board, playerList);
    
    do
    {
        whoseTurn = 0;
        for(whoseTurn = 0; whoseTurn < 4; whoseTurn++)
        {
            if(debug)
            {
                debugPlayer = playerStatus(playerList);
                cout << "Selected player info: " << endl;
                cout << "Color: " << debugPlayer.color << endl;
                cout << "Positions, 1 to 4: " << debugPlayer.p1pos << " | " 
                        << debugPlayer.p2pos << " | " << debugPlayer.p3pos 
                        << " | " << debugPlayer.p4pos << endl;
                cout << "Start Score: " << debugPlayer.startScore << endl;
                cout << "Safe Score: " << debugPlayer.safeScore << endl;
                cout << "Safe Zone: " << endl;
                for(short count = 0; count < 4; count++)
                {
                    cout << debugPlayer.piecesInSafe[count];
                }
                cout << endl;
            }
            if(win)
                break;
            if(*(playerTurn + whoseTurn))
                prntBoard(board, playerList);
            
            if(*(playerTurn + whoseTurn) == true)
                playerTurnNow = true;
            
            turn(board, playerTurnNow, whoseTurn, playerList);
            playerTurnNow = false;
            
            if((*(playerList + whoseTurn)).safeScore > 3)
            {
                win = true;
                winner = whoseTurn;
            }
        }
    }while(!(win));
    
    delete[] board;
    switch((*(playerList + winner)).color)
    {
        case('R'): cout << endl << "The winner is Red!" << endl; *(rcrdPtr) += 1; break;
        case('B'): cout << endl << "The winner is Blue!" << endl; *(rcrdPtr + 1) += 1; break;
        case('Y'): cout << endl << "The winner is Yellow!" << endl; *(rcrdPtr + 2) += 1; break;
        case('G'): cout << endl << "The winner is Green!" << endl; *(rcrdPtr + 3) += 1; break;
    }
    
    if(*(playerTurn + winner) == true)
    {
        cout << "Congratulations " << name << "!" << endl;
        *(rcrdPtr + 4) += 1;
    }
    
    for(short count = 0; count < 4; count++)
    {
        if(winner != count)
             *(rcrdPtr + count) -= 1;
    }
}

void prntBoard(short *board, player *playerList)
{
    cout << "Turn info: " << endl;
    
    for(int count = 0; count < 4; count++)
    {
        cout << (*(playerList + count)).color << " status: " << endl;
        cout << "Start Area: " << (*(playerList + count)).startScore << " pieces in start." << endl;
        
        cout << "Safe Area: " << endl;
        for(int count2 = 0; count2 < 4; count2++)
        {
            cout << *((*(playerList + count)).piecesInSafe + count2);
        }
        //cout << endl << (*(playerList + count)).p1pos << " | " << (*(playerList + count)).p2pos << " | " << (*(playerList + count)).p3pos << " | " << (*(playerList + count)).p4pos;
        cout << endl << endl;
    }
    
    //printing top row
    for(int count = 0; count < 7; count++)
    {
        if(*(board + count) == 0)
            cout << *(board + count) << " ";
        else
            cout << char(*(board + count)) << " ";
    }
    cout << endl;
    
    //printing middle rows
    for(int count = 7; count < 14; count++)
    {
        if(*(board + 34 - count) == 0)
            cout << *(board + 34 - count);
        else
            cout << char(*(board + 34 - count));
        if(*(board + count) == 0)
            cout << setw(12) << *(board + count) << endl;
        else
            cout << setw(12) << char(*(board + count)) << endl;
    }
    
    //printing bottom row
    for(int count = 20; count > 13; count--)
    {
        if(*(board + count) == 0)
            cout << *(board + count) << " ";
        else
            cout << char(*(board + count)) << " ";
    }
    cout << endl;
}

void turn(short *board,bool playerTurn, short whoseTurn, player *playerList)
{
    enum turn { turn1, turn2, turn3, turn4 };
    short startPos, tempPos, tempRoll, specPos, safePos;    
    bool stomp, block, stopRolling, safe, popRes;
    
    //calculating start position on board based on color
    if((*(playerList + whoseTurn)).color == 'R')
        startPos = 0;
    else if((*(playerList + whoseTurn)).color == 'B')
        startPos = 6;
    else if((*(playerList + whoseTurn)).color == 'Y')
        startPos = 13;
    else if((*(playerList + whoseTurn)).color == 'G')
        startPos = 20;
    
    if(playerTurn)
    {
        //for loop checking each player's pieces from 1 to 4 in sequence, and ending turn when done
        for(short pieceCount = turn1; pieceCount <= turn4; pieceCount++)
        {
            stomp = false;
            block = false;
            safe = false;
            popRes = false;
            stopRolling = false;
            
            if(pieceCount == 0)
            {
                //checking for if the piece is safe, skipping to next for loop iteration if true
                if((*(playerList + whoseTurn)).p1pos == -2)
                    continue;
                //checking if piece is on the board, doing pop logic if it isn't AND the start space is clear
                else if(((*(playerList + whoseTurn)).p1pos == -1)&&((*(board + startPos)) == 0))
                {
                    cout << "Now popping for piece 1." << endl;
                    popRes = pop(playerTurn);
                    //putting the piece on the board if the pop succeeds
                    if(popRes)
                    {
                        (*(playerList + whoseTurn)).startScore -= 1;
                        (*(board + startPos)) = short((*(playerList + whoseTurn)).color);
                        (*(playerList + whoseTurn)).p1pos = 0;
                    }
                }
                //checking if the piece is on the board
                if((*(playerList + whoseTurn)).p1pos >= 0)
                {
                    //launching into movement logic if the piece is on the board
                    cout << "Now rolling for piece 1." << endl;
                    do
                    {
                        //rolling
                        tempRoll = roll(playerTurn);
                        if(tempRoll < 6)
                            stopRolling = true;
                        //calculating resulting position
                        tempPos = (*(playerList + whoseTurn)).p1pos + startPos;
                        specPos = tempPos + tempRoll;
                        
                        //determining if the safe zone is in play
                        if(specPos > 27)
                        {
                            safe = true;
                            stopRolling = true;
                            safePos = specPos - 28;
                            //going into safe zone logic
                            if(safePos > 3)
                            {
                                block = true;
                            }
                            else if(*((*(playerList + whoseTurn)).piecesInSafe + safePos) == 0)
                            {
                                *((*(playerList + whoseTurn)).piecesInSafe + safePos) = 1;
                                (*(playerList + whoseTurn)).p1pos = -2;
                                (*(playerList + whoseTurn)).safeScore += 1;
                                *(board + tempPos) = 0;
                            }
                            else
                                block = true;
                        }
                        
                        //determining if a block is in play
                        else if(*(board + tempPos) == *(board + specPos))
                            block = true;
                        //determining if a stomp is in play
                        else if((*(board + tempPos) != *(board + specPos))&&(*(board + specPos) != 0)&&!(safe))
                            stomp = true;
                        
                        //this is where all non safe zone movement occurs
                        if(block)
                        {
                            cout << "That space is blocked by your own piece, or out of bounds for a safe zone." << endl;
                            stopRolling = true;
                        }
                        else if(stomp)
                        {
                             stompLog(board, playerList, specPos);
                             *(board + tempPos) = 0;
                             *(board + specPos) = int((*(playerList + whoseTurn)).color);
                             (*(playerList + whoseTurn)).p1pos += tempRoll;
                        }
                        else if(!safe)
                        {
                            *(board + tempPos) = 0;
                            *(board + specPos) = int((*(playerList + whoseTurn)).color);
                            (*(playerList + whoseTurn)).p1pos += tempRoll;
                        }
                    }while(!(stopRolling));
                }
            }
            else if(pieceCount == 1)
            {
                //checking for if the piece is safe, skipping to next for loop iteration if true
                if((*(playerList + whoseTurn)).p2pos == -2)
                    continue;
                //checking if piece is on the board, doing pop logic if it isn't AND the start space is clear
                else if(((*(playerList + whoseTurn)).p2pos == -1)&&((*(board + startPos)) == 0))
                {
                    cout << "Now popping for piece 2." << endl;
                    popRes = pop(playerTurn);
                    //putting the piece on the board if the pop succeeds
                    if(popRes)
                    {
                        (*(playerList + whoseTurn)).startScore -= 1;
                        (*(board + startPos)) = short((*(playerList + whoseTurn)).color);
                        (*(playerList + whoseTurn)).p2pos = 0;
                    }
                }
                //checking if the piece is on the board
                if((*(playerList + whoseTurn)).p2pos >= 0)
                {
                    //launching into movement logic if the piece is on the board
                    cout << "Now rolling for piece 2." << endl;
                    do
                    {
                        //rolling
                        tempRoll = roll(playerTurn);
                        if(tempRoll < 6)
                            stopRolling = true;
                        //calculating resulting position
                        tempPos = (*(playerList + whoseTurn)).p2pos + startPos;
                        specPos = tempPos + tempRoll;
                        
                        //determining if the safe zone is in play
                        if(specPos > 27)
                        {
                            safe = true;
                            stopRolling = true;
                            safePos = specPos - 28;
                            //going into safe zone logic
                            if(safePos > 3)
                            {
                                block = true;
                            }
                            else if(*((*(playerList + whoseTurn)).piecesInSafe + safePos) == 0)
                            {
                                *((*(playerList + whoseTurn)).piecesInSafe + safePos) = 1;
                                (*(playerList + whoseTurn)).p2pos = -2;
                                (*(playerList + whoseTurn)).safeScore += 1;
                                *(board + tempPos) = 0;
                            }
                            else
                                block = true;
                        }
                        
                        //determining if a block is in play
                        else if(*(board + tempPos) == *(board + specPos))
                            block = true;
                        //determining if a stomp is in play
                        else if((*(board + tempPos) != *(board + specPos))&&(*(board + specPos) != 0)&&!(safe))
                            stomp = true;
                        
                        //this is where all non safe zone movement occurs
                        if(block)
                        {
                            cout << "That space is blocked by your own piece, or out of bounds for a safe zone." << endl;
                            stopRolling = true;
                        }
                        else if(stomp)
                        {
                             stompLog(board, playerList, specPos);
                             *(board + tempPos) = 0;
                             *(board + specPos) = int((*(playerList + whoseTurn)).color);
                             (*(playerList + whoseTurn)).p2pos += tempRoll;
                        }
                        else if(!safe)
                        {
                            *(board + tempPos) = 0;
                            *(board + specPos) = int((*(playerList + whoseTurn)).color);
                            (*(playerList + whoseTurn)).p2pos += tempRoll;
                        }
                    }while(!(stopRolling));
                }
            }
            else if(pieceCount == 2)
            {
                //checking for if the piece is safe, skipping to next for loop iteration if true
                if((*(playerList + whoseTurn)).p3pos == -2)
                    continue;
                //checking if piece is on the board, doing pop logic if it isn't AND the start space is clear
                else if(((*(playerList + whoseTurn)).p3pos == -1)&&((*(board + startPos)) == 0))
                {
                    cout << "Now popping for piece 3." << endl;
                    popRes = pop(playerTurn);
                    //putting the piece on the board if the pop succeeds
                    if(popRes)
                    {
                        (*(playerList + whoseTurn)).startScore -= 1;
                        (*(board + startPos)) = short((*(playerList + whoseTurn)).color);
                        (*(playerList + whoseTurn)).p3pos = 0;
                    }
                }
                //checking if the piece is on the board
                if((*(playerList + whoseTurn)).p3pos >= 0)
                {
                    //launching into movement logic if the piece is on the board
                    cout << "Now rolling for piece 3." << endl;
                    do
                    {
                        //rolling
                        tempRoll = roll(playerTurn);
                        if(tempRoll < 6)
                            stopRolling = true;
                        //calculating resulting position
                        tempPos = (*(playerList + whoseTurn)).p3pos + startPos;
                        specPos = tempPos + tempRoll;
                        
                        //determining if the safe zone is in play
                        if(specPos > 27)
                        {
                            safe = true;
                            stopRolling = true;
                            safePos = specPos - 28;
                            //going into safe zone logic
                            if(safePos > 3)
                            {
                                block = true;
                            }
                            else if(*((*(playerList + whoseTurn)).piecesInSafe + safePos) == 0)
                            {
                                *((*(playerList + whoseTurn)).piecesInSafe + safePos) = 1;
                                (*(playerList + whoseTurn)).p3pos = -2;
                                (*(playerList + whoseTurn)).safeScore += 1;
                                *(board + tempPos) = 0;
                            }
                            else
                                block = true;
                        }
                        
                        //determining if a block is in play
                        else if(*(board + tempPos) == *(board + specPos))
                            block = true;
                        //determining if a stomp is in play
                        else if((*(board + tempPos) != *(board + specPos))&&(*(board + specPos) != 0)&&!(safe))
                            stomp = true;
                        
                        //this is where all non safe zone movement occurs
                        if(block)
                        {
                            cout << "That space is blocked by your own piece, or out of bounds for a safe zone." << endl;
                            stopRolling = true;
                        }
                        else if(stomp)
                        {
                             stompLog(board, playerList, specPos);
                             *(board + tempPos) = 0;
                             *(board + specPos) = int((*(playerList + whoseTurn)).color);
                             (*(playerList + whoseTurn)).p3pos += tempRoll;
                        }
                        else if(!safe)
                        {
                            *(board + tempPos) = 0;
                            *(board + specPos) = int((*(playerList + whoseTurn)).color);
                            (*(playerList + whoseTurn)).p3pos += tempRoll;
                        }
                    }while(!(stopRolling));
                }
            }
            else if(pieceCount == 3)
            {
                //checking for if the piece is safe, skipping to next for loop iteration if true
                if((*(playerList + whoseTurn)).p4pos == -2)
                    continue;
                //checking if piece is on the board, doing pop logic if it isn't AND the start space is clear
                else if(((*(playerList + whoseTurn)).p4pos == -1)&&((*(board + startPos)) == 0))
                {
                    cout << "Now popping for piece 4." << endl;
                    popRes = pop(playerTurn);
                    //putting the piece on the board if the pop succeeds
                    if(popRes)
                    {
                        (*(playerList + whoseTurn)).startScore -= 1;
                        (*(board + startPos)) = short((*(playerList + whoseTurn)).color);
                        (*(playerList + whoseTurn)).p4pos = 0;
                    }
                }
                //checking if the piece is on the board
                if((*(playerList + whoseTurn)).p4pos >= 0)
                {
                    //launching into movement logic if the piece is on the board
                    cout << "Now rolling for piece 4." << endl;
                    do
                    {
                        //rolling
                        tempRoll = roll(playerTurn);
                        if(tempRoll < 6)
                            stopRolling = true;
                        //calculating resulting position
                        tempPos = (*(playerList + whoseTurn)).p4pos + startPos;
                        specPos = tempPos + tempRoll;
                        
                        //determining if the safe zone is in play
                        if(specPos > 27)
                        {
                            safe = true;
                            stopRolling = true;
                            safePos = specPos - 28;
                            //going into safe zone logic
                            if(safePos > 3)
                            {
                                block = true;
                            }
                            else if(*((*(playerList + whoseTurn)).piecesInSafe + safePos) == 0)
                            {
                                *((*(playerList + whoseTurn)).piecesInSafe + safePos) = 1;
                                (*(playerList + whoseTurn)).p4pos = -2;
                                (*(playerList + whoseTurn)).safeScore += 1;
                                *(board + tempPos) = 0;
                            }
                            else
                                block = true;
                        }
                        
                        //determining if a block is in play
                        else if(*(board + tempPos) == *(board + specPos))
                            block = true;
                        //determining if a stomp is in play
                        else if((*(board + tempPos) != *(board + specPos))&&(*(board + specPos) != 0)&&!(safe))
                            stomp = true;
                        
                        //this is where all non safe zone movement occurs
                        if(block)
                        {
                            cout << "That space is blocked by your own piece, or out of bounds for a safe zone." << endl;
                            stopRolling = true;
                        }
                        else if(stomp)
                        {
                             stompLog(board, playerList, specPos);
                             *(board + tempPos) = 0;
                             *(board + specPos) = int((*(playerList + whoseTurn)).color);
                             (*(playerList + whoseTurn)).p4pos += tempRoll;
                        }
                        else if(!safe)
                        {
                            *(board + tempPos) = 0;
                            *(board + specPos) = int((*(playerList + whoseTurn)).color);
                            (*(playerList + whoseTurn)).p4pos += tempRoll;
                        }
                    }while(!(stopRolling));
                }
            }
        }
    }
    else
    {
        //for loop checking each player's pieces from 1 to 4 in sequence, and ending turn when done
        for(short pieceCount = 0; pieceCount < 4; pieceCount++)
        {
            stomp = false;
            block = false;
            safe = false;
            popRes = false;
            stopRolling = false;
            
            if(pieceCount == 0)
            {
                //checking for if the piece is safe, skipping to next for loop iteration if true
                if((*(playerList + whoseTurn)).p1pos == -2)
                    continue;
                //checking if piece is on the board, doing pop logic if it isn't AND the start space is clear
                else if(((*(playerList + whoseTurn)).p1pos == -1)&&((*(board + startPos)) == 0))
                {
                    popRes = pop(playerTurn);
                    //putting the piece on the board if the pop succeeds
                    if(popRes)
                    {
                        (*(playerList + whoseTurn)).startScore -= 1;
                        (*(board + startPos)) = short((*(playerList + whoseTurn)).color);
                        (*(playerList + whoseTurn)).p1pos = 0;
                    }
                }
                //checking if the piece is on the board
                if((*(playerList + whoseTurn)).p1pos >= 0)
                {
                    //launching into movement logic if the piece is on the board
                    do
                    {
                        //rolling
                        tempRoll = roll(playerTurn);
                        if(tempRoll < 6)
                            stopRolling = true;
                        //calculating resulting position
                        tempPos = (*(playerList + whoseTurn)).p1pos + startPos;
                        specPos = tempPos + tempRoll;
                        
                        //determining if the safe zone is in play
                        if(specPos > 27)
                        {
                            safe = true;
                            stopRolling = true;
                            safePos = specPos - 28;
                            //going into safe zone logic
                            if(safePos > 3)
                            {
                                block = true;
                            }
                            else if(*((*(playerList + whoseTurn)).piecesInSafe + safePos) == 0)
                            {
                                *((*(playerList + whoseTurn)).piecesInSafe + safePos) = 1;
                                (*(playerList + whoseTurn)).p1pos = -2;
                                (*(playerList + whoseTurn)).safeScore += 1;
                                *(board + tempPos) = 0;
                            }
                            else
                                block = true;
                        }
                        
                        //determining if a block is in play
                        else if(*(board + tempPos) == *(board + specPos))
                            block = true;
                        //determining if a stomp is in play
                        else if((*(board + tempPos) != *(board + specPos))&&(*(board + specPos) != 0)&&!(safe))
                            stomp = true;
                        
                        //this is where all non safe zone movement occurs
                        if(block)
                        {
                            stopRolling = true;
                        }
                        else if(stomp)
                        {
                             stompLog(board, playerList, specPos);
                             *(board + tempPos) = 0;
                             *(board + specPos) = int((*(playerList + whoseTurn)).color);
                             (*(playerList + whoseTurn)).p1pos += tempRoll;
                        }
                        else if(!safe)
                        {
                            *(board + tempPos) = 0;
                            *(board + specPos) = int((*(playerList + whoseTurn)).color);
                            (*(playerList + whoseTurn)).p1pos += tempRoll;
                        }
                    }while(!(stopRolling));
                }
            }
            else if(pieceCount == 1)
            {
                //checking for if the piece is safe, skipping to next for loop iteration if true
                if((*(playerList + whoseTurn)).p2pos == -2)
                    continue;
                //checking if piece is on the board, doing pop logic if it isn't AND the start space is clear
                else if(((*(playerList + whoseTurn)).p2pos == -1)&&((*(board + startPos)) == 0))
                {
                    popRes = pop(playerTurn);
                    //putting the piece on the board if the pop succeeds
                    if(popRes)
                    {
                        (*(playerList + whoseTurn)).startScore -= 1;
                        (*(board + startPos)) = short((*(playerList + whoseTurn)).color);
                        (*(playerList + whoseTurn)).p2pos = 0;
                    }
                }
                //checking if the piece is on the board
                if((*(playerList + whoseTurn)).p2pos >= 0)
                {
                    //launching into movement logic if the piece is on the board
                    do
                    {
                        //rolling
                        tempRoll = roll(playerTurn);
                        if(tempRoll < 6)
                            stopRolling = true;
                        //calculating resulting position
                        tempPos = (*(playerList + whoseTurn)).p2pos + startPos;
                        specPos = tempPos + tempRoll;
                        
                        //determining if the safe zone is in play
                        if(specPos > 27)
                        {
                            safe = true;
                            stopRolling = true;
                            safePos = specPos - 28;
                            //going into safe zone logic
                            if(safePos > 3)
                            {
                                block = true;
                            }
                            else if(*((*(playerList + whoseTurn)).piecesInSafe + safePos) == 0)
                            {
                                *((*(playerList + whoseTurn)).piecesInSafe + safePos) = 1;
                                (*(playerList + whoseTurn)).p2pos = -2;
                                (*(playerList + whoseTurn)).safeScore += 1;
                                *(board + tempPos) = 0;
                            }
                            else
                                block = true;
                        }
                        
                        //determining if a block is in play
                        else if(*(board + tempPos) == *(board + specPos))
                            block = true;
                        //determining if a stomp is in play
                        else if((*(board + tempPos) != *(board + specPos))&&(*(board + specPos) != 0)&&!(safe))
                            stomp = true;
                        
                        //this is where all non safe zone movement occurs
                        if(block)
                        {
                            stopRolling = true;
                        }
                        else if(stomp)
                        {
                             stompLog(board, playerList, specPos);
                             *(board + tempPos) = 0;
                             *(board + specPos) = int((*(playerList + whoseTurn)).color);
                             (*(playerList + whoseTurn)).p2pos += tempRoll;
                        }
                        else if(!safe)
                        {
                            *(board + tempPos) = 0;
                            *(board + specPos) = int((*(playerList + whoseTurn)).color);
                            (*(playerList + whoseTurn)).p2pos += tempRoll;
                        }
                    }while(!(stopRolling));
                }
            }
            else if(pieceCount == 2)
            {
                //checking for if the piece is safe, skipping to next for loop iteration if true
                if((*(playerList + whoseTurn)).p3pos == -2)
                    continue;
                //checking if piece is on the board, doing pop logic if it isn't AND the start space is clear
                else if(((*(playerList + whoseTurn)).p3pos == -1)&&((*(board + startPos)) == 0))
                {
                    popRes = pop(playerTurn);
                    //putting the piece on the board if the pop succeeds
                    if(popRes)
                    {
                        (*(playerList + whoseTurn)).startScore -= 1;
                        (*(board + startPos)) = short((*(playerList + whoseTurn)).color);
                        (*(playerList + whoseTurn)).p3pos = 0;
                    }
                }
                //checking if the piece is on the board
                if((*(playerList + whoseTurn)).p3pos >= 0)
                {
                    //launching into movement logic if the piece is on the board
                    do
                    {
                        //rolling
                        tempRoll = roll(playerTurn);
                        if(tempRoll < 6)
                            stopRolling = true;
                        //calculating resulting position
                        tempPos = (*(playerList + whoseTurn)).p3pos + startPos;
                        specPos = tempPos + tempRoll;
                        
                        //determining if the safe zone is in play
                        if(specPos > 27)
                        {
                            safe = true;
                            stopRolling = true;
                            safePos = specPos - 28;
                            //going into safe zone logic
                            if(safePos > 3)
                            {
                                block = true;
                            }
                            else if(*((*(playerList + whoseTurn)).piecesInSafe + safePos) == 0)
                            {
                                *((*(playerList + whoseTurn)).piecesInSafe + safePos) = 1;
                                (*(playerList + whoseTurn)).p3pos = -2;
                                (*(playerList + whoseTurn)).safeScore += 1;
                                *(board + tempPos) = 0;
                            }
                            else
                                block = true;
                        }
                        
                        //determining if a block is in play
                        else if(*(board + tempPos) == *(board + specPos))
                            block = true;
                        //determining if a stomp is in play
                        else if((*(board + tempPos) != *(board + specPos))&&(*(board + specPos) != 0)&&!(safe))
                            stomp = true;
                        
                        //this is where all non safe zone movement occurs
                        if(block)
                        {
                            stopRolling = true;
                        }
                        else if(stomp)
                        {
                             stompLog(board, playerList, specPos);
                             *(board + tempPos) = 0;
                             *(board + specPos) = int((*(playerList + whoseTurn)).color);
                             (*(playerList + whoseTurn)).p3pos += tempRoll;
                        }
                        else if(!safe)
                        {
                            *(board + tempPos) = 0;
                            *(board + specPos) = int((*(playerList + whoseTurn)).color);
                            (*(playerList + whoseTurn)).p3pos += tempRoll;
                        }
                    }while(!(stopRolling));
                }
            }
            else if(pieceCount == 3)
            {
                //checking for if the piece is safe, skipping to next for loop iteration if true
                if((*(playerList + whoseTurn)).p4pos == -2)
                    continue;
                //checking if piece is on the board, doing pop logic if it isn't AND the start space is clear
                else if(((*(playerList + whoseTurn)).p4pos == -1)&&((*(board + startPos)) == 0))
                {
                    popRes = pop(playerTurn);
                    //putting the piece on the board if the pop succeeds
                    if(popRes)
                    {
                        (*(playerList + whoseTurn)).startScore -= 1;
                        (*(board + startPos)) = short((*(playerList + whoseTurn)).color);
                        (*(playerList + whoseTurn)).p4pos = 0;
                    }
                }
                //checking if the piece is on the board
                if((*(playerList + whoseTurn)).p4pos >= 0)
                {
                    //launching into movement logic if the piece is on the board
                    do
                    {
                        //rolling
                        tempRoll = roll(playerTurn);
                        if(tempRoll < 6)
                            stopRolling = true;
                        //calculating resulting position
                        tempPos = (*(playerList + whoseTurn)).p4pos + startPos;
                        specPos = tempPos + tempRoll;
                        
                        //determining if the safe zone is in play
                        if(specPos > 27)
                        {
                            safe = true;
                            stopRolling = true;
                            safePos = specPos - 28;
                            //going into safe zone logic
                            if(safePos > 3)
                            {
                                block = true;
                            }
                            else if(*((*(playerList + whoseTurn)).piecesInSafe + safePos) == 0)
                            {
                                *((*(playerList + whoseTurn)).piecesInSafe + safePos) = 1;
                                (*(playerList + whoseTurn)).p4pos = -2;
                                (*(playerList + whoseTurn)).safeScore += 1;
                                *(board + tempPos) = 0;
                            }
                            else
                                block = true;
                        }
                        
                        //determining if a block is in play
                        else if(*(board + tempPos) == *(board + specPos))
                            block = true;
                        //determining if a stomp is in play
                        else if((*(board + tempPos) != *(board + specPos))&&(*(board + specPos) != 0)&&!(safe))
                            stomp = true;
                        
                        //this is where all non safe zone movement occurs
                        if(block)
                        {
                            stopRolling = true;
                        }
                        else if(stomp)
                        {
                             stompLog(board, playerList, specPos);
                             *(board + tempPos) = 0;
                             *(board + specPos) = int((*(playerList + whoseTurn)).color);
                             (*(playerList + whoseTurn)).p4pos += tempRoll;
                        }
                        else if(!safe)
                        {
                            *(board + tempPos) = 0;
                            *(board + specPos) = int((*(playerList + whoseTurn)).color);
                            (*(playerList + whoseTurn)).p4pos += tempRoll;
                        }
                    }while(!(stopRolling));
                }
            }
        }
    }
}

player playerStatus(player * playerArray)
{
    player tempPlayer;
    short choice;
    
    cout << "Please select which player you would like to view the full status of.";
    cin >> choice;
    
    tempPlayer.color = ((*(playerArray + choice)).color);
    tempPlayer.p1pos = ((*(playerArray + choice)).p1pos);
    tempPlayer.p2pos = ((*(playerArray + choice)).p2pos);
    tempPlayer.p3pos = ((*(playerArray + choice)).p3pos);
    tempPlayer.p4pos = ((*(playerArray + choice)).p4pos);
    tempPlayer.startScore = ((*(playerArray + choice)).startScore);
    tempPlayer.safeScore = ((*(playerArray + choice)).safeScore);
    tempPlayer.piecesInSafe[0] = (*((*(playerArray + choice)).piecesInSafe + 0));
    tempPlayer.piecesInSafe[1] = (*((*(playerArray + choice)).piecesInSafe + 1));
    tempPlayer.piecesInSafe[2] = (*((*(playerArray + choice)).piecesInSafe + 2));
    tempPlayer.piecesInSafe[3] = (*((*(playerArray + choice)).piecesInSafe + 3));
    
    return tempPlayer;
}

bool pop(bool playerTurn)
{
    bool result = false;
    short roll;
    char input;
    
    if(playerTurn)
    {
        cout << "Roll to try and pop a piece out of start (enter 'Y')." << endl;
        do
        {
            cin >> input;
        }while(input != 'Y');
        
        roll = (rand()%6) + 1;
        
        cout << "You rolled: " << roll << endl << endl;
        if(roll == 6)
            result = true;
    }
    else
    {
        roll = (rand()%6) + 1;
        if(roll == 6)
            result = true;
    }
    
    return result;
}

short roll(bool playerTurn)
{
    short roll;
    char input;
    
    if(playerTurn)
    {
        cout << "Roll your piece (enter 'Y')." << endl;
        do
        {
            cin >> input;
        }while(input != 'Y');
        
        roll = (rand()%6) + 1;
        
        cout << "You rolled: " << roll << endl << endl;
    }
    else
    {
        roll = (rand()%6) + 1;
    }
    
    return roll;
}

void stompLog(short *board, player *playerList, short specPos)
{
    short startPos;
    
    short mapPos[4];
    for(short count = 0; count < 4; count++)
    {
        if(int((*(playerList + count)).color) == *(board+ specPos))
        {
            if((*(playerList + count)).color == 'R')
                startPos = 0;
            else if((*(playerList + count)).color == 'B')
                startPos = 7;
            else if((*(playerList + count)).color == 'B')
                startPos = 14;
            else if((*(playerList + count)).color == 'B')
                startPos = 21;
            
            mapPos[0] = (*(playerList + count)).p1pos + startPos;
            mapPos[1] = (*(playerList + count)).p2pos + startPos;
            mapPos[2] = (*(playerList + count)).p3pos + startPos;
            mapPos[3] = (*(playerList + count)).p4pos + startPos;
            
            for(short count2 = 0; count2 < 4; count2++)
            {
                if(mapPos[count2] == specPos)
                {
                    (*(playerList + count)).startScore += 1;
                    switch(count2)
                    {
                        case(0):
                        {
                            (*(playerList + count)).p1pos = -1;
                            break;
                        }
                        case(1):
                        {
                            (*(playerList + count)).p2pos = -1;
                            break;
                        }
                        case(2):
                        {
                            (*(playerList + count)).p3pos = -1;
                            break;
                        }
                        case(3):
                        {
                            (*(playerList + count)).p4pos = -1;
                            break;
                        }
                    }
                }
            }
        } 
    }
}

char* scoreSave(short *rcrdPtr)
{
    char* returnCharPtr;
    string loser = "loserBoard.bin";
    string winner = "winnerBoard.bin";
    string english = "englishBoard.txt";
    fstream* fileAccess1,* fileAccess2,* fileAccess3;
    char playerName[] = "Name: ";
    char* tempName[30];
    winnerLine winners;
    loserLine losers;
    short score, indexW;
    
    for(short count = 0; count < 4; count++)
    {
        if(*(rcrdPtr + count) > 0)
            score = *(rcrdPtr + count);
    }
    
    //taking player name again for records
    cout << "Please enter your name for the records." << endl;
    cin.getline(*tempName,30);
    
    strncat(playerName, *tempName, 30);
    
    winners.winnerName = playerName;
    winners.winnerScore = score;
    losers.loserScore1 = score *= -1;
    losers.loserScore2 = score *= -1;
    losers.loserScore3 = score *= -1;
    
    (*(fileAccess1)).open(loser, ios::out | ios::binary);
    (*(fileAccess2)).open(winner, ios::out | ios::binary);
    fOutOpen(fileAccess3, english);
    
    //I fully admit I should have gotten this working before submitting...
    //Unfortunately I'm not sure why it isn't working properly
    (*(fileAccess1)).write(reinterpret_cast<char *>(&losers), sizeof(losers));
    (*(fileAccess2)).write(reinterpret_cast<char *>(&winners), sizeof(winners));
    
    if((*(fileAccess1)).fail()||(*(fileAccess2)).fail()||(*(fileAccess3)).fail())
        *returnCharPtr = 'F';
    else
    {
        *returnCharPtr = 'S';
        fClose(fileAccess1);
        fClose(fileAccess2);
        fClose(fileAccess3);
    }

    return returnCharPtr;
}

void fOutOpen(fstream *fileObject, string fileName)
{
    (*(fileObject)).open(fileName, ios::in | ios::out);
}

void fClose(fstream *fileObject)
{
    (*(fileObject)).close();
}

void winnerBoard()
{
    ifstream board;
    char ch;
    board.open("winnerBoard.bin",ios::out | ios::binary);
    
    board.get(ch);
    
    while(board)
    {
        cout << ch;
        board.get(ch);
    }
    cout << endl << endl;
    
    board.close();
}

void loserBoard()
{
    ifstream board;
    char ch;
    board.open("loserBoard.bin",ios::out | ios::binary);
    
    board.get(ch);
    
    while(board)
    {
        cout << ch;
        board.get(ch);
    }
    cout << endl << endl;
    
    board.close();
}

void nameBoard()
{
    ifstream board;
    char ch, skip = 'N';
    long skipL, numBytes;
    
    board.open("englishBoard.txt",ios::out | ios::binary);
    
    board.seekg(0L, ios::end);
    numBytes = board.tellg();
    board.seekg(0L, ios::beg);
    
    while(int(skip) > 57|| int(skip) < 48|| skipL > numBytes)
    {
        cout << "Specify how far ahead you want to skip in the output." << endl;
        cin >> skip;
        skipL = (int(skip)-48);
    }
    
    board.seekg(skipL, ios::beg);
    board.get(ch);
    
    while(board)
    {
        cout << setw(2) << ch;
        board.get(ch);
    }
    cout << endl << endl;
    
    board.close();
}

void rules()
{
    cout << "The rules of the game are as follows: " << endl;
    cout << "Your objective is to transit all your pieces across the entire board, and into your safe zone." << endl;
    cout << "To get your pieces out of start, you must achieve a 'pop', that is, rolling a 6." << endl;
    cout << "You can only pop one piece per turn, and cannot pop while a piece is occupying your starting position." << endl;
    cout << "You cannot land your pieces on each other. However, if one player's piece lands on a different"
            " player's piece, then the piece that got landed on is sent back to that player's start area." << endl;
    cout << "Players cannot enter each other's respective safe and start zones." << endl;
    cout << "If you roll a 6, you get to roll again until you no longer roll a 6 for the piece in question." << endl;
    cout << "You must roll the exact number needed to land in an unoccupied safe zone for your pieces." << endl;
    cout << "All pieces move clockwise across the board." << endl;
    cout << "Red starts in the upper left corner, blue in the upper right, yellow in the bottom right, and green in the bottom left." << endl;
    cout << endl;
}