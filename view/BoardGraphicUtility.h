#ifndef BOARDGRAPHICUTILITY_H
#define BOARDGRAPHICUTILITY_H

#include <QRect>
#include <QPair>
#include <ModelTypes.hh>

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

    template<typename Dimension, typename Unit>
    static Dimension shiftDimension(const Dimension& from, Direction direction, Unit width)
    {
        Unit positiveWith = abs(width);
        Dimension shift;
        switch (direction)
        {
            case Direction::UP:
            {
                shift = {0, -positiveWith};
            }
            break;
            case Direction::DOWN:
            {
                shift = {0, positiveWith};
            }
            break;
            case Direction::LEFT:
            {
                shift = {-positiveWith, 0};
            }
            break;
            case Direction::RIGHT:
            {
                shift = {positiveWith, 0};
            }
        }
        return from + shift;
    }

    static DimensionQ fromLogicalCoordinate(const DimensionQ& from);
    static DimensionQ toLogicalCoordinate(const DimensionQ& from);
};

#endif // BOARDGRAPHICUTILITY_H
