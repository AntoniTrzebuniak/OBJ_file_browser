#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <algorithm>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "loaders/LoadOBJ.h"


//#include "loaders/LoadOBJ.h"
//#include "loaders/readMTL.h"

extern int screen_width;
extern int screen_height;

extern int pozycjaMyszyX; // na ekranie
extern int pozycjaMyszyY;
extern int mbutton; // wcisiety klawisz myszy


extern glm::vec3 camPos;
extern glm::vec3 camFront;
extern glm::vec3 camUp;
extern glm::vec3 camRight;
extern float camYaw;
extern float camPitch;
extern float camMouseSensitivity;
extern float camPanSensitivity;
extern float camScrollStep;

extern double rotation;
extern double blend;
extern double blend2;

extern GLint uniformTex0;
extern GLint uniformTex1;

extern GLuint programID;
extern GLuint tex_id0;

// ===== MACIERZE =====
extern glm::mat4 MV;
extern glm::mat4 P;

// globalna zmienna sceny
extern std::vector<Object> scene;
extern int sceneVertexCount;

extern double kameraLR;

typedef struct {
    unsigned char vertex : 1;
    unsigned char lines : 1;
    unsigned char bryla : 1;
    unsigned char texture : 1;
    unsigned char singleCol : 1;
} BitMap;

extern BitMap b;
extern glm::vec3 SolidCol;


void klawisz( unsigned char key, int x, int y );
void mysz( int button, int state, int x, int y );
void mysz_ruch( int x, int y );
void rysuj( void );
void rozmiar( int w, int h );
static void updateCameraVectors();
void initFreeCamera( const glm::vec3& center, float distance );
