#ifndef IOMANAGER_H
#define IOMANAGER_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class IOManager
{
public:
    IOManager();
    ~IOManager(){
        for(auto it : data)
            if(it)
                delete[] it;
    }
    static vector<string*> readFile(string path, IOManager& mng);
   // static void createFile(string path);
    static void writeFile(string path, string);
    static vector<string> split(string l, char regex);
    static bool exists(string path);
    vector <string*> data;
    vector <string> dates;
};

#endif // IOMANAGER_H
