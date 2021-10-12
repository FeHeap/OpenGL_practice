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
	void drawPolygon(float (*data)[2], int size) {
		glBegin(GL_POLYGON);
		for (int i = 0; i < size; i++) {
			glVertex2fv(data[i]);
		}
		glEnd();
	}
	
	//ch1_1
	float ch_1_1[4][2] = {
		{-0.778125, 0.303704},
		{-0.803125, 0.277778},
		{-0.803125, 0.027778},
		{-0.786458, 0.064815},
	};
	float ch_1_2[7][2] = {
		{-0.786458, 0.064815},
		{-0.803125, 0.027778},
		{-0.737500, 0.042593},
		{-0.677083, 0.044444},
		{-0.668750, 0.062963},
		{-0.676042, 0.074074},
		{-0.746875, 0.070370},
	};
	float ch_1_3[6][2] = {
		{-0.778125, 0.303704},
		{-0.785417, 0.253704},
		{-0.700000, 0.300000},
		{-0.662500, 0.309259},
		{-0.655208, 0.338889},
		{-0.687500, 0.335185},
	};
	float ch_1_4[6][2] = {
		{-0.655208, 0.338889},
		{-0.662500, 0.309259},
		{-0.658333, 0.033333},
		{-0.661458, -0.014815},
		{-0.642708, 0.024074},
		{-0.643750, 0.279630},
	};
	float ch_1_5[3][2] = {
		{-0.658333, 0.033333},
		{-0.671875, 0.029630},
		{-0.661458, -0.014815},
	};
	float ch_1_6[3][2] = {
		{-0.782292, 0.244444},
		{-0.789583, 0.168519},
		{-0.760417, 0.214815},
	};
	float ch_1_7[4][2] = {
		{-0.771875, 0.229630},
		{-0.760417, 0.214815},
		{-0.695833, 0.253704},
		{-0.702083, 0.266667},
	};
	float ch_1_8[4][2] = {
		{-0.702083, 0.288889},
		{-0.695833, 0.253704},
		{-0.670833, 0.250000},
		{-0.687500, 0.305556},
	};
	float ch_1_9[7][2] = {
		{-0.727083, 0.283333},
		{-0.712500, 0.200000},
		{-0.688542, 0.120370},
		{-0.670833, 0.079630},
		{-0.664583, 0.112963},
		{-0.691667, 0.194444},
		{-0.710417, 0.290741},
	};
	float ch_1_10[4][2] = {
		{-0.742708, 0.209259},
		{-0.782292, 0.151852},
		{-0.785417, 0.131481},
		{-0.762500, 0.151852},
	};
	float ch_1_11[6][2] = {
		{-0.762500, 0.151852},
		{-0.729167, 0.157407},
		{-0.712500, 0.198148},
		{-0.715625, 0.222222},
		{-0.733333, 0.196296},
		{-0.754167, 0.177778},
	};
	float ch_1_12[7][2] = {
		{-0.762500, 0.151852},
		{-0.775000, 0.122222},
		{-0.780208, 0.107407},
		{-0.775000, 0.081481},
		{-0.706250, 0.133333},
		{-0.704167, 0.164815},
		{-0.729167, 0.157407},
	};
	float ch_1_13[4][2] = {
		{-0.687500, 0.225926},
		{-0.701042, 0.131481},
		{-0.687500, 0.122222},
		{-0.670833, 0.203704},
	};
	float ch_1_14[4][2] = {
		{-0.701042, 0.131481},
		{-0.736458, 0.075926},
		{-0.708333, 0.079630},
		{-0.687500, 0.122222},
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
