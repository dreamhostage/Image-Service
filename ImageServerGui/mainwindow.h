#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settings.h"
#include "ui_settings.h"
#include "ImageServerLib.h"
#include "tviewer.h"
#include "ui_tviewer.h"

#include <QMediaPlayer>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>
#include <Windows.h>

extern HANDLE hFile;

namespace Ui {
class MainWindow;
}

struct information
{
    ImageServer *server;
    Ui::MainWindow *ui;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    information info;
    tviewer tv;
    ImageServer *server;
    HANDLE hnames_monytoring;
    HANDLE honline_monytoring;
    HANDLE hShooting_interval;
    HANDLE htap_monytoring;
    WSADATA WsaData;
    SOCKET soc;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    settings *s = new settings(this);
    QMediaPlayer m;

private slots:

    void on_pushButton_3_clicked();
    void on_actionDelete_all_images_triggered();
    void on_actionSettings_triggered();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_pushButton1_clicked();
    void on_pushButton_2_clicked();
    void on_actionText_viewer_triggered();
    void on_Launchvideostreaming_clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
