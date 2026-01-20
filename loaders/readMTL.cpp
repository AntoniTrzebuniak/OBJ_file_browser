#include "readMTL.h"

#include <algorithm>

int readMTL(const string& filename, unordered_map<string, Mtl>& materialMap){
    ifstream file(filename);

    if (!file.is_open()) {
        // Rzucamy wyj¹tek, aby przerwaæ program w przypadku b³êdu
        cerr << "Krytyczny blad: Nie udalo sie otworzyc pliku MTL: " << filename;
        return -1;
    }

    string line;
    Mtl* currentMtl = nullptr;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        stringstream ss(line);
        string prefix;
        ss >> prefix;

        if (prefix == "newmtl") {
            string mtlName;
            ss >> mtlName;

            // Tworzymy nowy materia³ w mapie i pobieramy wskaŸnik do niego
            materialMap[mtlName] = Mtl();
            currentMtl = &materialMap[mtlName];
            currentMtl->name = mtlName;
        } 
        else if (currentMtl) {
            if (prefix == "Ka") {
                ss >> currentMtl->Ka[0] >> currentMtl->Ka[1] >> currentMtl->Ka[2];
            } else if (prefix == "Kd") {
                ss >> currentMtl->Kd[0] >> currentMtl->Kd[1] >> currentMtl->Kd[2];
            } else if (prefix == "Ks") {
                ss >> currentMtl->Ks[0] >> currentMtl->Ks[1] >> currentMtl->Ks[2];
            } else if (prefix == "Ns") {
                ss >> currentMtl->Ns;
            } else if (prefix == "d") {
                ss >> currentMtl->d;
            } else if (prefix == "map_Kd") {
                // Read the rest of the line into a std::string, then convert to char*
                string path;
                getline(ss, path); // reads remaining characters (may include leading space)

                // Oczyszczanie œcie¿ki z bia³ych znaków
                size_t first = path.find_first_not_of(" \t");
                if (first != string::npos) {
                    path = path.substr(first);
                } else {
                    path.clear(); // no non-whitespace found
                }
                // Trim trailing whitespace
                size_t last = path.find_last_not_of(" \t\r\n");
                if (last != string::npos) {
                    path = path.substr(0, last + 1);
                }

                // Replace backslashes '\' with forward slashes '/'
                std::string target = "\\\\";
                std::string replacement = "/";

                size_t pos = path.find(target);
                while (pos != std::string::npos) {
                    path.replace(pos,target.length(), replacement );
                    pos = path.find(target, pos+replacement.length());
                }
                currentMtl->texturePath = path;

                cout<<currentMtl->texturePath << endl;
            }
        }
    }

    file.close();
    return 0;
}