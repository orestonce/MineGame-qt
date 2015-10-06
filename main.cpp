#include <QApplication>
#include "MineGameWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MineGameWindow w;
    w.setApp(&a);
    w.show ();

    return a.exec();
}
