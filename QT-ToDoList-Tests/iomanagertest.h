#include <QtTest>
#include <iomanager.h>

class IOManagerTest: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        qDebug("Starting I/O methods tests...");
    }
    void splitMethodTest();
    void readFileMethodTest();

    void cleanupTestCase()
    {
        qDebug("I/O methods integrity check finished.");
    }
};
