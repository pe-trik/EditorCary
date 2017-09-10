#include "hlavneokno.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HlavneOkno w;
    w.show();

    return a.exec();
}
