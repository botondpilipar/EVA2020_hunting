#include "HuntingMainWindow.h"
#include "ui_HuntingMainWindow.h"
#include <QBrush>
#include <QDrag>
#include <QCommonStyle>
#include <QMessageBox>

#include <BoardGraphicUtility.h>

HuntingMainWindow::HuntingMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HuntingMainWindow)
    , mSerializer()
    , mBoardDimension(5, 5)
    , mBoard(mSerializer, mBoardDimension)
    , mBoardCells(mBoardDimension.first, QVector<QRect>(mBoardDimension.second, QRect()))
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

    QObject::connect(ui->up, &QPushButton::clicked, this, &HuntingMainWindow::onUpButtonPushed);
    QObject::connect(ui->down, &QPushButton::clicked, this, &HuntingMainWindow::onDownButtonPushed);
    QObject::connect(ui->left, &QPushButton::clicked, this, &HuntingMainWindow::onLeftButtonPushed);
    QObject::connect(ui->right, &QPushButton::clicked, this, &HuntingMainWindow::onRightButtonPushed);

    QObject::connect(ui->saveButton, &QAction::triggered, this, &HuntingMainWindow::onGameSaveRequested);
    QObject::connect(ui->loadButton, &QAction::triggered, this, &HuntingMainWindow::onGameLoadRequested);

    QObject::connect(ui->newGameButton, &QPushButton::clicked, this, [this] { mBoard.startNewGame(); });
    QObject::connect(ui->dimension3Button, &QPushButton::clicked, this, [this] { mBoard.setDimensions({3, 3}); });
    QObject::connect(ui->dimension5Button, &QPushButton::clicked, this, [this] { mBoard.setDimensions({5, 5}); });
    QObject::connect(ui->dimension7Button, &QPushButton::clicked, this, [this] { mBoard.setDimensions({7, 7}); });

    QObject::connect(&mBoard, &HuntingBoard::boardChangedSignal, this, &HuntingMainWindow::onBoardChange);
    QObject::connect(&mBoard, &HuntingBoard::stepsTakenChangedSignal, this, &HuntingMainWindow::onStepsChanges);
    QObject::connect(&mBoard, &HuntingBoard::gameOverSignal, this, &HuntingMainWindow::onGameOver);
    QObject::connect(&mBoard, &HuntingBoard::dimensionChangedSignal, this, &HuntingMainWindow::onDimensionChanged);
    QObject::connect(&mBoard, &HuntingBoard::newGameSignal, this, &HuntingMainWindow::onNewGame);



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
        quint16 rowIndex;
        quint16 columnIndex;
        auto rowIt = std::find_if(mBoardCells.cbegin(), mBoardCells.cend(),
            [&] (const auto& row) {
            auto containingRect = std::find_if(row.cbegin(), row.cend(),
                                [&] (const auto& rect) { return rect.contains(event->pos()); });
            columnIndex = std::distance(row.begin(), containingRect);
            return containingRect != row.end(); });
        rowIndex = std::distance(mBoardCells.cbegin(), rowIt);

        if(rowIt != mBoardCells.end() && !isSelectionActive)
        {
            from = {columnIndex, rowIndex};
            setPlayerSelected(true);
        }
        else if(rowIt != mBoardCells.end() && isSelectionActive)
        {
            DimensionQ to = {columnIndex, rowIndex};
            setPlayerSelected(false);
            mBoard.movePlayer(BoardGraphicUtility::toLogicalCoordinate(from), BoardGraphicUtility::toLogicalCoordinate(to));
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
    DimensionQ to;
    if(event->key() == Qt::Key_Up ||
            event->key() == Qt::Key_W)
    {
        to = BoardGraphicUtility::shiftDimension(from, Direction::UP, 1);
    }
    else if(event->key() == Qt::Key_Down ||
            event->key() == Qt::Key_S)
    {
        to = BoardGraphicUtility::shiftDimension(from, Direction::DOWN, 1);
    }
    else if(event->key() == Qt::Key_Left ||
            event->key() == Qt::Key_A)
    {
        to = BoardGraphicUtility::shiftDimension(from, Direction::LEFT, 1);
    }
    else if(event->key() == Qt::Key_Right ||
             event->key() == Qt::Key_D)
     {
        to = BoardGraphicUtility::shiftDimension(from, Direction::RIGHT, 1);
     }
    mBoard.movePlayer(BoardGraphicUtility::toLogicalCoordinate(from), BoardGraphicUtility::toLogicalCoordinate(to));
    setPlayerSelected(false);
}
void HuntingMainWindow::fillBoardCells()
{
    mBoardCells.clear();
    mBoardCells.resize(mBoard.getDimensions().first);
    const int width = mBoard.getDimensions().first;
    const int height = mBoard.getDimensions().second;

    const QRect& geometry = ui->playerArea->geometry();

    const int playerAreaBottom = geometry.bottom();
    const int playerAreaRight = geometry.right();
    const int playerAreaWidth = geometry.width();
    const int playerAreaHeight = geometry.height();

    const int horizontalStep = playerAreaWidth / mBoardDimension.first - 1;
    const int verticalStep = playerAreaHeight / mBoardDimension.second - 1;

    QVector<QRect> boardRectangles;
    int i = 0;
    int j = 0;
    for(int v = geometry.top(); i<width && v<playerAreaBottom - verticalStep; v+= verticalStep)
    {
        mBoardCells.push_back(QVector<QRect>(mBoard.getDimensions().second));
        for(int u = geometry.left(); j < height && u<playerAreaRight - horizontalStep; u+= horizontalStep)
        {
            QRect boardCell(QPoint(u, v), QPoint(u + horizontalStep, v + verticalStep));
            mBoardCells[i].push_back(boardCell);
            int j = 0;
        }
        i += 1;
    }

    update();
}


