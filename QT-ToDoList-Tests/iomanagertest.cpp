#include "iomanagertest.h"
#include <vector>
#include <string>

void IOManagerTest::splitMethodTest(){
    std::string testInput = std::string("2001/07/25")+IOManager::regexChar+"Testing Title"+IOManager::regexChar+"100"+IOManager::regexChar+"Testing Description";
    std::vector<string> testOutput = IOManager::split(testInput, IOManager::regexChar);
    //checking vector size
    QCOMPARE(testOutput.size(), 4);
    QCOMPARE(testOutput.at(0), "2001/07/25");
    QCOMPARE(testOutput.at(1), "Testing Title");
    QCOMPARE(testOutput.at(2), "100");
    QCOMPARE(testOutput.at(3), "Testing Description");
}

void IOManagerTest::readFileMethodTest(){
    std::string inputDb = "RegularDbTest.cfg";
    IOManager mng;
    std::vector<string*> outputDb = IOManager::readFile(inputDb,mng);
    //checking vector size
    QCOMPARE(outputDb.size(), 3);

    //testing output of task 1
    QCOMPARE(outputDb.at(0)[0], "2023/06/19");
    QCOMPARE(outputDb.at(0)[1], "Testing title 1");
    QCOMPARE(outputDb.at(0)[2], "25");
    QCOMPARE(outputDb.at(0)[3], "Description test of task 1");

    //testing output of task 2
    QCOMPARE(outputDb.at(1)[0], "2022/12/28");
    QCOMPARE(outputDb.at(1)[1], "Testing title 2");
    QCOMPARE(outputDb.at(1)[2], "48");
    QCOMPARE(outputDb.at(1)[3], "Description test of task 2");

    //testing output of task 3
    QCOMPARE(outputDb.at(2)[0], "2021/05/31");
    QCOMPARE(outputDb.at(2)[1], "Testing title 3");
    QCOMPARE(outputDb.at(2)[2], "100");
    QCOMPARE(outputDb.at(2)[3], "Description test of task 3");
}
