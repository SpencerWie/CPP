#include <allegro.h>

BITMAP *buffer;
volatile int closeWindow = false;
volatile int ticks = 0;
void closeWindowCall(){closeWindow = true;} // close window event
END_OF_FUNCTION(closeWindowCall);
void ticker(){ ticks++; } // Timer
END_OF_FUNCTION(ticker)
void init();

class Block {
    public:
     double X, Y, dX, dY, aX, aY, Size = 0; // X,Y position and size of block. Doubles to more eased vel and acc.
     BITMAP *sprite; // The bitmap of the Block (sprite)
     int Color; // The Color of the object (32bit)
     Block(int X, int Y, int Size, int Color){ // init Block
        this->X = X; this->Y = Y;
        this->dX = 0; this->dY = 0;
        this->aX = 0; this->aY = 0;
        this->Size = Size;
        this->Color = Color;
        sprite = create_bitmap(Size,Size);
     }
     void Draw(){ // Draw the object on the buffer
        applyPhysics();
        checkBounds();
        rectfill(buffer,X,Y,X+Size,Y+Size,Color);
     }
     void checkBounds(){ // Stop X or Y position on edge of map.
        if(X > SCREEN_W-Size) X = SCREEN_W - Size;
        if(X < 0) X = 0;
        if(Y > SCREEN_H-Size){ Y = SCREEN_H - Size; aY=0; }
        if(Y < 0) Y = 0;
     }
     void applyPhysics(){ //Velocity, Acceleration, and friction.
        X += dX; dX += aX;
        Y += dY; dY += aY;
        dX *= 0.95;
        dY *= 0.95;
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

    while (!(key[KEY_ESC] || closeWindow)) {
        if(ticks>0){
            clear(buffer); // For double buffering
            rectfill(buffer,0,0,SCREEN_W,SCREEN_H,makecol(0,0,255));
            textprintf_ex(buffer,font,10,10,makecol(0,0,0),-1,"Move with Left and Right, Up to jump. Esc to exit.");
            if(key[KEY_LEFT]){ block.dX--; }
            if(key[KEY_RIGHT]){ block.dX++; }
            if(key[KEY_UP] && block.aY>=0 && block.Y>=SCREEN_H-block.Size){ block.dY = -5 ;block.aY= -1; } //No Y acc, and on ground. Then Jump.
            if(key[KEY_DOWN]){ block.dY++; }
            block.aY += 0.1; // gravity
            block.Draw();
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
