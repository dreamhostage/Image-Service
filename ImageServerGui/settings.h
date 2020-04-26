#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class settings;
}

class settings : public QDialog
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    QString getPort();
    ~settings();
    unsigned long interval;
    Ui::settings *ui;

private slots:
    void on_pushButton_clicked();

};

#endif // SETTINGS_H
