#include "trayicon.h"

TrayIcon::TrayIcon(QObject *parent):
    QSystemTrayIcon(parent)
{
    connect(&timer, &QTimer::timeout, this, &TrayIcon::on_timer);
    connect(this, &QSystemTrayIcon::activated, this,
                    &TrayIcon::on_activated);
    connect(this, &QSystemTrayIcon::messageClicked, this,
            &TrayIcon::on_message_clicked);
    setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));

    createMenu();
    setToolTip(tr("Bubble Message"));
    timer.setInterval(TIME_CONSTANTS::MINUTE);
    // Very coarse timers only keep full second accuracy
    timer.setTimerType(Qt::VeryCoarseTimer);
    timer.start();
}

TrayIcon::~TrayIcon()
{
    if(menu) delete menu;
}

void TrayIcon::createMenu()
{
    menu = new QMenu();

    QAction *exitAction = new QAction(tr("Exit"), menu);
    exitAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
    connect(exitAction, &QAction::triggered, this, &TrayIcon::on_exit);

    menu->addAction(exitAction);
    setContextMenu(menu);
}

void TrayIcon::on_exit(bool checked)
{
    Q_UNUSED(checked)
    QMessageBox::StandardButton reply = QMessageBox::question(nullptr,
                                                              tr("?"),
                                                              tr("Do you want to quite?"),
                                                              QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        if(timer.isActive()) {
            timer.stop();
        }
        hide();
        QApplication::instance()->exit();
    }
}

void TrayIcon::on_timer()
{
    if(supportsMessages()) {
        showMessage(tr("Message"), tr("Message"));
    }
}

void TrayIcon::on_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::ActivationReason::Unknown:
        //Unknown reason
        break;
    case QSystemTrayIcon::ActivationReason::Context:
        // The context menu for the system tray entry was requested
        break;
    case QSystemTrayIcon::ActivationReason::DoubleClick:
        // The system tray entry was double clicked
        break;
    case QSystemTrayIcon::ActivationReason::Trigger:
        // The system tray entry was clicked
        break;
    case QSystemTrayIcon::ActivationReason::MiddleClick:
        // The system tray entry was clicked with the middle mouse button
        QPoint point = QCursor::pos() - QPoint(0, 25);
        menu->popup(point);
        break;
    }
}

void TrayIcon::on_message_clicked()
{
    QMessageBox::information(nullptr,
                             tr("Systray"),
                             tr("Message has been clicked.")
                             );
}

void TrayIcon::setMainTimerInterval(int msec)
{
    if(msec < 0 || msec == timer.interval())
        return;
    if(timer.isActive()) {
        timer.stop();
        timer.setInterval(msec);
        timer.start();
    } else {
        timer.setInterval(msec);
    }
}

void TrayIcon::startMainTimer()
{
    timer.start();
}

void TrayIcon::stopMainTimer()
{
    if(timer.isActive()) {
        timer.stop();
    }
}
