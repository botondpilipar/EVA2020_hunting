#include "HuntingMainWindow.h"
#include "ui_HuntingMainWindow.h"
#include <QBrush>

HuntingMainWindow::HuntingMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HuntingMainWindow)
    , mSerializer()
    , mBoardDimension(5, 5)
    , mBoard(mSerializer, mBoardDimension)
    , mBoardSeparator(Qt::darkBlue)
    , mPreyFiller(Qt::green)
    , mHunterFiller(Qt::red)
{
    ui->setupUi(this);

//    mFileChooserDialog.exec();

    this->setWindowTitle("Hunting");
    this->setMouseTracking(true);

    QObject::connect(&mBoard, &HuntingBoard::boardChangedSignal, this, &HuntingMainWindow::onBoardChange);
    QObject::connect(&mBoard, &HuntingBoard::stepsTakenChangedSignal, this, &HuntingMainWindow::onStepsChanges);
    QObject::connect(&mBoard, &HuntingBoard::gameOverSignal, this, &HuntingMainWindow::onGameOver);
    QObject::connect(&mBoard, &HuntingBoard::dimensionChangedSignal, this, &HuntingMainWindow::onDimensionChanged);


    mFileChooserDialog.setDisplayedDirectory(mSerializer.getWorkingDirectory());
    mBoardSeparator.setWidth(1);

    mBoard.startNewGame();
}

HuntingMainWindow::~HuntingMainWindow()
{
    delete ui;
}

void HuntingMainWindow::mousePressEvent(QMouseEvent* event)
{
    const QPoint at(event->globalX(), event->globalY());
    auto rectIt = std::find_if(mBoardCells.cbegin(), mBoardCells.cend(),
                               [at] (const QRect& rect) { return rect.contains(at); });
    if(rectIt != mBoardCells.end())
    {
        from = rectangeToCoordinate(ui->playerArea->geometry(), *rectIt);
    }
}
void HuntingMainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    const QPoint at(event->globalX(), event->globalY());
    auto rectIt = std::find_if(mBoardCells.cbegin(), mBoardCells.cend(),
                               [at] (const QRect& rect) { return rect.contains(at); });
    if(rectIt != mBoardCells.end())
    {
        DimensionQ to = rectangeToCoordinate(ui->playerArea->geometry(), *rectIt);
        mBoard.movePlayer(from, to);
    }
}

void HuntingMainWindow::fillBoardCells()
{
    mBoardCells.clear();
    const QRect& geometry = ui->playerArea->geometry();

    const int playerAreaBottom = geometry.bottom();
    const int playerAreaRight = geometry.right();
    const int playerAreaWidth = geometry.width();
    const int playerAreaHeight = geometry.height();

    const int horizontalStep = playerAreaWidth / mBoardDimension.first - 1;
    const int verticalStep = playerAreaHeight / mBoardDimension.second - 1;

    QVector<QRect> boardRectangles;
    for(int v = geometry.top(); v<playerAreaBottom - verticalStep; v+= verticalStep)
    {
        for(int i = geometry.left(); i<playerAreaRight - horizontalStep; i+= horizontalStep)
        {
            QRect boardCell(QPoint(i, v), QPoint(i + horizontalStep, v + verticalStep));
            mBoardCells.push_back(boardCell);
        }
    }

    update();
}


void HuntingMainWindow::paintEvent(QPaintEvent* event)
{
    QPainter boardPainter(this);
    boardPainter.setPen(mBoardSeparator);

    boardPainter.drawRects(mBoardCells);

    // hunters
    boardPainter.setBrush(mHunterFiller);
    for(const auto& rect : mHunters) { boardPainter.drawEllipse(rect); }

    // prey
    boardPainter.setBrush(mPreyFiller);
    boardPainter.drawEllipse(mPrey);
}

QRect HuntingMainWindow::coordinateToRectangle(const QRect& area, const DimensionQ coordinate) const
{
    const int horizontalStep = area.width() / mBoardDimension.first;
    const int verticalStep = area.height() / mBoardDimension.second;
    QPoint topLeft(area.topLeft() + QPoint((coordinate.first - 1) * horizontalStep,
                                           ((coordinate.second - 1) * verticalStep)));
    QPoint rightBottom = topLeft + QPoint(horizontalStep, verticalStep);

    return QRect(topLeft, rightBottom);
}

DimensionQ HuntingMainWindow::rectangeToCoordinate(const QRect& area, const QRect& rect) const
{
    const int verticalStep = area.height() / mBoardDimension.second;
    const int horizontalStep = area.width() / mBoardDimension.first;

    QPoint leftTopDiff = rect.topLeft() - area.topLeft();
    QPoint rightBottomDiff = area.bottomRight() - rect.bottomRight();

    return { leftTopDiff.x() / horizontalStep, rightBottomDiff.y() / verticalStep };
}

void HuntingMainWindow::onBoardChange(PlayerCoordinatesPtr change)
{
    mHunters.clear();

    for(const PlayerIdPair& p : *change)
    {
        switch(p.second)
        {
        case HUNTER:
        {
            mHunters.push_back(coordinateToRectangle(ui->playerArea->geometry(), p.first));
        }
            break;
        case PREY:
        {
            mPrey = coordinateToRectangle(ui->playerArea->geometry(), p.first);
        }
        }
    }
}
void HuntingMainWindow::onStepsChanges(quint64 stepCount)
{
    ui->pointCounter->display(static_cast<int>(stepCount));
}
void HuntingMainWindow::onGameOver(quint64, hunting::PlayerType){}
void HuntingMainWindow::onDimensionChanged(DimensionQ d)
{
    mBoardDimension = d;
    fillBoardCells();
}


