#include <QtTest>
#include <mainwindow.h>
#include <taskmanager.h>
class TaskManagerTest: public QObject
{
    Q_OBJECT

public:
    TaskManagerTest() = default;
    ~TaskManagerTest();
private:
    QApplication* a {nullptr};
    MainWindow* m {nullptr};

private slots:
    void initTestCase()
    {
        qDebug("Starting TaskManager's methods tests...");
        int argc = 0;
        char *argv[100];
        a = new QApplication(argc, argv);
        m = new MainWindow;
        m->setPath("RegularDbTest.cfg");
        m->filter();
    }
    void taskLoadingTest();
    void taskAddingTest();

    void cleanupTestCase()
    {
        qDebug("TaskManager's methods integrity check finished.");
    }
};
