#include "MineGameWindow.h"
#include "ui_MineGameWindow.h"
#include <QTableWidgetItem>
#include <QPaintEvent>

using restonce::MineGame;

MineGameWindow::MineGameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MineGameWindow)
{
    ui->setupUi(this);
    this->setFixedSize (this->size ());
    ui->tableWidget->verticalHeader ()->setVisible (false);
    ui->tableWidget->horizontalHeader ()->setVisible (false);
    for ( int col =0; col < ui->tableWidget->columnCount (); ++col) {
        ui->tableWidget->setColumnWidth (col, ui->tableWidget->rowHeight (0));
    }
    connect (ui->tableWidget,SIGNAL(itemPressed(QTableWidgetItem*)),
             this, SLOT(slot_itemPressed(QTableWidgetItem*)) );
    game = new restonce::MineGame;
    game->init (10, 10);
    for( int c =0; c < ui->tableWidget->columnCount (); ++c) {
        for( int r=0; r<ui->tableWidget->rowCount (); ++r) {
            ui->tableWidget->setItem (r, c, new QTableWidgetItem);
        }
    }
    ui->tableWidget->resize (330, 340);
}

MineGameWindow::~MineGameWindow()
{
    delete ui;
}

void MineGameWindow::setApp (QApplication *a)
{
    this->a = a;
}
#include <iostream>

void MineGameWindow::slot_itemPressed (QTableWidgetItem *i)
{
    int row = i->row ();
    int col = i->column ();

//    std::cout << "press" << row << " " << col << std::endl;

    switch (a->mouseButtons () )
    {
    case Qt::LeftButton:
        game->leftClicked (col, row);
        break;
    case Qt::RightButton:
        game->rightClicked (col, row);
        break;
    default:
        break;
    }

    repaint ();
}

void MineGameWindow::on_pushButton_clicked()
{
    if ( ui->pushButton->text () == trUtf8 ("开始") ||
         ui->pushButton->text () == trUtf8 ("重新开始") ) {
        game->init (10, 13);
        game->gameStart ();
    } else if ( ui->pushButton->text () == trUtf8 ("停止游戏") ){
        game->gameStop ();
    }
    repaint ();
}

void MineGameWindow::paintEvent (QPaintEvent *)
{
    ui->label_win->clear ();
    switch (game->getGameStatus () )
    {
    case MineGame::GameStatus::undo:
        ui->label->setText (trUtf8 ("未开始"));
        ui->label->setText (trUtf8 ("开始游戏"));
        break;
    case MineGame::GameStatus::runing:
        ui->label->setText (trUtf8 ("正在游戏"));
        ui->pushButton->setText (trUtf8 ("停止游戏"));
        break;
    default:
        ui->label->setText (trUtf8 ("游戏结束"));
        ui->pushButton->setText (trUtf8 ("重新开始"));
        if ( game->getWinStatus ()== MineGame::WinStatus::lose ) {
            ui->label_win->setText (trUtf8 ("你输了"));
        } else {
            ui->label_win->setText (trUtf8 ("你赢了"));
        }
        break;
    }
    for( int c =0; c < ui->tableWidget->columnCount (); ++c) {
        for( int r=0; r<ui->tableWidget->rowCount (); ++r) {
            auto u = game->getMineUnit (c, r);
            auto i = new QTableWidgetItem();
            switch( u.v ) {
            case MineGame::MineView::close:
                i->setBackground (Qt::gray);
                break;
            case MineGame::MineView::bome:
                i->setText (" B");
                break;
            case MineGame::MineView::marked:
                i->setBackground (Qt::blue);
                break;
            case MineGame::MineView::open:
                if ( u.isMine ) {
                    i->setText (" * ");
                } else if (u.aroundMineCount > 0) {
                    i->setText(QString().sprintf(" %d ", u.aroundMineCount));
                } else {
                    i->setText("   ");
                }
                break;
            }
            ui->tableWidget->setItem (r, c, i );
        }
    }
}
