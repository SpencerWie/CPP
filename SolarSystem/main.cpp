#include <windows.h>  
#include <stdlib.h>
#include <iostream>
#include <glut.h>
#include <math.h>

int XSize = 640, YSize = 480;
int DELAY = 500; // How fast the animation is, this is done by a timer for consitency accross systems.
int ZOOM = 45;
float ROLL = 0.0, YAW = 0.0, PITCH = 0.0;
float LOOK_X = 0.0, LOOK_Y = 0.0, LOOK_Z = 0.0;
int mouseX = 0, mouseY = 0;

const int SPACE = 32;
const int PLUS = 43;
const int MINUS = 45;
const int ONE = 49;
const int TWO = 50;


// States
bool ANIMATION = true; // toggles by the space bar.
bool cameraLight = true;
bool sunLight  = true;
bool SPAN = false;
bool MOUSE_LEFT = false;

// Camera 
float camX = 10.0, camY = 10.0, camZ = 22.0;

void display(void);
void animate(int t);
void reshape(int w, int h);
void keyDown(unsigned char key, int x, int y);
void arrowKeys(int key, int x, int y);
void mouseMove(int x, int y);
void mouseUpdate(int x, int y);
void handleCamera();
GLuint LoadTexture( const char * filename );
void updateSun();
void updateEarth();
void updateMars();
void updatePlanet(float OrbitRadius, float SelfAngle, float Radius, float Angle, float Tilt, GLUquadric *Planet, GLuint Texture);

//Sun Data
GLUquadric *Sun = gluNewQuadric();
GLuint sunTexture;
float sunRadius = 3.0, sunAngle = 0.0 , sunSpin = 0.1; 

//Earth Data (1.0, 1.0, 1.0, 1.0)
GLUquadric *Earth = gluNewQuadric();
GLuint earthTexture;
float earthOrbitPeriod = 1.0, earthOrbitRadius = 1.0, earthSelfRotation = 1.0, earthRadius = 1.0, earthAngle = 0.0, earthSelfAngle = 0.0 ;

//Mercury Data (0.25, 0.4, 15, 0.4)
GLUquadric *Mercury = gluNewQuadric();
GLuint mercuryTexture;
float mercuryOrbitPeriod = 0.25, mercuryOrbitRadius = 0.4, mercurySelfRotation = 15.0, mercuryRadius = 0.4, mercuryTilt = 0.2, mercuryAngle = 0.0, mercurySelfAngle = 0.0 ;

//Venus Data (0.6, 0.7, 10, 0.9)
GLUquadric *Venus = gluNewQuadric();
GLuint venusTexture;
float venusOrbitPeriod = 0.6, venusOrbitRadius = 0.7, venusSelfRotation = 10.0, venusRadius = 0.9, venusTilt = 0.1, venusAngle = 0.0, venusSelfAngle = 0.0 ;

//Mars Data  (1.8, 1.5, 1, 0.5)
GLUquadric *Mars = gluNewQuadric();
GLuint marsTexture;
float marsOrbitPeriod = 1.8, marsOrbitRadius = 1.5, marsSelfRotation = 1.0, marsRadius = 0.5, marsTilt = 0.1, marsAngle = 0.0, marsSelfAngle = 0.0 ;

//Moon Data
GLUquadric *Moon = gluNewQuadric();
GLuint moonTexture;
float moonRadius = 0.3, moonAngle = 0.0 , moonSpin = 0.4, moonTilt = 0.2;

float moon1Radius = 0.1, moon1Angle = 0.0 , moon1Spin = 0.5, moon1Tilt = 0.5;
float moon2Radius = 0.1, moon2Angle = 30.0 , moon2Spin = 1.0, moon2Tilt = -0.2;


GLuint LoadTexture( const char * filename )
{

  GLuint texture;

  int width, height;

  unsigned char * data;

  FILE * file;

  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;
  width = 1024;
  height = 512;
  data = (unsigned char *)malloc( width * height * 3 );
  fread( data, width * height * 3, 1, file );
  fclose( file );

	for(int i = 0; i < width * height ; i++)
	{
	   int index = i*3;
	   unsigned char B,R;
	   B = data[index];
	   R = data[index+2];

	   data[index] = R;
	   data[index+2] = B;
	}

	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
	free( data );

	return texture;
}

