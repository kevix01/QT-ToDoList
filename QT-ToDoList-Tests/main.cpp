#include <QtTest>
#include "iomanagertest.h"
#include "mainwindowtest.h"
#include "taskmanagertest.h"

int main(int argc, char** argv)
{
   int status = 0;
   {
      IOManagerTest tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   {
      TaskManagerTest tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   {
      MainWindowTest tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   return status;
}
