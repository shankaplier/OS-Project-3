#include <iostream>
#include "Wad.h"
using namespace std;

int main()
{
    Wad* wadObject = Wad::loadWad("./P3_Files/sample1.wad");
    string testPath = "/ex/";

    wadObject->createDirectory(testPath);
    cout << "isDirectory: " << wadObject->isDirectory(testPath) << endl;
    cout << "isContent: " << wadObject->isContent(testPath) << endl;

    delete wadObject;
    cout << "Wad object has been deleted" << endl;

    wadObject = Wad::loadWad("./P3_Files/sample1.wad");

    cout << "isDirectory: " << wadObject->isDirectory(testPath) << endl;
    cout << "isContent: " << wadObject->isContent(testPath) << endl;



}
