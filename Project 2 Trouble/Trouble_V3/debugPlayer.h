/*#ifndef DEBUGPLAYER_H
#define DEBUGPLAYER_H

#include <cstdlib>
using namespace std;

template <class T>
class debugPlayer {
public:
    debugPlayer();
    ~debugPlayer();
protected:

};

template<class T>
class debugPlayerInherited:public debugPlayer<T>{
    protected:
        T *augTable;        //Augmented Table with sums
    public:
        debugPlayerInherited(char *,int,int);
        ~Prob3TableInherited(){delete [] augTable;};
        const T *getAugTable(void){return augTable;};
};

#endif*/ /* DEBUGPLAYER_H */

