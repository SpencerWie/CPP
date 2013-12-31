#include "Block.h"

     Block::Block(int X, int Y, int Size, int Color){ // init Block
        this->X = X; this->Y = Y;
        this->dX = 0; this->dY = 0;
        this->aX = 0; this->aY = 0;
        this->Size = Size;
        this->Color = Color;
        sprite = create_bitmap(Size,Size);
     }
     Block::Block(){ //empty constructor 10x10 black.
         X,Y,dX,dY,aX,aY=0;
         Size = 10; Color = makecol(0,0,0);
         sprite = create_bitmap(Size,Size);
      }
     void Block::Draw(){ // Draw the object on the buffer
        applyPhysics();
        checkBounds();
        rectfill(buffer,X,Y,X+Size,Y+Size,Color);
     }
     void Block::checkBounds(){ // Stop X or Y position on edge of map.
        if(X > SCREEN_W-Size) X = SCREEN_W - Size;
        if(X < 0) X = 0;
        if(Y > SCREEN_H-Size){ Y = SCREEN_H - Size; aY=0; }
        if(Y < 0) Y = 0;
     }
     void Block::applyPhysics(){ //Velocity, Acceleration, and friction.
        X += dX; dX += aX;
        Y += dY; dY += aY;
        dX *= 0.95;
        dY *= 0.95;
     }

