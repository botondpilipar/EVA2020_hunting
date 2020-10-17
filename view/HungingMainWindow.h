#ifndef HUNTINGMAINWINDOW_H
#define HUNTINGMAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class HuntingMainWindow; }
QT_END_NAMESPACE

class HuntingMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    HuntingMainWindow(QWidget *parent = nullptr);
    ~HuntingMainWindow();

private:
    Ui::HuntingMainWindow *ui;
};
#endif // HUNTINGMAINWINDOW_H
