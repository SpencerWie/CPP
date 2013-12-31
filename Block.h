//Block.h
#include <allegro.h>
#ifndef BLOCK_H
#define BLOCK_H
extern BITMAP *buffer; // external global var
class Block{
public:
    double X, Y, dX, dY, aX, aY, Size;
    int Color;
    BITMAP *sprite;
    Block();
    Block(int X, int Y, int Size, int Color);
    void Draw();
    void checkBounds();
    void applyPhysics();
};

#endif // BLOCK_H
