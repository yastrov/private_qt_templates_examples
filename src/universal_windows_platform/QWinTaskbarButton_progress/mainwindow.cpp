#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // setWindowTitle(qApp->applicationName());
    // Win Extras
    #if defined(USE_WIN_EXTRAS) && defined(Q_OS_WIN)
    buttonWinExtra = new QWinTaskbarButton(this);
//    buttonWinExtra->setOverlayIcon(QIcon(":/loading.png"));
    buttonWinExtra->setOverlayIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
    progressWinExtra = buttonWinExtra->progress();
    progressWinExtra->setVisible(false);
    #endif
    testShow();
}

void MainWindow::showEvent(QShowEvent *e)
{
    #if defined(USE_WIN_EXTRAS) && defined(Q_OS_WIN)
    buttonWinExtra->setWindow(windowHandle());
    #endif
    e->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWinExtraProgressValueAndVisible()
{
    #if defined(USE_WIN_EXTRAS) && defined(Q_OS_WIN)
    if(progressWinExtra) {
        progressWinExtra->setMinimum(0);
        progressWinExtra->setVisible(true);
    }
    #endif
}

void MainWindow::hideWinExtraProgress()
{
    #if defined(USE_WIN_EXTRAS) && defined(Q_OS_WIN)
    if(progressWinExtra)
        progressWinExtra->setVisible(false);
    #endif
}

void MainWindow::maximumValue(int value)
{
    #if defined(USE_WIN_EXTRAS) && defined(Q_OS_WIN)
    progressWinExtra->setMaximum(value);
    #endif
}

void MainWindow::currentValue(int value)
{
    #if defined(USE_WIN_EXTRAS) && defined(Q_OS_WIN)
    progressWinExtra->setValue(value);
    #endif
}

void MainWindow::testShow()
{
    initWinExtraProgressValueAndVisible();
    maximumValue(100);
    currentValue(50);
}
