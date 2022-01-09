﻿#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <ctime>
#include <comutil.h>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>
#include <windows.h>
#include <mmsystem.h>
#include <irrKlang.h>

#include "talk.h"


#pragma warning(disable : 4996) //close warning of 4996

#define WIDTH   960
#define HEIGHT  540

// clock (float)
#define CLOCK_MULTIPLY 1.0

// start up the engine
irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

typedef struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char* data;
} Image;


int ImageLoad(char* filename, Image* image, int character, int negative) {
    FILE* file;
    unsigned long size; // size of the image in bytes.
    unsigned long i, j; // standard counter.
    unsigned short int planes; // number of planes in image (must be 1)
    unsigned short int bpp; // number of bits per pixel (must be 24)
    char temp; // temporary color storage for bgr-rgb conversion.
    // make sure the file is there.
    if ((file = fopen(filename, "rb")) == NULL) {
        printf("File Not Found : %s\n", filename);
        return 0;
    }
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);
    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
        printf("Error reading width from %s.\n", filename);
        return 0;
    }
    //printf("Width of %s: %lu\n", filename, image->sizeX);
    // read the height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }
    //printf("Height of %s: %lu\n", filename, image->sizeY);
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;
    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
        printf("Error reading planes from %s.\n", filename);
        return 0;
    }
    if (planes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, planes);
        return 0;
    }
    // read the bitsperpixel
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
        printf("Error reading bpp from %s.\n", filename);
        return 0;
    }
    if (bpp != 24) {
        printf("Bpp from %s is not 24: %u\n", filename, bpp);
        return 0;
    }
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);
    // read the data.
    image->data = (char*)malloc(size);
    if (image->data == NULL) {
        printf("Error allocating memory for color-corrected image data");
        return 0;
    }
    if ((i = fread(image->data, size, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return 0;
    }
    for (i = 0; i < size; i += 3) { // reverse all of the colors. (bgr -> rgb)
        temp = image->data[i];
        image->data[i] = image->data[i + 2];
        image->data[i + 2] = temp;
    }
    
    

    // we're done.
    char* tempData = (char*)malloc(((size)*4/3) * sizeof(char));
    for (i = 0, j = 0; i < size; i += 3, j+= 4) {
        if (negative == 0) {
            tempData[j] = image->data[i];
            tempData[j + 1] = image->data[i + 1];
            tempData[j + 2] = image->data[i + 2];
        }
        else {
            tempData[j] = 255 - image->data[i];
            tempData[j + 1] = 255 - image->data[i + 1];
            tempData[j + 2] = 255 - image->data[i + 2];
        }
        if (tempData[j] == 0 && tempData[j+1] == 0 && tempData[j+2] == 0 && character == 1) {
            tempData[j + 3] = 0x00;
            tempData[j] = 255;
            tempData[j + 1] = 255;
            tempData[j + 2] = 255;
        }
        else {
            tempData[j + 3] = 0xff;
        }
    }
    free(image->data);
    image->data = tempData;

    return 1;
}

Image* loadTexture(char* fileName, int character, int negative) {
    Image* image1;
    // allocate space for texture
    image1 = (Image*)malloc(sizeof(Image));
    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad(fileName, image1, character, negative)) {
        system("PAUSE");
        exit(1);
    }
    return image1;
}


//show string
#ifdef _DEBUG
#pragma comment(lib, "comsuppwd.lib")
#else
#pragma comment(lib, "comsuppw.lib")
#endif

