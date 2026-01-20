#ifndef LOADOBJ_H
#define LOADOBJ_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


extern std::vector<float> Gpositions;
extern std::vector<float> Gtexcoords;
extern std::vector<float> Gnormals;

struct Object {
    string Oname;
    vector<int> faces;
    vector<string> materialsNames;
};
int loadOBJ(const string& filename, vector<Object>& scene);

static void parseFaceElement(const string& element, vector<int>& faces);

void fixmaterialnames(vector<Object>& scene);

#endif