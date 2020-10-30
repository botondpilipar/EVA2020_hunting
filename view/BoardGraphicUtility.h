#ifndef BOARDGRAPHICUTILITY_H
#define BOARDGRAPHICUTILITY_H

#include <QRect>

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class BoardGraphicUtility
{
public:
    BoardGraphicUtility();
    static QRect shift(const QRect& from, Direction direction, int with);

};

#endif // BOARDGRAPHICUTILITY_H
