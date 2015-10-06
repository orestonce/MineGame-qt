#include "MineGame.h"
#include <stdexcept>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

namespace restonce {

class MineGameImpl
{
public :
    MineGameImpl()
    {
        gameStatus = MineGame::GameStatus::undo;
    }

    struct Position {
        int line, row;
    };

    void init (int rowCount, int mineCount)
    {
//        std::clog << rowCount << " " << mineCount << std::endl;

        int positions = rowCount * rowCount;
        if ( rowCount <MineGame:: MIN_ROW ||
             rowCount > MineGame:: MAX_ROW ||
             mineCount > positions/6 ||
             mineCount < positions/10 ) {
            throw std::runtime_error("init failed !");
        }
        this->rowCount = rowCount;
        this->gameStatus = MineGame::GameStatus::undo;
        // 清空地图
        for ( int i=0; i<MineGame::MAX_ROW; ++i) {
            for( int j=0; j<MineGame::MAX_ROW; ++j) {
                maps[i][j].isMine = false;
                maps[i][j].v = MineGame:: MineView::close;
                maps[i][j].aroundMineCount = 0;
            }
        }
        gameStatus = MineGame::GameStatus::undo;
        // 随机产生地雷
        srand(time(NULL));
        for ( int i=0; i<mineCount ; ++i) {
            int mineIndex = rand() % positions;
            while ( hasMine (mineIndex) ) {
                mineIndex = rand() % positions;
            }
            maps[mineIndex/rowCount][mineIndex%rowCount].isMine = true;
        }
        // 统计每个位置周围的地雷数目


        for ( int line=0; line < rowCount; ++line) {
            for(int row=0; row<rowCount; ++row) {
                int aroundMineCount = 0;
                for( auto p : aroundPositions (line, row) ) {
                    if (positionValid (p.line,p.row)
                            && maps[p.line][p.row].isMine ) {
                        aroundMineCount ++;
                    }
                }
                maps[line][row].aroundMineCount = aroundMineCount;
            }
        }
//        debugPrint ();
    }
    MineGame::MineUnit getMineUnit(int l, int r) const
    {
        if ( !positionValid (l, r) ) {
            throw std::out_of_range("Out of range !");
        }
        return maps[l][r];
    }
    MineGame::GameStatus getGameStatus() const
    {
        return gameStatus;
    }
    MineGame::WinStatus getWinStatus() const
    {
        if ( gameStatus != MineGame::GameStatus::stop ) {
            throw std::logic_error("Game not stop !");
        }
        return winStatus;
    }

    void leftClicked(int l, int r)
    {
        if ( !positionValid (l, r)
             || gameStatus != MineGame::GameStatus::runing ) {
            return ;
        }
//        std::clog << "clicked " << std::endl;
        if ( maps[l][r].v == MineGame::MineView::close ) {
            if ( maps[l][r].isMine ) {// 左击中地雷
                maps[l][r].v = MineGame::MineView::bome; // 此处位置状态为爆炸
                // 打开所有位置
                for(int line=0; line<rowCount; ++line) {
                    for(int row=0; row<rowCount; ++row) {
                        if ( !(line == l && row == r) )
                            maps[line][row].v = MineGame::MineView::open;
                    }
                }
                // 设置游戏结束，玩家输了
                gameStatus = MineGame::GameStatus::stop;
                winStatus = MineGame::WinStatus::lose;
            }else {
                maps[l][r].v = MineGame::MineView::open; // 打开当前位置
                // 游戏结束?
                updateGameWiner();

                if ( maps[l][r].aroundMineCount == 0 ) {
                    // 若当前位置的周围都没有地雷，打开周围所有位置
                    for ( auto m : aroundPositions(l, r) ) {
                        this->leftClicked (m.line, m.row);
                    }
                }
            }
        }
    }

    void rightClicked(int l, int r)
    {
        if ( !positionValid (l, r)
             || gameStatus != MineGame::GameStatus::runing ) {
            return ;
        }
        switch( maps[l][r].v )
        {
        case MineGame::MineView::marked:
            maps[l][r].v = MineGame::MineView::close;
            break;
        case MineGame::MineView::close:
            maps[l][r].v = MineGame::MineView::marked;
            break;
        default:
            break;
        }
    }

    void gameStart()
    {
        gameStatus = MineGame::GameStatus::runing;
    }
    void gameStop()
    {
        gameStatus = MineGame::GameStatus::stop;
        winStatus = MineGame::WinStatus::lose;
    }

private:
    bool hasMine(int index)
    {
        return maps[index/rowCount][index%rowCount].isMine;
    }
    bool positionValid(int l, int r) const
    {
        return  l < rowCount && l>=0 && r < rowCount && r>=0;
    }
    void updateGameWiner()
    {
        //不是地雷的位置都打开了，玩家就胜利了
        for(int l=0; l<rowCount; ++l) {
            for( int r=0; r<rowCount; ++r) {
                if ( !maps[l][r].isMine) {
                    if ( maps[l][r].v != MineGame::MineView::open ) {
                        return ;
                    }
                }
            }
        }
        gameStatus = MineGame::GameStatus::stop;
        winStatus = MineGame::WinStatus::win;
        for(int l=0; l<rowCount; ++l) {
            for( int r=0; r<rowCount; ++r) {
                leftClicked (l, r);
            }
        }
    }

    static std::vector<Position> aroundPositions(int l, int r)
    {
        struct Moves {
            int dl, dr;
        };
        Moves moves[8] = {
            {-1,-1},
            {0, -1},
            {1, -1},
            {-1,0},
            {1, 0},
            {-1,1},
            {0,1},
            {1,1}
        };
        std::vector<Position> v;

        for ( auto m : moves) {
            Position p ;
            p.line = m.dl + l;
            p.row = m.dr + r;
            v.push_back (p);
        }
        return std::move(v);
    }

    void debugPrint()
    {
        for(int i=0; i<MineGame::MAX_ROW; ++i) {
            for( int j=0; j<MineGame::MAX_ROW; ++j) {
                if ( maps[i][j].isMine ) {
                    std::cout << "* ";
                } else {
                    std::cout << maps[i][j].aroundMineCount << " ";
                }
            }
            std::cout << std::endl;
        }
    }
private:
    MineGame::MineUnit maps[MineGame::MAX_ROW][MineGame::MAX_ROW];
    int rowCount;
    MineGame::GameStatus gameStatus;
    MineGame::WinStatus   winStatus;
};

MineGame::MineGame()
{
    mineGameImpl = new MineGameImpl;
}

MineGame::~MineGame()
{
    delete mineGameImpl;
}

void MineGame::init (int rowCount, int mineCount)
{
    mineGameImpl->init(rowCount, mineCount);
}

MineGame::MineUnit MineGame::getMineUnit (int line, int row) const
{
    return mineGameImpl->getMineUnit (line, row);
}

MineGame::GameStatus MineGame::getGameStatus () const
{
    return mineGameImpl->getGameStatus();
}

MineGame::WinStatus MineGame::getWinStatus () const
{
    return mineGameImpl->getWinStatus ();
}

void MineGame::gameStart ()
{
    mineGameImpl->gameStart ();
}

void MineGame::gameStop ()
{
    mineGameImpl->gameStop();
}

void MineGame::leftClicked (int line, int row)
{
    mineGameImpl->leftClicked (line, row);
//    std::cout << __func__ << std::endl;
}

void MineGame::rightClicked (int line, int row)
{
    mineGameImpl->rightClicked (line, row);
//    std::cout << __func__ << std::endl;
}

} // namespace restonce
