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
    ~MineGameWindow();
private slots:
    void on_pushButton_clicked();
    void on_actionRestart_triggered();

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    QRect getMineRect(int lx, int ly);
    void stateChanged();
private:
    Ui::MineGameWindow *ui;
    restonce::MineGame *game;
    int m_x0 = 0;
    int m_y0 = 0;
    int m_stepX = 0;
    int m_stepY = 0;
};

#endif // MINEGAMEWINDOW_H
