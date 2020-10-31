#ifndef BOARDGRAPHICUTILITY_H
#define BOARDGRAPHICUTILITY_H

#include <QRect>
#include <QPair>
#include <ModelTypes.hh>
#include <Direction.h>


class BoardGraphicUtility
{
public:
    BoardGraphicUtility();
    static QRect shift(const QRect& from, Direction direction, int with);
};

#endif // BOARDGRAPHICUTILITY_H
