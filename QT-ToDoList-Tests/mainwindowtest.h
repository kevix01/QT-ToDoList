#include <QtTest>
#include <QApplication>
#include <mainwindow.h>

class MainWindowTest: public QObject
{
    Q_OBJECT

public:
    MainWindowTest();
    ~MainWindowTest();

private:
    QApplication* a {nullptr};
    MainWindow* m {nullptr};

private slots:
    void initTestCase()
    {
        qDebug() << "Starting tests about Ui components...";
        m->show();
    }

    void UiComponentsConstructionTest();

    void cleanupTestCase()
    {
        qDebug("Called after myFirstTest and mySecondTest.");
    }
};
