#include <BoardUtility.h>

DimensionQ BoardUtility::middlePosition(DimensionQ& boardDimensions)
{
    if(!isValidDimension(boardDimensions)) { return {}; }

    DimensionQ returnDimensions;
    bool horizontallyEven = boardDimensions.first % 2 == 0;
    bool verticallyEven = boardDimensions.second % 2 == 0;

    returnDimensions.first = horizontallyEven ? boardDimensions.first / 2
                                              : boardDimensions.first / 2 + 1;

    returnDimensions.second = verticallyEven ? boardDimensions.second / 2
                                              : boardDimensions.second / 2 + 1;

    return returnDimensions;
}

QVector<DimensionQ> BoardUtility::cornerPositions(DimensionQ& boardDimensions)
{
    if(!isValidDimension(boardDimensions)) { return {}; }

    return {    DimensionQ(1, boardDimensions.second),
                DimensionQ(boardDimensions.first, boardDimensions.second),
                DimensionQ(1, 1),
                DimensionQ(boardDimensions.first, 1)    };
}

bool BoardUtility::isNeighboring(DimensionQ& l, DimensionQ& r)
{
    if(!(isValidDimension(l) && isValidDimension(r)))  { return false; }
    qreal totalDifference = qFabs(l.first - r.first) + qFabs(l.second - r.second);
    return (totalDifference == 1);
}

bool BoardUtility::isValidDimension(DimensionQ &p)
{
    return (p.first >= smallestPosition() && p.second >= smallestPosition());
}
