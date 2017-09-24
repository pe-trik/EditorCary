#include "hlavneokno.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/ikonky/volnaciara.svg"));
    HlavneOkno w;
    w.show();

    return a.exec();
}
