//#define WORKING_DIR "C:/Users/micha/Desktop/studia/sem5/grafika/projekt3/trees/"
#define OBJ_FILE  "./trees9.obj"
#define MTL_FILE "./trees9.mtl"
#define WORKING_DIR "./obj_container/"
//#define OBJ_FILE  "./Rendersss.obj"
//#define MTL_FILE "./Rendersss.mtl"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>
#include <iostream>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <commdlg.h>
#endif

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "shaderLoader.h"
#include "tekstura.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "loaders/LoadOBJ.h"
#include "loaders/readMTL.h"

#include "utility.h"
#include "convertOBJ.h"
#include "GPUservice.h"

#ifdef _WIN32
static std::string BrowseForFileA(const char* filter, const char* initialDir)
{
    CHAR szFile[MAX_PATH] = { 0 };
    OPENFILENAMEA ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    std::string initialAbs;
    if (initialDir && initialDir[0] != '\0') {
        CHAR fullPath[MAX_PATH] = { 0 };
        DWORD ret = GetFullPathNameA(initialDir, MAX_PATH, fullPath, nullptr);
        if (ret && ret < MAX_PATH) initialAbs = fullPath;
        else initialAbs = initialDir; // fallback
    }

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = initialAbs.c_str();
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_EXPLORER;
    if (GetOpenFileNameA(&ofn))
        return std::string(szFile);
    return std::string();
}
#endif

void printMatNames(vector<Object>& scene);
void rysuj(void);



vector<Object> scene;
unordered_map<string, Mtl> matsy;

GLint uniformTex0 = 0;
GLint uniformTex1 = 0;
//GLint uniformRenderMode = -1;

GLuint programID = 0;
vector<GPUmesh> Meshes;

GLint uKaLoc;
GLint uKdLoc;
GLint uKsLoc;
GLint uNsLoc;
GLint uAlphaLoc;
GLint uLightPosLoc;
GLint uViewPosLoc;
GLint locHasTex;
GLint uniformRenderMode;
GLint uLocCol;

// ===== MACIERZE =====
glm::mat4 MV = glm::mat4(1.0f);
glm::mat4 P = glm::mat4(1.0f);

vector<Vertex> gVertices;

constexpr float fovY = glm::radians(45.0f);
float aspect = (float)screen_width / (float)screen_height;
glm::vec3 sceneCenter = glm::vec3(0.0f);
float sceneDistance = 5.0f;
const float BGcol[4] = { 0.8f, 0.8f, 0.8f, 1 };  // BackGround Color

