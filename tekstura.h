#include <stdio.h>
#include <windows.h>


#include "gl/glew.h"
#include "GL/freeglut.h"
#include <gl\gl.h> 
#include <stdlib.h>
#include <string.h>
#include "GPUservice.h"

#ifndef WORKING_DIR
#define WORKING_DIR "./obj_container/"
#endif

#define LOAD_BMP(path) ( WczytajTeksture( (std::string(WORKING_DIR) + (path)).c_str() ) )
#define LOAD_JPG(path) ( WczytajTekstureJPG( (std::string(WORKING_DIR) + (path)).c_str() ) )
#define LOAD_PNG(path) ( WczytajTeksturePNG( (std::string(WORKING_DIR) + (path)).c_str() ) )

enum class ImageExt {
    PNG,
    JPG,
    JPEG,
    BMP,
    UNKNOWN
};



GLuint WczytajTeksture(const char *filename);
GLuint WczytajTekstureJPG(const char* filename);
GLuint WczytajTeksturePNG(const char* filename);

void addText2Mesh(GPUmesh& m);
GLuint LoadTextureAuto(const std::string& path);
ImageExt getExtension(const std::string& filename);