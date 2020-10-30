#include "HuntingMainWindow.h"
#include "ui_HuntingMainWindow.h"
#include <QBrush>
#include <QDrag>
#include <QCommonStyle>

#include <BoardGraphicUtility.h>

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
    QCommonStyle commonStyle;
    ui->up->setIcon(commonStyle.standardIcon(QStyle::SP_ArrowUp));
    ui->down->setIcon(commonStyle.standardIcon(QStyle::SP_ArrowDown));
    ui->left->setIcon(commonStyle.standardIcon(QStyle::SP_ArrowBack));
    ui->right->setIcon(commonStyle.standardIcon(QStyle::SP_ArrowForward));
    ui->up->setDisabled(true);
    ui->down->setDisabled(true);
    ui->left->setDisabled(true);
    ui->right->setDisabled(true);

    this->setWindowTitle("Vadászat");
    this->setMouseTracking(true);
    this->setAcceptDrops(true);

    QObject::connect(&mBoard, &HuntingBoard::boardChangedSignal, this, &HuntingMainWindow::onBoardChange);
    QObject::connect(&mBoard, &HuntingBoard::stepsTakenChangedSignal, this, &HuntingMainWindow::onStepsChanges);
    QObject::connect(&mBoard, &HuntingBoard::gameOverSignal, this, &HuntingMainWindow::onGameOver);
    QObject::connect(&mBoard, &HuntingBoard::dimensionChangedSignal, this, &HuntingMainWindow::onDimensionChanged);

    QObject::connect(ui->saveButton, &QAction::triggered, this, &HuntingMainWindow::onGameSaveRequested);
    QObject::connect(ui->loadButton, &QAction::triggered, this, &HuntingMainWindow::onGameLoadRequested);

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
    if(event->button() == Qt::LeftButton)
    {
        auto rectIt = std::find_if(mBoardCells.cbegin(), mBoardCells.cend(),
                                [event] (const QRect& rect) { return rect.contains(event->pos()); });
        if(rectIt != mBoardCells.end() && !isSelectionActive)
        {
            from = *rectIt;
            isSelectionActive = true;
            ui->up->setEnabled(true);
            ui->down->setEnabled(true);
            ui->left->setEnabled(true);
            ui->right->setEnabled(true);
        }
        else if(rectIt != mBoardCells.end() && isSelectionActive)
        {
            isSelectionActive = false;
            ui->up->setEnabled(false);
            ui->down->setEnabled(false);
            ui->left->setEnabled(false);
            ui->right->setEnabled(false);
            graphicStep(from, *rectIt);
        }
    }
}

void HuntingMainWindow::mouseMoveEvent(QMouseEvent* event)
{
}

void HuntingMainWindow::mouseReleaseEvent(QMouseEvent* event)
{
}

void HuntingMainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Up ||
            event->key() == Qt::Key_W)
    {
        graphicStep(from, BoardGraphicUtility::shift(from, Direction::UP, from.height()));
    }
    else if(event->key() == Qt::Key_Down ||
            event->key() == Qt::Key_S)
    {
        graphicStep(from, BoardGraphicUtility::shift(from, Direction::DOWN, from.height()));
    }
    else if(event->key() == Qt::Key_Left ||
            event->key() == Qt::Key_A)
    {
        graphicStep(from, BoardGraphicUtility::shift(from, Direction::LEFT, from.width()));
    }
    else if(event->key() == Qt::Key_Right ||
             event->key() == Qt::Key_D)
     {
         graphicStep(from, BoardGraphicUtility::shift(from, Direction::RIGHT, from.width()));
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
    if(stepCount % 2 == 0) { ui->stepping->setText("Vadász lép"); }
    else { ui->stepping->setText("Préda lép"); }

    ui->pointCounter->display(static_cast<int>(stepCount));

}
void HuntingMainWindow::onGameOver(quint64, hunting::PlayerType){}
void HuntingMainWindow::onDimensionChanged(DimensionQ d)
{
    mBoardDimension = d;
    fillBoardCells();
}

void HuntingMainWindow::onGameLoadRequested(bool)
{
    mFileChooserDialog.exec();
}

void HuntingMainWindow::onGameSaveRequested(bool)
{
}

void HuntingMainWindow::graphicStep(const QRect& from, const QRect& to)
{
    const QRect geometry = ui->playerArea->geometry();
    DimensionQ stepFrom = rectangeToCoordinate(geometry, from);
    DimensionQ stepTo = rectangeToCoordinate(geometry, to);
    mBoard.movePlayer(stepFrom, stepTo);
}
