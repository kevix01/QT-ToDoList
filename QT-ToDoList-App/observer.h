#ifndef OBSERVER_H
#define OBSERVER_H
#include <iostream>
#include <vector>

class Observer
{
public:
    virtual void updateOutput() = 0;
    virtual void decreaseActiveSubject() = 0;
    virtual void decreaseActiveSubject(const string& subjectName) = 0;
};

#endif // OBSERVER_H
