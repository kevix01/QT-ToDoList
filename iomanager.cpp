#include "iomanager.h"
#include <sys/stat.h>
#include <unistd.h>

IOManager::IOManager()
{

}

string* r = nullptr;
string* temp = nullptr;

bool IOManager::exists(string path){
    ifstream infile;
    infile.exceptions ( ifstream::failbit | ifstream::badbit );

    try{
        infile.open(path);
        infile.close();
        return true;
    }catch (const ifstream::failure& e) {
        return false;
    }
}

vector<string*> IOManager::readFile(string path, IOManager& mng)
{
    vector<string> v;
    string str;
    ifstream infile;
    infile.exceptions ( ifstream::failbit | ifstream::badbit );

    try{
        infile.open(path);
        while(!infile.eof()){
            getline(infile, str);
            if(str.length()>0){
                v = IOManager::split(str, ';');
                temp = new string [v.size()];
                for(unsigned int i = 0; i<v.size();i++)
                    temp[i] = v.at(i);
                mng.data.push_back(temp);
            }
        }
        infile.close();
    }catch (const ifstream::failure& e) {

    }
    return mng.data;
}

void IOManager::writeFile(string path, string file)
{
    ofstream outfile(path);
    outfile << file;
    outfile.close();
}

vector<string> IOManager::split(string l, char regex)
{
    vector<string> vect;
    stringstream ss(l);
    string token;
    while (getline(ss,token, regex))
    {
        vect.push_back(token);
    }
    return vect ;
}
