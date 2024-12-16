#ifndef TROUBLEPLAYERFUNCTIONS_H
#define TROUBLEPLAYERFUNCTIONS_H

# include "position.h"

class troublePlayerFunctions {
public:
    troublePlayerFunctions();
    troublePlayerFunctions(const troublePlayerFunctions& orig);
    ~troublePlayerFunctions();
    char getColor() const
    { return *color; }
    short getstartScore() const
    { return startScore; }
    short getsafeScore() const
    { return safeScore; }
    short getsafeIndex(int index) const
    { return piecesInSafe[index]; }
    
    void setColor(char);
    void setstartScore(short);
    void decstartScore(){startScore--;};
    void incstartScore(){startScore++;};
    void setsafeScore(short);
    void setsafeIndex(short);
    void decsafeScore(){safeScore--;};
    void incsafeScore(){safeScore++;};
    
    short getpos1(){return position.getp1pos();};
    short getpos2(){return position.getp2pos();};
    short getpos3(){return position.getp3pos();};
    short getpos4(){return position.getp4pos();};
    
    void setpos1(short a){position.setp1pos(a);};
    void setpos2(short a){position.setp2pos(a);};
    void setpos3(short a){position.setp3pos(a);};
    void setpos4(short a){position.setp4pos(a);};
    
    void incpos1(short);
    void incpos2(short);
    void incpos3(short);
    void incpos4(short);
    
    troublePlayerFunctions operator + (const troublePlayerFunctions &);
    troublePlayerFunctions operator - (const troublePlayerFunctions &);
    troublePlayerFunctions operator * (const troublePlayerFunctions &);
    
private:
    position position;
    char *color;
    short startScore;
    short safeScore;
    short piecesInSafe[4];
    static short numObj;

};

#endif /* TROUBLEPLAYERFUNCTIONS_H */

