#ifndef TVIEWER_H
#define TVIEWER_H

#include <QDialog>
#include <QListWidgetItem>
#include <ShlObj_core.h>

namespace Ui {
class tviewer;
}

class tviewer : public QDialog
{
    Q_OBJECT

public:
    explicit tviewer(QWidget *parent = nullptr);
    ~tviewer();
    Ui::tviewer *ui;

private slots:
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_checkBoxRU_toggled(bool checked);
    void on_pushButton_clicked();
    void on_pushButton_Update_clicked();
};

#endif // TVIEWER_H
