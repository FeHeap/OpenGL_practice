#include <GL/glew.h>
#include <GL/glut.h>

#define DrawPolygon(X) drawPolygon(X, sizeof(X) / sizeof(X[0]))
#define DrawTriangles(X) drawTriangles(X, sizeof(X) / sizeof(X[0]))
#define Polygon_xyConverse(X) polygon_xyConverse(X, sizeof(X) / sizeof(X[0]))
#define Triangles_xyConverse(X) triangles_xyConverse(X, sizeof(X) / sizeof(X[0]))

#define WIDTH 1920
#define HEIGHT 1080

float background_color[3] = { 1.0, 1.0, 1.0, };

static GLfloat spin = 0.0;
static GLfloat spinX = 0.0;
static GLfloat spinY = 0.0;
static GLfloat spinZ = 0.0;

static char drawMethod = '0';


void init(void);
void background(void);
void Display(void);
void Reshape(int, int);
void mouse(int, int, int, int);
void keyboard(unsigned char, int, int);
void spinDisplay();

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);		// set the position of Window
	glutInitWindowSize(1920, 1080);		// set the size of Window
	glutCreateWindow("NCHU");			// set the title of Window
	init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

void spinDisplay(void) {
	spin = spin + 2.0;
	if (spin > 360.0)
		spin = spin - 360.0;
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			glutIdleFunc(NULL);
			spinX = 0.0;
			spinY = 0.0;
			spinZ = 1.0;
			glutIdleFunc(spinDisplay);
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_DOWN) {
			glutIdleFunc(NULL);
			spinX = 0.0;
			spinY = 1.0;
			spinZ = 0.0;
			glutIdleFunc(spinDisplay);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			glutIdleFunc(NULL);
			spinX = 1.0;
			spinY = 0.0;
			spinZ = 0.0;
			glutIdleFunc(spinDisplay);
		}
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '0':
		drawMethod = '0';
		break;
	case '1':
		drawMethod = '1';
		break;
	case '2':
		drawMethod = '2';
		break;
	case '3':
		drawMethod = '3';
		break;
	case '4':
		drawMethod = '4';
		break;
	}
}

