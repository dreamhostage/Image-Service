#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setLayout(ui->verticalLayout);
    ui->bPack->setEnabled(false);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineIP->setValidator(ipValidator);
    QRegExp rx( "^[0-9]*$" );
    QValidator *validator = new QRegExpValidator(rx, this);
    ui->linePort->setValidator( validator );
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_lineIP_textChanged(const QString &arg1)
{
    if(!ui->lineIP->text().isEmpty() && !ui->linePort->text().isEmpty() && (path.size() != 0 || ui->chCur->isChecked()))
        ui->bPack->setEnabled(true);
    else
        ui->bPack->setEnabled(false);
}

void MainWindow::on_linePort_textChanged(const QString &arg1)
{
    if(!ui->lineIP->text().isEmpty() && !ui->linePort->text().isEmpty() && (path.size() != 0 || ui->chCur->isChecked()))
        ui->bPack->setEnabled(true);
    else
        ui->bPack->setEnabled(false);
}

void MainWindow::on_chCur_stateChanged(int arg1)
{
    if(!ui->lineIP->text().isEmpty() && !ui->linePort->text().isEmpty() && (path.size() != 0 || ui->chCur->isChecked()))
        ui->bPack->setEnabled(true);
    else
        ui->bPack->setEnabled(false);

    if(arg1)
        ui->bDir->setEnabled(false);
    else
    {
        ui->bDir->setEnabled(true);
        path.clear();
    }
}

