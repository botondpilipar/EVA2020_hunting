#ifndef HUNTINGMAINWINDOW_H
#define HUNTINGMAINWINDOW_H

#include <QMainWindow>
#include <FileChooserDialog.h>
#include <HuntingBoard.h>
#include <Serializer.h>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QStateMachine>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class HuntingMainWindow; }
QT_END_NAMESPACE

using namespace kd417d::eva;
class HuntingMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    HuntingMainWindow(QWidget *parent = nullptr);
    ~HuntingMainWindow();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent*) override;
    virtual void keyPressEvent(QKeyEvent* event) override;

    virtual void fillBoardCells();
    virtual QRect coordinateToRectangle(const QRect& area, const DimensionQ coordinate) const;
    virtual DimensionQ rectangeToCoordinate(const QRect& area, const QRect& rect) const;

protected slots:
    // Board slots
    void onBoardChange(PlayerCoordinatesPtr);
    void onStepsChanges(quint64);
    void onGameOver(quint64, hunting::PlayerType);
    void onDimensionChanged(DimensionQ);

    // Ui slots
    void onGameLoadRequested(bool);
    void onGameSaveRequested(bool);
private:
    Ui::HuntingMainWindow *ui;
    FileChooserDialog mFileChooserDialog;
    Serializer mSerializer;
    DimensionQ mBoardDimension;
    HuntingBoard mBoard;

    QPen mBoardSeparator;
    QBrush mPreyFiller;
    QBrush mHunterFiller;

    QVector<QRect> mBoardCells;
    QVector<QRect> mHunters;
    QRect mPrey;

    // Utility
    QRect from;
    bool isSelectionActive;
    void graphicStep(const QRect& from, const QRect& to);
};
#endif // HUNTINGMAINWINDOW_H
