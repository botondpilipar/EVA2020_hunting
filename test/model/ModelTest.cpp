#include <QtTest>
#include <QSignalSpy>


#include <model_pch.h>
#include <HuntingBoard.h>
#include <HuntingBoardData.h>
#include <HuntingTypes.h>

using namespace kd417d::eva;
class ModelTest : public QObject
{
    Q_OBJECT

public:
    ModelTest() : mDimension(xDimension, yDimension)
    {
    }

    static const DimensionType xDimension;
    static const DimensionType yDimension;
    static const PlayerCoordinates coordinateSetup;
    static const PlayerCoordinates gameOverCoordinateSetup;
    static const DimensionQ dimensionSetup;
    static const HuntingBoardData setupData;


private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

    void testGetSetDimensions();
    void testGetInitialDimensions();

    void testInitialize();
    void testSave();

    void testStartNewGame();
    void testPlayerPositionOnNewGame();

    void testPauseGame();
    void testContinueGame();

    void testMovePlayer();
    void testNextToMove();

    void testIsGameOverScenario();

private:
    std::unique_ptr<HuntingBoard> mBoard;
    std::unique_ptr<QSignalSpy> mGameOverSignalSpy;
    std::unique_ptr<QSignalSpy> mBoardChangedSignalSpy;
    std::unique_ptr<QSignalSpy> mStepsTakenChangedSignalSpy;
    std::unique_ptr<QSignalSpy> mDimensionChangedSignalSpy;
    std::unique_ptr<QSignalSpy> mNewGameStartedSignalSpy;
    Serializer mSerializer;
    DimensionQ mDimension;
};

const DimensionType ModelTest::xDimension = 5;
const DimensionType ModelTest::yDimension = 5;
const PlayerCoordinates ModelTest::coordinateSetup = {
    {{1,2}, HUNTER},
    {{3, 4}, HUNTER},
    {{5, 6}, PREY},
    {{5, 7}, HUNTER} };
const PlayerCoordinates ModelTest::gameOverCoordinateSetup =
{
    {{1,2}, HUNTER},
    {{3, 4}, HUNTER},
    {{5, 6}, PREY},
    {{5, 7}, HUNTER},
    {{5, 6}, HUNTER}
};
const DimensionQ ModelTest::dimensionSetup = {7, 7};
const HuntingBoardData ModelTest::setupData(dimensionSetup, 4*7, 2,
                                            HUNTER, coordinateSetup);

void ModelTest::initTestCase() {}
void ModelTest::cleanupTestCase() {}

void ModelTest::init()
{
    qRegisterMetaType<hunting::PlayerCoordinates>();
    qRegisterMetaType<hunting::PlayerCoordinatesPtr>();
    qRegisterMetaType<hunting::PlayerIdPair>();
    qRegisterMetaType<hunting::PlayerType>();

    mSerializer = Serializer();
    mBoard = std::make_unique<HuntingBoard>(mSerializer, mDimension);
    mGameOverSignalSpy = std::make_unique<QSignalSpy>(mBoard.get(),
                                                      SIGNAL(gameOverSignal(quint64, PlayerType)));
    mBoardChangedSignalSpy = std::make_unique<QSignalSpy>(mBoard.get(),
                                                      SIGNAL(boardChangedSignal(PlayerCoordinatesPtr)));
    mStepsTakenChangedSignalSpy = std::make_unique<QSignalSpy>(mBoard.get(),
                                                      SIGNAL(stepsTakenChangedSignal(quint64)));
    mDimensionChangedSignalSpy = std::make_unique<QSignalSpy>(mBoard.get(),
                                                      SIGNAL(dimensionChangedSignal(DimensionQ)));
    mNewGameStartedSignalSpy = std::make_unique<QSignalSpy>(mBoard.get(),
                                                      SIGNAL(newGameSignal(PlayerCoordinatesPtr)));
}

