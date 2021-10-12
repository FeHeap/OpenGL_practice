#include <GL/glut.h>
#define DrawPolygon(X) drawPolygon(X, sizeof(X) / sizeof(X[0]))


void background();
void Display(void);

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);	//set the position of Window
	glutInitWindowSize(1920, 1080);		//set the size of Window
	glutCreateWindow("Square");			//set the title of Window
		glutDisplayFunc(Display);
	glutMainLoop();
	return 0;
}


void background() {
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
		glVertex2f(-1.0, 1.0);
		glVertex2f(-1.0, -1.0);
		glVertex2f(1.0, -1.0);
		glVertex2f(1.0, 1.0);
	glEnd();
}

class NchuCh {

private:
	float origin[2] = { 960.0, 540.0 };
	void drawPolygon(int (*data)[2], int size) {
		float bufxy[2] = { 0.0, 0.0};
		glBegin(GL_POLYGON);
		for (int i = 0; i < size; i++) {
			bufxy[0] = (data[i][0] - origin[0]) / origin[0];
			bufxy[1] = (origin[1] - data[i][1]) / origin[1];
			glVertex2fv(bufxy);
		}
		glEnd();
	}
	
	//ch1_1
	int ch_1_1[4][2] = {
		{213, 375},
		{188, 389},
		{188, 524},
		{205, 504},
	};
	int ch_1_2[7][2] = {
		{205, 504},
		{188, 524},
		{251, 516},
		{310, 516},
		{318, 505},
		{310, 500},
		{243, 502},
	};
	int ch_1_3[6][2] = {
		{213, 375},
		{205, 402},
		{288, 377},
		{323, 373},
		{331, 356},
		{300, 359},
	};
	int ch_1_4[6][2] = {
		{331, 356},
		{323, 373},
		{328, 522},
		{325, 548},
		{343, 527},
		{341, 388},
	};
	int ch_1_5[3][2] = {
		{328, 522},
		{315, 523},
		{325, 548},
	};
	int ch_1_6[3][2] = {
		{208, 408},
		{202, 448},
		{229, 423},
	};
	int ch_1_7[4][2] = {
		{218, 415},
		{229, 423},
		{292, 402},
		{286, 395},
	};
	int ch_1_8[4][2] = {
		{286, 383},
		{292, 402},
		{316, 405},
		{300, 374},
	};
	int ch_1_9[7][2] = {
		{262, 387},
		{276, 431},
		{298, 475},
		{316, 496},
		{322, 478},
		{295, 435},
		{277, 382},
	};
	int ch_1_10[4][2] = {
		{247, 427},
		{208, 457},
		{205, 469},
		{228, 457},
	};
	int ch_1_11[6][2] = {
		{228, 457},
		{259, 455},
		{276, 433},
		{273, 420},
		{256, 434},
		{235, 443},
	};
	int ch_1_12[7][2] = {
		{228, 457},
		{216, 474},
		{211, 482},
		{216, 496},
		{281, 468},
		{283, 450},
		{259, 455},
	};
	int ch_1_13[4][2] = {
		{300, 417},
		{286, 469},
		{300, 474},
		{316, 429},
	};
	int ch_1_14[4][2] = {
		{286, 469},
		{253, 498},
		{280, 496},
		{300, 474},
	};
	void display_ch_1() {
		glColor3f(0.007, 0.4375, 0.589);
		DrawPolygon(ch_1_1);
		DrawPolygon(ch_1_2);
		DrawPolygon(ch_1_3);
		DrawPolygon(ch_1_4);
		DrawPolygon(ch_1_5);
		DrawPolygon(ch_1_6);
		DrawPolygon(ch_1_7);
		DrawPolygon(ch_1_8);
		DrawPolygon(ch_1_9);
		DrawPolygon(ch_1_10);
		DrawPolygon(ch_1_11);
		DrawPolygon(ch_1_12);
		DrawPolygon(ch_1_13);
		DrawPolygon(ch_1_14);
	}


public:
	NchuCh() {

	}
	void display_ch() {
		display_ch_1();
	}
};


void Display(void) {
	NchuCh word_ch;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	background();
	word_ch.display_ch();
	glutSwapBuffers();
}
