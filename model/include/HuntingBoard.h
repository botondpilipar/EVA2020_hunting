#ifndef HUNTINGBOARD_H
#define HUNTINGBOARD_H

#include <QObject>

#include <ModelTypes.hh>
#include <HuntingTypes.h>
#include <IGameBoard.h>
#include <HuntingBoardData.h>
#include <Serializer.h>
#include <CircularVariable.hpp>
#include <BoardUtility.h>

namespace kd417d {
namespace eva {

using namespace hunting;

class HuntingBoard : public QObject,
                    public logic::IGameBoard<HuntingBoardData>
{
    Q_OBJECT
public:
    explicit HuntingBoard(QObject* parent = nullptr);

    HuntingBoard(Serializer& serializer)
        : mSerializer(serializer),
          mIsPaused(false),
        mCurrentlyMoving(PlayerType::HUNTER)
    {}

    HuntingBoard(Serializer& serializer, DimensionQ& dimension);


    //IGameBoard
    virtual DimensionQ getDimensions() const override { return mDimension; }
    virtual void setDimensions(const DimensionQ& dimension) override;

    //IDataInitializable
    virtual void initialize(const HuntingBoardData& representation) override;

    virtual HuntingBoardData* save() const override;

    //IGameControlUnit
    virtual void startNewGame() override;
    virtual void pauseGame() override;
    virtual void continueGame() override;

    //ISettingsChangedObserver
    virtual void settingsChanged() override {}

    //Own methods
    virtual void movePlayer(const DimensionQ& from, const DimensionQ& to);
    virtual bool isGameOverScenario() const;
    virtual inline hunting::PlayerType getCurrentlyMoving() const { return mCurrentlyMoving; }
    virtual bool isOccupied(const DimensionQ& spot) const;

    virtual bool saveFile(const QString& fileName);
    virtual bool loadFile(const QString& fileName);

    static hunting::PlayerType getNextMove(hunting::PlayerType);


private:
    Serializer& mSerializer;
    DimensionQ mDimension;
    PlayerCoordinates mPlayerMap;
    quint64 mStepsTaken;
    quint64 mMaxSteps;
    bool mIsPaused;
    hunting::PlayerType mCurrentlyMoving;
    BoardUtility mUtility;

    DimensionQ mMiddle;
    QVector<DimensionQ> mCorners;
signals:
    void gameOverSignal(quint64 finalSteps, PlayerType winningSide);
    void boardChangedSignal(PlayerCoordinatesPtr newSetup);
    void stepsTakenChangedSignal(quint64 stepsTaken);
    void dimensionChangedSignal(DimensionQ newDimensions);
    void newGameSignal(PlayerCoordinatesPtr newSetup);

};
}
}


#endif // HUNTINGBOARD_H
