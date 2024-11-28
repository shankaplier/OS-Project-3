#include <iostream>
#include "Wad.h"
using namespace std;

int main()
{
    Wad* wadObject = Wad::loadWad("./P3_Files/sample1.WAD");
    cout << wadObject->getContents("/Gl/ad/os/cake.jpg") << endl;
}
