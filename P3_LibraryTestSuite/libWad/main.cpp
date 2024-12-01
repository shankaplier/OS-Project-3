#include <iostream>
#include "Wad.h"
using namespace std;

int main()
{
    Wad* wadObject = Wad::loadWad("./P3_Files/sample1.wad");
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


    //createFile Test 2, creating file in directory
    std::string testPath = "/Gl/ad/example";

    wadObject->createFile(testPath);

    std::vector<std::string> testVector;
    cout << "the result from getDirectory should 2, got: " << wadObject->getDirectory("/Gl/ad/", &testVector) << endl;

    cout << "The result from isDirectory should be 0, got: " << wadObject->isDirectory(testPath) << endl;
    cout << "The result from isContent should be 1, got: " << wadObject->isContent(testPath) << endl;

    cout << "The vector size should 2, got: " << testVector.size() << endl;

    std::vector<std::string> expectedVector = {
        "os",
        "example"
};

    for (int i = 0; i < testVector.size(); i++) {
        if (testVector[i] != expectedVector[i]) {
            cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
            break;
        }
    }

    //Reinstantiating object and rerunning tests
    delete wadObject;
    wadObject = Wad::loadWad("./P3_Files/sample1.wad");

    cout << "The result from isDirectory should be 0, got: " << wadObject->isDirectory(testPath) << endl;
    cout << "The result from isContent should be 1, got: " << wadObject->isContent(testPath) << endl;

    testVector.clear();
    cout << "the result from getDirectory should 2, got: " << wadObject->getDirectory("/Gl/ad/", &testVector) << endl;
    cout << "The vector size should 2, got: " << testVector.size() << endl;

    for (int i = 0; i < testVector.size(); i++) {
        if (testVector[i] != expectedVector[i]) {
            cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
            break;
        }
    }


    delete wadObject;






//     //createDirectory Test 3, create nested directories back to back
//     std::string testPath = "/ex/";
//     std::string testPath2 = "/ex/am/";
//
//     wadObject->createDirectory(testPath);
//
//     cout << "The result from isDirectory should be 1, got: " << wadObject->isDirectory(testPath) << endl;
//     cout << "The result from isContent should be 0, got: " << wadObject->isContent(testPath) << endl;
//
//     std::vector<std::string> testVector;
//     cout << "the result from getDirectory should 0, got: " << wadObject->getDirectory(testPath, &testVector) << endl;
//     cout << "The vector size should 0, got: " << testVector.size() << endl;
//
//     testVector.clear();
//     cout << "The result from getDirectory should be 4, got: " << wadObject->getDirectory("/", &testVector) << endl;
//     cout << "The vector size should be 4, got: " << testVector.size() << endl;
//
//     std::vector<std::string> expectedVector = {
//         "E1M0",
//         "Gl",
//         "mp.txt",
//         "ex",
// };
//
//     for (int i = 0; i < testVector.size(); i++) {
//             if (testVector[i] != expectedVector[i]) {
//                 cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
//                 break;
//             }
//         }
//
//     wadObject->createDirectory(testPath2);
//
//     cout << "The result from isDirectory should be 1, got: " << wadObject->isDirectory(testPath2) << endl;
//     cout << "The result from isContent should be 0, got: " << wadObject->isContent(testPath2) << endl;
//
//     testVector.clear();
//     cout << "the result from getDirectory should 1, got: " << wadObject->getDirectory(testPath, &testVector) << endl;
//     cout << "The vector size should 1, got: " << testVector.size() << endl;
//
//     expectedVector.clear();
//     expectedVector = {
//         "E1M0",
//         "Gl",
//         "mp.txt",
//         "ex",
// };
//
//     for (int i = 0; i < testVector.size(); i++) {
//         if (testVector[i] != expectedVector[i]) {
//             cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
//             break;
//         }
//     }
//
//     testVector.clear();
//     cout << "The result from getDirectory should be 0, got: " << wadObject->getDirectory(testPath2, &testVector) << endl;
//     cout << "The vector size should be 0, got: " << testVector.size() << endl;
//
//
//     //Deleting and reinitiating object
//     delete wadObject;
//     cout << "wadObject has been deleted" << endl;
//     wadObject = Wad::loadWad("./P3_Files/sample1.wad");
//
//     cout << "The result from isDirectory should be 1, got: " << wadObject->isDirectory(testPath) << endl;
//     cout << "The result from isContent should be 0, got: " << wadObject->isContent(testPath) << endl;
//
//     cout << "The result from isDirectory should be 1, got: " << wadObject->isDirectory(testPath2) << endl;
//     cout << "The result from isContent should be 0, got: " << wadObject->isContent(testPath2) << endl;
//
//     testVector.clear();
//     cout << "the result from getDirectory should 1, got: " << wadObject->getDirectory(testPath, &testVector) << endl;
//     cout << "The vector size should 1, got: " << testVector.size() << endl;
//
//     expectedVector.clear();
//     expectedVector = {
//         "am"
//     };
//
//     for (int i = 0; i < testVector.size(); i++) {
//         if (testVector[i] != expectedVector[i]) {
//             cout << "Error, expected :" << expectedVector[i] << ", got " << testVector[i] << endl;
//             break;
//         }
//     }
//
//     testVector.clear();
//     cout << "The result from getDirectory should be 0, got: " << wadObject->getDirectory(testPath2, &testVector) << endl;
//     cout << "The vector size should be 0, got: " << testVector.size() << endl;
//
//     delete wadObject;
//

}
