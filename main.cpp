#include <QApplication>
#include "MineGameWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MineGameWindow w;
    w.show ();

    return a.exec();
}
