#ifndef HUNTINGTYPES_H
#define HUNTINGTYPES_H

#include <ModelTypes.hh>
#include <SerializableEnum.h>
#include <access_pch.h>
#include <QMetaType>

namespace kd417d {
namespace eva {

namespace hunting {

enum PlayerType : int
{
    HUNTER,
    PREY,
    EITHER
};


using PlayerIdPair = QPair<DimensionQ, PlayerType>;
using PlayerCoordinates = QVector<PlayerIdPair>;
using PlayerCoordinatesPtr = std::shared_ptr<PlayerCoordinates>;


}
}
}

using namespace kd417d::eva::hunting;
Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr);
Q_DECLARE_METATYPE(PlayerCoordinatesPtr);
Q_DECLARE_METATYPE(PlayerCoordinates);
Q_DECLARE_METATYPE(PlayerIdPair);
Q_DECLARE_METATYPE(PlayerType);


#endif // HUNTINGTYPES_H
