#ifndef BOARDUTILITY_H
#define BOARDUTILITY_H

#include <ModelTypes.hh>
#include <model_pch.h>

class BoardUtility
{
public:
    static DimensionQ middlePosition(DimensionQ& boardDimensions);
    static QVector<DimensionQ> cornerPositions(DimensionQ& boardDimension);
    static bool isNeighboring(DimensionQ& l, DimensionQ& r);
    static bool isValidDimension(DimensionQ& p);

    static constexpr DimensionType smallestPosition() { return 1; }
};

#endif // BOARDUTILITY_H
