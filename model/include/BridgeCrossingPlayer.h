#ifndef BRIDGECROSSINGPLAYER_H
#define BRIDGECROSSINGPLAYER_H

#include "IGameActor.hpp"
#include "BridgeCrossingPlayerData.h"
#include "BridgeCrossingTypes.h"
#include <BridgeCrossingSettings.h>

namespace kd417d
{
namespace eva
{
namespace logic
{
using namespace bridge;

class BridgeCrossingPlayer : public QObject,
                             public IGameActor<PlayerData, BridgeCrossingTypes::PlayerActionSet>
{
    Q_OBJECT;

    Q_PROPERTY(BridgeCrossingTypes::PlayerState mState
               READ getPlayerState
               WRITE setPlayerState
               NOTIFY notifyStateChangedSignal)
    Q_PROPERTY(Identifier mUniqueId
               READ getUniqueId)
    Q_PROPERTY(BridgeCrossingTypes::PlayerType mType
               READ getPlayerType)
    Q_PROPERTY(CrossSpeed mCrossSpeed
               READ getCrossSpeed)
public:
    virtual ~BridgeCrossingPlayer() override = default;

    BridgeCrossingPlayer(Identifier uniqueId,
                         BridgeCrossingTypes::PlayerState state,
                         BridgeCrossingTypes::PlayerType type);

    inline BridgeCrossingTypes::PlayerState getPlayerState() const { return mState; }
    inline void setPlayerState(BridgeCrossingTypes::PlayerState state)
    {
        mState = state;
        emit notifyStateChangedSignal(state);
    }

    inline BridgeCrossingTypes::PlayerType getPlayerType() const { return mType; }

    inline CrossSpeed getCrossSpeed() const { return mCrossSpeed; }

    // IGameActor
    virtual void performAction(BridgeCrossingTypes::PlayerActionSet action) override;
    virtual Identifier getUniqueId() const override { return mUniqueId; }

    // IMovableObject
    virtual void move(QPair<int, int> newPosition) override;
    virtual QPair<int, int> getPositionOnBoard() const override;

    // IDataInitializable
    virtual void initialize(const PlayerData& representation) override;
    virtual PlayerData* save() const override;

    // ISettingsChangedObserver

    virtual void settingsChanged() override;
signals:
    void actionPerformedSignal(BridgeCrossingTypes::PlayerActionSet action);
    void notifyStateChangedSignal(BridgeCrossingTypes::PlayerState state);


private:
    Identifier mUniqueId;
    BridgeCrossingTypes::PlayerState mState;
    BridgeCrossingTypes::PlayerType mType;
    BridgeCrossingSettings& mSettings;
    CrossSpeed mCrossSpeed;

};

}
}
}
#endif // BRIDGECROSSINGPLAYER_H
