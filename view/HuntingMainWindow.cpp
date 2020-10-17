#include "HungingMainWindow.h"
#include "ui_HuntingMainWindow.h"

HuntingMainWindow::HuntingMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HuntingMainWindow)
{
    ui->setupUi(this);
}

HuntingMainWindow::~HuntingMainWindow()
{
    delete ui;
}

