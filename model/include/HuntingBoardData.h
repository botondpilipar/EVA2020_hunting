#ifndef HUNTINGBOARDDATA_H
#define HUNTINGBOARDDATA_H

#include <model_pch.h>
#include <ModelTypes.hh>
#include <HuntingTypes.h>
#include <QDataStreamSerializable.h>

namespace kd417d {
namespace eva {

struct HuntingBoardData
{

    HuntingBoardData() = default;
    HuntingBoardData(const DimensionQ& dimension,
    const quint64 maxStep,
    const quint64 stepsTaken,
    const hunting::PlayerType currentlyMoving,
    const hunting::PlayerCoordinates& playerMap)
        : playerSet(playerMap)
        , dimension(dimension)
        , maxStep(maxStep)
        , stepsTaken(stepsTaken)
        , nextToMove(currentlyMoving)
    {}

    hunting::PlayerCoordinates playerSet;
    DimensionQ dimension;
    quint64 maxStep;
    quint64 stepsTaken;
    hunting::PlayerType nextToMove;

    virtual QDataStream& serialize(QDataStream& s) const
    {
        s << dimension << maxStep << stepsTaken << nextToMove << playerSet;
        return s;
    }
    virtual QDataStream& deserialize(QDataStream& s)
    {
        s >> dimension >> maxStep >> stepsTaken >> nextToMove >> playerSet;
        return s;
    }

    bool operator==(const HuntingBoardData& other) const
    {
        return std::tie(playerSet, dimension, maxStep, stepsTaken, nextToMove) ==
                std::tie(other.playerSet, other.dimension, other.maxStep, other.stepsTaken, other.nextToMove);
    }
};

}
}
#endif // HUNTINGBOARDDATA_H
