#include <iostream>
#include "Wad.h"
using namespace std;

int main()
{
    string mainPath = "./P3_Files/sample1.wad";
    Wad* wadObject = Wad::loadWad(mainPath);
    // string testPath = "/file.txt";

    // //Creating dir '/Ex'
    // wadObject->createDirectory("/Ex");
    // std::vector<std::string> expectedVector = {"E1M0","Gl","mp.txt","Ex"};
    // cout << "The answer from isDirectory should be true, got: " << wadObject->isDirectory("/Ex") << endl;
    // std::vector<std::string> testVector;
    // cout << "The answer from getDirectory should be 4, got: " << wadObject->getDirectory("/", &testVector) << endl;
    // cout << "The answer from testVector.size() should be 4, got: " << testVector.size() << endl;
    // for (int i = 0; i < testVector.size(); i++) {
    //     if (testVector[i] != expectedVector[i]) {
    //         cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
    //         break;
    //     }
    // }
    //
    // //Creating dir '/Ex/ad/'
    // wadObject->createDirectory("/Ex/ad/");
    // cout << "The answer from isDirectory should be true, got: " << wadObject->isDirectory("/Ex/ad/") << endl;
    // testVector.clear();
    // expectedVector.clear();
    // expectedVector = {"ad"};
    // cout << "The answer from getDirectory should be 1, got: " << wadObject->getDirectory("/Ex", &testVector) << endl;
    // cout << "The answer from testVector.size() should be 1, got: " << testVector.size() << endl;
    // for (int i = 0; i < testVector.size(); i++) {
    //     if (testVector[i] != expectedVector[i]) {
    //         cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
    //         break;
    //     }
    // }
    //
    // //Creating and writing to '/Ex/ad/test.txt'
    // wadObject->createFile("/Ex/ad/test.txt");
    // cout << "The answer from isContent should be true, got: " << wadObject->isContent("/Ex/ad/test.txt") << endl;
    // testVector.clear();
    // expectedVector.clear();
    // expectedVector = {"test.txt"};
    // cout << "The answer from getDirectory should be 1, got: " << wadObject->getDirectory("/Ex/ad/", &testVector) << endl;
    // cout << "The answer from testVector.size() should be 1, got: " << testVector.size() << endl;
    // for (int i = 0; i < testVector.size(); i++) {
    //     if (testVector[i] != expectedVector[i]) {
    //         cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
    //         break;
    //     }
    // }
    //
    // const char inputText[] =
    //         "This text is pretty long, but will be "
    //         "concatenated into just a single string. "
    //         "The disadvantage is that you have to quote "
    //         "each part, and newlines must be literal as "
    //         "usual.";
    //
    // //Removing one from size to exclude null term
    // int inputSize = 170;
    //
    // cout << "The answer from writeToFile should be 170, got: " << wadObject->writeToFile("/Ex/ad/test.txt", inputText, inputSize) << endl;
    //
    // char buffer[200];
    // memset(buffer, 0 , 200);
    // cout << "The answer from getContents should be 170, got: " << wadObject->getContents("/Ex/ad/test.txt", buffer, inputSize) << endl;
    // cout << "The answer from getSize should be 170, got: " << wadObject->getSize("/Ex/ad/test.txt") << endl;
    // cout << "The answer from memcmp should be 0, got: " << memcmp(buffer, inputText, inputSize) << endl;
    //
    // //Creating dir '/Ex/ad/os/'
    // wadObject->createDirectory("/Ex/ad/os");
    // cout << "The answer from isDirectory should be 1, got: " << wadObject->isDirectory("/Ex/ad/os") << endl;
    // testVector.clear();
    // expectedVector.clear();
    // expectedVector = {"test.txt", "os"};
    //
    // cout << "The answer from getDirectory should be 2, got: " << wadObject->getDirectory("/Ex/ad", &testVector) << endl;
    // cout << "The answer from testVector.size() should be 2, got: " << testVector.size() << endl;
    // for (int i = 0; i < testVector.size(); i++) {
    //     if (testVector[i] != expectedVector[i]) {
    //         cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
    //         break;
    //     }
    // }
    //
    // //Creating and writing to '/Ex/ad/os/test.txt'
    //
    // wadObject->createFile("/Ex/ad/os/test.txt");
    // cout << "The answer from isContent should be true, got: " << wadObject->isContent("/Ex/ad/os/test.txt") << endl;
    // testVector.clear();
    // expectedVector.clear();
    // expectedVector = {"test.txt"};
    // cout << "The answer from getDirectory should be 1, got: " << wadObject->getDirectory("/Ex/ad/os", &testVector) << endl;
    // cout << "The answer from testVector.size() should be 1, got: " << testVector.size() << endl;
    // for (int i = 0; i < testVector.size(); i++) {
    //     if (testVector[i] != expectedVector[i]) {
    //         cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
    //         break;
    //     }
    // }
    //
    // cout << "The answer from writeToFile should be 170, got: " << wadObject->writeToFile("/Ex/ad/os/test.txt", inputText, inputSize) << endl;
    //
    // memset(buffer, 0 , 200);
    // cout << "The answer from getContents should be 170, got: " << wadObject->getContents("/Ex/ad/os/test.txt", buffer, inputSize) << endl;
    // cout << "The answer from getSize should be 170, got: " << wadObject->getSize("/Ex/ad/os/test.txt") << endl;
    // cout << "The answer from memcmp should be 0, got: " << memcmp(buffer, inputText, inputSize) << endl;
    //
    //
    // //Deleting and reinstantiating object
    // delete wadObject;
    // cout << "The wad Object has been deleted!" << endl;
    // wadObject = Wad::loadWad("./P3_Files/sample1.wad");
    //
    // testVector.clear();
    // expectedVector.clear();
    // expectedVector = {"ad"};
    // cout << "The answer from getDirectory should be 1, got: " << wadObject->getDirectory("/Ex/", &testVector) << endl;
    // cout << "The answer from isDirectory should be 1, got: " << wadObject->isDirectory("/Ex/") << endl;
    // cout << "The answer from isDirectory should be 1, got: " << wadObject->isDirectory("/Ex/ad") << endl;
    // cout << "The answer from isContent should be 0, got: " << wadObject->isContent("/Ex/test.txt") << endl;
    // cout << "The answer from testvector.size() should be 1, got: " << testVector.size() << endl;
    // for (int i = 0; i < testVector.size(); i++) {
    //     if (testVector[i] != expectedVector[i]) {
    //         cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
    //         break;
    //     }
    // }
    //
    // testVector.clear();
    // expectedVector.clear();
    // expectedVector = {"test.txt", "os"};
    // cout << "The answer from getDirectory should be 2, got: " << wadObject->getDirectory("/Ex/ad", &testVector) << endl;
    // cout << "The answer from testvector.size() should be 2, got: " << testVector.size() << endl;
    // for (int i = 0; i < testVector.size(); i++) {
    //     if (testVector[i] != expectedVector[i]) {
    //         cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
    //         break;
    //     }
    // }
    //
    // testVector.clear();
    // expectedVector.clear();
    // expectedVector = {"test.txt"};
    // cout << "The answer from getDirectory should be 1, got: " << wadObject->getDirectory("/Ex/ad/os", &testVector) << endl;
    // cout << "The answer from testvector.size() should be 1, got: " << testVector.size() << endl;
    // for (int i = 0; i < testVector.size(); i++) {
    //     if (testVector[i] != expectedVector[i]) {
    //         cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
    //         break;
    //     }
    // }
    //
    // cout << "The answer from isContent should be 1, got: " << wadObject->isContent("/Ex/ad/test.txt") << endl;
    // cout << "The answer from getSize should be 170, got: " << wadObject->getSize("/Ex/ad/test.txt") << endl;
    //
    // memset(buffer, 0 , 200);
    // cout << "The answer from getContents should be 170, got: " << wadObject->getContents("/Ex/ad/test.txt", buffer, inputSize) << endl;
    // cout << "The answer from memcmp should be 0, got: " << memcmp(buffer, inputText, inputSize) << endl;
    //
    // cout << "The answer from isContent should be 1, got: " << wadObject->isContent("/Ex/ad/os/test.txt") << endl;
    // cout << "The answer from getSize should be 170, got: " << wadObject->getSize("/Ex/ad/os/test.txt") << endl;
    // memset(buffer, 0 , 200);
    // cout << "The answer from getContents should be 170, got: " << wadObject->getContents("/Ex/ad/os/test.txt", buffer, inputSize) << endl;
    // cout << "The answer from memcmp should be 0, got: " << memcmp(buffer, inputText, inputSize) << endl;
    //
    // delete wadObject;


    //writeToFile Test 1, creating and writing to a text file
    std::string testPath = "/file.txt";

    //testing file creation

    wadObject->createFile(testPath);


    cout << "The result from isContent should be 1, got: " << wadObject->isContent(testPath) << endl;
    cout << "The result from isContent should be 0, got: " << wadObject->isDirectory(testPath) << endl;

    std::vector<std::string> testVector;
    cout << "The result from getDirectory should be 4, got: " << wadObject->getDirectory("/", &testVector) << endl;
    cout << "The testvector,size must be 4, got: " << testVector.size() << endl;

    std::vector<std::string> expectedVector = {
            "E1M0",
            "Gl",
            "mp.txt",
            "file.txt"
    };

    for (int i = 0; i < testVector.size(); i++) {
        if (testVector[i] != expectedVector[i]) {
            cout << "Error: expected" << expectedVector[i] << " but got" << testVector[i] << endl;
        }
    }

    //testing file writing
    const char expectedFileContents[] = "Hello! This is a test to make sure I can write to files.\n";
    int expectedSizeOfFile = 57;

    cout << "The result of writeToFile should be 57, got: " << wadObject->writeToFile(testPath, expectedFileContents, expectedSizeOfFile) << endl;
    cout << "The result of getSize() should be 57, got: " << wadObject->getSize(testPath) << endl;

    char buffer[100];
    memset(buffer, 0, 100);
    cout << "The result of getContents should be 57, got: " << wadObject->getContents(testPath, buffer, expectedSizeOfFile) << endl;
    cout << "The result of memcmp should be 0, got: " << memcmp(buffer, expectedFileContents, expectedSizeOfFile) << endl;

    //Reinstantiating object and rerunning tests
    delete wadObject;

    cout << "The wad has been deleted" << endl;

    wadObject = Wad::loadWad(mainPath);

    cout << "The result of getSize() should be 57, got: " << wadObject->getSize(testPath) << endl;

    memset(buffer, 0, 100);
    cout << "The result of getContents should be 57, got: " << wadObject->getContents(testPath, buffer, expectedSizeOfFile) << endl;
    cout << "The result of memcmp should be 0, got: " << memcmp(buffer, expectedFileContents, expectedSizeOfFile) << endl;

    //Attemping to write to the file again

    cout << "The result of writeToFile should be 0, got: " << wadObject->writeToFile(testPath, expectedFileContents, expectedSizeOfFile) << endl;
    cout << "The result of getSize() should be 57, got: " << wadObject->getSize(testPath) << endl;

    memset(buffer, 0, 100);
    cout << "The result of getContents should be 57, got: " << wadObject->getContents(testPath, buffer, expectedSizeOfFile) << endl;
    cout << "The result of memcmp should be 0, got: " << memcmp(buffer, expectedFileContents, expectedSizeOfFile) << endl;

    delete wadObject;


}
