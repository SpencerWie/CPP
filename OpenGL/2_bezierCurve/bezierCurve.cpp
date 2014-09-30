#include <windows.h>  
#include <stdlib.h>
#include <iostream>
#include <glut.h>
#include <vector>
#include <sstream>

// Window init size.
int XSize = 640, YSize = 480;
using namespace std;

void drawCurve();

// Class Point will be used to store the points in the users polygon.
class Point {
public:
	float x;
	float y;
	int level;
	Point();
	Point(float X, float Y, int Level);
};


Point::Point() {
	x = 0; 
	y = 0;
	level = 0;
}

Point::Point(float X, float Y, int Level) {
	x = X; 
	y = Y;
	level = Level;
}

vector<Point> points;				// Point array (20 points max)
vector<Point> curve;				// The entire bezier curve, generated by stiching togehter many sub-polys.
vector< vector<Point> > levels;		// Point array showing current level-n poly.
vector< vector<Point> > polys;		// generated sub-bezier polys.

int currentLevel = -1;
int bezierLevel = 2;				// The level-n bezier curve
int oldLevel = 2;					// A temp level-n bezier, in case we need to change the level then covert back to origonal.
int currentBezierLevel = 0;			// A countdown used in generateBezier to determine when to stop.
const int ENABLE_INIT_POLY = 0;		// Shows the init-poly and allows user to create points.
const int DISABLE_INIT_POLY = 1;	// Hides the init-poly and does not allow user to create points.
int STATE = ENABLE_INIT_POLY; 
bool mouseDown = false;

void * FONT = GLUT_BITMAP_9_BY_15;

// Generate Bezier Curve of given poly (point vector)
void generateBezier( vector<vector<Point>> givenPolys ) 
{
	vector<vector<Point>> Polys;	
	for(int k = 0; (size_t)k < givenPolys.size(); k++) 
	{
		vector<Point> poly1;
		vector<Point> poly2;
		int N0 = givenPolys.size() - 1;
		int n0 = givenPolys[N0].size() - 1;
		poly1.push_back(*new Point(givenPolys[k][0].x, givenPolys[k][0].y, 0));
		poly2.push_back(*new Point(givenPolys[k][n0].x, givenPolys[k][n0].y, 0));
		// Do subdivisions until we reach a single point.
		while(true)
		{
			levels.push_back( *new vector<Point> );
			currentLevel++;
			for( size_t i = 1; i < givenPolys[k].size(); i++ ) {
				float midx = (givenPolys[k][i-1].x + givenPolys[k][i].x) / 2;
				float midy = (givenPolys[k][i-1].y + givenPolys[k][i].y) / 2;
				levels[currentLevel].push_back(*new Point(midx, midy, currentBezierLevel));
			}
			// When we are down to a point stop get mid-points
			if( levels[currentLevel].size() < (size_t)2 ) break; 
			givenPolys[k] = levels[currentLevel];
		}
		
		// (2) break up into two polys.
		size_t n = levels.size();
		for(size_t i = 0; i < n; i++) {
			size_t subN = levels[i].size();
			if(subN > 0) {
				poly1.push_back(*new Point(levels[i][0].x, levels[i][0].y, 0));
				poly2.push_back(*new Point(levels[i][subN - 1].x, levels[i][subN - 1].y, 0));
			}
		}
		//Place vectors end-by-end, we need to flip ploy2 to do that.
		Polys.push_back(poly1);
		reverse(poly2.begin(),poly2.end());
		Polys.push_back(poly2);
		currentLevel = -1;
		levels.clear();
	}
	if(bezierLevel > currentBezierLevel) {
		currentBezierLevel++;
		currentLevel = -1;
		generateBezier(Polys);
	} else {
		for(int i = 0; (size_t)i < Polys.size(); i++) polys.push_back(Polys[i]);
		currentBezierLevel = 0;
	}
}

void drawString(float x, float y, string str){
    glRasterPos2f(x, y);
    for (string::iterator i = str.begin(); i != str.end(); ++i) {
		char c = *i;
		glutBitmapCharacter(FONT, c);
    }
} 


void renderScene(void) 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the screen-buffer.

	// Draw text info
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	stringstream ss;
	ss << "Level: " << (bezierLevel + 1);
	string str = ss.str();
	drawString( 10, 15, str );

	if( STATE == ENABLE_INIT_POLY ) 
	{
		// Draw original poly
		glBegin(GL_LINE_STRIP);
			glColor3f(0.0f, 0.0f, 1.0f);
			for(size_t i = 0; i < points.size(); i++){
				glVertex2i(points[i].x, points[i].y);
			}
		glEnd();
		// Draw points
		glBegin(GL_POINTS);
			glColor3f(0.0f, 0.0f, 1.0f);
			for(size_t i = 0; i < points.size(); i++){
				glVertex2i(points[i].x, points[i].y);
			}
		glEnd();
	}
	// Note : Merge all sub polys to single poly, then draw.
	// Draw level-n poly
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for(size_t i = 0; i	< polys.size(); i++) {
		size_t subPolyN = polys[i].size();
			for(size_t j = 0; j	< subPolyN; j++) {
				glVertex2i(polys[i][j].x, polys[i][j].y);
			}
	}
	glEnd();
	glFlush();  // Show buffer.
}

