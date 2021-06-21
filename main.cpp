#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <signal.h>
#include <string>

void signalHandler(int signum){
    signal(signum, SIG_DFL);
    QMessageBox::critical(nullptr, "App crashed", "The application has encountered an error! \nDatabase file could be corrupted. \n"
                                                  "Solutions: \n1.Try restarting the app. \n2.Delete 'DefaultDBPath.cfg' file to reset the app settings "
                                                  "(NOTE: this will not delete your database files!)");
}

int main(int argc, char *argv[])
{
    signal(SIGSEGV, signalHandler);
    signal(SIGABRT, signalHandler);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
