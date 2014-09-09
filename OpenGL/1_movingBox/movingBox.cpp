#include <windows.h>  
#include <stdlib.h>
#include <iostream>
#include <glut.h>

int XSize = 640, YSize = 480;

int xPos = 50, yPos = 50, size = 50, speed = 4;

bool LEFT = false, RIGHT = false, UP = false, DOWN = false;

using namespace std;

void renderScene(void) 
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
 
   // Draw a Red 1x1 Square centered at origin
   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      glVertex2i(xPos, yPos);    // x, y
      glVertex2i(xPos+size, yPos);
      glVertex2i(xPos+size,  yPos+size);
      glVertex2i(xPos,  yPos+size);
   glEnd();
 
   glFlush();  // Render now
}

void fixScene(int width, int height) 
{
	// Extend Screen
	glLoadIdentity();
	XSize = width; YSize = height;
	glOrtho(0, width, height, 0, 0, 1);
	glViewport(0, 0, width, height);

	// Move player if player is outside of screen.
	if(xPos > width) xPos = width - size;
	if(yPos > height) yPos = height - size;

}

void updateBlock() 
{
	if( LEFT && (xPos > 0) ) 
		xPos -= speed;
	if( RIGHT && (xPos < XSize - size) ) 
		xPos += speed;
	if( UP && (yPos > 0) ) 
		yPos -= speed;
	if( DOWN && (yPos < YSize - size) ) 
		yPos += speed;

	glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y)
{
	if(key == 'a') LEFT = true;
	if(key == 'd') RIGHT = true;
	if(key == 'w') UP = true;
	if(key == 's') DOWN = true;
}

void keyRelease(unsigned char key, int x, int y)
{
	if(key == 'a') LEFT = false;
	if(key == 'd') RIGHT = false;
	if(key == 'w') UP = false;
	if(key == 's') DOWN = false;
}

void update(int time) 
{
	// Update Block
	updateBlock();

	//Update timer
	time++;
	glutTimerFunc(10, update, time);
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glMatrixMode (GL_PROJECTION);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(XSize,YSize);
	glutCreateWindow("Test");
	glOrtho(0, XSize, YSize, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, XSize, YSize);
	glPointSize( 5.0 );

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(fixScene);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutTimerFunc(10, update, 0);

	// enter GLUT event processing loop
	glutMainLoop();

	return 1;
}