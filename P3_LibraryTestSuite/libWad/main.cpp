#include <iostream>
#include "Wad.h"
using namespace std;

int main()
{
    Wad* wadObject = Wad::loadWad("./P3_Files/sample1.WAD");
    string testPath = "/E1M0/01.txt";
    char expectedString[] = "He loves to sing\n";
    char buffer[1024];
    memset(buffer, 0, 1024);

    int sizeOfFile = wadObject->getSize(testPath);
    cout << sizeOfFile << endl;

    int retValue = wadObject->getContents(testPath, buffer, sizeOfFile);
    cout << retValue << endl;

    cout << "Expected string: " << expectedString;
    cout << "Returned string: " << buffer;
}
