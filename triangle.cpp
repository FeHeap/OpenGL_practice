#include <GL/glut.h>

void Display(void) {
	glPushMatrix();
	glBegin(GL_TRIANGLES);		//start to draw a triangle
	glColor3f(1.0f, 0.0f, 0.0f);	//set the output color to red
	glVertex2f(0.0f, 1.0f);		//(x1,y1)=(0,1)
	glColor3f(0.0f, 1.0f, 0.0f);	//set the output color to green
	glVertex2f(0.87f, -0.5f);	//(x2,x2)=(0.87,-0.5)
	glColor3f(0.0f, 0.0f, 1.0f);	//set the output color to blue
	glVertex2f(-0.87f, -0.5f);	//(x3,y3)=(-0.87,-0.5)
	glEnd();			//finish drawing a triangle
	glPopMatrix();
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);	//set the position of Window
	glutInitWindowSize(400, 400);		//set the size of Window
	glutCreateWindow("Colorful Triangle");	//set the title of Window
	glutDisplayFunc(Display);		//call function
	glutMainLoop();
	return 0;
}