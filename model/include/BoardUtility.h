#ifndef BOARDUTILITY_H
#define BOARDUTILITY_H

#include <ModelTypes.hh>
#include <model_pch.h>

#include <Direction.h>


class BoardUtility
{
public:
    BoardUtility(const DimensionQ& boardDimensions);
    BoardUtility() = default;

    inline void setDimensions(const DimensionQ& dim) { mDimensions = dim; }

    DimensionQ middlePosition() const;
    QVector<DimensionQ> cornerPositions() const;
    QList<DimensionQ> possibleSteps(DimensionQ current) const;
    static bool isNeighboring(const DimensionQ& l, const DimensionQ& r);
    bool isValidDimension(const DimensionQ& p) const;

    static constexpr DimensionType smallestPosition() { return 1; }

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

private:
    DimensionQ mDimensions;
};

#endif // BOARDUTILITY_H