void init() {
	glewInit();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void Reshape(int width, int height) {
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-WIDTH, WIDTH, -HEIGHT, HEIGHT, 1000.0, -1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void background() {
	glColor3fv(background_color);
	glBegin(GL_POLYGON);
		glVertex2f(-WIDTH, HEIGHT);
		glVertex2f(-WIDTH, -HEIGHT);
		glVertex2f(WIDTH, -HEIGHT);
		glVertex2f(WIDTH, HEIGHT);
	glEnd();
}

class NchuCh {

private:

	int origin[2];
	void drawPolygon(int (*data)[2], int size) {
		if (drawMethod == '0') {
			glBegin(GL_POLYGON);
			for (int i = 0; i < size; i++) {
				glVertex2iv(data[i]);
			}
			glEnd();
		}
		else if (drawMethod == '1') {
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_INT, 0, data);
			glDrawArrays(GL_POLYGON, 0, size);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		else if (drawMethod == '2') {
			GLuint *offset = new GLuint[size];
			
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_INT, 0, data);
			for (unsigned int i = 0; i < size; i++) {
				*(offset + i) = i;
			}
			glDrawElements(GL_POLYGON, size, GL_UNSIGNED_INT, offset);
			glDisableClientState(GL_VERTEX_ARRAY);
			delete [] offset;
		}
	}
	
	// glMultiDrawArrays
	void displayMultiArrays_ch() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_INT, 0, MultiDrawPointsXY);
		glMultiDrawArrays(GL_POLYGON, MultiDrawStart, MultiDrawSize, 136);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	void displayMultiArrays_logo_out() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_INT, 0, &MultiDrawPointsXY[672]);
		int temp_start[1] = { 0 };
		glMultiDrawArrays(GL_POLYGON, temp_start, &MultiDrawSize[136], 1);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	void displayMultiArrays_logo_core() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_INT, 0, &MultiDrawPointsXY[676]);
		int temp_start[1] = { 0 };
		glMultiDrawArrays(GL_POLYGON, temp_start, &MultiDrawSize[137], 1);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	void displayMultiArrays_logo_cut() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_INT, 0, &MultiDrawPointsXY[699]);
		int temp_start[25];
		for (int i = 0; i < 25; i++) {
			temp_start[i] = MultiDrawStart[138 + i] - 699;
		}
		glMultiDrawArrays(GL_POLYGON, temp_start, &MultiDrawSize[138], 25);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	// glMultiDrawElements
	void displayMultiElements_ch() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_INT, 0, MultiDrawPointsXY);
		glMultiDrawElements(GL_POLYGON, MultiDrawSize, GL_UNSIGNED_INT, (const void**)MultiDrawIndices, 136);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	void displayMultiElements_logo_out() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_INT, 0, &MultiDrawPointsXY[672]);
		glMultiDrawElements(GL_POLYGON, &MultiDrawSize[136], GL_UNSIGNED_INT, (const void**)&MultiDrawIndices[136], 1);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	void displayMultiElements_logo_core() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_INT, 0, &MultiDrawPointsXY[676]);
		glMultiDrawElements(GL_POLYGON, &MultiDrawSize[137], GL_UNSIGNED_INT, (const void**)&MultiDrawIndices[137], 1);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	void displayMultiElements_logo_cut() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_INT, 0, &MultiDrawPointsXY[699]);
		glMultiDrawElements(GL_POLYGON, &MultiDrawSize[138], GL_UNSIGNED_INT, (const void**)&MultiDrawIndices[138], 25);
		glDisableClientState(GL_VERTEX_ARRAY);
	}


	// ch_color
	float ch_color[3] = { 0.007, 0.4375, 0.589 };

	// ch_1
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

	// ch_2
	int ch_2_1[6][2] = {
		{461, 371},
		{458, 385},
		{462, 394},
		{478, 403},
		{500, 398},
		{501, 390},
	};
	int ch_2_2[9][2] = {
		{500, 398},
		{478, 403},
		{433, 415},
		{429, 412},
		{414, 419},
		{409, 430},
		{413, 439},
		{422, 441},
		{477, 421},
	};
	int ch_2_3[5][2] = {
		{500, 398},
		{477, 421},
		{526, 417},
		{526, 403},
		{518, 397},
	};
	int ch_2_4[4][2] = {
		{477, 421},
		{439, 454},
		{471, 443},
		{500, 416},
	};
	int ch_2_5[4][2] = {
		{471, 443},
		{439, 454},
		{438, 471},
		{447, 476},
	};
	int ch_2_6[4][2] = {
		{471, 443},
		{447, 476},
		{514, 437},
		{518, 430},
	};
	int ch_2_7[5][2] = {
		{518, 430},
		{514, 437},
		{496, 483},
		{517, 462},
		{532, 438},
	};
	int ch_2_8[4][2] = {
		{514, 437},
		{462, 498},
		{485, 496},
		{496, 483},
	};
	int ch_2_9[7][2] = {
		{485, 496},
		{462, 498},
		{409, 504},
		{417, 515},
		{428, 522},
		{438, 523},
		{454, 517},
	};
	int ch_2_10[5][2] = {
		{485, 496},
		{454, 517},
		{499, 512},
		{508, 508},
		{518, 495},
	};
	int ch_2_11[6][2] = {
		{518, 495},
		{508, 508},
		{554, 512},
		{558, 507},
		{555, 496},
		{541, 494},
	};
	void display_ch_2() {
		DrawPolygon(ch_2_1);
		DrawPolygon(ch_2_2);
		DrawPolygon(ch_2_3);
		DrawPolygon(ch_2_4);
		DrawPolygon(ch_2_5);
		DrawPolygon(ch_2_6);
		DrawPolygon(ch_2_7);
		DrawPolygon(ch_2_8);
		DrawPolygon(ch_2_9);
		DrawPolygon(ch_2_10);
		DrawPolygon(ch_2_11);
	}

	// ch_3
	int ch_3_1[9][2] = {
		{596, 397},
		{589, 411},
		{591, 430},
		{601, 451},
		{608, 461},
		{617, 463},
		{610, 415},
		{607, 406},
		{603, 399},
	};
	int ch_3_2[4][2] = {
		{603, 399},
		{607, 406},
		{621, 407},
		{624, 393},
	};
	int ch_3_3[5][2] = {
		{624, 393},
		{621, 407},
		{629, 413},
		{647, 405},
		{642, 391},
	};
	int ch_3_4[4][2] = {
		{642, 391},
		{647, 405},
		{673, 394},
		{677, 385},
	};
	int ch_3_5[6][2] = {
		{677, 385},
		{673, 394},
		{740, 390},
		{734, 382},
		{724, 381},
		{693, 382},
	};
	int ch_3_6[6][2] = {
		{724, 381},
		{705, 404},
		{698, 413},
		{700, 422},
		{734, 402},
		{740, 390},
	};
	int ch_3_7[5][2] = {
		{700, 422},
		{698, 413},
		{681, 415},
		{678, 428},
		{691, 428},
	};
	int ch_3_8[5][2] = {
		{689, 357},
		{680, 362},
		{676, 388},
		{693, 386},
		{694, 363},
	};
	int ch_3_9[4][2] = {
		{684, 393},
		{681, 415},
		{692, 415},
		{693, 390},
	};
	int ch_3_10[10][2] = {
		{678, 428},
		{669, 430},
		{656, 438},
		{647, 444},
		{645, 452},
		{646, 460},
		{666, 454},
		{676, 451},
		{692, 436},
		{691, 428},
	};
	int ch_3_11[7][2] = {
		{692, 436},
		{676, 451},
		{675, 563},
		{677, 595},
		{687, 587},
		{692, 542},
		{692, 450},
	};
	void display_ch_3() {
		DrawPolygon(ch_3_1);
		DrawPolygon(ch_3_2);
		DrawPolygon(ch_3_3);
		DrawPolygon(ch_3_4);
		DrawPolygon(ch_3_5);
		DrawPolygon(ch_3_6);
		DrawPolygon(ch_3_7);
		DrawPolygon(ch_3_8);
		DrawPolygon(ch_3_9);
		DrawPolygon(ch_3_10);
		DrawPolygon(ch_3_11);
	}

	// ch_4
	int ch_4_1[5][2] = {
		{807, 383},
		{813, 388},
		{826, 385},
		{824, 381},
		{818, 377},
	};
	int ch_4_2[4][2] = {
		{813, 388},
		{813, 463},
		{825, 460},
		{826, 385},
	};
	int ch_4_3[8][2] = {
		{825, 460},
		{813, 463},
		{768, 475},
		{750, 485},
		{749, 493},
		{754, 498},
		{790, 482},
		{818, 470},
	};
	int ch_4_4[3][2] = {
		{754, 498},
		{763, 499},
		{790, 482},
	};
	int ch_4_5[3][2] = {
		{768, 475},
		{750, 475},
		{750, 485},
	};
	int ch_4_6[5][2] = {
		{825, 460},
		{818, 470},
		{838, 468},
		{852, 461},
		{836, 458},
	};
	int ch_4_7[3][2] = {
		{836, 458},
		{852, 461},
		{855, 451},
	};
	int ch_4_8[7][2] = {
		{835, 460},
		{844, 455},
		{845, 438},
		{846, 384},
		{841, 376},
		{836, 382},
		{835, 406},
	};
	int ch_4_9[5][2] = {
		{836, 382},
		{841, 376},
		{834, 372},
		{828, 373},
		{829, 379},
	};
	int ch_4_10[4][2] = {
		{855, 451},
		{852, 461},
		{891, 453},
		{891, 446},
	};
	int ch_4_11[7][2] = {
		{855, 451},
		{867, 452},
		{866, 381},
		{866, 375},
		{863, 372},
		{857, 382},
		{856, 414},
	};
	int ch_4_12[4][2] = {
		{857, 382},
		{863, 372},
		{853, 373},
		{854, 377},
	};
	int ch_4_13[8][2] = {
		{857, 395},
		{854, 403},
		{854, 409},
		{856, 414},
		{873, 404},
		{897, 394},
		{882, 388},
		{867, 391},
	};
	int ch_4_14[4][2] = {
		{873, 404},
		{892, 405},
		{897, 394},
		{882, 388},
	};
	int ch_4_15[4][2] = {
		{874, 403},
		{867, 427},
		{866, 439},
		{884, 403},
	};
	int ch_4_16[4][2] = {
		{866, 439},
		{870, 451},
		{883, 451},
		{873, 439},
	};
	int ch_4_17[4][2] = {
		{866, 439},
		{873, 439},
		{880, 428},
		{885, 416},
	};
	int ch_4_18[4][2] = {
		{880, 428},
		{878, 448},
		{892, 450},
		{892, 406},
	};
	int ch_4_19[7][2] = {
		{880, 428},
		{892, 406},
		{902, 364},
		{898, 358},
		{890, 356},
		{882, 362},
		{882, 391},
	};
	int ch_4_20[4][2] = {
		{882, 362},
		{870, 366},
		{864, 377},
		{886, 369},
	};
	int ch_4_21[5][2] = {
		{877, 455},
		{875, 466},
		{881, 468},
		{886, 463},
		{893, 448},
	};
	int ch_4_22[3][2] = {
		{858, 455},
		{858, 464},
		{869, 453},
	};
	int ch_4_23[7][2] = {
		{890, 445},
		{889, 452},
		{963, 450},
		{968, 446},
		{968, 438},
		{960, 434},
		{948, 438},
	};
	int ch_4_24[5][2] = {
		{902, 430},
		{903, 445},
		{905, 458},
		{910, 459},
		{917, 449},
	};
	int ch_4_25[3][2] = {
		{902, 430},
		{899, 418},
		{897, 419},
	};
	int ch_4_26[4][2] = {
		{912, 406},
		{894, 419},
		{926, 420},
		{925, 413},
	};
	int ch_4_27[6][2] = {
		{895, 393},
		{894, 399},
		{910, 422},
		{917, 426},
		{923, 425},
		{923, 417},
	};
	int ch_4_28[4][2] = {
		{908, 446},
		{918, 444},
		{918, 423},
		{910, 413},
	};
	int ch_4_29[3][2] = {
		{913, 409},
		{921, 413},
		{919, 380},
	};
	int ch_4_30[3][2] = {
		{919, 380},
		{920, 411},
		{928, 378},
	};
	int ch_4_31[7][2] = {
		{919, 380},
		{928, 378},
		{938, 365},
		{937, 358},
		{930, 359},
		{925, 362},
		{920, 372},
	};
	int ch_4_32[4][2] = {
		{925, 362},
		{917, 367},
		{910, 379},
		{922, 379},
	};
	int ch_4_33[4][2] = {
		{917, 367},
		{899, 373},
		{895, 380},
		{910, 379},
	};
	int ch_4_34[5][2] = {
		{851, 494},
		{801, 506},
		{820, 515},
		{834, 513},
		{844, 506},
	};
	int ch_4_35[7][2] = {
		{789, 495},
		{793, 510},
		{802, 521},
		{809, 525},
		{818, 521},
		{832, 513},
		{807, 504},
	};
	int ch_4_36[3][2] = {
		{859, 493},
		{874, 490},
		{886, 482},
	};
	int ch_4_37[5][2] = {
		{919, 477},
		{925, 490},
		{953, 527},
		{960, 532},
		{960, 524},
	};
	int ch_4_38[5][2] = {
		{960, 524},
		{964, 517},
		{963, 508},
		{960, 503},
		{922, 482},
	};
	void display_ch_4() {
		DrawPolygon(ch_4_1);
		DrawPolygon(ch_4_2);
		DrawPolygon(ch_4_3);
		DrawPolygon(ch_4_4);
		DrawPolygon(ch_4_5);
		DrawPolygon(ch_4_6);
		DrawPolygon(ch_4_7);
		DrawPolygon(ch_4_8);
		DrawPolygon(ch_4_9);
		DrawPolygon(ch_4_10);
		DrawPolygon(ch_4_11);
		DrawPolygon(ch_4_12);
		DrawPolygon(ch_4_13);
		DrawPolygon(ch_4_14);
		DrawPolygon(ch_4_15);
		DrawPolygon(ch_4_16);
		DrawPolygon(ch_4_17);
		DrawPolygon(ch_4_18);
		DrawPolygon(ch_4_19);
		DrawPolygon(ch_4_20);
		DrawPolygon(ch_4_21);
		DrawPolygon(ch_4_22);
		DrawPolygon(ch_4_23);
		DrawPolygon(ch_4_24);
		DrawPolygon(ch_4_25);
		DrawPolygon(ch_4_26);
		DrawPolygon(ch_4_27);
		DrawPolygon(ch_4_28);
		DrawPolygon(ch_4_29);
		DrawPolygon(ch_4_30);
		DrawPolygon(ch_4_31);
		DrawPolygon(ch_4_32);
		DrawPolygon(ch_4_33);
		DrawPolygon(ch_4_34);
		DrawPolygon(ch_4_35);
		DrawPolygon(ch_4_36);
		DrawPolygon(ch_4_37);
		DrawPolygon(ch_4_38);
	}

	// ch_5
	int ch_5_1[6][2] = {
		{1021, 436},
		{1020, 450},
		{1028, 459},
		{1042, 457},
		{1058, 443},
		{1054, 429},
	};
	int ch_5_2[4][2] = {
		{1054, 429},
		{1058, 443},
		{1095, 413},
		{1070, 422},
	};
	int ch_5_3[3][2] = {
		{1095, 413},
		{1058, 443},
		{1107, 424},
	};
	int ch_5_4[8][2] = {
		{1095, 413},
		{1107, 424},
		{1132, 412},
		{1137, 407},
		{1130, 406},
		{1123, 408},
		{1114, 408},
		{1107, 411},
	};
	int ch_5_5[4][2] = {
		{1132, 412},
		{1107, 424},
		{1130, 423},
		{1136, 418},
	};
	int ch_5_6[4][2] = {
		{1107, 411},
		{1114, 408},
		{1114, 366},
		{1108, 386},
	};
	int ch_5_7[5][2] = {
		{1108, 386},
		{1114, 366},
		{1111, 363},
		{1107, 366},
		{1101, 393},
	};
	int ch_5_8[4][2] = {
		{1108, 386},
		{1101, 393},
		{1094, 417},
		{1100, 413},
	};
	int ch_5_9[6][2] = {
		{1088, 431},
		{1080, 457},
		{1088, 467},
		{1096, 450},
		{1098, 444},
		{1099, 425},
	};
	int ch_5_10[5][2] = {
		{1088, 467},
		{1080, 457},
		{1067, 473},
		{1070, 493},
		{1082, 483},
	};
	int ch_5_11[5][2] = {
		{1067, 473},
		{1048, 487},
		{1037, 497},
		{1056, 497},
		{1070, 493},
	};
	int ch_5_12[6][2] = {
		{1111, 461},
		{1143, 485},
		{1164, 490},
		{1164, 484},
		{1159, 478},
		{1142, 471}
	};
	int ch_5_13[5][2] = {
		{1143, 485},
		{1149, 501},
		{1156, 506},
		{1163, 500},
		{1164, 490},
	};
	int ch_5_14[3][2] = {
		{1143, 485},
		{1127, 494},
		{1149, 501},
	};
	int ch_5_15[3][2] = {
		{1127, 494},
		{1119, 499},
		{1140, 494},
	};
	void display_ch_5() {
		DrawPolygon(ch_5_1);
		DrawPolygon(ch_5_2);
		DrawPolygon(ch_5_3);
		DrawPolygon(ch_5_4);
		DrawPolygon(ch_5_5);
		DrawPolygon(ch_5_6);
		DrawPolygon(ch_5_7);
		DrawPolygon(ch_5_8);
		DrawPolygon(ch_5_9);
		DrawPolygon(ch_5_10);
		DrawPolygon(ch_5_11);
		DrawPolygon(ch_5_12);
		DrawPolygon(ch_5_13);
		DrawPolygon(ch_5_14);
		DrawPolygon(ch_5_15);
	}

	// ch_6
	int ch_6_1[6][2] = {
		{1243, 351},
		{1238, 356},
		{1231, 372},
		{1235, 378},
		{1248, 366},
		{1249, 359},
	};
	int ch_6_2[7][2] = {
		{1222, 366},
		{1219, 375},
		{1219, 387},
		{1221, 398},
		{1245, 389},
		{1235, 378},
		{1231, 372},
	};
	int ch_6_3[3][2] = {
		{1235, 378},
		{1245, 389},
		{1248, 379},
	};
	int ch_6_4[5][2] = {
		{1250, 390},
		{1245, 389},
		{1221, 398},
		{1221, 405},
		{1240, 400},
	};
	int ch_6_5[4][2] = {
		{1240, 400},
		{1221, 405},
		{1224, 414},
		{1236, 412},
	};
	int ch_6_6[4][2] = {
		{1236, 412},
		{1251, 409},
		{1260, 401},
		{1255, 399},
	};
	int ch_6_7[4][2] = {
		{1236, 412},
		{1234, 428},
		{1249, 418},
		{1251, 409},
	};
	int ch_6_8[4][2] = {
		{1236, 412},
		{1224, 414},
		{1222, 449},
		{1234, 428},
	};
	int ch_6_9[3][2] = {
		{1222, 449},
		{1234, 451},
		{1234, 428},
	};
	int ch_6_10[5][2] = {
		{1222, 449},
		{1205, 459},
		{1216, 465},
		{1228, 463},
		{1234, 451},
	};
	int ch_6_11[3][2] = {
		{1205, 459},
		{1205, 483},
		{1216, 465},
	};
	int ch_6_12[5][2] = {
		{1208, 452},
		{1192, 471},
		{1188, 480},
		{1187, 489},
		{1205, 483},
	};
	int ch_6_13[5][2] = {
		{1205, 483},
		{1187, 489},
		{1190, 498},
		{1196, 499},
		{1202, 496},
	};
	int ch_6_14[4][2] = {
		{1230, 447},
		{1278, 425},
		{1276, 441},
		{1229, 457},
	};
	int ch_6_15[4][2] = {
		{1276, 441},
		{1342, 421},
		{1325, 421},
		{1274, 432},
	};
	int ch_6_16[7][2] = {
		{1342, 421},
		{1325, 426},
		{1341, 442},
		{1353, 441},
		{1359, 435},
		{1358, 429},
		{1352, 423},
	};
	int ch_6_17[4][2] = {
		{1334, 433},
		{1328, 442},
		{1331, 445},
		{1341, 442},
	};
	int ch_6_18[5][2] = {
		{1328, 442},
		{1323, 445},
		{1316, 452},
		{1327, 448},
		{1331, 445},
	};
	int ch_6_19[5][2] = {
		{1282, 419},
		{1264, 422},
		{1260, 428},
		{1258, 438},
		{1275, 433},
	};
	int ch_6_20[5][2] = {
		{1282, 419},
		{1261, 406},
		{1257, 410},
		{1259, 416},
		{1264, 422},
	};
	int ch_6_21[4][2] = {
		{1267, 412},
		{1278, 418},
		{1279, 407},
		{1272, 404},
	};
	int ch_6_22[4][2] = {
		{1272, 404},
		{1279, 407},
		{1284, 398},
		{1276, 395},
	};
	int ch_6_23[4][2] = {
		{1269, 392},
		{1279, 395},
		{1280, 385},
		{1276, 397},
	};
	int ch_6_24[7][2] = {
		{1279, 355},
		{1273, 363},
		{1268, 373},
		{1260, 390},
		{1263, 399},
		{1287, 365},
		{1285, 359},
	};
	int ch_6_25[7][2] = {
		{1291, 376},
		{1282, 376},
		{1267, 378},
		{1252, 381},
		{1256, 387},
		{1269, 390},
		{1289, 387}
	};
	int ch_6_26[4][2] = {
		{1290, 371},
		{1298, 375},
		{1308, 376},
		{1324, 373},
	};
	int ch_6_27[7][2] = {
		{1297, 373},
		{1324, 373},
		{1328, 367},
		{1327, 359},
		{1323, 356},
		{1317, 358},
		{1308, 365},
	};
	int ch_6_28[4][2] = {
		{1312, 374},
		{1312, 383},
		{1316, 390},
		{1324, 373},
	};
	int ch_6_29[8][2] = {
		{1316, 390},
		{1312, 383},
		{1303, 384},
		{1286, 388},
		{1287, 393},
		{1293, 400},
		{1314, 400},
		{1316, 396},
	};
	int ch_6_30[8][2] = {
		{1293, 400},
		{1288, 405},
		{1286, 412},
		{1287, 417},
		{1297, 418},
		{1313, 416},
		{1315, 409},
		{1314, 400},
	};
	int ch_6_31[4][2] = {
		{1297, 418},
		{1295, 429},
		{1310, 427},
		{1313, 416},
	};
	int ch_6_32[5][2] = {
		{1282, 452},
		{1275, 456},
		{1264, 461},
		{1271, 466},
		{1283, 469},
	};
	int ch_6_33[8][2] = {
		{1282, 452},
		{1283, 469},
		{1290, 469},
		{1295, 464},
		{1298, 454},
		{1297, 448},
		{1290, 445},
		{1285, 447},
	};
	int ch_6_34[9][2] = {
		{1290, 469},
		{1283, 469},
		{1278, 478},
		{1277, 483},
		{1279, 489},
		{1288, 491},
		{1299, 489},
		{1306, 483},
		{1299, 478},
	};
	int ch_6_35[5][2] = {
		{1279, 489},
		{1277, 483},
		{1268, 485},
		{1253, 490},
		{1260, 492},
	};
	int ch_6_36[5][2] = {
		{1260, 492},
		{1253, 490},
		{1243, 491},
		{1240, 496},
		{1249, 498},
	};
	int ch_6_37[5][2] = {
		{1299, 478},
		{1306, 483},
		{1320, 483},
		{1327, 477},
		{1316, 476},
	};
	int ch_6_38[4][2] = {
		{1327, 477},
		{1320, 483},
		{1329, 487},
		{1332, 482},
	};
	int ch_6_39[5][2] = {
		{1288, 491},
		{1290, 500},
		{1298, 534},
		{1300, 507},
		{1299, 489},
	};
	int ch_6_40[6][2] = {
		{1298, 534},
		{1290, 500},
		{1287, 529},
		{1287, 561},
		{1293, 554},
		{1297, 547},
	};
	int ch_6_41[4][2] = {
		{1287, 561},
		{1287, 529},
		{1276, 547},
		{1271, 571},
	};
	int ch_6_42[3][2] = {
		{1271, 571},
		{1251, 568},
		{1276, 547},
	};
	int ch_6_43[3][2] = {
		{1271, 571},
		{1251, 568},
		{1248, 584},
	};
	int ch_6_44[3][2] = {
		{1251, 568},
		{1232, 579},
		{1248, 584},
	};
	int ch_6_45[4][2] = {
		{1232, 579},
		{1209, 586},
		{1227, 590},
		{1248, 584},
	};
	int ch_6_46[3][2] = {
		{1232, 579},
		{1200, 584},
		{1209, 586},
	};
	int ch_6_47[3][2] = {
		{1223, 583},
		{1186, 578},
		{1200, 584},
	};
	void display_ch_6() {
		DrawPolygon(ch_6_1);
		DrawPolygon(ch_6_2);
		DrawPolygon(ch_6_3);
		DrawPolygon(ch_6_4);
		DrawPolygon(ch_6_5);
		DrawPolygon(ch_6_6);
		DrawPolygon(ch_6_7);
		DrawPolygon(ch_6_8);
		DrawPolygon(ch_6_9);
		DrawPolygon(ch_6_10);
		DrawPolygon(ch_6_11);
		DrawPolygon(ch_6_12);
		DrawPolygon(ch_6_13);
		DrawPolygon(ch_6_14);
		DrawPolygon(ch_6_15);
		DrawPolygon(ch_6_16);
		DrawPolygon(ch_6_17);
		DrawPolygon(ch_6_18);
		DrawPolygon(ch_6_19);
		DrawPolygon(ch_6_20);
		DrawPolygon(ch_6_21);
		DrawPolygon(ch_6_22);
		DrawPolygon(ch_6_23);
		DrawPolygon(ch_6_24);
		DrawPolygon(ch_6_25);
		DrawPolygon(ch_6_26);
		DrawPolygon(ch_6_27);
		DrawPolygon(ch_6_28);
		DrawPolygon(ch_6_29);
		DrawPolygon(ch_6_30);
		DrawPolygon(ch_6_31);
		DrawPolygon(ch_6_32);
		DrawPolygon(ch_6_33);
		DrawPolygon(ch_6_34);
		DrawPolygon(ch_6_35);
		DrawPolygon(ch_6_36);
		DrawPolygon(ch_6_37);
		DrawPolygon(ch_6_38);
		DrawPolygon(ch_6_39);
		DrawPolygon(ch_6_40);
		DrawPolygon(ch_6_41);
		DrawPolygon(ch_6_42);
		DrawPolygon(ch_6_43);
		DrawPolygon(ch_6_44);
		DrawPolygon(ch_6_45);
		DrawPolygon(ch_6_46);
		DrawPolygon(ch_6_47);
	}

	// logo_out_color
	float logo_out_color[3] = { 0.007, 0.4375, 0.589 };
	// logo_core_color
	float logo_core_color[3] = { 0.898, 0.102, 0.129 };
	// logo_out
	int logo_out[4][2] = {
		{1491, 327},
		{1475, 561},
		{1734, 564},
		{1719, 328},
	};
	// logo_core
	int logo_core[23][2] = {
		{1601, 386},
		{1588, 388},
		{1575, 395},
		{1568, 400},
		{1559, 413},
		{1554, 428},
		{1553, 444},
		{1557, 458},
		{1565, 470},
		{1576, 480},
		{1590, 487},
		{1608, 489},
		{1625, 486},
		{1638, 479},
		{1650, 469},
		{1656, 456},
		{1657, 443},
		{1657, 430},
		{1654, 417},
		{1647, 405},
		{1638, 397},
		{1627, 389},
		{1614, 387},
	};
	// logo_cut
	int logo_cut_left[14][2] = {
		{1491, 327},
		{1475, 561},
		{1481, 549},
		{1486, 537},
		{1492, 519},
		{1498, 499},
		{1504, 475},
		{1507, 453},
		{1509, 431},
		{1510, 409},
		{1508, 386},
		{1504, 368},
		{1500, 354},
		{1496, 338},
	};
	int logo_cut_right[13][2] = {
		{1719, 328},
		{1710, 346},
		{1704, 365},
		{1700, 385},
		{1697, 407},
		{1697, 431},
		{1699, 451},
		{1702, 468},
		{1706, 487},
		{1711, 506},
		{1719, 526},
		{1726, 544},
		{1734, 564},
	};
	int logo_cut_above[11][2] = {
		{1491, 327},
		{1510, 332},
		{1527, 336},
		{1547, 340},
		{1571, 343},
		{1598, 344},
		{1625, 343},
		{1648, 341},
		{1673, 338},
		{1695, 334},
		{1719, 328},
	};
	int logo_cut_below[14][2] = {
		{1475, 561},
		{1734, 564},
		{1722, 557},
		{1704, 550},
		{1682, 542},
		{1672, 539},
		{1652, 534},
		{1632, 531},
		{1606, 529},
		{1579, 530},
		{1555, 533},
		{1535, 538},
		{1517, 545},
		{1536, 551},
	};
	int logo_cut_naka[21][3][2] = {
		{{1496, 338}, {1500, 354}, {1513, 343},},
		{{1513, 343}, {1500, 354}, {1518, 360},},
		{{1513, 343}, {1518, 360}, {1536, 353},},
		{{1536, 353}, {1518, 360}, {1536, 369},},
		{{1536, 353}, {1536, 369}, {1560, 366},},
		{{1560, 366}, {1536, 369}, {1561, 383},},
		{{1560, 366}, {1561, 383}, {1583, 382},},
		{{1583, 382}, {1561, 383}, {1582, 398},},
		{{1583, 382}, {1582, 398}, {1601, 401},},
		{{1601, 401}, {1582, 398}, {1597, 412},},
		{{1601, 401}, {1597, 412}, {1617, 419},},
		{{1617, 419}, {1597, 412}, {1613, 431},},
		{{1617, 419}, {1613, 431}, {1635, 441},},
		{{1635, 441}, {1613, 431}, {1625, 446},},
		{{1635, 441}, {1625, 446}, {1650, 467},},
		{{1650, 467}, {1625, 446}, {1642, 472},},
		{{1650, 467}, {1642, 472}, {1659, 487},},
		{{1659, 487}, {1642, 472}, {1656, 501},},
		{{1659, 487}, {1656, 501}, {1670, 516},},
		{{1670, 516}, {1656, 501}, {1664, 522},},
		{{1670, 516}, {1664, 522}, {1672, 539},},
	};

	GLint MultiDrawPointsXY[814][2];
	int countPointIndex = 0;
	GLint MultiDrawStart[163];
	GLsizei MultiDrawSize[163];
	int countGraphIndex = 0;
	GLuint* MultiDrawIndices[163];

	void polygon_xyConverse(int (*data)[2], int size) {
		MultiDrawIndices[countGraphIndex] = new GLuint[size];
		MultiDrawStart[countGraphIndex] = countPointIndex;
		MultiDrawSize[countGraphIndex++] = size;
		for (int i = 0; i < size; i++) {
			data[i][0] = (data[i][0] - origin[0]);
			data[i][1] = (origin[1] - data[i][1]);
			if (countPointIndex < 672) {
				MultiDrawIndices[countGraphIndex - 1][i] = countPointIndex;
			}
			else if (countPointIndex < 676) {
				MultiDrawIndices[countGraphIndex - 1][i] = countPointIndex-672;
			}
			else if (countPointIndex < 699) {
				MultiDrawIndices[countGraphIndex - 1][i] = countPointIndex - 676;
			}
			else {
				MultiDrawIndices[countGraphIndex - 1][i] = countPointIndex - 699;
			}
			MultiDrawPointsXY[countPointIndex][0] = data[i][0];
			MultiDrawPointsXY[countPointIndex++][1] = data[i][1];
		}
	}

	void presetData() {
		Polygon_xyConverse(ch_1_1);
		Polygon_xyConverse(ch_1_2);
		Polygon_xyConverse(ch_1_3);
		Polygon_xyConverse(ch_1_4);
		Polygon_xyConverse(ch_1_5);
		Polygon_xyConverse(ch_1_6);
		Polygon_xyConverse(ch_1_7);
		Polygon_xyConverse(ch_1_8);
		Polygon_xyConverse(ch_1_9);
		Polygon_xyConverse(ch_1_10);
		Polygon_xyConverse(ch_1_11);
		Polygon_xyConverse(ch_1_12);
		Polygon_xyConverse(ch_1_13);
		Polygon_xyConverse(ch_1_14);
		Polygon_xyConverse(ch_2_1);
		Polygon_xyConverse(ch_2_2);
		Polygon_xyConverse(ch_2_3);
		Polygon_xyConverse(ch_2_4);
		Polygon_xyConverse(ch_2_5);
		Polygon_xyConverse(ch_2_6);
		Polygon_xyConverse(ch_2_7);
		Polygon_xyConverse(ch_2_8);
		Polygon_xyConverse(ch_2_9);
		Polygon_xyConverse(ch_2_10);
		Polygon_xyConverse(ch_2_11);
		Polygon_xyConverse(ch_3_1);
		Polygon_xyConverse(ch_3_2);
		Polygon_xyConverse(ch_3_3);
		Polygon_xyConverse(ch_3_4);
		Polygon_xyConverse(ch_3_5);
		Polygon_xyConverse(ch_3_6);
		Polygon_xyConverse(ch_3_7);
		Polygon_xyConverse(ch_3_8);
		Polygon_xyConverse(ch_3_9);
		Polygon_xyConverse(ch_3_10);
		Polygon_xyConverse(ch_3_11);
		Polygon_xyConverse(ch_4_1);
		Polygon_xyConverse(ch_4_2);
		Polygon_xyConverse(ch_4_3);
		Polygon_xyConverse(ch_4_4);
		Polygon_xyConverse(ch_4_5);
		Polygon_xyConverse(ch_4_6);
		Polygon_xyConverse(ch_4_7);
		Polygon_xyConverse(ch_4_8);
		Polygon_xyConverse(ch_4_9);
		Polygon_xyConverse(ch_4_10);
		Polygon_xyConverse(ch_4_11);
		Polygon_xyConverse(ch_4_12);
		Polygon_xyConverse(ch_4_13);
		Polygon_xyConverse(ch_4_14);
		Polygon_xyConverse(ch_4_15);
		Polygon_xyConverse(ch_4_16);
		Polygon_xyConverse(ch_4_17);
		Polygon_xyConverse(ch_4_18);
		Polygon_xyConverse(ch_4_19);
		Polygon_xyConverse(ch_4_20);
		Polygon_xyConverse(ch_4_21);
		Polygon_xyConverse(ch_4_22);
		Polygon_xyConverse(ch_4_23);
		Polygon_xyConverse(ch_4_24);
		Polygon_xyConverse(ch_4_25);
		Polygon_xyConverse(ch_4_26);
		Polygon_xyConverse(ch_4_27);
		Polygon_xyConverse(ch_4_28);
		Polygon_xyConverse(ch_4_29);
		Polygon_xyConverse(ch_4_30);
		Polygon_xyConverse(ch_4_31);
		Polygon_xyConverse(ch_4_32);
		Polygon_xyConverse(ch_4_33);
		Polygon_xyConverse(ch_4_34);
		Polygon_xyConverse(ch_4_35);
		Polygon_xyConverse(ch_4_36);
		Polygon_xyConverse(ch_4_37);
		Polygon_xyConverse(ch_4_38);
		Polygon_xyConverse(ch_5_1);
		Polygon_xyConverse(ch_5_2);
		Polygon_xyConverse(ch_5_3);
		Polygon_xyConverse(ch_5_4);
		Polygon_xyConverse(ch_5_5);
		Polygon_xyConverse(ch_5_6);
		Polygon_xyConverse(ch_5_7);
		Polygon_xyConverse(ch_5_8);
		Polygon_xyConverse(ch_5_9);
		Polygon_xyConverse(ch_5_10);
		Polygon_xyConverse(ch_5_11);
		Polygon_xyConverse(ch_5_12);
		Polygon_xyConverse(ch_5_13);
		Polygon_xyConverse(ch_5_14);
		Polygon_xyConverse(ch_5_15);
		Polygon_xyConverse(ch_6_1);
		Polygon_xyConverse(ch_6_2);
		Polygon_xyConverse(ch_6_3);
		Polygon_xyConverse(ch_6_4);
		Polygon_xyConverse(ch_6_5);
		Polygon_xyConverse(ch_6_6);
		Polygon_xyConverse(ch_6_7);
		Polygon_xyConverse(ch_6_8);
		Polygon_xyConverse(ch_6_9);
		Polygon_xyConverse(ch_6_10);
		Polygon_xyConverse(ch_6_11);
		Polygon_xyConverse(ch_6_12);
		Polygon_xyConverse(ch_6_13);
		Polygon_xyConverse(ch_6_14);
		Polygon_xyConverse(ch_6_15);
		Polygon_xyConverse(ch_6_16);
		Polygon_xyConverse(ch_6_17);
		Polygon_xyConverse(ch_6_18);
		Polygon_xyConverse(ch_6_19);
		Polygon_xyConverse(ch_6_20);
		Polygon_xyConverse(ch_6_21);
		Polygon_xyConverse(ch_6_22);
		Polygon_xyConverse(ch_6_23);
		Polygon_xyConverse(ch_6_24);
		Polygon_xyConverse(ch_6_25);
		Polygon_xyConverse(ch_6_26);
		Polygon_xyConverse(ch_6_27);
		Polygon_xyConverse(ch_6_28);
		Polygon_xyConverse(ch_6_29);
		Polygon_xyConverse(ch_6_30);
		Polygon_xyConverse(ch_6_31);
		Polygon_xyConverse(ch_6_32);
		Polygon_xyConverse(ch_6_33);
		Polygon_xyConverse(ch_6_34);
		Polygon_xyConverse(ch_6_35);
		Polygon_xyConverse(ch_6_36);
		Polygon_xyConverse(ch_6_37);
		Polygon_xyConverse(ch_6_38);
		Polygon_xyConverse(ch_6_39);
		Polygon_xyConverse(ch_6_40);
		Polygon_xyConverse(ch_6_41);
		Polygon_xyConverse(ch_6_42);
		Polygon_xyConverse(ch_6_43);
		Polygon_xyConverse(ch_6_44);
		Polygon_xyConverse(ch_6_45);
		Polygon_xyConverse(ch_6_46);
		Polygon_xyConverse(ch_6_47);
		Polygon_xyConverse(logo_out);
		Polygon_xyConverse(logo_core);
		Polygon_xyConverse(logo_cut_left);
		Polygon_xyConverse(logo_cut_right);
		Polygon_xyConverse(logo_cut_above);
		Polygon_xyConverse(logo_cut_below);
		for (int i = 0; i < sizeof(logo_cut_naka) / sizeof(logo_cut_naka[0]); i++) {
			Polygon_xyConverse(logo_cut_naka[i]);
		}
	}

