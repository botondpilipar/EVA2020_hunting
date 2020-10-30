#ifndef BOARDUTILITY_H
#define BOARDUTILITY_H

#include <ModelTypes.hh>
#include <model_pch.h>

class BoardUtility
{
public:
    static DimensionQ middlePosition(const DimensionQ& boardDimensions);
    static QVector<DimensionQ> cornerPositions(const DimensionQ& boardDimension);
    static bool isNeighboring(const DimensionQ& l, const DimensionQ& r);
    static bool isValidDimension(const DimensionQ& p);

    static constexpr DimensionType smallestPosition() { return 1; }
};

#endif // BOARDUTILITY_H
