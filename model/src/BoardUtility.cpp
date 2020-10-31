#include <BoardUtility.h>

DimensionQ BoardUtility::middlePosition() const
{
    DimensionQ returnDimensions;
    bool horizontallyEven = mDimensions.first % 2 == 0;
    bool verticallyEven = mDimensions.second % 2 == 0;

    returnDimensions.first = horizontallyEven ? mDimensions.first / 2
                                              : mDimensions.first / 2 + 1;

    returnDimensions.second = verticallyEven ? mDimensions.second / 2
                                              : mDimensions.second / 2 + 1;

    return returnDimensions;
}

QVector<DimensionQ> BoardUtility::cornerPositions() const
{
    return {    DimensionQ(1, mDimensions.second),
                DimensionQ(mDimensions.first, mDimensions.second),
                DimensionQ(1, 1),
                DimensionQ(mDimensions.first, 1)    };
}

bool BoardUtility::isNeighboring(const DimensionQ& l, const DimensionQ& r)
{
    auto xDiff = static_cast<std::int64_t>(l.first - r.first);
    auto yDiff = static_cast<std::int64_t>(l.second - r.second);
    auto totalDifference = abs(xDiff) + abs(yDiff);
    return (totalDifference == 1);
}

bool BoardUtility::isValidDimension(const DimensionQ &p) const
{
    return (p.first >= smallestPosition() &&
            p.first <= mDimensions.first &&
            p.second >= smallestPosition() &&
            p.second <= mDimensions.second);
}

QList<DimensionQ> BoardUtility::possibleSteps(DimensionQ current) const
{
    QList<DimensionQ> all = {
        BoardUtility::shiftDimension(current, Direction::DOWN, 1),
        BoardUtility::shiftDimension(current, Direction::UP, 1),
        BoardUtility::shiftDimension(current, Direction::LEFT, 1),
        BoardUtility::shiftDimension(current, Direction::RIGHT, 1),
    };
    QList<DimensionQ> valid;
    std::copy_if(all.begin(), all.end(), std::back_inserter(valid),
                 [this](const DimensionQ& dim) { return isValidDimension(dim); });

    return valid;
}

DimensionQ BoardUtility::fromLogicalCoordinate(const DimensionQ& from)
{
    return { from.first - 1, from.second - 1};
}

DimensionQ BoardUtility::toLogicalCoordinate(const DimensionQ& from)
{
    return { from.first + 1, from.second + 1};
}

