#include <iostream>
#include "Wad.h"
using namespace std;

int main()
{
    Wad* wadObject = Wad::loadWad("./P3_Files/sample1.wad");
    string testPath = "/Gl/ad/example";

    wadObject->createFile(testPath);
    cout << "is content: " << wadObject->isContent(testPath) << endl;
    cout << "is directory: " << wadObject->isDirectory(testPath) << endl;

    std::vector<std::string> testVector;
    cout << "Returned from get directory: " << wadObject->getDirectory("/", &testVector) << endl;
    cout << "Test vector size: " << testVector.size() << endl;

    delete wadObject;
    cout << "Wad has been deleted." << endl;
    wadObject = Wad::loadWad("./P3_Files/sample1.wad");

    cout << "is content: " << wadObject->isContent(testPath) << endl;
    cout << "is directory: " << wadObject->isDirectory(testPath) << endl;


}