public:

	NchuCh(int width, int height) {
		origin[0] = width / 2;
		origin[1] = height / 2;
		presetData();
	}

	~NchuCh() {
		for (int i = 0; i < 163; i++) {
			delete [] MultiDrawIndices[i];
		}
	}

	void display_ch() {
		glColor3fv(ch_color);
		switch (drawMethod) {
		case '0':
		case '1':
		case '2':
			display_ch_1();
			display_ch_2();
			display_ch_3();
			display_ch_4();
			display_ch_5();
			display_ch_6();
			break;
		case '3':
			displayMultiArrays_ch();
			break;
		case '4':
			displayMultiElements_ch();
			break;
		}
	}

	void display_logo() {
		switch (drawMethod) {
		case '0':
		case '1':
		case '2':
			glColor3fv(logo_out_color);
			DrawPolygon(logo_out);
			glColor3fv(logo_core_color);
			DrawPolygon(logo_core);
			glColor3fv(background_color);
			DrawPolygon(logo_cut_left);
			DrawPolygon(logo_cut_right);
			DrawPolygon(logo_cut_above);
			DrawPolygon(logo_cut_below);
			for (int i = 0; i < sizeof(logo_cut_naka) / sizeof(logo_cut_naka[0]); i++) {
				DrawPolygon(logo_cut_naka[i]);
			}
			break;
		case '3':
			glColor3fv(logo_out_color);
			displayMultiArrays_logo_out();
			glColor3fv(logo_core_color);
			displayMultiArrays_logo_core();
			glColor3fv(background_color);
			displayMultiArrays_logo_cut();
			break;
		case '4':
			glColor3fv(logo_out_color);
			displayMultiElements_logo_out();
			glColor3fv(logo_core_color);
			displayMultiElements_logo_core();
			glColor3fv(background_color);
			displayMultiElements_logo_cut();
			break;
		}
	}

} word_ch(WIDTH, HEIGHT);

