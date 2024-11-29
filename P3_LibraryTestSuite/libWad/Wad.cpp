//
// Created by Shashank Gutta on 11/28/24.
//

#include "Wad.h"
#include <regex>

Wad* Wad::loadWad(const string &path) {
    return new Wad(path);
}

Wad::Wad(const string &path) {
    //Creates new fstream
    file.open(path, ios::in | ios::binary | ios::out);

    //Creating variables to hold various values for initializing the tree
    char magic[5];
    uint32_t numberOfDescriptors;
    uint32_t descriptorOffset;
    tree root = makeTree("/", 0, 0);
    treeMap = new map<string, tree>;
    treeMap->insert(pair<string, tree>("/", root));
    treeStack = new stack<string>;
    treeStack->push("/");

    //Reading things into the variables to go through the descriptor list
    file.read(magic, 4);
    file.read((char*)&numberOfDescriptors, 4);
    file.read((char*)&descriptorOffset, 4);

    //Changing the pointer of file to point to the start of descriptor
    file.seekg(descriptorOffset, ios::beg);

    //Assigning the read magic value to a Magic attribute on Wad for the method getMagic
    Magic = magic;

    // cout << "Magic" << magic << endl;
    // cout << "Number Of Descriptors" << numberOfDescriptors << endl;
    // cout << "Descriptor Offset" << descriptorOffset << endl;

    bool mapDirectory = false;
    int mapElementsLeft = 0;
    for (int i = 0; i < numberOfDescriptors; i++) {
        tree currentDirectory = treeMap->find(treeStack->top())->second;

        uint32_t elementOffset;
        uint32_t elementLength;
        char name[9];

        file.read((char*)&elementOffset, 4);
        file.read((char*)&elementLength, 4);
        file.read(name, 8);

        string nameString = name;
        string directoryName;

        if (treeStack->top() == "/") {
            directoryName = "/";
        }
        else {
            directoryName = treeStack->top() + "/";
        }


        regex startPattern("_START$");
        regex endPattern("_END$");
        regex mapPattern("^E\\dM\\d$");
        regex contentPattern("\\.[a-zA-Z0-9.]+$");

        //Check if the name is the beginning namespace directory
        if (regex_search(nameString, startPattern)) {
            string file_name = nameString.substr(0, nameString.find("_START"));
            directoryName += file_name;
            tree directoryObject = makeTree(file_name, (int)elementOffset, (int)elementLength);
            treeMap->find(treeStack->top())->second.children.push_back(directoryObject);
            treeMap->insert(pair<string, tree>(directoryName, directoryObject));
            treeStack->push(directoryName);
        }
        //Check if the name is the ending namespace directory
        else if (regex_search(nameString, endPattern)) {
            treeStack->pop();
        }
        //Check if the name is a map directory
        else if (regex_search(nameString, mapPattern)) {
            directoryName += nameString;
            tree directoryObject = makeTree(nameString, (int)elementOffset, (int)elementLength);
            treeMap->insert(pair<string, tree>(directoryName, directoryObject));
            treeMap->find(treeStack->top())->second.children.push_back(directoryObject);
            treeStack->push(directoryName);
            mapElementsLeft = 11;
            mapDirectory = true;
        }
        //Everything here is a content
        else if (regex_search(nameString, contentPattern)){
            directoryName += nameString;
            tree directoryObject = makeTree(nameString, (int)elementOffset, (int)elementLength);
            treeMap->find(treeStack->top())->second.children.push_back(directoryObject);
            treeMap->insert(pair<string, tree>(directoryName, directoryObject));
        }

        if (mapElementsLeft > 0 && mapDirectory) {
            mapElementsLeft -= 1;
            if (mapElementsLeft == 0) {
                mapDirectory = false;
                treeStack->pop();
            }
        }
        // cout << "element Offset: " << elementOffset << endl;
        // cout << "element Length: " << elementLength << endl;
        // cout << "name: " << name << endl;

    }
}

string Wad::getMagic() {
    return Magic;
}

struct Wad::tree Wad::makeTree(string name, int offset, int length) {
    Wad::tree treeObject;
    treeObject.name = name;
    treeObject.offset = offset;
    treeObject.length = length;
    return treeObject;
}

bool Wad::isContent(const string &path) {
    vector<string> files = pathSeperator(path);
    string file = files[files.size() - 1];
    regex contentPattern("\\.[a-zA-Z0-9.]+$");
    if (regex_search(file, contentPattern)) {
        if (treeMap->find(path) != treeMap->end()) {
            return true;
        }
        else {
            return false;
        }
    }

    return false;

}

bool Wad::isDirectory(const string &path) {
    vector<string> files = pathSeperator(path);
    string file = files[files.size() - 1];

    regex contentPattern("\\.[a-zA-Z0-9.]+$");
    if (regex_search(file, contentPattern)) {
        return false;
    }
    else {
        if (treeMap->find(path) != treeMap->end()) {
            return true;
        }
        else {
            return false;
        }
    }
}

vector<string> Wad::pathSeperator(const string &path) {
    string file = path;

    vector<string> result;
    int res = 0;
    int prev_num;
    if (file.size() > 0) {
        prev_num = 1;
    }
    else {
     prev_num = 0;
    }
    string name;
    while ((res = file.find("/", prev_num)) !=
            string::npos) {
        result.push_back(file.substr(prev_num, (res-prev_num) < 0 ? prev_num - res : res - prev_num));
        prev_num = res+1;
    }
    result.push_back(file.substr(prev_num, res-10));
    return result;
}

int Wad::getSize(const string &path) {
    if (isContent(path)) {
        tree pathObject = treeMap->find(path)->second;
        return pathObject.length;
    }
    return -1;
}

int Wad::getContents(const string &path, char *buffer, int length, int offset) {
    if (isContent(path)) {
        tree pathObject = treeMap->find(path)->second;
        file.seekg( (offset != 0) ? pathObject.offset + offset : pathObject.offset, ios::beg);
        file.read(buffer, length);
        if (offset > pathObject.length) {
            return 0;
        }
        return (length < pathObject.length) ? length : pathObject.length - offset;

    }
    return -1;
}

int Wad::getDirectory(const string &path, vector<string> *directory) {
    string longfile = path;

    if (!path.empty()) {
        char lastChar = longfile[longfile.size() - 1];
        if (lastChar == '/') {
            longfile.pop_back();
        }
    }

    if (isDirectory(longfile)) {
        tree pathObject = treeMap->find(longfile)->second;
        for (unsigned int i = 0; i < pathObject.children.size(); i++) {
            directory->push_back(pathObject.children[i].name);
        }
        return pathObject.children.size();
    }
    return -1;
}

void Wad::createDirectory(const string &path) {
    cout << "Creating directory " << path << endl;
}

void Wad::createFile(const string &path) {
    cout << "Creating file " << path << endl;
}

int Wad::writeToFile(const string &path, const char *buffer, int length, int offset) {
    return -1;
}










// bool Wad::isContent(const string &path) {
//
// }





