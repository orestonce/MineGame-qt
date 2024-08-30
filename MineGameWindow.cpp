#include "MineGameWindow.h"
#include "ui_MineGameWindow.h"
#include <QTableWidgetItem>
#include <QPaintEvent>
#include <iostream>
#include <QDebug>
#include <QPainter>

using restonce::MineGame;

MineGameWindow::MineGameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MineGameWindow)
{
    ui->setupUi(this);
    game = new restonce::MineGame;
    game->setStatusChangeCallback(std::bind(&MineGameWindow::stateChanged, this));
    game->init (10, 10);
    ui->widget_down->installEventFilter(this);
}

MineGameWindow::~MineGameWindow()
{
    delete ui;
}

void MineGameWindow::on_pushButton_clicked()
{
    game->init (10, 13);
    game->gameStart();
    repaint();
}

bool MineGameWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->widget_down && event->type() == QEvent::Paint) {
        QPainter painter(ui->widget_down);
        QSize sz = ui->widget_down->size();
        int x0 = 5;
        int y0 = 5;
        int x1 = sz.width() -5;
        int y1 = sz.height()-5;
        int rowCount = game->getRowCount();

        int stepY = 0;
        int stepX = 0;

        if(rowCount > 0 && y1 > y0 + rowCount && x1 > x0 + rowCount)
        {
            stepY = (y1 - y0) / rowCount;
            stepX = (x1 - x0) / rowCount;

            int spaceY = ui->widget_down->height() - stepY * rowCount;
            int spaceX = ui->widget_down->width() - stepX * rowCount;

            x0 = spaceX/2;
            y0 = spaceY/2;

            y1 = y0 + rowCount * stepY;
            x1 = x0 + rowCount * stepX;
        }

        if(stepY > 0)
        {
            for(int i=0; i <= y1 - y0; i += stepY)
            {
                int y = y0 + i;
                painter.drawLine(x0, y, x1, y);
            }
        }

        if(stepX > 0)
        {
            for(int i=0; i <= x1 - x0; i += stepX)
            {
                int x = x0 + i;
                painter.drawLine(x, y0, x, y1);
            }
        }

        m_x0 = x0;
        m_y0 = y0;
        m_stepX = stepX;
        m_stepY = stepY;


        if(stepX > 0 && stepY > 0)
        {
            QImage boom = QImage(":/image/mine/boom.png").scaled(stepX, stepY);
            QImage flag = QImage(":/image/mine/flag.png").scaled(stepX, stepY);
            QImage normal = QImage(":/image/mine/normal.png").scaled(stepX, stepY);

            for(int lx = 0; lx < rowCount; lx++)
            {
                for(int ly = 0; ly < rowCount; ly++)
                {
                    QRect r = getMineRect(lx, ly);
                    if(r.isValid() == false)
                        continue;
                    auto u = game->getMineUnit (lx, ly);
                    switch( u.v ) {
                        case MineGame::MineView::close:
                            painter.fillRect(r, Qt::gray);
                            break;
                        case MineGame::MineView::bome:
                            painter.drawImage(r, boom);
                            break;
                        case MineGame::MineView::marked:
                            painter.drawImage(r, flag);
                            break;
                        case MineGame::MineView::open:
                            if ( u.isMine ) {
                                painter.drawImage(r, normal);
                            } else if (u.aroundMineCount > 0) {
                                painter.drawText(QRectF(r), QString::number(u.aroundMineCount),
                                                 QTextOption(Qt::AlignCenter));
                            }
                            break;
                    }

                }
            }
        }
    } else if(watched == ui->widget_down && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* me = dynamic_cast<QMouseEvent*>(event);
        int rowCount = game->getRowCount();

        if(me != nullptr && m_stepX > 0 && m_stepY > 0 && rowCount > 0)
        {
            int logicX = (me->pos().x() - m_x0) / m_stepX;
            int logicY = (me->pos().y() - m_y0) / m_stepY;

            switch(me->button())
            {
            case Qt::LeftButton:
                game->leftClicked (logicX, logicY);
                ui->widget_down->repaint();
                break;
            case Qt::RightButton:
                game->rightClicked (logicX, logicY);
                ui->widget_down->repaint();
                break;
            default:
                break;
            }
        }
    }
    return true;
}

QRect MineGameWindow::getMineRect(int lx, int ly)
{
    QRect r;
    if(lx >= 0 && ly >= 0 && m_stepX > 0 && m_stepY > 0)
    {
        r = QRect(QPoint(m_x0 + m_stepX*lx + 2, m_y0 + m_stepY*ly + 2),
                  QSize(m_stepX-2, m_stepY-2));
    }
    return r;
}

void MineGameWindow::stateChanged()
{
    repaint();
}

void MineGameWindow::on_actionRestart_triggered()
{
    game->init (10, 13);
    game->gameStart();
    repaint();
}

void MineGameWindow::paintEvent(QPaintEvent *event)
{
    QString iconPath;

    switch(game->getGameStatus())
    {
    case MineGame::GameStatus::runing:
        iconPath = ":/image/face/runing.png";
        break;
    case MineGame::GameStatus::stop:
        if(game->getWinStatus() == MineGame::WinStatus::win) {
            iconPath = ":/image/face/win.png";
        } else if(game->getWinStatus() == MineGame::WinStatus::lose) {
            iconPath = ":/image/face/lose.png";
        } else {
            iconPath = ":/image/face/none.png";
        }
        break;
    }

    if(iconPath.isEmpty() == false)
    {
        ui->pushButton->setStyleSheet(QString("QPushButton{border-image: url(%1)}").arg(iconPath));
    }
}