COLORREF m_FontColor = RGB(0, 0, 0);
void DrawString(float x, float y, float z, const char* strText, int length) {
    HDC hDC = wglGetCurrentDC();
    int xx;
    float r, g, b;

    xx = m_FontColor;
    r = GetRValue(m_FontColor) / 255.0f;
    g = GetGValue(m_FontColor) / 255.0f;
    b = GetBValue(m_FontColor) / 255.0f;
    glColor3f(r, g, b);
    glRasterPos3f(x, y, z);
    int ListNum;
    DWORD dwChar;
    unsigned char* text = (unsigned char*)strText;
    glPushAttrib(GL_LIST_BIT);  // Pushes The Display List Bits  

    HFONT hOldFont, hFont;
    hFont = ::CreateFont(glutGet(GLUT_WINDOW_HEIGHT)/15, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Segoe Print"));
    hOldFont = (HFONT)::SelectObject(hDC, hFont);

    for (size_t i = 0; i < length; i++) {
        if (IsDBCSLeadByte(text[i])) {
            char tmpchar[3];
            tmpchar[0] = text[i];
            tmpchar[1] = text[i + 1];
            tmpchar[2] = 0;
            BSTR xx = _com_util::ConvertStringToBSTR(tmpchar);
            BYTE tmpchr[2];
            ::CopyMemory(tmpchr, xx, 2);
            dwChar = tmpchr[0] + tmpchr[1] * 256;
            i++;
        }
        else {
            dwChar = text[i];
        }
        ListNum = glGenLists(1);
        BOOL ret = FALSE;
        ret = wglUseFontBitmapsW(hDC, dwChar, 1, ListNum);
        glCallList(ListNum);
        glDeleteLists(ListNum, 1);
    }
    ::SelectObject(hDC, hOldFont);
    ::DeleteObject(hFont);
    glPopAttrib();
}


/* ame */
#define STAND_IMG_NUM 3
#define CLOCK_PER_STAND_IMG 3
GLuint ame_stand_texture[STAND_IMG_NUM];
int ame_stand_state = 0;
#define RUN_IMG_NUM 7
#define CLOCK_PER_RUN_IMG 3
GLuint ame_run_texture[RUN_IMG_NUM];
int ame_run_state = 0;
#define JUMP_IMG_NUM 23
#define CLOCK_PER_JUMP_IMG 3
GLuint ame_jump_texture;
int ame_jump_state = 0;
int ame_jump_flag = 0;
float ame_jump_height[29] = {
    0.25, 0.5, 0.8, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.5, -0.1, -0.1, -0.1, 0.0,
    0.05, 0.05, 0.05, 0.05, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
};
enum ame_state {
    run,
    jump,
    stand,
} ame_state;
/* unrivaled ame */
#define BASIC_AME_GUARD 80
int ame_guard = 0;
void ame_guard_idleFunc() {
    if (ame_guard > 0) {
        ame_guard--;
    }
}

/* gura */
#define GURA_IMG_NUM 3
#define CLOCK_PER_GURA_IMG 2
GLuint gura_texture[GURA_IMG_NUM];
int gura_state = 0;
float gura_translate = 3.0;
int gura_vector = 0;
float gura_translate_unit = 0.045 / CLOCK_MULTIPLY;

/* kiara */
#define KIARA_IMG_NUM 3
#define CLOCK_PER_KIARA_IMG 2
GLuint kiara_texture[KIARA_IMG_NUM];
int kiara_state = 0;
float kiara_translate = 5.0;
float kiara_translate_unit = 0.05 / CLOCK_MULTIPLY;

/* background */
#define BG_NUM 5
#define AME_BG_NUM 7
GLuint bg_texture[BG_NUM][2];
GLuint bg_ame[AME_BG_NUM];
int bg_state = 0;
float bg_translate = 0.0;
float bg_translate_unit = 0.03 / CLOCK_MULTIPLY;

/* object */
// diamond
#define DIAMOND_FULL 16
#define DIAMOND_UP -1.2f
#define DIAMOND_DOWN -2.6f
GLuint diamond;
float diamond_bucket[DIAMOND_FULL][2];
float diamond_step = 0.023 / CLOCK_MULTIPLY;
int float_diamond_state = 0;
float float_diamond = 0.0;
void diamond_init() {
    for (int i = 0; i < DIAMOND_FULL; i++) {
        diamond_bucket[i][0] = 0;
        diamond_bucket[i][1] = 0;
    }
}
// bubba
GLuint bubba;

/* start */
GLuint ame_head;
GLuint ame_talk;
GLuint sm_texture[4];
int sm_state = 0;
int start_mark_appear = 1;

/* control */
int start_flag = 0;
int the_world = 0;

/* the world */
#define THE_WORLD_BASIC_COUNT 220
#define THE_WORLD_BASIC_COUNT_RE 110
int the_world_voice_count = 0;
void the_world_idleFunc() {
    if (the_world_voice_count > 0) {
        if (rand() % 20 == 0) {
            int back = rand() % 100 - 50;
            bg_translate += (back * bg_translate_unit);
            for (int i = 0; i < DIAMOND_FULL; i++) {
                if (diamond_bucket[i][1] != 0) {
                    diamond_bucket[i][0] += (back * diamond_step);
                }
            }
        }
        the_world_voice_count--;
        if (the_world_voice_count < THE_WORLD_BASIC_COUNT * CLOCK_MULTIPLY / 2) {
            gura_translate_unit *= 0.96;
            kiara_translate_unit *= 0.96;
        }
    }
}

void out_of_the_world_idleFunc() {
    if (the_world_voice_count > 0) {
        the_world_voice_count--;
        gura_translate_unit *= 1.042;
        kiara_translate_unit *= 1.042;
        if (the_world_voice_count == 0) {
            gura_translate_unit = 0.05 / CLOCK_MULTIPLY;
            kiara_translate_unit = 0.055 / CLOCK_MULTIPLY;
        }
    }
}

void init(void)
{
    char strBuff[50];
    
    /* ame head */
    strcpy(strBuff, "ame/ame_head.bmp");
    Image* image1 = loadTexture((char*)strBuff, 0, 0);
    if (image1 == NULL) {
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }
    // Create Texture
    glGenTextures(1, &ame_head);
    glBindTexture(GL_TEXTURE_2D, ame_head);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
    free(image1);
    /* ame talk */
    strcpy(strBuff, "ame/ame_talk.bmp");
    image1 = loadTexture((char*)strBuff, 0, 0);
    if (image1 == NULL) {
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }
    // Create Texture
    glGenTextures(1, &ame_talk);
    glBindTexture(GL_TEXTURE_2D, ame_talk);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
    free(image1);


    /* ame stand */
    strcpy(strBuff, "ame/ame-stand-0.bmp");
    for (int i = 0; i < 3; i++) {
        strBuff[14] = i + '0';
        Image* image1 = loadTexture((char*)strBuff, 1, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, ame_stand_texture + i);
        glBindTexture(GL_TEXTURE_2D, ame_stand_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 2, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }
    ame_state = stand;

    /* ame run */
    strcpy(strBuff, "ame/ame-run-0.bmp");
    for (int i = 0; i < 7; i++) {
        strBuff[12] = i+'0';
        Image* image1 = loadTexture((char*)strBuff, 1, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, ame_run_texture+i);
        glBindTexture(GL_TEXTURE_2D, ame_run_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 2, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }

    /* ame jump */
    image1 = loadTexture((char*)"ame/ame-jump.bmp", 1, 0);
    if (image1 == NULL) {
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }
    // Create Texture
    glGenTextures(1, &ame_jump_texture);
    glBindTexture(GL_TEXTURE_2D, ame_jump_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 2, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
    free(image1);

    /* gura */
    strcpy(strBuff, "gura/gura-0.bmp");
    for (int i = 0; i < GURA_IMG_NUM; i++) {
        strBuff[10] = i + '0';
        Image* image1 = loadTexture((char*)strBuff, 1, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, gura_texture + i);
        glBindTexture(GL_TEXTURE_2D, gura_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 2, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }

    /* kiara */
    strcpy(strBuff, "kiara/kiara-0.bmp");
    for (int i = 0; i < KIARA_IMG_NUM; i++) {
        strBuff[12] = i + '0';
        Image* image1 = loadTexture((char*)strBuff, 1, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, kiara_texture + i);
        glBindTexture(GL_TEXTURE_2D, kiara_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 2, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }
    
    /* Background */
    strcpy(strBuff, "Background/BG_00.bmp");
    for (int i = 0; i < BG_NUM; i++) {
        for (int j = 0; j < 2; j++) {
            strBuff[15] = i + '0';
            Image* image1 = loadTexture((char*)strBuff, 0, j);
            if (image1 == NULL) {
                printf("Image was not returned from loadTexture\n");
                exit(0);
            }
            // Create Texture
            glGenTextures(1, *(bg_texture + i) + j);
            glBindTexture(GL_TEXTURE_2D, bg_texture[i][j]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
            glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
            free(image1);
        }
    }
    strcpy(strBuff, "Background/BG_ame_00.bmp");
    for (int i = 0; i < AME_BG_NUM; i++) {
        strBuff[19] = i + '0';
        Image* image1 = loadTexture((char*)strBuff, 0, 1);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, bg_ame+i);
        glBindTexture(GL_TEXTURE_2D, bg_ame[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }

    /* start mark */
    strcpy(strBuff, "Background/start_mark_0.bmp");
    for (int i = 0; i < 4; i++) {
        strBuff[22] = i + '0';
        Image* image1 = loadTexture((char*)strBuff, 0, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, sm_texture + i);
        glBindTexture(GL_TEXTURE_2D, sm_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }
    
    /* Diamond */
    image1 = loadTexture((char*)"object/Diamond.bmp", 1, 0);
    if (image1 == NULL) {
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }
    // Create Texture
    glGenTextures(1, &diamond);
    glBindTexture(GL_TEXTURE_2D, diamond);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 2, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
    free(image1);

    /* Bubba */
    image1 = loadTexture((char*)"object/bubba.bmp", 1, 0);
    if (image1 == NULL) {
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }
    // Create Texture
    glGenTextures(1, &bubba);
    glBindTexture(GL_TEXTURE_2D, bubba);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 2, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
    free(image1);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ALPHA);
    glAlphaFunc(GL_GREATER, .8f);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_FLAT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.5, 0.5, 0.5, 0.5);
    srand(time(NULL));

    PlaySound(L"ame/Amelia_Watsons_BGM_OP.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
    diamond_init();
}

int talk_state = 0;
int voice_state[6] = { 0, 0, 0, 0, 0, 0 };
int words_length_state[6] = { 1, 1, 1, 1, 1, 1 };
int word_clock = (int)(4 * CLOCK_MULTIPLY);
void talk_display() {
    /* ame head */
    glPushMatrix();
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glTranslated(-6.0, 3.0, 0);
    glBindTexture(GL_TEXTURE_2D, ame_head);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.13, -1.255, -0.9);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.13, -1.255, -0.9);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.13, 1.255, -0.9);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.13, 1.255, -0.9);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, ame_talk);
    glTranslated(1.255, 0, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(0, -1.255, -0.9);
    glTexCoord2f(1.0, 0.0); glVertex3f(12, -1.255, -0.9);
    glTexCoord2f(1.0, 1.0); glVertex3f(12, 1.255, -0.9);
    glTexCoord2f(0.0, 1.0); glVertex3f(0, 1.255, -0.9);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    if (talk_state < 6) {
        switch (talk_state) {
        case 0:
            if (Talk.talk_clock_0[words_length_state[0] / word_clock] < strlen(Talk.talk_0[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_0[0], Talk.talk_clock_0[words_length_state[0] / word_clock]);
                words_length_state[0]++;
            }
            else if (Talk.talk_clock_0[words_length_state[0] / word_clock] < strlen(Talk.talk_0[0]) + strlen(Talk.talk_0[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_0[0], strlen(Talk.talk_0[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_0[1], Talk.talk_clock_0[words_length_state[0] / word_clock] - strlen(Talk.talk_0[0]));
                words_length_state[0]++;
            }
            else if (Talk.talk_clock_0[words_length_state[0] / word_clock] < strlen(Talk.talk_0[0]) + strlen(Talk.talk_0[1]) + strlen(Talk.talk_0[2])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_0[0], strlen(Talk.talk_0[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_0[1], strlen(Talk.talk_0[1]));
                DrawString(-3.0, 1.60, 0.2, Talk.talk_0[2], Talk.talk_clock_0[words_length_state[0] / word_clock] - strlen(Talk.talk_0[0]) - strlen(Talk.talk_0[1]));
                words_length_state[0]++;
            }
            else {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_0[0], strlen(Talk.talk_0[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_0[1], strlen(Talk.talk_0[1]));
                DrawString(-3.0, 1.60, 0.2, Talk.talk_0[2], strlen(Talk.talk_0[2]));
                glTranslated(4.4, 1.5, 0.2);
                glBegin(GL_TRIANGLES);
                    glVertex3f(-0.15f, 0.0f, 0.2f);
                    glVertex3f(0.15f, 0.0f, 0.2f);
                    glVertex3f(0.0f, -0.10f, 0.2f);
                glEnd();
            }
            if (voice_state[0] == 0) {
                PlaySound(L"ame/voice/voice_0.wav", NULL, SND_ASYNC | SND_FILENAME);
                voice_state[0] = 1;
            }
            break;
        case 1:
            if (Talk.talk_clock_1[words_length_state[1] / word_clock] < strlen(Talk.talk_1[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_1[0], Talk.talk_clock_1[words_length_state[1] / word_clock]);
                words_length_state[1]++;
            }
            else if (Talk.talk_clock_1[words_length_state[1] / word_clock] < strlen(Talk.talk_1[0]) + strlen(Talk.talk_1[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_1[0], strlen(Talk.talk_1[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_1[1], Talk.talk_clock_1[words_length_state[1] / word_clock] - strlen(Talk.talk_1[0]));
                words_length_state[1]++;
            }
            else if (Talk.talk_clock_1[words_length_state[1] / word_clock] < strlen(Talk.talk_1[0]) + strlen(Talk.talk_1[1]) + strlen(Talk.talk_1[2])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_1[0], strlen(Talk.talk_1[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_1[1], strlen(Talk.talk_1[1]));
                DrawString(-3.0, 1.60, 0.2, Talk.talk_1[2], Talk.talk_clock_1[words_length_state[1] / word_clock] - strlen(Talk.talk_1[0]) - strlen(Talk.talk_1[1]));
                words_length_state[1]++;
            }
            else {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_1[0], strlen(Talk.talk_1[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_1[1], strlen(Talk.talk_1[1]));
                DrawString(-3.0, 1.60, 0.2, Talk.talk_1[2], strlen(Talk.talk_1[2]));
                glTranslated(4.4, 1.5, 0.2);
                glBegin(GL_TRIANGLES);
                    glVertex3f(-0.15f, 0.0f, 0.2f);
                    glVertex3f(0.15f, 0.0f, 0.2f);
                    glVertex3f(0.0f, -0.10f, 0.2f);
                glEnd();
            }
            if (voice_state[1] == 0) {
                PlaySound(L"ame/voice/voice_1.wav", NULL, SND_ASYNC | SND_FILENAME);
                voice_state[1] = 1;
            }
            break;
        case 2:
            if (Talk.talk_clock_2[words_length_state[2] / word_clock] < strlen(Talk.talk_2[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_2[0], Talk.talk_clock_2[words_length_state[2] / word_clock]);
                words_length_state[2]++;
            }
            else if (Talk.talk_clock_2[words_length_state[2] / word_clock] < strlen(Talk.talk_2[0]) + strlen(Talk.talk_2[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_2[0], strlen(Talk.talk_2[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_2[1], Talk.talk_clock_2[words_length_state[2] / word_clock] - strlen(Talk.talk_2[0]));
                words_length_state[2]++;
            }
            else {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_2[0], strlen(Talk.talk_2[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_2[1], strlen(Talk.talk_2[1]));
                glTranslated(4.4, 1.5, 0.2);
                glBegin(GL_TRIANGLES);
                    glVertex3f(-0.15f, 0.0f, 0.2f);
                    glVertex3f(0.15f, 0.0f, 0.2f);
                    glVertex3f(0.0f, -0.10f, 0.2f);
                glEnd();
            }
            if (voice_state[2] == 0) {
                PlaySound(L"ame/voice/voice_2.wav", NULL, SND_ASYNC | SND_FILENAME);
                voice_state[2] = 1;
            }
            break;
        case 3:
            if (Talk.talk_clock_3[words_length_state[3] / word_clock] < strlen(Talk.talk_3[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_3[0], Talk.talk_clock_3[words_length_state[3] / word_clock]);
                words_length_state[3]++;
            }
            else if (Talk.talk_clock_3[words_length_state[3] / word_clock] < strlen(Talk.talk_3[0]) + strlen(Talk.talk_3[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_3[0], strlen(Talk.talk_3[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_3[1], Talk.talk_clock_3[words_length_state[3] / word_clock] - strlen(Talk.talk_3[0]));
                words_length_state[3]++;
            }
            else {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_3[0], strlen(Talk.talk_3[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_3[1], strlen(Talk.talk_3[1]));
                glTranslated(4.4, 1.5, 0.2);
                glBegin(GL_TRIANGLES);
                    glVertex3f(-0.15f, 0.0f, 0.2f);
                    glVertex3f(0.15f, 0.0f, 0.2f);
                    glVertex3f(0.0f, -0.10f, 0.2f);
                glEnd();
            }
            if (voice_state[3] == 0) {
                PlaySound(L"ame/voice/voice_3.wav", NULL, SND_ASYNC | SND_FILENAME);
                voice_state[3] = 1;
            }
            break;
        case 4:
            if (Talk.talk_clock_4[words_length_state[4] / word_clock] < strlen(Talk.talk_4[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_4[0], Talk.talk_clock_4[words_length_state[4] / word_clock]);
                words_length_state[4]++;
            }
            else if (Talk.talk_clock_4[words_length_state[4] / word_clock] < strlen(Talk.talk_4[0]) + strlen(Talk.talk_4[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_4[0], strlen(Talk.talk_4[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_4[1], Talk.talk_clock_4[words_length_state[4] / word_clock] - strlen(Talk.talk_4[0]));
                words_length_state[4]++;
            }
            else if (Talk.talk_clock_4[words_length_state[4] / word_clock] < strlen(Talk.talk_4[0]) + strlen(Talk.talk_4[1]) + strlen(Talk.talk_4[2])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_4[0], strlen(Talk.talk_4[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_4[1], strlen(Talk.talk_4[1]));
                DrawString(-3.0, 1.60, 0.2, Talk.talk_4[2], Talk.talk_clock_4[words_length_state[4] / word_clock] - strlen(Talk.talk_4[0]) - strlen(Talk.talk_4[1]));
                words_length_state[4]++;
            }
            else {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_4[0], strlen(Talk.talk_4[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_4[1], strlen(Talk.talk_4[1]));
                DrawString(-3.0, 1.60, 0.2, Talk.talk_4[2], strlen(Talk.talk_4[2]));
                glTranslated(4.4, 1.5, 0.2);
                glBegin(GL_TRIANGLES);
                    glVertex3f(-0.15f, 0.0f, 0.2f);
                    glVertex3f(0.15f, 0.0f, 0.2f);
                    glVertex3f(0.0f, -0.10f, 0.2f);
                glEnd();
            }
            if (voice_state[4] == 0) {
                PlaySound(L"ame/voice/voice_4.wav", NULL, SND_ASYNC | SND_FILENAME);
                voice_state[4] = 1;
            }
            break;
        case 5:
            if (Talk.talk_clock_5[words_length_state[5] / word_clock] < strlen(Talk.talk_5[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_5[0], Talk.talk_clock_5[words_length_state[5] / word_clock]);
                words_length_state[5]++;
            }
            else if (Talk.talk_clock_5[words_length_state[5] / word_clock] < strlen(Talk.talk_5[0]) + strlen(Talk.talk_5[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_5[0], strlen(Talk.talk_5[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_5[1], Talk.talk_clock_5[words_length_state[5] / word_clock] - strlen(Talk.talk_5[0]));
                words_length_state[5]++;
            }
            else {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_5[0], strlen(Talk.talk_5[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_5[1], strlen(Talk.talk_5[1]));
                glTranslated(4.4, 1.5, 0.2);
                glBegin(GL_TRIANGLES);
                    glVertex3f(-0.15f, 0.0f, 0.2f);
                    glVertex3f(0.15f, 0.0f, 0.2f);
                    glVertex3f(0.0f, -0.10f, 0.2f);
                glEnd();
            }
            if (voice_state[5] == 0) {
                PlaySound(L"ame/voice/voice_5.wav", NULL, SND_ASYNC | SND_FILENAME);
                voice_state[5] = 1;
            }
            break;
        default:
            break;
        }
    }
    glPopMatrix();
}

/* score */
int score = 0;
void score_display() {
    static char score_buff[7] = "000000";
    glPushMatrix();
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glBindTexture(GL_TEXTURE_2D, ame_talk);
    glTranslated(7.2, 4.2, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -0.3, -0.99);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -0.3, -0.99);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 0.3, -0.99);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 0.3, -0.99);
    glEnd();
    sprintf(score_buff, "%06d", score);
    DrawString(-0.65, -0.1, -0.98, score_buff, strlen(score_buff));
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glPopMatrix();
}
/* blood */
#define BLOOD_MAX 3
int blood = 3;
void blood_display() {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, bubba);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glTranslated(-5.6f, 3.0f, 0);
    for (int i = 0; i < BLOOD_MAX; i++) {
        if (i >= blood) {
            glColor4f(0.f, 0.f, 0.f, 0.2f);
            glDisable(GL_ALPHA_TEST);
        }
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-0.48, -0.52, -0.00003);
        glTexCoord2f(1.0, 0.0); glVertex3f(0.48, -0.52, -0.00003);
        glTexCoord2f(1.0, 1.0); glVertex3f(0.48, 0.52, -0.00003);
        glTexCoord2f(0.0, 1.0); glVertex3f(-0.48, 0.52, -0.00003);
        glEnd();
        glTranslated(1.0f, 0, 0);
    }
    glEnable(GL_ALPHA_TEST);
    glPopMatrix();
}

/* diamond */
void diamond_display() {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, diamond);
    for (int i = 0; i < DIAMOND_FULL; i++) {
        if (diamond_bucket[i][1] != 0.0f) {
            glTranslated(diamond_bucket[i][0], diamond_bucket[i][1] + float_diamond, 0);
            if (the_world) {
                glColor4f(1.f, 0.f, 0.f, 1.f);
            }
            else {
                glColor4f(0.29f, 0.93f, 0.855f, 1.f);
            }
            glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex3f(-0.24, -0.26, -0.00003);
            glTexCoord2f(1.0, 0.0); glVertex3f(0.24, -0.26, -0.00003);
            glTexCoord2f(1.0, 1.0); glVertex3f(0.24, 0.26, -0.00003);
            glTexCoord2f(0.0, 1.0); glVertex3f(-0.24, 0.26, -0.00003);
            glEnd();
            glTranslated(0, -float_diamond, 0);
            glColor4f(0.f, 0.f, 0.f, 0.2f);
            glDisable(GL_ALPHA_TEST);
            if (diamond_bucket[i][1] == DIAMOND_DOWN) {
                glBegin(GL_QUADS);
                glTexCoord2f(0.0, 1.0); glVertex3f(-0.34 + float_diamond * 0.75, -0.785, -0.00003);
                glTexCoord2f(1.0, 1.0); glVertex3f(0.14 - float_diamond * 0.75, -0.785, -0.00003);
                glTexCoord2f(1.0, 0.0); glVertex3f(0.24 - float_diamond * 0.75, -0.665, -0.00003);
                glTexCoord2f(0.0, 0.0); glVertex3f(-0.24 + float_diamond * 0.75, -0.665, -0.00003);
                glEnd();
            }
            else {
                glBegin(GL_QUADS);
                glTexCoord2f(0.0, 1.0); glVertex3f(-0.34 + float_diamond * 0.5, -2.385, -0.00003);
                glTexCoord2f(1.0, 1.0); glVertex3f(0.14 - float_diamond * 0.5, -2.385, -0.00003);
                glTexCoord2f(1.0, 0.0); glVertex3f(0.24 - float_diamond * 0.5, -2.265, -0.00003);
                glTexCoord2f(0.0, 0.0); glVertex3f(-0.24 + float_diamond * 0.5, -2.265, -0.00003);
                glEnd();
            }
            glEnable(GL_ALPHA_TEST);
            glTranslated(-diamond_bucket[i][0], -diamond_bucket[i][1], 0);
        }
    }
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glPopMatrix();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 3, 0, 0, 2, 0.0, 1.0, 0.0);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    
    glPushMatrix();
    glTranslated(bg_translate, 0, 0);
    /* start mark */
    if (start_flag == 0 || (start_mark_appear == 1 && (start_flag == 1 || start_flag == 2))) {
        if (bg_translate <= -12) {
            start_mark_appear = 0;
        }

        glBindTexture(GL_TEXTURE_2D, sm_texture[sm_state]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-1.25, -0.75, -0.999);
        glTexCoord2f(1.0, 0.0); glVertex3f(1.25, -0.75, -0.999);
        glTexCoord2f(1.0, 1.0); glVertex3f(1.25, 0.75, -0.999);
        glTexCoord2f(0.0, 1.0); glVertex3f(-1.25, 0.75, -0.999);
        glEnd();
    }
    /* background */
    for (int i = bg_state, j = 0; j < 2; i = (i + 1) % BG_NUM, j++) {
        glBindTexture(GL_TEXTURE_2D, bg_texture[i][the_world]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-13, -5.0, -1.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(13, -5.0, -1.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(13, 5.0, -1.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(-13, 5.0, -1.0);
        glEnd();
        glTranslated(26.0, 0, 0);
    }
    glPopMatrix();
    /* the word ame */
    glPushMatrix();
    if (the_world && rand() % 17 == 0) {
        int ame_bg_index = rand() % AME_BG_NUM;
        glBindTexture(GL_TEXTURE_2D, bg_ame[ame_bg_index]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-9.6, -5.4, -0.999);
        glTexCoord2f(1.0, 0.0); glVertex3f(9.6, -5.4, -0.999);
        glTexCoord2f(1.0, 1.0); glVertex3f(9.6, 5.4, -0.999);
        glTexCoord2f(0.0, 1.0); glVertex3f(-9.6, 5.4, -0.999);
        glEnd();
    }
    glPopMatrix();

    /* diamond_diaplay */
    diamond_display();

    /* ame start talk */
    if (start_flag == 2) {
        talk_display();
    }
    
    if(start_flag == 1) {
        score_display();
        blood_display();
    }

    

    static float run_per_img = CLOCK_PER_RUN_IMG * CLOCK_MULTIPLY;
    static float jump_per_img = CLOCK_PER_JUMP_IMG * CLOCK_MULTIPLY;
    static float stand_per_img = CLOCK_PER_STAND_IMG * CLOCK_MULTIPLY;
    glPushMatrix();
    glTranslated(-4.0, -3.1, 0);
    if (ame_guard <= 0) {
        glColor4f(1.f, 1.f, 1.f, 1.f);
    }
    else {
        static int guard_color = 0;
        for (int i = 0; i < 40; i++) {
            glColor4f(1.f, 1.f, 1.f-40*0.01, 1.f);
        }
        if (guard_color = 0) {
            glColor4f(1.f, 1.f, 1.f, 1.f);
            guard_color = 1;
        }
        else if (guard_color == 1 || guard_color == 3) {
            glColor4f(1.f, 1.f, 0.8f, 1.f);
            if (guard_color == 1) {
                guard_color = 2;
            }
            else {
                guard_color = 0;
            }
        }
        else {
            glColor4f(1.f, 1.f, 0.7f, 1.f);
            guard_color = 3;
        }
    }
    if (ame_state == run) {
        glTranslated(0.13, 0, 0);
        glBindTexture(GL_TEXTURE_2D, ame_run_texture[(int)(ame_run_state / run_per_img)]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex2f(0.0, -0.1);
        glTexCoord2f(1.0, 0.0); glVertex2f(1.2, -0.1);
        glTexCoord2f(1.0, 1.0); glVertex2f(1.2, 1.3);
        glTexCoord2f(0.0, 1.0); glVertex2f(0.0, 1.3);
        glEnd();

        glDisable(GL_ALPHA_TEST);
        glBegin(GL_QUADS);
        glColor4f(0.f, 0.f, 0.f, 0.2f);
        glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
        glTexCoord2f(1.0, 0.0); glVertex2f(1.2, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex2f(0.6, -0.5);
        glTexCoord2f(0.0, 1.0); glVertex2f(-0.6, -0.5);
        glEnd();
        glEnable(GL_ALPHA_TEST);
    }
    else if (ame_state == jump) {
        float jumpTectureX = 0.03 + ((int)(ame_jump_state / jump_per_img) % 6) * 0.16666667, jumpTectureY = 1.0 - ((int)(ame_jump_state / jump_per_img) / 6) * 0.2;
        glPushMatrix();
        glTranslated(0.0, ame_jump_height[(int)(ame_jump_state / jump_per_img)], 0);
        glBindTexture(GL_TEXTURE_2D, ame_jump_texture);
        glBegin(GL_QUADS);
        glTexCoord2f(jumpTectureX, jumpTectureY - 0.2); glVertex2f(1.5, -0.1);
        glTexCoord2f(jumpTectureX + 0.16666667, jumpTectureY - 0.2); glVertex2f(0.0, -0.1);
        glTexCoord2f(jumpTectureX + 0.16666667, jumpTectureY); glVertex2f(0.0, 2);
        glTexCoord2f(jumpTectureX, jumpTectureY); glVertex2f(1.5, 2);
        glEnd();
        glPopMatrix();

        glDisable(GL_ALPHA_TEST);
        glBegin(GL_QUADS);
        glColor4f(0.f, 0.f, 0.f, 0.2f);
        glTexCoord2f(jumpTectureX, jumpTectureY - 0.2); glVertex2f(0.0, 0.0);
        glTexCoord2f(jumpTectureX + 0.16666667, jumpTectureY - 0.2); glVertex2f(1.5, 0.0);
        glTexCoord2f(jumpTectureX + 0.16666667, jumpTectureY); glVertex2f(0.75, -0.76);
        glTexCoord2f(jumpTectureX, jumpTectureY); glVertex2f(-0.75, -0.76);
        glEnd();
        glEnable(GL_ALPHA_TEST);
    }
    else if (ame_state == stand) {
        glTranslated(0.13, 0, 0);
        glBindTexture(GL_TEXTURE_2D, ame_stand_texture[(int)(ame_stand_state / stand_per_img)]);
        glBegin(GL_QUADS);
        glTexCoord2f(1.0, 0.0); glVertex2f(0.0, -0.1);
        glTexCoord2f(0.0, 0.0); glVertex2f(1.2, -0.1);
        glTexCoord2f(0.0, 1.0); glVertex2f(1.2, 1.3);
        glTexCoord2f(1.0, 1.0); glVertex2f(0.0, 1.3);
        glEnd();

        glDisable(GL_ALPHA_TEST);
        glBegin(GL_QUADS);
        glColor4f(0.f, 0.f, 0.f, 0.2f);
        glTexCoord2f(1.0, 0.0); glVertex2f(0.0, 0.0);
        glTexCoord2f(0.0, 0.0); glVertex2f(1.2, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex2f(0.6, -0.5);
        glTexCoord2f(1.0, 1.0); glVertex2f(-0.6, -0.5);
        glEnd();
        glEnable(GL_ALPHA_TEST);
    }
    glPopMatrix();
    
    static float gura_per_img = CLOCK_PER_GURA_IMG * CLOCK_MULTIPLY;
    glPushMatrix();
    glTranslated(gura_translate, -3.1, 0);
    if (!the_world) { glColor4f(1.f, 1.f, 1.f, 1.f); }
    else { glColor4f(0.5f, 0.5f, 0.5f, 1.f); }
    if (gura_vector == 0) {
        glBindTexture(GL_TEXTURE_2D, gura_texture[(int)(gura_state / gura_per_img)]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(0.0, -0.1, 0.00001);
        glTexCoord2f(1.0, 0.0); glVertex3f(1.2, -0.1, 0.00001);
        glTexCoord2f(1.0, 1.0); glVertex3f(1.2, 1.3, 0.00001);
        glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1.3, 0.00001);
        glEnd();

        glDisable(GL_ALPHA_TEST);
        glBegin(GL_QUADS);
        glColor4f(0.f, 0.f, 0.f, 0.2f);
        glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.00001);
        glTexCoord2f(1.0, 0.0); glVertex3f(1.2, 0.0, 0.00001);
        glTexCoord2f(1.0, 1.0); glVertex3f(0.6, -0.5, 0.00001);
        glTexCoord2f(0.0, 1.0); glVertex3f(-0.6, -0.5, 0.00001);
        glEnd();
        glEnable(GL_ALPHA_TEST);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, gura_texture[(int)(gura_state / (CLOCK_PER_GURA_IMG * CLOCK_MULTIPLY))]);
        glBegin(GL_QUADS);
        glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -0.1, 0.0);
        glTexCoord2f(0.0, 0.0); glVertex3f(1.2, -0.1, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(1.2, 1.3, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.3, 0.0);
        glEnd();

        glDisable(GL_ALPHA_TEST);
        glBegin(GL_QUADS);
        glColor4f(0.f, 0.f, 0.f, 0.2f);
        glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, 0.00001);
        glTexCoord2f(0.0, 0.0); glVertex3f(1.2, 0.0, 0.00001);
        glTexCoord2f(0.0, 1.0); glVertex3f(0.6, -0.5, 0.00001);
        glTexCoord2f(1.0, 1.0); glVertex3f(-0.6, -0.5, 0.00001);
        glEnd();
        glEnable(GL_ALPHA_TEST);
    }
    glPopMatrix();

    static float kiara_per_img = CLOCK_PER_KIARA_IMG * CLOCK_MULTIPLY;
    glPushMatrix();
    glTranslated(kiara_translate, -1.5, 0);
    if (!the_world) { glColor4f(1.f, 1.f, 1.f, 1.f); }
    else { glColor4f(0.5f, 0.5f, 0.5f, 1.f); }
    glBindTexture(GL_TEXTURE_2D, kiara_texture[(int)(kiara_state / kiara_per_img)]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, -0.1, 0.00001);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.2, -0.1, 0.00001);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.2, 1.3, 0.00001);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1.3, 0.00001);
    glEnd();

    glTranslated(0, -1.8, 0);
    glDisable(GL_ALPHA_TEST);
    glBegin(GL_QUADS);
    glColor4f(0.f, 0.f, 0.f, 0.2f);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.00002);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.2, 0.0, 0.00002);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.6, -0.5, 0.00002);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.6, -0.5, 0.00002);
    glEnd();
    glEnable(GL_ALPHA_TEST);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
    

    glutSwapBuffers();
}

void myReshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, (GLfloat)width / (GLfloat)height, 2.0, 1200.0);
    glMatrixMode(GL_MODELVIEW);
}

#define keyboardArraySize 500
int keyboardArray[keyboardArraySize];
void keyboardArrayInit() {
    for (int i = 0; i < keyboardArraySize; i++) {
        keyboardArray[i] = 0;
    }
}
int multi_jump_boundary_early = (int)(8 * CLOCK_PER_JUMP_IMG * CLOCK_MULTIPLY);
int multi_jump_boundary_last = (int)(12 * CLOCK_PER_JUMP_IMG * CLOCK_MULTIPLY);
void keyboard(unsigned char key, int x, int y) {
    if (start_flag == 1) {
        switch (key) {
        case ' ':
            if (ame_state == run) {
                if (the_world && rand() % 7 == 1) {
                    engine->play2D("ame/watson_amelia_get_the_fuck_up.wav");
                }
                else {
                    engine->play2D("ame/watson_amelia_hic.mp3");
                }
                if (-8.0 <= gura_translate && gura_translate <= -4.0 && gura_vector == 1 || -4.0 <= gura_translate && gura_translate <= 0.0 && gura_vector == 0) {
                    if (rand() % 43 == 1) {
                        engine->play2D("gura/Haachama.wav");
                    }
                    else {
                        engine->play2D("gura/gawr_a.wav");
                    }
                }
                ame_state = jump;
                keyboardArray[' '] = 1;
            }
            else if (ame_state == jump && multi_jump_boundary_early <= ame_jump_state && ame_jump_state <= multi_jump_boundary_last) {
                keyboardArray[' '] = 1;
            }
            break;
        case 'f':
        case 'F':
            if (start_mark_appear == 0) {
                if (!the_world) {
                    the_world_voice_count = THE_WORLD_BASIC_COUNT * CLOCK_MULTIPLY;
                    the_world = 1;
                    PlaySound(L"ame/Giornos_Theme_Il_vento_doro.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
                    engine->play2D("ame/za_warudo_stop_time_sound.mp3");
                }
                else if (the_world && the_world_voice_count == 0) {
                    the_world_voice_count = THE_WORLD_BASIC_COUNT_RE * CLOCK_MULTIPLY;
                    the_world = 0;
                    engine->play2D("ame/za_warudo_the_world_time_resume.mp3");
                    PlaySound(L"ame/Amelia_Watsons_BGM.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
                }
            }
            break;
        default: // pass
            break;
        }
    }
    else if(start_flag == 2) {
        switch (key) {
        case ' ':
            if (words_length_state[talk_state] / word_clock < Talk.talks_end[talk_state]) {
                words_length_state[talk_state] = Talk.talks_end[talk_state] * word_clock;
            }
            else {
                talk_state += 1;
                if (talk_state == 6) {
                    start_flag = 1;
                    ame_state = run;
                    PlaySound(L"ame/Amelia_Watsons_BGM.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
                }
            }
            break;
        default: // pass
            break;
        }
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    if (start_flag == 1) {
        switch (key) {
        case ' ':
            keyboardArray[' '] = 0;
            break;
        default: // pass
            break;
        }
    }
}

void keyboardIdle() {
    if (keyboardArray[' '] == 1 && ame_state == jump && multi_jump_boundary_early <= ame_jump_state && ame_jump_state <= multi_jump_boundary_last) {
        ame_jump_state = 4;
    }
}

/* mouse */
void mouse(int button, int state, int x, int y) {
    //obtain window size
    float WindowWidth = glutGet(GLUT_WINDOW_WIDTH);
    float WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    if (start_flag == 0) {
        if (WindowWidth / 2 - 85 <= x && x <= WindowWidth / 2 + 85 && WindowHeight / 2 - 65 <= y && y <= WindowHeight / 2 + 65 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            sm_state = 2;
        }
        if (WindowWidth / 2 - 85 <= x && x <= WindowWidth / 2 + 85 && WindowHeight / 2 - 65 <= y && y <= WindowHeight / 2 + 65 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            sm_state = 3;
            start_flag = 2;
        }
        else if (sm_state == 2 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            sm_state = 1;
        }
    }
}
void mouse_position_idleFunc() {
    static POINT pt;
    BOOL bReturn = GetCursorPos(&pt); //獲取滑鼠指標位置到pt

    if (bReturn != 0) { //如果函式執行成功
        if (WIDTH - 85 <= pt.x  && pt.x <= WIDTH + 85 && HEIGHT - 65 <= pt.y && pt.y <= HEIGHT + 65 && (sm_state == 0 || sm_state == 1)) {
            sm_state = 1;
        }
        else if(sm_state == 1) {
            sm_state = 0;
        }
    }
}


/* ame idle function */
void ame_stand_state_idleFunc() {
    static int ame_stand_state_boundary = (int)(STAND_IMG_NUM * CLOCK_PER_STAND_IMG * CLOCK_MULTIPLY) - 1;
    if (ame_stand_state == ame_stand_state_boundary) {
        ame_stand_state = 0;
    }
    else {
        ame_stand_state++;
    }
}

void ame_run_state_idleFunc() {
    static int ame_run_state_boundary = (int)(CLOCK_PER_RUN_IMG * RUN_IMG_NUM * CLOCK_MULTIPLY) - 1;
    if (ame_run_state == ame_run_state_boundary) {
        ame_run_state = 0;
    }
    else {
        ame_run_state++;
    }
}

void ame_jump_state_idleFunc() {
    static int ame_jump_state_boundary = (int)(JUMP_IMG_NUM * CLOCK_PER_JUMP_IMG * CLOCK_MULTIPLY);
    static int ame_jump_down = (int)(14 * CLOCK_PER_JUMP_IMG * CLOCK_MULTIPLY);
    if (ame_jump_state == ame_jump_state_boundary) {
        ame_jump_state = 0;
        ame_state = run;
    }
    else {
        ame_jump_state++;
        if (ame_jump_state == ame_jump_down) {
            engine->play2D("ame/ame_jump_down.wav");
        }
    }
}

/* kiara idle function */
void kiara_idleFunc() {
    static int kiara_state_boundary = ((int)(KIARA_IMG_NUM * CLOCK_PER_KIARA_IMG * CLOCK_MULTIPLY) - 1);
    if (kiara_state == kiara_state_boundary) {
        kiara_state = 0;
    }
    else {
        kiara_state++;
    }
}

void kiara_move_idleFunc() {
    if (kiara_translate < -7.5) {
        kiara_translate = 7.5 + rand() % 40;
    }

    if (-3.67f < kiara_translate && kiara_translate < -2.87f) {
        int ame_y_detect = ame_jump_state / CLOCK_PER_JUMP_IMG;
        if ((2 <= ame_y_detect && ame_y_detect <= 13) && ame_guard == 0) {
            blood -= 1;
            if (blood == 0) {
                ame_guard = -1;
            }
            else {
                ame_guard = BASIC_AME_GUARD * CLOCK_MULTIPLY;
            }
        }
    }

    kiara_translate -= kiara_translate_unit;
}

/* gura idle function */
void gura_state_idleFunc() {
    static int gura_state_boundary = ((int)(GURA_IMG_NUM * CLOCK_PER_GURA_IMG * CLOCK_MULTIPLY) - 1);
    if (gura_state == gura_state_boundary) {
        gura_state = 0;
    }
    else {
        gura_state++;
    }
}

void gura_move_idleFunc() {
    static int gura_vector_point = (int)(79 * CLOCK_MULTIPLY);
    if (!the_world && rand() % gura_vector_point == 1) {
        gura_vector = !gura_vector;
    }

    if (-3.62f < gura_translate && gura_translate < -2.92f) {
        int ame_y_detect = ame_jump_state / CLOCK_PER_JUMP_IMG;
        if ((ame_y_detect == 0 || ame_y_detect >= 13) && ame_guard == 0) {
            blood -= 1;
            if (blood == 0) {
                ame_guard = -1;
            }
            else {
                ame_guard = BASIC_AME_GUARD * CLOCK_MULTIPLY;
            }
        }
    }

    if (gura_vector == 0) {
        if (gura_translate < -7.5) {
            gura_vector = 1;
        }
        if (rand() % 5 != 0) {
            gura_translate -= gura_translate_unit;
        }
    }
    else {
        if (gura_translate > 7.5) {
            gura_vector = 0;
        }
        if (rand() % 5 != 0) {
            gura_translate += gura_translate_unit;
        }
    }
}


/* diamond idle function */
void diamond_move_idleFunc() {
    int ame_y_detect = ame_jump_state / CLOCK_PER_JUMP_IMG;
    for (int i = 0; i < DIAMOND_FULL; i++) {
        if (diamond_bucket[i][1] != 0.0f) {
            if (diamond_bucket[i][0] < -12.5) {
                diamond_bucket[i][1] = 0;
            }
            else {
                if (-3.77f <= diamond_bucket[i][0] && diamond_bucket[i][0] <= -2.77f) {
                    if (diamond_bucket[i][1] == DIAMOND_DOWN && (ame_y_detect == 0 || ame_y_detect >= 13)) {
                        if (the_world) {
                            score += 2;
                        }
                        else {
                            score += 7;
                        }
                        engine->play2D("object/coin.wav");
                        diamond_bucket[i][1] = 0.0f;
                    }
                    else if(diamond_bucket[i][1] == DIAMOND_UP && (2 <= ame_y_detect && ame_y_detect <= 13)) {
                        if (the_world) {
                            score += 2;
                        }
                        else {
                            score += 7;
                        }
                        engine->play2D("object/coin.wav");
                        diamond_bucket[i][1] = 0.0f;
                    }
                }
            }
            diamond_bucket[i][0] -= diamond_step;
        }
    }
}

int diamond_frequence = 40 * CLOCK_MULTIPLY;
int diamond_clock = 0;
void diamond_bucket_idleFunc() {
    if (diamond_clock == diamond_frequence) {
        if (rand() % 5 != 1) {
            for (int i = 0; i < DIAMOND_FULL; i++) {
                if (diamond_bucket[i][1] == 0.0f) {
                    diamond_bucket[i][0] = 12.5f;
                    if (rand() % 2 == 0) {
                        diamond_bucket[i][1] = DIAMOND_UP;
                    }
                    else {
                        diamond_bucket[i][1] = DIAMOND_DOWN;
                    }
                    break;
                }
            }
        }
        diamond_clock = 0;
    }
    diamond_clock++;
}


void diamond_float_idleFunc() {
    float_diamond_state += 1;
    float_diamond = 0.02 * cos(0.062831 * float_diamond_state / CLOCK_MULTIPLY);
    if (float_diamond_state == 100 * CLOCK_MULTIPLY) {
        float_diamond_state = 0;
    }
}

/* background idle function */
void bg_state_idleFunc() {
    if (bg_translate <= -26.0) {
        bg_state = (bg_state + 1) % BG_NUM;
        bg_translate = 0.0;
    }
    else {
        bg_translate -= bg_translate_unit;
    }
}

void idleFunc() {
    if (start_flag == 1) {
        if (ame_state == run) {
            ame_run_state_idleFunc();
        }
        else if (ame_state == jump) {
            ame_jump_state_idleFunc();
        }
        bg_state_idleFunc();
        diamond_move_idleFunc();
        diamond_bucket_idleFunc();
        diamond_float_idleFunc();
        gura_move_idleFunc();
        kiara_move_idleFunc();
        ame_guard_idleFunc();
        keyboardIdle();
        if (the_world) {
            the_world_idleFunc();
        }
        else {
            out_of_the_world_idleFunc();
        }
    }
    else {
        ame_stand_state_idleFunc();
        mouse_position_idleFunc();
    }
        
    gura_state_idleFunc();
    kiara_idleFunc();
    
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInitWindowPosition(480, 270);
    glutInitWindowSize(WIDTH, HEIGHT);		// set the size of Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Texture Mapping - Programming Techniques");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    
    glutIdleFunc(idleFunc);
    glutMainLoop();
    return 0;
}