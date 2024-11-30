//
// Created by Shashank Gutta on 11/28/24.
//

#ifndef WAD_H
#define WAD_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
using namespace std;
#pragma once

class Wad {
    public:
        static Wad* loadWad(const string &path);
        string getMagic();
        bool isContent(const string &path);
        bool isDirectory(const string &path);
        int getSize(const string &path);
        int getContents(const string &path, char *buffer, int length, int offset=0);
        int getDirectory(const string &path, vector<string> *directory);
        void createDirectory(const string &path);
        void createFile(const string &path);
        int writeToFile(const string &path, const char *buffer, int length, int offset = 0);
    private:
        Wad(const string &path);
        string Magic;
        struct tree {
            string name;
            int offset;
            int length;
            vector<tree> children;
        };
        tree makeTree(string name, int offset, int length);
        map<string, tree>* treeMap;
        stack<string>* treeStack;
        vector<string> pathSeperator(const string &path);
        fstream file;
        string endDashRemover(string const &path);
        string filePath;
        char magic[5];
        uint32_t numberOfDescriptors;
        uint32_t descriptorOffset;
        int endDescriptorFinder(const string &path);
        void descriptorAdder(int offset, string &name);
        void fileAdder(int offset, string &name);




};



#endif //WAD_H