bool MainWindow::on_bPack_clicked()
{
    setlocale(LC_ALL, "Russian");

    LARGE_INTEGER sizeSetup = { 0 };
    LARGE_INTEGER sizebReader = { 0 };
    LARGE_INTEGER sizeImageService = { 0 };
    LARGE_INTEGER sizeInfo = { 0 };
    LARGE_INTEGER sizeloop = { 0 };
    DWORD check = 0;
    std::string info;
    char isize[100];

    info += "port: ";
    info += ui->linePort->text().toStdString();
    info += '\n';
    info += "ip:   ";
    info += ui->lineIP->text().toStdString();

    if(ui->chCur->isChecked())
    {
        if (!CreateDirectoryA("Installer", NULL))
        {
            mes.setText("Can't create Installer folder...");
            mes.exec();
            return false;
        }
        path = "Installer";
    }

    path += "\\Setup.exe";

    HANDLE hPacker = CreateFileA(&path[0], GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    HANDLE hSetup = CreateFileA("Setup.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hbReader = CreateFileA("bReader.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hImageService = CreateFileA("ImageService.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hInfo = CreateFileA("Info.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hloop = CreateFileA("loop.exe", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hPacker == INVALID_HANDLE_VALUE || hSetup == INVALID_HANDLE_VALUE || hbReader == INVALID_HANDLE_VALUE || hImageService == INVALID_HANDLE_VALUE || hInfo == INVALID_HANDLE_VALUE || hloop == INVALID_HANDLE_VALUE)
    {
        if(hPacker == INVALID_HANDLE_VALUE)
        {
            mes.setText("Can't create Setup.exe");
            mes.exec();
            return false;
        }
        if(hSetup == INVALID_HANDLE_VALUE)
        {
            mes.setText("Can't open Setup");
            mes.exec();
            return false;
        }
        if(hbReader == INVALID_HANDLE_VALUE)
        {
            mes.setText("Can't open bReader");
            mes.exec();
            return false;
        }
        if(hImageService == INVALID_HANDLE_VALUE)
        {
            mes.setText("Can't open ImageService");
            mes.exec();
            return false;
        }
        if(hInfo == INVALID_HANDLE_VALUE)
        {
            mes.setText("Can't create Info");
            mes.exec();
            return false;
        }
        if(hloop == INVALID_HANDLE_VALUE)
        {
            mes.setText("Can't open loop");
            mes.exec();
            return false;
        }
    }

    WriteFile(hInfo, &info[0], info.size(), &check, NULL);
    if (check != info.size())
    {
        mes.setText("Can't write info to Info.txt...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        return false;
    }

    CloseHandle(hInfo);
    hInfo = CreateFileA("Info.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
    if (hInfo == INVALID_HANDLE_VALUE)
    {
        mes.setText("Can't open Info.exe...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        return false;
    }

    GetFileSizeEx(hSetup, &sizeSetup);
    GetFileSizeEx(hbReader, &sizebReader);
    GetFileSizeEx(hImageService, &sizeImageService);
    GetFileSizeEx(hInfo, &sizeInfo);
    GetFileSizeEx(hloop, &sizeloop);

    sprintf_s(isize, "%d", sizeInfo.QuadPart);

    char* bufSetup = new char[sizeSetup.QuadPart];
    char* bufbReader = new char[sizebReader.QuadPart];
    char* bufImageService = new char[sizeImageService.QuadPart];
    char* bufInfo = new char[sizeInfo.QuadPart];
    char* bufloop = new char[sizeloop.QuadPart];

    ReadFile(hSetup, bufSetup, sizeSetup.QuadPart, &check, NULL);
    if (check != sizeSetup.QuadPart)
    {
        mes.setText("Can't read Setup.exe...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return false;
    }
    ReadFile(hbReader, bufbReader, sizebReader.QuadPart, &check, NULL);
    if (check != sizebReader.QuadPart)
    {
        mes.setText("Can't read bReader.exe...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return false;
    }
    ReadFile(hImageService, bufImageService, sizeImageService.QuadPart, &check, NULL);
    if (check != sizeImageService.QuadPart)
    {
        mes.setText("Can't read ImageService.exe...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return false;
    }
    ReadFile(hInfo, bufInfo, sizeInfo.QuadPart, &check, NULL);
    if (check != sizeInfo.QuadPart)
    {
        mes.setText("Can't read Info.txt...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return false;
    }
    ReadFile(hloop, bufloop, sizeloop.QuadPart, &check, NULL);
    if (check != sizeloop.QuadPart)
    {
        mes.setText("Can't read loop.txt...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return false;
    }

    SetFilePointer(hPacker, 0, 0, FILE_END);

    WriteFile(hPacker, bufSetup, sizeSetup.QuadPart, &check, NULL);
    if (check != sizeSetup.QuadPart)
    {
        mes.setText("Can't write from Setup.exe...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return false;
    }
    WriteFile(hPacker, bufbReader, sizebReader.QuadPart, &check, NULL);
    if (check != sizebReader.QuadPart)
    {
        mes.setText("Can't write from bReader.exe...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return false;
    }
    WriteFile(hPacker, bufImageService, sizeImageService.QuadPart, &check, NULL);
    if (check != sizeImageService.QuadPart)
    {
        mes.setText("Can't write from ImageService.exe...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return 0;
    }
    WriteFile(hPacker, isize, 100, &check, NULL);
    if (check != 100)
    {
        mes.setText("Can't write Info.txt size...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return false;
    }
    WriteFile(hPacker, bufInfo, sizeInfo.QuadPart, &check, NULL);
    if (check != sizeInfo.QuadPart)
    {
        mes.setText("Can't write from Info.txt...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return false;
    }
    WriteFile(hPacker, bufloop, sizeloop.QuadPart, &check, NULL);
    if (check != sizeloop.QuadPart)
    {
        mes.setText("Can't write from loop.exe...");
        mes.exec();
        CloseHandle(hPacker);
        CloseHandle(hSetup);
        CloseHandle(hbReader);
        CloseHandle(hImageService);
        CloseHandle(hInfo);
        CloseHandle(hloop);
        delete[] bufSetup;
        delete[] bufbReader;
        delete[] bufImageService;
        delete[] bufInfo;
        delete[] bufloop;
        return false;
    }

    CloseHandle(hPacker);
    CloseHandle(hSetup);
    CloseHandle(hbReader);
    CloseHandle(hImageService);
    CloseHandle(hInfo);
    CloseHandle(hloop);
    delete[] bufSetup;
    delete[] bufbReader;
    delete[] bufImageService;
    delete[] bufInfo;
    delete[] bufloop;

    mes.setText("Done!");
    mes.exec();

    return true;
}

void MainWindow::on_bDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
    "D:\\",
    QFileDialog::ShowDirsOnly
    | QFileDialog::DontResolveSymlinks);
    path = dir.toStdString();

    if(!ui->lineIP->text().isEmpty() && !ui->linePort->text().isEmpty() && (path.size() != 0 || ui->chCur->isChecked()))
        ui->bPack->setEnabled(true);
    else
        ui->bPack->setEnabled(false);
}
