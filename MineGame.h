#ifndef RESTONCE_MINEGAME_H
#define RESTONCE_MINEGAME_H

#include <functional>

namespace restonce {

class MineGameImpl;

class MineGame
{
public:
    enum {
        MIN_ROW = 8,
        MAX_ROW = 32
    };
    enum class GameStatus {
        runing, stop
    };

    enum class WinStatus {
        none, win, lose
    };
    enum class MineView {
        close, open, marked, bome
    };
    struct MineUnit {
        bool isMine;
        MineView v;
        int aroundMineCount;
    };

    MineGame();
    ~MineGame();
    void init(int rowCount, int mineCount);
    MineUnit getMineUnit(int line, int row) const;
    GameStatus getGameStatus() const;
    WinStatus   getWinStatus() const;
    int getRowCount() const;
    void gameStart();
    void gameStop();
    void leftClicked(int line, int row);
    void rightClicked(int line, int row);

    void setStatusChangeCallback(std::function<void()> cb);
private:
    MineGameImpl* mineGameImpl;
};

} // namespace restonce

#endif // RESTONCE_MINEGAME_H
