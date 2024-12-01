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
    tree root = makeTree("/", 0, 0, "directory");
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
    delete[] magic;

    char name[9];
    bool mapDirectory = false;
    int mapElementsLeft = 0;
    for (int i = 0; i < numberOfDescriptors; i++) {
        tree currentDirectory = treeMap->find(treeStack->top())->second;

        uint32_t elementOffset;
        uint32_t elementLength;


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
            tree directoryObject = makeTree(file_name, (int)elementOffset, (int)elementLength, "directory");
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
            tree directoryObject = makeTree(nameString, (int)elementOffset, (int)elementLength, "directory");
            treeMap->insert(pair<string, tree>(directoryName, directoryObject));
            treeMap->find(treeStack->top())->second.children.push_back(directoryObject);
            treeStack->push(directoryName);
            mapElementsLeft = 11;
            mapDirectory = true;
        }
        //Everything here is a content
        else {
            directoryName += nameString;
            tree directoryObject = makeTree(nameString, (int)elementOffset, (int)elementLength, "file");
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

    }
    delete[] name;
    file.close();
}

string Wad::getMagic() {
    return Magic;
}

Wad::tree Wad::makeTree(string name, int offset, int length, string type) {
    tree treeObject;
    treeObject.name = name;
    treeObject.offset = offset;
    treeObject.length = length;
    treeObject.type = type;
    return treeObject;
}


bool Wad::isContent(const string &path) {
    vector<string> files = pathSeperator(path);
    string file = files[files.size() - 1];
        if (treeMap->find(path) != treeMap->end() && treeMap->find(path)->second.type == "file") {
            return true;
        }
        else {
            return false;
        }

    return false;

}

bool Wad::isDirectory(const string &path) {
    string cleanPath = endDashRemover(path);
    vector<string> files = pathSeperator(path);
    string file = files[files.size() - 1];

        if (treeMap->find(cleanPath) != treeMap->end() && treeMap->find(cleanPath)->second.type == "directory") {
            return true;
        }
        else {
            return false;
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
        if (offset > pathObject.length) {
            return 0;
        }
        file.open(filePath);
        file.seekg( (offset != 0) ? pathObject.offset + offset : pathObject.offset, ios::beg);
        file.read(buffer, length);
        file.close();
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
            beforeDirectory += files[i] + "/";
        }
        else if (i == (files.size() - 1)){
            newDirectory = files[i];
        }
    }

    beforeDirectory = endDashRemover(beforeDirectory);
    regex mapPattern("E\\dM\\d$");
    auto it = treeMap->find(beforeDirectory);
    if (it != treeMap->end()) {
        if (!(regex_search(beforeDirectory, mapPattern)) && !(regex_search(newDirectory, mapPattern)) && newDirectory.size() <= 2)
        {
            int position = endDescriptorFinder(beforeDirectory);
            descriptorAdder(position, newDirectory);
            tree newPathObject = makeTree(newDirectory, 0, 0, "directory");
            if (beforeDirectory == "/") {
                treeMap->insert(pair<string, tree>(beforeDirectory + newDirectory, newPathObject));
                treeMap->find("/")->second.children.push_back(newPathObject);
            }
            else {
                treeMap->insert(pair<string, tree>(beforeDirectory + "/" + newDirectory, newPathObject));
                treeMap->find(beforeDirectory)->second.children.push_back(newPathObject);
            }

        }
    }
}

void Wad::createFile(const string &path) {
    vector<string> files = pathSeperator(path);
    string beforeDirectory = "/";
    string beforeDirectoryName = "";
    string newFile;
    for (unsigned int i = 0; i < files.size(); i++) {
        if (i < (files.size() - 1))
        {
            beforeDirectory += files[i] + "/";
        }
        else if (i == (files.size() - 1)){
            newFile = files[i];
        }
    }

    beforeDirectory = endDashRemover(beforeDirectory);
    regex mapPattern("E\\dM\\d$");
    regex startPattern("_START$");
    regex endPattern("_END$");
    auto it = treeMap->find(beforeDirectory);
    if (it != treeMap->end()) {
        if (!(regex_search(beforeDirectory, mapPattern)) && !(regex_search(newFile, mapPattern)) && !(regex_search(newFile, startPattern)) && !(regex_search(newFile, endPattern)) && newFile.size() <= 8)
        {
            int position = endDescriptorFinder(beforeDirectory);
            fileAdder(position, newFile);
            tree newPathObject = makeTree(newFile, 0, 0, "file");
            if (beforeDirectory == "/") {
                treeMap->insert(pair<string, tree>(beforeDirectory + newFile, newPathObject));
                treeMap->find("/")->second.children.push_back(newPathObject);
            }
            else {
                treeMap->insert(pair<string, tree>(beforeDirectory + "/" + newFile, newPathObject));
                treeMap->find(beforeDirectory)->second.children.push_back(newPathObject);
            }
        }
    }
}