void HuntingMainWindow::paintEvent(QPaintEvent* event)
{
    QPainter boardPainter(this);
    boardPainter.setPen(mBoardSeparator);

    for(const auto& row : mBoardCells) { boardPainter.drawRects(row); }

    // prey
    boardPainter.setBrush(mPreyFiller);
    boardPainter.drawEllipse(mBoardCells.at(mPrey.second).at(mPrey.first));

    // hunters
    boardPainter.setBrush(mHunterFiller);
    for(const auto& dim : mHunters) { boardPainter.drawEllipse(mBoardCells.at(dim.second).at(dim.first)); }
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

    QPoint leftTopDiff = (rect.topLeft() - area.topLeft()) + QPoint(1, 1);

    return {
        (leftTopDiff.x() / horizontalStep) + 1,
        (leftTopDiff.y() / verticalStep) + 1
    };
}

void HuntingMainWindow::onNewGame(PlayerCoordinatesPtr change)
{
    fillBoardCells();
    mHunters.clear();
    if(mBoard.getCurrentlyMoving() == PlayerType::HUNTER) { ui->stepping->setText("Vadász lép"); }
    else { ui->stepping->setText("Préda lép"); }

    for(const PlayerIdPair& p : *change)
    {
        switch(p.second)
        {
            case HUNTER:
            {
                mHunters.push_back(BoardGraphicUtility::fromLogicalCoordinate(p.first));
            }
                break;
            case PREY:
            {
                mPrey = BoardGraphicUtility::fromLogicalCoordinate(p.first);
            }
        }
    }

    update();
}
void HuntingMainWindow::onBoardChange(PlayerCoordinatesPtr change)
{

    if(mBoard.getCurrentlyMoving() == PlayerType::HUNTER) { ui->stepping->setText("Vadász lép"); }
    else { ui->stepping->setText("Préda lép"); }

    for(const PlayerIdPair& p : *change)
    {
        switch(p.second)
        {
            case HUNTER:
            {
                auto toChange = std::find(mHunters.begin(), mHunters.end(), from);
                int pos = std::distance(mHunters.begin(), toChange);
                mHunters[pos] = BoardGraphicUtility::fromLogicalCoordinate(p.first);
            }
                break;
            case PREY:
            {
                mPrey = BoardGraphicUtility::fromLogicalCoordinate(p.first);
            }
            break;
        }
    }
    update();
}
void HuntingMainWindow::onStepsChanges(quint64 stepCount)
{
    ui->pointCounter->display(static_cast<int>(stepCount));
}
void HuntingMainWindow::onGameOver(quint64 steps, hunting::PlayerType side)
{
    using namespace hunting;
    QString winningSide = side == PlayerType::HUNTER ? "Vadász" : "Préda";
    QMessageBox message(QMessageBox::Information, "Játék vége, ", QString::number(steps) + " lépésből nyert: " + winningSide,
                QMessageBox::Ok);

    message.exec();
    mBoard.startNewGame();
}
void HuntingMainWindow::onDimensionChanged(DimensionQ d)
{
    mBoardDimension = d;
}

void HuntingMainWindow::onGameLoadRequested(bool)
{
    mFileChooserDialog.exec();
}

void HuntingMainWindow::onGameSaveRequested(bool)
{
}

void HuntingMainWindow::onUpButtonPushed()
{
    DimensionQ to = BoardGraphicUtility::shiftDimension(from, Direction::UP, 1);
    mBoard.movePlayer(BoardGraphicUtility::toLogicalCoordinate(from), BoardGraphicUtility::toLogicalCoordinate(to));
    setPlayerSelected(false);
}
void HuntingMainWindow::onDownButtonPushed()
{
    DimensionQ to = BoardGraphicUtility::shiftDimension(from, Direction::DOWN, 1);
    mBoard.movePlayer(BoardGraphicUtility::toLogicalCoordinate(from), BoardGraphicUtility::toLogicalCoordinate(to));
    setPlayerSelected(false);
}
void HuntingMainWindow::onLeftButtonPushed()
{
    DimensionQ to = BoardGraphicUtility::shiftDimension(from, Direction::LEFT, 1);
    mBoard.movePlayer(BoardGraphicUtility::toLogicalCoordinate(from), BoardGraphicUtility::toLogicalCoordinate(to));
    setPlayerSelected(false);
}
void HuntingMainWindow::onRightButtonPushed()
{
    DimensionQ to = BoardGraphicUtility::shiftDimension(from, Direction::RIGHT, 1);
    mBoard.movePlayer(BoardGraphicUtility::toLogicalCoordinate(from), BoardGraphicUtility::toLogicalCoordinate(to));
    setPlayerSelected(false);
}
void HuntingMainWindow::graphicStep(const QRect& from, const QRect& to)
{
    const QRect geometry = ui->playerArea->geometry();
    DimensionQ stepFrom = rectangeToCoordinate(geometry, from);
    DimensionQ stepTo = rectangeToCoordinate(geometry, to);
    mBoard.movePlayer(stepFrom, stepTo);
}

void HuntingMainWindow::setPlayerSelected(bool select)
{
    isSelectionActive = select;
    ui->up->setEnabled(select);
    ui->down->setEnabled(select);
    ui->left->setEnabled(select);
    ui->right->setEnabled(select);
}
