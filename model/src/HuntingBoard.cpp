#include "HuntingBoard.h"

using namespace kd417d::eva;


HuntingBoard::HuntingBoard(Serializer& serializer, DimensionQ& dimension)
    : HuntingBoard(serializer)
{
    mDimension = dimension;
    mUtility.setDimensions(dimension);
    assert(dimension.first == dimension.second);
    mMaxSteps = 4*dimension.first;
    mStepsTaken = 0;
    mMiddle = mUtility.middlePosition();
    mCorners = mUtility.BoardUtility::cornerPositions();
    mCurrentlyMoving = hunting::PlayerType::HUNTER;
}

void HuntingBoard::initialize(const HuntingBoardData& representation)
{
    mDimension = representation.dimension;
    mUtility.setDimensions(mDimension);
    mPlayerMap = representation.playerSet;
    mCurrentlyMoving = representation.nextToMove;
    mMaxSteps = representation.maxStep;
    mStepsTaken = representation.stepsTaken;
    mMiddle = mUtility.middlePosition();
    mCorners = mUtility.cornerPositions();

    emit dimensionChangedSignal(mDimension);
    emit newGameSignal(std::make_shared<PlayerCoordinates>(mPlayerMap));
    emit stepsTakenChangedSignal(mStepsTaken);
}

HuntingBoardData* HuntingBoard::save() const
{
    return new HuntingBoardData(mDimension,
                                mMaxSteps,
                                mStepsTaken,
                                mCurrentlyMoving,
                                mPlayerMap);
}

void HuntingBoard::setDimensions(const DimensionQ &dimension)
{
    mDimension = dimension;
    mUtility.setDimensions(mDimension);
    mMiddle = mUtility.middlePosition();
    mCorners = mUtility.cornerPositions();
    mMaxSteps = 4*mDimension.first;

    emit dimensionChangedSignal(mDimension);
    startNewGame();
}
void HuntingBoard::startNewGame()
{
    mStepsTaken = 0;
    mIsPaused = false;
    mPlayerMap.clear();
    mPlayerMap.push_back(PlayerIdPair(mMiddle, PlayerType::PREY));
    mCurrentlyMoving = PlayerType::HUNTER;

    for(auto position : mCorners) { mPlayerMap.push_back(PlayerIdPair(position, PlayerType::HUNTER)); }

    emit stepsTakenChangedSignal(0);
    emit newGameSignal(PlayerCoordinatesPtr(new PlayerCoordinates(mPlayerMap)));
}

void HuntingBoard::pauseGame() { mIsPaused = true; }
void HuntingBoard::continueGame() { mIsPaused = false; }

void HuntingBoard::movePlayer(const DimensionQ& from, const DimensionQ& to)
{
    if(mIsPaused) { return; }
    if(!mUtility.isValidDimension(from) || !mUtility.isValidDimension(to))
    {
        return;
    }
    if(!BoardUtility::isNeighboring(from, to)) { return; }
    
    auto moveCandidate = std::find(mPlayerMap.begin(), mPlayerMap.end(), PlayerIdPair(from, mCurrentlyMoving));
    
    if(moveCandidate == mPlayerMap.end()) { return; }
    if(moveCandidate->second != mCurrentlyMoving) { return; }

    auto secondCandidate = std::find_if(mPlayerMap.begin(), mPlayerMap.end(),
                                        [&](const auto& playerIdPair) { return playerIdPair.first == to; });

    // Another player has already occupied this spot
    const bool isSpotOccupied = secondCandidate != mPlayerMap.end();
    bool haveReachedMaxSteps = false;
    bool isPlayerSurrounded = false;

    if(!isSpotOccupied)
    {
        moveCandidate->first = to;
        mCurrentlyMoving = getNextMove(mCurrentlyMoving);
        emit boardChangedSignal(std::make_shared<PlayerCoordinates>(1, *moveCandidate));
        emit stepsTakenChangedSignal(++mStepsTaken);
        haveReachedMaxSteps = (mStepsTaken == mMaxSteps);
    }

    if(!isSpotOccupied && moveCandidate->second == PlayerType::HUNTER)
    {
        // Check game over scenario whether prey cannot step
        auto prey = std::find_if(mPlayerMap.begin(), mPlayerMap.end(),
                                 [](const auto& player) { return player.second == PlayerType::PREY; });
        assert(prey != mPlayerMap.end());
        QList<DimensionQ> possibleSteps = mUtility.possibleSteps(prey->first);

        isPlayerSurrounded = std::all_of(possibleSteps.begin(), possibleSteps.end(),
                                                    [this](const DimensionQ& pos) { return this->isOccupied(pos); });
    }

    if(isPlayerSurrounded && haveReachedMaxSteps)
    {
        emit gameOverSignal(mStepsTaken, PlayerType::EITHER);
    }
    else if(isPlayerSurrounded)
    {
        emit gameOverSignal(mStepsTaken, PlayerType::HUNTER);
    }
    else if(haveReachedMaxSteps)
    {
        emit gameOverSignal(mStepsTaken, PlayerType::PREY);
    }

}

PlayerType HuntingBoard::getNextMove(PlayerType type)
{
    if(type == PlayerType::HUNTER) { return PlayerType::PREY; }
    else { return PlayerType::HUNTER; }

}

bool HuntingBoard::isGameOverScenario() const
{
    auto prey = std::find_if(mPlayerMap.begin(), mPlayerMap.end(),
                             [](const auto& p) { return p.second == PREY; });
    auto hunter = std::find_if(mPlayerMap.begin(), mPlayerMap.end(),
                               [prey](const auto& p) { return p.first == prey->first; });
    return (hunter != mPlayerMap.end());
}

bool HuntingBoard::isOccupied(const DimensionQ& spot) const
{
    return std::find_if(mPlayerMap.begin(), mPlayerMap.end(),
                        [spot](const auto& player) { return player.first == spot; })
            != mPlayerMap.end();
}
bool HuntingBoard::saveFile(const QString& fileName)
{
    try {
        std::unique_ptr<HuntingBoardData> data(this->save());
        mSerializer.serialize(*data, fileName);
    }  catch (std::exception e) {
        return false;
    }
    return true;
}

bool HuntingBoard::loadFile(const QString& fileName)
{
    HuntingBoardData data;
    bool success = mSerializer.deserialize(data, fileName);
    this->initialize(data);

    return success;
}
