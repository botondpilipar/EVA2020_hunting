#ifndef BRIDGECROSSINGBOARDDATA_H
#define BRIDGECROSSINGBOARDDATA_H

#include <model_pch.h>
#include <ISerializable.h>
#include <IDeserializable.h>
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

struct PlayerData;

struct BoardData : public ISerializable,
                   public IDeserializable
{
    virtual ~BoardData() override = default;

    BoardData();
    BoardData(QVector<std::shared_ptr<PlayerData>> playerData,
              ScoredPoint point,
              unsigned int playerNumber,
              BridgeCrossingTypes::GameState state);

    BoardData(const BoardData& other);
    BoardData(BoardData&& other);

    BoardData& operator=(const BoardData& other);

    BoardData& operator=(const BoardData&& other);

    virtual void deserialize(ISerializationSource&) override;

    virtual void serialize(ISerializationTarget&) override;

    bool operator==(const BoardData& other) const;

    ScoredPoint point;
    BridgeCrossingTypes::GameState state;
    unsigned int playerNumber;
    QVector<std::shared_ptr<PlayerData>> playerData;

};

}

}
}
}

#endif // BRIDGECROSSINGBOARDDATA_H
