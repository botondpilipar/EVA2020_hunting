#include <BridgeCrossingPlayer.h>
#include <UnimplementedException.h>
#include <SingletonFactory.hpp>
#include <CachedRandomDevice.h>

using namespace kd417d::eva::logic;

const std::map<BridgeCrossingTypes::PlayerActionSet,
    const std::set<std::pair<BridgeCrossingTypes::PlayerState, BridgeCrossingTypes::PlayerState>>> playerStateTransitionMap =
{
    { BridgeCrossingTypes::PlayerActionSet::CROSS,
      {
          { BridgeCrossingTypes::PlayerState::SELECTED_TO_CROSS, BridgeCrossingTypes::PlayerState::ON_RETURNING_SIDE }
      }
    },
    { BridgeCrossingTypes::PlayerActionSet::MOVE_TO_BRIDGE,
      {
          { BridgeCrossingTypes::PlayerState::SELECTED_TO_CROSS, BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE},

          { BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE, BridgeCrossingTypes::PlayerState::SELECTED_TO_CROSS},

          { BridgeCrossingTypes::PlayerState::SELECTED_TO_RETURN, BridgeCrossingTypes::PlayerState::ON_RETURNING_SIDE},

          { BridgeCrossingTypes::PlayerState::ON_RETURNING_SIDE, BridgeCrossingTypes::PlayerState::SELECTED_TO_RETURN}
      }
    },

    { BridgeCrossingTypes::PlayerActionSet::RETURN,
      {
          { BridgeCrossingTypes::PlayerState::SELECTED_TO_RETURN, BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE }
      }
    }
};


BridgeCrossingPlayer::BridgeCrossingPlayer(Identifier uniqueId,
                                           BridgeCrossingTypes::PlayerState state,
                                           BridgeCrossingTypes::PlayerType type)
    : mUniqueId(uniqueId),
      mState(state),
      mType(type),
      mSettings(SingletonFactory<BridgeCrossingSettings>::instance())
{
    switch(mType)
    {
        case BridgeCrossingTypes::PlayerType::SLOW:
        {
            mCrossSpeed = mSettings.getSlowPlayerCrossTime();
            break;
        }
        case BridgeCrossingTypes::PlayerType::MEDIUM:
        {
            mCrossSpeed = mSettings.getMediumPlayerCrossTime();
            break;
        }
        case BridgeCrossingTypes::PlayerType::FAST:
        {
            mCrossSpeed = mSettings.getFastPlayerCrossTime();
            break;
        }
    }
}

// IGameActor
 void
 BridgeCrossingPlayer::performAction(BridgeCrossingTypes::PlayerActionSet action)
 {
    auto transitionSetIt = playerStateTransitionMap.find(action);
    auto newStateIt = std::find_if(transitionSetIt->second.cbegin(),
                                   transitionSetIt->second.cend(),
                                   [this](const auto& transitionPair) { return transitionPair.first == mState; });
    if(newStateIt != transitionSetIt->second.cend())
    {
        mState = newStateIt->second;
        emit actionPerformedSignal(action);
    }
 }

// IMovableObject
 void
 BridgeCrossingPlayer::move(QPair<int, int>)
 {
    throw UnimplementedException("BridgeCrossingPlayer", "move");
 }

 QPair<int, int> BridgeCrossingPlayer::getPositionOnBoard() const
 {
     throw UnimplementedException("BridgeCrossingPlayer", "getPositionOnBoard");
 }

// IDataInitializable
 void
 BridgeCrossingPlayer::initialize(const PlayerData& representation)
 {
     mUniqueId  = representation.id;
     mType = representation.type;
     mCrossSpeed = representation.speed;
     mState = representation.state;
 }

 PlayerData*
 BridgeCrossingPlayer::save() const
 {
     return new PlayerData(mState,
                           mUniqueId,
                           mType,
                           mCrossSpeed);
 }

// ISettingsChangedObserver

void BridgeCrossingPlayer::settingsChanged()
{
    switch(mType)
    {
        case BridgeCrossingTypes::PlayerType::SLOW:
        {
            mCrossSpeed = mSettings.getSlowPlayerCrossTime();
        }
        case BridgeCrossingTypes::PlayerType::MEDIUM:
        {
            mCrossSpeed = mSettings.getMediumPlayerCrossTime();
        }
        case BridgeCrossingTypes::PlayerType::FAST:
        {
            mCrossSpeed = mSettings.getFastPlayerCrossTime();
        }
    }
}
