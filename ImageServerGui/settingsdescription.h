#ifndef SETTINGSDESCRIPTION_H
#define SETTINGSDESCRIPTION_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class SettingsDescription;
}

class SettingsDescription : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsDescription(QWidget *parent = nullptr);
    ~SettingsDescription();

private slots:
    void on_pushButton_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::SettingsDescription *ui;
};

#endif // SETTINGSDESCRIPTION_H