void ModelTest::testGetInitialDimensions()
{
    QCOMPARE(mBoard->getDimensions(), DimensionQ(xDimension, yDimension));
}
void ModelTest::testGetSetDimensions()
{
    DimensionQ newDimension(6, 6);
    mBoard->setDimensions(newDimension);

    QCOMPARE(mDimensionChangedSignalSpy->count(), 1);
    QCOMPARE(mBoard->getDimensions(), newDimension);
}

void ModelTest::testInitialize()
{
    mBoard->initialize(setupData);

    QCOMPARE(mBoard->getDimensions(), setupData.dimension);
}

void ModelTest::testSave()
{
    mBoard->initialize(setupData);

    QCOMPARE(setupData, *mBoard->save());
}
void ModelTest::testStartNewGame()
{
    mBoard->startNewGame();

    QCOMPARE(mNewGameStartedSignalSpy->count(), 1);
    PlayerCoordinatesPtr playersOnBoard = qvariant_cast<PlayerCoordinatesPtr>(mNewGameStartedSignalSpy->at(0).at(0));

    QCOMPARE(playersOnBoard->size(), 5);
}

void ModelTest::testPlayerPositionOnNewGame()
{
    mBoard->startNewGame();
    PlayerCoordinatesPtr playersOnBoard = qvariant_cast<PlayerCoordinatesPtr>(mNewGameStartedSignalSpy->at(0).at(0));

    PlayerCoordinates hunters;
    QVector<DimensionQ> hunterPositions;
    PlayerIdPair prey = *std::find_if(playersOnBoard->begin(), playersOnBoard->end(),
                                      [](const PlayerIdPair& playerIdPair) { return playerIdPair.second == PREY; });
    std::copy_if(playersOnBoard->begin(), playersOnBoard->end(), std::back_inserter(hunters),
                 [](const PlayerIdPair& playerIdPair) { return playerIdPair.second == HUNTER; });
    std::transform(hunters.begin(), hunters.end(), std::back_inserter(hunterPositions),
                   [](const PlayerIdPair& player) { return player.first; });

    QVector<DimensionQ> expectedHunterPositions = { {1, 5}, {1, 1}, {5, 1}, {5, 5}};
    DimensionQ expectedPreyPosition = {3, 3};

    QCOMPARE(prey.first, expectedPreyPosition);
    QVERIFY(std::is_permutation(hunterPositions.begin(), hunterPositions.end(), expectedHunterPositions.begin()));
}

void ModelTest::testMovePlayer()
{
    mBoard->initialize(setupData);
    DimensionQ aHunterDimension = {3, 4};
    DimensionQ moveTo = {3, 5};
    mBoard->movePlayer(aHunterDimension, moveTo);

    QCOMPARE(mBoardChangedSignalSpy->count(), 1);
    QCOMPARE(mNewGameStartedSignalSpy->count(), 1);
    PlayerCoordinatesPtr change = qvariant_cast<PlayerCoordinatesPtr>(mBoardChangedSignalSpy->at(0).at(0));
    QCOMPARE(change->size(), 1);
}
void ModelTest::testPauseGame()
{
    mBoard->pauseGame();
    DimensionQ from = {2, 3};
    DimensionQ to = {3, 3,};
    mBoard->movePlayer(from, to);

    QVERIFY(mBoardChangedSignalSpy->empty());
}
void ModelTest::testContinueGame()
{
    mBoard->startNewGame();
    mBoard->pauseGame();
    mBoard->continueGame();
    DimensionQ from = {1, 5};
    DimensionQ to = {2, 5};
    mBoard->movePlayer(from, to);

    QCOMPARE(mBoardChangedSignalSpy->count(), 1);
}
void ModelTest::testNextToMove()
{
    QCOMPARE(mBoard->getNextMove(HUNTER), PREY);
    QCOMPARE(mBoard->getNextMove(PREY), HUNTER);
}

void ModelTest::testIsGameOverScenario()
{
    HuntingBoardData gameOverData(dimensionSetup, 4*dimensionSetup.first,
                                  2, PREY, gameOverCoordinateSetup);
    mBoard->initialize(gameOverData);
    QVERIFY(mBoard->isGameOverScenario());
}

QTEST_APPLESS_MAIN(ModelTest);

#include "ModelTest.moc"
