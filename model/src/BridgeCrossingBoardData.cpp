#include <BridgeCrossingBoardData.h>
#include <ISerializationSource.h>
#include <ISerializationTarget.h>
#include <BridgeCrossingPlayerData.h>
#include <SingletonFactory.hpp>
#include <BridgeCrossingSettings.h>

using namespace kd417d::eva;
using namespace kd417d::eva::logic::bridge;

BoardData::BoardData() : point(0),
    state(BridgeCrossingTypes::GameState::CROSS_SELECTION),
    playerNumber(SingletonFactory<BridgeCrossingSettings>::instance().allPlayers()),
    playerData()
{}
BoardData::BoardData(QVector<std::shared_ptr<PlayerData>> playerData,
          ScoredPoint point,
          unsigned int playerNumber,
          BridgeCrossingTypes::GameState state)
    : point(point),
      state(state),
      playerNumber(playerNumber),
      playerData(playerData)
      {}

BoardData::BoardData(const BoardData& other)
    : point(other.point),
      state(other.state),
      playerNumber(other.playerNumber),
      playerData(other.playerData)
      {}

BoardData::BoardData(BoardData&& other)
    : point(std::move(other.point)),
      state(std::move(other.state)),
      playerNumber(std::move(other.playerNumber)),
      playerData(std::move(other.playerData))
      {}

BoardData& BoardData::operator=(const BoardData& other)
{
    this->point = other.point;
    this->state = other.state;
    this->playerNumber = other.playerNumber;
    this->playerData = other.playerData;
    return *this;
}

BoardData& BoardData::operator=(const BoardData&& other)
{
    this->point = std::move(other.point);
    this->state = std::move(other.state);
    this->playerNumber = std::move(other.playerNumber);
    this->playerData = std::move(other.playerData);
    return *this;
}
void
BoardData::deserialize(ISerializationSource& source)
{
    std::tuple singleDataDeserialized = { source.deserializeUnsigned(),
                                          source.deserializeInt(),
                                          source.deserializeUnsigned()};

    auto [point, state, playerNumber] = singleDataDeserialized;
    if(point.has_value() && state.has_value() && playerNumber.has_value())
    {
        this->point = point.value();
        this->state = static_cast<BridgeCrossingTypes::GameState>(state.value());
        this->playerNumber = playerNumber.value();
        std::vector<PlayerData> players(this->playerNumber);

        for(unsigned i = 0; i<playerNumber; ++i)
        {
            
        }
    }
}

void
BoardData::serialize(ISerializationTarget& target)
{
    return;
}

bool
BoardData::operator==(const BoardData& other) const
{
    return other.playerNumber == this->playerNumber &&
            other.point == this->point &&
            other.state == this->state &&
            std::equal(other.playerData.begin(), other.playerData.end(), this->playerData.begin());
}
