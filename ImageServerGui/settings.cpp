#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings");
    settings::setLayout(ui->verticalLayout);
    ui->comboBox->addItem("s");
    ui->comboBox->addItem("m");
    ui->comboBox->addItem("h");
    ui->checkSound->setChecked(true);
}

settings::~settings()
{
    delete ui;
}

QString settings::getPort()
{
    return ui->portLine->text();
}

void settings::on_pushButton_clicked()
{
    interval = ui->intervalLine->text().toULong();
    if(ui->comboBox->findText("s") == ui->comboBox->currentIndex())
        interval = interval * 1000;
    else if(ui->comboBox->findText("m") == ui->comboBox->currentIndex())
        interval = interval * 60 * 1000;
    else if(ui->comboBox->findText("h") == ui->comboBox->currentIndex())
        interval = interval * 60 * 60 * 1000;
    settings::close();
}