class NchuEn {
private:

	int origin[2];
	void drawTriangles(int(*data)[3][2], int size) {
		for (int i = 0; i < size; i++) {
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < 3; j++) {
				glVertex2iv(data[i][j]);
			}
			glEnd();
		}

	}

	// en_color
	float en_color[3] = { 0.007, 0.4375, 0.589 };

	// National
	int National[130][3][2] = {
		// N
		{{200, 696}, {200, 763}, {207, 763},},
		{{200, 696}, {207, 763}, {207, 696},},
		{{207, 696}, {200, 696}, {247, 763},},
		{{207, 696}, {247, 763}, {255, 763},},
		{{247, 763}, {255, 763}, {247, 696},},
		{{247, 696}, {255, 763}, {255, 696},},
		// a
		{{276, 725}, {280, 727}, {280, 718},},
		{{280, 718}, {280, 727}, {286, 721},},
		{{280, 718}, {286, 721}, {288, 715},},
		{{288, 715}, {286, 721}, {294, 719},},
		{{288, 715}, {294, 719}, {298, 714},},
		{{298, 714}, {294, 719}, {301, 720},},
		{{298, 714}, {301, 720}, {309, 716},},
		{{309, 716}, {301, 720}, {308, 724},},
		{{309, 716}, {308, 724}, {316, 725},},
		{{316, 725}, {308, 724}, {310, 732},},
		{{316, 725}, {310, 732}, {310, 763},},
		{{316, 725}, {310, 763}, {316, 763},},
		{{311, 732}, {286, 737}, {311, 737},},
		{{311, 737}, {286, 737}, {289, 741},},
		{{286, 737}, {278, 741}, {289, 741},},
		{{289, 741}, {278, 741}, {274, 746},},
		{{289, 741}, {278, 741}, {284, 744},},
		{{284, 744}, {278, 741}, {274, 746},},
		{{274, 746}, {278, 750}, {284, 744},},
		{{278, 741}, {274, 755}, {274, 746},},
		{{274, 755}, {279, 761}, {274, 746},},
		{{274, 746}, {279, 761}, {288, 763},},
		{{281, 756}, {279, 761}, {288, 763},},
		{{281, 756}, {288, 763}, {300, 763},},
		{{304, 759}, {285, 760}, {300, 763},},
		{{304, 759}, {310, 748}, {295, 760},},
		{{310, 748}, {304, 759}, {310, 753},},
		{{311, 759}, {317, 763}, {317, 759},},
		{{311, 759}, {311, 763}, {317, 763},},
		// t
		{{332, 716}, {332, 720}, {354, 720},},
		{{332, 716}, {354, 720}, {354, 716},},
		{{344, 699}, {338, 703}, {338, 757},},
		{{344, 699}, {338, 757}, {344, 757},},
		{{344, 757}, {338, 757}, {341, 761},},
		{{344, 757}, {341, 761}, {343, 763},},
		{{344, 757}, {343, 763}, {351, 763},},
		{{344, 757}, {351, 763}, {354, 758},},
		{{351, 763}, {354, 758}, {355, 763},},
		// i
		{{368, 763}, {375, 763}, {368, 715},},
		{{368, 715}, {375, 763}, {375, 715},},
		{{368, 695}, {368, 704}, {375, 704},},
		{{368, 695}, {375, 704}, {375, 695},},
		// o
		{{439, 739}, {432, 739}, {430, 749},},
		{{439, 739}, {430, 749}, {438, 749},},
		{{438, 749}, {430, 749}, {435, 756},},
		{{435, 756}, {430, 749}, {428, 754},},
		{{428, 754}, {428, 760}, {435, 756},},
		{{428, 754}, {423, 758}, {428, 760},},
		{{428, 760}, {423, 758}, {421, 763},},
		{{423, 758}, {418, 760}, {421, 763},},
		{{421, 763}, {418, 760}, {412, 763},},
		{{418, 760}, {409, 758}, {412, 763},},
		{{409, 758}, {406, 761}, {412, 763},},
		{{409, 758}, {405, 755}, {406, 761},},
		{{405, 755}, {401, 758}, {406, 761},},
		{{405, 755}, {403, 749}, {401, 758},},
		{{403, 749}, {396, 750}, {401, 758},},
		{{403, 749}, {401, 743}, {396, 750},},
		{{401, 743}, {395, 745}, {396, 750},},
		{{401, 743}, {401, 739}, {395, 745},},
		{{401, 739}, {395, 740}, {395, 745},},
		{{401, 739}, {402, 734}, {395, 740},},
		{{402, 734}, {395, 733}, {395, 740},},
		{{402, 734}, {404, 729}, {395, 733},},
		{{404, 729}, {397, 727}, {395, 733},},
		{{404, 729}, {406, 725}, {397, 727},},
		{{406, 725}, {400, 723}, {397, 727},},
		{{406, 725}, {405, 719}, {400, 723},},
		{{406, 725}, {413, 721}, {405, 719},},
		{{413, 721}, {411, 717}, {405, 719},},
		{{413, 721}, {420, 717}, {411, 717},},
		{{413, 721}, {421, 721}, {420, 717},},
		{{421, 721}, {429, 719}, {420, 717},},
		{{421, 721}, {428, 724}, {429, 719},},
		{{428, 724}, {437, 726}, {429, 719},},
		{{428, 724}, {431, 729}, {437, 726},},
		{{431, 729}, {439, 734}, {437, 726},},
		{{431, 729}, {433, 734}, {439, 734},},
		{{433, 734}, {439, 739}, {439, 734},},
		{{433, 734}, {432, 739}, {439, 739},},
		// n
		{{464, 716}, {459, 716}, {459, 763},},
		{{464, 716}, {459, 763}, {464, 763},},
		{{464, 733}, {470, 717}, {464, 724},},
		{{470, 717}, {468, 723}, {474, 720},},
		{{470, 717}, {474, 720}, {478, 716},},
		{{478, 716}, {474, 720}, {481, 719},},
		{{478, 716}, {481, 719}, {485, 715},},
		{{485, 715}, {481, 719}, {488, 722},},
		{{485, 715}, {488, 722}, {494, 719},},
		{{494, 719}, {488, 722}, {491, 725},},
		{{494, 719}, {491, 725}, {497, 725},},
		{{497, 725}, {491, 725}, {491, 763},},
		{{497, 725}, {491, 763}, {497, 763},},
		// a
		{{517, 725}, {521, 727}, {521, 718},},
		{{521, 718}, {521, 727}, {527, 721},},
		{{521, 718}, {527, 721}, {529, 715},},
		{{529, 715}, {527, 721}, {535, 719},},
		{{529, 715}, {535, 719}, {539, 714},},
		{{539, 714}, {535, 719}, {542, 720},},
		{{539, 714}, {542, 720}, {550, 716},},
		{{550, 716}, {542, 720}, {549, 724},},
		{{550, 716}, {549, 724}, {557, 725},},
		{{557, 725}, {549, 724}, {551, 732},},
		{{557, 725}, {551, 732}, {551, 763},},
		{{557, 725}, {551, 763}, {557, 763},},
		{{552, 732}, {527, 737}, {552, 737},},
		{{552, 737}, {527, 737}, {530, 741},},
		{{527, 737}, {519, 741}, {530, 741},},
		{{530, 741}, {519, 741}, {515, 746},},
		{{530, 741}, {519, 741}, {525, 744},},
		{{525, 744}, {519, 741}, {515, 746},},
		{{515, 746}, {519, 750}, {525, 744},},
		{{519, 741}, {515, 755}, {515, 746},},
		{{515, 755}, {520, 761}, {515, 746},},
		{{515, 746}, {520, 761}, {529, 763},},
		{{522, 756}, {520, 761}, {529, 763},},
		{{522, 756}, {529, 763}, {541, 763},},
		{{545, 759}, {526, 760}, {541, 763},},
		{{545, 759}, {551, 748}, {536, 760},},
		{{551, 748}, {545, 759}, {551, 753},},
		{{552, 759}, {558, 763}, {558, 759},},
		{{552, 759}, {552, 763}, {558, 763},},
		// l
		{{580, 763}, {586, 763}, {580, 695},},
		{{580, 695}, {586, 763}, {586, 695},},
	};

	// Chung
	int Chung[121][3][2] = {
		// C
		{{686, 741}, {679, 741}, {684, 750},},
		{{679, 741}, {677, 748}, {684, 750},},
		{{677, 748}, {678, 756}, {684, 750},},
		{{677, 748}, {672, 754}, {678, 756},},
		{{672, 754}, {672, 761}, {678, 756},},
		{{672, 754}, {666, 757}, {672, 761},},
		{{666, 757}, {663, 763}, {672, 761},},
		{{666, 757}, {658, 758}, {663, 763},},
		{{658, 758}, {652, 763}, {663, 763},},
		{{658, 758}, {650, 757}, {652, 763},},
		{{650, 757}, {643, 761}, {652, 763},},
		{{650, 757}, {643, 754}, {643, 761},},
		{{643, 754}, {635, 754}, {643, 761},},
		{{643, 754}, {638, 748}, {635, 754},},
		{{638, 748}, {631, 748}, {635, 754},},
		{{638, 748}, {635, 742}, {631, 748},},
		{{635, 742}, {628, 741}, {631, 748},},
		{{635, 742}, {628, 735}, {628, 741},},
		{{635, 742}, {634, 732}, {628, 735},},
		{{634, 732}, {627, 729}, {628, 735},},
		{{634, 732}, {634, 725}, {627, 729},},
		{{634, 725}, {629, 722}, {627, 729},},
		{{634, 725}, {628, 722}, {627, 729},},
		{{634, 725}, {635, 720}, {628, 722},},
		{{635, 720}, {630, 713}, {628, 722},},
		{{635, 720}, {637, 714}, {630, 713},},
		{{637, 714}, {634, 707}, {630, 713},},
		{{637, 714}, {640, 708}, {634, 707},},
		{{640, 708}, {640, 701}, {634, 707},},
		{{640, 708}, {646, 704}, {640, 701},},
		{{646, 704}, {648, 697}, {640, 701},},
		{{646, 704}, {655, 701}, {648, 697},},
		{{655, 701}, {657, 695}, {648, 697},},
		{{655, 701}, {662, 701}, {657, 695},},
		{{662, 701}, {668, 697}, {657, 695},},
		{{662, 701}, {670, 705}, {668, 697},},
		{{670, 705}, {678, 703}, {668, 697},},
		{{670, 705}, {676, 709}, {678, 703},},
		{{676, 709}, {683, 708}, {678, 703},},
		{{676, 709}, {677, 714}, {683, 708},},
		{{677, 714}, {685, 714}, {683, 708},},
		// h
		{{709, 695}, {704, 695}, {704, 763},},
		{{709, 695}, {704, 763}, {709, 763},},
		{{709, 733}, {715, 717}, {709, 724},},
		{{715, 717}, {713, 723}, {719, 720},},
		{{715, 717}, {719, 720}, {723, 716},},
		{{723, 716}, {719, 720}, {726, 719},},
		{{723, 716}, {726, 719}, {730, 715},},
		{{730, 715}, {726, 719}, {733, 722},},
		{{730, 715}, {733, 722}, {739, 719},},
		{{739, 719}, {733, 722}, {736, 725},},
		{{739, 719}, {736, 725}, {742, 725},},
		{{742, 725}, {736, 725}, {736, 763},},
		{{742, 725}, {736, 763}, {742, 763},},
		// u
		{{801, 763}, {806, 763}, {806, 716},},
		{{801, 763}, {806, 716}, {801, 716},},
		{{801, 746}, {795, 762}, {801, 755},},
		{{795, 762}, {797, 756}, {791, 759},},
		{{795, 762}, {791, 759}, {787, 763},},
		{{787, 763}, {791, 759}, {784, 760},},
		{{787, 763}, {784, 760}, {780, 764},},
		{{780, 764}, {784, 760}, {777, 757},},
		{{780, 764}, {777, 757}, {771, 760},},
		{{771, 760}, {777, 757}, {774, 754},},
		{{771, 760}, {774, 754}, {768, 754},},
		{{768, 754}, {774, 754}, {774, 716},},
		{{768, 754}, {774, 716}, {768, 716},},
		// n
		{{832, 716}, {827, 716}, {827, 763},},
		{{832, 716}, {827, 763}, {832, 763},},
		{{832, 733}, {838, 717}, {832, 724},},
		{{838, 717}, {836, 723}, {842, 720},},
		{{838, 717}, {842, 720}, {846, 716},},
		{{846, 716}, {842, 720}, {849, 719},},
		{{846, 716}, {849, 719}, {853, 715},},
		{{853, 715}, {849, 719}, {856, 722},},
		{{853, 715}, {856, 722}, {862, 719},},
		{{862, 719}, {856, 722}, {859, 725},},
		{{862, 719}, {859, 725}, {865, 725},},
		{{865, 725}, {859, 725}, {859, 763},},
		{{865, 725}, {859, 763}, {865, 763},},
		// g
		{{919, 715}, {926, 768}, {926, 715},},
		{{919, 770}, {926, 768}, {919, 715},},
		{{919, 770}, {919, 777}, {926, 768},},
		{{919, 770}, {911, 778}, {919, 777},},
		{{911, 778}, {910, 782}, {919, 777},},
		{{911, 778}, {905, 779}, {910, 782},},
		{{905, 779}, {904, 784}, {910, 782},},
		{{905, 779}, {899, 779}, {904, 784},},
		{{899, 779}, {895, 781}, {904, 784},},
		{{899, 779}, {894, 776}, {895, 781},},
		{{894, 776}, {887, 776}, {895, 781},},
		{{894, 776}, {891, 771}, {887, 776},},
		{{891, 771}, {885, 771}, {887, 776},},
		{{919, 724}, {914, 722}, {919, 730},},
		{{919, 724}, {914, 717}, {914, 722},},
		{{914, 717}, {909, 719}, {914, 722},},
		{{914, 717}, {908, 715}, {909, 719},},
		{{908, 715}, {905, 719}, {909, 719},},
		{{908, 715}, {902, 715}, {905, 719},},
		{{902, 715}, {899, 720}, {905, 719},},
		{{902, 715}, {894, 717}, {899, 720},},
		{{894, 717}, {894, 724}, {899, 720},},
		{{894, 717}, {887, 723}, {894, 724},},
		{{887, 723}, {891, 730}, {894, 724},},
		{{887, 723}, {885, 730}, {891, 730},},
		{{885, 730}, {891, 740}, {891, 730},},
		{{885, 730}, {885, 740}, {891, 740},},
		{{919, 754}, {914, 756}, {919, 748},},
		{{919, 754}, {914, 761}, {914, 756},},
		{{914, 761}, {909, 759}, {914, 756},},
		{{914, 761}, {908, 763}, {909, 759},},
		{{908, 763}, {905, 759}, {909, 759},},
		{{908, 763}, {902, 763}, {905, 759},},
		{{902, 763}, {899, 758}, {905, 759},},
		{{902, 763}, {894, 761}, {899, 758},},
		{{894, 761}, {894, 754}, {899, 758},},
		{{894, 761}, {887, 755}, {894, 754},},
		{{887, 755}, {891, 748}, {894, 754},},
		{{887, 755}, {885, 748}, {891, 748},},
		{{885, 748}, {891, 738}, {891, 748},},
		{{885, 748}, {885, 738}, {891, 738},},
	};

	// Hsing
	int Hsing[99][3][2] = {
		// H
		{{971, 696}, {971, 763}, {978, 763},},
		{{971, 696}, {978, 763}, {978, 696},},
		{{978, 724}, {978, 731}, {1018, 731},},
		{{978, 724}, {1018, 731}, {1018, 724},},
		{{1018, 763}, {1026, 763}, {1018, 696},},
		{{1018, 696}, {1026, 763}, {1026, 696},},
		// s
		{{1082, 725}, {1079, 719}, {1076, 725},},
		{{1079, 719}, {1073, 721}, {1076, 725},},
		{{1079, 719}, {1073, 715}, {1073, 721},},
		{{1073, 715}, {1067, 719}, {1073, 721},},
		{{1073, 715}, {1064, 714}, {1067, 719},},
		{{1064, 714}, {1059, 719}, {1067, 719},},
		{{1064, 714}, {1056, 716}, {1059, 719},},
		{{1056, 716}, {1054, 721}, {1059, 719},},
		{{1056, 716}, {1049, 720}, {1054, 721},},
		{{1049, 720}, {1051, 723}, {1054, 721},},
		{{1049, 720}, {1045, 724}, {1051, 723},},
		{{1045, 724}, {1051, 729}, {1051, 723},},
		{{1045, 724}, {1045, 730}, {1051, 729},},
		{{1045, 730}, {1049, 735}, {1051, 729},},
		{{1049, 735}, {1058, 733}, {1051, 729},},
		{{1049, 735}, {1058, 739}, {1058, 733},},
		{{1058, 739}, {1070, 736}, {1058, 733},},
		{{1058, 739}, {1069, 742}, {1070, 736},},
		{{1069, 742}, {1080, 740}, {1070, 736},},
		{{1069, 742}, {1076, 746}, {1080, 740},},
		{{1076, 746}, {1083, 746}, {1080, 740},},
		{{1076, 746}, {1078, 752}, {1083, 746},},
		{{1078, 752}, {1083, 754}, {1083, 746},},
		{{1078, 752}, {1074, 757}, {1083, 754},},
		{{1074, 757}, {1077, 761}, {1083, 754},},
		{{1074, 757}, {1068, 759}, {1077, 761},},
		{{1068, 759}, {1068, 763}, {1077, 761},},
		{{1068, 759}, {1061, 759}, {1068, 763},},
		{{1061, 759}, {1056, 763}, {1068, 763},},
		{{1056, 763}, {1055, 757}, {1068, 763},},
		{{1056, 763}, {1049, 758}, {1055, 757},},
		{{1049, 758}, {1052, 754}, {1055, 757},},
		{{1049, 758}, {1045, 754}, {1052, 754},},
		{{1045, 754}, {1051, 749}, {1052, 754},},
		{{1045, 754}, {1045, 749}, {1051, 749},},
		// i
		{{1101, 763}, {1108, 763}, {1101, 715},},
		{{1101, 715}, {1108, 763}, {1108, 715},},
		{{1101, 695}, {1101, 704}, {1108, 704},},
		{{1101, 695}, {1108, 704}, {1108, 695},},
		// n
		{{1135, 716}, {1130, 716}, {1130, 763},},
		{{1135, 716}, {1130, 763}, {1135, 763},},
		{{1135, 733}, {1141, 717}, {1135, 724},},
		{{1141, 717}, {1139, 723}, {1145, 720},},
		{{1141, 717}, {1145, 720}, {1149, 716},},
		{{1149, 716}, {1145, 720}, {1152, 719},},
		{{1149, 716}, {1152, 719}, {1156, 715},},
		{{1156, 715}, {1152, 719}, {1159, 722},},
		{{1156, 715}, {1159, 722}, {1165, 719},},
		{{1165, 719}, {1159, 722}, {1162, 725},},
		{{1165, 719}, {1162, 725}, {1168, 725},},
		{{1168, 725}, {1162, 725}, {1162, 763},},
		{{1168, 725}, {1162, 763}, {1168, 763},},
		// g
		{{1222, 715}, {1229, 768}, {1229, 715},},
		{{1222, 770}, {1229, 768}, {1222, 715},},
		{{1222, 770}, {1222, 777}, {1229, 768},},
		{{1222, 770}, {1214, 778}, {1222, 777},},
		{{1214, 778}, {1213, 782}, {1222, 777},},
		{{1214, 778}, {1208, 779}, {1213, 782},},
		{{1208, 779}, {1207, 784}, {1213, 782},},
		{{1208, 779}, {1202, 779}, {1207, 784},},
		{{1202, 779}, {1198, 781}, {1207, 784},},
		{{1202, 779}, {1197, 776}, {1198, 781},},
		{{1197, 776}, {1190, 776}, {1198, 781},},
		{{1197, 776}, {1194, 771}, {1190, 776},},
		{{1194, 771}, {1188, 771}, {1190, 776},},
		{{1222, 724}, {1217, 722}, {1222, 730},},
		{{1222, 724}, {1217, 717}, {1217, 722},},
		{{1217, 717}, {1212, 719}, {1217, 722},},
		{{1217, 717}, {1211, 715}, {1212, 719},},
		{{1211, 715}, {1208, 719}, {1212, 719},},
		{{1211, 715}, {1205, 715}, {1208, 719},},
		{{1205, 715}, {1202, 720}, {1208, 719},},
		{{1205, 715}, {1197, 717}, {1202, 720},},
		{{1197, 717}, {1197, 724}, {1202, 720},},
		{{1197, 717}, {1190, 723}, {1197, 724},},
		{{1190, 723}, {1194, 730}, {1197, 724},},
		{{1190, 723}, {1188, 730}, {1194, 730},},
		{{1188, 730}, {1194, 740}, {1194, 730},},
		{{1188, 730}, {1188, 740}, {1194, 740},},
		{{1222, 754}, {1217, 756}, {1222, 748},},
		{{1222, 754}, {1217, 761}, {1217, 756},},
		{{1217, 761}, {1212, 759}, {1217, 756},},
		{{1217, 761}, {1211, 763}, {1212, 759},},
		{{1211, 763}, {1208, 759}, {1212, 759},},
		{{1211, 763}, {1205, 763}, {1208, 759},},
		{{1205, 763}, {1202, 758}, {1208, 759},},
		{{1205, 763}, {1197, 761}, {1202, 758},},
		{{1197, 761}, {1197, 754}, {1202, 758},},
		{{1197, 761}, {1190, 755}, {1197, 754},},
		{{1190, 755}, {1194, 748}, {1197, 754},},
		{{1190, 755}, {1188, 748}, {1194, 748},},
		{{1188, 748}, {1194, 738}, {1194, 748},},
		{{1188, 748}, {1188, 738}, {1194, 738},},
	};

	// University
	int University[145][3][2] = {
		// U
		{{1274, 696}, {1274, 746}, {1281, 696},},
		{{1274, 746}, {1281, 746}, {1281, 696},},
		{{1274, 746}, {1277, 753}, {1281, 746},},
		{{1277, 753}, {1286, 754}, {1281, 746},},
		{{1277, 753}, {1284, 760}, {1286, 754},},
		{{1284, 760}, {1290, 757}, {1286, 754},},
		{{1284, 760}, {1290, 763}, {1290, 757},},
		{{1290, 763}, {1301, 757}, {1290, 757},},
		{{1290, 763}, {1301, 763}, {1301, 757},},
		{{1328, 696}, {1328, 746}, {1321, 696},},
		{{1328, 746}, {1321, 746}, {1321, 696},},
		{{1328, 746}, {1325, 753}, {1321, 746},},
		{{1325, 753}, {1316, 754}, {1321, 746},},
		{{1325, 753}, {1318, 760}, {1316, 754},},
		{{1318, 760}, {1312, 757}, {1316, 754},},
		{{1318, 760}, {1312, 763}, {1312, 757},},
		{{1312, 763}, {1301, 757}, {1312, 757},},
		{{1312, 763}, {1301, 763}, {1301, 757},},
		// n
		{{1354, 716}, {1349, 716}, {1349, 763},},
		{{1354, 716}, {1349, 763}, {1354, 763},},
		{{1354, 733}, {1360, 717}, {1354, 724},},
		{{1360, 717}, {1358, 723}, {1364, 720},},
		{{1360, 717}, {1364, 720}, {1368, 716},},
		{{1368, 716}, {1364, 720}, {1371, 719},},
		{{1368, 716}, {1371, 719}, {1375, 715},},
		{{1375, 715}, {1371, 719}, {1378, 722},},
		{{1375, 715}, {1378, 722}, {1384, 719},},
		{{1384, 719}, {1378, 722}, {1381, 725},},
		{{1384, 719}, {1381, 725}, {1387, 725},},
		{{1387, 725}, {1381, 725}, {1381, 763},},
		{{1387, 725}, {1381, 763}, {1387, 763},},
		// i
		{{1410, 763}, {1417, 763}, {1410, 715},},
		{{1410, 715}, {1417, 763}, {1417, 715},},
		{{1410, 695}, {1410, 704}, {1417, 704},},
		{{1410, 695}, {1417, 704}, {1417, 695},},
		// v
		{{1440, 715}, {1433, 715}, {1451, 763},},
		{{1440, 715}, {1451, 763}, {1458, 763},},
		{{1451, 763}, {1458, 763}, {1468, 715},},
		{{1468, 715}, {1458, 763}, {1475, 715},},
		// e
		{{1535, 749}, {1527, 749}, {1532, 756},},
		{{1532, 756}, {1527, 749}, {1525, 754},},
		{{1525, 754}, {1525, 760}, {1532, 756},},
		{{1525, 754}, {1520, 758}, {1525, 760},},
		{{1525, 760}, {1520, 758}, {1518, 763},},
		{{1520, 758}, {1515, 760}, {1518, 763},},
		{{1518, 763}, {1515, 760}, {1509, 763},},
		{{1515, 760}, {1506, 758}, {1509, 763},},
		{{1506, 758}, {1503, 761}, {1509, 763},},
		{{1506, 758}, {1502, 755}, {1503, 761},},
		{{1502, 755}, {1498, 758}, {1503, 761},},
		{{1502, 755}, {1500, 749}, {1498, 758},},
		{{1500, 749}, {1493, 750}, {1498, 758},},
		{{1500, 749}, {1498, 743}, {1493, 750},},
		{{1498, 743}, {1492, 745}, {1493, 750},},
		{{1498, 743}, {1498, 739}, {1492, 745},},
		{{1498, 739}, {1492, 740}, {1492, 745},},
		{{1498, 739}, {1499, 734}, {1492, 740},},
		{{1499, 734}, {1492, 733}, {1492, 740},},
		{{1499, 734}, {1501, 729}, {1492, 733},},
		{{1501, 729}, {1494, 727}, {1492, 733},},
		{{1501, 729}, {1503, 725}, {1494, 727},},
		{{1503, 725}, {1497, 723}, {1494, 727},},
		{{1503, 725}, {1502, 719}, {1497, 723},},
		{{1503, 725}, {1510, 721}, {1502, 719},},
		{{1510, 721}, {1508, 717}, {1502, 719},},
		{{1510, 721}, {1517, 717}, {1508, 717},},
		{{1510, 721}, {1518, 721}, {1517, 717},},
		{{1518, 721}, {1526, 719}, {1517, 717},},
		{{1518, 721}, {1525, 724}, {1526, 719},},
		{{1525, 724}, {1534, 726}, {1526, 719},},
		{{1525, 724}, {1528, 729}, {1534, 726},},
		{{1528, 729}, {1536, 734}, {1534, 726},},
		{{1528, 729}, {1530, 734}, {1536, 734},},
		{{1530, 734}, {1536, 739}, {1536, 734},},
		{{1530, 734}, {1529, 739}, {1536, 739},},
		{{1533, 739}, {1533, 734}, {1494, 734},},
		{{1533, 739}, {1494, 734}, {1494, 739},},
		// r
		{{1553, 763}, {1560, 763}, {1553, 715},},
		{{1553, 715}, {1560, 763}, {1560, 715},},
		{{1560, 730}, {1562, 723}, {1560, 723},},
		{{1562, 723}, {1564, 717}, {1560, 723},},
		{{1562, 723}, {1567, 721}, {1564, 717},},
		{{1567, 721}, {1569, 715}, {1564, 717},},
		{{1567, 721}, {1572, 720}, {1569, 715},},
		{{1572, 720}, {1576, 716}, {1569, 715},},
		{{1572, 720}, {1576, 721}, {1576, 716},},
		// s
		{{1627, 725}, {1624, 719}, {1621, 725},},
		{{1624, 719}, {1618, 721}, {1621, 725},},
		{{1624, 719}, {1618, 715}, {1618, 721},},
		{{1618, 715}, {1612, 719}, {1618, 721},},
		{{1618, 715}, {1609, 714}, {1612, 719},},
		{{1609, 714}, {1604, 719}, {1612, 719},},
		{{1609, 714}, {1601, 716}, {1604, 719},},
		{{1601, 716}, {1599, 721}, {1604, 719},},
		{{1601, 716}, {1594, 720}, {1599, 721},},
		{{1594, 720}, {1596, 723}, {1599, 721},},
		{{1594, 720}, {1590, 724}, {1596, 723},},
		{{1590, 724}, {1596, 729}, {1596, 723},},
		{{1590, 724}, {1590, 730}, {1596, 729},},
		{{1590, 730}, {1594, 735}, {1596, 729},},
		{{1594, 735}, {1603, 733}, {1596, 729},},
		{{1594, 735}, {1603, 739}, {1603, 733},},
		{{1603, 739}, {1615, 736}, {1603, 733},},
		{{1603, 739}, {1614, 742}, {1615, 736},},
		{{1614, 742}, {1625, 740}, {1615, 736},},
		{{1614, 742}, {1621, 746}, {1625, 740},},
		{{1621, 746}, {1628, 746}, {1625, 740},},
		{{1621, 746}, {1623, 752}, {1628, 746},},
		{{1623, 752}, {1628, 754}, {1628, 746},},
		{{1623, 752}, {1619, 757}, {1628, 754},},
		{{1619, 757}, {1622, 761}, {1628, 754},},
		{{1619, 757}, {1613, 759}, {1622, 761},},
		{{1613, 759}, {1613, 763}, {1622, 761},},
		{{1613, 759}, {1606, 759}, {1613, 763},},
		{{1606, 759}, {1601, 763}, {1613, 763},},
		{{1601, 763}, {1600, 757}, {1613, 763},},
		{{1601, 763}, {1594, 758}, {1600, 757},},
		{{1594, 758}, {1597, 754}, {1600, 757},},
		{{1594, 758}, {1590, 754}, {1597, 754},},
		{{1590, 754}, {1596, 749}, {1597, 754},},
		{{1590, 754}, {1590, 749}, {1596, 749},},
		// i
		{{1646, 763}, {1653, 763}, {1646, 715},},
		{{1646, 715}, {1653, 763}, {1653, 715},},
		{{1646, 695}, {1646, 704}, {1653, 704},},
		{{1646, 695}, {1653, 704}, {1653, 695},},
		// t
		{{1668, 716}, {1668, 720}, {1690, 720},},
		{{1668, 716}, {1690, 720}, {1690, 716},},
		{{1680, 699}, {1674, 703}, {1674, 757},},
		{{1680, 699}, {1674, 757}, {1680, 757},},
		{{1680, 757}, {1674, 757}, {1677, 761},},
		{{1680, 757}, {1677, 761}, {1679, 763},},
		{{1680, 757}, {1679, 763}, {1687, 763},},
		{{1680, 757}, {1687, 763}, {1690, 758},},
		{{1687, 763}, {1690, 758}, {1691, 763},},
		// y
		{ {1710, 715}, {1703, 715}, {1721, 763},},
		{ {1710, 715}, {1721, 763}, {1728, 763},},
		{ {1721, 763}, {1728, 763}, {1738, 715},},
		{ {1738, 715}, {1728, 763}, {1745, 715},},
		{ {1728, 763}, {1721, 763}, {1726, 772},},
		{ {1721, 763}, {1720, 771}, {1726, 772},},
		{ {1720, 771}, {1721, 777}, {1726, 772},},
		{ {1720, 771}, {1713, 775}, {1721, 777},},
		{ {1713, 775}, {1714, 783}, {1721, 777},},
		{ {1713, 775}, {1706, 777}, {1714, 783},},
		{ {1706, 777}, {1706, 783}, {1714, 783},},
	};

	void triangles_xyConverse(int(*data)[3][2], int size) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3; j++) {
				data[i][j][0] = (data[i][j][0] - origin[0]);
				data[i][j][1] = (origin[1] - data[i][j][1]);
			}
		}
	}

	void presetData() {
		Triangles_xyConverse(National);
		Triangles_xyConverse(Chung);
		Triangles_xyConverse(Hsing);
		Triangles_xyConverse(University);
	}

public:

	NchuEn(int width, int height) {
		origin[0] = width / 2;
		origin[1] = height / 2;
		presetData();
	}

	void display_en() {
		glColor3fv(en_color);
		DrawTriangles(National);
		DrawTriangles(Chung);
		DrawTriangles(Hsing);
		DrawTriangles(University);
	}

} word_en(WIDTH, HEIGHT);

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(spin, spinX, spinY, spinZ);
	background();
	word_ch.display_ch();
	word_ch.display_logo();
	word_en.display_en();
	glPopMatrix();

	glutSwapBuffers();
}
