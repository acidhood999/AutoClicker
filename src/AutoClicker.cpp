#include "include/AutoClicker.h"
#include <QStyle>
#include <QCloseEvent>
#include <QIcon>

AutoClicker::AutoClicker(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    settingWindow();

    ui.mainToolBar->setVisible(false);
    ui.statusBar->setVisible(false);

    fun = new Functional(this);
    setCentralWidget(fun);

    setupTrayIcon();

    if (QCoreApplication::arguments().contains("--tray")) this->hide();
    else this->show();
}

void AutoClicker::settingWindow()
{
    setWindowTitle("Auto Clicker");
    setFixedSize(605, 400);
    setWindowIcon(QIcon(":/AutoClicker/mouse.ico"));

}

void AutoClicker::setupTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/AutoClicker/mouse.ico"));

    trayMenu = new QMenu(this);
    QAction* restoreAction = trayMenu->addAction("Open");
    QAction* quitAction = trayMenu->addAction("Exit");

    connect(restoreAction, &QAction::triggered, this, [this]() 
    {
        this->showNormal();
        this->raise();
        this->activateWindow();
        if (fun) fun->restoreMarkersFromTray();
    });

    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason)
    {
        if (reason == QSystemTrayIcon::Trigger) 
        {
            if (this->isVisible())
            {
                this->hide();
                if (fun) fun->hideMarkersForTray();
            }
            else
            {
                this->showNormal();
                this->raise();
                this->activateWindow();
                if (fun) fun->restoreMarkersFromTray();
            }
        }
    });
}

void AutoClicker::closeEvent(QCloseEvent* event)
{
    
    if (trayIcon->isVisible()) {
        this->hide();
        if (fun) fun->hideMarkersForTray();
        trayIcon->showMessage(
            "AutoClicker",
            "The application is minimized to the tray. Click the icon to open it.",
            QSystemTrayIcon::Information,
            2000
        );

        event->ignore();
    }
    else event->accept();
}