#include <QtTest>

class IOManagerTest: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        qDebug("Called before everything else.");
    }
    void splitMethodTest();

    void cleanupTestCase()
    {
        qDebug("Called after myFirstTest and mySecondTest.");
    }
};
