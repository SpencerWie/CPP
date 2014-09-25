#include <windows.h>  
#include <stdlib.h>
#include <iostream>
#include <glut.h>
#include <vector>

// Window init size.
int XSize = 640, YSize = 480;

using namespace std;

// Class Point will be used to store the points in the users polygon.
class Point {
public:
	int x;
	int y;
	int level;
	Point();
	Point(int X, int Y, int Level);
};


Point::Point() {
	x = 0; 
	y = 0;
	level = 0;
}

Point::Point(int X, int Y, int Level) {
	x = X; 
	y = Y;
	level = Level;
}

vector<Point> points; // Point array (20 points max)
vector< vector<Point> > levels; // Point array showing current level-n poly.
vector< vector<Point> > polys; // generated sub-bezier polys.
int currentLevel = -1;


// Generate Bezier Curve of given poly (point vector)
void generateBezier( vector<Point> points ) 
{
	levels.push_back( *new vector<Point> );
	currentLevel++;
	for( size_t i = 1; i < points.size(); i++ ) {
		int midx = (points[i-1].x + points[i].x) / 2;
		int midy = (points[i-1].y + points[i].y) / 2;
		levels[currentLevel].push_back(*new Point(midx, midy, 0));
	}
	// If we are down to a single point, generate generate new poly-lines. Otherwise keep spliting.
	if( levels[currentLevel].size() < (size_t)2 ) {
		// (2) break up into two polys.
		polys.clear();
		size_t n = levels.size();
		vector<Point> poly1;
		vector<Point> poly2;
		for(size_t i = 0; i < n; i++) {
			size_t subN = levels[i].size();
			if(subN > 0) {
				poly1.push_back(*new Point(levels[i][0].x, levels[i][0].y, 0));
				poly2.push_back(*new Point(levels[i][subN - 1].x, levels[i][subN - 1].y, 0));
			}
		}
		polys.push_back(poly1);
		polys.push_back(poly2);
		currentLevel = -1;
		return;
	} else {
		generateBezier(levels[currentLevel]);
	}
}

void renderScene(void) 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the screen-buffer.
 
	// Draw original poly
	glBegin(GL_LINE_STRIP);
		glColor3f(0.0f, 0.0f, 1.0f);
		for(size_t i = 0; i < points.size(); i++){
			glVertex2i(points[i].x, points[i].y);
		}
	glEnd();
	
	// Draw level-n poly
	glColor3f(0.0f, 1.0f, 0.0f);
	for(size_t i = 0; i	< polys.size(); i++) {
		size_t subPolyN = polys[i].size();
		glBegin(GL_LINE_STRIP);
			for(size_t j = 0; j	< subPolyN; j++) {
				glVertex2i(polys[i][j].x, polys[i][j].y);
			}
		glEnd();
	}

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

// Keyboard events : "q" = Quit , "n" = New , "+/-" = change level , "p" = toggle states.
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

// mouse() => When the user clicks on the screen a new point is created on the screen based on mouse pos.
void mouse(int btn, int state, int x, int y) {
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_UP){
		points.push_back(*new Point(x, y, 0));
		levels.clear();
		generateBezier(points);
		glutPostRedisplay();
	}
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
	glutMouseFunc(mouse);

	// enter GLUT event processing loop
	glutMainLoop();

	return 1;
}
