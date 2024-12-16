#ifndef POSITION_H
#define POSITION_H


class position {
public:
    position();
    position(const position& orig);
    ~position();
    
    short getp1pos() const
    { return p1pos; }
    short getp2pos() const
    { return p2pos; }
    short getp3pos() const
    { return p3pos; }
    short getp4pos() const
    { return p4pos; }
    
    void setp1pos(short);
    void setp2pos(short);
    void setp3pos(short);
    void setp4pos(short);
    
    short getpos(int) const;
    void getpos(char) const;
    
private:
    short p1pos;
    short p2pos;
    short p3pos;
    short p4pos;
};

#endif /* POSITION_H */