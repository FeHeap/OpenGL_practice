#include <cstdlib>
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


// sleep clock(int)
//#define SLEEP_CLOCK 0 // Need?

// start up the engine
irrklang::ISoundEngine * engine = irrklang::createIrrKlangDevice();

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
    char* tempData = (char*)malloc(((size) * 4 / 3) * sizeof(char));
    for (i = 0, j = 0; i < size; i += 3, j += 4) {
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
        if (tempData[j] == 0 && tempData[j + 1] == 0 && tempData[j + 2] == 0 && character == 1) {
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
void DrawString(float x, float y, float z, const char* strText, int length, int size_inverse = 15) {
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
    hFont = ::CreateFont(glutGet(GLUT_WINDOW_HEIGHT) / size_inverse, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0,
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
enum ame_state {
    run,
    jump,
    stand,
} ame_state;
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
int jump_i;
GLuint ame_jump_texture[24];
int ame_jump_state = 0;
int ame_jump_flag = 0;
float ame_jump_height[29] = {
    0.25, 0.5, 0.8, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.5, -0.1, -0.1, -0.1, 0.0,
    0.05, 0.05, 0.05, 0.05, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
};
/* unrivaled ame */
#define BASIC_AME_GUARD 75
int guard_color_state = 0;
int ame_guard = 0;
void ame_guard_idleFunc() {
    if (ame_guard > 0) {
        ame_guard--;
    }
}
enum ame_dead_state {
    live,
    gura,
    kiara,
} ame_dead_state;
#define DEAD_AMIMA_CLOCK_GURA 3
#define DEAD_AMIMA_CLOCK_KIARA 5
GLuint ame_dead_gura[12];
GLuint ame_dead_kiara[8];
int ame_dead_gura_state = 0;
int ame_dead_kiara_state = 0;
void ame_init() {
    ame_state = stand;
    ame_stand_state = 0;
    ame_run_state = 0;
    ame_jump_state = 0;
    ame_jump_flag = 0;
    guard_color_state = 0;
    ame_guard = 0;
    ame_dead_state = live;
    ame_dead_gura_state = 0;
    ame_dead_kiara_state = 0;
}

/* gura */
#define GURA_IMG_NUM 3
#define CLOCK_PER_GURA_IMG 2
float gura_move_unit = 0.05f;
GLuint gura_texture[GURA_IMG_NUM];
int gura_state = 0;
float gura_translate = 3.0;
int gura_vector = 0;
void gura_init() {
    gura_state = 0;
    gura_translate = 3.0;
    gura_vector = 0;
    gura_move_unit = 0.05f;
}

/* kiara */
#define KIARA_IMG_NUM 3
#define CLOCK_PER_KIARA_IMG 2
float kiara_move_unit = 0.055;
GLuint kiara_texture[KIARA_IMG_NUM];
int kiara_state = 0;
float kiara_translate = 5.0;
void kiara_init() {
    kiara_state = 0;
    kiara_translate = 5.0;
    kiara_move_unit = 0.055;
}

/* background */
#define BG_NUM 9
#define AME_BG_NUM 7
#define BG_TRANSLATE_UNIT 0.03
GLuint bg_texture[BG_NUM][2];
GLuint bg_ame[AME_BG_NUM];
int bg_state = 0;
float bg_translate = 0.0;
void background_init() {
    bg_state = 0;
    bg_translate = 0.0;
    PlaySound(L"ame/Amelia_Watsons_BGM_OP.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

/* object */
// diamond
#define DIAMOND_FULL 16
#define DIAMOND_UP -1.2f
#define DIAMOND_DOWN -2.6f
#define DIAMOND_FREQUENCE 40
#define DIAMOND_MOVE_UNIT 0.023
GLuint diamond;
float diamond_bucket[DIAMOND_FULL][2];
int diamond_clock = 0;
int float_diamond_state = 0;
float float_diamond = 0.0;
void diamond_init() {
    diamond_clock = 0;
    float_diamond_state = 0;
    float_diamond = 0.0;
    for (int i = 0; i < DIAMOND_FULL; i++) {
        diamond_bucket[i][0] = 0;
        diamond_bucket[i][1] = 0;
    }
}

/* bubba */
GLuint bubba;

/* start */
GLuint ame_head;
GLuint ame_talk;
GLuint sttm_texture[4];
int sttm_state = 0;
int start_mark_appear = 1;
/* stop */
GLuint stpm_texture[4];
int stpm_state = 0;
/* manu */
GLuint menu_texture[4];
int menu_state = 0;
GLuint restart;
int err_lots_count = 0;
const int err_lots[] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                       1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                1,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                       1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                         1, 0, 0, 0, 0, 0, 0, 0, 0,
                                             1, 0, 0, 0, 0,
                                                 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
                                                 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
                                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
const int err_lots_full = sizeof(err_lots) / sizeof(int);
GLuint blue;
int blue_count = 0;
int black_count = 0;
/* control */
int start_flag = 0;
int stop_flag = 0;
int the_world = 0;
void control_init() {
    sttm_state = 0;
    start_mark_appear = 1;
    stpm_state = 0;
    menu_state = 0;
    err_lots_count = 0;
    blue_count = 0;
    black_count = 0;
    start_flag = 0;
    stop_flag = 0;
    the_world = 0;
}

/* the world */
#define THE_WORLD_BASIC_COUNT 220
#define THE_WORLD_BASIC_COUNT_RE 110
int the_world_voice_count = 0;
void the_world_idleFunc() {
    if (the_world_voice_count > 0) {
        if (rand() % 20 == 0) {
            int back = rand() % 100 - 50;
            bg_translate += (back * BG_TRANSLATE_UNIT);
            for (int i = 0; i < DIAMOND_FULL; i++) {
                if (diamond_bucket[i][1] != 0) {
                    diamond_bucket[i][0] += (back * DIAMOND_MOVE_UNIT);
                }
            }
        }
        the_world_voice_count--;
        if (the_world_voice_count < THE_WORLD_BASIC_COUNT / 2) {
            gura_move_unit *= 0.96;
            kiara_move_unit *= 0.96;
        }
    }
}

void out_of_the_world_idleFunc() {
    if (the_world_voice_count > 0) {
        the_world_voice_count--;
        gura_move_unit *= 1.042;
        kiara_move_unit *= 1.042;
        if (the_world_voice_count == 0) {
            gura_move_unit = 0.05;
            kiara_move_unit = 0.055;
        }
    }
}
void the_world_init() {
    the_world_voice_count = 0;
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
    ame_dead_state = live;

    /* ame run */
    strcpy(strBuff, "ame/ame-run-0.bmp");
    for (int i = 0; i < 7; i++) {
        strBuff[12] = i + '0';
        Image* image1 = loadTexture((char*)strBuff, 1, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, ame_run_texture + i);
        glBindTexture(GL_TEXTURE_2D, ame_run_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 2, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }

    /* ame jump */
    strcpy(strBuff, "ame/ame-jump-00.bmp");
    for (int i = 0; i <= JUMP_IMG_NUM; i++) {
        strBuff[13] = '0' + i / 10;
        strBuff[14] = '0' + i % 10;
        Image* image1 = loadTexture((char*)strBuff, 1, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, ame_jump_texture + i);
        glBindTexture(GL_TEXTURE_2D, ame_jump_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 2, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }

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
            strBuff[14] = i / 10 + '0';
            strBuff[15] = i % 10 + '0';
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
        glGenTextures(1, bg_ame + i);
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
        glGenTextures(1, sttm_texture + i);
        glBindTexture(GL_TEXTURE_2D, sttm_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }
    /* stop mark */
    strcpy(strBuff, "Background/stop_mark_0.bmp");
    for (int i = 0; i < 4; i++) {
        strBuff[21] = i + '0';
        Image* image1 = loadTexture((char*)strBuff, 0, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, stpm_texture + i);
        glBindTexture(GL_TEXTURE_2D, stpm_texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }

    /* menu mark */
    strcpy(strBuff, "Background/menu_mark_0.bmp");
    for (int i = 0; i < 4; i++) {
        strBuff[21] = i + '0';
        Image* image1 = loadTexture((char*)strBuff, 0, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, menu_texture + i);
        glBindTexture(GL_TEXTURE_2D, menu_texture[i]);
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

    /* restart */
    image1 = loadTexture((char*)"restart/restart_error.bmp", 1, 0);
    if (image1 == NULL) {
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }
    // Create Texture
    glGenTextures(1, &restart);
    glBindTexture(GL_TEXTURE_2D, restart);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 2, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
    free(image1);

    /* blue */
    image1 = loadTexture((char*)"restart/restart_blue.bmp", 0, 0);
    if (image1 == NULL) {
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }
    // Create Texture
    glGenTextures(1, &blue);
    glBindTexture(GL_TEXTURE_2D, blue);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
    free(image1);

    /* gura dead */
    strcpy(strBuff, "gura/gura_rotate_00.bmp");
    for (int i = 0; i < 12; i++) {
        strBuff[17] = '0' + i / 10;
        strBuff[18] = '0' + i % 10;
        Image* image1 = loadTexture((char*)strBuff, 0, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, ame_dead_gura + i);
        glBindTexture(GL_TEXTURE_2D, ame_dead_gura[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }

    /* kiara dead */
    strcpy(strBuff, "kiara/kiara_walk_0.bmp");
    for (int i = 0; i < 8; i++) {
        strBuff[17] = '0' + i;
        Image* image1 = loadTexture((char*)strBuff, 0, 0);
        if (image1 == NULL) {
            printf("Image was not returned from loadTexture\n");
            exit(0);
        }
        // Create Texture
        glGenTextures(1, ame_dead_kiara + i);
        glBindTexture(GL_TEXTURE_2D, ame_dead_kiara[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->data);
        free(image1);
    }


    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ALPHA);
    glAlphaFunc(GL_GREATER, .8f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_FLAT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.5, 0.5, 0.5, 0.5);
    srand(time(NULL));

    background_init();
    diamond_init();
}

#define WORD_CLOCK 4
int talk_state = 0;
int voice_state[6] = { 0, 0, 0, 0, 0, 0 };
int words_length_state[6] = { 1, 1, 1, 1, 1, 1 };
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
            if (Talk.talk_clock_0[words_length_state[0] / WORD_CLOCK] < strlen(Talk.talk_0[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_0[0], Talk.talk_clock_0[words_length_state[0] / WORD_CLOCK]);
                words_length_state[0]++;
            }
            else if (Talk.talk_clock_0[words_length_state[0] / WORD_CLOCK] < strlen(Talk.talk_0[0]) + strlen(Talk.talk_0[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_0[0], strlen(Talk.talk_0[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_0[1], Talk.talk_clock_0[words_length_state[0] / WORD_CLOCK] - strlen(Talk.talk_0[0]));
                words_length_state[0]++;
            }
            else if (Talk.talk_clock_0[words_length_state[0] / WORD_CLOCK] < strlen(Talk.talk_0[0]) + strlen(Talk.talk_0[1]) + strlen(Talk.talk_0[2])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_0[0], strlen(Talk.talk_0[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_0[1], strlen(Talk.talk_0[1]));
                DrawString(-3.0, 1.60, 0.2, Talk.talk_0[2], Talk.talk_clock_0[words_length_state[0] / WORD_CLOCK] - strlen(Talk.talk_0[0]) - strlen(Talk.talk_0[1]));
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
            if (Talk.talk_clock_1[words_length_state[1] / WORD_CLOCK] < strlen(Talk.talk_1[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_1[0], Talk.talk_clock_1[words_length_state[1] / WORD_CLOCK]);
                words_length_state[1]++;
            }
            else if (Talk.talk_clock_1[words_length_state[1] / WORD_CLOCK] < strlen(Talk.talk_1[0]) + strlen(Talk.talk_1[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_1[0], strlen(Talk.talk_1[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_1[1], Talk.talk_clock_1[words_length_state[1] / WORD_CLOCK] - strlen(Talk.talk_1[0]));
                words_length_state[1]++;
            }
            else if (Talk.talk_clock_1[words_length_state[1] / WORD_CLOCK] < strlen(Talk.talk_1[0]) + strlen(Talk.talk_1[1]) + strlen(Talk.talk_1[2])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_1[0], strlen(Talk.talk_1[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_1[1], strlen(Talk.talk_1[1]));
                DrawString(-3.0, 1.60, 0.2, Talk.talk_1[2], Talk.talk_clock_1[words_length_state[1] / WORD_CLOCK] - strlen(Talk.talk_1[0]) - strlen(Talk.talk_1[1]));
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
            if (Talk.talk_clock_2[words_length_state[2] / WORD_CLOCK] < strlen(Talk.talk_2[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_2[0], Talk.talk_clock_2[words_length_state[2] / WORD_CLOCK]);
                words_length_state[2]++;
            }
            else if (Talk.talk_clock_2[words_length_state[2] / WORD_CLOCK] < strlen(Talk.talk_2[0]) + strlen(Talk.talk_2[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_2[0], strlen(Talk.talk_2[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_2[1], Talk.talk_clock_2[words_length_state[2] / WORD_CLOCK] - strlen(Talk.talk_2[0]));
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
            if (Talk.talk_clock_3[words_length_state[3] / WORD_CLOCK] < strlen(Talk.talk_3[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_3[0], Talk.talk_clock_3[words_length_state[3] / WORD_CLOCK]);
                words_length_state[3]++;
            }
            else if (Talk.talk_clock_3[words_length_state[3] / WORD_CLOCK] < strlen(Talk.talk_3[0]) + strlen(Talk.talk_3[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_3[0], strlen(Talk.talk_3[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_3[1], Talk.talk_clock_3[words_length_state[3] / WORD_CLOCK] - strlen(Talk.talk_3[0]));
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
            if (Talk.talk_clock_4[words_length_state[4] / WORD_CLOCK] < strlen(Talk.talk_4[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_4[0], Talk.talk_clock_4[words_length_state[4] / WORD_CLOCK]);
                words_length_state[4]++;
            }
            else if (Talk.talk_clock_4[words_length_state[4] / WORD_CLOCK] < strlen(Talk.talk_4[0]) + strlen(Talk.talk_4[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_4[0], strlen(Talk.talk_4[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_4[1], Talk.talk_clock_4[words_length_state[4] / WORD_CLOCK] - strlen(Talk.talk_4[0]));
                words_length_state[4]++;
            }
            else if (Talk.talk_clock_4[words_length_state[4] / WORD_CLOCK] < strlen(Talk.talk_4[0]) + strlen(Talk.talk_4[1]) + strlen(Talk.talk_4[2])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_4[0], strlen(Talk.talk_4[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_4[1], strlen(Talk.talk_4[1]));
                DrawString(-3.0, 1.60, 0.2, Talk.talk_4[2], Talk.talk_clock_4[words_length_state[4] / WORD_CLOCK] - strlen(Talk.talk_4[0]) - strlen(Talk.talk_4[1]));
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
            if (Talk.talk_clock_5[words_length_state[5] / WORD_CLOCK] < strlen(Talk.talk_5[0])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_5[0], Talk.talk_clock_5[words_length_state[5] / WORD_CLOCK]);
                words_length_state[5]++;
            }
            else if (Talk.talk_clock_5[words_length_state[5] / WORD_CLOCK] < strlen(Talk.talk_5[0]) + strlen(Talk.talk_5[1])) {
                DrawString(-3.0, 2.5, 0.2, Talk.talk_5[0], strlen(Talk.talk_5[0]));
                DrawString(-3.0, 2.05, 0.2, Talk.talk_5[1], Talk.talk_clock_5[words_length_state[5] / WORD_CLOCK] - strlen(Talk.talk_5[0]));
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
void talk_init() {
    talk_state = 0;
    for (int i = 0; i < 6; i++) {
        voice_state[i] = 0;
        words_length_state[i] = 1;
    }
}

/* score */
int score = 0;
void score_display() {
    static char score_buff[7] = "000000";
    glPushMatrix();
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glBindTexture(GL_TEXTURE_2D, ame_talk);
    glTranslated(6.2, 4.2, 0);
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
void dead_score_display() {
    static char score_buff[7] = "000000";
    static char score_s[] = "score";
    static char restart_comment[] = "press 'R' to restart the game";
    glPushMatrix();
    glColor4f(1.f, 1.f, 1.f, 1.f);
    sprintf(score_buff, "%06d", score);
    DrawString(3.6, 0.2, 0.00006, score_s, strlen(score_s), 10);
    DrawString(3.2, -0.2, 0.00006, score_buff, strlen(score_buff), 10);
    DrawString(-6.2, 0.0, 0.00006, restart_comment, strlen(restart_comment), 16);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glPopMatrix();
}
void score_init() {
    score = 0;
}

/* blood */
#define BLOOD_MAX 3
int blood = BLOOD_MAX;
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
void blood_init() {
    blood = BLOOD_MAX;
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

void stop_display() {
    glPushMatrix();
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glTranslated(8.0, 4.2, 0);
    glBindTexture(GL_TEXTURE_2D, stpm_texture[stpm_state]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.4, -0.4, -0.999);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.4, -0.4, -0.999);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.4, 0.4, -0.999);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.4, 0.4, -0.999);
    glEnd();
    glPopMatrix();
}

void menu_display() {
    glPushMatrix();
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glTranslated(8.0, 3.2, 0);
    glBindTexture(GL_TEXTURE_2D, menu_texture[menu_state]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.4, -0.4, -0.999);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.4, -0.4, -0.999);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.4, 0.4, -0.999);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.4, 0.4, -0.999);
    glEnd();
    glPopMatrix();
}

/* restart process */
void restart_process();

void restart_press_display() {
    glPushMatrix();
    for (int i = 0, windows_error_count = 0; i < err_lots_count; i++) {
        if (err_lots[i] == 1) {
            if (err_lots_count != err_lots_full) {
                windows_error_count++;
                glBindTexture(GL_TEXTURE_2D, restart);
                glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0); glVertex3f(-1.25, -0.75, 0.00003);
                glTexCoord2f(1.0, 0.0); glVertex3f(1.25, -0.75, 0.00003);
                glTexCoord2f(1.0, 1.0); glVertex3f(1.25, 0.75, 0.00003);
                glTexCoord2f(0.0, 1.0); glVertex3f(-1.25, 0.75, 0.00003);
                glEnd();
                if (windows_error_count % 72 == 0) {
                    glTranslated(-16, 0, 0);
                }
                else if (windows_error_count % 18 == 0) {
                    glTranslated(2.5, 0, 0);
                }
                else if (windows_error_count % 9 == 0) {
                    glTranslated(-8, 8, 0.000001);
                }
                else {
                    glTranslated(0.5, -0.5, 0.000001);
                }
            }
            else {
                if (blue_count < 200) {
                    glBindTexture(GL_TEXTURE_2D, blue);
                    glBegin(GL_QUADS);
                    glTexCoord2f(0.0, 0.0); glVertex3f(-6.8, -3.6, 0.00003);
                    glTexCoord2f(1.0, 0.0); glVertex3f(6.8, -3.6, 0.00003);
                    glTexCoord2f(1.0, 1.0); glVertex3f(6.8, 3.6, 0.00003);
                    glTexCoord2f(0.0, 1.0); glVertex3f(-6.8, 3.6, 0.00003);
                    glEnd();
                    blue_count++;
                }
                else {
                    if (blue_count == 200) {
                        Sleep(2000);
                        blue_count++;
                    }
                    if (black_count == 10000) {
                        restart_process();
                    }
                    glColor4f(0.f, 0.f, 0.f, 1.f);
                    glBegin(GL_QUADS);
                    glVertex3f(-6.8, -3.6, 0.00004);
                    glVertex3f(6.8, -3.6, 0.00004);
                    glVertex3f(6.8, 3.6, 0.00004);
                    glVertex3f(-6.8, 3.6, 0.00004);
                    glEnd();
                    black_count++;
                }
            }
        }
    }
    glPopMatrix();
}

/* main display */
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

        glBindTexture(GL_TEXTURE_2D, sttm_texture[sttm_state]);
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

    /* score blood */
    if (start_flag == 1) {
        score_display();
        blood_display();
        stop_display();
        if (stop_flag == 1) {
            if (ame_dead_state == live) {
                if (menu_state == 3) {
                    restart_press_display();
                }
                menu_display();
            }
            else {
                glDisable(GL_TEXTURE_2D);
                glColor4f(1.f, 1.f, 1.f, 1.f);
                glBegin(GL_QUADS);
                glVertex3f(-6.8, -3.6, 0.00004);
                glVertex3f(6.8, -3.6, 0.00004);
                glVertex3f(6.8, 3.6, 0.00004);
                glVertex3f(-6.8, 3.6, 0.00004);
                glEnd();
                dead_score_display();
                glEnable(GL_TEXTURE_2D);

                glColor4f(1.f, 1.f, 1.f, 1.f);
                if (ame_dead_state == gura) {
                    glBindTexture(GL_TEXTURE_2D, ame_dead_gura[ame_dead_gura_state / DEAD_AMIMA_CLOCK_GURA]);
                }
                else if (ame_dead_state == kiara) {
                    glBindTexture(GL_TEXTURE_2D, ame_dead_kiara[ame_dead_kiara_state / DEAD_AMIMA_CLOCK_KIARA]);
                }
                glBegin(GL_QUADS);
                glTexCoord2f(0.0, 0.0); glVertex3f(-3.0, -3.0, 0.00005);
                glTexCoord2f(1.0, 0.0); glVertex3f(3.0, -3.0, 0.00005);
                glTexCoord2f(1.0, 1.0); glVertex3f(3.0, 3.0, 0.00005);
                glTexCoord2f(0.0, 1.0); glVertex3f(-3.0, 3.0, 0.00005);
                glEnd();
            }
        }
    }

    glColor4f(1.f, 1.f, 1.f, 1.f);
    glPushMatrix();
    glTranslated(-4.0, -3.1, 0);
    if (ame_guard <= 0) {
        glColor4f(1.f, 1.f, 1.f, 1.f);
    }
    else {
        for (int i = 0; i < 40; i++) {
            glColor4f(1.f, 1.f, 1.f - 40 * 0.01, 1.f);
        }
        if (guard_color_state = 0) {
            glColor4f(1.f, 1.f, 1.f, 1.f);
            guard_color_state = 1;
        }
        else if (guard_color_state == 1 || guard_color_state == 3) {
            glColor4f(1.f, 1.f, 0.8f, 1.f);
            if (guard_color_state == 1) {
                guard_color_state = 2;
            }
            else {
                guard_color_state = 0;
            }
        }
        else {
            glColor4f(1.f, 1.f, 0.7f, 1.f);
            guard_color_state = 3;
        }
    }
    if (ame_state == run) {
        glTranslated(0.13, 0, 0);
        glBindTexture(GL_TEXTURE_2D, ame_run_texture[(int)(ame_run_state / CLOCK_PER_RUN_IMG)]);
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
        glPushMatrix();
        glTranslated(0.0, ame_jump_height[ame_jump_state], 0);
        glBindTexture(GL_TEXTURE_2D, ame_jump_texture[ame_jump_state]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(1.5, -0.1);
        glTexCoord2f(1, 0); glVertex2f(0.0, -0.1);
        glTexCoord2f(1, 1); glVertex2f(0.0, 2);
        glTexCoord2f(0, 1); glVertex2f(1.5, 2);
        glEnd();
        glPopMatrix();

        glDisable(GL_ALPHA_TEST);
        glBegin(GL_QUADS);
        glColor4f(0.f, 0.f, 0.f, 0.2f);
        glTexCoord2f(0, 0); glVertex2f(0.0, 0.0);
        glTexCoord2f(1, 0); glVertex2f(1.5, 0.0);
        glTexCoord2f(1, 1); glVertex2f(0.75, -0.76);
        glTexCoord2f(0, 1); glVertex2f(-0.75, -0.76);
        glEnd();
        glEnable(GL_ALPHA_TEST);
    }
    else if (ame_state == stand) {
        glTranslated(0.13, 0, 0);
        glBindTexture(GL_TEXTURE_2D, ame_stand_texture[ame_stand_state / CLOCK_PER_STAND_IMG]);
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


    glPushMatrix();
    glTranslated(gura_translate, -3.1, 0);
    if (!the_world) { glColor4f(1.f, 1.f, 1.f, 1.f); }
    else { glColor4f(0.5f, 0.5f, 0.5f, 1.f); }
    if (gura_vector == 0) {
        glBindTexture(GL_TEXTURE_2D, gura_texture[gura_state / CLOCK_PER_GURA_IMG]);
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
        glBindTexture(GL_TEXTURE_2D, gura_texture[gura_state / CLOCK_PER_GURA_IMG]);
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


    glPushMatrix();
    glTranslated(kiara_translate, -1.5, 0);
    if (!the_world) { glColor4f(1.f, 1.f, 1.f, 1.f); }
    else { glColor4f(0.5f, 0.5f, 0.5f, 1.f); }
    glBindTexture(GL_TEXTURE_2D, kiara_texture[kiara_state / CLOCK_PER_KIARA_IMG]);
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

#define ESC 27
#define keyboardArraySize 500
int keyboardArray[keyboardArraySize];
void keyboardArrayInit() {
    for (int i = 0; i < keyboardArraySize; i++) {
        keyboardArray[i] = 0;
    }
}
#define MULTI_JUMP_BOUNDARY_EARLY 8
#define MULTI_JUMP_BOUNDARY_LAST 12
void keyboard(unsigned char key, int x, int y) {
    if (start_flag == 1 && stop_flag == 0) {
        switch (key) {
        case ' ':
            if (ame_state == jump) {
                keyboardArray[' '] = 1;
            }
            else if (ame_state == run) {
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
                jump_i = 0;
                keyboardArray[' '] = 1;
            }
            break;
        case 'f':
        case 'F':
            if (start_mark_appear == 0) {
                if (!the_world) {
                    the_world_voice_count = THE_WORLD_BASIC_COUNT;
                    the_world = 1;
                    PlaySound(L"ame/Giornos_Theme_Il_vento_doro.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
                    engine->play2D("ame/za_warudo_stop_time_sound.mp3");
                }
                else if (the_world && the_world_voice_count == 0) {
                    the_world_voice_count = THE_WORLD_BASIC_COUNT_RE;
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
    else if (start_flag == 2) {
        switch (key) {
        case ' ':
            if (words_length_state[talk_state] / WORD_CLOCK < Talk.talks_end[talk_state]) {
                words_length_state[talk_state] = Talk.talks_end[talk_state] * WORD_CLOCK;
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
        case 'r':
        case 'R':
            if (stop_flag == 1 && ame_dead_state != live) {
                restart_process();
            }
            break;
        case ESC: // "esc" on keyboard
            if (start_flag == 1 && ame_dead_state == live) {
                if (stop_flag == 0) {
                    stop_flag = 1;
                    stpm_state = 3;
                }
                else {
                    stop_flag = 0;
                    stpm_state = 0;
                }
            }
            break;
        default: // pass
            break;
        }
    }
}

void keyboardIdle() {
    if (keyboardArray[' '] == 1 && MULTI_JUMP_BOUNDARY_EARLY <= ame_jump_state && ame_jump_state <= MULTI_JUMP_BOUNDARY_LAST) {
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
            sttm_state = 2;
        }
        if (WindowWidth / 2 - 85 <= x && x <= WindowWidth / 2 + 85 && WindowHeight / 2 - 65 <= y && y <= WindowHeight / 2 + 65 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            engine->play2D("Background/start_button.wav");
            sttm_state = 3;
            start_flag = 2;
        }
        else if (sttm_state == 2 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            sttm_state = 1;
        }
    }
    else if (start_flag == 1 && menu_state != 3) {
        if (ame_dead_state == live && stop_flag == 1) {
            if (WindowWidth * 0.9 <= x && x <= WindowWidth && WindowHeight * 0.1 <= y && y <= WindowHeight * 0.2 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                menu_state = 2;
            }
            if (WindowWidth * 0.9 <= x && x <= WindowWidth && WindowHeight * 0.1 <= y && y <= WindowHeight * 0.2 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
                menu_state = 3;
                PlaySound(NULL, NULL, NULL);
            }
            else if (menu_state == 2 && button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
                menu_state = 1;
            }
        }

        if (WindowWidth * 0.9 <= x && x <= WindowWidth && 0 <= y && y <= WindowHeight * 0.1 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && stop_flag == 0) {
            stpm_state = 2;
        }
        else if (WindowWidth * 0.9 <= x && x <= WindowWidth && 0 <= y && y <= WindowHeight * 0.1 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && ame_dead_state == live && stop_flag == 1) {
            stpm_state = 1;
        }
        if (WindowWidth * 0.9 <= x && x <= WindowWidth && 0 <= y && y <= WindowHeight * 0.1 && button == GLUT_LEFT_BUTTON && state == GLUT_UP && stpm_state == 2 && stop_flag == 0) {
            stpm_state = 3;
            stop_flag = 1;
            menu_state = 0;
        }
        else if (WindowWidth * 0.9 <= x && x <= WindowWidth && 0 <= y && y <= WindowHeight * 0.1 && button == GLUT_LEFT_BUTTON && state == GLUT_UP && stpm_state == 2 && ame_dead_state == live && stop_flag == 1) {
            stpm_state = 0;
            stop_flag = 0;
        }
        else if (stpm_state == 2 && button == GLUT_LEFT_BUTTON && state == GLUT_UP && stop_flag == 0) {
            stpm_state = 2;
            stop_flag = 1;
        }
        else if (stpm_state == 1 && button == GLUT_LEFT_BUTTON && state == GLUT_UP && ame_dead_state == live && stop_flag == 1) {
            stpm_state = 0;
            stop_flag = 0;
        }
    }
}
void mouse_position_start_idleFunc() {
    static POINT pt;
    BOOL bReturn = GetCursorPos(&pt); // get the position of Cursor

    //obtain window size
    float WindowWidth = glutGet(GLUT_WINDOW_WIDTH);
    float WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    //obtain window position
    float WindowPosX = glutGet(GLUT_WINDOW_X);
    float WindowPosY = glutGet(GLUT_WINDOW_Y);

    if (bReturn != 0) { // get the position succrssfully
        if (WindowPosX + WindowWidth / 2 - 85 <= pt.x && pt.x <= WindowPosX + WindowWidth / 2 + 85 && WindowPosY + WindowHeight / 2 - 65 <= pt.y && pt.y <= WindowPosY + WindowHeight / 2 + 65 && sttm_state == 0) {
            engine->play2D("Background/start_button_float.wav");
            sttm_state = 1;
        }
        else if (WindowPosX + WindowWidth / 2 - 85 <= pt.x && pt.x <= WindowPosX + WindowWidth / 2 + 85 && WindowPosY + WindowHeight / 2 - 65 <= pt.y && pt.y <= WindowPosY + WindowHeight / 2 + 65 && sttm_state == 1) {
            sttm_state = 1;
        }
        else if (sttm_state == 1) {
            sttm_state = 0;
        }
    }
}
void mouse_position_stop_idleFunc() {
    static POINT pt;
    BOOL bReturn = GetCursorPos(&pt); // get the position of Cursor

    //obtain window size
    float WindowWidth = glutGet(GLUT_WINDOW_WIDTH);
    float WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    //obtain window position
    float WindowPosX = glutGet(GLUT_WINDOW_X);
    float WindowPosY = glutGet(GLUT_WINDOW_Y);

    if (bReturn != 0 && menu_state != 3) { // get the position succrssfully
        if (ame_dead_state == live && stop_flag == 1) {
            if (WindowPosX + WindowWidth * 0.9 <= pt.x && pt.x <= WindowPosX + WindowWidth && WindowPosY + WindowHeight * 0.1 <= pt.y && pt.y <= WindowPosY + WindowHeight * 0.2 && (menu_state == 0 || menu_state == 1)) {
                menu_state = 1;
            }
            else if (menu_state == 1) {
                menu_state = 0;
            }
        }

        if (stop_flag == 0 && WindowPosX + WindowWidth * 0.9 <= pt.x && pt.x <= WindowPosX + WindowWidth && WindowPosY <= pt.y && pt.y <= WindowPosY + WindowHeight * 0.1 && (stpm_state == 0 || stpm_state == 1)) {
            stpm_state = 1;
        }
        else if (stop_flag == 0 && stpm_state == 1) {
            stpm_state = 0;
        }
        else if (ame_dead_state == live && stop_flag == 1 && WindowPosX + WindowWidth * 0.9 <= pt.x && pt.x <= WindowPosX + WindowWidth && WindowPosY <= pt.y && pt.y <= WindowPosY + WindowHeight * 0.1 && (stpm_state == 3 || stpm_state == 2)) {
            stpm_state = 2;
        }
        else if (ame_dead_state == live && stop_flag == 1 && stpm_state == 2) {
            stpm_state = 3;
        }
    }
}


/* ame idle function */
void ame_stand_state_idleFunc() {
    static int ame_stand_state_boundary = STAND_IMG_NUM * CLOCK_PER_STAND_IMG - 1;
    if (ame_stand_state == ame_stand_state_boundary) {
        ame_stand_state = 0;
    }
    else {
        ame_stand_state++;
    }
}

void ame_run_state_idleFunc() {
    static int ame_run_state_boundary = CLOCK_PER_RUN_IMG * RUN_IMG_NUM - 1;
    if (ame_run_state == ame_run_state_boundary) {
        ame_run_state = 0;
    }
    else {
        ame_run_state++;
    }
}

#define AME_JUMP_DOWN 14
void ame_jump_state_idleFunc() {
    if (ame_jump_state == JUMP_IMG_NUM) {
        ame_jump_state = 0;
        ame_state = run;
    }
    else {
        ame_jump_state++;
        if (ame_jump_state == AME_JUMP_DOWN) {
            engine->play2D("ame/ame_jump_down.wav");
        }
    }
}

/* kiara idle function */
void kiara_idleFunc() {
    static int kiara_state_boundary = KIARA_IMG_NUM * CLOCK_PER_KIARA_IMG - 1;
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

    if (-3.87f < kiara_translate && kiara_translate < -3.07f) {
        if ((2 <= ame_jump_state && ame_jump_state <= 13) && ame_guard == 0) {
            engine->play2D("ame/middle_punch.wav");
            engine->play2D("kiara/kiara_tskr.wav");
            blood -= 1;
            if (blood == 0) {
                ame_guard = -1;
                ame_dead_state = kiara;
                stop_flag = 1;
                PlaySound(L"kiara/kiara_never_gonna_give_you_up.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
            }
            else {
                ame_guard = BASIC_AME_GUARD;
            }
        }
    }

    kiara_translate -= kiara_move_unit;
}

/* gura idle function */
void gura_state_idleFunc() {
    static int gura_state_boundary = GURA_IMG_NUM * CLOCK_PER_GURA_IMG - 1;
    if (gura_state == gura_state_boundary) {
        gura_state = 0;
    }
    else {
        gura_state++;
    }
}

void gura_move_idleFunc() {
    static int gura_vector_point = 79;
    if (!the_world && rand() % gura_vector_point == 1) {
        gura_vector = !gura_vector;
    }

    if (-3.82f < gura_translate && gura_translate < -3.12f) {
        if ((ame_jump_state == 0 || ame_jump_state >= 13) && ame_guard == 0) {
            engine->play2D("ame/middle_punch.wav");
            if (rand() % 2 == 0) {
                engine->play2D("gura/gura_fbi.wav");
            }
            else {
                engine->play2D("gura/gura_goddemit.wav");
            }
            blood -= 1;
            if (blood == 0) {
                ame_guard = -1;
                ame_dead_state = gura;
                stop_flag = 1;
                PlaySound(L"gura/gura_john_cena.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
            }
            else {
                ame_guard = BASIC_AME_GUARD;
            }
        }
    }

    if (gura_vector == 0) {
        if (gura_translate < -7.5) {
            gura_vector = 1;
        }
        if (rand() % 5 != 0) {
            gura_translate -= gura_move_unit;
        }
    }
    else {
        if (gura_translate > 7.5) {
            gura_vector = 0;
        }
        if (rand() % 5 != 0) {
            gura_translate += gura_move_unit;
        }
    }
}


/* diamond idle function */
void diamond_move_idleFunc() {
    for (int i = 0; i < DIAMOND_FULL; i++) {
        if (diamond_bucket[i][1] != 0.0f) {
            if (diamond_bucket[i][0] < -12.5) {
                diamond_bucket[i][1] = 0;
            }
            else {
                if (-3.77f <= diamond_bucket[i][0] && diamond_bucket[i][0] <= -2.77f) {
                    if (diamond_bucket[i][1] == DIAMOND_DOWN && (ame_jump_state == 0 || ame_jump_state >= 13)) {
                        if (the_world) {
                            score += 2;
                        }
                        else {
                            score += 7;
                        }
                        engine->play2D("object/coin.wav");
                        diamond_bucket[i][1] = 0.0f;
                    }
                    else if (diamond_bucket[i][1] == DIAMOND_UP && (2 <= ame_jump_state && ame_jump_state <= 13)) {
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
            diamond_bucket[i][0] -= DIAMOND_MOVE_UNIT;
        }
    }
}

void diamond_bucket_idleFunc() {
    if (diamond_clock == DIAMOND_FREQUENCE) {
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
    float_diamond = 0.02 * cos(0.062831 * float_diamond_state);
    if (float_diamond_state == 100) {
        float_diamond_state = 0;
    }
}


void menu_idleFunc() {
    if (menu_state == 3) {
        if (err_lots[err_lots_count] == 1) {
            engine->play2D("restart/erro.wav");
        }
        if (err_lots_count < err_lots_full) {
            err_lots_count++;
        }
    }
}

void dead_anima_idleFunc() {
    if (ame_dead_state == gura) {
        ame_dead_gura_state = (ame_dead_gura_state + 1) % (12 * DEAD_AMIMA_CLOCK_GURA);
    }
    else if (ame_dead_state == kiara) {
        ame_dead_kiara_state = (ame_dead_kiara_state + 1) % (8 * DEAD_AMIMA_CLOCK_KIARA);
    }
}

/* background idle function */
void bg_state_idleFunc() {
    if (bg_translate <= -26.0) {
        bg_state = (bg_state + 1) % BG_NUM;
        bg_translate = 0.0;
    }
    else {
        bg_translate -= BG_TRANSLATE_UNIT;
    }
}


#define TIMER_SECOND 15
void timerIdleFunc(int zero) {
    if (start_flag == 1) {
        if (stop_flag == 0) {
            if (ame_state == run) {
                ame_run_state_idleFunc();
            }
            else if (ame_state == jump) {
                jump_i++;
                if (jump_i % CLOCK_PER_JUMP_IMG == 0) {
                    ame_jump_state_idleFunc();
                    jump_i = 0;
                }
                keyboardIdle();
            }
            bg_state_idleFunc();
            diamond_move_idleFunc();
            diamond_bucket_idleFunc();
            diamond_float_idleFunc();
            gura_move_idleFunc();
            kiara_move_idleFunc();
            ame_guard_idleFunc();
            if (the_world) {
                the_world_idleFunc();
            }
            else {
                out_of_the_world_idleFunc();
            }
        }
        else {
            if (ame_dead_state == live) {
                menu_idleFunc();
            }
            else {
                dead_anima_idleFunc();
            }
        }
        mouse_position_stop_idleFunc();
    }
    else {
        ame_stand_state_idleFunc();
        mouse_position_start_idleFunc();
    }
    if (stop_flag == 0) {
        gura_state_idleFunc();
        kiara_idleFunc();
    }

    glutPostRedisplay();
    glutTimerFunc(TIMER_SECOND, timerIdleFunc, 0);
}

/* restart process */
void restart_process() {
    ame_init();
    gura_init();
    kiara_init();
    background_init();
    diamond_init();
    control_init();
    the_world_init();
    talk_init();
    score_init();
    blood_init();
    keyboardArrayInit();
}

int main(int argc, char** argv)
{
    glutInitWindowPosition(480, 270);
    glutInitWindowSize(WIDTH, HEIGHT);		// set the size of Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Crazy Ame");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(myReshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutTimerFunc(TIMER_SECOND, timerIdleFunc, 0);
    glutMainLoop();
    return 0;
}
