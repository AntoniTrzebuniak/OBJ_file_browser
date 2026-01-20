#include "utility.h"


int screen_width = 1920/2;
int screen_height = 1080/2;

int pozycjaMyszyX = 0; // na ekranie
int pozycjaMyszyY = 0;
int mbutton = 0; // wcisiety klawisz myszy

double kameraX = 10.0;
double kameraZ = 20.0;
double kameraD = -3;
double kameraPredkosc = 0;
double kameraKat = -20;
double kameraPredkoscObrotu = 0;
double poprzednie_kameraX = 0;
double poprzednie_kameraZ = 0;
double poprzednie_kameraD = 0;

double rotation = 0;
double blend = 0.5;
double blend2 = 0;

double kameraLR = 0.0;
BitMap b = { b.vertex=0, b.lines=0, b.bryla=1, b.texture=1, b.singleCol=0 };
glm::vec3 SolidCol(0.f);

//vector<Object> scene;
//int sceneVertexCount = 0;

void klawisz( unsigned char key, int x, int y )
{
    int mod = glutGetModifiers();
    
    switch(key)
    {
    case 27: // Esc - koniec
        exit(1);
        break;
    case 'z': // kamera w lewo
        kameraLR -= 0.5;
        break;
    case 'x': // kamera w prawo
        kameraLR += 0.5;
        break;
    case 'w':
        b.vertex = ~b.vertex;
        break;
    case 'l':
        b.lines = ~b.lines;
        break;
    case 'b':
        b.bryla = ~b.bryla;
        break;
    case 't':
        b.texture = ~b.texture;
        break;
    case 'f':
        b.singleCol = ~b.singleCol;
        break;
    case 'r':
        SolidCol = glm::vec3(0.f);
    case 'G':
    case 'g':
        SolidCol.r += (mod & GLUT_ACTIVE_SHIFT) ? -0.05f : 0.05f;
        SolidCol.r = std::clamp(SolidCol.r, 0.0f, 1.0f);
        break;
    case 'H':
    case 'h':
        SolidCol.g += (mod & GLUT_ACTIVE_SHIFT) ? -0.05f : 0.05f;
        SolidCol.g = std::clamp(SolidCol.g, 0.0f, 1.0f);
        break;
    case 'J':
    case 'j':
        SolidCol.b += (mod & GLUT_ACTIVE_SHIFT) ? -0.05f : 0.05f;
        SolidCol.b = std::clamp(SolidCol.b, 0.0f, 1.0f);
        break;
    }

}

// ================== OBS£UGA MYSZY ==================
void mysz(int button, int state, int x, int y)
{
    mbutton = button;

    if (state == GLUT_DOWN)
    {
        pozycjaMyszyX = x;
        pozycjaMyszyY = y;

        poprzednie_kameraX = kameraX;
        poprzednie_kameraZ = kameraZ;
        poprzednie_kameraD = kameraD;
    }
}

void mysz_ruch(int x, int y)
{
    if (mbutton == GLUT_LEFT_BUTTON)
    {
        kameraX = poprzednie_kameraX - (pozycjaMyszyX - x) * 0.1;
        kameraZ = poprzednie_kameraZ - (pozycjaMyszyY - y) * 0.1;
    }

    if (mbutton == GLUT_RIGHT_BUTTON)
    {
        kameraD = poprzednie_kameraD + (pozycjaMyszyY - y) * 0.1;
    }
}



// ================== RESIZE ==================
void rozmiar(int w, int h)
{
    screen_width = w;
    screen_height = h;

    glViewport(0, 0, w, h);
    P = glm::perspective(glm::radians(60.0f),
        (float)w / (float)h, 1.0f, 1000.0f);
}