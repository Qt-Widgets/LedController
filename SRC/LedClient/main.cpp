#include "ledclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LedClient w;
    w.show();

    return a.exec();
}
