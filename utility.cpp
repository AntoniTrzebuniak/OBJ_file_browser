
#include "utility.h"


int screen_width = 1920 / 2;
int screen_height = 1080 / 2;

int pozycjaMyszyX = 0; // na ekranie
int pozycjaMyszyY = 0;
int mbutton = 0; // wcisiety klawisz myszy

glm::vec3 camPos( 0.0f, 0.0f, 5.0f );
glm::vec3 camFront( 0.0f, 0.0f, -1.0f );
glm::vec3 camUp( 0.0f, 1.0f, 0.0f );
glm::vec3 camRight( 1.0f, 0.0f, 0.0f );

float camYaw = -90.0f;
float camPitch = 0.0f;

float camMouseSensitivity = 0.2f; 
float camPanSensitivity = 0.1f; 
float camScrollStep = 2.0f; 

static bool gMouseLook = false;
static bool gMousePan = false;
static int gLastMouseX = 0;
static int gLastMouseY = 0;


double rotation = 0;
double blend = 0.5;
double blend2 = 0;

BitMap b = { b.vertex = 0, b.lines = 0, b.bryla = 1, b.texture = 1, b.singleCol = 0 };
glm::vec3 SolidCol( 0.f );

//vector<Object> scene;
//int sceneVertexCount = 0;

void klawisz( unsigned char key, int x, int y )
{
    int mod = glutGetModifiers();

    switch( key )
    {
    case 27: // Esc - koniec
        exit( 1 );
        break;
    case 'z':

        break;
    case 'x':

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
        SolidCol = glm::vec3( 0.f );
    case 'G':
    case 'g':
        SolidCol.r += ( mod & GLUT_ACTIVE_SHIFT ) ? -0.05f : 0.05f;
        SolidCol.r = std::clamp( SolidCol.r, 0.0f, 1.0f );
        break;
    case 'H':
    case 'h':
        SolidCol.g += ( mod & GLUT_ACTIVE_SHIFT ) ? -0.05f : 0.05f;
        SolidCol.g = std::clamp( SolidCol.g, 0.0f, 1.0f );
        break;
    case 'J':
    case 'j':
        SolidCol.b += ( mod & GLUT_ACTIVE_SHIFT ) ? -0.05f : 0.05f;
        SolidCol.b = std::clamp( SolidCol.b, 0.0f, 1.0f );
        break;
    }

}

// ================== OBS£UGA MYSZY ==================
void mysz( int button, int state, int x, int y )
{
    mbutton = button;

    if( state == GLUT_DOWN )
    {
        if( button == 3 ) { camPos += camFront * camScrollStep; return; } 
        if( button == 4 ) { camPos -= camFront * camScrollStep; return; } 
    }

    if( state == GLUT_DOWN )
    {
        gLastMouseX = x;
        gLastMouseY = y;
        pozycjaMyszyX = x;
        pozycjaMyszyY = y;

        gMouseLook = ( button == GLUT_RIGHT_BUTTON );
        gMousePan = ( button == GLUT_LEFT_BUTTON );
    }
    else
    {
        if( button == GLUT_RIGHT_BUTTON ) gMouseLook = false;
        if( button == GLUT_LEFT_BUTTON )  gMousePan = false;
    }
}

void mysz_ruch( int x, int y )
{
    const float dx = ( float )( x - gLastMouseX );
    const float dy = ( float )( y - gLastMouseY );
    gLastMouseX = x;
    gLastMouseY = y;

    if( gMouseLook )
    {
        camYaw += dx * camMouseSensitivity;
        camPitch += ( -dy ) * camMouseSensitivity; 

        if( camPitch > 89.0f ) camPitch = 89.0f;
        if( camPitch < -89.0f ) camPitch = -89.0f;

        updateCameraVectors();
        return;
    }

    if( gMousePan )
    {
        camPos += camRight * ( -dx * camPanSensitivity );
        camPos += camUp * ( dy * camPanSensitivity );
        return;
    }
}


// ================== RESIZE ==================
void rozmiar( int w, int h )
{
    screen_width = w;
    screen_height = h;

    glViewport( 0, 0, w, h );
    P = glm::perspective( glm::radians( 60.0f ),
        ( float )w / ( float )h, 1.0f, 1000.0f );
}

static void updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos( glm::radians( camYaw ) ) * cos( glm::radians( camPitch ) );
    front.y = sin( glm::radians( camPitch ) );
    front.z = sin( glm::radians( camYaw ) ) * cos( glm::radians( camPitch ) );
    camFront = glm::normalize( front );

    camRight = glm::normalize( glm::cross( camFront, glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
    camUp = glm::normalize( glm::cross( camRight, camFront ) );
}

void initFreeCamera( const glm::vec3& center, float distance )
{
    camPos = center + glm::vec3( 0.0f, 0.0f, distance );
    camYaw = -90.0f;   
    camPitch = 0.0f;
    updateCameraVectors();
}