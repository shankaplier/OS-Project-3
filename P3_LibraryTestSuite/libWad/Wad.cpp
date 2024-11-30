//
// Created by Shashank Gutta on 11/28/24.
//

#include "Wad.h"
#include <regex>

Wad* Wad::loadWad(const string &path) {
    return new Wad(path);
}

Wad::Wad(const string &path) {
    filePath = path;
    //Creates new fstream
    file.open(path, ios::in | ios::binary | ios::out);

    //Creating variables to hold various values for initializing the tree
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
    file.close();
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
    string file = endDashRemover(path);

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
    // cout <<result[0] << endl;
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
        if (lastChar == '/' && longfile.size() > 1) {
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

    vector<string> files = pathSeperator(path);
    string beforeDirectory = "/";
    string beforeDirectoryName = "";
    string newDirectory;
    for (unsigned int i = 0; i < files.size(); i++) {
        if (i < (files.size() - 1))
        {
            beforeDirectory += files[i];
        }
        else if (i == (files.size() - 1)){
            newDirectory = files[i];
        }
    }

    // for (auto it = treeMap->begin(); it != treeMap->end(); it++) {
    //     cout << it->first << endl;
    // }

    regex mapPattern("E\\dM\\d$");
    // cout << "BeforeDirectory: " << beforeDirectory << endl;
    auto it = treeMap->find(beforeDirectory);
    if (it != treeMap->end()) {
        if (!(regex_search(beforeDirectory, mapPattern)))
        {
            int position = endDescriptorFinder(beforeDirectory);
            // cout << "The position is: " << position << endl;
            descriptorAdder(position, newDirectory);
        }
        // else {
        //     cout << "false" << endl;
        // }

    }
    // for (auto it = treeMap->begin(); it != treeMap->end(); it++) {
    //     cout << it->first << endl;
    // }

    // cout << "newDirectory: " << newDirectory << endl;
}

void Wad::createFile(const string &path) {
    cout << "Creating file " << path << endl;
}

int Wad::writeToFile(const string &path, const char *buffer, int length, int offset) {
    return -1;
}

string Wad::endDashRemover(string const &path) {
    string longfile = path;

    if (!path.empty() && path.size() != 1) {
        char lastChar = longfile[longfile.size() - 1];
        if (lastChar == '/' && longfile.size() > 1) {
            longfile.pop_back();
        }
    }
    return longfile;
}

int Wad::endDescriptorFinder(const string &path) {
    string fileName;
    if (path != "/") {
        vector<string> files = pathSeperator(path);
        fileName = files[files.size() - 1];
    }
    else {
        file.open(filePath, ios_base::in | ios_base::binary | ios::ate);
        streamsize fileSize = file.tellg();
        file.close();
        return (int) fileSize;
    }
    file.open(filePath, ios_base::in | ios_base::binary | ios_base::out);
    file.seekg(descriptorOffset, ios_base::beg);

    // cout << "fileName: " << fileName << endl;
    for (int i = 0; i < numberOfDescriptors; i++) {

        char name[9];


        file.read(name, 4);
        file.read(name, 4);
        file.read(name, 8);


        string nameString = name;
        if (nameString == fileName + "_END") {
            int position = file.tellg();
            file.close();
            return position - 16; //16
        }
    }

}

void Wad::descriptorAdder(int offset, string &name) {
    //check if the offset is same as the end of the file
    numberOfDescriptors += 2;
    file.open(filePath, ios_base::in | ios_base::binary | ios_base::out | ios::ate);
    // if (!file) {
    //     cout << "File open error" << endl;
    // }
    streamsize fileSize = file.tellg();
    // cout << "The filesize is: " << fileSize << endl;
    file.close();
    if (offset == fileSize) {
        file.open(filePath, ios_base::binary | ios_base::app | ios_base::out);
        file.seekp(0, ios_base::end);
        cout << "true" << endl;
        int offset = 0;
        int position = 0;
        const size_t bufferSize = 8;
        char beginningBuffer[bufferSize];
        char endingBuffer[bufferSize];
        string beginningName = name + "_START";
        string endName =  name +"_END";

        memset(beginningBuffer, 0, bufferSize);
        memset(endingBuffer, 0, bufferSize);
        memcpy(beginningBuffer, beginningName.c_str(), min(beginningName.size(), bufferSize));
        memcpy(endingBuffer, endName.c_str(), min(endName.size(), bufferSize));
        file.write(reinterpret_cast<char *>(&offset), sizeof(offset));
        file.write(reinterpret_cast<char *>(&position), sizeof(position));
        file.write(beginningBuffer, bufferSize);
        file.write(reinterpret_cast<char *>(&offset), sizeof(offset));
        file.write(reinterpret_cast<char *>(&position), sizeof(position));
        file.write(endingBuffer, bufferSize);
        file.close();
    }
    else {
        file.open(filePath, ios_base::in | ios_base::binary | ios_base::out);
        file.seekg(0, ios_base::beg);

        vector<char> firstPartBuffer(offset);
        file.read(firstPartBuffer.data(), offset);
        // for (unsigned int i = 0; i < offset; i++) {
        //     cout << firstPartBuffer[i] << endl;
        // }
        vector<char> secondPartBuffer(fileSize - offset);
        file.read(secondPartBuffer.data(), fileSize - offset);
        // for (unsigned int i = 0; i < fileSize-offset; i++) {
        //     cout << secondPartBuffer[i] << endl;
        // }


        file.close();

        file.open(filePath, ios::trunc | ios::binary | ios::out);
        file.seekp(0, ios_base::beg);
        file.write(firstPartBuffer.data(), firstPartBuffer.size());


        int offset1 = 0;
        int position = 0;
        const size_t bufferSize = 8;
        char beginningBuffer[bufferSize];
        char endingBuffer[bufferSize];
        string beginningName = name + "_START";
        string endName =  name +"_END";

        memset(beginningBuffer, 0, bufferSize);
        memset(endingBuffer, 0, bufferSize);
        // min(beginningName.size(), bufferSize)
        memcpy(beginningBuffer, beginningName.c_str(), bufferSize);
        memcpy(endingBuffer, endName.c_str(),  bufferSize);
        file.write(reinterpret_cast<char *>(&offset1), sizeof(offset1));
        file.write(reinterpret_cast<char *>(&position), sizeof(position));
        file.write(beginningBuffer, bufferSize);
        file.write(reinterpret_cast<char *>(&offset1), sizeof(offset1));
        file.write(reinterpret_cast<char *>(&position), sizeof(position));
        file.write(endingBuffer, bufferSize);

        file.write(secondPartBuffer.data(), secondPartBuffer.size());
        file.close();


    }

    // file.open(filePath, ios_base::in | ios_base::binary | ios_base::out);
    // file.seekg(descriptorOffset, ios_base::beg);
    // for (int i = 0; i < numberOfDescriptors; i++) {
    //     char name[9];
    //     int num;
    //
    //     file.read((char*)&num, 4);
    //     file.read((char*)&num, 4);
    //     file.read(name, 8);
    //
    //     cout << num << endl;
    //     cout << num << endl;
    //     cout << "The name is: " << name << endl;
    //
    // }
    // file.close();

}













// bool Wad::isContent(const string &path) {
//
// }





