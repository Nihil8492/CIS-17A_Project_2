#ifndef POSITION_H
#define POSITION_H

class position {
public:
    position();
    position(const position& orig);
    ~position();
private:
    short p1pos;
    short p2pos;
    short p3pos;
    short p4pos;
};

#endif /* POSITION_H */

