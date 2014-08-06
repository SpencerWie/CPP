#include <stdlib.h>
#include <glut.h>

float angle = 0.0f;

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(		0.0f, 0.0f, 10.0f,
					0.0f, 0.0f,  0.0f,
					0.0f, 1.0f, 0.0f);

	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	glColor3f(0.4f, 0.4f, 0.4f);

	glBegin(GL_TRIANGLES);
		glVertex3f(-2.0f, -2.0f, 0.0f);
		glVertex3f(0.0f ,0.0f, 0.0f);
		glVertex3f(2.0f ,-2.0f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
		glVertex3f(2.0f, 2.0f, 0.0f);
		glVertex3f(0.0f ,0.0f, 0.0f);
		glVertex3f(-2.0f ,2.0f, 0.0f);
	glEnd();

	glColor3f(0.2f, 0.2f, 0.2f);

	glBegin(GL_TRIANGLES);
		glVertex3f(-1.8f, -1.5f, 0.0f);
		glVertex3f(-0.2f ,0.0f, 0.0f);
		glVertex3f(-1.8f ,1.5f, 0.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
		glVertex3f(1.8f, -1.5f, 0.0f);
		glVertex3f(0.2f ,0.0f, 0.0f);
		glVertex3f(1.8f ,1.5f, 0.0f);
	glEnd();

	angle += 0.1f;

    glutSwapBuffers();
}

void changeSize(int w, int h) {
	if(h == 0) h = 1.0;

	float ratio = w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {

	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("My Window");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// enter GLUT event processing cycle
	glutMainLoop();
	
	return 1;
}