int main (int argc, char **argv)
{
    glutInit(&argc, argv); 
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(XSize,YSize);
    glutCreateWindow("Solar System");

	// set the background color
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// set the shading model
	glShadeModel(GL_SMOOTH);

	// set up a single white light
	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);

	GLfloat lightColor2[] = { 0.5f, 0.5f, 0.5f, 0.5 };

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor2);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	// make the modelview matrix active, and initialize it
	glMatrixMode(GL_MODELVIEW);

	// Setup textures
	gluQuadricTexture(Sun, GL_TRUE);
	gluQuadricOrientation(Sun, GLU_OUTSIDE);
	sunTexture = LoadTexture( "Sun.bmp" );

	gluQuadricTexture(Earth, GL_TRUE);
	earthTexture = LoadTexture( "Earth.bmp" );

	gluQuadricTexture(Moon, GL_TRUE);
	moonTexture = LoadTexture( "Moon.bmp" );

	gluQuadricTexture(Mercury, GL_TRUE);
	mercuryTexture = LoadTexture( "Moon.bmp" ); // Mercury and the Moon have nearly the same surface

	gluQuadricTexture(Venus, GL_TRUE);
	venusTexture = LoadTexture( "Venus.bmp" ); 

	gluQuadricTexture(Mars, GL_TRUE);
	marsTexture = LoadTexture( "Mars.bmp" ); 

    glutDisplayFunc(display);
	glutTimerFunc(DELAY, animate, 0);

	glutKeyboardFunc(keyDown);
	glutSpecialFunc(arrowKeys); 

	//glutMouseFunc(mouseDown);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseUpdate);

    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}

void display(void)
{
    glLoadIdentity();
	gluLookAt( camX, camY, camZ,  LOOK_X, LOOK_Y, LOOK_Z,  0.0,1.0,0.0 ); 
	handleCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen-buffer.

	sunAngle += sunSpin;
	updateSun();

	earthAngle += (1/earthOrbitPeriod)/10;
	earthSelfAngle += (earthSelfRotation/10);
	moonAngle += moonSpin;
	updateEarth();

	mercuryAngle += (1/mercuryOrbitPeriod)/10;
	mercurySelfAngle += (mercurySelfRotation/10);
	updatePlanet(mercuryOrbitRadius, mercurySelfAngle, mercuryRadius, mercuryAngle, mercuryTilt, Mercury, mercuryTexture);

	venusAngle += (1/venusOrbitPeriod)/10;
	venusSelfAngle += (venusSelfRotation/10);
	updatePlanet(venusOrbitRadius, venusSelfAngle, venusRadius, venusAngle, venusTilt, Venus, venusTexture);

	marsAngle += (1/marsOrbitPeriod)/10;
	marsSelfAngle += (marsSelfRotation/10);
	moon1Angle += moon1Spin;
	moon2Angle += moon2Spin;
	updateMars();

    glutSwapBuffers();       
}

void animate(int t)
{ 
	if(ANIMATION) glutPostRedisplay(); 
	glutTimerFunc(DELAY/60.0, animate, t);
}

// Whent the user presses SPACE this toggles animation. Such that it is paused and un-paused.
void toggleAnimation() {
	if(ANIMATION) ANIMATION = false;
	else ANIMATION = true;
}

void mouseUpdate(int x, int y) 
{
		mouseX = x; 
		mouseY = y;
}

void mouseMove(int x, int y) 
{
	float diffX = mouseX - x;
	float diffY = mouseY - y;

	if(SPAN) { LOOK_X += diffX/10; LOOK_Y += diffY/10; }
	else {YAW += diffX; PITCH += diffY;}

	mouseX = x; 
	mouseY = y;
}

void keyDown(unsigned char key, int x, int y) 
{
	if( (key == 'Q' || key == 'q') )  exit(0);
	if( key == SPACE ) toggleAnimation(); // 32 = SPACE

	if( key == PLUS && ZOOM >=0 ) ZOOM--;
	if( key == MINUS && ZOOM < 150 ) ZOOM++;

	// If we are zooming change the perspective
	if(key == PLUS || key == MINUS) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity(); 
		gluPerspective(ZOOM,(GLdouble)XSize/(GLdouble)YSize,0.1,100.0);
		glMatrixMode(GL_MODELVIEW);
	}

	// 1 - Toggles camera light
	if( key == ONE ) {
		if(cameraLight){ cameraLight = false; glDisable(GL_LIGHT0); }
		else { cameraLight = true; glEnable(GL_LIGHT0); }
	}
	// 2 - Toggles sun light
	if( key == TWO ) {
		if(sunLight) { sunLight = false; glDisable(GL_LIGHT1); }
		else { sunLight = true; glEnable(GL_LIGHT1); }
	}
	
	// z- toggles spanning with the mouse and keyboard
	if( key == 'z' ) SPAN = !SPAN;

}

void arrowKeys(int key, int x, int y) 
{
	if( key == GLUT_KEY_LEFT ) ROLL++;
	if( key == GLUT_KEY_RIGHT ) ROLL--;
}

void handleCamera()
{
	// Camera Light
	GLfloat lightPos[4] = {camX, camY, camZ, 1.0};
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

	glRotatef(ROLL, 0.0, 0.0, 1.0);
	glRotatef(YAW, 0.0, 1.0, 0.0);
	glRotatef(PITCH, 1.0, 0.0, 0.0);

}

