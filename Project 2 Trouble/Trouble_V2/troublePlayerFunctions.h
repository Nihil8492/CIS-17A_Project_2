#ifndef TROUBLEPLAYERFUNCTIONS_H
#define TROUBLEPLAYERFUNCTIONS_H

class troublePlayerFunctions {
public:
    troublePlayerFunctions();
    troublePlayerFunctions(const troublePlayerFunctions& orig);
    ~troublePlayerFunctions();
    char getColor() const
    { return *color; }
    short getp1pos() const
    { return p1pos; }
    short getp2pos() const
    { return p2pos; }
    short getp3pos() const
    { return p3pos; }
    short getp4pos() const
    { return p4pos; }
    short getstartScore() const
    { return startScore; }
    short getsafeScore() const
    { return safeScore; }
    short getsafeIndex(int index) const
    { return piecesInSafe[index]; }
    
    short getpos(int) const;
    void getpos(char) const;
    
    void setColor(char);
    void setp1pos(short);
    void setp2pos(short);
    void setp3pos(short);
    void setp4pos(short);
    void setstartScore(short);
    void setsafeScore(short);
    void setsafeIndex(short);
    
    troublePlayerFunctions operator + (const troublePlayerFunctions &);
    troublePlayerFunctions operator - (const troublePlayerFunctions &);
    troublePlayerFunctions operator * (const troublePlayerFunctions &);
    
private:
    char *color;
    short p1pos;
    short p2pos;
    short p3pos;
    short p4pos;
    short startScore;
    short safeScore;
    short piecesInSafe[4];
    static short numObj;

};

#endif /* TROUBLEPLAYERFUNCTIONS_H */

