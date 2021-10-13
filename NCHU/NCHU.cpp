#include <GL/glut.h>
#define DrawPolygon(X) drawPolygon(X, sizeof(X) / sizeof(X[0]))
#define WIDTH 1920.0
#define HEIGHT 1080.0

void background();
void Display(void);

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);	//set the position of Window
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
	float origin[2];
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

	//ch_1
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

	//ch_2
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

	//ch_3
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

	//ch_4
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
		//DrawPolygon(ch_4_39);
		//DrawPolygon(ch_4_40);
	}

public:
	NchuCh(float width, float height) {
		origin[0] = width / 2;
		origin[1] = height / 2;
	}
	void display_ch() {
		glColor3f(0.007, 0.4375, 0.589);
		display_ch_1();
		display_ch_2();
		display_ch_3();
		display_ch_4();
	}
};

void Reshape() {
	glViewport(0, 0, WIDTH, HEIGHT);
	glOrtho(0.0f, WIDTH, 0.0f, HEIGHT, 0.0f, 0.0f);
}

void Display(void) {
	NchuCh word_ch(WIDTH, HEIGHT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WIDTH, HEIGHT);
	glOrtho(0.0f, WIDTH, 0.0f, HEIGHT, 0.0f, 0.0f);
	background();
	word_ch.display_ch();
	glutSwapBuffers();
}
