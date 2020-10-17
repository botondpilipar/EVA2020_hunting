#include <BoardUtility.h>
#include "HuntingBoard.h"

using namespace kd417d::eva;


HuntingBoard::HuntingBoard(Serializer& serializer, DimensionQ& dimension)
    : HuntingBoard(serializer)
{
    mDimension = dimension;
    assert(dimension.first == dimension.second);
    mMaxSteps = dimension.first;
    mStepsTaken = 0;
    mMiddle = BoardUtility::middlePosition(dimension);
    mCorners = BoardUtility::cornerPositions(dimension);

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
}

HuntingBoardData* HuntingBoard::save() const
{
    return new HuntingBoardData(mDimension,
                                mMaxSteps,
                                mStepsTaken,
                                mCurrentlyMoving,
                                mPlayerMap);
}

void HuntingBoard::setDimensions(DimensionQ &dimension)
{
    mDimension = dimension;
    mMiddle = BoardUtility::middlePosition(mDimension);
    mCorners = BoardUtility::cornerPositions(mDimension);
    mMaxSteps = mDimension.first;
    ;
}
void HuntingBoard::startNewGame()
{
    mStepsTaken = 0;
    mIsPaused = false;
    mPlayerMap.clear();
    mPlayerMap.push_back(QPair<DimensionQ, PlayerType>(mMiddle, PlayerType::PREY));

    for(auto position : mCorners) { mPlayerMap.push_back(QPair<DimensionQ, PlayerType>(position, PlayerType::PREY)); }
    emit boardChangedSignal(PlayerCoordinatesPtr(new PlayerCoordinates(mPlayerMap)));
}

void HuntingBoard::pauseGame() { mIsPaused = true; }
void HuntingBoard::continueGame() { mIsPaused = false; }

void HuntingBoard::movePlayer(DimensionQ& from, DimensionQ& to)
{
    if(mIsPaused) { return; }
    if(!BoardUtility::isNeighboring(from, to)) { return; }
    
    auto moveCandidate = std::find(mPlayerMap.begin(), mPlayerMap.end(), PlayerIdPair(from, mCurrentlyMoving));
    
    if(moveCandidate == mPlayerMap.end()) { return; }
    if(moveCandidate->second != mCurrentlyMoving) { return; }

    auto secondCandidate = std::find(++moveCandidate, mPlayerMap.end(), PlayerIdPair(from, mCurrentlyMoving));

    // Another player has already occupied this spot
    const bool isSpotOccupied = secondCandidate != mPlayerMap.end();
    const bool isGameOverScenario = isSpotOccupied && moveCandidate->second != secondCandidate->second;

    if(!isSpotOccupied)
    {
        moveCandidate->first = to;
        ++mStepsTaken;
    }

    if(mMaxSteps == mStepsTaken)
    {
        emit gameOverSignal(mStepsTaken, isGameOverScenario ? PlayerType::HUNTER :
                                                               PlayerType::PREY);
    }

    mCurrentlyMoving = getNextMove(mCurrentlyMoving);
}

PlayerType HuntingBoard::getNextMove(PlayerType type)
{
    switch (type)
    {
        case PlayerType::HUNTER:
            return PlayerType::PREY;
        case PlayerType::PREY:
            return PlayerType::HUNTER;
    }
}
