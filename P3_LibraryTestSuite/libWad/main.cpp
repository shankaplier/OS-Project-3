#include <iostream>
#include "Wad.h"
using namespace std;

int main()
{
    Wad* wadObject = Wad::loadWad("./P3_Files/sample1.wad");
    string testPath = "/E1M0/01.txt";
    char buffer[1024];
    memset(buffer, 0, 1024);
    int sizeOfFile = wadObject->getSize(testPath);

    int result = wadObject->getContents(testPath, buffer, sizeOfFile);

    cout << result << endl;

    string ans = buffer;

    cout << ans << endl;

    for (int i = 0; i < 17; i++) {
        cout << buffer[i];
    }

}
