#include "ledserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LedServer w;
    w.show();

    return a.exec();
}
