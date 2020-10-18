#include "HuntingMainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HuntingMainWindow w;
    w.show();
    return a.exec();
}