// ================== MAIN ==================
int main(int argc, char** argv)
{

    string startDir = WORKING_DIR;
    // Filters must be double-null terminated for the Win32 API.
#ifdef _WIN32
    const char objFilter[] = "OBJ files (*.obj)\0*.obj\0All files (*.*)\0*.*\0\0";
    const char mtlFilter[] = "MTL files (*.mtl)\0*.mtl\0All files (*.*)\0*.*\0\0";
    
    cout<<"wybierz plik OBJ..."<<endl;
    string selectedOBJ = BrowseForFileA(objFilter, startDir.c_str());
    cout<<"wybrano plik obj: " << selectedOBJ;
    cout<<"wybierz towarzyszący plik MTL..."<<endl;
    string selectedMTL = BrowseForFileA(mtlFilter, startDir.c_str());
    cout<<"wybrano plik mtl: " << selectedMTL;
#else
    string selectedOBJ;
    string selectedMTL;
#endif

    // Fallback to the original macro-based defaults if the user cancelled selection or selection failed.
    const std::string fallbackOBJ = std::string(WORKING_DIR) + OBJ_FILE;
    const std::string fallbackMTL = std::string(WORKING_DIR) + MTL_FILE;
    
    const std::string OBJfile = selectedOBJ.empty() ? fallbackOBJ : selectedOBJ;
    const std::string MTLfile = selectedMTL.empty() ? fallbackMTL : selectedMTL;

    std::cout << "OBJ file: " << OBJfile << std::endl;
    std::cout << "MTL file: " << MTLfile << std::endl;
    //====================== CZYTANIE PLIKOW =================================================================
    //const string OBJfile = WORKING_DIR  OBJ_FILE;
    //const string MTLfile = WORKING_DIR  MTL_FILE;


    if (loadOBJ(OBJfile, scene) != 0 || readMTL(MTLfile, matsy) != 0) {
        printf("An error during reading files!");
        return -1;
    }

    cout << "Wczytano obiektow: " << scene.size() << endl;
    fixmaterialnames( scene );
    printMatNames(scene);
    cout << matsy[scene[0].materialsNames[0]].texturePath;

    // ================== Środek Sceny  =================================
    computeSceneBoundsAndDistance(Gpositions, sceneCenter, sceneDistance, fovY, aspect, 1.15f);

    // ==================== PROGRAM GRAFICZNY ===================================
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(screen_width, screen_height);
    glutCreateWindow("Podstawa + kamera myszą");

    glewInit();
    P = glm::perspective(glm::radians(45.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
    glViewport(0, 0, screen_width, screen_height);

    glutDisplayFunc(rysuj);
    glutReshapeFunc(rozmiar);
    glutMouseFunc(mysz);
    glutMotionFunc(mysz_ruch);
    glutIdleFunc(rysuj);
    glutKeyboardFunc( klawisz );
    glEnable(GL_DEPTH_TEST);


    unordered_map<string, GLuint> textureCache;
    Meshes.reserve(scene.size());
    for(int i = 0; i<scene.size(); i++){
        Object& o = scene[i];
        Shape s;
        s.materialsNames = o.materialsNames;
        convertOBJToMeshIndexed( Gpositions, Gtexcoords, Gnormals, o.faces, s.Vertices, s.Indices);

        GPUmesh m(matsy[o.materialsNames[0]]);
        createMesh(s,m);
        m.material.hasText = m.material.texturePath!="" ? 1 : 0;
        Meshes.push_back(m);
        cout<< "TEXT PATH: " << m.material.texturePath << endl;
        cout << "TEXT PATH:" << m.material.name << endl;


    }
    //============ ZWALNIANIE PAMIĘCI ==================
    vector<float>().swap(Gpositions);
    vector<float>().swap(Gtexcoords);
    vector<float>().swap(Gnormals);
    vector<Object>().swap(scene);


    programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");
    uniformTex0 = glGetUniformLocation(programID, "tex0");
    uniformRenderMode = glGetUniformLocation(programID, "uRenderMode");
    uKaLoc = glGetUniformLocation(programID, "uKa");
    uKdLoc = glGetUniformLocation(programID, "uKd");
    uKsLoc = glGetUniformLocation(programID, "uKs");
    uNsLoc = glGetUniformLocation(programID, "uNs");
    uAlphaLoc = glGetUniformLocation(programID, "uAlpha");
    uLightPosLoc = glGetUniformLocation(programID, "uLightPos");
    uViewPosLoc = glGetUniformLocation(programID, "uViewPos");  
    locHasTex = glGetUniformLocation(programID, "uHasTexture");
    uLocCol = glGetUniformLocation(programID, "uCol");
    


    //glBindBuffer(GL_ARRAY_BUFFER, VBOtex);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glutMainLoop();
    return 0;
}



void rysuj(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(programID);
    //******************

    // ===== MTL → SHADER =====

    // ===== ŚWIATŁO =====
    glm::vec3 lightPos(5.0f, 6.0f, 5.0f);
    if (uLightPosLoc != -1)
        glUniform3fv(uLightPosLoc, 1, &lightPos[0]);

    // ===== KAMERA =====
    glm::vec3 viewPos( sceneCenter.x, sceneCenter.y, sceneDistance);
    if (uViewPosLoc != -1)
        glUniform3fv(uViewPosLoc, 1, &viewPos[0]);

    //******************

    GLuint MVP_id = glGetUniformLocation(programID, "MVP");

    MV = glm::mat4(1.0f);
    //MV = glm::translate(MV, glm::vec3(0.0f, 0.0f, -sceneDistance + kameraD));
    MV = glm::translate(MV, glm::vec3((float)kameraLR, 0.0f, -sceneDistance + (float)kameraD));

    MV = glm::rotate(MV, glm::radians((float)kameraZ), glm::vec3(1, 0, 0));
    MV = glm::rotate(MV, glm::radians((float)kameraX), glm::vec3(0, 1, 0));
    MV = glm::translate(MV, -sceneCenter);
    glm::mat4 MVP = P * MV;
    if (MVP_id != -1)
        glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &MVP[0][0]);

    GLuint MV_id = glGetUniformLocation(programID, "MV");
    if (MV_id != -1)
        glUniformMatrix4fv(MV_id, 1, GL_FALSE, &MV[0][0]);

    glClearBufferfv(GL_COLOR, 0, BGcol); // zmiana koloru tła 

    for(int i = 0; i<Meshes.size(); i++){   
        const GPUmesh& mesh = Meshes[i];
        const Mtl& mat = mesh.material;

        if (uKaLoc != -1)    glUniform3fv( uKaLoc, 1, mat.Ka );
        if (uKdLoc != -1)    glUniform3fv( uKdLoc, 1, mat.Kd );
        if (uKsLoc != -1)    glUniform3fv( uKsLoc, 1, mat.Ks );
        if (uNsLoc != -1)    glUniform1f( uNsLoc, mat.Ns );
        if (uAlphaLoc != -1) glUniform1f( uAlphaLoc, mat.d );
        if (locHasTex != -1) glUniform1i(locHasTex, b.texture & mat.hasText);
        if (locHasTex != -1) glUniform3f(uLocCol, SolidCol.r, SolidCol.g, SolidCol.b );
        
        glBindVertexArray(mesh.VAO);
        int modifier = 0;
        if(uniformRenderMode!=-1) glUniform1i(uniformRenderMode, b.singleCol);

        if(b.bryla){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,mesh.textureID);
            if(uniformTex0!=-1) glUniform1i(uniformTex0,0);

            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glDrawElements(GL_TRIANGLES,Meshes[i].indexCount,GL_UNSIGNED_INT,0);
        }
        if(b.vertex){
            glPointSize(2.f);
            glDrawElements(GL_POINTS, Meshes[i].indexCount,GL_UNSIGNED_INT, 0);
        }
        if(b.lines){
            glPointSize(2.f);
            glDrawElements(GL_LINES, Meshes[i].indexCount,GL_UNSIGNED_INT, 0);
        }
        
    }


    // Restore fill mode (optional)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glutSwapBuffers();
}

void printMatNames(vector<Object>& scene) {
    for (unsigned i = 0; i < scene.size(); i++) {
        cout << "\nObject: " << scene[i].Oname;
        for (const auto& name : scene[i].materialsNames) {
            cout << "\nmaterial name: ";
            cout << scene[i].materialsNames[0];
        }
    }
}

