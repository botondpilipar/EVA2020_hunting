#include "BoardGraphicUtility.h"

BoardGraphicUtility::BoardGraphicUtility()
{

}

QRect BoardGraphicUtility::shift(const QRect& from, Direction direction, int with)
{
    QPoint shiftCorner;
    int positiveWith = abs(with);
    switch(direction)
    {
        case Direction::UP:
        {
            shiftCorner = QPoint(0, positiveWith);
        }
            break;
        case Direction::DOWN:
        {
            shiftCorner = QPoint(0, -positiveWith);
        }
            break;
        case Direction::LEFT:
        {
            shiftCorner = QPoint(-positiveWith, 0);
        }
            break;
        case Direction::RIGHT:
        {
            shiftCorner = QPoint(positiveWith, 0);
        }
            break;
    }
    return QRect(from.topLeft() + shiftCorner, from.bottomRight() + shiftCorner);
}
