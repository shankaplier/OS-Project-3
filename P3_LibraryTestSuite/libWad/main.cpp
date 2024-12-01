#include <iostream>
#include "Wad.h"
using namespace std;

int main()
{
    Wad* wadObject = Wad::loadWad("./P3_Files/sample1.wad");
    string testPath = "/file.txt";

    wadObject->createFile(testPath);

    cout << "isContent: " << wadObject->isContent(testPath) << endl;
    cout << "isDirectory: " << wadObject->isDirectory(testPath) << endl;

    std::vector<std::string> testVector;
    cout << "The result is: " << wadObject->getDirectory("/", &testVector) << endl;
    cout << "The vector Size is: " << testVector.size() << endl;

    const char expectedFileContents[] = "Hello! This is a test to make sure I can write to files.\n";
    int expectedSizeOfFile = 57;

    cout << "The result from writing is: " << wadObject->writeToFile(testPath, expectedFileContents, expectedSizeOfFile) << endl;
    cout << "The size of the written object is: " << wadObject->getSize(testPath) << endl;

    char buffer[100];
    memset(buffer, 0, 100);
    cout << "The result from getContents is: " << wadObject->getContents(testPath, buffer, expectedSizeOfFile) << endl;
    cout << "The result of memcmp is: " << memcmp(buffer, expectedFileContents, expectedSizeOfFile) << endl;

    cout << "The returned string is: " << buffer << endl;




}
