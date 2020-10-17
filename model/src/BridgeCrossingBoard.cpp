#include <BridgeCrossingBoard.h>
#include <SingletonFactory.hpp>
#include <UnimplementedException.h>

using namespace kd417d::eva::logic;


const std::map<BridgeCrossingTypes::PlayerActionSet, BridgeCrossingTypes::GameState> boardStateTransitionMap =
{
    { BridgeCrossingTypes::PlayerActionSet::CROSS, BridgeCrossingTypes::GameState::RETURN_SELECTION },
    { BridgeCrossingTypes::PlayerActionSet::RETURN, BridgeCrossingTypes::GameState::CROSS_SELECTION }
};

const std::map<BridgeCrossingTypes::PlayerState, BridgeCrossingTypes::GameState> legalPlayerStateMap =
{
    { BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE, BridgeCrossingTypes::GameState::CROSS_SELECTION },
    { BridgeCrossingTypes::PlayerState::SELECTED_TO_CROSS, BridgeCrossingTypes::GameState::CROSS_SELECTION },
    { BridgeCrossingTypes::PlayerState::ON_RETURNING_SIDE, BridgeCrossingTypes::GameState::RETURN_SELECTION },
    { BridgeCrossingTypes::PlayerState::SELECTED_TO_RETURN, BridgeCrossingTypes::GameState::RETURN_SELECTION },

};

const std::map<BridgeCrossingTypes::PlayerState, BridgeCrossingTypes::PlayerState> reversePlayerStateMap =
{
    { BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE, BridgeCrossingTypes::PlayerState::SELECTED_TO_CROSS },
    { BridgeCrossingTypes::PlayerState::SELECTED_TO_CROSS, BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE },
    { BridgeCrossingTypes::PlayerState::ON_RETURNING_SIDE, BridgeCrossingTypes::PlayerState::SELECTED_TO_RETURN },
    { BridgeCrossingTypes::PlayerState::SELECTED_TO_RETURN, BridgeCrossingTypes::PlayerState::ON_RETURNING_SIDE },
};

BridgeCrossingBoard::BridgeCrossingBoard()
    : mGameState(BridgeCrossingTypes::GameState::CROSS_SELECTION),
      mSettings(SingletonFactory<BridgeCrossingSettings>::instance()),
      mRandomDevice(SingletonFactory<CachedRandomDevice>::instance())
{
    mBridgeBuffer.resize(BridgeCrossingSettingDefaults::playersCanCrossAtOneTime);

    for(unsigned i = 0; i<mSettings.getSlowPlayerNumber(); ++i)
    {
        Identifier id = mRandomDevice.random();
        std::shared_ptr<BridgeCrossingPlayer> player = std::make_shared<BridgeCrossingPlayer>(id,
                                                                BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE,
                                                                BridgeCrossingTypes::PlayerType::SLOW);
        mPlayerIdMap.insert(id, player);
        mPlayers.append(player);
    }
    for(unsigned i = 0; i<mSettings.getMediumPlayerNumber(); ++i)
    {
        Identifier id = mRandomDevice.random();
        std::shared_ptr<BridgeCrossingPlayer> player = std::make_shared<BridgeCrossingPlayer>(id,
                                                                BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE,
                                                                BridgeCrossingTypes::PlayerType::MEDIUM);
        mPlayerIdMap.insert(id, player);
        mPlayers.append(player);
    }
    for(unsigned i = 0; i<mSettings.getFastPlayerNumber(); ++i)
    {
        Identifier id = mRandomDevice.random();
        std::shared_ptr<BridgeCrossingPlayer> player = std::make_shared<BridgeCrossingPlayer>(id,
                                                                BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE,
                                                                BridgeCrossingTypes::PlayerType::FAST);
        mPlayerIdMap.insert(id, player);
        mPlayers.append(player);
    }

    connectToPlayers();
}

BridgeCrossingBoard::~BridgeCrossingBoard()
{
}

