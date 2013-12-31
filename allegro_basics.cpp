#include <allegro.h>
#include <iostream>
#include "Block.h"
BITMAP *buffer;
volatile int closeWindow = false;
volatile int ticks = 0;
void closeWindowCall(){closeWindow = true;} // close window event
END_OF_FUNCTION(closeWindowCall);
void ticker(){ ticks++; } // Timer
END_OF_FUNCTION(ticker)
void init();

class Tile {
public:
    double X,Y,Size;
    BITMAP *sprite = create_bitmap(Size,Size);
    int Color = makecol(0,0,0);
    Tile(){ //Note: Need an empty contructor to create an array of this type.
        X=0;Y=0;Size=0;Color = makecol(0,0,0);
    }
    Tile(int X,int Y,int Size){
        this->X = X; this->Y = Y;
        this->Size = Size;
    }
    void Draw(){
        rectfill(buffer,X,Y,X+Size,Y+Size,Color);
    }
    void checkCollision(Block block){
        Color = makecol(0,0,0);
        if(block.X>X-Size && block.X<X+Size)
        { //check for collision to player (x-part)
            if(block.Y>Y-Size && block.Y<Y+Size)
            { // (y-part)
                Color = makecol(0,255,0);
            }
        }
    }
};

int main(){
    init();

    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(40));

    LOCK_FUNCTION(closeWindowCall);
    set_close_button_callback(closeWindowCall);

    buffer = create_bitmap(640, 480);
    Block block (50,50,50,makecol(255,0,0));

    Tile tileArray[] {
        Tile(100,350,50),
        Tile(400,350,50),
        Tile(250,430,50)
    };

    while (!(key[KEY_ESC] || closeWindow)) {
        if(ticks>0){
            clear(buffer); // For double buffering

            rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(0,0,255));
            textprintf_ex(buffer,font,10,10,makecol(0,0,0),-1,"Move with Left and Right, Up to jump. Esc to exit.");

            if(key[KEY_LEFT]){ block.dX--; }
            if(key[KEY_RIGHT]){ block.dX++; }
            if(key[KEY_UP] && block.aY>=0 && block.Y>=SCREEN_H-block.Size){ block.dY = -10 ;block.aY= -1; } //No Y acc, and on ground. Then Jump.
            if(key[KEY_DOWN]){ block.dY++; }
            block.aY += 0.1; // gravity
            block.Draw();

            for(unsigned int i=0;i<(sizeof(tileArray)/sizeof(tileArray[0]));i++){
                tileArray[i].Draw(); // Draw all tiles in tileArray
                tileArray[i].checkCollision(block);
            }
            show_mouse(buffer);

            blit(buffer, screen, 0, 0, 0, 0, buffer->w, buffer->h);//Needs to be last draw command (Draw everything).
            ticks=0;
        } else {
            rest(1);
        }
    }
    allegro_exit();
    return 0;
}
END_OF_MAIN()

void init(){
	allegro_init();
	int depth = desktop_color_depth();
	set_color_depth(depth);// set color depth based on desktop
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0); // Window 640x480
	install_timer(); // Setup timer,keyboard,mouse.
	install_keyboard();
	install_mouse(); // This is not needed for now.
	set_window_title("Hello World"); // set window title.
}