void updateSun()
{
	glPushMatrix();
		glRotatef(-90.0,1.0,0.0,0.0);
		glRotatef(sunAngle, 0.0, 0.0, 1.0);
		GLfloat lightPos[4] = {0.0, 0.0, 0.0, 1.0};
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, sunTexture );
		gluQuadricDrawStyle( Sun, GLU_FILL );
		gluQuadricOrientation(Sun, GLU_INSIDE);
		gluQuadricTexture(Sun, GL_TRUE);
		gluSphere(Sun, sunRadius, 36, 18);
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void updateEarth()
{
	// Update Earth respect to the Sun
	glPushMatrix();

		glRotatef(-90.0,1.0,0.0,0.0);
		glRotatef(earthAngle, 0.0, 0.0, 1.0); // rotate about sun
		
		glTranslatef(earthOrbitRadius*10,0.0,0.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, earthTexture );
		gluQuadricDrawStyle( Earth, GLU_FILL );
		gluQuadricTexture(Earth, GL_TRUE);

		glPushMatrix(); // Update Earth itself

			glRotatef(earthAngle, 0.0, 0.0, 1.0); // self-rotate
			gluSphere(Earth, earthRadius, 36, 18); // draw Earth

		glPopMatrix();

		glPushMatrix(); // Update Moon

			glRotatef(moonAngle, 0.0, moonTilt, 1.0); //  rotate Moon about earth, with tilt

			glTranslatef(2.0,0.0,0.0); // give moon 2 units of space from Earth

			glRotatef(moonAngle, 0.0, 0.0, 1.0); // self-rotate moon
			glBindTexture( GL_TEXTURE_2D, moonTexture );
			gluQuadricTexture(Moon, GL_TRUE);
			gluSphere(Moon, moonRadius, 18, 9); // Draw Moon

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void updateMars()
{
	// Update Mars respect to the Sun
	glPushMatrix();

		glRotatef(-90.0,1.0,0.0,0.0);
		glRotatef(marsAngle, 0.0, 0.0, 1.0); // rotate about sun
		
		glTranslatef(marsOrbitRadius*10,0.0,0.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, marsTexture );
		gluQuadricDrawStyle( Mars, GLU_FILL );
		gluQuadricTexture(Mars, GL_TRUE);

		glPushMatrix(); // Update Mars itself

			glRotatef(marsAngle, 0.0, 0.0, 1.0); // self-rotate
			gluSphere(Mars, marsRadius, 36, 18); // draw Mars

		glPopMatrix();

		glPushMatrix(); // Update Moon1

			glRotatef(moon1Angle, 0.0, moon1Tilt, 1.0); //  rotate Moon about earth, with tilt

			glTranslatef(1.0,0.0,0.0); // give moon 1 units of space from Mars

			glRotatef(moon1Angle, 0.0, 0.0, 1.0); // self-rotate moon
			glBindTexture( GL_TEXTURE_2D, moonTexture );
			gluQuadricTexture(Moon, GL_TRUE);
			gluSphere(Moon, moon1Radius, 18, 9); // Draw Moon

		glPopMatrix();

		glPushMatrix(); // Update Moon2

			glRotatef(moon2Angle, 0.0, moon2Tilt, 1.0); //  rotate Moon about earth, with tilt

			glTranslatef(0.8,0.0,0.0); // give moon 0.8 units of space from Mars

			glRotatef(moon2Angle, 0.0, 0.0, 1.0); // self-rotate moon
			glBindTexture( GL_TEXTURE_2D, moonTexture );
			gluQuadricTexture(Moon, GL_TRUE);
			gluSphere(Moon, moon2Radius, 18, 9); // Draw Moon

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void updatePlanet(float OrbitRadius, float SelfAngle, float Radius, float Angle, float Tilt, GLUquadric *Planet, GLuint Texture)
{
	glPushMatrix();

		glRotatef(-90.0,1.0,0.0,0.0);
		glRotatef(Angle, 0.0, Tilt, 1.0); // rotate about sun
		
		glTranslatef(OrbitRadius*10,0.0,0.0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, Texture );
		gluQuadricDrawStyle( Planet, GLU_FILL );
		gluQuadricTexture(Planet, GL_TRUE);

		glPushMatrix(); // Update Planet itself

			glRotatef(SelfAngle, 0.0, 0.0, 1.0); // self-rotate
			gluSphere(Planet, Radius, 36, 18); // draw 

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void reshape(int w, int h)
{
	XSize = w; YSize = h;
    if (w == 0 || h == 0) return;   
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
    gluPerspective(ZOOM,(GLdouble)w/(GLdouble)h,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w , h);
}
