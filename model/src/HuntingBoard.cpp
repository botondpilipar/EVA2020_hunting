#include <BoardUtility.h>
#include "HuntingBoard.h"

using namespace kd417d::eva;


HuntingBoard::HuntingBoard(Serializer& serializer, DimensionQ& dimension)
    : HuntingBoard(serializer)
{
    mDimension = dimension;
    assert(dimension.first == dimension.second);
    mMaxSteps = 4*dimension.first;
    mStepsTaken = 0;
    mMiddle = BoardUtility::middlePosition(dimension);
    mCorners = BoardUtility::cornerPositions(dimension);
    mCurrentlyMoving = hunting::PlayerType::HUNTER;
}

void HuntingBoard::initialize(const HuntingBoardData& representation)
{
    mDimension = representation.dimension;
    mPlayerMap = representation.playerSet;
    mCurrentlyMoving = representation.nextToMove;
    mMaxSteps = representation.maxStep;
    mStepsTaken = representation.stepsTaken;
    mMiddle = BoardUtility::middlePosition(mDimension);
    mCorners = BoardUtility::cornerPositions(mDimension);

    emit dimensionChangedSignal(mDimension);
    emit boardChangedSignal(std::make_shared<PlayerCoordinates>(mPlayerMap));
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
    mMiddle = BoardUtility::middlePosition(mDimension);
    mCorners = BoardUtility::cornerPositions(mDimension);
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
    if(!BoardUtility::isValidDimension(from, mDimension) || !BoardUtility::isValidDimension(to, mDimension))
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
    const bool isGameOverScenario = isSpotOccupied && moveCandidate->second != secondCandidate->second;

    if(!isSpotOccupied)
    {
        moveCandidate->first = to;
        mCurrentlyMoving = getNextMove(mCurrentlyMoving);
        emit boardChangedSignal(std::make_shared<PlayerCoordinates>(1, *moveCandidate));
        emit stepsTakenChangedSignal(++mStepsTaken);
    }
    else if(isGameOverScenario)
    {
        ++mStepsTaken;
        emit gameOverSignal(mStepsTaken, PlayerType::HUNTER);
    }

    if(mMaxSteps == mStepsTaken)
    {
        emit gameOverSignal(mStepsTaken, isGameOverScenario ? PlayerType::HUNTER :
                                                               PlayerType::PREY);
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
