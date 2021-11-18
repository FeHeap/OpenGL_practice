#include <GL/glut.h>
#include <cmath>

int CPT[3] = {0.0, 0.0, 500.0};
float rotateWS = 0.0;
float rotateAD = 0.0;
int headRotateState = 0;
float headRotate = 0.0;
int rotateSpotState = 0;
float rotateSpot = 0.0;

// jump
float jumpState = 0;
float jumpSpeed = 0;
float jumpMove = 0;
float jumpSumMove = 0.0;

void Display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glLoadIdentity();
	
	gluLookAt(CPT[0], CPT[1] + jumpSumMove, CPT[2], CPT[0], CPT[1] + jumpSumMove, CPT[2]-1.0, 0.0, 1.0, 0.0);
	

	glTranslated(CPT[0], CPT[1] + jumpSumMove, CPT[2]-100);
	glRotatef(rotateSpot, 0.0, 1.0, 0.0);
	glTranslated(-CPT[0], -CPT[1] - jumpSumMove, -CPT[2]+100);
	
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	glBegin(GL_POLYGON);
		glColor3f(0.153f, 0.102f, 0.51f);
		glVertex3i(-1000, -50, 1000);
		glVertex3i(1000, -50, 1000);
		glVertex3i(1000, -50, -1000);
		glVertex3i(-1000, -50, -1000);
	glEnd();
	glPopMatrix();
	

	glPushMatrix();
	glTranslated(200.0, 100.0, 0.0);
	glBegin(GL_POLYGON);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2i(-60, 60);
		glVertex2i(60, 60);
		glVertex2i(60, -60);
		glVertex2i(-60, -60);
	glEnd();
	glTranslated(0.0, -100.0, 0.0);
	glBegin(GL_POLYGON);
		glColor3f(0.155f, 0.015f, 0.013f);
		glVertex2i(0, 100);
		glVertex2i(37, -50);
		glVertex2i(-37, -50);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-200.0, 100.0, 00.0);
	glBegin(GL_POLYGON);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2i(-60, 60);
		glVertex2i(60, 60);
		glVertex2i(60, -60);
		glVertex2i(-60, -60);
	glEnd();
	glTranslated(0.0, -100.0, 0.0);
	glBegin(GL_POLYGON);
		glColor3f(0.155f, 0.015f, 0.013f);
		glVertex2i(0, 100);
		glVertex2i(37, -50);
		glVertex2i(-37, -50);
	glEnd();
	glPopMatrix();


	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	glTranslated(CPT[0], CPT[1] + jumpSumMove - 40.0, CPT[2] - 100.0);
	glRotatef(rotateWS, 1.0, 0.0, 0.0);
	glRotatef(rotateAD, 0.0, 1.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glutWireSphere(10.0, 20, 16);   /* draw sun */
	glPopMatrix();
	glPushMatrix();
	glTranslated(CPT[0], CPT[1] + jumpSumMove - 30.0, CPT[2] - 100.0);
	glRotatef(headRotate, 0.0, 1.0, 0.0);
	for (int r = 0; r < 180; r++) {
		glColor3f(0.0, r / 200.0, r / 100.0);
		glRotatef(r, 0.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 51; i++) {
			float j = 0.0628 * i;
			float x = cos(j) * 10;
			float y = sin(j) * 10;
			glVertex3f(x, y, 0.0);
		}
		glEnd();
	}
	glPopMatrix();



	glDisable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 1.0, 700.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
	case 'W':
		rotateWS += 4.0;
		if (rotateWS > 360.0) {
			rotateWS = 0.0;
		}
		CPT[2] -= 4;
		break;
	case 's':
	case 'S':
		rotateWS -= 4.0;
		if (rotateWS < 0.0) {
			rotateWS = 360.0;
		}
		CPT[2] += 4;
		break;
	case 'a':
	case 'A':
		rotateAD += 4.0;
		if (rotateAD > 360.0) {
			rotateAD = 0.0;
		}
		CPT[0] -= 4;
		break;
	case 'd':
	case 'D':
		rotateAD -= 4.0;
		if (rotateAD < 0.0) {
			rotateAD = 360.0;
		}
		CPT[0] += 4;
		break;
	case ' ':
		if (jumpState == 0) {
			jumpState = 1;
			jumpSpeed = 3;
		}
	}
}


void mouseFunction(int button, int state, int x, int y) {
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		if(headRotateState == 0) {
			headRotateState = 1;
		}
		else {
			headRotateState = 0;
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		rotateSpotState = 1;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		rotateSpotState = -1;
	}
	else {
		rotateSpotState = 0;
	}
}

void jump(void) {
	if (jumpState == 1) {
		jumpMove = jumpSpeed;
		jumpSpeed -= 0.01;
		jumpMove = (jumpMove + jumpSpeed) / 2;
		if (jumpSpeed <= -2.996) {
			jumpState = 0;
		}
		jumpSumMove += jumpMove;
	}
}

void headRotateFunc(void) {
	if (headRotateState == 1) {
		headRotate += 1.0;
		if (headRotate > 360.0) {
			headRotate = 0.0;
		}
	}
}

void rotateSpotFunc(void) {
	if (rotateSpotState == 1) {
		rotateSpot += 0.5;
		if (rotateSpot > 360.0) {
			rotateSpot = 0.0;
		}
	}
	else if (rotateSpotState == -1) {
		rotateSpot -= 0.5;
		if (rotateSpot < 0.0) {
			rotateSpot = 360.0;
		}
	}
}


void IdleFunc(void) {
	jump();
	headRotateFunc();
	rotateSpotFunc();
	glutPostRedisplay();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(Display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseFunction);
	glutIdleFunc(IdleFunc);
	glutMainLoop();
	return 0;
}
