#ifndef HUNTINGTYPES_H
#define HUNTINGTYPES_H

#include <ModelTypes.hh>
#include <SerializableEnum.h>
#include <access_pch.h>

namespace kd417d {
namespace eva {

namespace hunting {

enum PlayerType : int
{
    HUNTER,
    PREY
};


using PlayerCoordinates = QVector<QPair<DimensionQ, PlayerType>>;
using PlayerIdPair = QPair<DimensionQ, PlayerType>;
using PlayerCoordinatesPtr = std::shared_ptr<PlayerCoordinates>;

}
}
}
#endif // HUNTINGTYPES_H
