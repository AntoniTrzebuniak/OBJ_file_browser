#include "LoadOBJ.h"

using namespace std;

std::vector<float> Gpositions;
std::vector<float> Gtexcoords;
std::vector<float> Gnormals;

// Funkcja pomocnicza do parsowania jednej sekcji f (np. "1/2/3" lub "1//3")
void parseFaceElement(const string& element, vector<int>& faces){
    int v = 0, t = 0, n = 0;
    size_t firstSlash = element.find('/');
    size_t lastSlash = element.find_last_of('/');

    v = stoi(element.substr(0, firstSlash));

    if (firstSlash != string::npos) {

        if (lastSlash > firstSlash + 1) {
            string middle = element.substr(firstSlash + 1, lastSlash - firstSlash - 1);
            if (!middle.empty()) t = stoi(middle);
        } 
        else if (firstSlash == lastSlash && firstSlash < element.length() - 1) {
            t = stoi(element.substr(firstSlash + 1));
        }
        if (lastSlash != firstSlash && lastSlash < element.length() - 1) {
            n = stoi(element.substr(lastSlash + 1));
        }
    }


    faces.push_back(v); // default 0
    faces.push_back(t); // default 0
    faces.push_back(n); // default 0
}

int loadOBJ(const string& filename, vector<Object>& scene) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: file did not open: " << filename << endl;
        return -1;
    }

    string line;
    Object* currObj = nullptr;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        float x, y, z, u, v, nx, ny, nz;

        stringstream ss(line);
        string prefix;
        ss >> prefix;
  
        if (prefix == "o" || prefix =="g") {
            Object newObj;
            ss >> newObj.Oname;
            scene.push_back(newObj);

            currObj = &scene.back();
        }
        if (prefix == "v") {
            ss >> x >> y >> z;
            Gpositions.push_back(x);
            Gpositions.push_back(y);
            Gpositions.push_back(z);
        } 
        else if (prefix == "vt") {
            ss >> u >> v;
            Gtexcoords.push_back(u);
            Gtexcoords.push_back(v);
        } 
        else if (prefix == "vn") {
            ss >> nx >> ny >> nz;
            Gnormals.push_back(nx);
            Gnormals.push_back(ny);
            Gnormals.push_back(nz);
        }
        else if (currObj != nullptr) {
            // Jeœli mamy aktywny obiekt, parsujemy dane
            if (prefix == "f") {
                string element;
                while (ss >> element) {
                    parseFaceElement(element, currObj->faces);
                }
            } 
            else if (prefix == "usemtl") {
                string matName;
                ss >> matName;
                currObj->materialsNames.push_back(matName);
            }
        }
    }
    // SUPER IMPORTANT - converting indexes

    return 0;
}
