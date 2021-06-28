#include <QtTest>
#include "iomanagertest.h"
#include "mainwindowtest.h"

int main(int argc, char** argv)
{
   int status = 0;
   {
      MainWindowTest tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   {
      IOManagerTest tc;
      status |= QTest::qExec(&tc, argc, argv);
   }
   return status;
}
