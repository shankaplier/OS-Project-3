#include <iostream>
#include "Wad.h"
using namespace std;

int main()
{
    Wad* wadObject = Wad::loadWad("./P3_Files/sample1.wad");
    string testPath = "/E1M0/";
    std::vector<std::string> testVector;
    int ret = wadObject->getDirectory(testPath, &testVector);

    cout << ret << endl;

    for (int i = 0; i < testVector.size(); i++) {
        cout << testVector[i] << endl;
    }




}