// Adjust Viewport to match World View. (An extending effect).
void extendScreen(int width, int height) 
{
	// Extend Screen on resize.
	glLoadIdentity();
	XSize = width; YSize = height;
	glOrtho(0, width, height, 0, 0, 1);
	glViewport(0, 0, width, height);
}

// Keyboard events : "q" = Quit , "n" = New 
void keyPress(unsigned char key, int x, int y)
{
	if(key == 'Q' || key == 'q')  exit(0); 
	if(key == 'N' || key == 'n') {
		polys.clear();
		levels.clear(); // Clear level-i poly.
		points.clear(); // Clear init poly lines 
		currentLevel = -1; // Reset level.
		glutPostRedisplay();
	}
}

// Keyboard release events:  "+/-" = change level , "p" = toggle states.
void keyRelease(unsigned char key, int x, int y)
{
	if(key == 'Q' || key == 'q')  exit(0); 
	if(key == 'N' || key == 'n') {
		polys.clear();
		levels.clear(); // Clear level-i poly.
		points.clear(); // Clear init poly lines 
		currentLevel = -1; // Reset level.
		glutPostRedisplay();
	}
	if(key == '+') {
		if(bezierLevel < 8) bezierLevel++;
		drawCurve();
	}
	if(key == '-') {
		if(bezierLevel > 0) bezierLevel--;
		drawCurve();
	}
	if(key == 'p' || key == 'P') {
		if( STATE == ENABLE_INIT_POLY ) 
			STATE = DISABLE_INIT_POLY;
		else if( STATE == DISABLE_INIT_POLY )
			STATE = ENABLE_INIT_POLY;
		glutPostRedisplay();
	}
	if(key == 'd' || key == 'D'){
		if(points.size() > 0) {
			points.pop_back();
			if(points.size() >= 1) drawCurve();
			glutPostRedisplay();
		}
	}
}

// mouse() => When the user clicks on the screen a new point is created on the screen based on mouse pos.
void mouse(int btn, int state, int x, int y) {
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP && STATE == ENABLE_INIT_POLY){
		points.push_back(*new Point(x, y, 0));
		drawCurve();
	}
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){ mouseDown = true; } 
	else if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP){ mouseDown = false; } 
}

void drawCurve() {
	levels.clear();
	polys.clear();
	vector< vector<Point> > initPoly;
	initPoly.push_back(points);
	generateBezier(initPoly);
	glutPostRedisplay();
}

//When the mouse button is held down, show user how the next curve looks.
// To make this more efficent we lower the level-n to be a max of 2, then set it back to what it was afterwards.
void approximateCurvePoint(int x, int y) 
{
	if(mouseDown == true && STATE == ENABLE_INIT_POLY) {
		// Save old curve level, then set to level-2.
		oldLevel = bezierLevel;
		if(oldLevel > 1 ) bezierLevel = 2;

		// Draw curve with current point of mouse.
		vector<Point> tempPoints = points;
		tempPoints.push_back(*new Point(x,y,0));
		levels.clear();
		polys.clear();
		vector< vector<Point> > initPoly;
		initPoly.push_back(tempPoints);
		generateBezier(initPoly);
		bezierLevel = oldLevel;
		glutPostRedisplay();
	}
}

void mymenu (int id) {
	// Inc bezier
	if (id == 1){
		if(bezierLevel < 8) bezierLevel++;
		drawCurve();
	}
	// Dec bezier
	else if (id == 2) {
		if(bezierLevel > 0) bezierLevel--;
		drawCurve();
	}
	// Toggle state
	else if (id == 3) {
		if( STATE == ENABLE_INIT_POLY ) 
			STATE = DISABLE_INIT_POLY;
		else if( STATE == DISABLE_INIT_POLY )
			STATE = ENABLE_INIT_POLY;
		glutPostRedisplay();
	}
	// Exit
	else if(id == 4) exit(0);
}

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glMatrixMode (GL_PROJECTION);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(XSize,YSize);
	glutCreateWindow("Bezier Curve");
	glOrtho(0, XSize, YSize, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, XSize, YSize);
	glPointSize( 5.0 );

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(extendScreen);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutMouseFunc(mouse);
	glutMotionFunc(approximateCurvePoint);

	//Menu
	glutCreateMenu (mymenu);
	glutAddMenuEntry ("Inc beizer level [ + ]", 1);
	glutAddMenuEntry ("Dec beizer level [ - ]", 2);
	glutAddMenuEntry ("Toggle Polygon", 3);
	glutAddMenuEntry ("Quit", 4);
	glutAttachMenu (GLUT_RIGHT_BUTTON);

	// enter GLUT event processing loop
	glutMainLoop();
	return 1;
}
