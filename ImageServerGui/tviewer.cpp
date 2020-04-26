#include "tviewer.h"
#include "ui_tviewer.h"
#include <Windows.h>
#include <QMessageBox>

tviewer::tviewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tviewer)
{
    ui->setupUi(this);
    this->setWindowTitle("Pressed buttons monitoring");
    setLayout(ui->verticalLayout);
}

tviewer::~tviewer()
{
    delete ui;
}

void tviewer::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current)
    {
        ui->textBrowser->clear();
        ui->checkBoxRU->setChecked(false);
        char wBuff[500];
        SHGetSpecialFolderPathA(0, wBuff, CSIDL_PERSONAL, true);
        QString way;
        way += QString::fromLocal8Bit(wBuff);
        way += "\\imageLog\\";
        way += current->text();
        way += "\\Buttons.txt";
        HANDLE hFile = CreateFileA(way.toLocal8Bit(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if(hFile != INVALID_HANDLE_VALUE)
        {
            LARGE_INTEGER size = { 0 };
            QString data;
            GetFileSizeEx(hFile, &size);
            char *buf = new char[size.QuadPart];
            ReadFile(hFile, buf, size.QuadPart, NULL, NULL);
            data = buf;
            ui->textBrowser->append(data);
            CloseHandle(hFile);
            if(buf)
                delete[] buf;
        }
    }
}

void tviewer::on_checkBoxRU_toggled(bool checked)
{
    if(checked)
    {
        QString data = ui->textBrowser->toPlainText();
        QString ndata;
        ui->textBrowser->clear();

        for(int i = 0; i < data.size(); ++i)
        {
            if(data[i] == 'q')
                ndata += "й";
            else if(data[i] == "w")
                ndata += "ц";
            else if(data[i] == 'e')
                ndata += "у";
            else if(data[i] == "r")
                ndata += "к";
            else if(data[i] == 't')
                ndata += "е";
            else if(data[i] == "y")
                ndata += "н";
            else if(data[i] == 'u')
                ndata += "г";
            else if(data[i] == "i")
                ndata += "ш";
            else if(data[i] == 'o')
                ndata += "щ";
            else if(data[i] == "p")
                ndata += "з";
            else if(data[i] == '[')
                ndata += "х";
            else if(data[i] == "]")
                ndata += "ъ";
            else if(data[i] == 'a')
                ndata += "ф";
            else if(data[i] == "s")
                ndata += "ы";
            else if(data[i] == 'd')
                ndata += "в";
            else if(data[i] == "f")
                ndata += "а";
            else if(data[i] == 'g')
                ndata += "п";
            else if(data[i] == "h")
                ndata += "р";
            else if(data[i] == 'j')
                ndata += "о";
            else if(data[i] == "k")
                ndata += "л";
            else if(data[i] == 'l')
                ndata += "д";
            else if(data[i] == ";")
                ndata += "ж";
            else if(data[i] == "'")
                ndata += "э";
            else if(data[i] == "z")
                ndata += "я";
            else if(data[i] == "x")
                ndata += "ч";
            else if(data[i] == "c")
                ndata += "с";
            else if(data[i] == 'v')
                ndata += "м";
            else if(data[i] == "b")
                ndata += "и";
            else if(data[i] == 'n')
                ndata += "т";
            else if(data[i] == "m")
                ndata += "ь";
            else if(data[i] == ',')
                ndata += "б";
            else if(data[i] == ".")
                ndata += "ю";
            else
                ndata += data[i];
        }
        ui->textBrowser->append(ndata);
    }
    else
    {
        ui->textBrowser->clear();
        char wBuff[500];
        SHGetSpecialFolderPathA(0, wBuff, CSIDL_PERSONAL, true);
        QString way;
        way += wBuff;
        way += "\\imageLog\\";
        if(ui->listWidget->currentItem())
        {
            way += ui->listWidget->currentItem()->text();
            way += "\\Buttons.txt";
            HANDLE hFile = CreateFileA(way.toLocal8Bit(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if(hFile != INVALID_HANDLE_VALUE)
            {
                PLARGE_INTEGER size;
                QString data;
                GetFileSizeEx(hFile, size);
                char *buf = new char[size->QuadPart];
                ReadFile(hFile, buf, size->QuadPart, NULL, NULL);
                data = buf;
                ui->textBrowser->append(data);
                CloseHandle(hFile);
                delete[] buf;
            }
        }
    }
}

void tviewer::on_pushButton_clicked()
{
    QString data = ui->textBrowser->toPlainText();
    ui->textBrowser->clear();

    data.remove("\nESC\n");
    data.remove("\nF1\n");
    data.remove("\nF2\n");
    data.remove("\nF3\n");
    data.remove("\nF4\n");
    data.remove("\nF5\n");
    data.remove("\nF6\n");
    data.remove("\nF7\n");
    data.remove("\nF8\n");
    data.remove("\nF9\n");
    data.remove("\nF10\n");
    data.remove("\nF11\n");
    data.remove("\nF12\n");
    data.remove("\nDEL\n");
    data.remove("\nBackspace\n");
    data.remove("\nTab\n");
    data.remove("\nCapsLock\n");
    data.remove("\nlShift\n");
    data.remove("\nrShift\n");
    data.remove("\nlCtrl\n");
    data.remove("\nWin\n");
    data.remove("\nrCtrl\n");
    data.remove("\nLeft\n");
    data.remove("\nRight\n");
    data.remove("\nUp\n");
    data.remove("\nDown\n");
    data.remove("\nEnter");
    ui->textBrowser->append(data);
}



void tviewer::on_pushButton_Update_clicked()
{
    if( ui->listWidget->currentItem())
    {
        ui->textBrowser->clear();
        ui->checkBoxRU->setChecked(false);
        char wBuff[500];
        SHGetSpecialFolderPathA(0, wBuff, CSIDL_PERSONAL, true);
        QString way;
        way += wBuff;
        way += "\\imageLog\\";
        way += ui->listWidget->currentItem()->text();
        way += "\\Buttons.txt";

        HANDLE hFile = CreateFileA(way.toLocal8Bit(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if(hFile != INVALID_HANDLE_VALUE)
        {
            LARGE_INTEGER size = { 0 };
            QString data;
            GetFileSizeEx(hFile, &size);
            char *buf = new char[size.QuadPart];
            ReadFile(hFile, buf, size.QuadPart, NULL, NULL);
            data = buf;
            ui->textBrowser->append(data);
            CloseHandle(hFile);
            delete[] buf;
        }
    }
}