void
BridgeCrossingBoard::cross()
{
    if(mIsPaused)
    {
        return;
    }

    if(!mBridgeBuffer.empty())
    {
        PlayerIdMap boardChange{};

        mTimeEllapsed += maxBridgeCrosserSpeed();

        auto action = mGameState == BridgeCrossingTypes::GameState::CROSS_SELECTION ?
                BridgeCrossingTypes::PlayerActionSet::CROSS :
                    BridgeCrossingTypes::PlayerActionSet::RETURN;
        for(const auto& player : mBridgeBuffer)
        {
            player->performAction(action);
            boardChange.insert(player->getUniqueId(), std::shared_ptr<PlayerData>(player->save()));
        }
        mBridgeBuffer.clear();

        if(isGameOverScenario())
        {
            emit gameOverSignal(mTimeEllapsed);
            startNewGame();
        }
        else
        {
            emit scoredPointChangedSignal(mTimeEllapsed);
            emit boardChangedSignal(boardChange);
        }
    }
}

void
BridgeCrossingBoard::movePlayer(Identifier uniquePlayerId)
{
    if(mIsPaused)
    {
        return;
    }
    if(!mPlayerIdMap.contains(uniquePlayerId))
    {
        return;
    }

    mPlayerIdMap.value(uniquePlayerId)->performAction(BridgeCrossingTypes::PlayerActionSet::MOVE_TO_BRIDGE);
}

Dimension2D
BridgeCrossingBoard::getDimensions() const
{
    throw UnimplementedException("BridgeCrossingBoard", "getDimensions");
}

void
BridgeCrossingBoard::setDimensions(Dimension2D&)
{
    throw UnimplementedException("BridgeCrossingBoard", "setDimensions");
}

BridgeCrossingTypes::GameState BridgeCrossingBoard::getGameState() const
{
    return mGameState;
}

ScoredPoint BridgeCrossingBoard::getTimeEllapsed() const
{
    return mTimeEllapsed;
}

void
BridgeCrossingBoard::startNewGame()
{
    mTimeEllapsed = 0;
    mBridgeBuffer.clear();
    mGameState = BridgeCrossingTypes::GameState::CROSS_SELECTION;
    mIsPaused = false;
    PlayerIdMap playerIdData{};

    for(const auto& player : mPlayers)
    {
        player->setPlayerState(BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE);
        PlayerData* data = player->save();
        playerIdData.insert(player->getUniqueId(), std::shared_ptr<PlayerData>(data));
    }

    emit newGameSignal();
    emit boardChangedSignal(playerIdData);
}

void
BridgeCrossingBoard::pauseGame()
{
    if(mIsPaused)
    {
        return;
    }
    disconnectFromPlayers();
    mIsPaused = true;
}

void
BridgeCrossingBoard::continueGame()
{
    if(!mIsPaused)
    {
        return;
    }
    connectToPlayers();
    mIsPaused = false;
}

void
BridgeCrossingBoard::settingsChanged()
{
    mPlayers.resize(mSettings.allPlayers());
    mPlayers.clear();

    for(unsigned i = 0; i<mSettings.getSlowPlayerNumber(); ++i)
    {
        Identifier id = mRandomDevice.random();
        std::shared_ptr<BridgeCrossingPlayer> player = std::make_shared<BridgeCrossingPlayer>(id,
                                                                BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE,
                                                                BridgeCrossingTypes::PlayerType::SLOW);
        mPlayerIdMap.insert(id, player);
        mPlayers.push_back(player);
    }
    for(unsigned i = 0; i<mSettings.getMediumPlayerNumber(); ++i)
    {
        Identifier id = mRandomDevice.random();
        std::shared_ptr<BridgeCrossingPlayer> player = std::make_shared<BridgeCrossingPlayer>(id,
                                                                BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE,
                                                                BridgeCrossingTypes::PlayerType::MEDIUM);
        mPlayerIdMap.insert(id, player);
        mPlayers.push_back(player);
    }
    for(unsigned i = 0; i<mSettings.getFastPlayerNumber(); ++i)
    {
        Identifier id = mRandomDevice.random();
        std::shared_ptr<BridgeCrossingPlayer> player = std::make_shared<BridgeCrossingPlayer>(id,
                                                                BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE,
                                                                BridgeCrossingTypes::PlayerType::FAST);
        mPlayerIdMap.insert(id, player);
        mPlayers.push_back(player);
    }

    connectToPlayers();
    startNewGame();
}

