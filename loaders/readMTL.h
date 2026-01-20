#ifndef READMTL_H
#define READMTL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct Mtl {
    string name;
    float Ka[3], Kd[3], Ks[3];
    float Ns, d;
    string texturePath;
    bool hasText;
    Mtl() : Ns(0.0f), d(1.0f), hasText(1), texturePath("") {    // ustawienie wartoœci domyœlnych gdyby nie pojawi³y siê w pliku
        for(int i=0; i<3; ++i) {
            Ka[i] = 0.0f; Kd[i] = 1.0f; Ks[i] = 0.0f;
        }
    }
};

int readMTL(const string& filename, unordered_map<string, Mtl>& materialMap);

#endif