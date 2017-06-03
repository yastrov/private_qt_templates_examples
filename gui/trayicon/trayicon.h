#ifndef TRAYICON_H
#define TRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>
#include <QApplication>
#include <QMessageBox>
#include <QCursor>
#include <QPoint>
#include <QStyle>

namespace TIME_CONSTANTS {
    const int MSEC_TO_SEC_MULT = 1000;
    const int MINUTE = 60*MSEC_TO_SEC_MULT;
    const int HALF_OF_HOUR = 30*MINUTE;
    const int HOUR = 60*MINUTE;
}

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit TrayIcon(QObject *parent=nullptr);
    virtual ~TrayIcon();
    void setMainTimerInterval(int msec);
    void startMainTimer();
    void stopMainTimer();

public slots:
    void on_exit(bool checked);

private slots:
    void on_timer();
    void on_activated(QSystemTrayIcon::ActivationReason reason);
    void on_message_clicked();

private:
    void createMenu();
    QMenu *menu;
    QTimer timer;

};

#endif // TRAYICON_H
