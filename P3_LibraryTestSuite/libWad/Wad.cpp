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

    // cout << Magic << endl;
    // cout << "Number of descriptors: " << numberOfDescriptors << endl;
    // cout << "Descriptor offset: " << descriptorOffset << endl;

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


        // cout << "element offset: " << elementOffset << endl;
        // cout << "element length: " << elementLength << endl;


        string nameString = name;

        // cout << nameString << endl;
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
    // for (auto it = treeMap->begin(); it != treeMap->end(); it++) {
    //     cout << it->first << endl;
    // }
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
        // cout << "The children names are: " << endl;
        for (unsigned int i = 0; i < pathObject.children.size(); i++) {
            // cout << pathObject.children[i].name << endl;
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
    // cout << "beforeDirectory: " << beforeDirectory << endl;
    // for (auto it = treeMap->begin(); it != treeMap->end(); it++) {
    //     cout << it->first << endl;
    // }
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
        // cout << descriptorOffset << endl;

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

        // file.open(filePath, std::ios::in | std::ios::binary);
        // vector<char> testing(57);
        // file.seekg(12, std::ios::beg);
        // file.read(testing.data(), 57);
        // for (unsigned int i = 0; i < testing.size(); i++) {
        //     cout << testing[i] << endl;
        // }
        // file.close();

        treeMap->find(path)->second.length = length;
        treeMap->find(path)->second.offset = 12 + offset;
        // cout << treeMap->find(path)->second.offset << endl;

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

        // file.open(filePath, std::ios::in | std::ios::binary);
        // vector<char> testing(57);
        // file.seekg(12, std::ios::beg);
        // file.read(testing.data(), 57);
        // for (unsigned int i = 0; i < testing.size(); i++) {
        //     cout << testing[i] << endl;
        // }
        // file.close();

        //testing
        // file.open(filePath, ios::in | ios::binary);
        // file.seekg(8, std::ios::beg);
        // int test;
        // file.read(reinterpret_cast<char *>(&test), 4);
        // cout << test << endl;
        // file.close();

        //Writing the descriptor things
        // cout << "calling endDFinder" << endl;
        int position = endDescriptorFinder(path);
        file.open(filePath, ios::in | ios::binary);
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        // cout << "The position is: " << position << endl;
        // cout << "The size is: " << fileSize << endl;
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

        // file.open(filePath, ios::in | ios::binary);
        // file.seekg(position, std::ios::beg);
        // int test1;
        // int test2;
        // file.read(reinterpret_cast<char *>(&test1), 4);
        // file.read(reinterpret_cast<char *>(&test2), 4);
        // cout << "The printed test1 is: " << test1 << endl;
        // cout << "The printed test2 is: " << test2 << endl;
        // file.close();

        // file.open(filePath, std::ios::in | std::ios::binary);
        // vector<char> testing(57);
        // file.seekg(12, std::ios::beg);
        // file.read(testing.data(), 57);
        // for (unsigned int i = 0; i < testing.size(); i++) {
        //     cout << testing[i] << endl;
        // }
        // file.close();

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
        // for (unsigned int i = 0; i < files.size(); i++) {
        //     cout << files[i] << endl;
        // }
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

    // cout << "Number of files to look through: " << files.size() << endl;
    while (index < files.size()-1) {
        // cout << "The array size is: " << files.size() << endl;
        // cout << "Current Index: " << index << endl;
        // cout << "Current File: " << files[index] << endl;
        // cout << "Next file: " << files[index+1] << endl;
        file.seekg(file_position, ios_base::beg);
            while (file.tellg() < fileSize) {
                char name[9];


                file.read(name, 4);
                file.read(name, 4);
                file.read(name, 8);

                string nameString = name;
                // cout << nameString << endl;
                if (nameString == files[index+1] + "_START" || nameString == files[index+1]) {
                    int position = file.tellg();
                    file_position =  position; //16
                    // cout << "File " << files[index+1] << " found at position " << file_position << endl;
                    index += 1;
                    break;
                }
                // cout << "The current posiiton is: " << file.tellg() << endl;
                // cout << "The filesize is: " << fileSize << endl;
                // cout << "End of loop has been reached" << endl;
            }
        // cout << "Loop has been exited" << endl;
    }
    file.seekg(file_position, ios_base::beg);
    // cout << "The array size is: " << files.size() << endl;
    // cout << "Current Index: " << index << endl;
    // cout << "Current File: " << files[index] << endl;
    // cout << "Next file: " << files[index+1] << endl;
    while (file.tellg() < fileSize) {
        char name[9];


        file.read(name, 4);
        file.read(name, 4);
        file.read(name, 8);

        string nameString = name;
        // cout << nameString << endl;
        if (nameString == files[index] + "_END" || nameString == files[index]) {
            int position = file.tellg();
            file_position =  position - 16; //16
            // cout << "File " << files[index+1] << " found at position " << file_position << endl;
            index += 1;
            break;
        }
        // cout << "The current posiiton is: " << file.tellg() << endl;
        // cout << "The filesize is: " << fileSize << endl;
        // cout << "End of loop has been reached" << endl;
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

        // file.open(filePath, ios_base::binary | ios_base::out);
        // file.seekp(4, ios_base::beg);
        // file.write(reinterpret_cast<char *>(&numberOfDescriptors), 4);
        // file.close();

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
    }

    // file.open(filePath, ios_base::in | ios_base::binary | ios_base::out);
    // file.seekg(descriptorOffset, ios_base::beg);
    // uint32_t testelementOffset;
    // uint32_t testelementLength;
    // char testname[9];
    //
    // for (int i = 0; i < numberOfDescriptors; i++) {
    //     file.read((char*)&testelementOffset, 4);
    //     file.read((char*)&testelementLength, 4);
    //     file.read(testname, 8);
    //     string nameString = testname;
    //
    //     cout << "element offset: " << testelementOffset << endl;
    //     cout << "element length: " << testelementLength << endl;
    //     cout << "name: " << nameString << endl;
    // }
    // file.close();


}