void
BridgeCrossingBoard::onPlayerActionPerformed(BridgeCrossingTypes::PlayerActionSet action)
{
    if(mIsPaused)
    {
        return;
    }

    BridgeCrossingPlayer* sender = qobject_cast<BridgeCrossingPlayer*>(QObject::sender());

    auto transitionIt = boardStateTransitionMap.find(action);
    if(transitionIt != boardStateTransitionMap.end())
    {
        mGameState = transitionIt->second;
        return;
    }

    auto legalGameState = legalPlayerStateMap.find(sender->getPlayerState());
    if(legalGameState->second != mGameState)
    {
        resetPlayerOnIllegalMove(sender->getUniqueId());
        return;
    }
    PlayerIdMap changed{};

    // When player is moved at bridge

    switch(sender->getPlayerState())
    {
        case BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE:
        case BridgeCrossingTypes::PlayerState::ON_RETURNING_SIDE:
        {
            auto iteratorToSender = std::find(mBridgeBuffer.begin(), mBridgeBuffer.end(), sender);
            assert(iteratorToSender != mBridgeBuffer.end());

            mBridgeBuffer.erase(iteratorToSender);
            changed.insert(sender->getUniqueId(), std::shared_ptr<PlayerData>(sender->save()));
            emit boardChangedSignal(changed);
            break;
        }
        case BridgeCrossingTypes::PlayerState::SELECTED_TO_CROSS:
        case BridgeCrossingTypes::PlayerState::SELECTED_TO_RETURN:
        {
            if(isBridgeBufferFull())
            {
                resetPlayerOnIllegalMove(sender->getUniqueId());
            }
            else
            {
                mBridgeBuffer.push_back(sender);
                changed.insert(sender->getUniqueId(), std::shared_ptr<PlayerData>(sender->save()));
                emit boardChangedSignal(changed);
            }
            break;
        }
    }
}

void BridgeCrossingBoard::initialize(const BoardData& data)
{
    mTimeEllapsed = data.point;
    mGameState = data.state;
    mPlayerIdMap.clear();
    mPlayers.clear();
    PlayerIdMap changed;

    for(const auto& playerDataPtr : data.playerData)
    {
        std::shared_ptr<BridgeCrossingPlayer> newPlayer
                = std::make_shared<BridgeCrossingPlayer>(0,
                                                        BridgeCrossingTypes::PlayerState::ON_CROSSING_SIDE,
                                                        BridgeCrossingTypes::PlayerType::FAST);
        newPlayer->initialize(*playerDataPtr);
        mPlayers.push_back(newPlayer);
        mPlayerIdMap.insert(newPlayer->getUniqueId(), newPlayer);
        changed.insert(newPlayer->getUniqueId(), playerDataPtr);
    }

    connectToPlayers();
    emit boardChangedSignal(changed);
}

BoardData* BridgeCrossingBoard::save() const
{
    QVector<std::shared_ptr<PlayerData>> playerData{};

    std::transform(mPlayers.begin(), mPlayers.end(), std::back_inserter(playerData),
                   [](const auto& player){ return std::shared_ptr<PlayerData>(player->save()); });

    return new BoardData(playerData,
                     mTimeEllapsed,
                     mPlayers.size(),
                     mGameState);
}

void
BridgeCrossingBoard::connectToPlayers()
{
    for(const auto& player : mPlayers)
    {
        QObject::connect(player.get(), &BridgeCrossingPlayer::actionPerformedSignal,
                         this, &BridgeCrossingBoard::onPlayerActionPerformed);
    }
}

void BridgeCrossingBoard::disconnectFromPlayers()
{
    for(const auto& player : mPlayers)
    {
        QObject::disconnect(player.get(), &BridgeCrossingPlayer::actionPerformedSignal,
                         this, &BridgeCrossingBoard::onPlayerActionPerformed);
    }
}

CrossSpeed
BridgeCrossingBoard::maxBridgeCrosserSpeed() const
{
    QVector<CrossSpeed> speeds{};
    std::transform(mBridgeBuffer.begin(), mBridgeBuffer.end(), std::back_inserter(speeds),
                   [](const auto& p){ return p->getCrossSpeed(); });
    CrossSpeed max = *std::max_element(speeds.begin(), speeds.end());
    return max;
}

bool
BridgeCrossingBoard::isGameOverScenario() const
{
    return std::all_of(mPlayers.begin(), mPlayers.end(),
                       [](const auto& player) {
            return player->getPlayerState() == BridgeCrossingTypes::PlayerState::ON_RETURNING_SIDE;});
}

void
BridgeCrossingBoard::resetPlayerOnIllegalMove(Identifier id)
{
    std::shared_ptr<BridgeCrossingPlayer> player = mPlayerIdMap.value(id);
    auto reversedState = reversePlayerStateMap.find(player->getPlayerState())->second;
    player->setPlayerState(reversedState);
}
