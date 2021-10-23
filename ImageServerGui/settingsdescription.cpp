#include "settingsdescription.h"
#include "ui_settingsdescription.h"

SettingsDescription::SettingsDescription(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsDescription)
{
    ui->setupUi(this);
    this->setWindowTitle("Help");
    SettingsDescription::setLayout(ui->verticalLayout);
    this->ui->listWidget->addItem("Main menu");
    this->ui->listWidget->addItem("File menu");
    this->ui->listWidget->addItem("Text Viewer");
    this->ui->listWidget->addItem("Settings menu");

    this->ui->listWidget->findItems("Main menu", Qt::MatchFlag::MatchExactly).first()->setTextAlignment(20);
    this->ui->listWidget->findItems("File menu", Qt::MatchFlag::MatchExactly).first()->setTextAlignment(20);
    this->ui->listWidget->findItems("Text Viewer", Qt::MatchFlag::MatchExactly).first()->setTextAlignment(20);
    this->ui->listWidget->findItems("Settings menu", Qt::MatchFlag::MatchExactly).first()->setTextAlignment(20);
}

SettingsDescription::~SettingsDescription()
{
    delete ui;
}

void SettingsDescription::on_pushButton_clicked()
{
    this->close();
}

void SettingsDescription::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if (item->text() == "Main menu")
    {
        ui->textBrowser->clear();
        ui->textBrowser->append(QLatin1String("<img src=\":/mainMenuEN.png\" />"));
    }
    else if (item->text() == "File menu")
    {
        ui->textBrowser->clear();
        ui->textBrowser->append(QLatin1String("<img src=\":/FileMenuEN.png\" />"));
    }
    else if (item->text() == "Text Viewer")
    {
        ui->textBrowser->clear();
        ui->textBrowser->append(QLatin1String("<img src=\":/TextViewer.png\" />"));
    }
    else if (item->text() == "Settings menu")
    {
        ui->textBrowser->clear();
        ui->textBrowser->append(QLatin1String("<img src=\":/SettingsMenu.png\" />"));
    }
}
