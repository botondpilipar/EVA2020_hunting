#ifndef BRIDGECROSSINGBOARD_H
#define BRIDGECROSSINGBOARD_H

#include "IGameBoard.h"
#include "BridgeCrossingBoardData.h"
#include "BridgeCrossingPlayer.h"
#include "BridgeCrossingTypes.h"

#include <CachedRandomDevice.h>

using namespace kd417d::eva::logic;
using PlayerIdMap  = QMap<Identifier, std::shared_ptr<PlayerData>>;

namespace kd417d
{
namespace eva
{
namespace logic
{
using namespace bridge;

class BridgeCrossingBoard : public QObject,
                            public IGameBoard<BoardData>
{
    Q_OBJECT

    Q_PROPERTY(BridgeCrossingTypes::GameState mGameState
               READ getGameState);
    Q_PROPERTY(ScoredPoint mTimeEllapsed
               READ getTimeEllapsed
               NOTIFY scoredPointChangedSignal)
public:

    BridgeCrossingBoard();
    virtual ~BridgeCrossingBoard() override;

    virtual void movePlayer(Identifier uniquePlayerId);
    virtual void cross();
    virtual BridgeCrossingTypes::GameState getGameState() const;
    virtual ScoredPoint getTimeEllapsed() const;
    virtual bool isGameOverScenario() const;

    // IGameBoard
    virtual Dimension2D getDimensions() const override;
    virtual void setDimensions(Dimension2D&) override;

    // IGameControlUnit
    virtual void startNewGame() override;
    virtual void pauseGame() override;
    virtual void continueGame() override;

    // ISettingsChangedObserver
    virtual void settingsChanged() override;

    // IDataInitializable
    virtual void initialize(const BoardData&) override;
    virtual BoardData* save() const override;

signals:
    void scoredPointChangedSignal(ScoredPoint newScore);
    void gameOverSignal(ScoredPoint finalScore);
    void newGameSignal();
    void boardChangedSignal(PlayerIdMap map);

protected:
    QVector<std::shared_ptr<BridgeCrossingPlayer>> mPlayers;
    QMap<int, std::shared_ptr<BridgeCrossingPlayer>> mPlayerIdMap;
    ScoredPoint mTimeEllapsed;
    BridgeCrossingTypes::GameState mGameState;

    void connectToPlayers();
    void disconnectFromPlayers();

private:
    BridgeCrossingSettings& mSettings;
    CachedRandomDevice& mRandomDevice;
    QVector<BridgeCrossingPlayer*> mBridgeBuffer;
    bool mIsPaused;

    bool isBridgeBufferFull() const { return mBridgeBuffer.size() ==
                                   BridgeCrossingSettingDefaults::playersCanCrossAtOneTime; }


    CrossSpeed maxBridgeCrosserSpeed() const;
    void resetPlayerOnIllegalMove(Identifier id);

protected slots:
    void onPlayerActionPerformed(BridgeCrossingTypes::PlayerActionSet action);
};
}
}
}

Q_DECLARE_METATYPE(PlayerIdMap)
Q_DECLARE_METATYPE(PlayerData)
Q_DECLARE_METATYPE(ScoredPoint)


#endif // BRIDGECROSSINGBOARD_H
