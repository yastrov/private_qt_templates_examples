#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShowEvent>
#if defined(USE_WIN_EXTRAS) && defined(Q_OS_WIN)
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
#if defined(USE_WIN_EXTRAS) && defined(Q_OS_WIN)
    QWinTaskbarButton *buttonWinExtra;
    QWinTaskbarProgress *progressWinExtra;
#endif
    void initWinExtraProgressValueAndVisible();
    void hideWinExtraProgress();
    void maximumValue(int value);
    void currentValue(int value);
    void testShow();

protected:
    void showEvent(QShowEvent *e) Q_DECL_OVERRIDE;
//    void changeEvent(QEvent *e) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