int Wad::writeToFile(const string &path, const char *buffer, int length, int offset) {
    string cleanPath = endDashRemover(path);

    if (isDirectory(path) || treeMap->find(path) == treeMap->end()) {
        return -1;
    }
    else if (treeMap->find(path)->second.length != 0)
    {
        return 0;
    }
    else {
        descriptorOffset += length;
        //Write the new file first
        file.open(filePath, std::ios::in | std::ios::binary);
        file.seekg(0, std::ios::end);
        streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        vector<char> filePartOne(12 + offset);
        file.read(filePartOne.data(), 12 + offset);
        vector<char> filePartTwo(fileSize - (12 + offset));
        file.read(filePartTwo.data(), fileSize - (12 + offset));\
        file.close();

        file.open(filePath, std::ios::out | std::ios::binary | ios::trunc);
        file.seekp(0, std::ios::beg);
        file.write(filePartOne.data(), filePartOne.size());
        file.write(buffer, length);
        file.write(filePartTwo.data(), filePartTwo.size());
        file.close();

        treeMap->find(path)->second.length = length;
        treeMap->find(path)->second.offset = 12 + offset;

        //Write the descriptor offset
        file.open(filePath, ios::in | ios::binary);
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        filePartOne = vector<char>(8);
        file.read(filePartOne.data(), 8);
        vector<char>dummy(4);
        file.read(dummy.data(), 4);
        filePartTwo = vector<char>(fileSize - 12);
        file.read(filePartTwo.data(), fileSize - 12);
        file.close();

        file.open(filePath, ios::out | ios::binary | ios::trunc);
        file.seekp(0, std::ios::beg);
        file.write(filePartOne.data(), filePartOne.size());
        file.write(reinterpret_cast<char * >(&descriptorOffset), dummy.size());
        file.write(filePartTwo.data(), filePartTwo.size());
        file.close();

        //Writing the descriptor things

        int position = endDescriptorFinder(path);
        file.open(filePath, ios::in | ios::binary);
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        if (position == fileSize) {
            position -= 16;
        }
        filePartOne = vector<char> (position);
        file.read(filePartOne.data(), position);
        dummy = vector<char> (4);
        file.read(dummy.data(), 4);
        vector<char> dummy1 = vector<char> (4);
        file.read(dummy1.data(), 4);
        filePartTwo = vector<char> (fileSize - (position + 8));
        file.read(filePartTwo.data(), fileSize - (position + 8));
        file.close();
        offset = 12 + offset;
        file.open(filePath, ios::out | ios::binary | ios::trunc);
        file.seekp(0, std::ios::beg);
        file.write(filePartOne.data(), filePartOne.size());
        file.write(reinterpret_cast<char *>(&offset), dummy.size());
        file.write(reinterpret_cast<char *>(&length), dummy.size());
        file.write(filePartTwo.data(), filePartTwo.size());
        file.close();
        return length;
    }
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

//Change its name
int Wad::endDescriptorFinder(const string &path) {
    string fileName;
    vector<string> files;
    if (path != "/") {
        files = pathSeperator(path);
        files.insert(files.begin(), "/");
        fileName = files[files.size() - 1];
    }
    else {
        file.open(filePath, ios_base::in | ios_base::binary | ios::ate);
        streamsize fileSize = file.tellg();
        file.close();
        return (int) fileSize;
    }
    file.open(filePath, ios_base::in | ios_base::binary | ios_base::out);
    file.seekg(0, std::ios::end);
    streamsize fileSize = file.tellg();


    int index = 0;
    int file_position = descriptorOffset;

    while (index < files.size()-1) {
        file.seekg(file_position, ios_base::beg);
            while (file.tellg() < fileSize) {
                char name[9];


                file.read(name, 4);
                file.read(name, 4);
                file.read(name, 8);

                string nameString = name;
                if (nameString == files[index+1] + "_START") {
                    int position = file.tellg();
                    file_position =  position; //16
                    index += 1;
                    break;
                }
                else if (nameString == files[index+1]) {
                    int position = file.tellg();
                    file_position =  position- 16; //16
                    index += 1;
                    file.close();
                    return file_position;
                }
            }
    }
    file.seekg(file_position, ios_base::beg);
    while (file.tellg() < fileSize) {
        char name[9];


        file.read(name, 4);
        file.read(name, 4);
        file.read(name, 8);

        string nameString = name;
        if (nameString == files[index] + "_END" || nameString == files[index]) {
            int position = file.tellg();
            file_position =  position - 16; //16
            index += 1;
            break;
        }
    }



    file.close();
    return file_position;
}

void Wad::descriptorAdder(int offset, string &name) {
    //check if the offset is same as the end of the file
    numberOfDescriptors += 2;
    file.open(filePath, ios_base::in | ios_base::binary | ios_base::out | ios::ate);
    streamsize fileSize = file.tellg();
    file.close();
    if (offset == fileSize) {
        file.open(filePath, ios_base::binary | ios_base::app | ios_base::out);
        file.seekp(0, ios_base::end);
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

        file.open(filePath, ios_base::in | ios_base::binary | ios_base::out);
        file.seekg(0, ios_base::end);
        streamsize newFileSize = file.tellg();
        file.seekg(0, ios_base::beg);
        vector<char> magicBytes(4);
        file.read(magicBytes.data(), 4);
        vector<char> dummy(4);
        file.read(dummy.data(), 4);
        vector<char> everythingElseBytes(newFileSize-8);
        file.read(everythingElseBytes.data(), newFileSize-8);
        file.close();


        file.open(filePath, ios_base::binary | ios_base::out | ios::trunc);
        file.write(magicBytes.data(), magicBytes.size());
        file.write(reinterpret_cast<char *>(&numberOfDescriptors), dummy.size());
        file.write(everythingElseBytes.data(), everythingElseBytes.size());
        file.close();

        delete[] beginningBuffer;
        delete[] endingBuffer;
    }
    else {
        file.open(filePath, ios_base::in | ios_base::binary | ios_base::out);
        file.seekg(0, ios_base::beg);

        vector<char> magicBytes(4);
        file.read(magicBytes.data(), 4);
        vector<char> dummy(4);
        file.read(dummy.data(), 4);
        vector<char> firstPartBuffer(offset-8);
        file.read(firstPartBuffer.data(), offset-8);
        vector<char> secondPartBuffer(fileSize - offset);
        file.read(secondPartBuffer.data(), fileSize - offset);

        file.close();

        file.open(filePath, ios::trunc | ios::binary | ios::out);
        file.seekp(0, ios_base::beg);
        file.write(magicBytes.data(), magicBytes.size());
        file.write(reinterpret_cast<char *>(&numberOfDescriptors), dummy.size());
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

        delete[] beginningBuffer;
        delete[] endingBuffer;

    }

}

void Wad::fileAdder(int offset, string &name) {
    //check if the offset is same as the end of the file
    numberOfDescriptors += 1;
    file.open(filePath, ios_base::in | ios_base::binary | ios_base::out | ios::ate);
    streamsize fileSize = file.tellg();
    file.close();
    if (offset == fileSize) {
        file.open(filePath, ios_base::binary | ios_base::app | ios_base::out);
        file.seekp(0, ios_base::end);
        int offset = 0;
        int position = 0;
        const size_t bufferSize = 8;
        char nameBuffer[bufferSize];

        memset(nameBuffer, 0, bufferSize);
        memcpy(nameBuffer, name.c_str(), min(name.size(), bufferSize));
        file.write(reinterpret_cast<char *>(&offset), sizeof(offset));
        file.write(reinterpret_cast<char *>(&position), sizeof(position));
        file.write(nameBuffer, bufferSize);
        file.close();

        file.open(filePath, ios_base::in | ios_base::binary | ios_base::out);
        file.seekg(0, ios_base::end);
        streamsize newFileSize = file.tellg();
        file.seekg(0, ios_base::beg);
        vector<char> magicBytes(4);
        file.read(magicBytes.data(), 4);
        vector<char> dummy(4);
        file.read(dummy.data(), 4);
        vector<char> everythingElseBytes(newFileSize-8);
        file.read(everythingElseBytes.data(), newFileSize-8);
        file.close();


        file.open(filePath, ios_base::binary | ios_base::out | ios::trunc);
        file.write(magicBytes.data(), magicBytes.size());
        file.write(reinterpret_cast<char *>(&numberOfDescriptors), dummy.size());
        file.write(everythingElseBytes.data(), everythingElseBytes.size());
        file.close();

        delete[] nameBuffer;
    }
    else {
        file.open(filePath, ios_base::in | ios_base::binary | ios_base::out);
        file.seekg(0, ios_base::beg);

        vector<char> magicBytes(4);
        file.read(magicBytes.data(), 4);
        vector<char> dummy(4);
        file.read(dummy.data(), 4);
        vector<char> firstPartBuffer(offset-8);
        file.read(firstPartBuffer.data(), offset-8);
        vector<char> secondPartBuffer(fileSize - offset);
        file.read(secondPartBuffer.data(), fileSize - offset);


        file.close();

        file.open(filePath, ios::trunc | ios::binary | ios::out);
        file.seekp(0, ios_base::beg);
        file.write(magicBytes.data(), magicBytes.size());
        file.write(reinterpret_cast<char *>(&numberOfDescriptors), dummy.size());
        file.write(firstPartBuffer.data(), firstPartBuffer.size());

        int offset1 = 0;
        int position = 0;
        const size_t bufferSize = 8;
        char nameBuffer[bufferSize];

        memset(nameBuffer, 0, bufferSize);
        memcpy(nameBuffer, name.c_str(), min(name.size(), bufferSize));
        file.write(reinterpret_cast<char *>(&offset1), sizeof(offset1));
        file.write(reinterpret_cast<char *>(&position), sizeof(position));
        file.write(nameBuffer, bufferSize);

        file.write(secondPartBuffer.data(), secondPartBuffer.size());
        file.close();

        delete[] nameBuffer;
    }
}






