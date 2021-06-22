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
    IOManager() = default; //class constructor
    ~IOManager(){ //destructor that frees memory pointed by string pointers in data vector
        for(auto it : data)
            if(it)
                delete[] it;
    }
    static vector<string*> readFile(string path, IOManager& mng); //static method in charge of reading database file
   // static void createFile(string path);
    static void writeFile(string path, string); //static method in charge of writing changes in database file (es. task add/delete/change)
    static vector<string> split(string l, char regex); //static method that split the string received as parameter every time it finds the regex char
                                                       //and return a vector of string that cointains the "splitted string" in multiple string values
    static bool exists(string path); //static method that finds if the database file exists
    vector <string*> data; //vector of string pointers (to array of strings) to store tasks data
    vector <string> dates; //vector of string used to manage the dates of each task (in MainWindow class)
    static constexpr const char regexChar = '|'; //regex char value used for data separation
};

#endif // IOMANAGER_H
