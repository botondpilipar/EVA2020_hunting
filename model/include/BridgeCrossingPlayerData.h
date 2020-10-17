#ifndef BRIDGECROSSINGPLAYERDATA_H
#define BRIDGECROSSINGPLAYERDATA_H

#include <ISerializable.h>
#include <IDeserializable.h>
#include <DeserializationFailedException.h>

#include "BridgeCrossingTypes.h"

namespace kd417d
{
namespace eva
{
namespace logic
{
namespace bridge
{

using namespace kd417d::eva::access;

struct PlayerData : public ISerializable, public IDeserializable
{
    virtual ~PlayerData() override = default;

    PlayerData();

    PlayerData(BridgeCrossingTypes::PlayerState state,
               Identifier id,
               BridgeCrossingTypes::PlayerType type,
               CrossSpeed speed)
        : state(state), id(id), type(type), speed(speed) {}

    PlayerData(const PlayerData&) = default;
    PlayerData(PlayerData&&) = default;
    PlayerData& operator=(const PlayerData&) = default;
    PlayerData& operator=(PlayerData&&) = default;

    BridgeCrossingTypes::PlayerState state;
    Identifier id;
    BridgeCrossingTypes::PlayerType type;
    CrossSpeed speed;

    virtual void deserialize(ISerializationSource&) override;

    virtual void serialize(ISerializationTarget&) override;

    bool operator==(const PlayerData& other) const;
};

}


}
}
}
#endif // BRIDGECROSSINGPLAYERDATA_H
