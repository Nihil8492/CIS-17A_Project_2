#ifndef DEBUGPLAYER_H
#define DEBUGPLAYER_H

#include <cstdlib>
#include <iostream>
using namespace std;

template <class T>
class debugPlayer {
public:
    debugPlayer();
    ~debugPlayer();
    virtual int virtclass() = 0;
protected:
    int exVal = 0;
};

template<class T>
class debugPlayerInherited1:public debugPlayer<T>{
    protected:
        
    public:
        debugPlayerInherited1(){;};
        debugPlayerInherited1(const debugPlayerInherited1& other)
        {cout<< "Copy constructor has been called." << endl << endl;};
        int virtclass() override
        {
            return 1;
        }
};

template<class T>
class debugPlayerInherited2:public debugPlayer<T>{
    protected:
        
        
    public:
        debugPlayerInherited2(){;};
        debugPlayerInherited2(const debugPlayerInherited2& other)
        {cout<< "Copy constructor has been called." << endl << endl;};
        int virtclass() override
        {
            return 2;
        }
};

class friendDemo{
    protected:
        int exVal2 = 23980;
        friend void getVal(friendDemo);
    public:
        
};

#endif /* DEBUGPLAYER_H */