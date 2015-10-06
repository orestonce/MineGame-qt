#ifndef MINEGAMEWINDOW_H
#define MINEGAMEWINDOW_H

#include <QMainWindow>
#include "MineGame.h"

namespace Ui {
class MineGameWindow;
}

class QTableWidgetItem;
class QApplication;

class MineGameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MineGameWindow(QWidget *parent = 0);
    void setApp(QApplication *a);
    ~MineGameWindow();
private slots:
    void slot_itemPressed(QTableWidgetItem* );
    void on_pushButton_clicked();
protected:
    void paintEvent (QPaintEvent *);
private:
    Ui::MineGameWindow *ui;
    QApplication *a;
    restonce::MineGame *game;
};

#endif // MINEGAMEWINDOW_H
